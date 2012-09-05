#include <stdio.h>
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
    q->Reset();
    int d = -1;
    HqMessageImpl msg("hello world");
    while (true)
    {
        scanf("%d", &d);
        if (d == 0)
        {
            break;
        }
        printf("push message\n");        
        HqError err = q->Enqueue(msg);
        if (err != ERR_OK) printf("unable to push\n");
    }
    delete q;
}
