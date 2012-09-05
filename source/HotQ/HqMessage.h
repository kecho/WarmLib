#ifndef __HOT_Q_MESSAGE_
#define __HOT_Q_MESSAGE_



namespace HotQ
{
    class HqMessage
    {
    public:
        HqMessage(){}
        virtual ~HqMessage(){}
        virtual void Serialize(char * outputBuffer, int size) const = 0;
        virtual void Read(const char * inputBuffer, int size) = 0;
    };

}

#endif
