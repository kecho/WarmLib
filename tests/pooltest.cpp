#include <stdio.h>
#include "WarmLib/WarmWorkerPoolManager.h"

struct Tt
{
    int a;
    int b;
    int c;
    int d;
    int kk;
};

using namespace WarmLib;

const char * GetWorkerStateName(WorkerState s)
{
    switch(s)
    {
    case WS_SHUTDOWN:
        return "WS_SHUTDOWN";
    case WS_IDLE:
        return "WS_IDLE";
    case WS_PROCESSING:
        return "WS_PROCESSING";
    case WS_LOCKED:
        return "WS_LOCKED";
    case WS_ERROR:
        return "WS_ERROR";
    case WS_UNKNOWN:
        return "WS_UNKNOWN";
    }
    return "";
}

void explain()
{
    printf("Menu selection:\n");
    printf("1) List Worker States\n");
    printf("2) Shutdown Workers\n");
    printf("3) Schedule Job\n");
    printf("0) exit\n");
}

int main()
{
    Tt t;
    t.a = 1;
    t.b = 2;
    t.c = 3;
    t.d = 99;
    short poolid = 4975; 
    int decision = 0;
    WarmWorkerPool<Tt> poolManager(7, poolid, "./example_cold_plugin.so");
    poolManager.StartWorkers();
    do
    {
        explain();
        scanf("%d", &decision);
        switch(decision)
        {
        case 1:
            {
                for (int i = 0; i < poolManager.GetNumberOfWorkers(); ++i)
                {
                    printf("%d) status: %s\n", poolManager.GetWorkerId(i), GetWorkerStateName(poolManager.GetWorkerState(i)));
                }
            }
            break;
        case 2:
        case 0:
            {
                poolManager.ShutdownAll();
            }
            break;
        case 3:
            {
                poolManager.Schedule(t);
            }
        default:
            break;
        }
    }while(decision != 0);
    printf("done\n");
}


