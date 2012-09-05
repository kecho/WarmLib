#ifndef __HOT_Q_THREAD_MUTEX__
#define __HOT_Q_THREAD_MUTEX__

#include "HqMutex.h"
#include <pthread.h>

namespace HotQ
{
    class PosixThreadMutex : public HqMutex
    {
    public:
        PosixThreadMutex();
        virtual ~PosixThreadMutex();
        virtual bool EnterCriticalArea(int timeout);
        virtual void LeaveCriticalArea();
        virtual void Reset();
    private:
        pthread_mutex_t mMutex; 

    };
}

#endif
