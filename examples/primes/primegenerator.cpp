/**
Kleber Garcia (c) 2012
**/

#include <stdio.h>
#include "WarmLib/WarmWorkerPoolManager.h"
#include "common/jobtypes.h"
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
    printf("2) Schedule Job\n");
    printf("0) exit\n");
}

void ask(ComputePrimesJob * job)
{
    printf("The plugin can only calculate up to %d numbers", MAX_PRIME_NUMBERS); 
    printf("Enter initial number (doesnt have to be prime): ");
    scanf("%d", &job->initialNumber);
    printf("\n Enter end number: ");
    scanf("%d", &job->endNumber);
    printf("\n");
}

void printResponse(const ComputedPrimesResponse& response)
{
    printf("count:%d p:[", response.count);
    for (int i = 0; i < response.count; ++i)
    {
        printf("%d ", response.primeList[i]);
    }
    printf("]\n");
}

typedef WarmWorkerPool<ComputePrimesJob, ComputedPrimesResponse> PrimesWorkerPool;

class PrimesListener : public PrimesWorkerPool::Listener
{
public:
    PrimesListener(){}
    virtual ~PrimesListener(){}
    virtual void OnJobFinished(int workerId, const ComputedPrimesResponse& response)
    {
        printf("worker id: %d. Prime numbers generated have finished.\n",workerId, response.count);
        printResponse(response);
    }

    virtual void OnWorkerError (int workerId, WarmLib::Error error)
    {
        printf("Worker has errord. Error code is %d, worker id is %d\n", error, workerId);
    
    }

    virtual void OnJobStarted(int workerId)
    {
        printf("Job Started at worker ID: %d\n", workerId);
    }

};

int main()
{
    ComputePrimesJob job; 
    int poolId = 1337;   
    int decision = 0;
    PrimesListener listener;
    PrimesWorkerPool poolManager(7, poolId, "./primeplugin.so", &listener);

    poolManager.StartWorkers();
    do
    {
        explain();
        scanf("%d", &decision);
        switch(decision)
        {
        case 0:
            {
                poolManager.ShutdownAll();
            }
            break;
        case 1:
            {
                for (int i = 0; i < poolManager.GetNumberOfWorkers(); ++i)
                {
                    printf("%i) worker id: %d status: %s\n", i, poolManager.GetWorkerId(i), GetWorkerStateName(poolManager.GetWorkerState(i)));
                }
            }
            break;
        case 2:
            {
                ask(&job);
                poolManager.Schedule(job);
            }
        default:
            break;
        }
    }while(decision != 0);
}

