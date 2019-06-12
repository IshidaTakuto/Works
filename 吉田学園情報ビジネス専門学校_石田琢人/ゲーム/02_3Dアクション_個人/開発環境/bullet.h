//=============================================================================
//
// 弾処理 [bullet.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	float fWidth;
	float fHeight;
	D3DXCOLOR col;
	int nIdxShadow;
	bool bUse;
}Bullet;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 rot);
bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax);

#endif
