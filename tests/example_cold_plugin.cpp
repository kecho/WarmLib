#include "ColdSdk/ColdPluginExport.h"

#include <stdio.h>
#include <unistd.h>

struct Tt
{
    int a;
    int b;
    int c;
    int d;
};

class TestPlugin : public ColdSdk::ColdPlugin
{
public:
    TestPlugin() 
    {
    }
    virtual ~TestPlugin()
    {
    }
    virtual int PluginMain(int bufferSize, const char * buffer, int outputBufferSize, char * outputBuffer)
    {
        const Tt * t = reinterpret_cast<const Tt*>(buffer);
        printf("processing job %d %d %d %d\n", t->a, t->b, t->c, t->d);
        sleep(10);
        printf("done processing job\n");

        return 0;
    }
};

ColdSdk::ColdPlugin * ColdSdk::ExportColdPlugin()
{
    return new TestPlugin();
}

