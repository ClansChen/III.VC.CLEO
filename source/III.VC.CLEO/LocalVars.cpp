#include "Game.h"
#include "byte_pattern.h"
#include "injector/hooking.hpp"

static const std::size_t lvar_off = offsetof(CCustomScript, m_aLargeLVars);

void GtaGame::PatchLocalVars()
{
    byte_pattern pat;
    byte_pattern::start_log(L"CLEO_LVARS");

#ifdef CLEO_VC

//--------------------------------------------------------------------------------------------
    //movzx ebp, cx
    //mov eax, [esi+ebp*4+30h]
    pat.find_pattern("0F B7 E9 8B 44 AE 30");

//--------------------------------------------------------------------------------------------
    //add [ecx + 70h], esi;
    //add [ecx + 74h], esi;
    pat.find_pattern("01 71 70 01 71 74");

//--------------------------------------------------------------------------------------------
    //movzx reg32, reg16
    //mov reg32, [reg32*4+ScriptParams]
    //mov [reg32+reg32*4+30h], reg32
    pat.find_pattern("0F B7 ? 8B ? ? ? ? ? 00 89 ? ? 30");

//--------------------------------------------------------------------------------------------
    //lea reg32, [reg32*4]
    //add reg32, reg32
    //add reg32, 30h
    pat.find_pattern("00 00 00 00 01 ? 83 ? 30");

//----------------------------------------------------------------------------------------
    //mov cl, [eax]
    //add edi, 30h
    pat.find_pattern("8A 08 83 C7 30");

#elif defined CLEO_III

#endif
    byte_pattern::shutdown_log();
}
