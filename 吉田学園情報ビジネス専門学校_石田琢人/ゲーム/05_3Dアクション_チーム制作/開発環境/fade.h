//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "game.h"

//*************************************
// �t�F�[�h�̏��
//*************************************
typedef enum
{
	FADE_NONE = 0,		// �������Ă��Ȃ����
	FADE_IN,			// �t�F�[�h�C�����
	FADE_OUT,			// �t�F�[�h�A�E�g���
	FADE_MAX
} FADE;

typedef enum
{
	MODEFADE_NONE = 0,	// �������Ă��Ȃ����
	MODEFADE_NOLMAL,	// �ʏ�
	MODEFADE_GAME,		// �Q�[��
	MODEFADE_MODE,		// ���[�h
	MODEFADE_MAX		// �ő�
} MODEFADE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetNumGame(STAGENUM StageNext);
void SetFadeMode(MODE modeNext);
FADE *GetFade(void);
bool GetFadeNow(void);

#endif
