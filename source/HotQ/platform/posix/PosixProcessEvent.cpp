#include "PosixProcessEvent.h"
#include "PosixSemaphore.h"

HotQ::PosixProcessEvent::PosixProcessEvent(const char * name)
{
    //initialize the semaphore with a value of 0
   mSem = new HotQ::PosixSemaphore(name, 0); 
}

HotQ::PosixProcessEvent::~PosixProcessEvent()
{
    if (mSem)
    {
        delete mSem;
        mSem = 0;
    }
}

bool HotQ::PosixProcessEvent::Wait(int timeout)
{
    return mSem->Decrement(timeout);
}

void HotQ::PosixProcessEvent::Signal()
{
    mSem->Increment();
}

void HotQ::PosixProcessEvent::Reset()
{
    mSem->Reset(0); 
}

HotQ::HqProcessEvent * HotQ::CreateHotQProcessEvent(const char * name)
{ 
    return new HotQ::PosixProcessEvent(name);
}
