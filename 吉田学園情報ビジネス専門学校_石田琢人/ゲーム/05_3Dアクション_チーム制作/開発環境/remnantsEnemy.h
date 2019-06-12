//=============================================================================
//
// �G�c���\������ [remnantsEnemy.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _REMNANTSENEMY_H_
#define _REMNANTSENEMY_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	int nType;
}Remnants;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitRemnants(void);
void UninitRemnants(void);
void UpdateRemnants(void);
void DrawRemnants(void);

void SetRemnants(int nRemnant);

#endif
