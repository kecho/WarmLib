#ifndef __WARMLIB_MESSAGE_RECEIVER__
#define __WARMLIB_MESSAGE_RECEIVER__

#include "HotQ/HqRunnable.h"

namespace HotQ
{
    class HotQInterface;
    class HqThread;
    class HqThreadFactory;
}

namespace WarmLib
{
    class WarmWorkerMessage;

    class WarmWorkerMessageReceiver : public HotQ::HqRunnable
    {
    public:
        WarmWorkerMessageReceiver(int queueId, const HotQ::HqThreadFactory * factory, bool resetQueue = true, int messageSize = -1);
        virtual ~WarmWorkerMessageReceiver();
        void Listen();
        void JoinListening();
        int GetId() const {return mId;}
        int GetMessageReceiverSize() { return mMaxMessageSize; }
    protected:
        virtual bool OnMessage(const WarmLib::WarmWorkerMessage& message) = 0;
        virtual void * Run();
        
    private:
        int mId;
        HotQ::HotQInterface * mQueue;
        HotQ::HqThread * mListenerThread;
        int mMaxMessageSize;
        

    };
}

#endif
