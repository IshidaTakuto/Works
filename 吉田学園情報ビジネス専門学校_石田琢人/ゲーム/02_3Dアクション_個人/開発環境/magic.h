//=============================================================================
//
// ���@�̏��� [magic.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MAGIC_H_
#define _MAGIC_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	MAGICTYPE_NONE = 0,
	MAGICTYPE_FIRE,
	MAGICTYPE_WATER,
	MAGICTYPE_MAX
}MAGICTYPE;

typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXCOLOR col;			// �F
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	float fRadius;			// ���a(�傫��)
	int nLife;				// �\������(����)
	MAGICTYPE type;			// �G�t�F�N�g�̏��
	bool bUse;				// �g�p���Ă��邩�ǂ���
} MAGIC;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMagic(void);
void UninitMagic(void);
void UpdateMagic(void);
void DrawMagic(void);

void MakeVertexMagic(LPDIRECT3DDEVICE9 pDevice);
void SetMagic(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius,int nLife, int nCntNumber, MAGICTYPE type);

#endif
