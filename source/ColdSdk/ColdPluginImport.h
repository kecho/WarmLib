#ifndef __COLD_SDK_IMPORT__
#define __COLD_SDK_IMPORT__
#include "ColdPlugin.h"
namespace ColdSdk
{
    enum Error
    {
        ERR_OK,
        ERR_LOADING_DLL,
        ERR_INVALID_DLL,
        ERR_UNKNOWN
    };
    Error ImportColdPlugin(const char * dynamicLibraryFileName, ColdPlugin ** pluginPtr);
}

#endif

