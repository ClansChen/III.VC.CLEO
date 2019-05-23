#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "III.VC.CLEO.h"

#ifdef CLEO_VC
#pragma comment (lib, "../bin/VC.CLEO.lib")
#elif defined CLEO_III
#pragma comment (lib, "../bin/III.CLEO.lib")
#endif

#define OPCODE_INTOP_AND			0x0B10
#define OPCODE_INTOP_OR				0x0B11
#define OPCODE_INTOP_XOR			0x0B12
#define OPCODE_INTOP_NOT			0x0B13
#define OPCODE_INTOP_MOD			0x0B14
#define OPCODE_INTOP_SHR			0x0B15
#define OPCODE_INTOP_SHL			0x0B16
#define SCR_INTOP_AND		 		0x0B17
#define SCR_INTOP_OR				0x0B18
#define SCR_INTOP_XOR				0x0B19
#define SCR_INTOP_NOT				0x0B1A
#define SCR_INTOP_MOD				0x0B1B
#define SCR_INTOP_SHR				0x0B1C
#define SCR_INTOP_SHL				0x0B1D

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  5

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

tScriptVar *Params;

eOpcodeResult WINAPI Script_IntOp_AND(CCustomScript *script)
/****************************************************************
Opcode Format
0B10=3,%3d% = %1d% AND %2d%
****************************************************************/
{
    CLEO_Script_Collect(script, 2);
    Params[0].nVar = Params[0].nVar & Params[1].nVar;
    CLEO_Script_Store(script, 1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_OR(CCustomScript *script)
/****************************************************************
Opcode Format
0B11=3,%3d% = %1d% OR %2d%
****************************************************************/
{
    CLEO_Script_Collect(script, 2);
    Params[0].nVar = Params[0].nVar | Params[1].nVar;
    CLEO_Script_Store(script, 1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_XOR(CCustomScript *script)
/****************************************************************
Opcode Format
0B12=3,%3d% = %1d% XOR %2d%
****************************************************************/
{
    CLEO_Script_Collect(script, 2);
    Params[0].nVar = Params[0].nVar ^ Params[1].nVar;
    CLEO_Script_Store(script, 1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_NOT(CCustomScript *script)
/****************************************************************
Opcode Format
0B13=2,%2d% = NOT %1d%
****************************************************************/
{
    CLEO_Script_Collect(script, 1);
    Params[0].nVar = ~Params[0].nVar;
    CLEO_Script_Store(script, 1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_MOD(CCustomScript *script)
/****************************************************************
Opcode Format
0B14=3,%3d% = %1d% MOD %2d%
****************************************************************/
{
    CLEO_Script_Collect(script, 2);
    Params[0].nVar = Params[0].nVar % Params[1].nVar;
    CLEO_Script_Store(script, 1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_SHR(CCustomScript *script)
/****************************************************************
Opcode Format
0B15=3,%3d% = %1d% SHR %2d%
****************************************************************/
{
    CLEO_Script_Collect(script, 2);
    Params[0].nVar = Params[0].nVar >> Params[1].nVar;
    CLEO_Script_Store(script, 1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_SHL(CCustomScript *script)
/****************************************************************
Opcode Format
0B16=3,%3d% = %1d% SHL %2d%
****************************************************************/
{
    CLEO_Script_Collect(script, 2);
    Params[0].nVar = Params[0].nVar << Params[1].nVar;
    CLEO_Script_Store(script, 1);
    return OR_CONTINUE;
}

/****************************************************************
Now do them as real operators...
*****************************************************************/

eOpcodeResult WINAPI Scr_IntOp_AND(CCustomScript *script)
/****************************************************************
Opcode Format
0B17=2,%1d% &= %2d%
****************************************************************/
{
    tScriptVar* op = CLEO_Script_GetPointerToScriptVariable(script);
    CLEO_Script_Collect(script, 1);
    int val = Params[0].nVar;
    op->nVar &= val;
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_OR(CCustomScript *script)
/****************************************************************
Opcode Format
0B18=2,%1d% |= %2d%
****************************************************************/
{
    tScriptVar* op = CLEO_Script_GetPointerToScriptVariable(script);
    CLEO_Script_Collect(script, 1);
    int val = Params[1].nVar;
    op->nVar |= val;
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_XOR(CCustomScript *script)
/****************************************************************
Opcode Format
0B19=2,%1d% ^= %2d%
****************************************************************/
{
    tScriptVar* op = CLEO_Script_GetPointerToScriptVariable(script);
    CLEO_Script_Collect(script, 1);
    int val = Params[1].nVar;
    op->nVar ^= val;
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_NOT(CCustomScript *script)
/****************************************************************
Opcode Format
0B1A=1,~%1d%
****************************************************************/
{
    tScriptVar* op = CLEO_Script_GetPointerToScriptVariable(script);
    op->nVar = ~op->nVar;
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_MOD(CCustomScript *script)
/****************************************************************
Opcode Format
0B1B=2,%1d% %= %2d%
****************************************************************/
{
    tScriptVar* op = CLEO_Script_GetPointerToScriptVariable(script);
    CLEO_Script_Collect(script, 1);
    int val = Params[1].nVar;
    op->nVar %= val;
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_SHR(CCustomScript *script)
/****************************************************************
Opcode Format
0B1C=2,%1d% >>= %2d%
****************************************************************/
{
    tScriptVar* op = CLEO_Script_GetPointerToScriptVariable(script);
    CLEO_Script_Collect(script, 1);
    int val = Params[1].nVar;
    op->nVar >>= val;
    return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_SHL(CCustomScript *script)
/****************************************************************
Opcode Format
0B1D=2,%1d% <<= %2d%
****************************************************************/
{
    tScriptVar* op = CLEO_Script_GetPointerToScriptVariable(script);
    CLEO_Script_Collect(script, 1);
    int val = Params[1].nVar;
    op->nVar <<= val;
    return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        if (CLEO_GetVersion() < CLEO_VERSION)
        {
            MessageBox(HWND_DESKTOP, "An incorrect version of CLEO was loaded.", "IntOperations.cleo", MB_ICONERROR);
            return FALSE;
        }

        Params = CLEO_Script_GetParamsAddress();
        CLEO_Opcode_RegisterOpcode(OPCODE_INTOP_AND, Script_IntOp_AND);
        CLEO_Opcode_RegisterOpcode(OPCODE_INTOP_OR, Script_IntOp_OR);
        CLEO_Opcode_RegisterOpcode(OPCODE_INTOP_XOR, Script_IntOp_XOR);
        CLEO_Opcode_RegisterOpcode(OPCODE_INTOP_NOT, Script_IntOp_NOT);
        CLEO_Opcode_RegisterOpcode(OPCODE_INTOP_MOD, Script_IntOp_MOD);
        CLEO_Opcode_RegisterOpcode(OPCODE_INTOP_SHR, Script_IntOp_SHR);
        CLEO_Opcode_RegisterOpcode(OPCODE_INTOP_SHL, Script_IntOp_SHL);
        CLEO_Opcode_RegisterOpcode(SCR_INTOP_AND, Scr_IntOp_AND);
        CLEO_Opcode_RegisterOpcode(SCR_INTOP_OR, Scr_IntOp_OR);
        CLEO_Opcode_RegisterOpcode(SCR_INTOP_XOR, Scr_IntOp_XOR);
        CLEO_Opcode_RegisterOpcode(SCR_INTOP_NOT, Scr_IntOp_NOT);
        CLEO_Opcode_RegisterOpcode(SCR_INTOP_MOD, Scr_IntOp_MOD);
        CLEO_Opcode_RegisterOpcode(SCR_INTOP_SHR, Scr_IntOp_SHR);
        CLEO_Opcode_RegisterOpcode(SCR_INTOP_SHL, Scr_IntOp_SHL);
    }
    return TRUE;
}