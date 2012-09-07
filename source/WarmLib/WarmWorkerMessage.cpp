#include "WarmWorkerMessage.h"
#include <string.h>
#include <stdio.h>

void WarmLib::WarmWorkerMessage::Read(const char * inputBuffer, int bufferSize)
{
    memcpy(&mState, inputBuffer, sizeof(mState));
    if (mState.mType == WarmLib::MESSAGE_JOB_START || mState.mType == WarmLib::MESSAGE_JOB_FINISHED)
    { 
        int jobBufferSize = mState.mJobBufferSize;
        mState.mJobBuffer = 0;
        mState.mJobBufferSize = 0;
        SetJobBuffer(inputBuffer + sizeof(mState), jobBufferSize);
    }

}

WarmLib::WarmWorkerMessage::~WarmWorkerMessage()
{
    DestroyJobBuffer();
}

void WarmLib::WarmWorkerMessage::Serialize(char * targetBuffer, int bufferSize) const
{
    memcpy(targetBuffer, &mState, sizeof(mState));
    if (mState.mType == WarmLib::MESSAGE_JOB_START || mState.mType == WarmLib::MESSAGE_JOB_FINISHED)
    {
        memcpy(targetBuffer + sizeof(mState), mState.mJobBuffer, mState.mJobBufferSize);
    }
}

void WarmLib::WarmWorkerMessage::SetJobBuffer(const void * job, int size)
{
    DestroyJobBuffer();
    mState.mJobBuffer = static_cast<void*>(new char[size]); 
    memcpy(mState.mJobBuffer, job, size);
    mState.mJobBufferSize = size;
}

void WarmLib::WarmWorkerMessage::DestroyJobBuffer()
{
    if (mState.mJobBuffer != 0)
    {
        delete [] reinterpret_cast<char*>(mState.mJobBuffer);
        mState.mJobBuffer = 0;
        mState.mJobBufferSize = 0;
    }
}
