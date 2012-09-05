#ifndef __HOT_Q_SEM_POSIX__
#define __HOT_Q_SEM_POSIX__

#include <semaphore.h>

#define SEM_NAME_BUFFER_SZ 60

namespace HotQ
{
    class PosixSemaphore 
    {
    public:
        PosixSemaphore (const char * name, int initialCount);
        virtual ~PosixSemaphore ();
        virtual bool Decrement(int secondsTimeout=-1);
        virtual void Increment();
        virtual void Reset(int newInitialCount);
        
    private:
        void InitializeSemaphore(int initVal);
        void DestroySemaphore (bool forceUnlink);
        sem_t * mSem;
        char mSemName[SEM_NAME_BUFFER_SZ];
        bool mIsOwner;
    };
}

#endif
