//=============================================================================
//
// ブロック処理 [block.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BLOCK	(255)	// 敵の最大数
#define BLOCK_SIZE	(100.0f)	// ブロックの大きさ
#define SCROLL_SPEED	(1.5f)	// スクロールする速さ

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	BLOCKTYPE_NORMAL = 0,
	BLOCKTYPE_VERTICAL,
	BLOCKTYPE_HORIZONTAL,
	BLOCKTYPE_TITLE,
	BLOCKTYPE_BELT_LEFT,
	BLOCKTYPE_BELT_RIGHT,
	BLOCKTYPE_FALL,
	BLOCKTYPE_GOAL,
	BLOCKTYPE_SHARK,
	BLOCKTYPE_ENEMY,
	BLOCKTYPE_MAX
}BLOCKTYPE;

typedef enum
{
	BLOCKSTATE_NONE = 0,
	BLOCKSTATE_NORMAL,
	BLOCKSTATE_MOVE,
	BLOCKSTATE_CLEAR,
	BLOCKSTATE_MAX
}BLOCKSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 posOld; // 前回の位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 des;	// 目的地
	int nType;			// テクスチャタイプ
	int nCounterAnim;	// アニメーションのカウンター
	int nPatternAnim;	// アニメーションパターンNo.
	int nDirectionMove;	// 向いてる方向
	float fWidth;		// 幅
	float fHeight;		// 高さ
	int nCounterState;	// ステートのカウンター
	int nCntMove;		// 追ってくる時間
	BLOCKTYPE type;		// 種類
	BLOCKSTATE state;	// 状態
	int nVertical;		// 上下移動
	bool bScroll;		// 強制スクロール
	bool bUse;			// 使用しているかどうか
}BLOCK;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 des, BLOCKTYPE type);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float *pHeight, float *pWidth);
BLOCK *GetBlock(void);

#endif
