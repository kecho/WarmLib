#ifndef __WARM_WORKER__
#define __WARM_WORKER__

#include "WarmLibDefines.h"
#include "WarmWorkerMessage.h"
#include "WarmWorkerMessageReceiver.h"
#include "HotQ/HqRunnable.h"
#include "ColdSdk/ColdPluginImport.h"

namespace HotQ
{
    class HqThread;
    class HqThreadFactory;
    class HotQInterface;
}

namespace WarmLib
{

    class WarmWorker : public WarmWorkerMessageReceiver
    {
    public:
        WarmWorker(const char * pluginName, int workerId, int outgoingConnectionId, int jobmessageSize, int responseSize, const HotQ::HqThreadFactory * factory = 0);
        virtual ~WarmWorker();
        bool Execute(bool blockExecution = true);

    protected:
        virtual bool OnMessage(const WarmWorkerMessage& message);

    private:
        void SendMessage(WarmMessageType type);
        ColdSdk::ColdPlugin * mPlugin;
        HotQ::HotQInterface * mOutgoingQueue;
        ColdSdk::Error mPluginError;
        char* mResponseBuffer;
        int  mResponseBufferSize;
    };
}
#endif
