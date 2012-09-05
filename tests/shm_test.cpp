#include "HotQ/HotQ.h"
#include "HotQ/HqShm.h"
#include <stdio.h>

int start(HotQ::HqShm * shm);
void print_shm (int * c, int sz);
void inc (int * c, int sz);

int main ()
{
    HotQ::HqShm * shm = HotQ::CreateHotQShm(30, 20 * sizeof(int));
    int * c = static_cast<int*>(shm->Get());
    start(shm);
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
            inc(c, 20);
        }
        else
        {
            print_shm(c, 20);
        }
       
    }
    delete shm;
}

int start (HotQ::HqShm * shm)
{
    int * c = static_cast<int*>(shm->Get());
    if (shm->IsOwner())
    {
        for (int i = 0; i < 20; ++i)
        {
            c[i] = i;
        }
    }
}

void print_shm (int * c, int sz)
{
    for (int i = 0; i < sz; ++i)
    {
        printf("%s%i%s", i == 0 ? "[" : "", c[i], i == sz -1 ? "]" : ", ");
    }
}

void inc (int * c, int sz)
{
    for (int i = 0; i < sz; ++i)
    {
        c[i]++;
    }
}


