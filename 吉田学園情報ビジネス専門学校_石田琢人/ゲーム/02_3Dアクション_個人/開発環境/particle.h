//=============================================================================
//
// パーティクル処理 [particle.h]
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
	PARTICLETYPE_NONE = 0,
	PARTICLETYPE_NORMAL,
	PARTICLETYPE_SPARK,		// 火花
	PARTICLETYPE_SMOKE,
	PARTICLETYPE_FIRE,
	PARTICLETYPE_FLOWERS,
	PARTICLETYPE_ELECT,
	PARTICLETYPE_BLOOD,
	PARTICLETYPE_GATHER,
	PARTICLETYPE_FOUNTAIN,
	PARTICLETYPE_MAX
}PARTICLETYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXCOLOR col;
	PARTICLETYPE type;
	float fAngle;
	float fRadius;
	int nLife;
	int nType;
	bool bUse;
	D3DXMATRIX mtxWorld;
}Particle;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	PARTICLETYPE type;
	float fRadius;
	int nLife;
	bool bUse;
}Emitter;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, PARTICLETYPE type);

#endif