#include "PosixThreadMutex.h"

HotQ::PosixThreadMutex::PosixThreadMutex()
{
    pthread_mutex_init(&mMutex, 0);
}

HotQ::PosixThreadMutex::~PosixThreadMutex()
{
    pthread_mutex_destroy(&mMutex);
}

bool HotQ::PosixThreadMutex::EnterCriticalArea(int timeout /*ignored*/)
{
    return pthread_mutex_lock(&mMutex) == 0;
}

void HotQ::PosixThreadMutex::LeaveCriticalArea()
{
    pthread_mutex_unlock(&mMutex);
}

void HotQ::PosixThreadMutex::Reset()
{
    pthread_mutex_destroy(&mMutex);
    pthread_mutex_init(&mMutex, 0);
}

HotQ::HqMutex * HotQ::CreateHotQThreadMutex()
{
    return new HotQ::PosixThreadMutex();
}
