#ifndef __HOT_Q_INTERFACE__
#define __HOT_Q_INTERFACE__

#include "HqDefines.h"

#define DEFAULT_HOT_Q_MESSAGE_BUFFER_SZ 256

namespace HotQ
{

    class HqMessage;

    class HotQInterface
    {
    public:
        HotQInterface(){}
        virtual ~HotQInterface(){}
        virtual HqError Enqueue(const HqMessage& msg) = 0;
        virtual HqError Dequeue(HqMessage& msg, int timeout = HOT_Q_INFINITE) = 0;
        virtual void Reset() = 0;
        virtual int GetSize() const = 0;
    };

    HotQInterface * CreateHotQ(int id, int maxMessageSize = DEFAULT_HOT_Q_MESSAGE_BUFFER_SZ);
}

#endif
