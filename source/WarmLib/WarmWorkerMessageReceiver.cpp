#include "WarmWorkerMessageReceiver.h"
#include "WarmWorkerMessage.h"
#include "HotQ/HotQ.h"
#include "HotQ/HqThread.h"
#include "HotQ/HqThreadFactory.h"
#include "HotQ/HqMessage.h"
#include <stdio.h>


WarmLib::WarmWorkerMessageReceiver::WarmWorkerMessageReceiver(int queueId, const HotQ::HqThreadFactory * factory, bool resetQueue, int messageSize)
:
mId(queueId),
mMaxMessageSize(messageSize),
mQueue(0)
{
    if (factory == 0)
    {
        HotQ::HqThreadFactory * defaultFactory = HotQ::CreateDefaultHqThreadFactory();
        defaultFactory->CreateThread(this, &mListenerThread);
        delete defaultFactory;
    }
    else
    {
        factory->CreateThread(this, &mListenerThread);
    }
    if (messageSize == -1)
    {
        mQueue = HotQ::CreateHotQ(mId);
    }
    else
    {
        mQueue = HotQ::CreateHotQ(mId, sizeof(WarmLib::WarmWorkerMessage) + mMaxMessageSize);
    }

    if (resetQueue)
    {
        mQueue->Reset();
    }
}

WarmLib::WarmWorkerMessageReceiver::~WarmWorkerMessageReceiver()
{
    if (mListenerThread)
    {
        delete mListenerThread;
    }
    
    if (mQueue)
    {
        delete mQueue;
    }
}


void WarmLib::WarmWorkerMessageReceiver::Listen()
{
    mListenerThread->Start();
}

void WarmLib::WarmWorkerMessageReceiver::JoinListening()
{
    mListenerThread->Join();
}

void * WarmLib::WarmWorkerMessageReceiver::Run()
{
    WarmLib::WarmWorkerMessage message;
    HotQ::HqError dequeueError = HotQ::ERR_OK;
    bool exit = false;
    do
    {
        dequeueError = mQueue->Dequeue(message);
        exit = dequeueError != HotQ::ERR_OK || !OnMessage(message);
        
    }while(!exit);
    return 0;
}
