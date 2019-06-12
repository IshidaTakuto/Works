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
// マクロ定義
//*****************************************************************************
#define MAX_WALL		(12)		// 壁の数
#define WALL_WIDTH		(2500.0f)	// 壁の幅
#define WALL_HEIGHT		(200.0f)	// 壁の高さ

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
	float fTexPos;			// テクスチャ座標
	bool bUse;
}MeshWall;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

#endif
