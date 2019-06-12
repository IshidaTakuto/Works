//=============================================================================
//
// ジョイパッド処理 [joypad.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#include "main.h"

typedef struct
{
	XINPUT_STATE state;
	XINPUT_CAPABILITIES cap;
	DWORD dwResult;
	bool bConnected;
}CONTROLLER;

//typedef BOOL(FAR PASCAL * LPDIENUMDEVICESCALLBACKA)(LPCDIDEVICEINSTANCEA, LPVOID);

void InitJoystick(void);
void ConnectJoystick(void);
int GetPadTrigger(int nKey, int nPad, XINPUT_STATE *state);
int GetAnalogTrigger(int nKey, int nPress, float fTrigger, XINPUT_STATE *state);
int GetPadTPress(int nKey, int nPad, XINPUT_STATE *state);
int GetAnalogTPress(int nKey, int nPress, float fTrigger, bool bWhich, XINPUT_STATE *state);
void Trigger(void);
XINPUT_STATE GetJoyPad(void);

#endif