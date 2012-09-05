#ifndef __WARM_WORKER_DEFINES__
#define __WARM_WORKER_DEFINES__
#include <map>

namespace WarmLib
{
    enum Error
    {
        ERR_OK,
        ERR_BAD_ARGS,
        ERR_NO_PLUGIN,
        ERR_PROCESS_START,
        ERR_NO_WORKER_AVAILABLE,
        ERR_UNKNOWN
    };

    enum WorkerState
    {
        WS_SHUTDOWN,
        WS_IDLE,
        WS_LOCKED,
        WS_PROCESSING,
        WS_ERROR,
        WS_UNKNOWN
    };


}

#define WARM_LIB_WORKER_BIN "./wworker"

#endif
