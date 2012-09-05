#ifndef __POSIX_THREAD__
#define __POSIX_THREAD__

#include "HqThread.h"
#include "HqRunnable.h"
#include <pthread.h>

namespace HotQ
{
    class PosixThread : public HqThread
    {
    public:
        explicit PosixThread(HqRunnable * runnableObject);
        virtual ~PosixThread();
        virtual void * Join();
        virtual HqError Start();
        virtual void ForceKill();
    private:
        pthread_t mHandle;
        HqRunnable * mRunnableObject;
    };
}

#endif
