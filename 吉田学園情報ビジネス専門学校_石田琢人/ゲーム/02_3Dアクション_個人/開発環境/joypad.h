//=============================================================================
//
// �W���C�p�b�h���� [joypad.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XINPUT_STATE state;
	XINPUT_CAPABILITIES cap;
	DWORD dwResult;
	bool bConnected;
}CONTROLLER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void ConnectJoystick(void);
int GetPadTrigger(int nKey);
void Trigger(void);
XINPUT_STATE GetJoyPad(void);

#endif