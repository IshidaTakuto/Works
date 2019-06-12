//=============================================================================
//
// �Ԃт珈�� [petals.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PETALS_H_
#define _PETALS_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	PETALSTYPE_SAKURA = 0,
	PETALSTYPE_MOMIJI,
	PETALSTYPE_MAX
}PETALSTYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 angle;
	D3DXCOLOR col;
	float fRadius;
	bool bUse;
	int nType;
	PETALSTYPE type;
	D3DXMATRIX mtxWorld;
}PETALS;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPetals(void);
void UninitPetals(void);
void UpdatePetals(PETALSTYPE type);
void DrawPetals(void);

#endif