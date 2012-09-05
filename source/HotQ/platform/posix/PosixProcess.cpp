#include "PosixProcess.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
HotQ::PosixProcess::PosixProcess(const char * execPath)
: 
mArgc(0), mArgv(0), mPid(0)
{
    sprintf(mProcessName, "%s", execPath);
}

HotQ::PosixProcess::~PosixProcess()
{
    DestroyArgv();
}

int HotQ::PosixProcess::GetProcessId()
{
    return static_cast<int>(mPid);
}

void HotQ::PosixProcess::InitializeArgv(int argc, const char * argv[])
{
    mArgv = new char*[argc + 2];
    mArgc = argc + 1;
    for (int i = 0; i < mArgc; ++i)
    {
        if (i == 0)
        {
            mArgv[i] = new char[strlen(mProcessName) + 1];
            sprintf(mArgv[i], "%s", mProcessName);
        }
        else
        {
            mArgv[i] = new char[strlen(argv[i-1]) + 1];
            sprintf(mArgv[i], "%s", argv[i-1]);
        }
    }
    mArgv[mArgc] = 0;
}

void HotQ::PosixProcess::DestroyArgv()
{
    if (mArgv != 0)
    {
        for (int i = 0; i < mArgc; ++i)
        {
            
            delete mArgv[i];
        }
        delete mArgv;
        mArgc = 0;
        mArgv = 0;
    }
}

extern char ** environ;

HotQ::HqError HotQ::PosixProcess::Start(int argc, const char * argv[])
{
    HotQ::HqError ret = HotQ::ERR_OK;
    InitializeArgv(argc, argv);
    //clear errno
    errno = 0;
    int posixRet = posix_spawnp(&mPid, mProcessName, NULL, NULL, mArgv, environ); 
    DestroyArgv();//no need anymore
    if (errno != 0)
    {
        ret = HotQ::ERR_PROCESS_INIT;
    }
    return ret;
}

HotQ::HqError HotQ::PosixProcess::Kill()
{
    HotQ::HqError ret = HotQ::ERR_OK;
    /*
     * Not implemented yet!
     * */
    return ret;
}

HotQ::HqError HotQ::PosixProcess::Join(int * exitVal)
{
    int status = 0;
    siginfo_t infop;
    int returnValue = waitid(P_PID, mPid, &infop, WEXITED | WSTOPPED);
    if (exitVal != 0)
    {
        *exitVal = infop.si_status;
    }
    return returnValue == 0 ? HotQ::ERR_OK : HotQ::ERR_PROCESS_WAIT;
}

HotQ::HqProcess * HotQ::CreateHotQProcess(const char * executablePath)
{
    return new HotQ::PosixProcess(executablePath);
}
