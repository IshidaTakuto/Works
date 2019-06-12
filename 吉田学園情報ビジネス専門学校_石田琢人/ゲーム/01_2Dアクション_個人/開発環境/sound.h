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
	SOUND_LABEL_TITLE000 = 0,		// BGM0
	SOUND_LABEL_GAME000,			// BGM1
	SOUND_LABEL_RESULT000,			// BGM2
	SOUND_LABEL_GAMEOVER000,		// BGM3
	SOUND_LABEL_TUTORIAL000,		// BGM4
	SOUND_LABEL_SE_DECIDE,			// ���艹
	SOUND_LABEL_SE_PAUSEON,			// �|�[�Y��on
	SOUND_LABEL_SE_PAUSEOFF,		// �|�[�Y��off
	SOUND_LABEL_SE_CURSOR,			// �J�[�\����
	SOUND_LABEL_SE_EGG,				// �擾��
	SOUND_LABEL_SE_JUMP,			// �W�����v��
	SOUND_LABEL_SE_SPLASH,			// �h�{����
	SOUND_LABEL_SE_DRESSING,		// �t����
	SOUND_LABEL_SE_SCREAM000,		// �ߖ�0
	SOUND_LABEL_SE_SCREAM001,		// �ߖ�1
	SOUND_LABEL_SE_JUMP_VOICE000,	// �W�����v��0
	SOUND_LABEL_SE_JUMP_VOICE001,	// �W�����v��1
	SOUND_LABEL_SE_TURN,			// �`���[�g���A���̃y�[�W�߂���
	SOUND_LABEL_SE_FLY,				// �H�΂�����
	SOUND_LABEL_SE_SCORE_UP,		// �X�R�A�l����
	SOUND_LABEL_SE_BIRD,			// ���̖���
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
