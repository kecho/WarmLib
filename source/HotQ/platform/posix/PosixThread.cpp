#include "PosixThread.h"
#include <stdio.h>

HotQ::PosixThread::PosixThread(HotQ::HqRunnable * runnableObject)
: mRunnableObject(runnableObject)
{
}

HotQ::PosixThread::~PosixThread()
{
}

void * HotQ::PosixThread::Join()
{
    void * result = 0;
    pthread_join(mHandle, &result);
    return result;
}

void * _runnable_main(void * runnable)
{
    HotQ::HqRunnable * r = static_cast<HotQ::HqRunnable*>(runnable);
    return r->Run();
}

HotQ::HqError HotQ::PosixThread::Start()
{
    HotQ::HqError ret = HotQ::ERR_OK;
    int status = pthread_create(&mHandle, NULL, _runnable_main,mRunnableObject);
    if (status != 0)
    {
        ret = HotQ::ERR_THREAD_INIT;
    }
    return ret;
}

void HotQ::PosixThread::ForceKill()
{
    //unimplemented
}


