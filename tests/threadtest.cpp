#include "HotQ/HqThreadFactory.h"
#include "HotQ/HqRunnable.h"
#include "HotQ/HqThread.h"
#include "HotQ/HqMutex.h"
#include <stdio.h>

class Thread : public HotQ::HqRunnable
{
public:
    Thread( int id, HotQ::HqMutex * m) : mMutex(m), mId(id) {};
    virtual ~Thread(){}
    virtual void * Run();
protected:
    int mId;
    HotQ::HqMutex *mMutex;
    
};


void * Thread::Run()
{
    int d;
    mMutex->EnterCriticalArea();
    printf("Thread %d has entered critical area\n", mId);
    scanf("%d",&d);
    mMutex->LeaveCriticalArea();
    return 0;
}

int main()
{
   HotQ::HqMutex * mutex = HotQ::CreateHotQThreadMutex();
   Thread t1(1, mutex);
   Thread t2(2, mutex);
   HotQ::HqThreadFactory * f = HotQ::CreateDefaultHqThreadFactory();
   HotQ::HqThread * thread1 = 0;
   HotQ::HqThread * thread2 = 0;
   HotQ::HqError err = f->CreateThread(&t1, &thread1); 
   err = f->CreateThread(&t2, &thread2); 

   if (err == HotQ::ERR_OK)
   {
        printf("starting thread\n");
        thread1->Start();
        thread2->Start();
        printf("joining threads\n");
        printf("t1 returned: %p\n", thread1->Join());
        printf("t2 returned: %p\n", thread2->Join());
   }
   else
   {
        printf("error!\n");
   }
   
   f->DestroyThread(&thread1);
   f->DestroyThread(&thread2);
   delete f;
   delete mutex;
}
