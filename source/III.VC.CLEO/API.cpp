#include "API.h"
#include "CustomScript.h"
#include "OpcodesSystem.h"
#include "CleoVersion.h"
#include "Game.h"

tScriptVar *CLEO_Script_GetParamsAddress()
{
    return game.Scripts.Params;
}

char *CLEO_Script_GetScriptSpaceAddress()
{
    return game.Scripts.Space;
}

unsigned CLEO_GetVersion()
{
    return CLEO_VERSION;
}

void CLEO_Script_Collect(CCustomScript* pScript, int numParams)
{
    pScript->Collect(numParams);
}

tScriptVar CLEO_Script_CollectNextWithoutIncreasingPC(CCustomScript* pScript, unsigned int ip)
{
    return pScript->CollectNextWithoutIncreasingPC(ip);
}

eParamType CLEO_Script_GetNextParamType(CCustomScript* pScript)
{
    return pScript->GetNextParamType();
}

void CLEO_Script_Store(CCustomScript* pScript, unsigned int numParams)
{
    pScript->Store(numParams);
}

void CLEO_Script_ReadShortString(CCustomScript* pScript, char* out)
{
    pScript->ReadShortString(out);
}

void CLEO_Script_UpdateCompareFlag(CCustomScript* pScript, bool result)
{
    pScript->UpdateCompareFlag(result);
}

tScriptVar *CLEO_Script_GetPointerToScriptVariable(CCustomScript* pScript)
{
    return pScript->GetPointerToScriptVariable();
}

void CLEO_Script_JumpTo(CCustomScript* pScript, int address)
{
    pScript->JumpTo(address);
}

bool CLEO_Opcode_RegisterOpcode(unsigned short id, Opcode func)
{
    return Opcodes::RegisterOpcode(id, func);
}
