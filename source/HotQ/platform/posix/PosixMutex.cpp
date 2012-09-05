#include "PosixMutex.h"
#include "PosixSemaphore.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

HotQ::PosixMutex::PosixMutex(const char * mutexName)
: mSem(0)
{
    mSem = new PosixSemaphore(mutexName,1);
}

HotQ::PosixMutex::~PosixMutex()
{
    if (mSem)
    {
        delete mSem;
        mSem = 0;
    }
}

bool HotQ::PosixMutex::EnterCriticalArea(int timeout)
{
    return mSem->Decrement(timeout);
}

void HotQ::PosixMutex::LeaveCriticalArea()
{
    mSem->Increment();
}

void HotQ::PosixMutex::Reset()
{
    mSem->Reset(1);
}

HotQ::HqMutex * HotQ::CreateHotQMutex (const char * name)
{
    return new HotQ::PosixMutex(name);
}
