//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PARTICLE2D_H_
#define _PARTICLE2D_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	PARTICLETYPE_NONE = 0,
	PARTICLETYPE_NORMAL,
	PARTICLETYPE_CHANGE,	// �ϐg�p
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
	int nIdx;
	D3DXMATRIX mtxWorld;
}Particle2D;

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
// �v���g�^�C�v�錾
//*****************************************************************************
void InitParticle2D(void);
void UninitParticle2D(void);
void UpdateParticle2D(void);
void DrawParticle2D(void);

void SetParticle2D(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, PARTICLETYPE type);

#endif