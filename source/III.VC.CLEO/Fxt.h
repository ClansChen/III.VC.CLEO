#pragma once
#include <string>
#include <unordered_map>

class CustomText
{
public:
    static void Load();

    static void Unload();

    static const wchar_t * __fastcall GetText(int theText, int, const char *key);

    static void LoadFxtFile(const char *filepath);

    static std::unordered_map<std::string, std::wstring> CustomTexts;
};