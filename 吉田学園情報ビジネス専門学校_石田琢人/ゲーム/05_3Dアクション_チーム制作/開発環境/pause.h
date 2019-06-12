//=============================================================================
//
// �|�[�Y���j���[���� [pause.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "input.h"
#include "game.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	CONTINUE,
	RETRY,
	QUIT,
	MAX_PAUSE
}PAUSESTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	int nType;			//���
	PAUSESTATE state;	//���
} PAUSE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SelectPause(void);
#endif