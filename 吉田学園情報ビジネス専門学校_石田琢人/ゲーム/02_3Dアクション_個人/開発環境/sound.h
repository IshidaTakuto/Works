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
	SOUND_LABEL_TITLE000 = 0,	// taitol
	SOUND_LABEL_GAME000,
	SOUND_LABEL_RESULT000,
	SOUND_LABEL_DECIDE,		// 決定音
	SOUND_LABEL_BOTTON001,		//カーソル移動音
	SOUND_LABEL_JUMP000,		//ジャンプ
	SOUND_LABEL_COIN,			//コイン
	SOUND_LABEL_TURN,
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
