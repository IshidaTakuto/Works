//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// カウントシーン処理 [countScene.h]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef _COUNTSCENE_H_
#define _COUNTSCENE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_COUNTSCENE	(1)		// カウントシーンの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	char *pFilename;		// ファイル名
}CountSceneTexInfo;

//--- 種類 ---//
typedef enum
{
	COUNTSCENETYPE_START = 0,		// スタート
	COUNTSCENETYPE_GOAL,			// ゴール
	COUNTSCENETYPE_MAX
}COUNTSCENETYPE;

//--- 状態 ---//
typedef enum
{
	COUNTSCENESTATE_NONE = 0,		// 何もしていない
	COUNTSCENESTATE_IN,				// カウントシーンイン
	COUNTSCENESTATE_WAIT,			// カウントシーン待機
	COUNTSCENESTATE_OUT,			// カウントシーンアウト
	COUNTSCENESTATE_MAX
}COUNTSCENESTATE;

// 情報
typedef struct
{
	D3DXCOLOR color;					// 色
	COUNTSCENETYPE type;				// 種類
	COUNTSCENESTATE countSceneState;	// カウントシーンの状態
	int countSceneStateCounter;			// 状態カウンター
}COUNTSCENE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitcountScene(void);
void UninitcountScene(void);
void UpdatecountScene(void);
void DrawcountScene(void);
void SetcountScene(COUNTSCENETYPE type);

#endif
