#include "ScriptManager.h"
#include <Windows.h>
#include "Log.h"
#include "Game.h"
#include <algorithm>
#include <iterator>

ScriptManager scriptMgr;

CCustomScript * ScriptManager::FindScriptByName(const char * name)
{
    for (auto it = CustomScripts.begin(); it != CustomScripts.end(); ++it)
    {
        CCustomScript *script = &*it;

        if (_stricmp(script->m_acName, name) == 0)
        {
            return script;
        }
    }

    for (auto it = std::begin(gameScripts); it != std::end(gameScripts); ++it)
    {
        CCustomScript *script = it;

        if (_stricmp(script->m_acName, name) == 0)
        {
            return script;
        }
    }

    return nullptr;
}

CCustomScript * ScriptManager::TerminateScriptByName(const char * name)
{
    for (auto it = CustomScripts.begin(); it != CustomScripts.end(); ++it)
    {
        CCustomScript *script = &*it;

        if (_stricmp(script->m_acName, name) == 0)
        {
            game.Scripts.RemoveScriptFromList(script, game.Scripts.pActiveScriptsList);
            LOGL(LOG_PRIORITY_OPCODE, "TERMINATE_NAMED_CUSTOM_THREAD: Terminating custom script with name \"%s\"", script->m_acName);
            CustomScripts.erase(it);
            return script;
        }
    }

    return nullptr;
}

void ScriptManager::TerminateScriptByPointer(CCustomScript * pScript)
{
    for (auto it = CustomScripts.begin(); it != CustomScripts.end(); ++it)
    {
        CCustomScript *script = &*it;
        if (script == pScript)
        {
            game.Scripts.RemoveScriptFromList(script, game.Scripts.pActiveScriptsList);
            LOGL(LOG_PRIORITY_OPCODE, "TERMINATE_CUSTOM_THREAD: Terminating custom script \"%s\"", script->m_acName);
            CustomScripts.erase(it);
            break;
        }
    }
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

                try
                {
                    CustomScripts.emplace_back(filename);
                }
                catch (std::bad_alloc *)
                {
                    LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Failed to allocate custom script \"%s\"", filename);
                    continue;
                }

                CCustomScript *newScript = &CustomScripts.back();
                if (newScript->Loaded())
                {
                    game.Scripts.AddScriptToList(newScript, game.Scripts.pActiveScriptsList);
                    newScript->m_bIsActive = true;
                }
                else
                {
                    CustomScripts.pop_back();
                    LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Failed to allocate custom script \"%s\"", filename);
                }
            }
        } while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
}

void ScriptManager::UnloadScripts()
{
    for (auto &script : CustomScripts)
    {
        game.Scripts.RemoveScriptFromList(&script, game.Scripts.pActiveScriptsList);
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Unloading custom script \"%s\"", script.m_acName);
    }

    CustomScripts.clear();

    LOGL(LOG_PRIORITY_MEMORY_ALLOCATION, "Released all scripts allocated memory");
}

void ScriptManager::DisableAllScripts()
{
    for (auto &script : CustomScripts)
    {
        game.Scripts.RemoveScriptFromList(&script, game.Scripts.pActiveScriptsList);
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Disabled script \"%s\"", script.m_acName);
    }
}

void ScriptManager::EnableAllScripts()
{
    for (auto &script : CustomScripts)
    {
        game.Scripts.AddScriptToList(&script, game.Scripts.pActiveScriptsList);
        LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Enabled script \"%s\"", script.m_acName);
    }
}
