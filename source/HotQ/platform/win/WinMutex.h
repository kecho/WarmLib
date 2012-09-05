#ifndef __WIN_Q_MUTEX__
#define __WIN_Q_MUTEX__

#include "HqMutex.h"
#include <Windows.h>

#define MAX_MUTEX_NAME_BUFF_SZ 60

namespace HotQ
{
	class WinMutex : public HqMutex
	{
	public:
		WinMutex(const char * name);
        virtual ~WinMutex();
        virtual bool EnterCriticalArea(int timeout);
        virtual void LeaveCriticalArea();
        virtual void Reset();
		bool IsErrorState() { return mIsErrorState; }
	private:
		void InitializeNamedMutex();
		void DestroyMutex();
		HANDLE mMutexHandle;
		char mMutexName[MAX_MUTEX_NAME_BUFF_SZ];
		bool mIsErrorState;
	};
}

#endif