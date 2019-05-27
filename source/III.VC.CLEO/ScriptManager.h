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
};

extern ScriptManager scriptMgr;
