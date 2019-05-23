#include "ScriptManager.h"
#include <Windows.h>
#include "Log.h"
#include "Game.h"

ScriptManager scriptMgr;

ScriptManager::ScriptManager()
{
    this->pCustomScripts = NULL;
    this->numLoadedCustomScripts = 0;
}

CCustomScript * ScriptManager::FindScriptByName(const char * name)
{

    return nullptr;
}

void ScriptManager::TerminateScriptByName(const char * name)
{

}

void ScriptManager::LoadScripts()
{
    WIN32_FIND_DATA FindFileData;
    memset(&FindFileData, 0, sizeof(WIN32_FIND_DATA));
    HANDLE hFind = FindFirstFile("cleo\\*.cs", &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                char filename[MAX_PATH];
                strcpy(filename, "cleo\\");
                strcat(filename, FindFileData.cFileName);
                CCustomScript *newScript = new CCustomScript(filename);
                if (newScript)
                {
                    if (newScript->Loaded())
                    {
                        newScript->AddToCustomList(&this->pCustomScripts);
                        game.Scripts.AddScriptToList(newScript, game.Scripts.pActiveScriptsList);
                        newScript->m_bIsActive = true;
                        numLoadedCustomScripts++;
                    }
                    else
                    {
                        delete newScript;
                        LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Failed to allocate custom script \"%s\"", filename);
                    }
                }
            }
        } while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
}

void ScriptManager::UnloadScripts()
{
    CCustomScript *script = this->pCustomScripts;
    while (script)
    {
        game.Scripts.RemoveScriptFromList(script, game.Scripts.pActiveScriptsList);
        scriptMgr.numLoadedCustomScripts--;
        CCustomScript *next = script->m_pNextCustom;
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Unloading custom script \"%s\"", script->m_acName);
        delete script;
        script = next;
    }
    this->pCustomScripts = NULL;
    this->ReleaseScriptsMemory();
}

char *ScriptManager::AllocateMemoryForScript(const char *scriptName, unsigned int size)
{
    char *data = new char[size];
    if (data)
    {
        this->scriptMemory.push_back(data);
        LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Allocated memory for script: \"%s\", %d", scriptName, size);
    }
    else
    {
        LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Failed to allocate memory for script: \"%s\", %d", scriptName, size);
    }
    return data;
}

void ScriptManager::DeleteScriptMemory(const char *scriptName, char *data)
{
    for (auto i = this->scriptMemory.begin(); i != this->scriptMemory.end(); i++)
    {
        if ((*i) == data)
        {
            this->scriptMemory.erase(i);
            delete[] data;
            LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Deleted script memory: \"%s\", %d", scriptName, data);
            return;
        }
    }
    LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Trying to delete script memory, script not found: \"%s\", %d", scriptName, data);
}

void ScriptManager::ReleaseScriptsMemory()
{
    for (auto i = this->scriptMemory.begin(); i != this->scriptMemory.end(); i++)
        delete[](*i);
    this->scriptMemory.clear();
    LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Released all scripts allocated memory");
}

void ScriptManager::DisableAllScripts()
{
    CCustomScript *script = this->pCustomScripts;
    while (script)
    {
        game.Scripts.RemoveScriptFromList(script, game.Scripts.pActiveScriptsList);
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Disabled script \"%s\"", script->m_acName);
        script = script->m_pNextCustom;
    }
}

void ScriptManager::EnableAllScripts()
{
    CCustomScript *script = this->pCustomScripts;
    while (script)
    {
        game.Scripts.AddScriptToList(script, game.Scripts.pActiveScriptsList);
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Enabled script \"%s\"", script->m_acName);
        script = script->m_pNextCustom;
    }
}

void ScriptManager::InitialiseScript(CCustomScript *script)
{
    script->Init();
}

eOpcodeResult ScriptManager::ProcessScriptCommand(CCustomScript *script)
{
    return script->ProcessOneCommand();
}

void ScriptManager::CollectScriptParameters(CCustomScript *script, int, unsigned int *pIp, unsigned int numParams)
{
    script->Collect(pIp, numParams);
}

tScriptVar ScriptManager::CollectScriptNextParameterWithoutIncreasingPC(CCustomScript *script, int, unsigned int ip)
{
    return script->CollectNextWithoutIncreasingPC(ip);
}