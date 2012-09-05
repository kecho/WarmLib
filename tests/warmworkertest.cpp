#include <stdio.h>
#include "HotQ/HotQ.h" 
#include "WarmLib/WarmWorkerMessage.h"
using namespace HotQ;

int main(int argc, char ** argv)
{
    HotQInterface * qoutputQueue = CreateHotQ(100); 
    qoutputQueue->Reset();

    HotQInterface * qinputQueue = CreateHotQ(101);
    qinputQueue->Reset();

    while(true)
    {
        int d = 0;
        scanf("%d", &d); 
        switch(d)
        {
        case 0:
            return 0;
        case 1:
            {
                WarmLib::WarmWorkerMessage message(WarmLib::MESSAGE_JOB_START);
                HotQ::HqError err = qoutputQueue->Enqueue(message);
                printf("Message send, with success = %s\n", err == HotQ::ERR_OK ? "true" : "false");
            }
        }
    }


    delete qoutputQueue;
    delete qinputQueue;



}
