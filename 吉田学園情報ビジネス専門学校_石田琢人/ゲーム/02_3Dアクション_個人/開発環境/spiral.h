//=============================================================================
//
// 螺旋処理 [spiral.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _SPIRAL_H_
#define _SPIRAL_H_

#include "main.h"

#define SPIRAL_RING			(100)
#define SPIRAL_POWERUP		(4)
#define SPIRAL_MAGIC		(10)

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef enum
{
	SPIRALTYPE_NONE = 0,
	SPIRALTYPE_RING,
	SPIRALTYPE_POWERUP,
	SPIRALTYPE_MAGIC,
	SPIRALTYPE_MAX
}SPIRALTYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXCOLOR col;
	int nCntAngle;
	float fSize;
	float fHeight;
	float fRadius;
	SPIRALTYPE type;
	D3DXVECTOR3 PlayerPos;
	bool bUse;
}Spiral;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSpiral(void);
void UninitSpiral(void);
void UpdateSpiral(void);
void DrawSpiral(void);

void MakeVertexSpiral(LPDIRECT3DDEVICE9 pDevice);

void SetSpiral(D3DXVECTOR3 pos, D3DXCOLOR col, SPIRALTYPE type);

#endif
