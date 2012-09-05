#ifndef __WIN_THREAD__
#define __WIN_THREAD__

#include "HqThread.h"
#include <Windows.h>

namespace HotQ
{
	class HqRunnable;

	class WinThread : public HqThread
	{
	public:
		WinThread(HqRunnable * runnable);
		virtual ~WinThread();
		virtual HqError Start();
        virtual void * Join();
        virtual void ForceKill();

		

	private:
		static DWORD WINAPI RunnableMain(LPVOID threadPtr);
		HqRunnable * mRunnable;
		void * mReturnValue;
		HANDLE mThreadHandle;

	};
}

#endif
