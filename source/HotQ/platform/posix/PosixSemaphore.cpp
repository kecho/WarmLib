#include "PosixSemaphore.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>

HotQ::PosixSemaphore::PosixSemaphore(const char * name, int initialCount)
: mSem(0), mIsOwner(false) 
{
    sprintf(mSemName, "%s", name); 
    InitializeSemaphore(initialCount);
}

void HotQ::PosixSemaphore::InitializeSemaphore(int initVal)
{
    mIsOwner = true; //assume ownership
    mSem = sem_open(mSemName, O_CREAT | O_EXCL ,S_IRUSR | S_IWUSR , initVal);
    if (mSem == SEM_FAILED && errno == EEXIST)
    {
       mIsOwner = false;
       //this means we are not the owner, so we open the semaphore just using O_CREAT 
       //also means that this object will not unlink the semaphore
       mSem = sem_open(mSemName, O_CREAT,S_IRUSR | S_IWUSR , initVal);
    }
}

HotQ::PosixSemaphore::~PosixSemaphore()
{
    DestroySemaphore(false);
}

void HotQ::PosixSemaphore::DestroySemaphore(bool forceUnlink)
{
    sem_close(mSem);
    if (mIsOwner || forceUnlink)
    {
        sem_unlink(mSemName);
    }
}

void HotQ::PosixSemaphore::Reset(int newInitialCount)
{
    DestroySemaphore(true); 
    InitializeSemaphore(newInitialCount);
}

bool HotQ::PosixSemaphore::Decrement(int secondsTimeout)
{
    if (secondsTimeout < 0)
    {
        sem_wait(mSem);
        return true;
    }
    
    struct timeval tv;
    struct timespec timeout;
    gettimeofday(&tv, NULL);
    timeout.tv_sec = tv.tv_sec + secondsTimeout;  //wait 20 more seconds to unlock
    timeout.tv_nsec = 0;
    if (sem_timedwait(mSem, &timeout) != 0)
    {
        return false;
    }
    return true;
}

void HotQ::PosixSemaphore::Increment()
{
    sem_post(mSem);
}
