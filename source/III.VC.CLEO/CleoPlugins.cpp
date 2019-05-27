#include "CleoPlugins.h"
#include "Log.h"

std::vector<CleoPlugin> CleoPlugins::plugins;

CleoPlugin::CleoPlugin(const char *pluginname, const char *librarypath)
{
    name = pluginname;
    this->module = LoadLibrary(librarypath);
}

CleoPlugin::~CleoPlugin()
{
    FreeLibrary(this->module);
}

void CleoPlugins::LoadPlugins()
{
    char oldDir[MAX_PATH]; // store the current directory
    GetCurrentDirectory(MAX_PATH, oldDir);

    auto FolderExists = [](LPCTSTR szPath) -> BOOL
    {
        DWORD dwAttrib = GetFileAttributes(szPath);
        return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    };

    if (!FolderExists("CLEO"))
    {
        char  moduleName[MAX_PATH];
        char* tempPointer;
        GetModuleFileName(NULL, moduleName, MAX_PATH);
        tempPointer = strrchr(moduleName, '\\');
        *tempPointer = '\0';
        SetCurrentDirectory(moduleName);
    }

    WIN32_FIND_DATA FindFileData;
    memset(&FindFileData, 0, sizeof(WIN32_FIND_DATA));
    HANDLE hFind = FindFirstFile("CLEO\\CLEO_PLUGINS\\*.cleo", &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                char filename[MAX_PATH];
                strcpy(filename, "CLEO\\CLEO_PLUGINS\\");
                strcat(filename, FindFileData.cFileName);
                plugins.emplace_back(FindFileData.cFileName, filename);
                CleoPlugin *plugin = &plugins.back();
                if (!plugin->module)
                {
                    LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Failed to load plugin %s", plugin->name);
                    plugins.pop_back();
                }
                else
                {
                    LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Loaded plugin %s", plugin->name);
                }
            }
        } while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
    SetCurrentDirectory(oldDir); // Reset the current directory
}

void CleoPlugins::UnloadPlugins()
{
    while (!plugins.empty())
    {
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Unloaded plugin %s", plugins.back().name.c_str());
        plugins.pop_back();
    }
}
