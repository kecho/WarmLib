#ifndef __HOT_Q_PROCESS__
#define __HOT_Q_PROCESS__

#include "HqDefines.h"

namespace HotQ
{
    class HqProcess
    {
    public:
        HqProcess(){}
        virtual ~HqProcess(){}
        virtual int GetProcessId() = 0;
        virtual HqError Start(int argc, const char * argv[]) = 0;
        virtual HqError Join(int * exitVal = 0) = 0;
        virtual HqError Kill() = 0;
    };

    HqProcess * CreateHotQProcess(const char * executablePath);
}
#endif
