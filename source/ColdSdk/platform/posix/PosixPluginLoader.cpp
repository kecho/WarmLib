#include "ColdPluginImport.h"
#include "ColdPlugin.h"

#include <dlfcn.h>
#include <stdio.h>

typedef ColdSdk::ColdPlugin*(*export_plugin)();

class PosixColdSdkPlugin : public ColdSdk::ColdPlugin
{
public:
    PosixColdSdkPlugin (void * handle, ColdSdk::ColdPlugin * implementation );
    virtual ~PosixColdSdkPlugin ();
    virtual int PluginMain(int inputBufferSize, const char * inputBuffer, int outputBufferSize, char * outputBuffer);
protected:
    ColdSdk::ColdPlugin * mImpl;
    void * mHandle;
};


PosixColdSdkPlugin::PosixColdSdkPlugin (void * handle, ColdSdk::ColdPlugin * implementation)
: mHandle(handle), mImpl(implementation)
{
}

PosixColdSdkPlugin::~PosixColdSdkPlugin()
{
    delete mImpl;
    dlclose(mHandle);
}

int PosixColdSdkPlugin::PluginMain(int inputBufferSize, const char * inputBuffer, int outputBufferSize, char * outputBuffer)
{
    return mImpl->PluginMain(inputBufferSize, inputBuffer, outputBufferSize, outputBuffer);
}

ColdSdk::Error ColdSdk::ImportColdPlugin(const char * dynamicLibraryFileName, ColdSdk::ColdPlugin ** pluginPtr)
{
    ColdSdk::ColdPlugin * p = 0;
    ColdSdk::Error ret = ColdSdk::ERR_OK;
    void * handle = dlopen(dynamicLibraryFileName, RTLD_NOW); 
    if (!handle)
    {
       ret = ColdSdk::ERR_LOADING_DLL;
    }
    else
    {
        export_plugin fnExportPlugin;
        fnExportPlugin = reinterpret_cast<export_plugin>(dlsym(handle, "ExportColdPlugin"));
        if (fnExportPlugin == 0)
        {
            dlclose(handle);
            ret = ColdSdk::ERR_INVALID_DLL;
        }
        else
        {
            ColdSdk::ColdPlugin * pluginImpl = fnExportPlugin();
            ColdSdk::ColdPlugin * wrapper = new PosixColdSdkPlugin(handle, pluginImpl);
            p = wrapper;
        }
    }
    *pluginPtr = p; 
    return ret;
}
