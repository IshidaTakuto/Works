//=============================================================================
//
// ���f������ [Object2.cpp]
// Author : �L�n ���u
//
//=============================================================================
#include "object2.h"	//�I�u�W�F�N�g
#include "input.h"		//�L�[�{�[�h
#include "camera.h"		//�J����
#include "shadow.h"		//�e
#include "player.h"		//�v���C���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define OBJECT2_NAME		"data\\MODEL\\��3.x"			//�ǂݍ��ރe�N�X�`���t�@�C��
#define MAX_TEX2			(1)								//�I�u�W�F�N�g�̃e�N�X�`���ő吔
#define MAX_OBJECT2			(83)								//�I�u�W�F�N�g�̍ő吔
#define MAX_VARIATION2		(1)								//�I�u�W�F�N�g�̎��
#define PULS2				(35)							//�����蔻��
#define PULS2_X				(35)							//�����蔻��		

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Object2 g_Object2[MAX_OBJECT2];

//=============================================================================
// ����������
//=============================================================================
void InitObject2(void)
{
	//�f�o�C�X�|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���̓ǂݍ���	
	D3DXLoadMeshFromX
	(
		OBJECT2_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Object2[0].pBuffMatObject,
		NULL, &g_Object2[0].nNumMatObject,
		&g_Object2[0].pMeshObject
	);
	
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 *pMatDef;

	//�e�N�X�`��������
	for (int nCount = 0; nCount < MAX_VARIATION2; nCount++)
	{
		pMat = (D3DXMATERIAL*)g_Object2[nCount].pBuffMatObject->GetBufferPointer();
		pMatDef = new D3DMATERIAL9[g_Object2[nCount].nNumMatObject];
		g_Object2[nCount].pMeshTextures = new LPDIRECT3DTEXTURE9[g_Object2[nCount].nNumMatObject];

		for (DWORD tex = 0; tex < g_Object2[nCount].nNumMatObject; tex++)
		{
			pMatDef[tex] = pMat[tex].MatD3D;
			pMatDef[tex].Ambient = pMatDef[tex].Diffuse;
			g_Object2[nCount].pMeshTextures[tex] = NULL;
			if (pMat[tex].pTextureFilename != NULL &&
				lstrlen(pMat[tex].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pMat[tex].pTextureFilename,
					&g_Object2[nCount].pMeshTextures[tex])))
				{
					//MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
				}
			}
		}
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT2; nCntObject++)
	{
		g_Object2[nCntObject].VtxMinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_Object2[nCntObject].VtxMaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_Object2[nCntObject].pos = D3DXVECTOR3(10000, 0, 0);
		g_Object2[nCntObject].rot = D3DXVECTOR3(0, 0, 0);
		g_Object2[nCntObject].bUse = false;
	}

	int nNumVtx;			//���_��
	DWORD sizeFVF;			//���_�t�H�[�}�b�g7�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^

	for (int nCount = 0; nCount < MAX_VARIATION2; nCount++)
	{
		//���_�����擾
		nNumVtx = g_Object2[nCount].pMeshObject->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_Object2[nCount].pMeshObject->GetFVF());

		//���_�o�b�t�@�����b�N
		g_Object2[nCount].pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //���_���W�̑��

													   //�͈͎w��
													   //�ő�l
			if (vtx.x < g_Object2[nCount].VtxMinObject.x)
			{
				g_Object2[nCount].VtxMinObject.x = vtx.x;
			}
			if (vtx.z < g_Object2[nCount].VtxMinObject.z)
			{
				g_Object2[nCount].VtxMinObject.z = vtx.z;
			}
			if (vtx.y < g_Object2[nCount].VtxMinObject.y)
			{
				g_Object2[nCount].VtxMinObject.y = vtx.y;
			}
			//�ŏ��l
			if (vtx.x > g_Object2[nCount].VtxMaxObject.x)
			{
				g_Object2[nCount].VtxMaxObject.x = vtx.x;
			}
			if (vtx.z > g_Object2[nCount].VtxMaxObject.z)
			{
				g_Object2[nCount].VtxMaxObject.z = vtx.z;
			}
			if (vtx.y > g_Object2[nCount].VtxMaxObject.y)
			{
				g_Object2[nCount].VtxMaxObject.y = vtx.y;
			}
			pVtxBuff += sizeFVF;		//�T�C�Y���|�C���^��i�߂�
		}
		//���_�o�b�t�@���A�����b�N
		g_Object2[nCount].pMeshObject->UnlockVertexBuffer();
	}

	//�I�u�W�F�N�g�̑��
	//for(�J�E���g�@= �S�̂̃I�u�W�F�N�g�̐����ŏ���+����)
	for (int nPotaCountObject = MAX_VARIATION2; nPotaCountObject < MAX_OBJECT2; nPotaCountObject++)
	{
		if (nPotaCountObject < MAX_OBJECT2 + MAX_VARIATION2)
		{//����̂ł�����
			g_Object2[nPotaCountObject] = g_Object2[0];
		}
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitObject2(void)
{
	for (int nCntObject = 0; nCntObject < MAX_VARIATION2; nCntObject++)
	{
		// ���b�V���̊J��
		if (g_Object2[nCntObject].pMeshObject != NULL)
		{
			g_Object2[nCntObject].pMeshObject->Release();
			g_Object2[nCntObject].pMeshObject = NULL;
		}
		// �}�e���A���̊J��
		if (g_Object2[nCntObject].pBuffMatObject != NULL)
		{
			g_Object2[nCntObject].pBuffMatObject->Release();
			g_Object2[nCntObject].pBuffMatObject = NULL;
		}

		//�e�N�X�`��
		for (DWORD nTex = 0; nTex < g_Object2[nCntObject].nNumMatObject; nTex++)
		{
			if (g_Object2[nCntObject].pMeshTextures[nTex] != NULL)
			{
				g_Object2[nCntObject].pMeshTextures[nTex]->Release();
				g_Object2[nCntObject].pMeshTextures[nTex] = NULL;
			}
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateObject2(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawObject2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	for (int nCount2 = 0; nCount2 < MAX_OBJECT2; nCount2++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Object2[nCount2].mtxWorldObject);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object2[nCount2].rot.y, g_Object2[nCount2].rot.x, g_Object2[nCount2].rot.z);

		D3DXMatrixMultiply(&g_Object2[nCount2].mtxWorldObject, &g_Object2[nCount2].mtxWorldObject, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Object2[nCount2].pos.x, g_Object2[nCount2].pos.y, g_Object2[nCount2].pos.z);

		D3DXMatrixMultiply(&g_Object2[nCount2].mtxWorldObject, &g_Object2[nCount2].mtxWorldObject, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Object2[nCount2].mtxWorldObject);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Object2[nCount2].pBuffMatObject->GetBufferPointer();

		for (int nCntMat = 0; nCntMat<(int)g_Object2[nCount2].nNumMatObject; nCntMat++)
		{
			if (g_Object2[nCount2].bUse == true)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Object2[nCount2].pMeshTextures[nCntMat]);
				// ���f��(�p�[�c)�̕`��
				g_Object2[nCount2].pMeshObject->DrawSubset(nCntMat);
			}
		}
		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
}
//=============================================================================
// �Z�b�g���f��
//=============================================================================
void SetObject2(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntObject;

	for (nCntObject = 0; nCntObject < MAX_OBJECT2; nCntObject++)
	{
		if (g_Object2[nCntObject].bUse == false)
		{
			g_Object2[nCntObject].pos = pos;
			g_Object2[nCntObject].rot = rot;
			g_Object2[nCntObject].bUse = true;
			break;
		}
	}
}
//=============================================================================
// ���f���̓����蔻��
//=============================================================================
bool CollisionObject2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove)
{
	int nCount;
	bool bLand = false;
	Player * pPlayer = GetPlayer();

	for (nCount = 0; nCount < MAX_OBJECT2; nCount++)
	{
		if (pPos->x > g_Object2[nCount].pos.x + g_Object2[nCount].VtxMinObject.x - PULS2_X && pPos->x < g_Object2[nCount].pos.x + g_Object2[nCount].VtxMaxObject.x + PULS2_X)
		{	//X�͈̔�
			if (pPosold->z >= g_Object2[nCount].pos.z + g_Object2[nCount].VtxMaxObject.z + PULS2_X && pPos->z < g_Object2[nCount].pos.z + g_Object2[nCount].VtxMaxObject.z + PULS2_X)
			{
				pPos->z = g_Object2[nCount].pos.z + g_Object2[nCount].VtxMaxObject.z + PULS2_X;
				pPos->z = pPosold->z;
			}
			else if (pPosold->z <= g_Object2[nCount].pos.z + g_Object2[nCount].VtxMinObject.z - PULS2_X && pPos->z > g_Object2[nCount].pos.z + g_Object2[nCount].VtxMinObject.z - PULS2_X)
			{
				pPos->z = g_Object2[nCount].pos.z + g_Object2[nCount].VtxMinObject.z - PULS2_X;
				pPos->z = pPosold->z;
			}
		}
		if (pPos->z > g_Object2[nCount].pos.z + g_Object2[nCount].VtxMinObject.z - PULS2 && pPos->z < g_Object2[nCount].pos.z + g_Object2[nCount].VtxMaxObject.z + PULS2)
		{	//Z�͈̔�
			if (pPosold->x >= g_Object2[nCount].pos.x + g_Object2[nCount].VtxMaxObject.x + PULS2 && pPos->x < g_Object2[nCount].pos.x + g_Object2[nCount].VtxMaxObject.x + PULS2)
			{
				pPos->x = g_Object2[nCount].pos.x + g_Object2[nCount].VtxMaxObject.x + PULS2;
				pPos->x = pPosold->x;
			}
			else if (pPosold->x <= g_Object2[nCount].pos.x + g_Object2[nCount].VtxMinObject.x - PULS2 && pPos->x > g_Object2[nCount].pos.x + g_Object2[nCount].VtxMinObject.x - PULS2)
			{
				pPos->x = g_Object2[nCount].pos.x + g_Object2[nCount].VtxMinObject.x - PULS2;
				pPos->x = pPosold->x;
			}
		}
	}
	return bLand;
}
//=========================================================================================================================
// �I�u�W�F�N�g�̎擾����
//=========================================================================================================================
Object2 *GetObject2(void)
{
	return &g_Object2[0];
}
//
//
//
void DeleteObject2(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT2; nCntObject++)
	{
		//g_Object2[nCntObject].VtxMinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		//g_Object2[nCntObject].VtxMaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_Object2[nCntObject].pos = D3DXVECTOR3(10000, 0, 0);
		g_Object2[nCntObject].rot = D3DXVECTOR3(0, 0, 0);
		g_Object2[nCntObject].bUse = false;
	}
}