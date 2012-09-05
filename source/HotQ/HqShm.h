#ifndef __HOT_Q_SHM__
#define __HOT_Q_SHM__

namespace HotQ
{
    class HqShm
    {
    public: 
        HqShm(){}
        virtual ~HqShm(){}
        virtual void * Get() = 0;
        virtual int GetSize() = 0;
        virtual bool IsOwner() = 0;
    };

    HqShm * CreateHotQShm(int id, int size);
}

#endif
