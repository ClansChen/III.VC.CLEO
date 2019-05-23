#pragma once

#define CLEOAPI __declspec(dllimport)

class CCustomScript;

enum eScriptType :unsigned short
{
    SCRIPT_TYPE_DEFAULT = 0,
    SCRIPT_TYPE_CUSTOM = 1
};

enum eOpcodeResult :char
{
    OR_CONTINUE = 0,
    OR_TERMINATE = 1,
    OR_UNDEFINED = -1
};

union tScriptVar
{
    int nVar;
    float fVar;
    char *cVar;
    void *pVar;
};

enum eParamType :unsigned char
{
    PARAM_TYPE_END_OF_PARAMS = 0,
    PARAM_TYPE_INT32 = 1,
    PARAM_TYPE_GVAR = 2,
    PARAM_TYPE_LVAR = 3,
    PARAM_TYPE_INT8 = 4,
    PARAM_TYPE_INT16 = 5,
    PARAM_TYPE_FLOAT = 6,
    PARAM_TYPE_STRING = 14
};

struct tParamType
{
    eParamType type : 7; // eParamType
    bool processed : 1; // did we process long string already
};

typedef eOpcodeResult(__stdcall* Opcode)(CCustomScript *);
typedef eOpcodeResult(__thiscall* OpcodeHandler)(CCustomScript *, int);

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
