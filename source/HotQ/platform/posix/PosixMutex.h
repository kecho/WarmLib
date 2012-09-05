#ifndef __HOT_Q_MUTEX_POSIX__
#define __HOT_Q_MUTEX_POSIX__

#include "HqMutex.h"

#define MUTEX_NAME_BUFFER_SZ 60
#define MUTEX_TIMEOUT 20

namespace HotQ
{
    class PosixSemaphore;
    class PosixMutex : public HqMutex
    {
    public:
        PosixMutex (const char * mutexName);
        virtual ~PosixMutex ();
        virtual bool EnterCriticalArea(int timeout);
        virtual void LeaveCriticalArea();
        virtual void Reset();
    private:
        void InitializeMutex();
        PosixSemaphore * mSem;
    };
}

#endif
