//=============================================================================
//
// �Ǐ��� [meshWall.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	int nSplitX;			// ������X
	int nSplitY;			// ������Y
	float fWidth;			// ��
	float fHeight;			// ����
	int nType;
	bool bUse;
}MeshWall;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

#endif
