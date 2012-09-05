#ifndef __HOT_Q_FACTORY__
#define __HOT_Q_FACTORY__

#include "HqDefines.h"
namespace HotQ
{
    class HqRunnable;
    class HqRunnable;
    class HqThread;
    class HqThreadFactory
    {
    public:
        HqThreadFactory(){}
        virtual ~HqThreadFactory(){}
        virtual HqError CreateThread(HqRunnable * runnable, HqThread ** outThreadPtr) const = 0;
        virtual void DestroyThread(HqThread ** threadPtr) const = 0;

    };

    HqThreadFactory * CreateDefaultHqThreadFactory();
}

#endif
