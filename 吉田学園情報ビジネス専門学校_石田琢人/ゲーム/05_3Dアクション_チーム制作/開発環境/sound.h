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
	SOUND_LABEL_TITLE = 0,		//タイトル
	SOUND_LABEL_STAGE1,			//ステージ1
	SOUND_LABEL_STAGE2,			//ステージ2
	SOUND_LABEL_STAGE3,			//ステージ3
	SOUND_LABEL_DIERANKING,		//死にランキング
	SOUND_LABEL_RANKING,		//ランキング
	SOUND_LABEL_ENEMYVOICE,		//エネミーボイス
	SOUND_LABEL_HIT,			//エネミーヒット
	SOUND_LABEL_RAIN,			//雨
	SOUND_LABEL_SELECT,			//決定音
	SOUND_LABEL_SELECT2,		//決定音
	SOUND_LABEL_SWORDGESTURE,	//ソード空振り
	SOUND_LABEL_COMBO,			//コンボ音
	SOUND_LABEL_SWORDHIT,		//ソードヒット
	SOUND_LABEL_PAUSE,			//ポーズ
	SOUND_LABEL_CANCEL,			//キャンセル
	SOUND_LABEL_SENTOU,			//戦闘開始
	SOUND_LABEL_SEIATU,			//制圧完了
	SOUND_LABEL_HUNDUP,			//手を挙げる
	SOUND_LABEL_BREATH,			// ドラゴンの攻撃
	SOUND_LABEL_NEXTSTAGE,		// ステージ移動
	SOUND_LABEL_FOOTSTEPS,		// 足音
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
