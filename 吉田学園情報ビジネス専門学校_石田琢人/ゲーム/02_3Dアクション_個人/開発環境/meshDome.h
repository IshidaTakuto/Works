//=============================================================================
//
// メッシュドーム処理 [meshDome.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MESHDOME_H_
#define _MESHDOME_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	float fRadius;			// 半径
	float fAngle;			// 角度
	int nSizeX;				// Xのサイズ
	int nSizeZ;				// Zのサイズ
}MeshDome;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshDome(void);
void UninitMeshDome(void);
void UpdateMeshDome(void);
void DrawMeshDome(void);

#endif
