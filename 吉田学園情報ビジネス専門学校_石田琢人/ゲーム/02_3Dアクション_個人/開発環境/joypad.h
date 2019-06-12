//=============================================================================
//
// ジョイパッド処理 [joypad.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XINPUT_STATE state;
	XINPUT_CAPABILITIES cap;
	DWORD dwResult;
	bool bConnected;
}CONTROLLER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ConnectJoystick(void);
int GetPadTrigger(int nKey);
void Trigger(void);
XINPUT_STATE GetJoyPad(void);

#endif