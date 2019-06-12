//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// �T�E���h�t�@�C��
//*************************************
typedef enum
{
	SOUND_LABEL_TITLE000 = 0,	// taitol
	SOUND_LABEL_GAME000,
	SOUND_LABEL_RESULT000,
	SOUND_LABEL_DECIDE,		// ���艹
	SOUND_LABEL_BOTTON001,		//�J�[�\���ړ���
	SOUND_LABEL_JUMP000,		//�W�����v
	SOUND_LABEL_COIN,			//�R�C��
	SOUND_LABEL_TURN,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
