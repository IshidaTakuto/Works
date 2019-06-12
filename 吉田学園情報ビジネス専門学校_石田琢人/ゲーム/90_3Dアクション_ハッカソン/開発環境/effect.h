//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// エフェクトの処理 [effect.h]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// エフェクトの種類
typedef enum
{
	EFFECTTYPE_WEAK_ATTACK = 0,		// 弱攻撃
	EFFECTTYPE_STRONG_ATTACK,		// 強攻撃
	EFFECTTYPE_AKR,					// 妖刀『A・K・R』
	EFFECTTYPE_3DBULLET,			// 投てき武器
	EFFECTTYPE_MAX
}EFFECTTYPE;

// テクスチャの情報
typedef struct
{
	char *pFilename;		// テクスチャ名
}EffectTexture;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rot;		// 向き
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXCOLOR col;			// 色
	float fRadius;			// 半径(大きさ)
	int nLife;				// 表示時間(寿命)
	int nType;				// 種類
	bool bUse;				// 使用しているかどうか
} EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nType);
#endif
