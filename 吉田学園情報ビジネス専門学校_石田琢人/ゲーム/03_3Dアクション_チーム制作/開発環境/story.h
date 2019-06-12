//=============================================================================
//
// ストーリーの処理 [story.h]
// Author : ABE YUUTAROU
//
//=============================================================================
#ifndef _STORY_H_
#define _STORY_H_

#include "main.h"

//*****************************************************************************
//構造体
//*****************************************************************************
typedef enum
{
	STORY_000 = 0,		//通常
	STORY_001,				//END
	STORY_002,
	STORY_003,
	STORY_004,
	STORY_005,
	STORY_006,
	STORY_007,
	STORY_008,
	STORY_009,
	STORY_010,
	STORY_011,
	STORY_012,
	STORY_013,
	STORY_014,
	STORY_015,
	STORY_016,
	STORY_017,
	STORY_MAX
}STORYSTATE;

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
void InitStory(void);		//初期化処理ストーリー
void UninitStory(void);		//終了処理ストーリー
void UpdateStory(void);		//更新処理タイトル
void DrawStory(void);		//描画処理タイトル
STORYSTATE GetStstate(void);//
int GetStoryMenu(void);
#endif

