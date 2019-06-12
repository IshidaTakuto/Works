//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// ゲーム本編処理 [game.h]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// ゲームの状態の種類
typedef enum
{
	GAMESTATE_NONE = 0,		// 何もしていない状態
	GAMESTATE_START,		// 開始状態
	GAMESTATE_NORMAL,		// 通常状態
	GAMESTATE_END,			// 終了状態
	GAMESTATE_MAX
}GAMESTATE;

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);
void PauseRelease(void);
void SubLivePlayer(int nValue);
//void SetUpdateGame(bool flag);		// 更新フラグの設定]
int GetNumLivePlayer(void);

#endif