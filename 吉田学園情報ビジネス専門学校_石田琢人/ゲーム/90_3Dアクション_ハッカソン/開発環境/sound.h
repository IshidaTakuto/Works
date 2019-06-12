//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 日角 隼人
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_		// 2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM [タイトル]
	SOUND_LABEL_BGM001,			// BGM []
	SOUND_LABEL_BGM002,			// BGM [リザルト]
	SOUND_LABEL_BGM003,			// BGM []
	SOUND_LABEL_BGM004,			// BGM []
	SOUND_LABEL_BGM005,			// BGM [ゲーム]
	SOUND_LABEL_BGM_WATER,		// 川

	SOUND_LABEL_SE_GONG_START,		// ゴング [開始]
	SOUND_LABEL_SE_GONG_END,		// ゴング [終了]
	SOUND_LABEL_SE_BREAK,			// ガード破壊
	SOUND_LABEL_SE_CUT00,			// 切る音00
	SOUND_LABEL_SE_CUT01,			// 切る音01
	SOUND_LABEL_SE_CUT02,			// 切る音02
	SOUND_LABEL_SE_CUT03,			// 切る音03
	SOUND_LABEL_SE_DECISION00,		// 決定音00
	SOUND_LABEL_SE_DECISION01,		// 決定音01
	SOUND_LABEL_SE_DECISION02,		// 決定音02
	SOUND_LABEL_SE_DECISION03,		// 決定音03
	SOUND_LABEL_SE_GRAB00,			// 掴む音00
	SOUND_LABEL_SE_GRAB01,			// 掴む音01
	SOUND_LABEL_SE_HIT000,			// 攻撃ヒット音00
	SOUND_LABEL_SE_HIT001,			// 攻撃ヒット音01
	SOUND_LABEL_SE_HIT002,			// 攻撃ヒット音02
	SOUND_LABEL_SE_HIT003,			// 攻撃ヒット音03
	SOUND_LABEL_SE_HIT004,			// 攻撃ヒット音04
	SOUND_LABEL_SE_HORAGAI,			// ほら貝
	SOUND_LABEL_SE_KEYBOARD,		// キーボード音
	SOUND_LABEL_SE_PICOHAN00,		// ピコピコハンマー00
	SOUND_LABEL_SE_PICOHAN01,		// ピコピコハンマー01
	SOUND_LABEL_SE_SELECT00,		// 選択音00
	SOUND_LABEL_SE_SELECT01,		// 選択音01
	SOUND_LABEL_SE_SWING00,			// 振る音00
	SOUND_LABEL_SE_SWING01,			// 振る音01
	SOUND_LABEL_SE_SWING02,			// 振る音02
	SOUND_LABEL_SE_WATER,			// 漕ぐ音

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);				// サウンドの初期化処理
void UninitSound(void);						// サウンドの終了処理
HRESULT PlaySound(SOUND_LABEL label);		// サウンドの再生処理(labelで再生するサウンドを指定)
void StopSound(SOUND_LABEL label);			// サウンドの停止処理(labelで停止するサウンドを指定)
void StopSound(void);						// サウンドの停止処理

#endif