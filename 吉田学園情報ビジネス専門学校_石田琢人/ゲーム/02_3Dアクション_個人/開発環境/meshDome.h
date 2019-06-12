//=============================================================================
//
// ���b�V���h�[������ [meshDome.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MESHDOME_H_
#define _MESHDOME_H_

#include "main.h"

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	float fRadius;			// ���a
	float fAngle;			// �p�x
	int nSizeX;				// X�̃T�C�Y
	int nSizeZ;				// Z�̃T�C�Y
}MeshDome;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMeshDome(void);
void UninitMeshDome(void);
void UpdateMeshDome(void);
void DrawMeshDome(void);

#endif
