#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "III.VC.CLEO.h"

#ifdef CLEO_VC
#pragma comment (lib, "../bin/VC.CLEO.lib")
#elif defined CLEO_III
#pragma comment (lib, "../bin/III.CLEO.lib")
#endif

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  5

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

tScriptVar *Params;

eOpcodeResult WINAPI OPCODE_0B20(CCustomScript *script)
/****************************************************************
Opcode Format
0B20=2,read_clipboard_data %1d% size %2d%
****************************************************************/
{
    CLEO_Script_Collect(script, 2);
    HANDLE ClipbData;
    if (OpenClipboard(0))
    {
        ClipbData = GetClipboardData(1u);
        if (ClipbData)
        {
            memcpy(Params[0].pVar, ClipbData, Params[1].nVar);
            CLEO_Script_UpdateCompareFlag(script, true);
        }
        CloseClipboard();
    }
    return OR_CONTINUE;
}

eOpcodeResult WINAPI OPCODE_0B21(CCustomScript *script)
/****************************************************************
Opcode Format
0B21=2,write_clipboard_data %1d% size %2d%
****************************************************************/
{
    CLEO_Script_Collect(script, 2);

    HGLOBAL hGl, hMem;
    void *Lock;

    if (OpenClipboard(0))
    {
        EmptyClipboard();
        hGl = GlobalAlloc(0x2042u, 0x800u);
        hMem = hGl;
        if (hGl)
        {
            Lock = GlobalLock(hGl);
            memcpy(Lock, Params[0].pVar, Params[1].nVar);
            GlobalUnlock(hMem);
            SetClipboardData(1u, hMem);
            CLEO_Script_UpdateCompareFlag(script, true);
        }
        CloseClipboard();
    }
    return OR_CONTINUE;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        if (CLEO_GetVersion() < CLEO_VERSION)
        {
            MessageBox(HWND_DESKTOP, "An incorrect version of CLEO was loaded.", "ClipboardControl.cleo", MB_ICONERROR);
            return FALSE;
        }

        Params = CLEO_Script_GetParamsAddress();
        CLEO_Opcode_RegisterOpcode(0x0B20, OPCODE_0B20);
        CLEO_Opcode_RegisterOpcode(0x0B21, OPCODE_0B21);
    }
    return TRUE;
}