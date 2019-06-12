//=============================================================================
//
// ���b�V���I�[�r�b�g���� [meshOrbit.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "main.h"

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	int nType;				// �e�N�X�`��
	D3DXCOLOR col;			// �J���[
	D3DXVECTOR3 start;		// �J�n���W
	D3DXVECTOR3 end;		// �I�����W
	int nTime;				// �c�鎞��
	bool bUse;
}MeshOrbit;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMeshOrbit(void);
void UninitMeshOrbit(void);
void UpdateMeshOrbit(void);
void DrawMeshOrbit(void);

void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col, int nTime, int nType);

#endif
