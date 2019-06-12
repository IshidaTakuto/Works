//=============================================================================
//
// 敵残数表示処理 [remnantsEnemy.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _REMNANTSENEMY_H_
#define _REMNANTSENEMY_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	int nType;
}Remnants;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRemnants(void);
void UninitRemnants(void);
void UpdateRemnants(void);
void DrawRemnants(void);

void SetRemnants(int nRemnant);

#endif
