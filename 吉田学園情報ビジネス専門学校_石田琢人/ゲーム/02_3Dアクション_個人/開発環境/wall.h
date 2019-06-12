//=============================================================================
//
// 壁処理 [wall.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	float fWidth;
	float fDepth;
	D3DXCOLOR col;
	bool bUse;
}Wall;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);

void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif
