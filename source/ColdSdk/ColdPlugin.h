#ifndef __COLD_PLUGIN__
#define __COLD_PLUGIN__

namespace ColdSdk
{
    class ColdPlugin
    {
    public:
        ColdPlugin(){}
        virtual ~ColdPlugin(){}
        virtual int PluginMain(int inputBufferSize, const char * inputBuffer, int outputBufferSize, char * outputBuffer) = 0;
    };
}

#endif
