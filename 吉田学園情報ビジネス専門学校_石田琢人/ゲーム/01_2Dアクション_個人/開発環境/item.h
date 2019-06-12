//=============================================================================
//
// アイテム処理 [item.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_ITEM	(128)	// アイテムの最大数

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	ITEMTYPE_NONE = 0,
	ITEMTYPE_EGG,		// コイン
	ITEMTYPE_DIRTY,		// 汚れ
	ITEMTYPE_MAX		// アイテム種類の総数
}ITEMTYPE;

typedef enum
{
	ITEMSTATE_APPEAR = 0,
	ITEMSTATE_NORMAL,
	ITEMSTATE_GET,
	ITEMSTATE_MOVE,
	ITEMSTATE_MAX
}ITEMSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	float fWidth;		// 幅
	float fHeight;		// 高さ
	float fAngle;		// 角度
	float fLength;		// 頂点までの長さ
	int nCounterAnim;	// アニメーションのカウンター
	int nPatternAnim;	// アニメーションパターンNo.
	ITEMTYPE type;		// アイテムの種類
	ITEMSTATE state;	// 状態
	int nCntState;		// 状態のカウンター
	bool bUse;			// 使用されているかどうか
}ITEM;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, ITEMTYPE state);
ITEM *GetItem(void);

#endif