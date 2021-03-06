﻿#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "III.VC.CLEO.h"

#ifdef CLEO_VC
#pragma comment (lib, "../bin/VC.CLEO.lib")
#elif defined CLEO_III
#pragma comment (lib, "../bin/III.CLEO.lib")
#endif

#define OPCODE_INIFILE_GET_INT		0x0AF0
#define OPCODE_INIFILE_WRITE_INT	0x0AF1  
#define OPCODE_INIFILE_GET_FLOAT	0x0AF2  
#define	OPCODE_INIFILE_WRITE_FLOAT	0x0AF3 
#define	OPCODE_INIFILE_READ_STRING	0x0AF4 
#define	OPCODE_INIFILE_WRITE_STRING	0x0AF5

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  5

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

tScriptVar *Params;

char* MakeFullPath(char *path, char *dst)
{
    if (path[1] != ':')
    {
        //get current working directory
        GetCurrentDirectory(MAX_PATH, dst);
        strcat(dst, "\\");
        strcat(dst, path);
    }
    else
    {
        strcpy(dst, path);
    }
    return dst;
}

eOpcodeResult WINAPI Script_InifileGetInt(CCustomScript* script)
/****************************************************************
Opcode Format
0AF0=4,%4d% = get_int_from_ini_file %1s% section %2s% key %3s%
****************************************************************/
{
    CLEO_Script_Collect(script, 3);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    int result;

    strcpy(path, Params[0].cVar);
    strcpy(sectionName, Params[1].cVar);
    strcpy(key, Params[2].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    result = GetPrivateProfileInt(sectionName, key, 0x80000000, iniPath);
    Params[0].nVar = result;
    CLEO_Script_Store(script, 1);
    CLEO_Script_UpdateCompareFlag(script, result != 0x80000000);

    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_InifileWriteInt(CCustomScript* script)
/****************************************************************
Opcode Format
0AF1=4,write_int %1d% to_ini_file %2s% section %3s% key %4s%
****************************************************************/
{
    CLEO_Script_Collect(script, 4);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    DWORD value;
    char strValue[100];
    BOOL result;

    value = Params[0].nVar;
    strcpy(path, Params[1].cVar);
    strcpy(sectionName, Params[2].cVar);
    strcpy(key, Params[3].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    result = WritePrivateProfileString(sectionName, key, _itoa(value, strValue, 10), iniPath);
    CLEO_Script_UpdateCompareFlag(script, result != 0);

    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_InifileGetFloat(CCustomScript* script)
/****************************************************************
Opcode Format
0AF2=4,%4d% = get_float_from_ini_file %1s% section %2s% key %3s%
****************************************************************/
{
    CLEO_Script_Collect(script, 3);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    float value = 0.0f;
    char strValue[100];
    BOOL result;

    strcpy(path, Params[0].cVar);
    strcpy(sectionName, Params[1].cVar);
    strcpy(key, Params[2].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    result = GetPrivateProfileString(sectionName, key, NULL, strValue, sizeof(strValue), iniPath);
    if (result)
    {
        value = (float)atof(strValue);
        Params[0].fVar = value;
        CLEO_Script_Store(script, 1);
    }
    else
    {
        Params[0].fVar = 0.0f;
        CLEO_Script_Store(script, 1);
    }

    CLEO_Script_UpdateCompareFlag(script, result != 0);

    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_InifileWriteFloat(CCustomScript* script)
/****************************************************************
Opcode Format
0AF3=4,write_float %1d% to_ini_file %2s% section %3s% key %4s%
****************************************************************/
{
    CLEO_Script_Collect(script, 4);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    float value;
    char strValue[100];
    BOOL result;

    value = Params[0].fVar;
    strcpy(path, Params[1].cVar);
    strcpy(sectionName, Params[2].cVar);
    strcpy(key, Params[3].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    sprintf(strValue, "%g", value);

    result = WritePrivateProfileString(sectionName, key, strValue, iniPath);
    CLEO_Script_UpdateCompareFlag(script, result != 0);

    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_InifileReadString(CCustomScript* script)
/****************************************************************
Opcode Format
0AF4=4,%4d% = read_string_from_ini_file %1s% section %2s% key %3s%
****************************************************************/
{
    CLEO_Script_Collect(script, 3);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    char strValue[100];
    BOOL result;

    strcpy(path, Params[0].cVar);
    strcpy(sectionName, Params[1].cVar);
    strcpy(key, Params[2].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    result = GetPrivateProfileString(sectionName, key, NULL, strValue, sizeof(strValue), iniPath);

    strcpy(Params[0].cVar, strValue);
    CLEO_Script_Store(script, 1);

    CLEO_Script_UpdateCompareFlag(script, result != 0);

    return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_InifileWriteString(CCustomScript* script)
/****************************************************************
Opcode Format
0AF5=4,write_string %1s% to_ini_file %2s% section %3s% key %4s%
****************************************************************/
{
    CLEO_Script_Collect(script, 4);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    char strValue[100];
    BOOL result;

    strcpy(strValue, Params[0].cVar);
    strcpy(path, Params[1].cVar);
    strcpy(sectionName, Params[2].cVar);
    strcpy(key, Params[3].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    result = WritePrivateProfileString(sectionName, key, strValue, iniPath);

    CLEO_Script_UpdateCompareFlag(script, result != 0);

    return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        if (CLEO_GetVersion() < CLEO_VERSION)
        {
            MessageBox(HWND_DESKTOP, "An incorrect version of CLEO was loaded.", "IniFiles.cleo", MB_ICONERROR);
            return FALSE;
        }

        Params = CLEO_Script_GetParamsAddress();
        CLEO_Opcode_RegisterOpcode(OPCODE_INIFILE_GET_INT, Script_InifileGetInt);
        CLEO_Opcode_RegisterOpcode(OPCODE_INIFILE_WRITE_INT, Script_InifileWriteInt);
        CLEO_Opcode_RegisterOpcode(OPCODE_INIFILE_GET_FLOAT, Script_InifileGetFloat);
        CLEO_Opcode_RegisterOpcode(OPCODE_INIFILE_WRITE_FLOAT, Script_InifileWriteFloat);
        CLEO_Opcode_RegisterOpcode(OPCODE_INIFILE_READ_STRING, Script_InifileReadString);
        CLEO_Opcode_RegisterOpcode(OPCODE_INIFILE_WRITE_STRING, Script_InifileWriteString);
    }
    return TRUE;
}