//=============================================================================
//
// �G�t�F�N�g�̏��� [effect.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	EFFECTSTATE_NONE = 0,
	EFFECTSTATE_BULLET,
	EFFECTSTATE_EXPLOSION,
	EFFECTSTATE_BEAM,
	EFFECTSTATE_TITLE,
	EFFECTSTATE_MAX
}EFFECTSTATE;

typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	float fRadius;			// ���a(�傫��)
	int nLife;				// �\������(����)
	EFFECTSTATE state;		// �G�t�F�N�g�̏��
	bool bUse;				// �g�p���Ă��邩�ǂ���
} EFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, EFFECTSTATE state);

#endif
