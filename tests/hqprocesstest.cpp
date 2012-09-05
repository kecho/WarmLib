#include "HotQ/HqProcess.h"
#include <stdio.h>

int main(int argc, char ** argv)
{
    using namespace HotQ;
    HqProcess * p = CreateHotQProcess("./threadtest");
    int pargc = 1;
    const char *pargv[1] = { "nothing" };
    printf("starting processes...\n");
    HqError err = p->Start(pargc, pargv);
    if (err != ERR_OK)
    {
        printf("Cant start ./threadtest\n");
    }
    p->Join();
    printf("done\n");
    delete p;
}
