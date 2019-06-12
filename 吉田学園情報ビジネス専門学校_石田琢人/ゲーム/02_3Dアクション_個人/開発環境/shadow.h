//=============================================================================
//
// �|���S������ [shadow.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
// �e�̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	float fWidth;
	float fDepth;
	float fHeightWidth;
	float fHeightDepth;
	D3DXCOLOR col;
	bool bUse;
}Shadow;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepth);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void SetVtxShadow(int nIdxShadow, D3DXVECTOR3 pos, float fAlpha);
void DeleteShadow(int nIdxShadow);

Shadow *GetShadow(void);

#endif
