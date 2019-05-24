#pragma once

#include "CustomScript.h"
#include <list>

class ScriptManager
{
public:
    CCustomScript gameScripts[128];

    std::list<CCustomScript> CustomScripts;

    CCustomScript *FindScriptByName(const char *name);

    CCustomScript *TerminateScriptByName(const char *name);

    void TerminateScriptByPointer(CCustomScript *pScript);

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
