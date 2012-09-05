#ifndef __HOT_Q_RUNNABLE__
#define __HOT_Q_RUNNABLE__

namespace HotQ
{
    class HqRunnable
    {
    public:
        HqRunnable(){}
        virtual ~HqRunnable(){}
        virtual void * Run() = 0;
    };
}

#endif
