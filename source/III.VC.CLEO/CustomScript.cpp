#include "CustomScript.h"
#include "Game.h"
#include "OpcodesSystem.h"
#include <Windows.h>
#include <cstdio>
#include "ScriptManager.h"
#include "Log.h"

void CCustomScript::Init()
{
    m_pNext = nullptr;
    m_pPrev = nullptr;
    strcpy(this->m_acName, "noname");
    m_dwIp = 0;
    std::fill(std::begin(m_aGosubAddr), std::end(m_aGosubAddr), 0);
    m_nCurrentGosub = 0;
    m_nScriptType = SCRIPT_TYPE_DEFAULT;
    std::fill(std::begin(m_aLVarsForSave), std::end(m_aLVarsForSave), tScriptVar{ 0 });
    m_bIsActive = false;
    m_bCondResult = false;
    m_bIsMission = false;
    m_bAwake = false;
    m_dwWakeTime = 0;
    m_wIfOp = 0;
    m_bNotFlag = false;
    m_bDeathArrestCheckEnabled = true;
    m_bWastedOrBusted = false;
    m_bMissionFlag = false;
    _pad = 0;

    m_nLastPedSearchIndex = 0;
    m_nLastVehicleSearchIndex = 0;
    m_nLastObjectSearchIndex = 0;

    m_Errors.m_bFileNotFound = false;
    m_Errors.m_bFileSeekError = false;
    m_Errors.m_bInternalError = false;
    m_Errors.m_bEmptyFile = false;
    m_Errors.m_bAllocationFailed = false;
    m_Errors.m_bEofReached = false;
    m_dwBaseIp = 0;

#ifdef CLEO_VC
    std::fill(std::begin(m_aLargeLVars), std::end(m_aLargeLVars), tScriptVar{ 0 });
#endif
}

bool CCustomScript::Loaded()
{
    return !(
        this->m_Errors.m_bAllocationFailed ||
        this->m_Errors.m_bEmptyFile ||
        this->m_Errors.m_bEofReached ||
        this->m_Errors.m_bFileNotFound ||
        this->m_Errors.m_bFileSeekError ||
        this->m_Errors.m_bInternalError);
}

CCustomScript::CCustomScript(const char *filepath)
{
    this->Init();
    this->m_nScriptType = SCRIPT_TYPE_CUSTOM;
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"File Not Found\"", filepath);
        this->m_Errors.m_bFileNotFound = true;
        return;
    }
    if (fseek(file, 0, SEEK_END))
    {
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"File Seek Error\"", filepath);
        fclose(file);
        this->m_Errors.m_bFileSeekError = true;
        return;
    }
    unsigned int filesize = ftell(file);
    if (!filesize)
    {
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"Empty File\"", filepath);
        fclose(file);
        this->m_Errors.m_bEmptyFile = true;
        return;
    }
    if (fseek(file, 0, SEEK_SET))
    {
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"File Seek Error\"", filepath);
        fclose(file);
        this->m_Errors.m_bFileSeekError = true;
        return;
    }

    try
    {
        this->m_vecCodeData.resize(filesize);
    }
    catch (std::bad_alloc *)
    {
        LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Failed to allocate memory for script: \"%s\", %d", filepath, filesize);
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"Allocation Failed\"", filepath);
        fclose(file);
        this->m_Errors.m_bAllocationFailed = true;
        return;
    }

    this->m_dwBaseIp = (unsigned int)this->m_vecCodeData.data() - (unsigned int)game.Scripts.Space;
    this->m_bMissionFlag = 0;
    this->m_dwIp = this->m_dwBaseIp;
    if (!fread(this->m_vecCodeData.data(), 1, filesize, file))
    {
        if (ferror(file))
        {
            LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"Internal Error\"", filepath);
            LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Deleted script memory: \"%s\"", filepath);
            this->m_vecCodeData.clear();
            this->m_vecCodeData.shrink_to_fit();
            this->m_dwBaseIp = this->m_dwIp = 0;
            this->m_Errors.m_bInternalError = true;
            fclose(file);
            return;
        }
        else if (feof(file))
        {
            LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"EOF Reached\"", filepath);
            LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Deleted script memory: \"%s\"", filepath);
            this->m_vecCodeData.clear();
            this->m_vecCodeData.shrink_to_fit();
            this->m_dwBaseIp = this->m_dwIp = 0;
            this->m_Errors.m_bEofReached = true;
            fclose(file);
            return;
        }
    }
    if (fclose(file) == -1)
    {
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"Internal Error\"", filepath);
        LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Deleted script memory: \"%s\"", filepath);
        this->m_vecCodeData.clear();
        this->m_vecCodeData.shrink_to_fit();
        this->m_dwBaseIp = this->m_dwIp = 0;
        this->m_Errors.m_bInternalError = true;
        fclose(file);
        return;
    }
    strncpy(this->m_acName, &strrchr(filepath, '\\')[1], 8);
    memset(&this->m_Errors, 0, 4);
    this->m_acName[7] = 0;
    LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Loaded script \"%s\" from \"%s\"", this->m_acName, filepath);
}

void CCustomScript::ReadShortString(char *out)
{
    strncpy(out, &game.Scripts.Space[this->m_dwIp], 7);
    out[7] = '\0';
    this->m_dwIp += 8;
}

void CCustomScript::JumpTo(int address)
{
    if (address >= 0)
        this->m_dwIp = address;
    else
    {
        if (this->m_nScriptType == SCRIPT_TYPE_CUSTOM)
            this->m_dwIp = this->m_dwBaseIp - address;
        else
        {
#ifdef CLEO_VC
            this->m_dwIp = 0x370E8 - address;
#elif defined CLEO_III
            this->m_dwIp = 0x20000 - address;
#endif
    }
}
    }

eParamType CCustomScript::GetNextParamType()
{
    return ((tParamType *)&game.Scripts.Space[this->m_dwIp])->type;
}

void CCustomScript::Collect(unsigned int numParams)
{
    this->Collect(&this->m_dwIp, numParams);
}

void CCustomScript::Collect(unsigned int *pIp, unsigned int numParams)
{
    for (unsigned int i = 0; i < numParams; i++)
    {
        tParamType *paramType = (tParamType *)&game.Scripts.Space[*pIp];
        *pIp += 1;

        switch (paramType->type)
        {
        case PARAM_TYPE_FLOAT:
#ifdef CLEO_VC
            game.Scripts.Params[i].nVar = *(int *)&game.Scripts.Space[*pIp];
            *pIp += 4;
            break;
#elif defined CLEO_III
            game.Scripts.Params[i].fVar = (float)(*(short *)&game.Scripts.Space[*pIp]) / 16.0f;
            *pIp += 2;
            break;
#endif
        case PARAM_TYPE_INT32:
            game.Scripts.Params[i].nVar = *(int *)&game.Scripts.Space[*pIp];
            *pIp += 4;
            break;
        case PARAM_TYPE_GVAR:
            game.Scripts.Params[i].nVar = *(int *)&game.Scripts.Space[*(unsigned short *)&game.Scripts.Space[*pIp]];
            *pIp += 2;
            break;
        case PARAM_TYPE_LVAR:
            game.Scripts.Params[i].nVar = this->m_aLargeLVars[*(unsigned short *)&game.Scripts.Space[*pIp]].nVar;
            *pIp += 2;
            break;
        case PARAM_TYPE_INT8:
            game.Scripts.Params[i].nVar = *(char *)&game.Scripts.Space[*pIp];
            *pIp += 1;
            break;
        case PARAM_TYPE_INT16:
            game.Scripts.Params[i].nVar = *(short *)&game.Scripts.Space[*pIp];
            *pIp += 2;
            break;
        case PARAM_TYPE_STRING:
            if (!paramType->processed)
            {
                unsigned char length = *(unsigned char *)&game.Scripts.Space[*pIp];
                *std::copy_n(&game.Scripts.Space[*pIp + 1], length, &game.Scripts.Space[*pIp]) = 0;
                paramType->processed = true;
            }

            game.Scripts.Params[i].cVar = &game.Scripts.Space[*pIp];
            *pIp += (strlen(&game.Scripts.Space[*pIp]) + 1);
            break;

        default:
            //default to short string, dangerous!! A short string leading with 0x8E(1000 1110) messing up.
            game.Scripts.Params[i].cVar = &game.Scripts.Space[*pIp - 1];
            *pIp += 7;
            break;
        }
    }
}

tScriptVar CCustomScript::CollectNextWithoutIncreasingPC(unsigned int ip)
{
    tScriptVar result;

    tParamType *paramType = (tParamType *)&game.Scripts.Space[ip];
    ip += 1;

    switch (paramType->type)
    {
    case PARAM_TYPE_FLOAT:
#ifdef CLEO_VC
        result.fVar = *(float *)&game.Scripts.Space[ip];
        break;

#elif defined CLEO_III
        {
            auto fParam = ((float)(*(short *)&game.Scripts.Space[ip]) / 16.0f);
            result.fVar = *(float*)&fParam;
            break;
    }
#endif
    case PARAM_TYPE_INT32:
        result.nVar = *(int *)&game.Scripts.Space[ip];
        break;

    case PARAM_TYPE_GVAR:
        result = *(tScriptVar *)&game.Scripts.Space[*(unsigned short *)&game.Scripts.Space[ip]];
        break;

    case PARAM_TYPE_LVAR:
        result = this->m_aLargeLVars[*(unsigned short *)&game.Scripts.Space[ip]];
        break;

    case PARAM_TYPE_INT8:
        result.nVar = *(char *)&game.Scripts.Space[ip];
        break;

    case PARAM_TYPE_INT16:
        result.nVar = *(short *)&game.Scripts.Space[ip];
        break;

    case PARAM_TYPE_STRING:
        if (!paramType->processed)
        {
            unsigned char length = *(unsigned char *)&game.Scripts.Space[ip];
            *std::copy_n(&game.Scripts.Space[ip + 1], length, &game.Scripts.Space[ip]) = 0;
            paramType->processed = true;
        }

        result.cVar = &game.Scripts.Space[ip + 1];
        break;

    default:
        //default to short string, dangerous!! A short string leading with 0x8E(1000 1110) messing up.
        result.cVar = &game.Scripts.Space[ip - 1];
        break;

}

    return result;
    }

void CCustomScript::Store(unsigned int numParams)
{
    game.Scripts.StoreParameters(this, &this->m_dwIp, numParams);
}

void CCustomScript::UpdateCompareFlag(bool result)
{
    game.Scripts.UpdateCompareFlag(this, result);
}

tScriptVar *CCustomScript::GetPointerToScriptVariable()
{
    return game.Scripts.GetPointerToScriptVariable(this, &this->m_dwIp, 1);
}

eOpcodeResult CCustomScript::ProcessOneCommand()
{
    *game.Scripts.pNumOpcodesExecuted += 1;
    unsigned short id = *(unsigned short *)&game.Scripts.Space[this->m_dwIp] & 0x7FFF;
    if (*(unsigned short *)&game.Scripts.Space[this->m_dwIp] & 0x8000)
        this->m_bNotFlag = true;
    else
        this->m_bNotFlag = false;
    this->m_dwIp += 2;
    // check for custom opcodes here
    if (Opcodes::functions[id])
    {
        // call custom opcode
        LOGL(LOG_PRIORITY_OPCODE_ID, "%s custom opcode %04X", this->m_acName, id);
        return Opcodes::functions[id](this);
    }
    else if (id >= CUSTOM_OPCODE_START_ID)
    {
        LOGL(LOG_PRIORITY_ALWAYS, "Error (incorrect opcode): %s, %04X", this->m_acName, id);
        Error("Incorrect opcode ID: %04X", id);
        return OR_UNDEFINED;
    }
    // call default opcode
    LOGL(LOG_PRIORITY_OPCODE_ID, "%s opcode %04X", this->m_acName, id);
    eOpcodeResult result = game.Scripts.OpcodeHandlers[id / 100](this, id);
    return result;
}

void CCustomScript::InitWrap(CCustomScript *script)
{
    script->Init();
}

eOpcodeResult CCustomScript::ProcessOneCommandWrap(CCustomScript *script)
{
    return script->ProcessOneCommand();
}

void CCustomScript::CollectWrap(CCustomScript *script, int, unsigned int *pIp, unsigned int numParams)
{
    script->Collect(pIp, numParams);
}

tScriptVar CCustomScript::CollectScriptNextParameterWithoutIncreasingPCWrap(CCustomScript *script, int, unsigned int ip)
{
    return script->CollectNextWithoutIncreasingPC(ip);
}
