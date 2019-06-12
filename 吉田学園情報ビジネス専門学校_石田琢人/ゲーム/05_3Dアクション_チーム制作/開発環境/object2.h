//=============================================================================
//
// ���f������ [object2.h]
// Author : �L�n�@���u
//
//=============================================================================
#ifndef _OBJECT2_H_
#define _OBJECT2_H_

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
}Object2;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitObject2(void);
void UninitObject2(void);
void UpdateObject2(void);
void DrawObject2(void);
Object2 *GetObject2(void);
void DeleteObject2(void);

bool CollisionObject2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
void SetObject2(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif
