#pragma once
#include <Windows.h>
#include <string>
#include <vector>

struct CleoPlugin
{
    std::string name;
    HMODULE module;

    CleoPlugin(const char *pluginname, const char *librarypath);

    ~CleoPlugin();
};

class CleoPlugins
{
public:
    static void LoadPlugins();
    static void UnloadPlugins();

    static std::vector<CleoPlugin> plugins;
};