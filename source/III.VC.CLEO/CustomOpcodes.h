﻿#pragma once
#include "CustomScript.h"
#include <Windows.h>

class CustomOpcodes
{
	static eOpcodeResult WINAPI DUMMY(CCustomScript *script);
	static eOpcodeResult WINAPI GOTO(CCustomScript *script);
	static eOpcodeResult WINAPI GOTO_IF_TRUE(CCustomScript *script);
	static eOpcodeResult WINAPI GOTO_IF_FALSE(CCustomScript *script);
	static eOpcodeResult WINAPI GOSUB(CCustomScript *script);
	static eOpcodeResult WINAPI TERMINATE_CUSTOM_THREAD(CCustomScript *script);
	static eOpcodeResult WINAPI TERMINATE_NAMED_CUSTOM_THREAD(CCustomScript *script);
	static eOpcodeResult WINAPI START_CUSTOM_THREAD(CCustomScript *script);
	static eOpcodeResult WINAPI MEMORY_WRITE(CCustomScript *script);
	static eOpcodeResult WINAPI MEMORY_READ(CCustomScript *script);
	static eOpcodeResult WINAPI CALL(CCustomScript *script);
	static eOpcodeResult WINAPI CALL_FUNCTION(CCustomScript *script);
	static eOpcodeResult WINAPI CALL_METHOD(CCustomScript *script);
	static eOpcodeResult WINAPI CALL_FUNCTION_METHOD(CCustomScript *script);
	static eOpcodeResult WINAPI GET_GAME_VERSION(CCustomScript *script);
	static eOpcodeResult WINAPI GET_CHAR_STRUCT(CCustomScript *script);
	static eOpcodeResult WINAPI GET_CAR_STRUCT(CCustomScript *script);
	static eOpcodeResult WINAPI GET_OBJECT_STRUCT(CCustomScript *script);
	static eOpcodeResult WINAPI GET_CHAR_HANDLE(CCustomScript *script);
	static eOpcodeResult WINAPI GET_CAR_HANDLE(CCustomScript *script);
	static eOpcodeResult WINAPI GET_OBJECT_HANDLE(CCustomScript *script);
	static eOpcodeResult WINAPI GET_THREAD_POINTER(CCustomScript *script);
	static eOpcodeResult WINAPI GET_NAMED_THREAD_POINTER(CCustomScript *script);
	static eOpcodeResult WINAPI IS_KEY_PRESSED(CCustomScript *script);
	static eOpcodeResult WINAPI FIND_RANDOM_CHAR(CCustomScript *script);
	static eOpcodeResult WINAPI FIND_RANDOM_CAR(CCustomScript *script);
	static eOpcodeResult WINAPI FIND_RANDOM_OBJECT(CCustomScript *script);
	static eOpcodeResult WINAPI CALL_POP_FLOAT(CCustomScript *script);
	static eOpcodeResult WINAPI MATH_EXP(CCustomScript *script);
	static eOpcodeResult WINAPI MATH_LOG(CCustomScript *script);
	static eOpcodeResult WINAPI CALL_SCM_FUNCTION(CCustomScript *script);
	static eOpcodeResult WINAPI SCM_FUNCTION_RET(CCustomScript *script);
	static eOpcodeResult WINAPI GET_LABEL_OFFSET(CCustomScript *script);
	static eOpcodeResult WINAPI GET_VAR_OFFSET(CCustomScript *script);
	static eOpcodeResult WINAPI BIT_AND(CCustomScript *script);
	static eOpcodeResult WINAPI BIT_OR(CCustomScript *script);
	static eOpcodeResult WINAPI BIT_XOR(CCustomScript *script);
	static eOpcodeResult WINAPI BIT_NOT(CCustomScript *script);
	static eOpcodeResult WINAPI BIT_MOD(CCustomScript *script);
	static eOpcodeResult WINAPI BIT_SHR(CCustomScript *script);
	static eOpcodeResult WINAPI BIT_SHL(CCustomScript *script);
	static eOpcodeResult WINAPI STORE_COORDS_FROM_OBJECT_WITH_OFFSET(CCustomScript *script);
	static eOpcodeResult WINAPI STORE_COORDS_FROM_CAR_WITH_OFFSET(CCustomScript *script);
	static eOpcodeResult WINAPI STORE_COORDS_FROM_ACTOR_WITH_OFFSET(CCustomScript *script);
	static eOpcodeResult WINAPI STORE_PLAYER_CURRENTLY_ARMED_WEAPON(CCustomScript *script);
	static eOpcodeResult WINAPI START_CUSTOM_THREAD_VSTRING(CCustomScript *script);
	static eOpcodeResult WINAPI IS_BUTTON_PRESSED_ON_PAD(CCustomScript *script);
	static eOpcodeResult WINAPI EMULATE_BUTTON_PRESS_ON_PAD(CCustomScript *script);
	static eOpcodeResult WINAPI IS_CAMERA_IN_WIDESCREEN_MODE(CCustomScript *script);
	static eOpcodeResult WINAPI GET_MODEL_ID_FROM_WEAPON_ID(CCustomScript *script);
	static eOpcodeResult WINAPI GET_WEAPON_ID_FROM_MODEL_ID(CCustomScript *script);
	static eOpcodeResult WINAPI SET_MEM_OFFSET(CCustomScript *script);
	static eOpcodeResult WINAPI GET_CURRENT_WEATHER(CCustomScript *script);
	static eOpcodeResult WINAPI SHOW_TEXT_POSITION(CCustomScript *script);
	static eOpcodeResult WINAPI SHOW_FORMATTED_TEXT_POSITION(CCustomScript *script);
	static eOpcodeResult WINAPI DRAW_SHADOW(CCustomScript *script);
	static eOpcodeResult WINAPI SET_TEXT_DRAW_FONT(CCustomScript *script);
	static eOpcodeResult WINAPI GET_CHAR_ARMOUR(CCustomScript *script);
	static eOpcodeResult WINAPI PLAYER_DRIVING_PLANE(CCustomScript *script);
	static eOpcodeResult WINAPI PLAYER_DRIVING_BOAT(CCustomScript *script);
	static eOpcodeResult WINAPI PLAYER_DRIVING_HELI(CCustomScript *script);
	static eOpcodeResult WINAPI PLAYER_DRIVING_A_MOTORBIKE(CCustomScript *script);
	static eOpcodeResult WINAPI IS_PC_VERSION(CCustomScript *script);
	static eOpcodeResult WINAPI IS_AUSTRALIAN_GAME(CCustomScript *script);
	static eOpcodeResult WINAPI PLAY_ANIMATION(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0A8E(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0A8F(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0A90(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0A91(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0A99(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0A9A(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0A9B(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0A9C(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0A9D(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0A9E(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AA0(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AA1(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AA2(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AA3(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AA4(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AA9(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AAB(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AC8(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AC9(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ACA(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ACB(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ACC(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ACD(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ACE(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ACF(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AD0(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AD1(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AD3(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AD4(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AD5(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AD6(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AD7(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AD8(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AD9(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ADA(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ADB(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ADC(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ADD(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ADE(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0ADF(CCustomScript *script);
	static eOpcodeResult WINAPI OPCODE_0AE0(CCustomScript *script);
public:
	static void Register();
};