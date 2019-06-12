//=============================================================================
//
// エフェクトの処理 [effect.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	EFFECTSTATE_NONE = 0,
	EFFECTSTATE_BULLET,
	EFFECTSTATE_RAIN,
	EFFECTSTATE_HIPFIRE,
	EFFECTSTATE_SPARK,
	EFFECTSTATE_BLOOD,
	EFFECTSTATE_MOON,
	EFFECTSTATE_GATHER,
	EFFECTSTATE_FOUNTAIN,
	EFFECTSTATE_RIPPLES,
	EFFECTSTATE_MAX
}EFFECTSTATE;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	D3DXVECTOR3 rot;		
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	float fRadius;			// 半径(大きさ)
	int nLife;				// 表示時間(寿命)
	EFFECTSTATE state;		// エフェクトの状態
	int nType;				// テクスチャの種類
	bool bUse;				// 使用しているかどうか
} EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, EFFECTSTATE state);

#endif
