#include "WinShm.h"
#include <stdio.h>

HotQ::WinShm::WinShm(int id, int size)
	:
mSize(size), mSharedBuffer(0), mIsOwner(false)
{
	sprintf_s(mName, MAX_SHM_NAME_BUFF_SZ, "shm.%d", id);
	Initialize();
}

HotQ::WinShm::~WinShm()
{
	Destroy();	
}

void HotQ::WinShm::Destroy()
{
	if (mFileHandle)
	{
		CloseHandle(mFileHandle);
	}
}

void HotQ::WinShm::Initialize()
{
	SetLastError(ERROR_SUCCESS);
	mFileHandle = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,mSize,mName);

	if (mFileHandle != 0)
	{
		mIsOwner = GetLastError() != ERROR_ALREADY_EXISTS;
		if (mIsOwner)
		{
            // doesnt matter
		}
		mSharedBuffer = MapViewOfFile(mFileHandle,FILE_MAP_ALL_ACCESS,0,0,mSize);	
	}
	else
	{
		DWORD err = GetLastError();
	}
}

void * HotQ::WinShm::Get()
{
	return mSharedBuffer;
}

int HotQ::WinShm::GetSize()
{
	return mSize;
}

bool HotQ::WinShm::IsOwner()
{
	return mIsOwner;
}

HotQ::HqShm * HotQ::CreateHotQShm(int id, int size)
{
	return new HotQ::WinShm(id, size);
}
