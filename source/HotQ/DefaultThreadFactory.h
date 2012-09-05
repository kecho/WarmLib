#ifndef __HOT_Q_THREAD_FACTORY__
#define __HOT_Q_THREAD_FACTORY__

#include "HqThreadFactory.h"

namespace HotQ
{
    class DefaultThreadFactory : public HqThreadFactory
    {
    public:
        DefaultThreadFactory(){}
        virtual ~DefaultThreadFactory(){}
        virtual HqError CreateThread(HqRunnable * runnable, HqThread ** outThreadPtr) const;
        virtual void DestroyThread(HqThread ** threadPtr) const;

    };
}
#endif
