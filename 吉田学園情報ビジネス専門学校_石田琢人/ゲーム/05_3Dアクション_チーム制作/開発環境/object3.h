//=============================================================================
//
// ���f������ [object3.h]
// Author : �L�n�@���u
//
//=============================================================================
#ifndef _OBJECT3_H_
#define _OBJECT3_H_

#include "main.h"

//*****************************************************************************
// ���f���̍\����
//*****************************************************************************
typedef struct
{
	LPD3DXMESH		pMeshObject = NULL;			//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	pBuffMatObject = NULL;		//�}�e���A���̏��ւ̃|�C���^
	DWORD			nNumMatObject = 0;			//�}�e���A���̏��
	D3DXMATRIX		mtxWorldObject;				//���[���h�}�g���b�N�X
	D3DXVECTOR3		pos;						//�ʒu
	D3DXVECTOR3		rot;						//����
	bool			bUse;						//���
	D3DXVECTOR3		VtxMinObject, VtxMaxObject;	//���f���̍ŏ��l�E�ő�l
	LPDIRECT3DTEXTURE9* pMeshTextures = NULL;
}Object3;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitObject3(void);
void UninitObject3(void);
void UpdateObject3(void);
void DrawObject3(void);
Object3 *GetObject3(void);
void DeleteObject3(void);

bool CollisionObject3(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
void SetObject3(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif
