//=============================================================================
//
// �J�������� [camera.h]
// Author :
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �J�����̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 posVDest;		// �ړI�̎��_
	D3DXVECTOR3 posRDest;		// �ړI�̒����_
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X	�i�e���r�j
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X			�i�J�����}���j
	D3DXVECTOR3 rot;			// ����
	D3DXVECTOR3 rotDest;		// �ړI�̌���
	float fLength;				// �����_�Ƃ̋���
}Camera;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

Camera *GetCamera(void);
#endif
