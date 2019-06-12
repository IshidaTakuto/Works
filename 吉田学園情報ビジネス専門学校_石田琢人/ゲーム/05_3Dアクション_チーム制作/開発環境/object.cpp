//=============================================================================
//
// ���f������ [Object.cpp]
// Author : �L�n ���u
//
//=============================================================================
#include "object.h"		//�I�u�W�F�N�g
#include "input.h"		//�L�[�{�[�h
#include "camera.h"		//�J����
#include "shadow.h"		//�e
#include "player.h"		//�v���C���[
#include "petals.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define OBJECT_NAME			"data\\MODEL\\��(��)�ŋ�.x"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define OBJECT_NAME1		"data\\MODEL\\��.x"				//�ǂݍ��ރe�N�X�`���t�@�C��
#define MAX_TEX				(1)								//�I�u�W�F�N�g�̃e�N�X�`���ő吔
#define MAX_OBJECT			(72)							//�I�u�W�F�N�g�̍ő吔
#define MAX_VARIATION		(1)								//�I�u�W�F�N�g�̎��
#define PLUS				(20)							//�����蔻��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Object g_Object[MAX_OBJECT];

//=============================================================================
// ����������
//=============================================================================
void InitObject(void)
{
	//�f�o�C�X�|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX
	(
		OBJECT_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Object[0].pBuffMatObject,
		NULL, &g_Object[0].nNumMatObject,
		&g_Object[0].pMeshObject
	);
	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX
	(
		OBJECT_NAME1,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Object[1].pBuffMatObject,
		NULL, &g_Object[1].nNumMatObject,
		&g_Object[1].pMeshObject
	);

	D3DXMATERIAL *pMat;
	D3DMATERIAL9 *pMatDef;

	//�e�N�X�`��������
	for (int nCount = 0; nCount < MAX_VARIATION; nCount++)
	{
		pMat = (D3DXMATERIAL*)g_Object[nCount].pBuffMatObject->GetBufferPointer();
		pMatDef = new D3DMATERIAL9[g_Object[nCount].nNumMatObject];
		g_Object[nCount].pMeshTextures = new LPDIRECT3DTEXTURE9[g_Object[nCount].nNumMatObject];

		for (DWORD tex = 0; tex < g_Object[nCount].nNumMatObject; tex++)
		{
			pMatDef[tex] = pMat[tex].MatD3D;
			pMatDef[tex].Ambient = pMatDef[tex].Diffuse;
			g_Object[nCount].pMeshTextures[tex] = NULL;
			if (pMat[tex].pTextureFilename != NULL &&
				lstrlen(pMat[tex].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pMat[tex].pTextureFilename,
					&g_Object[nCount].pMeshTextures[tex])))
				{
					//MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
				}
			}
		}
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		g_Object[nCntObject].VtxMinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_Object[nCntObject].VtxMaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_Object[nCntObject].pos = D3DXVECTOR3(0, 0, 0);
		g_Object[nCntObject].rot = D3DXVECTOR3(0, 0, 0);
		g_Object[nCntObject].bUse = false;
	}

	int nNumVtx;			//���_��
	DWORD sizeFVF;			//���_�t�H�[�}�b�g7�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^

	for (int nCount = 0; nCount < MAX_VARIATION; nCount++)
	{
		//���_�����擾
		nNumVtx = g_Object[nCount].pMeshObject->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_Object[nCount].pMeshObject->GetFVF());

		//���_�o�b�t�@�����b�N
		g_Object[nCount].pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //���_���W�̑��

			//�͈͎w��
			//�ő�l
			if (vtx.x < g_Object[nCount].VtxMinObject.x)
			{
				g_Object[nCount].VtxMinObject.x = vtx.x;
			}
			if (vtx.z < g_Object[nCount].VtxMinObject.z)
			{
				g_Object[nCount].VtxMinObject.z = vtx.z;
			}
			if (vtx.y < g_Object[nCount].VtxMinObject.y)
			{
				g_Object[nCount].VtxMinObject.y = vtx.y;
			}
			//�ŏ��l
			if (vtx.x > g_Object[nCount].VtxMaxObject.x)
			{
				g_Object[nCount].VtxMaxObject.x = vtx.x;
			}
			if (vtx.z > g_Object[nCount].VtxMaxObject.z)
			{
				g_Object[nCount].VtxMaxObject.z = vtx.z;
			}
			if (vtx.y > g_Object[nCount].VtxMaxObject.y)
			{
				g_Object[nCount].VtxMaxObject.y = vtx.y;
			}
			pVtxBuff += sizeFVF;		//�T�C�Y���|�C���^��i�߂�
		}
		//���_�o�b�t�@���A�����b�N
		g_Object[nCount].pMeshObject->UnlockVertexBuffer();
	}

	//�I�u�W�F�N�g�̑��
	//for(�J�E���g�@= �S�̂̃I�u�W�F�N�g�̐����ŏ���+����)
	for (int nPotaCountObject = MAX_VARIATION; nPotaCountObject < MAX_OBJECT; nPotaCountObject++)
	{
		if (nPotaCountObject < MAX_OBJECT + MAX_VARIATION)
		{//�؁i���j
			g_Object[nPotaCountObject] = g_Object[0];
		}
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_VARIATION; nCntObject++)
	{
		// ���b�V���̊J��
		if (g_Object[nCntObject].pMeshObject != NULL)
		{
			g_Object[nCntObject].pMeshObject->Release();
			g_Object[nCntObject].pMeshObject = NULL;
		}
		// �}�e���A���̊J��
		if (g_Object[nCntObject].pBuffMatObject != NULL)
		{
			g_Object[nCntObject].pBuffMatObject->Release();
			g_Object[nCntObject].pBuffMatObject = NULL;
		}

		//�e�N�X�`��
		for (DWORD nTex = 0; nTex < g_Object[nCntObject].nNumMatObject; nTex++)
		{
			if (g_Object[nCntObject].pMeshTextures[nTex] != NULL)
			{
				g_Object[nCntObject].pMeshTextures[nTex]->Release();
				g_Object[nCntObject].pMeshTextures[nTex] = NULL;
			}
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (CollisionAttack(g_Object[nCntObject].pos, g_Object[nCntObject].VtxMaxObject.x))
		{// �U�����q�b�g
			if (g_Object[nCntObject].bUse == true)
			{// �g���Ă���
				//PlaySound(SOUND_LABEL_SWORDHIT);
				SetPetals(g_Object[nCntObject].pos, 3);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	for (int nCount2 = 0; nCount2 < MAX_OBJECT; nCount2++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Object[nCount2].mtxWorldObject);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object[nCount2].rot.y, g_Object[nCount2].rot.x, g_Object[nCount2].rot.z);

		D3DXMatrixMultiply(&g_Object[nCount2].mtxWorldObject, &g_Object[nCount2].mtxWorldObject, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Object[nCount2].pos.x, g_Object[nCount2].pos.y, g_Object[nCount2].pos.z);

		D3DXMatrixMultiply(&g_Object[nCount2].mtxWorldObject, &g_Object[nCount2].mtxWorldObject, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Object[nCount2].mtxWorldObject);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Object[nCount2].pBuffMatObject->GetBufferPointer();

		for (int nCntMat = 0; nCntMat<(int)g_Object[nCount2].nNumMatObject; nCntMat++)
		{
			if (g_Object[nCount2].bUse == true)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Object[nCount2].pMeshTextures[nCntMat]);
				// ���f��(�p�[�c)�̕`��
				g_Object[nCount2].pMeshObject->DrawSubset(nCntMat);
			}
		}
		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
}
//=============================================================================
// �Z�b�g���f��
//=============================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntObject;

	for (nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == false)
		{
			g_Object[nCntObject].pos = pos;
			g_Object[nCntObject].rot = rot;
			g_Object[nCntObject].bUse = true;
			break;
		}
	}
}
//=============================================================================
// ���f���̓����蔻��
//=============================================================================
bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove)
{
	int nCount;
	bool bLand = false;
	Player * pPlayer = GetPlayer();

	for (nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (true == g_Object[nCount].bUse)
		{
			if (pPos->x > g_Object[nCount].pos.x + g_Object[nCount].VtxMinObject.x - PLUS && pPos->x < g_Object[nCount].pos.x + g_Object[nCount].VtxMaxObject.x + PLUS)
			{	//X�͈̔�
				if (pPosold->z >= g_Object[nCount].pos.z + g_Object[nCount].VtxMaxObject.z + PLUS && pPos->z < g_Object[nCount].pos.z + g_Object[nCount].VtxMaxObject.z + PLUS)
				{
					pPos->z = g_Object[nCount].pos.z + g_Object[nCount].VtxMaxObject.z + PLUS;
					pPos->z = pPosold->z;
				}
				else if (pPosold->z <= g_Object[nCount].pos.z + g_Object[nCount].VtxMinObject.z - PLUS && pPos->z > g_Object[nCount].pos.z + g_Object[nCount].VtxMinObject.z - PLUS)
				{
					pPos->z = g_Object[nCount].pos.z + g_Object[nCount].VtxMinObject.z - PLUS;
					pPos->z = pPosold->z;
				}
			}
			if (pPos->z > g_Object[nCount].pos.z + g_Object[nCount].VtxMinObject.z - PLUS && pPos->z < g_Object[nCount].pos.z + g_Object[nCount].VtxMaxObject.z + PLUS)
			{	//Z�͈̔�
				if (pPosold->x >= g_Object[nCount].pos.x + g_Object[nCount].VtxMaxObject.x + PLUS && pPos->x < g_Object[nCount].pos.x + g_Object[nCount].VtxMaxObject.x + PLUS)
				{
					pPos->x = g_Object[nCount].pos.x + g_Object[nCount].VtxMaxObject.x + PLUS;
					pPos->x = pPosold->x;
				}
				else if (pPosold->x <= g_Object[nCount].pos.x + g_Object[nCount].VtxMinObject.x - PLUS && pPos->x > g_Object[nCount].pos.x + g_Object[nCount].VtxMinObject.x - PLUS)
				{
					pPos->x = g_Object[nCount].pos.x + g_Object[nCount].VtxMinObject.x - PLUS;
					pPos->x = pPosold->x;
				}
			}
		}
	}
	return bLand;
}
//=========================================================================================================================
// �I�u�W�F�N�g�̎擾����
//=========================================================================================================================
Object *GetObject(void)
{
	return &g_Object[0];
}
//
//
//
void DeleteObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		//g_Object[nCntObject].VtxMinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		//g_Object[nCntObject].VtxMaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_Object[nCntObject].pos = D3DXVECTOR3(0, 0, 0);
		g_Object[nCntObject].rot = D3DXVECTOR3(0, 0, 0);
		g_Object[nCntObject].bUse = false;
	}
}