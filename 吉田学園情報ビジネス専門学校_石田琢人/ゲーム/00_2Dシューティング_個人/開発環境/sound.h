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
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_SE_SHOT000,		// 弾発射音
	SOUND_LABEL_SE_HIT000,		// ヒット音0
	SOUND_LABEL_SE_HIT001,		// ヒット音1
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_DECIDE,		// 決定音
	SOUND_LABEL_SE_BEAM,		// ビーム音
	SOUND_LABEL_SE_GAUGE,		// ゲージ満タン
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
