#ifndef __HOT_Q_DEFINES__
#define __HOT_Q_DEFINES__

#define HOT_Q_MAX_MESSAGES 10
#define HOT_Q_INFINITE -1

namespace HotQ
{

    enum HqError
    {
        ERR_OK = 0,
        ERR_UNKNOWN,
        ERR_QUEUE_FULL,
        ERR_QUEUE_EMPTY,
        ERR_QUEUE_TIMEOUT,
        ERR_THREAD_INIT,
        ERR_PROCESS_INIT,
        ERR_PROCESS_WAIT
    };

}

#endif
