//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
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
	SOUND_LABEL_SELECT_000,	// キャラクター作成
	SOUND_LABEL_STORY_000,	// キャラクター作成
	SOUND_LABEL_ENDSTORY,		// BOSSBGM
	SOUND_LABEL_ENDROLL,		// BOSSBGM
	SOUND_LABEL_CREDIT,		// BOSSBGM
	SOUND_LABEL_GAMEOVER,		// BOSSBGM
	SOUND_LABEL_CHAR_MAKE000,	// キャラクター作成
	SOUND_LABEL_FBGM000,		// FIELDBGM0
	SOUND_LABEL_BBGM001,		// BOSSBGM
	

	SOUND_LABEL_CLEAR,			//ボス名前出すときに
	SOUND_LABEL_OVER,			//ボス名前出すときに

	SOUND_LABEL_BOTTON000,		//決定音
	SOUND_LABEL_BOTTON001,		//キャンセル音
	SOUND_LABEL_LOCKON,			//lock-on
	SOUND_LABEL_LOCKOFF,			//lock-off

	SOUND_LABEL_TitleSE000,		//効果音0
	SOUND_LABEL_TitleSE001,		//効果音1//死亡時

	SOUND_LABEL_BOTTON002,		//カーソル移動音
	SOUND_LABEL_BOTTON003,		//スコアカウンター
	SOUND_LABEL_ATTACK000,		//攻撃音
	SOUND_LABEL_ATTACK001,		//攻撃爆発
	SOUND_LABEL_ATTACK002,		//攻撃火炎

	SOUND_LABEL_ATTACK400,		//攻撃爆発
	SOUND_LABEL_ATTACK402,		//攻撃爆発
	SOUND_LABEL_ATTACK403,		//攻撃雷
	SOUND_LABEL_ATTACK404,		//攻撃雷


	SOUND_LABEL_Damage000,		//被ダメ
	SOUND_LABEL_Damage001,		//被ダメ
	SOUND_LABEL_Damage002,		//被ダメ
	SOUND_LABEL_GUARD000,		//ガード
	SOUND_LABEL_JUMP000,		//ジャンプ		
	SOUND_LABEL_DASH000,		//ブースト
	SOUND_LABEL_DEAD_END,		//カーソル移動音
	SOUND_LABEL_BOSS_D,			//ボス撃破
	SOUND_LABEL_BOSS_N,			//ボス名前出すときに
	SOUND_LABEL_SKILL,			//ボス名前出すときに
	SOUND_LABEL_STORY_SE,			//ボス名前出すときに
	SOUND_LABEL_ENTER,			//ボス名前出すときに
	SOUND_LABEL_SELECTMOVE,			//ボス名前出すときに

	SOUND_LABEL_HELL,			//回復
	SOUND_LABEL_UP,				//ステアップ
	SOUND_LABEL_Right,			//雷


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
