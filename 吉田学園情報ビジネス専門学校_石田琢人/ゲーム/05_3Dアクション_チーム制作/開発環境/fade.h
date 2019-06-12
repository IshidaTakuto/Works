//=============================================================================
//
// フェード処理 [fade.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "game.h"

//*************************************
// フェードの状態
//*************************************
typedef enum
{
	FADE_NONE = 0,		// 何もしていない状態
	FADE_IN,			// フェードイン状態
	FADE_OUT,			// フェードアウト状態
	FADE_MAX
} FADE;

typedef enum
{
	MODEFADE_NONE = 0,	// 何もしていない状態
	MODEFADE_NOLMAL,	// 通常
	MODEFADE_GAME,		// ゲーム
	MODEFADE_MODE,		// モード
	MODEFADE_MAX		// 最大
} MODEFADE;

//*****************************************************************************
// プロトタイプ宣言
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
