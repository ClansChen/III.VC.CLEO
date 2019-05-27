#include "Game.h"
#include "injector/hooking.hpp"

#ifdef CLEO_VC
__declspec(naked) void SpecialInlinedReadHook()
{
    __asm
    {
        movzx ebp, cx;
        mov eax, [esi]CCustomScript.m_aLargeLVars[ebp * 4];
        ret;
    }
}

__declspec(naked) void ScriptTimerHook()
{
    __asm
    {
        add [ecx]CCustomScript.m_aLargeLVars[16 * 4], esi;
        add [ecx]CCustomScript.m_aLargeLVars[17 * 4], esi;
        ret;
    }
}

__declspec(naked) void CreateThreadOpcodeHook()
{
    __asm
    {
        mov cl, [eax];
        lea edi, [edi]CCustomScript.m_aLargeLVars;
        ret;
    }
}

#define InlineWriteHookName(dest_index,free_to_use,temp,source_index,script_pointer)  InlineWriteHook_##dest_index##_##free_to_use##_##temp##_##source_index##_##script_pointer
#define InlineWriteHookDecl(dest_index,free_to_use,temp,source_index,script_pointer) __declspec(naked) void InlineWriteHookName(dest_index,free_to_use,temp,source_index,script_pointer)()
#define InlineWriteHookBody(dest_index,free_to_use,temp,source_index,script_pointer) \
    InlineWriteHookDecl(dest_index,free_to_use,temp,source_index,script_pointer)\
    {\
       __asm mov free_to_use, game.Scripts.Params\
       __asm mov temp, [free_to_use + source_index * 4]\
       __asm mov [script_pointer]CCustomScript.m_aLargeLVars[dest_index * 4], temp\
       __asm ret\
    }

InlineWriteHookBody(esi, ecx, edi, ebp, ebx)
InlineWriteHookBody(edi, ecx, esi, ebp, ebx)
InlineWriteHookBody(edi, ecx, ebp, esi, ebx)
InlineWriteHookBody(ebp, ecx, edi, esi, ebx)
InlineWriteHookBody(edi, edx, esi, ebx, ebp)
InlineWriteHookBody(edi, ecx, edx, esi, ebp)
InlineWriteHookBody(edi, ecx, ebx, esi, ebp)
InlineWriteHookBody(esi, edx, ecx, edi, ebp)
InlineWriteHookBody(edi, ebx, edx, esi, ebp)
InlineWriteHookBody(edi, ecx, esi, ebx, ebp)
InlineWriteHookBody(edi, edx, ecx, esi, ebp)
InlineWriteHookBody(edi, ecx, ebp, ebx, esi)
InlineWriteHookBody(edi, edx, ebp, ecx, esi)
InlineWriteHookBody(edx, eax, ecx, ecx, ebp)

#define InlineReadHookName(reg1, reg2) InlineReadHook_##reg1##_##reg2
#define InlineReadHookDecl(reg1, reg2) __declspec(naked) void InlineReadHookName(reg1, reg2)()
#define InlineReadHookBody(reg1, reg2) \
    InlineReadHookDecl(reg1, reg2)\
    {\
       __asm add reg1, reg2\
       __asm lea reg1, [reg1]CCustomScript.m_aLargeLVars\
       __asm ret\
    }

InlineReadHookBody(ebx, ebp)
InlineReadHookBody(esi, ebp)
InlineReadHookBody(edi, ebp)
InlineReadHookBody(eax, ebp)
InlineReadHookBody(ecx, ebp)
InlineReadHookBody(edx, ebp)
InlineReadHookBody(ebx, esi)
InlineReadHookBody(edi, esi)
InlineReadHookBody(ecx, esi)
InlineReadHookBody(edx, esi)
InlineReadHookBody(eax, esi)
InlineReadHookBody(eax, ecx)

void GtaGame::PatchLocalVars()
{
    switch (game.GetGameVersion())
    {
    case GAME_V1_0:
        //--------------------------------------------------------------------------------------------
        //0F B7 E9 8B 44 AE 30
        //movzx ebp, cx
        //mov eax, [esi+ebp*4+30h]
        injector::MakeNOP(0x44E5FE, 7);
        injector::MakeCALL(0x44E5FE, SpecialInlinedReadHook);

        //--------------------------------------------------------------------------------------------
        //01 71 70 01 71 74
        //add [ecx + 70h], esi;
        //add [ecx + 74h], esi;
        injector::MakeNOP(0x45023F, 6);
        injector::MakeCALL(0x45023F, ScriptTimerHook);

        //--------------------------------------------------------------------------------------------
        //0F B7 ? 8B ? ? ? ? ? 00 89 ? ? 30
        //movzx reg32, reg16
        //mov reg32, [reg32*4+ScriptParams]
        //mov [reg32+reg32*4+30h], reg32
        injector::MakeNOP(0x444F01 + 3, 11);
        injector::MakeNOP(0x445111 + 3, 11);
        injector::MakeNOP(0x445672 + 3, 11);
        injector::MakeNOP(0x44585B + 3, 11);
        injector::MakeNOP(0x4460FE + 3, 11);
        injector::MakeNOP(0x4498DB + 3, 11);
        injector::MakeNOP(0x449C9B + 3, 11);
        injector::MakeNOP(0x44A3ED + 3, 11);
        injector::MakeNOP(0x44A68B + 3, 11);
        injector::MakeNOP(0x44A9F2 + 3, 11);
        injector::MakeNOP(0x44B0C0 + 3, 11);
        injector::MakeNOP(0x44B1AD + 3, 11);
        injector::MakeNOP(0x44E8D2 + 3, 11);
        injector::MakeNOP(0x44E9FB + 3, 11);
        injector::MakeNOP(0x450ECA + 3, 11);

        injector::MakeCALL(0x444F01 + 3, InlineWriteHookName(esi, ecx, edi, ebp, ebx));
        injector::MakeCALL(0x445111 + 3, InlineWriteHookName(edi, ecx, esi, ebp, ebx));
        injector::MakeCALL(0x445672 + 3, InlineWriteHookName(edi, ecx, ebp, esi, ebx));
        injector::MakeCALL(0x44585B + 3, InlineWriteHookName(ebp, ecx, edi, esi, ebx));
        injector::MakeCALL(0x4460FE + 3, InlineWriteHookName(esi, ecx, edi, ebp, ebx));
        injector::MakeCALL(0x4498DB + 3, InlineWriteHookName(edi, edx, esi, ebx, ebp));
        injector::MakeCALL(0x449C9B + 3, InlineWriteHookName(edi, ecx, edx, esi, ebp));
        injector::MakeCALL(0x44A3ED + 3, InlineWriteHookName(edi, ecx, ebx, esi, ebp));
        injector::MakeCALL(0x44A68B + 3, InlineWriteHookName(esi, edx, ecx, edi, ebp));
        injector::MakeCALL(0x44A9F2 + 3, InlineWriteHookName(edi, ebx, edx, esi, ebp));
        injector::MakeCALL(0x44B0C0 + 3, InlineWriteHookName(edi, ecx, esi, ebx, ebp));
        injector::MakeCALL(0x44B1AD + 3, InlineWriteHookName(edi, edx, ecx, esi, ebp));
        injector::MakeCALL(0x44E8D2 + 3, InlineWriteHookName(edi, ecx, ebp, ebx, esi));
        injector::MakeCALL(0x44E9FB + 3, InlineWriteHookName(edi, edx, ebp, ecx, esi));
        injector::MakeCALL(0x450ECA + 3, InlineWriteHookName(edx, eax, ecx, ecx, ebp));

        //--------------------------------------------------------------------------------------------
        //00 00 00 00 01 ? 83 ? 30
        //lea reg32, [reg32*4]
        //add reg32, reg32
        //add reg32, 30h
        injector::MakeCALL(0x4463FB, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x44646C, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4464F2, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x446565, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4465EC, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44665D, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x4466EA, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44675B, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4467E2, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x446855, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4468DE, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x446955, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4469DC, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x446A4D, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x446ADD, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x446B55, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x446BDC, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x446C4D, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x446CDD, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x446D55, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x446DDC, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x446E4D, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x446EDD, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x446F55, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x446FDC, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44704D, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4470DD, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x447155, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4471DC, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x44724D, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4472DD, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x447355, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4473DC, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44744D, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x4474DD, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x447555, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4475DC, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44764D, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x4476DD, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x447755, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4477DC, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44787B, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44791A, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44798B, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447A18, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x447A89, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447B16, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x447B87, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447C14, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x447C85, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447D12, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x447DB3, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x447E53, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x447EC5, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447F52, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x447FC5, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448052, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4480C5, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448152, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4481C5, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448252, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x4482C5, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44834B, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x4483BC, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448442, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4484B5, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44853B, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x4485AC, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448632, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x4486A5, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44872B, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44879C, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448822, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448895, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44891B, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44898C, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448A12, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448A85, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448B32, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x448BA5, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448C2A, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448C9A, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448D47, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448DB8, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448E3D, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448EAD, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448F61, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x448FD5, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x44905A, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4490CA, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x449177, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4491E8, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x44926D, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4492F8, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x449383, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44941E, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x4494BE, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x449575, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44B517, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B5AB, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B640, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B6D8, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B76D, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B808, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B89F, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B938, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B9CF, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BA68, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BAFF, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BB98, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BC2F, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BCCC, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BD63, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BDFC, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BE93, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BF2C, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BFC3, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C05C, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C0F3, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C195, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C243, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44C2E6, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C37B, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44C3EC, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C47D, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44C4F5, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C586, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C5F7, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44C688, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44C6F9, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C78A, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C831, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C8E8, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44C98E, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CA2E, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44CAA5, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CB3A, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44CBAB, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CC40, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44CCB5, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CD4A, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44CDBB, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CE50, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44CEF5, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44CFA3, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44D046, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D0DB, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44D14C, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D1DD, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44D255, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D2E6, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44D357, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44D3E8, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44D459, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44D4EA, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D591, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D648, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D6EE, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D78E, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D805, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D89A, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44D90B, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D9A0, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44DA15, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44DAAA, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44DB1B, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44DBB0, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44DC55, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44DCF6, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44DD67, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44DDF8, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44DE69, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44DEFA, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44DF6B, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44DFFC, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44E0A6, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44E150, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44E1C5, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44E25D, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44E2D5, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44E36D, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44E3E5, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F04D, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F0C5, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F149, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44F1BA, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F23F, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44F2B5, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F339, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F3AA, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F42F, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44F4A5, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F529, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44F59A, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F61F, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F695, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F719, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44F78A, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F80F, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44F885, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F909, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F97A, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F9FF, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44FA75, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44FAF9, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44FB6A, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x450E15, InlineReadHookName(eax, ecx));

        //----------------------------------------------------------------------------------------
        //8A 08 83 C7 30
        //mov cl, [eax]
        //add edi, 30h
        injector::MakeCALL(0x44E563, CreateThreadOpcodeHook);
        break;

    case GAME_V1_1:
        //--------------------------------------------------------------------------------------------
        //0F B7 E9 8B 44 AE 30
        //movzx ebp, cx
        //mov eax, [esi+ebp*4+30h]
        injector::MakeNOP(0x44E5FE, 7);
        injector::MakeCALL(0x44E5FE, SpecialInlinedReadHook);

        //--------------------------------------------------------------------------------------------
        //01 71 70 01 71 74
        //add [ecx + 70h], esi;
        //add [ecx + 74h], esi;
        injector::MakeNOP(0x45023F, 6);
        injector::MakeCALL(0x45023F, ScriptTimerHook);

        //--------------------------------------------------------------------------------------------
        //0F B7 ? 8B ? ? ? ? ? 00 89 ? ? 30
        //movzx reg32, reg16
        //mov reg32, [reg32*4+ScriptParams]
        //mov [reg32+reg32*4+30h], reg32
        injector::MakeNOP(0x444F01 + 3, 11);
        injector::MakeNOP(0x445111 + 3, 11);
        injector::MakeNOP(0x445672 + 3, 11);
        injector::MakeNOP(0x44585B + 3, 11);
        injector::MakeNOP(0x4460FE + 3, 11);
        injector::MakeNOP(0x4498DB + 3, 11);
        injector::MakeNOP(0x449C9B + 3, 11);
        injector::MakeNOP(0x44A3ED + 3, 11);
        injector::MakeNOP(0x44A68B + 3, 11);
        injector::MakeNOP(0x44A9F2 + 3, 11);
        injector::MakeNOP(0x44B0C0 + 3, 11);
        injector::MakeNOP(0x44B1AD + 3, 11);
        injector::MakeNOP(0x44E8D2 + 3, 11);
        injector::MakeNOP(0x44E9FB + 3, 11);
        injector::MakeNOP(0x450ECA + 3, 11);

        injector::MakeCALL(0x444F01 + 3, InlineWriteHookName(esi, ecx, edi, ebp, ebx));
        injector::MakeCALL(0x445111 + 3, InlineWriteHookName(edi, ecx, esi, ebp, ebx));
        injector::MakeCALL(0x445672 + 3, InlineWriteHookName(edi, ecx, ebp, esi, ebx));
        injector::MakeCALL(0x44585B + 3, InlineWriteHookName(ebp, ecx, edi, esi, ebx));
        injector::MakeCALL(0x4460FE + 3, InlineWriteHookName(esi, ecx, edi, ebp, ebx));
        injector::MakeCALL(0x4498DB + 3, InlineWriteHookName(edi, edx, esi, ebx, ebp));
        injector::MakeCALL(0x449C9B + 3, InlineWriteHookName(edi, ecx, edx, esi, ebp));
        injector::MakeCALL(0x44A3ED + 3, InlineWriteHookName(edi, ecx, ebx, esi, ebp));
        injector::MakeCALL(0x44A68B + 3, InlineWriteHookName(esi, edx, ecx, edi, ebp));
        injector::MakeCALL(0x44A9F2 + 3, InlineWriteHookName(edi, ebx, edx, esi, ebp));
        injector::MakeCALL(0x44B0C0 + 3, InlineWriteHookName(edi, ecx, esi, ebx, ebp));
        injector::MakeCALL(0x44B1AD + 3, InlineWriteHookName(edi, edx, ecx, esi, ebp));
        injector::MakeCALL(0x44E8D2 + 3, InlineWriteHookName(edi, ecx, ebp, ebx, esi));
        injector::MakeCALL(0x44E9FB + 3, InlineWriteHookName(edi, edx, ebp, ecx, esi));
        injector::MakeCALL(0x450ECA + 3, InlineWriteHookName(edx, eax, ecx, ecx, ebp));

        //--------------------------------------------------------------------------------------------
        //00 00 00 00 01 ? 83 ? 30
        //lea reg32, [reg32*4]
        //add reg32, reg32
        //add reg32, 30h
        injector::MakeCALL(0x4463FB - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x44646C - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4464F2 - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x446565 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4465EC - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44665D - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x4466EA - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44675B - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4467E2 - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x446855 - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4468DE - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x446955 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4469DC - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x446A4D - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x446ADD - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x446B55 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x446BDC - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x446C4D - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x446CDD - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x446D55 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x446DDC - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x446E4D - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x446EDD - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x446F55 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x446FDC - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44704D - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4470DD - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x447155 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4471DC - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x44724D - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4472DD - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x447355 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4473DC - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44744D - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x4474DD - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x447555 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4475DC - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44764D - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x4476DD - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x447755 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4477DC - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44787B - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44791A - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44798B - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447A18 - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x447A89 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447B16 - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x447B87 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447C14 - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x447C85 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447D12 - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x447DB3 - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x447E53 - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x447EC5 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447F52 - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x447FC5 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448052 - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4480C5 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448152 - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4481C5 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448252 - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x4482C5 - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44834B - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x4483BC - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448442 - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4484B5 - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44853B - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x4485AC - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448632 - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x4486A5 - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44872B - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44879C - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448822 - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448895 - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44891B - 4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44898C - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448A12 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448A85 - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448B32 - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x448BA5 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448C2A - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448C9A - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448D47 - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448DB8 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448E3D - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448EAD - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448F61 - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x448FD5 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x44905A - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4490CA - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x449177 - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4491E8 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x44926D - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4492F8 - 4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x449383 - 4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44941E - 4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x4494BE - 4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x449575 - 4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44B517 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B5AB - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B640 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B6D8 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B76D - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B808 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B89F - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B938 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B9CF - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BA68 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BAFF - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BB98 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BC2F - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BCCC - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BD63 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BDFC - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BE93 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BF2C - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BFC3 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C05C - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C0F3 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C195 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C243 - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44C2E6 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C37B - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44C3EC - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C47D - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44C4F5 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C586 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C5F7 - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44C688 - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44C6F9 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C78A - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C831 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C8E8 - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44C98E - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CA2E - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44CAA5 - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CB3A - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44CBAB - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CC40 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44CCB5 - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CD4A - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44CDBB - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CE50 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44CEF5 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44CFA3 - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44D046 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D0DB - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44D14C - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D1DD - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44D255 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D2E6 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44D357 - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44D3E8 - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44D459 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44D4EA - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D591 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D648 - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D6EE - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D78E - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D805 - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D89A - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44D90B - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D9A0 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44DA15 - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44DAAA - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44DB1B - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44DBB0 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44DC55 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44DCF6 - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44DD67 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44DDF8 - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44DE69 - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44DEFA - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44DF6B - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44DFFC - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44E0A6 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44E150 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44E1C5 - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44E25D - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44E2D5 - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44E36D - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44E3E5 - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F04D - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F0C5 - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F149 - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44F1BA - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F23F - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44F2B5 - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F339 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F3AA - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F42F - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44F4A5 - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F529 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44F59A - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F61F - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F695 - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F719 - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44F78A - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F80F - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44F885 - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F909 - 4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F97A - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F9FF - 4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44FA75 - 4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44FAF9 - 4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44FB6A - 4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x450E15 - 4, InlineReadHookName(eax, ecx));

        //----------------------------------------------------------------------------------------
        //8A 08 83 C7 30
        //mov cl, [eax]
        //add edi, 30h
        injector::MakeCALL(0x44E563, CreateThreadOpcodeHook);
        break;

    case GAME_VSTEAM:
        //--------------------------------------------------------------------------------------------
        //0F B7 E9 8B 44 AE 30
        //movzx ebp, cx
        //mov eax, [esi+ebp*4+30h]
        injector::MakeNOP(0x44E5FE - 0xF0, 7);
        injector::MakeCALL(0x44E5FE - 0xF0, SpecialInlinedReadHook);

        //--------------------------------------------------------------------------------------------
        //01 71 70 01 71 74
        //add [ecx + 70h], esi;
        //add [ecx + 74h], esi;
        injector::MakeNOP(0x45023F - 0xF0, 6);
        injector::MakeCALL(0x45023F - 0xF0, ScriptTimerHook);

        //--------------------------------------------------------------------------------------------
        //0F B7 ? 8B ? ? ? ? ? 00 89 ? ? 30
        //movzx reg32, reg16
        //mov reg32, [reg32*4+ScriptParams]
        //mov [reg32+reg32*4+30h], reg32
        injector::MakeNOP(0x444F01 - 0xF0 + 3, 11);
        injector::MakeNOP(0x445111 - 0xF0 + 3, 11);
        injector::MakeNOP(0x445672 - 0xF0 + 3, 11);
        injector::MakeNOP(0x44585B - 0xF0 + 3, 11);
        injector::MakeNOP(0x4460FE - 0xF0 + 3, 11);
        injector::MakeNOP(0x4498DB - 0xF0 + 3, 11);
        injector::MakeNOP(0x449C9B - 0xF0 + 3, 11);
        injector::MakeNOP(0x44A3ED - 0xF0 + 3, 11);
        injector::MakeNOP(0x44A68B - 0xF0 + 3, 11);
        injector::MakeNOP(0x44A9F2 - 0xF0 + 3, 11);
        injector::MakeNOP(0x44B0C0 - 0xF0 + 3, 11);
        injector::MakeNOP(0x44B1AD - 0xF0 + 3, 11);
        injector::MakeNOP(0x44E8D2 - 0xF0 + 3, 11);
        injector::MakeNOP(0x44E9FB - 0xF0 + 3, 11);
        injector::MakeNOP(0x450ECA - 0xF0 + 3, 11);

        injector::MakeCALL(0x444F01 - 0xF0 + 3, InlineWriteHookName(esi, ecx, edi, ebp, ebx));
        injector::MakeCALL(0x445111 - 0xF0 + 3, InlineWriteHookName(edi, ecx, esi, ebp, ebx));
        injector::MakeCALL(0x445672 - 0xF0 + 3, InlineWriteHookName(edi, ecx, ebp, esi, ebx));
        injector::MakeCALL(0x44585B - 0xF0 + 3, InlineWriteHookName(ebp, ecx, edi, esi, ebx));
        injector::MakeCALL(0x4460FE - 0xF0 + 3, InlineWriteHookName(esi, ecx, edi, ebp, ebx));
        injector::MakeCALL(0x4498DB - 0xF0 + 3, InlineWriteHookName(edi, edx, esi, ebx, ebp));
        injector::MakeCALL(0x449C9B - 0xF0 + 3, InlineWriteHookName(edi, ecx, edx, esi, ebp));
        injector::MakeCALL(0x44A3ED - 0xF0 + 3, InlineWriteHookName(edi, ecx, ebx, esi, ebp));
        injector::MakeCALL(0x44A68B - 0xF0 + 3, InlineWriteHookName(esi, edx, ecx, edi, ebp));
        injector::MakeCALL(0x44A9F2 - 0xF0 + 3, InlineWriteHookName(edi, ebx, edx, esi, ebp));
        injector::MakeCALL(0x44B0C0 - 0xF0 + 3, InlineWriteHookName(edi, ecx, esi, ebx, ebp));
        injector::MakeCALL(0x44B1AD - 0xF0 + 3, InlineWriteHookName(edi, edx, ecx, esi, ebp));
        injector::MakeCALL(0x44E8D2 - 0xF0 + 3, InlineWriteHookName(edi, ecx, ebp, ebx, esi));
        injector::MakeCALL(0x44E9FB - 0xF0 + 3, InlineWriteHookName(edi, edx, ebp, ecx, esi));
        injector::MakeCALL(0x450ECA - 0xF0 + 3, InlineWriteHookName(edx, eax, ecx, ecx, ebp));

        //--------------------------------------------------------------------------------------------
        //00 00 00 00 01 ? 83 ? 30
        //lea reg32, [reg32*4]
        //add reg32, reg32
        //add reg32, 30h
        injector::MakeCALL(0x4463FB - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x44646C - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4464F2 - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x446565 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4465EC - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44665D - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x4466EA - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44675B - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4467E2 - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x446855 - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4468DE - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x446955 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4469DC - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x446A4D - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x446ADD - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x446B55 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x446BDC - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x446C4D - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x446CDD - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x446D55 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x446DDC - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x446E4D - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x446EDD - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x446F55 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x446FDC - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44704D - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4470DD - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x447155 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4471DC - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x44724D - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4472DD - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x447355 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4473DC - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44744D - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x4474DD - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x447555 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4475DC - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44764D - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x4476DD - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x447755 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4477DC - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44787B - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44791A - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44798B - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447A18 - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x447A89 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447B16 - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x447B87 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447C14 - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x447C85 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447D12 - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x447DB3 - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x447E53 - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x447EC5 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x447F52 - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x447FC5 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448052 - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4480C5 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448152 - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4481C5 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448252 - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x4482C5 - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44834B - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x4483BC - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448442 - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x4484B5 - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44853B - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x4485AC - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448632 - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x4486A5 - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x44872B - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44879C - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448822 - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448895 - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44891B - 0xF4, InlineReadHookName(edi, ebp));
        injector::MakeCALL(0x44898C - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448A12 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448A85 - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448B32 - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x448BA5 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448C2A - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448C9A - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448D47 - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x448DB8 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448E3D - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x448EAD - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x448F61 - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x448FD5 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x44905A - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4490CA - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x449177 - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x4491E8 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x44926D - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x4492F8 - 0xF4, InlineReadHookName(eax, ebp));
        injector::MakeCALL(0x449383 - 0xF4, InlineReadHookName(ecx, ebp));
        injector::MakeCALL(0x44941E - 0xF4, InlineReadHookName(edx, ebp));
        injector::MakeCALL(0x4494BE - 0xF4, InlineReadHookName(ebx, ebp));
        injector::MakeCALL(0x449575 - 0xF4, InlineReadHookName(esi, ebp));
        injector::MakeCALL(0x44B517 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B5AB - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B640 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B6D8 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B76D - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B808 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B89F - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44B938 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44B9CF - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BA68 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BAFF - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BB98 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BC2F - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BCCC - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BD63 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BDFC - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BE93 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44BF2C - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44BFC3 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C05C - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C0F3 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C195 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C243 - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44C2E6 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C37B - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44C3EC - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C47D - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44C4F5 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C586 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C5F7 - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44C688 - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44C6F9 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44C78A - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C831 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44C8E8 - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44C98E - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CA2E - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44CAA5 - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CB3A - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44CBAB - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CC40 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44CCB5 - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CD4A - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44CDBB - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44CE50 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44CEF5 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44CFA3 - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44D046 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D0DB - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44D14C - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D1DD - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44D255 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D2E6 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44D357 - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44D3E8 - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44D459 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44D4EA - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D591 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D648 - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D6EE - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D78E - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44D805 - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D89A - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44D90B - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44D9A0 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44DA15 - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44DAAA - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44DB1B - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44DBB0 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44DC55 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44DCF6 - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44DD67 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44DDF8 - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44DE69 - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44DEFA - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44DF6B - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44DFFC - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44E0A6 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44E150 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44E1C5 - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44E25D - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44E2D5 - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44E36D - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44E3E5 - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F04D - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F0C5 - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F149 - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44F1BA - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F23F - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44F2B5 - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F339 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F3AA - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F42F - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44F4A5 - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F529 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44F59A - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F61F - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F695 - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F719 - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44F78A - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F80F - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44F885 - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44F909 - 0xF4, InlineReadHookName(ebx, esi));
        injector::MakeCALL(0x44F97A - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x44F9FF - 0xF4, InlineReadHookName(ecx, esi));
        injector::MakeCALL(0x44FA75 - 0xF4, InlineReadHookName(edx, esi));
        injector::MakeCALL(0x44FAF9 - 0xF4, InlineReadHookName(edi, esi));
        injector::MakeCALL(0x44FB6A - 0xF4, InlineReadHookName(eax, esi));
        injector::MakeCALL(0x450E15 - 0xF4, InlineReadHookName(eax, ecx));

        //----------------------------------------------------------------------------------------
        //8A 08 83 C7 30
        //mov cl, [eax]
        //add edi, 30h
        injector::MakeCALL(0x44E563 - 0xF0, CreateThreadOpcodeHook);
        break;

    default:
        break;
    }
}
#elif defined CLEO_III
__declspec(naked) void StoreHook()
{
    __asm
    {
        {
            mov eax, game.Scripts.Params;
            mov ecx, [eax + ecx * 4];
            mov [ebp]CCustomScript.m_aLargeLVars[edx * 4], ecx;
            ret;
        }
    }
}

__declspec(naked) void GetVariablePointerHook()
{
    __asm
    {
        add eax, ecx;
        lea eax, [eax]CCustomScript.m_aLargeLVars;
        ret;
    }
}

__declspec(naked) void CreateThreadOpcodeHook1()
{
    __asm
    {
        lea eax, [eax]CCustomScript.m_aLargeLVars;
        mov cl, [esi];
        ret;
    }
}

__declspec(naked) void CreateThreadOpcodeHook2()
{
    __asm
    {
        mov ebp, [ebx]CCustomScript.m_aLargeLVars[ecx * 4];
        mov [eax], ebp;
        ret;
    }
}

__declspec(naked) void ScriptTimerHook()
{
    __asm
    {
        add [ecx]CCustomScript.m_aLargeLVars[16 * 4], esi;
        add [ecx]CCustomScript.m_aLargeLVars[17 * 4], esi;
        ret;
    }
}

void GtaGame::PatchLocalVars()
{
    switch (game.GetGameVersion())
    {
    case GAME_V1_0:
    case GAME_V1_1:
    case GAME_VSTEAM:
        //8B 0C 8D ? ? ? 00 89 4C 95 30
        injector::MakeNOP(0x43861D, 11);
        injector::MakeCALL(0x43861D, StoreHook);

        //01 C8 83 C0 30
        injector::MakeCALL(0x438685, GetVariablePointerHook);

        //83 C0 30 8A 0E
        injector::MakeCALL(0x43A591, CreateThreadOpcodeHook1);

        //8B 6C 8B 30 89 28
        injector::MakeNOP(0x43A63E, 6);
        injector::MakeCALL(0x43A63E, CreateThreadOpcodeHook2);

        //01 71 70 01 71 74
        injector::MakeNOP(0x4393D9, 6);
        injector::MakeCALL(0x4393D9, ScriptTimerHook);
        break;

    default:
        break;
    }
}

#endif
