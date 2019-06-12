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
// �}�N����`
//*****************************************************************************
#define MAX_WALL		(12)		// �ǂ̐�
#define WALL_WIDTH		(2500.0f)	// �ǂ̕�
#define WALL_HEIGHT		(200.0f)	// �ǂ̍���

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
	float fTexPos;			// �e�N�X�`�����W
	bool bUse;
}MeshWall;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

#endif
