#include "HotQ/HotQ.h"
#include "HotQ/HqMutex.h"
#include <stdio.h>

#if defined(_WIN32) || defined (_WIN64)
#include <Windows.h>
#define sleep(x) Sleep(x*1000)
#else
#include <unistd.h>
#endif


void test(HotQ::HqMutex * m); 

int main (int argc, char ** argv)
{
    int d = 0;
    HotQ::HqMutex * m = HotQ::CreateHotQMutex("99");
    do
    { 
        scanf("%d", &d);
        if (d == 0) break;
        test(m);
    }while(true);
    delete m;
}

void test(HotQ::HqMutex * m)
{
    m->EnterCriticalArea();
    for (int i = 0; i < 10; ++i)
    {
        printf("%d\n", i);
        sleep(1);
    }
    m->LeaveCriticalArea();
}

