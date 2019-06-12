//=============================================================================
//
// サウンド処理 [sound.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// サウンドファイル
//*************************************
typedef enum
{
	SOUND_LABEL_TITLE000 = 0,		// BGM0
	SOUND_LABEL_GAME000,			// BGM1
	SOUND_LABEL_RESULT000,			// BGM2
	SOUND_LABEL_GAMEOVER000,		// BGM3
	SOUND_LABEL_TUTORIAL000,		// BGM4
	SOUND_LABEL_SE_DECIDE,			// 決定音
	SOUND_LABEL_SE_PAUSEON,			// ポーズ音on
	SOUND_LABEL_SE_PAUSEOFF,		// ポーズ音off
	SOUND_LABEL_SE_CURSOR,			// カーソル音
	SOUND_LABEL_SE_EGG,				// 取得音
	SOUND_LABEL_SE_JUMP,			// ジャンプ音
	SOUND_LABEL_SE_SPLASH,			// ドボン音
	SOUND_LABEL_SE_DRESSING,		// 付着音
	SOUND_LABEL_SE_SCREAM000,		// 悲鳴0
	SOUND_LABEL_SE_SCREAM001,		// 悲鳴1
	SOUND_LABEL_SE_JUMP_VOICE000,	// ジャンプ声0
	SOUND_LABEL_SE_JUMP_VOICE001,	// ジャンプ声1
	SOUND_LABEL_SE_TURN,			// チュートリアルのページめくり
	SOUND_LABEL_SE_FLY,				// 羽ばたき音
	SOUND_LABEL_SE_SCORE_UP,		// スコア獲得音
	SOUND_LABEL_SE_BIRD,			// 鳥の鳴き声
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
