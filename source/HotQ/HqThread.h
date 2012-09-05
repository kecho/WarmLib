#ifndef __HOT_Q_THREAD__
#define __HOT_Q_THREAD__

#include "HqDefines.h"

namespace HotQ
{
    class HqThread
    {
    public:
        HqThread(){}
        virtual ~HqThread(){}
        virtual HqError Start() = 0;
        virtual void * Join() = 0;
        virtual void ForceKill() = 0;

    };
}
#endif
