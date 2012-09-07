#include "WarmWorker.h"
#include "WarmWorkerMessage.h"
#include "HotQ/HqThreadFactory.h"
#include "HotQ/HqThread.h"
#include "HotQ/HotQ.h"
#include <stdio.h>

WarmLib::WarmWorker::WarmWorker(const char * pluginName, int workerId, int outgoingConnectionId, int jobMessageSize, int responseSize, const HotQ::HqThreadFactory * factory)
: 
WarmWorkerMessageReceiver(workerId, factory, /*do no reset the queue*/false, jobMessageSize),
mResponseBufferSize(responseSize),
mPlugin(0) 
{
    mPluginError = ColdSdk::ImportColdPlugin(pluginName, &mPlugin);
    mOutgoingQueue = HotQ::CreateHotQ(outgoingConnectionId, sizeof(WarmLib::WarmWorkerMessage) + responseSize);
    mResponseBuffer = new char[responseSize];
}

WarmLib::WarmWorker::~WarmWorker()
{
    if (mOutgoingQueue)
    {
        delete mOutgoingQueue;
    }

    if (mPlugin)
    {
        delete mPlugin;
    }
    
    if (mResponseBuffer)
    {
        delete [] mResponseBuffer;
    }
}

bool WarmLib::WarmWorker::OnMessage(const WarmLib::WarmWorkerMessage& message)
{
    bool success = true;
    switch (message.GetType())
    {
    
    case WarmLib::MESSAGE_PING:
    {
        SendMessage(WarmLib::MESSAGE_PING_RESPONSE);
    }
        break;
    case WarmLib::MESSAGE_JOB_START:
    {
        SendMessage(WarmLib::MESSAGE_JOB_STARTED);
        int reply = mPlugin->PluginMain(sizeof(int), reinterpret_cast<const char*>(message.GetJobBuffer()), mResponseBufferSize, mResponseBuffer);
        SendMessage(WarmLib::MESSAGE_JOB_FINISHED);
    }

        break;
    case WarmLib::MESSAGE_NONE:
    case WarmLib::MESSAGE_POISON:
    default:
        success = false;
    }
    return success;
}

void WarmLib::WarmWorker::SendMessage(WarmLib::WarmMessageType type)
{
    WarmLib::WarmWorkerMessage message(type, GetId());
    if (type == WarmLib::MESSAGE_JOB_FINISHED)
    {
        message.SetJobBuffer(mResponseBuffer, mResponseBufferSize);
    }
    mOutgoingQueue->Enqueue(message);
}

bool WarmLib::WarmWorker::Execute(bool blockExecution)
{
    if (mPluginError != ColdSdk::ERR_OK)
    {
        return false;
    }

    Listen();
    if (blockExecution)
    {
        JoinListening(); 
    }
    return true;
}

