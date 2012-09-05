#ifndef __COLD_PLUGIN_EXPORT__
#define __COLD_PLUGIN_EXPORT__

#include "ColdPlugin.h"

#if defined(_WIN32) || defined(_WIN64)
    #define DLL_EXPORT extern "C" __declspec(dllexport)
#else
    #define DLL_EXPORT extern "C"
#endif

namespace ColdSdk
{
	DLL_EXPORT ColdPlugin * ExportColdPlugin();
}

#endif
