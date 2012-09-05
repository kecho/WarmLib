#include "WarmWorkerThreadSafeStateBoard.h"
#include "HotQ/HqMutex.h"

WarmLib::WarmWorkerThreadSafeStateBoard::WarmWorkerThreadSafeStateBoard()
{
    mMutex = HotQ::CreateHotQThreadMutex();
}

WarmLib::WarmWorkerThreadSafeStateBoard::~WarmWorkerThreadSafeStateBoard()
{
    delete mMutex;
}

void WarmLib::WarmWorkerThreadSafeStateBoard::SetState(int workerId, WarmLib::WorkerState state)
{
    mMutex->EnterCriticalArea();
    mWorkerStateMap[workerId] = state;
    mMutex->LeaveCriticalArea();
}

WarmLib::WorkerState WarmLib::WarmWorkerThreadSafeStateBoard::GetState(int workerId)
{
    WarmLib::WorkerState s = WarmLib::WS_UNKNOWN;
    
    mMutex->EnterCriticalArea();
    WarmLib::WarmWorkerThreadSafeStateBoard::WorkerStateMap::iterator it = mWorkerStateMap.find(workerId);
    if (it != mWorkerStateMap.end())
    {
        s = it->second;
    }
    mMutex->LeaveCriticalArea();

    return s;
}
