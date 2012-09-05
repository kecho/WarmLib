#include "WinThread.h"
#include "HqRunnable.h"
#include <stdio.h>

HotQ::WinThread::WinThread(HotQ::HqRunnable * runnable)
	:
mRunnable(runnable), mThreadHandle(0), mReturnValue(0)
{
}

HotQ::WinThread::~WinThread()
{
	if (mThreadHandle)
	{
		CloseHandle(mThreadHandle);
	}
}

DWORD WINAPI HotQ::WinThread::RunnableMain(LPVOID threadPtr)
{
	HotQ::WinThread * winThread = reinterpret_cast<HotQ::WinThread*>(threadPtr);
	winThread->mReturnValue = winThread->mRunnable->Run();
	return 0;
}

HotQ::HqError HotQ::WinThread::Start()
{
	HotQ::HqError ret = HotQ::ERR_OK;
	DWORD unused = 0;
	mThreadHandle = CreateThread(NULL, 0, HotQ::WinThread::RunnableMain, this, 0, &unused);
	if (mThreadHandle == NULL)
	{
		ret = HotQ::ERR_THREAD_INIT;
	}
	return ret;
}

void * HotQ::WinThread::Join()
{
	WaitForSingleObject(mThreadHandle,INFINITE);
	return mReturnValue;
}

void HotQ::WinThread::ForceKill()
{
	/* not implemented */
}