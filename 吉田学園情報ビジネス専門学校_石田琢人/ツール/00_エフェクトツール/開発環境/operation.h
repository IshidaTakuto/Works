//=============================================================================
//
// 操作制御処理 [operation.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	MODE_EFECT = 0,
	MODE_PARTICLE,
	//MODE_TEXANIM,
	MODE_MAX
}MODE;

typedef enum
{
	TYPE_MODE = 0,
	TYPE_SAVE_FILE,
	TYPE_NUM,
	TYPE_SELECT,
	TYPE_MAX
}STRIG_TYPE;

//***************************
// キー入力のマクロ定義
//***************************
typedef struct
{
	char aKeyNamy[16];	// 画面に表示するキーの名前
	int nDIK;			// キーのDIK
}Information;

const Information MODE_CHANGE = { "[ Ｆ２ ]", DIK_F2 };		// モードの切り替え [エフェクト or パーティクル]

const Information FILE_OUTPUT = { "[ Ｆ９ ]", DIK_F9 };		// データをファイルに出力

const Information NUM_UP		 = { "[ → ]", DIK_RIGHT };		// 選択中の値を増やす
const Information NUM_DOWN	 = { "[ ← ]", DIK_LEFT };		// 選択中の値を増やす

const Information SELECT_UP	 = { "[ ↑ ]", DIK_UP };		// 選択カーソルを上げる
const Information SELECT_DOWN = { "[ ↓ ]", DIK_DOWN };		// 選択カーソルを下げる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitOperation(void);
void UninitOperation(void);
void UpdateOperation(void);
void DrawOperation(void);

#endif