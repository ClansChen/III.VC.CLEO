#pragma once

#include "CustomScript.h"
#include <list>

class ScriptManager
{
public:
    CCustomScript gameScripts[128];

    std::list<char *> scriptMemory;

    CCustomScript *pCustomScripts;

    unsigned int numLoadedCustomScripts;

    ScriptManager();

    CCustomScript *FindScriptByName(const char *name);

    void TerminateScriptByName(const char *name);

    char *AllocateMemoryForScript(const char *scriptName, unsigned int size);

    void DeleteScriptMemory(const char *scriptName, char *data);

    void ReleaseScriptsMemory();

    void LoadScripts();

    void UnloadScripts();

    void DisableAllScripts();

    void EnableAllScripts();

    static void __fastcall InitialiseScript(CCustomScript *script);

    static eOpcodeResult __fastcall ProcessScriptCommand(CCustomScript *script);

    static void __fastcall CollectScriptParameters(CCustomScript *script, int, unsigned int *pIp, unsigned int numParams);

    static tScriptVar __fastcall CollectScriptNextParameterWithoutIncreasingPC(CCustomScript *script, int, unsigned int ip);
};

extern ScriptManager scriptMgr;