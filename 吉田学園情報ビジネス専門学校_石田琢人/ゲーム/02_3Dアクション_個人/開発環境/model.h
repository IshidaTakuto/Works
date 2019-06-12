//=============================================================================
//
// ���f������ [model.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// �\����
//*****************************************************************************
typedef enum
{
	MODELTYPE_OBJECT = 0,
	MODELTYPE_ITEM,
	MODELTYPE_MAX
}MODELTYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	float fDestAngle;
	int nType;
	MODELTYPE type;
	int nIdxShadow;
	D3DXVECTOR3 vtxMin, vtxMax;	// ���f���̍ŏ��l�A�ő�l
	bool bUse;
}Model;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);

bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, MODELTYPE type);
#endif
