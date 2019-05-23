#pragma once
#include "CustomScript.h"
#include "OpcodesSystem.h"
#include "CleoVersion.h"
#include "Game.h"

#define CLEOAPI __declspec(dllexport)

extern "C"
{
    CLEOAPI void CLEO_Script_Collect(CCustomScript *pScript, int numParams);
    CLEOAPI tScriptVar CLEO_Script_CollectNextWithoutIncreasingPC(CCustomScript *pScript, unsigned int ip);
    CLEOAPI eParamType CLEO_Script_GetNextParamType(CCustomScript *pScript);
    CLEOAPI void CLEO_Script_Store(CCustomScript *pScript, unsigned int numParams);
    CLEOAPI void CLEO_Script_ReadShortString(CCustomScript *pScript, char *out);
    CLEOAPI void CLEO_Script_UpdateCompareFlag(CCustomScript *pScript, bool result);
    CLEOAPI tScriptVar *CLEO_Script_GetPointerToScriptVariable(CCustomScript *pScript);
    CLEOAPI void CLEO_Script_JumpTo(CCustomScript *pScript, int address);

    CLEOAPI tScriptVar *CLEO_Script_GetParamsAddress();

    CLEOAPI bool CLEO_Opcode_RegisterOpcode(unsigned short id, Opcode func);

    CLEOAPI unsigned CLEO_GetVersion();
}
