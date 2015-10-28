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
