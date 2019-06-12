//=============================================================================
//
// パーティクルの処理 [particle.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	PARTICLESTATE_NONE = 0,
	PARTICLESTATE_BULLET,
	PARTICLESTATE_EXPLOSION,
	PARTICLESTATE_MAX
}PARTICLESTATE;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動
	D3DXCOLOR col;			// 色
	float fRadius;			// 半径(大きさ)
	int nLife;				// 表示時間(寿命)
	int nParticles;			// 粒子の数
	PARTICLESTATE state;	// 状態
	bool bUse;				// 使用しているかどうか
} PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nParticles, PARTICLESTATE state);

#endif
