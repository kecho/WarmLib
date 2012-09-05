#include "WarmWorkerManager.h"
#include "WarmWorkerMessage.h"
#include "HotQ/HotQ.h"
#include "HotQ/HqThread.h"
#include "HotQ/HqDefines.h"
#include "HotQ/HqMessage.h"
#include "HotQ/HqProcess.h"
#include "HotQ/HqThreadFactory.h"

#include <stdio.h>

WarmLib::WarmWorkerManager::WarmWorkerManager(
    const char * coldPluginName, 
    int workerId, 
    int responseQueueId, 
    WarmLib::WarmWorkerEventListener * eventListener,
    int maxMessageSize,
    int responseSize,
    const HotQ::HqThreadFactory * threadFactory
 )
:
mPluginName(coldPluginName), 
mWatchDogThread(0),
mWorkerProcess(0),
mWorkerId(workerId),
mListener(eventListener),
mResponseSize(responseSize),
mMaxMessageSize(maxMessageSize),
mResponseQueueId(responseQueueId)
{
   threadFactory->CreateThread(this, &mWatchDogThread); 
   mWorkerProcess = HotQ::CreateHotQProcess(WARM_LIB_WORKER_BIN);
   mMessageQueue = HotQ::CreateHotQ(mWorkerId, sizeof(WarmLib::WarmWorkerMessage) + mMaxMessageSize);
   //this initializes the queue, purges any messages in it)
   mMessageQueue->Reset();
   mPluginName = coldPluginName;    
}

WarmLib::WarmWorkerManager::~WarmWorkerManager()
{
    mWatchDogThread->Join();
    if (mWatchDogThread)
    {
        delete mWatchDogThread;
    }

    if (mWorkerProcess)
    {
        delete mWorkerProcess;
    }

    if (mMessageQueue)
    {
        delete mMessageQueue;
    }
}

void * WarmLib::WarmWorkerManager::Run()
{
    int errorCode = 0;
    mWorkerProcess->Join(&errorCode);
    WarmLib::Error error = static_cast<WarmLib::Error>(errorCode);
    error = (error < 0 || error > WarmLib::ERR_UNKNOWN) ? WarmLib::ERR_UNKNOWN : error;
    if (error == WarmLib::ERR_OK)
    {
        mListener->OnShutdown(mWorkerId);
    }
    else
    {
        mListener->OnError(mWorkerId, error);
    }
    return 0;
}

bool WarmLib::WarmWorkerManager::StartWorker()
{
    HotQ::HqError error = HotQ::ERR_OK;     
    const char * argv[5];
    char pluginName[60];
    char workerId[60];
    char responseQId[60];
    char maxMessageSize[60];
    char responseSize[60];
    sprintf(pluginName,"%s",mPluginName.c_str());
    sprintf(workerId,"%d",mWorkerId);
    sprintf(responseQId,"%d",mResponseQueueId);
    sprintf(maxMessageSize,"%d",mMaxMessageSize);
    sprintf(responseSize, "%d", mResponseSize);
    argv[0] = pluginName;
    argv[1] = workerId;
    argv[2] = responseQId;
    argv[3] = maxMessageSize;
    argv[4] = responseSize;
    error = mWorkerProcess->Start(5, argv);
    //notify the listener we have started
    if (error == HotQ::ERR_OK)
    {
        mListener->OnSignalStart(mWorkerId);
        mWatchDogThread->Start();
        return true;
    }
    else
    {
        mListener->OnError(mWorkerId, WarmLib::ERR_PROCESS_START);
    }

    return false;
}

void WarmLib::WarmWorkerManager::SendMessageToWorker(const WarmLib::WarmWorkerMessage& message)
{
    mMessageQueue->Enqueue(message);
}
