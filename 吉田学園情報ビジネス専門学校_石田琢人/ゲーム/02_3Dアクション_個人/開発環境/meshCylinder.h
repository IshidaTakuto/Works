//=============================================================================
//
// メッシュシリンダー処理 [meshCylinder.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

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
}MeshCylinder;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif
