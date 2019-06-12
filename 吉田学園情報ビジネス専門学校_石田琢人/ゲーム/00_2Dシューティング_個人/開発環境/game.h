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
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "effect.h"
#include "fade.h"
#include "timer.h"
#include "life.h"
#include "pause.h"
#include "menu.h"
#include "particle.h"
#include "pausebg.h"
#include "ranking.h"
#include "beam.h"
#include "stock.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,	//�������Ă��Ȃ����
	GAMESTATE_NORMAL,	//�ʏ���
	GAMESTATE_END,		//�I�����
	GAMESTATE_MAX
}GAMESTATE;

typedef struct
{
	bool bPause;
}PAUSECHECK;
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