#ifndef __HOT_Q_MUTEX__
#define __HOT_Q_MUTEX__

#include "HqDefines.h"

namespace HotQ
{
    class HqMutex
    {
    public:
        HqMutex(){}
        virtual ~HqMutex(){}
        virtual bool EnterCriticalArea(int timeout = HOT_Q_INFINITE) = 0;
        virtual void LeaveCriticalArea() = 0;
        virtual void Reset() = 0;
    };

    HqMutex * CreateHotQMutex(const char * name);
    HqMutex * CreateHotQThreadMutex();
}

#endif
