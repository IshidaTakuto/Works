//=============================================================================
//
// 壁処理 [meshWall.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	int nSplitX;			// 分割数X
	int nSplitY;			// 分割数Y
	float fWidth;			// 幅
	float fHeight;			// 高さ
	int nType;
	bool bUse;
}MeshWall;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

#endif
