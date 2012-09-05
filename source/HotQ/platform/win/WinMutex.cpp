#include "WinMutex.h"
#include <stdio.h>
#include "HqDefines.h"

HotQ::WinMutex::WinMutex(const char * name)
	:
mMutexHandle(0), mIsErrorState(false)
{
	sprintf_s(mMutexName,MAX_MUTEX_NAME_BUFF_SZ,"%s",name);
	InitializeNamedMutex();
}

HotQ::WinMutex::~WinMutex()
{
	DestroyMutex();
}

void HotQ::WinMutex::InitializeNamedMutex()
{
	mMutexHandle = CreateMutex(NULL, false, mMutexName);
	mIsErrorState = !mMutexHandle;
}

void HotQ::WinMutex::DestroyMutex()
{
	if (mMutexHandle)
	{
		CloseHandle(mMutexHandle);
	}
}

bool HotQ::WinMutex::EnterCriticalArea(int timeout)
{
	DWORD winTimeout = timeout == HOT_Q_INFINITE ? INFINITE : timeout * 1000;
	DWORD result = WaitForSingleObject(mMutexHandle, winTimeout);

	if (result == WAIT_TIMEOUT)
	{
		return false;
	}

	return true;
}

void HotQ::WinMutex::LeaveCriticalArea()
{
	ReleaseMutex(mMutexHandle);
}

void HotQ::WinMutex::Reset()
{
	DestroyMutex();
	InitializeNamedMutex();
}

HotQ::HqMutex * HotQ::CreateHotQMutex(const char * name)
{
	WinMutex * ret = new WinMutex(name);
	if (ret->IsErrorState())
	{
		delete ret;
		ret = 0;
	}
	return ret;
}