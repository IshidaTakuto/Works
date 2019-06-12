//=============================================================================
//
// ジェネレータ処理 [generator.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "main.h"
#include "block.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 des;
	BLOCKTYPE type;
	float fWidth;
	float fHeight;
	int interval;
	bool bUse;
}GENERATOR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGenerator(void);
void UninitGenerator(void);
void UpdateGenerator(void);
void DrawGenerator(void);
void SetGenerator(D3DXVECTOR3 pos, BLOCKTYPE type, float fWidth, float fHeight, D3DXVECTOR3 des, int interval);

#endif