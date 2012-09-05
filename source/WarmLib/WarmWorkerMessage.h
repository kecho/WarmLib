#ifndef __WARM_WORKER_MESSAGE_
#define __WARM_WORKER_MESSAGE_

#include "HotQ/HqMessage.h"

namespace WarmLib
{
    enum WarmMessageType
    {
        MESSAGE_NONE,
        MESSAGE_JOB_START,
        MESSAGE_PING,
        MESSAGE_PING_RESPONSE,
        MESSAGE_POISON,
        MESSAGE_JOB_STARTED,
        MESSAGE_JOB_FINISHED,
        MESSAGE_UNKNOWN
    };

    class WarmWorkerMessage : public HotQ::HqMessage
    {
    public:
        WarmWorkerMessage() 
        {
            mState.mType = MESSAGE_NONE; 
            mState.mWorkerId = 0; 
            mState.mJobBufferSize = 0;
            mState.mJobBuffer = 0; 
        } 
        WarmWorkerMessage(WarmMessageType type, int workerId = 0)
        {
            mState.mType = type;
            mState.mWorkerId = workerId;
            mState.mJobBufferSize = 0;
            mState.mJobBuffer = 0; 
        }
        virtual ~WarmWorkerMessage();
        void SetJobBuffer(const void * job, int size);
        const void * GetJobBuffer() const {return mState.mJobBuffer;}
        int GetWorkerId() const {return mState.mWorkerId;}
        WarmLib::WarmMessageType GetType() const {return mState.mType;}
    protected:
        virtual void Serialize(char * outputBuffer, int size) const;
        virtual void Read(const char * outputBuffer, int size);
    private:
        struct State {
            WarmMessageType mType;
            int mWorkerId;
            int mJobBufferSize;
            void * mJobBuffer;
        } mState;

        void DestroyJobBuffer();
    };
}

#endif
