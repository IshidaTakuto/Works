//=============================================================================
//
// �p�[�e�B�N���̏��� [particle.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
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
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ�
	D3DXCOLOR col;			// �F
	float fRadius;			// ���a(�傫��)
	int nLife;				// �\������(����)
	int nParticles;			// ���q�̐�
	PARTICLESTATE state;	// ���
	bool bUse;				// �g�p���Ă��邩�ǂ���
} PARTICLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nParticles, PARTICLESTATE state);

#endif
