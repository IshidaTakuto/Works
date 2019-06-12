//=============================================================================
//
// 花びら処理 [petals.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PETALS_H_
#define _PETALS_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 angle;
	D3DXCOLOR col;
	float fRadius;
	bool bUse;
	int nType;
	bool bMoveAngle;		// 縦の緩急
	bool bMoveWidth;		// 横のゆらゆら
	D3DXMATRIX mtxWorld;
}PETALS;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPetals(void);
void UninitPetals(void);
void UpdatePetals(void);
void DrawPetals(void);

void SetPetals(D3DXVECTOR3 pos, int nNumber);
#endif