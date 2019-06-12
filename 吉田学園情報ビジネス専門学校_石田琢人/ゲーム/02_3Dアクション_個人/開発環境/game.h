//=============================================================================
//
// ゲーム処理 [game.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "model.h"
#include "meshWall.h"

#define SCREEN_HALF (SCREEN_WIDTH / 2)	// 画面の半分

//*****************************************************************************
// 構造体定義
//*****************************************************************************
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

// メッシュウォールの配置の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3	rot;
	int nType;
	float fWidth;
	float fHeight;
}MeshWallInfo;

// モデルの配置の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3	rot;
	int nType;
	MODELTYPE type;
}ModelInfo;

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