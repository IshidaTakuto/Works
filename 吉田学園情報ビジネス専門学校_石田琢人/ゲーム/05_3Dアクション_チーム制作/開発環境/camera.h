//=============================================================================
//
// カメラ処理 [camera.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// カメラの構造体
//*****************************************************************************
typedef enum
{
	CAMERATYPE_NONE = 0,
	CAMERATYPE_TITLE,
	CAMERATYPE_GAME,
	CAMERATYPE_RANKING,
	CAMERATYPE_MAX
}CAMERATYPE;

typedef struct
{
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 posVDest;		// 目的の視点
	D3DXVECTOR3 posRDest;		// 目的の注視点
	D3DXVECTOR3 vecU;			// 上方向ベクトル
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス	（テレビ）
	D3DXMATRIX mtxView;			// ビューマトリックス			（カメラマン）
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 rotDest;		// 目的の向き
	float fLength;				// 注視点との距離
	CAMERATYPE type;			// カメラの種類
	float fAngle;				//角度
}Camera;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(CAMERATYPE cameratype);
void DeleteCamera(void);

Camera *GetCamera(void);
#endif
