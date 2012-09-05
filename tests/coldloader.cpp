#include <stdio.h>
#include "ColdSdk/ColdPluginImport.h"
#include "ColdSdk/ColdPlugin.h"

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        printf("usage: coldloader <cold-plugin-dll>\n");
        return 0;
    }
    
    printf("loading plugin ... %s\n", argv[1]);
    ColdSdk::ColdPlugin * plugin = 0;
    ColdSdk::Error err = ColdSdk::ImportColdPlugin(argv[1], &plugin);
    if (err == ColdSdk::ERR_OK && plugin)
    {
        printf("Running %p\n", plugin);
        plugin->PluginMain(12, "hello world", 0, 0);
    }
    else
    {
        printf("ERROR %d\n", err);
    }
    delete plugin;
}
