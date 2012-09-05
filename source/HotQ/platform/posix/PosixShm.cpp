#include "PosixShm.h"
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

HotQ::PosixShm::PosixShm (int id, int size)
:
mId(id), mSize(size), mIsOwner(false), mSharedBuffer(0)
{
    InitializeShm();
    if (mSharedBuffer == 0 && errno == EINVAL)
    {
        //LINUX HACK
        //failed initializing memory so reset it.
        //this means that we are requesting a memory segment already existant with a bigger size. we are just then opening a memory segment with size 1, opening it and destroying it and reopening it with the correct size.
        mSize = 1;
        InitializeShm(); 
        mSize = size;
        DestroyShm();
        InitializeShm(); 
    }
}

HotQ::PosixShm::~PosixShm ()
{
    shmdt(mSharedBuffer);
    if (mIsOwner && mShmid != -1)
    {
        DestroyShm();
    }
}

void HotQ::PosixShm::DestroyShm()
{
    struct shmid_ds buff; 
    shmctl(mShmid, IPC_RMID, &buff);
}

void HotQ::PosixShm::InitializeShm()
{
    key_t key = mId;
    //create segment and test if it exists
    mShmid = shmget(key, mSize, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    mIsOwner = true;//assume is the owner
    if (mShmid == -1  && errno == EEXIST)
    {
        mIsOwner = false;
        //this means buffer already exists, so we just call with IPC_CREAT so we bind to it
        //and wont return an error if it already exists (flag IPC_EXCL)
        mShmid = shmget(key, mSize, IPC_CREAT | S_IRUSR | S_IWUSR);
    }

    if (mShmid != -1)
    {   
        mSharedBuffer = BindMemory();
    }
    else
    {
        //TODO: perform correct error handling
        perror("ERROR SHMID");
        mSharedBuffer = 0; //memory cant be initiualized
    }
}

void * HotQ::PosixShm::BindMemory()
{
    if (mShmid != -1)
    {
        mSharedBuffer = shmat(mShmid, NULL, 0);
    }
    else
    {
        mSharedBuffer = 0;
        perror("ERROR BIND MEMORY");
    }
    return mSharedBuffer;
}

void * HotQ::PosixShm::Get()
{
    return mSharedBuffer;
}

HotQ::HqShm * HotQ::CreateHotQShm (int id, int size)
{
    return new HotQ::PosixShm(id, size);
}

