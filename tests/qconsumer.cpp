#include <stdio.h>

#if defined(_WIN32) || defined (_WIN64)
#include <Windows.h>
#define sleep(x) Sleep(x*1000)
#else
#include <unistd.h>
#endif

#include "HotQ/HotQ.h"
#include "HotQ/HqMessage.h"

using namespace HotQ;
class HqMessageImpl : public HotQ::HqMessage
{
public: 
	HqMessageImpl(){}
	HqMessageImpl(const char * msg)
	{
		sprintf(mBuff, "%s", msg);
	}
	virtual ~HqMessageImpl(){}
	
	virtual void Serialize(char * b, int sz) const
	{
		sprintf(b, "%s", mBuff);
	}

	virtual void Read(const char * b, int sz)
	{
		sprintf(mBuff, "%s", b);
	}
	const char * GetMessage() const {return mBuff;}
private:
	char mBuff[60];
	
};

int main()
{
    HotQInterface * q = CreateHotQ(90); 

    HqMessageImpl msg;
    //while (true)
    {
        HotQ::HqError err = q->Dequeue(msg, 10);
        if (err == HotQ::ERR_QUEUE_TIMEOUT)
        {
            printf("timed out waiting\n");
        }
        else if (err == HotQ::ERR_OK)
        {
            sleep(1);
            printf("received: '%s'\n", msg.GetMessage() );
        }
        else
        {
            printf("UNKNOWN ERROR!");
        }
    }
    delete q;
}
