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
	EFFECTSTATE_RAIN,
	EFFECTSTATE_HIPFIRE,
	EFFECTSTATE_SPARK,
	EFFECTSTATE_BLOOD,
	EFFECTSTATE_MOON,
	EFFECTSTATE_GATHER,
	EFFECTSTATE_FOUNTAIN,
	EFFECTSTATE_RIPPLES,
	EFFECTSTATE_MAX
}EFFECTSTATE;

typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	D3DXVECTOR3 rot;		
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	float fRadius;			// ���a(�傫��)
	int nLife;				// �\������(����)
	EFFECTSTATE state;		// �G�t�F�N�g�̏��
	int nType;				// �e�N�X�`���̎��
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
