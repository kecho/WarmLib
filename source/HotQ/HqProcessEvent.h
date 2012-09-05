#ifndef __HOT_Q_PROC_EVENT_
#define __HOT_Q_PROC_EVENT_

#include "HqDefines.h"

namespace HotQ
{
    class HqProcessEvent
    {
    public:
        HqProcessEvent(){}
        virtual ~HqProcessEvent(){}
        virtual bool Wait(int timeout = HOT_Q_INFINITE) = 0;
        virtual void Signal() = 0;
        virtual void Reset() = 0;
    };

    HqProcessEvent * CreateHotQProcessEvent(const char * eventName);
}
#endif
