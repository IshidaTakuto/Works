//=============================================================================
//
// �Q�[������ [game.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "pause.h"
#include "pausemenu.h"
#include "pausebg.h"
#include "fade.h"
#include "block.h"
#include "score.h"
#include "item.h"
#include "timer.h"
#include "generator.h"
#include "effect.h"
#include "sound.h"
#include "stock.h"
#include "destroyer.h"
#include "blood.h"

#define SCREEN_HALF (SCREEN_WIDTH / 2)	// ��ʂ̔���

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,	// �������Ă��Ȃ����
	GAMESTATE_NORMAL,	// �ʏ���
	GAMESTATE_END,		// �Q�[���I�[�o�[
	GAMESTATE_CLEAR,	// �Q�[���N���A
	GAMESTATE_MAX
}GAMESTATE;

typedef struct
{
	bool bPause;
}PAUSECHECK;

typedef struct
{
	D3DXVECTOR3 pos;
	float fWidth;
	float fHeight;
	D3DXVECTOR3 des;
	BLOCKTYPE type;
}BlockInfo;

typedef struct
{
	D3DXVECTOR3 pos;
	ITEMTYPE	type;
}ItemInfo;

typedef struct
{
	D3DXVECTOR3 pos;
	BLOCKTYPE type;
	float fWidth;
	float fHeight;
	D3DXVECTOR3 des;
	int interval;
}GeneratorInfo;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);

PAUSECHECK *GetPauseMenu(void); 
#endif