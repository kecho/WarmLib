#ifndef _WIN_Q_EVENT_
#define _WIN_Q_EVENT_

#include "HqProcessEvent.h"
#include <Windows.h>
#define MAX_EVENT_NAME_BUFF_SZ 60

namespace HotQ
{
    class WinProcessEvent : public HqProcessEvent
    {
    public:
        WinProcessEvent(const char * name);
        virtual ~WinProcessEvent();
        virtual bool Wait(int timeout);
        virtual void Signal();
        virtual void Reset();
		bool IsErrorState() { return mIsErrorState; }
	private:
		void InitializeEvent();
		void DestroyEvent();
		char mEventName[MAX_EVENT_NAME_BUFF_SZ];
		HANDLE mEventHandle;
		bool mIsErrorState;
    };

}

#endif
