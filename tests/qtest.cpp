#include "HotQ/HotQ.h"
#include "HotQ/HqProcessEvent.h"
#include <stdio.h>
using namespace HotQ;

int main ()
{
    HqProcessEvent * e = CreateHotQProcessEvent("KleberEvent3");
    int d = 0;
    while (true)
    {
        scanf("%d",&d); 
        if (d == 0)
        {
            break; 
        }
        else if (d == 1)
        {
            printf("Waiting on event\n");
            e->Wait();
            printf("Done waiting event\n");
        }
        else if (d == 2)
        {
            printf("Signaling event\n");
            e->Signal();
        }
    }
}
