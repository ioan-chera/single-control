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

#include <stdio.h>
#include <string.h>
#include "server.h"

enum ExecType
{
    ExecClient,
    ExecServer
};

static ExecType g_execType;

static void printUsage()
{
    puts("Usage:\n"
        "single-control -type {client|server}");
}

static bool parseCommandLine(int argc, char **argv)
{
    for(int i = 1; i < argc; ++i)
    {
        if(!strcmp(argv[i], "-type"))
        {
            if(++i >= argc)
            return false;
            if(!strcmp(argv[i], "client"))
            {
                g_execType = ExecClient;
            }
            else if(!strcmp(argv[i], "server"))
            {
                g_execType = ExecServer;
            }
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    // Parse command line
    if(argc == 1)
    {
        printUsage();
        return 0;
    }

    if(!parseCommandLine(argc, argv))
    {
        fprintf(stderr, "Command error\n");
        printUsage();
        return -1;
    }

    if(g_execType == ExecServer)
    {
        runServer();
    }
    else if(g_execType == ExecClient)
    {
        // TODO: start client module
    }

    puts("Exitting");
    return 0;
}
