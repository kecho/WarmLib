#ifndef __HOT_Q_PROCESS_POSIX_
#define __HOT_Q_PROCESS_POSIX_
#include "HqProcess.h"
#include <spawn.h>
#define PROCESS_NAME_BUFFER_SIZE 60

namespace HotQ
{
    class PosixProcess : public HqProcess
    {
    public:
        PosixProcess(const char * execPath);
        virtual ~PosixProcess();
        virtual int GetProcessId();
        virtual HqError Start(int argc, const char * argv[]);
        virtual HqError Join(int * exitValue);
        virtual HqError Kill();
    private:
        void InitializeArgv(int argc, const char * argv[]);
        void DestroyArgv();
        char mProcessName[PROCESS_NAME_BUFFER_SIZE];
        char ** mArgv;
        int mArgc;
        pid_t mPid;
    };

}


#endif
