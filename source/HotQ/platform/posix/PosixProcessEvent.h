#ifndef __POSIX_PROC_EVENT__
#define __POSIX_PROC_EVENT__

#include "HqProcessEvent.h"

namespace HotQ
{
    class PosixSemaphore;
    class PosixProcessEvent : public HqProcessEvent
    {
    public:
        PosixProcessEvent(const char * name);
        virtual ~PosixProcessEvent();
        virtual bool Wait(int timeout) ;
        virtual void Signal();
        virtual void Reset();
    private:
       PosixSemaphore * mSem; 

    };
}

#endif
