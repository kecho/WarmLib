#ifndef __POSIX_SHM__
#define __POSIX_SHM__

#include "HqShm.h"

namespace HotQ
{
    class PosixShm : public HqShm
    {
    public:
        PosixShm (int id, int size);
        virtual ~PosixShm();
        virtual void * Get(); 
        virtual int GetSize() { return mSize; }
        virtual bool IsOwner() { return mIsOwner; }

    private:
        void InitializeShm();
        void DestroyShm();
        void * BindMemory();
        int mId;
        int mShmid;
        int mSize;
        bool mIsOwner;
        void * mSharedBuffer;
    };

}

#endif
