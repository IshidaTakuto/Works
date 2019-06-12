//=============================================================================
//
// ���b�V���V�����_�[���� [meshCylinder.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

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
}MeshCylinder;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif
