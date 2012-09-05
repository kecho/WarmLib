#ifndef __WARM_WORKER_POOL_MANAGER__
#define __WARM_WORKER_POOL_MANAGER__

#include "WarmWorkerManager.h"
#include "WarmWorkerMessageReceiver.h"
#include "WarmLibDefines.h"
#include "WarmWorkerThreadSafeStateBoard.h"
#include "HotQ/HqThread.h"
#include "HotQ/HqRunnable.h"
#include <map>

#define PLUGIN_NAME_BUFF_SIZE 60

namespace HotQ
{
    class HqThread;
    class HotQInterface;
    class HqThreadFactory;
    class HotQInterface;
}

namespace WarmLib
{
    class WarmWorkerPoolEventListener
    {
    public:
        WarmWorkerPoolEventListener(){} 
        virtual ~WarmWorkerPoolEventListener(){} 
        virtual void OnJobStarted(int workerId) = 0;
        virtual void OnWorkerError(int workerId, WarmLib::Error) = 0;
        virtual void OnWorkerJobFinished(int workerId, const void * jobResponse) = 0;
    };

    class WarmWorkerMessage;

    typedef std::map<int, WarmWorkerManager*> WarmWorkerManagerMap; 
    typedef std::map<int, WarmWorkerManager*>::iterator WarmWorkerManagerMapIterator;

    class WarmWorkerPoolManager : public WarmWorkerEventListener, public WarmWorkerMessageReceiver 
    {
    public:
        WarmWorkerPoolManager(int numberOfWorkers, short poolId, const char * coldPluginName, int jobMessageSize, int jobResponseMessageSize, WarmWorkerPoolEventListener * listener = 0, const HotQ::HqThreadFactory * threadFactory = 0);
        virtual ~WarmWorkerPoolManager();
        void StartWorkers();
        void ShutdownAll();
        int GetNumberOfWorkers() const {return mNumberOfWorkers;};
        WorkerState GetWorkerState(int index);
        int GetWorkerId (int index) const {return GetWorkerId(index, mPoolId);}
        Error ScheduleJob(const void * arg);
    protected:
        virtual void OnSignalStart(int workerId);
        virtual void OnShutdown(int workerId);
        virtual void OnError(int workerid, WarmLib::Error error);

        virtual bool OnMessage(const WarmWorkerMessage& message);
        WarmWorkerPoolEventListener * mListener;

    private:
        WarmWorkerManager * AcquireWorker();

        void ListenToResponses();

        void InitializeWorkers(const char* coldPluginName, const HotQ::HqThreadFactory * threadFactory);
        int GetWorkerId (int index, int poolId) const;
        int GetResponseQueueId(int seed, int poolId) const { return GetWorkerId(seed ,poolId);}
        int GetResponseQueueId() const { return GetResponseQueueId(mNumberOfWorkers, mPoolId);}

        int mNumberOfWorkers;
	    int mJobMessageSize;
        char mPluginName[PLUGIN_NAME_BUFF_SIZE];
        WarmWorkerManagerMap mManagerMap;
        short mPoolId;
        int   mNextWorkerIndex;
        HotQ::HotQInterface * mResponseQueue;

        WarmWorkerThreadSafeStateBoard mStateBoard;

    };

    template <typename jobtype, typename jobresponse = int>
    class WarmWorkerPool : public WarmWorkerPoolManager
    {
    public:
        class Listener : public WarmWorkerPoolEventListener 
        {
        public:
            Listener() {}
            virtual ~Listener() {}
            virtual void OnJobFinished(int workerId, const jobresponse& response) = 0;
            virtual void OnWorkerError(int workerId, WarmLib::Error error) = 0;
            virtual void OnJobStarted(int workerId) = 0;
        protected:
            virtual void OnWorkerJobFinished(int workerId, const void * jobResponse);
        private:
        
        };
        WarmWorkerPool(int numberOfWorkers, short poolId, const char * coldPluginName, Listener * listener = 0, const HotQ::HqThreadFactory * threadFactory = 0)
        :
        WarmWorkerPoolManager(numberOfWorkers, poolId, coldPluginName, sizeof(jobtype), sizeof(jobresponse), listener, threadFactory)
        {
        }
        virtual ~WarmWorkerPool(){}
        Error Schedule(const jobtype& job);        
    };

    template <typename jobtype, typename jobresponse> Error WarmWorkerPool<jobtype, jobresponse>::Schedule(const jobtype& job)
    {
        return ScheduleJob(reinterpret_cast<const void*>(&job)); 
    }

    template <typename jobtype, typename jobresponse> void WarmWorkerPool<jobtype, jobresponse>::Listener::OnWorkerJobFinished(int workerId, const void * response)
    {
        OnJobFinished(workerId, *reinterpret_cast<const jobresponse*>(response));
    }   
    
}

#endif
