#ifndef _WARMLIB_THREADSAFE_STATEBOARD_
#define _WARMLIB_THREADSAFE_STATEBOARD_
#include "WarmLibDefines.h"

namespace HotQ
{
    class HqMutex;
}

namespace WarmLib
{
    class WarmWorkerThreadSafeStateBoard
    {
    public:
        WarmWorkerThreadSafeStateBoard();
        ~WarmWorkerThreadSafeStateBoard();
        WorkerState GetState(int workerId);
        void SetState(int workerId, WorkerState workerState);

    private:
        typedef std::map<int, WorkerState> WorkerStateMap;
        WorkerStateMap mWorkerStateMap;
        HotQ::HqMutex* mMutex;
    };
}
#endif
