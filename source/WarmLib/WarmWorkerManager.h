#ifndef __WARM_WORKER_MANAGER__
#define __WARM_WORKER_MANAGER__

#include "WarmLibDefines.h"
#include "HotQ/HqRunnable.h"
#include <string>

namespace HotQ
{
    class HqThread;
    class HqThreadFactory;
    class HqProcess;
    class HotQInterface;
}

namespace WarmLib
{
    class WarmWorkerEventListener
    {
    public:
        WarmWorkerEventListener(){}
        virtual ~WarmWorkerEventListener(){}
        virtual void OnSignalStart(int workerId) = 0;
        virtual void OnError(int workerId, Error error)=0;
        virtual void OnShutdown(int workerId) = 0;

    };

    class WarmWorkerMessage;

    class WarmWorkerManager : public HotQ::HqRunnable
    {
    public:
        WarmWorkerManager(
            const char * coldPluginName, 
            int workerId, 
            int responseQueueId, 
            WarmWorkerEventListener * eventListener,
            int maxMessageSize, 
            int reponseSize,
            const HotQ::HqThreadFactory * threadFactory
        );
        virtual ~WarmWorkerManager();
        bool StartWorker();
        void KillWorker();
        void SendMessageToWorker(const WarmLib::WarmWorkerMessage& message);          
    protected:
        virtual void * Run();

        std::string mPluginName;
        HotQ::HqProcess * mWorkerProcess; 
        HotQ::HotQInterface * mMessageQueue;
        HotQ::HqThread * mWatchDogThread;

        WarmWorkerEventListener * mListener;

        int mResponseQueueId;
        int mWorkerId;
        int mMaxMessageSize;
        int mResponseSize;
    };
}

#endif
