//=============================================================================
//
// ゲーム処理 [game.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

#define SCREEN_HALF (SCREEN_WIDTH / 2)	// 画面の半分
#define GRAVITY					(3)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{//各ステージ
	STAGE_NONE = 0,			//何もしてない状態
	STAGENUM_1,				//ステージ1
	STAGENUM_2,				//ステージ2
	STAGENUM_3,				//ステージ3
}STAGENUM;

typedef enum
{
	GAMESTATE_NONE = 0,	// 何もしていない状態
	GAMESTATE_NORMAL,	// 通常状態
	GAMESTATE_END,		// ゲームオーバー
	GAMESTATE_CLEAR,	// ゲームクリア
	GAMESTATE_MAX
}GAMESTATE;

// ポーズON/OFFの構造体
typedef struct
{
	bool bPause;
}PAUSECHECK;

// 敵の情報の構造体
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	int nType;			// 敵の種類
	bool bUse;			// 情報を使用したかどうか
}EnemyInfo;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetStage(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);
STAGENUM GetNumState(void);
PAUSECHECK *GetPauseMenu(void);
void SetStageState(STAGENUM stage);				// ステージ設定処理
void StageStateSwat(bool bSwat);
bool GetStageSwat(void);

#endif