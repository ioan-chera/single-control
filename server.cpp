/*
 Single-Control
 Copyright (C) 2015  Ioan Chera

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <syslog.h>
#include "logging.h"
#include <unistd.h>
#include "defs.h"

// Send a broadcast that a server has been turned on.
static int broadcastServerHello()
{
    int sock;
    
    // help from http://stackoverflow.com/q/19771485
    
    // FIXME: also support IPv6
    log(LOG_INFO, "Broadcasting server hello");
    
    if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        log(LOG_ERR, "%s: socket error %d", __func__, errno);
        return -1;
    }
    
    int broadcast = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1)
    {
        log(LOG_ERR, "%s: setsockopt error %d", __func__, errno);
        close(sock);
        return -1;
    }
    
    sockaddr_in si = { 0 };
    si.sin_family = AF_INET;
    si.sin_port = htons(PORT);
    si.sin_addr.s_addr = INADDR_BROADCAST;    // broadcast
    
    // also send null char
    if(sendto(sock, SERVER_HELLO, sizeof(SERVER_HELLO), MSG_DONTWAIT, (sockaddr*)&si, sizeof(si)) == -1)
    {
        log(LOG_ERR, "%s: sendto error %d", __func__, errno);
        close(sock);
        return -1;
    }
    
    close(sock);
    
    return 0;
}

static int setupHelloListener()
{
    // http://stackoverflow.com/q/12515005
    int sock;
    if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        log(LOG_ERR, "%s: socket error %d", __func__, errno);
        return -1;
    }
    
    int broadcast = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1)
    {
        log(LOG_ERR, "%s: setsockopt error %d", __func__, errno);
        close(sock);
        return -1;
    }
    
    sockaddr_in si = { 0 };
    si.sin_family = AF_INET;
    si.sin_port = htons(PORT);
    si.sin_addr.s_addr = INADDR_ANY;
    
    sockaddr_in si_other;
    socklen_t si_other_len;
    
    if(bind(sock, (sockaddr*)&si, sizeof(sockaddr_in)) == -1)
    {
        log(LOG_ERR, "%s: bind error %d", __func__, errno);
        close(sock);
        return -1;
    }
    
    int errcount = 0;
    
    char buf[DATAGRAM_MAX_READ];
    char nice_address[INET_ADDRSTRLEN];
    
    for(;;)
    {
        si_other_len = sizeof(sockaddr);
        memset(buf, 0, sizeof(buf));
        if(recvfrom(sock, buf, sizeof(buf) - 1, 0, (sockaddr*)&si_other, &si_other_len) == -1)
        {
            log(LOG_WARNING, "%s: recvfrom error %d", __func__, errno);
            if(++errcount > MAX_ERRORS)
            {
                log(LOG_ALERT, "%s: too many errors, exiting", __func__);
                close(sock);
                return -1;
            }
            continue;
        }
        else
            errcount = 0;

        inet_ntop(AF_INET, &(si_other.sin_addr), nice_address, INET_ADDRSTRLEN);
        
        if(!strcmp(buf, SERVER_HELLO))
        {
            log(LOG_NOTICE, "Received hello from server %s", nice_address);
        }
        else
        {
            log(LOG_DEBUG, "Received unknown message %s from %s", buf, nice_address);
        }
    }
}

void runServer()
{
    log(LOG_INFO, "Starting server");
    if(broadcastServerHello() == -1)
    {
        log(LOG_WARNING, "%s: broadcastServerHello failed. Clients will not be notified of this server's start-up.", __func__);
    }
    
    if(setupHelloListener() == -1)
    {
        log(LOG_CRIT, "%s: setupHelloListener failed. This server will not be able to detect clients starting up!", __func__);
    }
}
