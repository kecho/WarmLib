#ifndef __HOT_Q_BASE__
#define __HOT_Q_BASE__

#include "HotQ.h"
#include "HqDefines.h"


namespace HotQ
{
    class HqMutex;
    class HqProcessEvent;
    class HqShm;
    class HqMessage;

    class HotQBase : public HotQInterface
    {
    public:
        HotQBase (int id, int maxMessageSize);
        virtual ~HotQBase();
        virtual HqError Enqueue(const HqMessage& msg);
        virtual HqError Dequeue(HqMessage& msg, int timeout);
        virtual void Reset();
        virtual int GetSize() const;

    private:
        void InitializeQueueContainer(bool forceReset = false);
         

    protected:

        struct QState
        {
            int mMaxMessageBufferSize;
            int mSize;
            int mHead;
            int mTail;
        };

        struct QContainer
        {
            struct QState * mState; 
            char * mMessageBuffer;
        } mContainer;

        int mId;

        HqMutex * mDataMutex;
        HqMutex * mDequeueMutex;
        HqProcessEvent * mEnqueueEvent;
        HqShm * mShm;
        int mCachedMessageSize;
    };
}

#endif
