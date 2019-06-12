//=============================================================================
//
// 魔法の処理 [magic.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MAGIC_H_
#define _MAGIC_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	MAGICTYPE_NONE = 0,
	MAGICTYPE_FIRE,
	MAGICTYPE_WATER,
	MAGICTYPE_MAX
}MAGICTYPE;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXCOLOR col;			// 色
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	float fRadius;			// 半径(大きさ)
	int nLife;				// 表示時間(寿命)
	MAGICTYPE type;			// エフェクトの状態
	bool bUse;				// 使用しているかどうか
} MAGIC;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMagic(void);
void UninitMagic(void);
void UpdateMagic(void);
void DrawMagic(void);

void MakeVertexMagic(LPDIRECT3DDEVICE9 pDevice);
void SetMagic(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius,int nLife, int nCntNumber, MAGICTYPE type);

#endif
