#include "HotQBase.h"
#include "stdio.h"
#include "HqMessage.h"
#include "HqMutex.h"
#include "HqProcessEvent.h"
#include "HqShm.h"

HotQ::HotQBase::HotQBase (int id, int maxMessageSize)
: mId(id), mCachedMessageSize(maxMessageSize)
{
    char nameBuffer[512];
    sprintf(nameBuffer, "%s.%d", "data_mutex", id);
    mDataMutex = HotQ::CreateHotQMutex(nameBuffer);
    sprintf(nameBuffer, "dequeue_mutex.%d", id);
    mDequeueMutex = HotQ::CreateHotQMutex(nameBuffer);
    sprintf(nameBuffer, "dequeue_event.%d", id);
    mEnqueueEvent = HotQ::CreateHotQProcessEvent(nameBuffer);
    //printf("setting shm: %d %d %d\n", id, maxMessageSize, sizeof(HotQ::HotQBase::QState) + ((maxMessageSize + sizeof(int)) * HOT_Q_MAX_MESSAGES) );
    mShm = HotQ::CreateHotQShm(id, sizeof(HotQ::HotQBase::QState) + ((maxMessageSize + sizeof(int)) * HOT_Q_MAX_MESSAGES));
    InitializeQueueContainer();
}

HotQ::HotQBase::~HotQBase ()
{
    if (mDataMutex)
    {
        delete mDataMutex;
    }

    if (mDequeueMutex)
    {
        delete mDequeueMutex;
    }

    if (mEnqueueEvent)
    {
        delete mEnqueueEvent;
    }

    if (mShm)
    {
        delete mShm;
    }
    
}

HotQ::HqError HotQ::HotQBase::Enqueue (const HotQ::HqMessage& msg)
{
    HotQ::HqError ret = HotQ::ERR_OK;
    mDataMutex->EnterCriticalArea();  
    if (mContainer.mState->mSize >= HOT_Q_MAX_MESSAGES)
    {
        ret = HotQ::ERR_QUEUE_FULL;
    }
    else
    {
        int currentTail = mContainer.mState->mTail;
        mContainer.mState->mSize++;
        msg.Serialize(mContainer.mMessageBuffer + (currentTail * (mContainer.mState->mMaxMessageBufferSize + sizeof(int))), mContainer.mState->mMaxMessageBufferSize);
        //mContainer.mMessageBuffer[currentTail].CopyFrom(msg);
        currentTail = (currentTail + 1) % HOT_Q_MAX_MESSAGES;
        mContainer.mState->mTail = currentTail;
        mEnqueueEvent->Signal();
    }
    mDataMutex->LeaveCriticalArea();  
    return ret;
}

HotQ::HqError HotQ::HotQBase::Dequeue (HqMessage& msg, int timeout)
{
    HotQ::HqError ret = HotQ::ERR_OK;
    if (mDequeueMutex->EnterCriticalArea(timeout))
    {
        if (mEnqueueEvent->Wait(timeout))
        {
            mDataMutex->EnterCriticalArea();
            mContainer.mState->mSize--;
            int currentHead = mContainer.mState->mHead;
            msg.Read(mContainer.mMessageBuffer+ (currentHead * (mContainer.mState->mMaxMessageBufferSize + sizeof(int))), mContainer.mState->mMaxMessageBufferSize);
            currentHead = (currentHead + 1) % HOT_Q_MAX_MESSAGES;
            mContainer.mState->mHead = currentHead;
            mDataMutex->LeaveCriticalArea();
        }
        else
        {
            ret = HotQ::ERR_QUEUE_TIMEOUT;
        }
        mDequeueMutex->LeaveCriticalArea();
    }
    else
    {
        ret = HotQ::ERR_QUEUE_TIMEOUT;
    }
    return ret;
}

void HotQ::HotQBase::Reset()
{
   mDataMutex->Reset(); 
   mDequeueMutex->Reset();
   mEnqueueEvent->Reset();
   InitializeQueueContainer(true);
}

int HotQ::HotQBase::GetSize() const
{
    int size = -1;
    mDataMutex->EnterCriticalArea();
    size = mContainer.mState->mSize;
    mDataMutex->LeaveCriticalArea();
    return size;
}

void HotQ::HotQBase::InitializeQueueContainer(bool forceReset)
{
    void * sharedBuffer = mShm->Get(); 
    mContainer.mState = static_cast<HotQ::HotQBase::QState*>(sharedBuffer); 
    mContainer.mMessageBuffer = reinterpret_cast<char*>(mContainer.mState + 1);

    if (mShm->IsOwner() || forceReset)
    {
        mDataMutex->EnterCriticalArea();
        mContainer.mState->mSize = 0;
        mContainer.mState->mHead = 0;
        mContainer.mState->mTail = 0;
        mContainer.mState->mMaxMessageBufferSize = mCachedMessageSize;
        mDataMutex->LeaveCriticalArea();
    }
    else
    {
        mCachedMessageSize = mContainer.mState->mMaxMessageBufferSize;
    }
}


HotQ::HotQInterface * HotQ::CreateHotQ(int id, int maxMessageSize)
{
    return new HotQBase(id, maxMessageSize);
}
