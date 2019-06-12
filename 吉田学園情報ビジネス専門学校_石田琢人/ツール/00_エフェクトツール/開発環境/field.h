//=============================================================================
//
// 水面処理 [field.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	int nType;
}Field;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

//bool CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

#endif
