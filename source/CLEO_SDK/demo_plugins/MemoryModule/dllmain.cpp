#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "MemoryModule.h"

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

//0BA2=2,%2h% = memory_load_library %1s% ; IF and SET
eOpcodeResult WINAPI OPCODE_0BA2(CCustomScript *script)
{
    CLEO_Script_Collect(script, 1);
    auto libHandle = MemoryLoadLibrary(Params[0].cVar);
    Params[0].pVar = libHandle;
    CLEO_Script_Store(script, 1);
    CLEO_Script_UpdateCompareFlag(script, libHandle != nullptr);
    return OR_CONTINUE;
}

//0BA3=1, memory_free_library %1h%
eOpcodeResult WINAPI OPCODE_0BA3(CCustomScript *script)
{
    CLEO_Script_Collect(script, 1);
    HMEMORYMODULE libHandle;
    libHandle = (HMEMORYMODULE)Params[0].pVar;
    MemoryFreeLibrary(libHandle);
    return OR_CONTINUE;
}

//0BA4=3,%3d% = memory_get_proc_address %1s% library %2d% ; IF and SET
eOpcodeResult WINAPI OPCODE_0BA4(CCustomScript *script)
{
    CLEO_Script_Collect(script, 2);
    const char *funcName = Params[0].cVar;
    HMEMORYMODULE libHandle;
    libHandle = (HMEMORYMODULE)Params[1].pVar;
    void *funcAddr = (void *)MemoryGetProcAddress(libHandle, funcName);
    Params[0].pVar = funcAddr;
    CLEO_Script_Store(script, 1);
    CLEO_Script_UpdateCompareFlag(script, funcAddr != nullptr);
    return OR_CONTINUE;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        if (CLEO_GetVersion() < CLEO_VERSION)
        {
            MessageBox(HWND_DESKTOP, "An incorrect version of CLEO was loaded.", "MemoryModule.cleo", MB_ICONERROR);
            return FALSE;
        }

        Params = CLEO_Script_GetParamsAddress();
        CLEO_Opcode_RegisterOpcode(0x0BA2, OPCODE_0BA2);
        CLEO_Opcode_RegisterOpcode(0x0BA3, OPCODE_0BA3);
        CLEO_Opcode_RegisterOpcode(0x0BA4, OPCODE_0BA4);
    }
    return TRUE;
}