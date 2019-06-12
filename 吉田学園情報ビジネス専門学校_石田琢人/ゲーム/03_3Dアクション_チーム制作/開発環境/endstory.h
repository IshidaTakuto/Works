#pragma once
//=============================================================================
//
// エンドストーリーの処理 [endstory.h]
// Author : ABE YUUTAROU
//
//=============================================================================
#ifndef _ENDSTORY_H_
#define _ENDSTORY_H_

#include "main.h"

//*****************************************************************************
//構造体
//*****************************************************************************
typedef enum
{
	ENDSTORY_000 = 0,		//通常
	ENDSTORY_001,				//END
	ENDSTORY_002,
	ENDSTORY_003,
	ENDSTORY_004,
	ENDSTORY_005,
	ENDSTORY_006,
	ENDSTORY_007,
	ENDSTORY_008,
	ENDSTORY_009,
	ENDSTORY_010,
	ENDSTORY_011,
	ENDSTORY_012,
	ENDSTORY_013,
	ENDSTORY_014,
	ENDSTORY_015,
	ENDSTORY_016,
	ENDSTORY_MAX
}ENDSTORYSTATE;

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
void InitEndStory(void);		//初期化処理ストーリー
void UninitEndStory(void);		//終了処理ストーリー
void UpdateEndStory(void);		//更新処理タイトル
void DrawEndStory(void);		//描画処理タイトル
ENDSTORYSTATE GetEndStstate(void);//
int GetEndStoryMenu(void);
#endif

