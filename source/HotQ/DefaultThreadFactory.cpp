#include "DefaultThreadFactory.h"

#if defined(_WIN32) || defined(_WIN64)
#include "platform/win/WinThread.h"
#else
#include "platform/posix/PosixThread.h"
#endif

HotQ::HqError HotQ::DefaultThreadFactory::CreateThread(HqRunnable * runnable, HqThread ** outThreadPtr) const
{
    HqThread * t = 0;
    HotQ::HqError ret = HotQ::ERR_OK;

#if defined(_WIN32) || defined(_WIN64)
	t = new HotQ::WinThread(runnable);	
#else
    t = new HotQ::PosixThread(runnable);
#endif

    *outThreadPtr = t;
    return ret;
}

void HotQ::DefaultThreadFactory::DestroyThread(HqThread ** threadPtr) const
{
    delete *threadPtr;
    *threadPtr = 0;
}

HotQ::HqThreadFactory * HotQ::CreateDefaultHqThreadFactory()
{
    return new HotQ::DefaultThreadFactory();
}
