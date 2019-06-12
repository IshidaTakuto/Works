//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "block.h"
#include "item.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_SPEED		(5.0f)	// プレイヤーの移動量
#define JUMP_HEIGHT		(-30)	// ジャンプの高さ

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	PLAYERSTATE_NONE = 0,	// 何もしていない状態
	PLAYERSTATE_APPEAR,		// 出現状態
	PLAYERSTATE_NORMAL,		// 通常状態
	PLAYERSTATE_DEATH,		// 死亡状態
	PLAYERSTATE_CLEAR,		// ゲームクリア状態
	PLAYERSTATE_MAX			// 状態の総数
}PLAYERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;		// 現在の位置
	D3DXVECTOR3 posOld;		// 前回の位置
	D3DXVECTOR3 move;		// 移動量
	float fHeight;			// 高さ
	float fWidth;			// 幅
	int nCounterAnim;		// アニメーションのカウンター
	int nPatternAnim;		// アニメーションパターンNo.
	int nDirectionMove;		// プレイヤーの向いてる方向
	bool bJump;				// ジャンプ中かどうか
	bool bDisp;				// 描画されているかいないか
	int nCounterState;		// ステートのカウンター
	BLOCK BlockOld;			// 最後に触ったブロック
	PLAYERSTATE state;		// 状態
	BLOCK *pBlock;			// 対象のブロックへのポインタ
}PLAYER;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER GetPlayer(void);
void CollisionItem(void);

#endif