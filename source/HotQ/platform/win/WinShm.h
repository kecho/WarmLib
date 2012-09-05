#ifndef __WIN_Q_SHM_
#define __WIN_Q_SHM_

#include "HqShm.h"
#include <Windows.h>

#define MAX_SHM_NAME_BUFF_SZ 60

namespace HotQ
{
	class WinShm : public HqShm
	{
	public:
		WinShm(int id, int size);
		virtual ~WinShm();
		virtual void * Get();
		virtual int GetSize();
		virtual bool IsOwner();
	private:
		void Initialize();
		void Destroy();
		HANDLE mFileHandle;
		void * mSharedBuffer;
		char mName[MAX_SHM_NAME_BUFF_SZ];
		bool mIsOwner;
		int mSize;
	};
}

#endif