//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// カメラの構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			// 視点		[現在]
	D3DXVECTOR3 posR;			// 注視点	[現在]
	D3DXVECTOR3 posVDest;		// 視点		[目的]
	D3DXVECTOR3 posRDest;		// 注視点	[目的]
	D3DXVECTOR3 vecU;			// 上方向ベクトル
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
	D3DXVECTOR3 rot;			// 視点の向き [現在]
	D3DXVECTOR3 rotDest;		// 視点の向き [目的]
	float fLength;				// 視点と注視点の距離
}Camera;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

Camera *GetCamera(void);
#endif
