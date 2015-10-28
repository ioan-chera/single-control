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

#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>
#include "logging.h"

static const char *levelString(int priority)
{
    switch(priority)
    {
        case LOG_EMERG:
            return "EMERG";
        case LOG_ALERT:
            return "ALERT";
        case LOG_CRIT:
            return "CRIT";
        case LOG_ERR:
            return "ERR";
        case LOG_WARNING:
            return "WARNING";
        case LOG_NOTICE:
            return "NOTICE";
        case LOG_INFO:
            return "INFO";
        case LOG_DEBUG:
            return "DEBUG";
    }
    return "";
}

void log(int priority, const char *message, ...)
{
    fprintf(stderr, "%s: ", levelString(priority));

    va_list args;
    va_start (args, message);
    vfprintf(stderr, message, args);
    va_end (args);

    fprintf(stderr, "\n");
}