//=============================================================================
//
// ポリゴン処理 [shadow.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
// 影の構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	float fWidth;
	float fDepth;
	float fHeightWidth;
	float fHeightDepth;
	D3DXCOLOR col;
	bool bUse;
}Shadow;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepth);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void SetVtxShadow(int nIdxShadow, D3DXVECTOR3 pos, float fAlpha);
void DeleteShadow(int nIdxShadow);

Shadow *GetShadow(void);

#endif
