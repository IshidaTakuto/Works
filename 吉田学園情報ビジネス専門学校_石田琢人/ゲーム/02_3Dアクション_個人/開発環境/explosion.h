//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	D3DXMATRIX mtxWorld;	// 
	int nCountAnim;			// アニメーションカウンター
	int nPatternAnim;		// アニメーションパターンNo.
	bool bUse;				// 使用しているかどうか
} Explosion;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif
