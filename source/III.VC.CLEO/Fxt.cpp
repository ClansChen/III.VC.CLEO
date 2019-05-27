#include "Fxt.h"
#include <stdio.h>
#include <Windows.h>
#include <algorithm>
#include <iterator>
#include "Log.h"
#include "Game.h"

std::unordered_map<std::string, std::wstring> CustomText::CustomTexts;

const wchar_t *CustomText::GetText(int theText, int, const char *key)
{
    const wchar_t *result = nullptr;

    auto it = CustomTexts.find(key);

    if (it != CustomTexts.end())
    {
        result = it->second.c_str();
    }
    else
    {
        result = game.Text.pfGetText(theText, key);

        if (result == nullptr)
            return L"";
    }

    return result;
}

char *StrFindKeyBegin(char *str)
{
    while (*str && *str != '\n' && *str != '\r' && *str != ';' && *str != '#')
    {
        if (*str != ' ' && *str != '\t')
            return str;
        str++;
    }
    return NULL;
}

char *StrFindKeyEnd(char *str)
{
    while (*str && *str != '\n' && *str != '\r' && *str != ';' && *str != '#')
    {
        if (*str == ' ' || *str == '\t')
            return str;
        str++;
    }
    return NULL;
}

char *StrFindTextBegin(char *str)
{
    while (*str && *str != '\n' && *str != '\r')
    {
        if (str[0] == '\\' && str[1] == '$')
            return str + 2;
        if (*str != ' ' && *str != '\t')
            return str;
        str++;
    }
    return NULL;
}

char *StrFindTextEnd(char *str)
{
    while (*str != '\0' && *str != '\n' && *str != '\r')
        str++;
    return str;
}

void CustomText::LoadFxtFile(const char *filepath)
{
    std::string key;
    std::wstring text;

    FILE *fxt = fopen(filepath, "rt");
    char line[512];
    if (fgets(line, 512, fxt))
    {
        do
        {
            char *keyBegin = StrFindKeyBegin(line);
            if (keyBegin)
            {
                char *keyEnd = StrFindKeyEnd(&keyBegin[1]);
                if (keyEnd)
                {
                    *keyEnd = '\0';
                    char *textBegin = StrFindTextBegin(&keyEnd[1]);
                    if (textBegin)
                    {
                        char *textEnd = StrFindTextEnd(&textBegin[1]);
                        *textEnd = '\0';

                        text.clear();
                        std::copy_n(textBegin, strlen(textBegin), std::back_inserter(text));
                        CustomTexts.insert_or_assign(keyBegin, text);

                        LOGL(LOG_PRIORITY_CUSTOM_TEXT, "Registered custom text: \"%s\", \"%s\"", keyBegin, textBegin);
                    }
                }
            }
        } while (fgets(line, 512, fxt));
    }
}

void CustomText::Load()
{
    WIN32_FIND_DATA FindFileData;
    memset(&FindFileData, 0, sizeof(WIN32_FIND_DATA));
    HANDLE hFind = FindFirstFile("CLEO\\CLEO_TEXT\\*.fxt", &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                char filename[MAX_PATH];
                strcpy(filename, "CLEO\\CLEO_TEXT\\");
                strcat(filename, FindFileData.cFileName);
                LoadFxtFile(filename);
            }
        } while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
}

void CustomText::Unload()
{
    for (auto &entry : CustomTexts)
    {
        LOGL(LOG_PRIORITY_CUSTOM_TEXT, "Unloaded custom text \"%s\"", entry.first.c_str());
    }

    CustomTexts.clear();
}
