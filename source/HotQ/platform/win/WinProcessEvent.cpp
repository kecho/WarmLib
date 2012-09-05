#include "WinProcessEvent.h"
#include <stdio.h>
#include "HqDefines.h"

HotQ::WinProcessEvent::WinProcessEvent(const char * name)
	:
mEventHandle(0), mIsErrorState(false)
{
	sprintf_s(mEventName, MAX_EVENT_NAME_BUFF_SZ, "%s", name);
	InitializeEvent();
}
HotQ::WinProcessEvent::~WinProcessEvent()
{
	DestroyEvent();
}

void HotQ::WinProcessEvent::InitializeEvent()
{
	mEventHandle = CreateSemaphore(NULL, 0, HOT_Q_MAX_MESSAGES, mEventName);
	mIsErrorState = !mEventHandle;
}

void HotQ::WinProcessEvent::DestroyEvent()
{
	if (mEventHandle)
	{
		CloseHandle(mEventHandle);
	}
}

bool HotQ::WinProcessEvent::Wait(int timeout)
{
	DWORD winTimeout = timeout == HOT_Q_INFINITE ? INFINITE : timeout * 1000;	
	DWORD result = WaitForSingleObject(mEventHandle, winTimeout);
	if (result == WAIT_TIMEOUT)
	{
		return false;
	}
	return true;
}
void HotQ::WinProcessEvent::Signal()
{
	ReleaseSemaphore(mEventHandle,1,NULL);
}

void HotQ::WinProcessEvent::Reset()
{
	DestroyEvent();
	InitializeEvent();
}

HotQ::HqProcessEvent * HotQ::CreateHotQProcessEvent(const char * name)
{
	HotQ::WinProcessEvent * e = new HotQ::WinProcessEvent(name);
	if (e->IsErrorState())
	{
		delete e;
		e = 0;
	}
	return e;
}