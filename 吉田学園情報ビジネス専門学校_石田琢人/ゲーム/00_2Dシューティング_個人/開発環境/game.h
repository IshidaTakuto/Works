//=============================================================================
//
// ゲーム処理 [game.h]
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
// 構造体定義
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,	//何もしていない状態
	GAMESTATE_NORMAL,	//通常状態
	GAMESTATE_END,		//終了状態
	GAMESTATE_MAX
}GAMESTATE;

typedef struct
{
	bool bPause;
}PAUSECHECK;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);

PAUSECHECK *GetPauseMenu(void);
#endif