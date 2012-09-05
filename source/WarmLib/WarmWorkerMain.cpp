// Kleber Garcia (c)
/*
This is the main worker instance entry point. Its meant to be called programmatically
from a WarmWorkerPoolManager
*/
#include <iostream>
#include <stdio.h>
#include "WarmWorker.h"
#include "WarmLibDefines.h"

#define WORKER_NUM_ARGS 6 
#define PLUGIN_ARG argv[1]
#define INCOMING_CONNECTION_ID argv[2] 
#define OUTGOING_CONNECTION_ID argv[3]
#define JOB_MESSAGE_SIZE argv[4]
#define JOB_RESPONSE_SIZE argv[5]



int main(int argc, char ** argv)
{
    if (argc <  WORKER_NUM_ARGS)
    {
	printf("%s\n", "WARNING: this executable is not meant to be ran stand alone. Requires a master pool manager.");
        //printf("%s\n","WarmWorker usage");
        //printf("%s\n","Worker <coldplugin> <incoming queue id> <outgoing queue id> <jobMessageSize> <response size>");
	printf("%s\n", "exiting :)");
        return WarmLib::ERR_BAD_ARGS;
    }

    const char * pluginName = PLUGIN_ARG;
    int incomingConnectionId = 0;
    int outgoingConnectionId = 0;
    int jobMessageSize = 0;
    int responseSize = 0;
    sscanf(INCOMING_CONNECTION_ID, "%d", &incomingConnectionId);
    sscanf(OUTGOING_CONNECTION_ID, "%d", &outgoingConnectionId);
    sscanf(JOB_MESSAGE_SIZE, "%d", &jobMessageSize);
    sscanf(JOB_RESPONSE_SIZE, "%d", &responseSize);
    WarmLib::WarmWorker worker(pluginName, incomingConnectionId, outgoingConnectionId, jobMessageSize, responseSize);
    if (!worker.Execute())
    {
        return WarmLib::ERR_NO_PLUGIN;
    }
    else
    {
        return WarmLib::ERR_OK;
    }
}
