#include "ColdPluginImport.h"
#include <Windows.h>

typedef ColdSdk::ColdPlugin * (*export_plugin)();

class WinPlugin : public ColdSdk::ColdPlugin
{
public:
	WinPlugin(HMODULE handle, ColdSdk::ColdPlugin * implementation);
	virtual ~WinPlugin();
    virtual int PluginMain(int inputBufferSize, const char * inputBuffer, int outputBufferSize, char * outputBuffer);
private:
	HMODULE mHandle;
	ColdSdk::ColdPlugin * mImpl;
};

WinPlugin::WinPlugin(HMODULE handle, ColdSdk::ColdPlugin * implementation)
:
mHandle(handle), mImpl(implementation)
{
}

WinPlugin::~WinPlugin()
{
	delete mImpl;
	FreeLibrary(mHandle);	
}


int WinPlugin::PluginMain(int inputBufferSize, const char * inputBuffer, int outputBufferSize, char * outputBuffer)
{
    return mImpl->PluginMain(inputBufferSize, inputBuffer, outputBufferSize, outputBuffer);
}


ColdSdk::Error ColdSdk::ImportColdPlugin(const char * dynamicLibraryFileName, ColdSdk::ColdPlugin ** pluginPtr)
{
	ColdSdk::Error ret = ColdSdk::ERR_OK;
	ColdSdk::ColdPlugin * p = 0;
	HMODULE pluginHandle = LoadLibrary(dynamicLibraryFileName);
	if (pluginHandle == NULL)
	{
		ret = ColdSdk::ERR_LOADING_DLL;
	}
	else
	{
		export_plugin fnExportPlugin = reinterpret_cast<export_plugin>(GetProcAddress(pluginHandle, "ExportColdPlugin"));
		if (fnExportPlugin == NULL)
		{
			FreeLibrary(pluginHandle);
			ret = ColdSdk::ERR_INVALID_DLL;
		}
		else
		{
			ColdSdk::ColdPlugin * plugin = fnExportPlugin();
			p = new WinPlugin(pluginHandle, plugin);

		}
	}
	*pluginPtr = p;
	return ret;
}
