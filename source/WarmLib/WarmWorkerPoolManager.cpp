#include "WarmWorkerPoolManager.h"
#include "WarmWorkerMessage.h"
#include "HotQ/HotQ.h"
#include "HotQ/HqThread.h"
#include "HotQ/HqThreadFactory.h"
#include "HotQ/HqMutex.h"

#include <stdio.h>

class ThreadSafeListener : public WarmLib::WarmWorkerPoolEventListener
{
public:
    ThreadSafeListener(WarmLib::WarmWorkerPoolEventListener * listener)
    :
    mDecoration(listener)
    {
       mMutex = HotQ::CreateHotQThreadMutex(); 
    }
    virtual ~ThreadSafeListener()
    {
        delete mMutex;
    }
    virtual void OnJobStarted(int workerId);
    virtual void OnWorkerError(int workerId, WarmLib::Error error);
    virtual void OnWorkerJobFinished(int workerId, const void * jobResponse);
    WarmLib::WarmWorkerPoolEventListener * mDecoration;
    HotQ::HqMutex * mMutex;
};

void ThreadSafeListener::OnJobStarted(int workerId)
{
    mMutex->EnterCriticalArea();
    if (mDecoration)
    {
        mDecoration->OnJobStarted(workerId);
    }
    mMutex->LeaveCriticalArea();
}

void ThreadSafeListener::OnWorkerError(int workerId, WarmLib::Error error)
{
    mMutex->EnterCriticalArea();
    if (mDecoration)
    {
        mDecoration->OnWorkerError(workerId, error);
    }
    mMutex->LeaveCriticalArea();
}

void ThreadSafeListener::OnWorkerJobFinished(int workerId, const void * jobResponse)
{
    mMutex->EnterCriticalArea();
    if (mDecoration)
    {
        mDecoration->OnWorkerJobFinished(workerId, jobResponse);
    }
    mMutex->LeaveCriticalArea();
}

WarmLib::WarmWorkerPoolManager::WarmWorkerPoolManager(int numberOfWorkers, short poolId, const char * coldPluginName, int jobMessageSize, int jobResponseMessageSize, WarmLib::WarmWorkerPoolEventListener * listener, const HotQ::HqThreadFactory * threadFactory)
:
mPoolId(poolId),
mJobMessageSize(jobMessageSize),
mNumberOfWorkers(numberOfWorkers),
mNextWorkerIndex(0),
WarmWorkerMessageReceiver(GetResponseQueueId(numberOfWorkers, poolId), threadFactory, true, jobResponseMessageSize)
{
    sprintf(mPluginName, "%s", coldPluginName);    
    if (threadFactory == 0)
    {
        HotQ::HqThreadFactory * defaultThreadFactory = HotQ::CreateDefaultHqThreadFactory();
        InitializeWorkers(coldPluginName, defaultThreadFactory);
        delete defaultThreadFactory;
    }
    else
    {
        InitializeWorkers(coldPluginName, threadFactory);
    }

    mListener = new ThreadSafeListener(listener);

}

void WarmLib::WarmWorkerPoolManager::InitializeWorkers(const char* coldPluginName, const HotQ::HqThreadFactory * threadFactory)
{
    for (int i = 0; i < mNumberOfWorkers; ++i)
    {
        int workerId = GetWorkerId(i);
        mManagerMap[workerId] = new WarmLib::WarmWorkerManager(coldPluginName, workerId, GetResponseQueueId(), this, mJobMessageSize, GetMessageReceiverSize(), threadFactory);
    }
}

void WarmLib::WarmWorkerPoolManager::StartWorkers()
{
    WarmLib::WarmWorkerManagerMapIterator end = mManagerMap.end(); 
    for (WarmLib::WarmWorkerManagerMapIterator it = mManagerMap.begin(); it != end; ++it)
    {
        it->second->StartWorker();
    }
    Listen();
}

int WarmLib::WarmWorkerPoolManager::GetWorkerId(int index, int poolId) const
{
    return (static_cast<int>(poolId) << 16) + index;  
}

bool WarmLib::WarmWorkerPoolManager::OnMessage(const WarmLib::WarmWorkerMessage& message)
{
    switch (message.GetType())
    {
    case MESSAGE_JOB_STARTED:
        mStateBoard.SetState(message.GetWorkerId(), WS_PROCESSING);
        mListener->OnJobStarted(message.GetWorkerId());
        break;
    case MESSAGE_JOB_FINISHED:
        mStateBoard.SetState(message.GetWorkerId(), WS_IDLE);
        mListener->OnWorkerJobFinished(message.GetWorkerId(), message.GetJobBuffer());
        break;
    defualt:
        printf("WTF!\n");
    }
    return true;
}

WarmLib::WarmWorkerPoolManager::~WarmWorkerPoolManager()
{
    WarmLib::WarmWorkerManagerMapIterator end = mManagerMap.end(); 
    for (WarmLib::WarmWorkerManagerMapIterator it = mManagerMap.begin(); it != end; ++it)
    {
        delete it->second;
    }

    mManagerMap.clear();
}

WarmLib::WorkerState WarmLib::WarmWorkerPoolManager::GetWorkerState(int index)
{
    int workerId = GetWorkerId(index);
    return mStateBoard.GetState(workerId);
}


void WarmLib::WarmWorkerPoolManager::ShutdownAll()
{
    WarmLib::WarmWorkerManagerMapIterator it = mManagerMap.begin();   
    for (;it != mManagerMap.end(); ++it)
    {
        it->second->SendMessageToWorker(WarmLib::WarmWorkerMessage(WarmLib::MESSAGE_POISON));
    }
}

void WarmLib::WarmWorkerPoolManager::OnSignalStart(int workerId)
{
    mStateBoard.SetState(workerId, WarmLib::WS_IDLE);
}

void WarmLib::WarmWorkerPoolManager::OnShutdown(int workerId)
{
    mStateBoard.SetState(workerId, WarmLib::WS_SHUTDOWN);
}

void WarmLib::WarmWorkerPoolManager::OnError(int workerId, WarmLib::Error error)
{
    mStateBoard.SetState(workerId, WarmLib::WS_ERROR);
    mListener->OnWorkerError(workerId, error);
}

WarmLib::WarmWorkerManager * WarmLib::WarmWorkerPoolManager::AcquireWorker()
{
    int attempt = 0;
    while (attempt++ < mNumberOfWorkers)
    {
        int index = mNextWorkerIndex;
        mNextWorkerIndex = (mNextWorkerIndex + 1) % mNumberOfWorkers;
        if (GetWorkerState(index) == WarmLib::WS_IDLE)
        {
            int workerId = GetWorkerId(index);
            mStateBoard.SetState(workerId, WarmLib::WS_LOCKED);
            return mManagerMap[workerId];
        }
    }

    return 0;

}

WarmLib::Error WarmLib::WarmWorkerPoolManager::ScheduleJob(const void * arg)
{
    WarmLib::WarmWorkerManager * worker = AcquireWorker();
    if (worker)
    {
        WarmLib::WarmWorkerMessage message(MESSAGE_JOB_START);
        message.SetJobBuffer(arg, mJobMessageSize);
        worker->SendMessageToWorker(message);
        return ERR_OK;
    }
    else
    {
        return ERR_NO_WORKER_AVAILABLE; 
    }
}
