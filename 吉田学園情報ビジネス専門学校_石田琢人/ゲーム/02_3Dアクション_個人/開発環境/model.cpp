//=============================================================================
//
// ���f������ [model.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "model.h"
#include "score.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODELNAME00		"data/MODEL/sakura.x"		// �ǂݍ���x�t�@�C����
#define MODELNAME01		"data/MODEL/tree.x"		// �ǂݍ���x�t�@�C����
#define MODELNAME02		"data/MODEL/momiji.x"			// �ǂݍ���x�t�@�C����
#define MODELNAME03		"data/MODEL/snowtree.x"			// �ǂݍ���x�t�@�C����
#define MODELNAME04		"data/MODEL/item.x"			// �ǂݍ���x�t�@�C����

#define MAX_TYPE	(5)
#define MAX_MODEL	(61)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPD3DXMESH g_pMeshModel[MAX_TYPE] = {};				// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel[MAX_TYPE] = {};		// �}�e���A�����ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pBuffTexModel[MAX_TYPE] = {};
DWORD g_nNumMatModel = 0;				// �}�e���A�����̐�
int g_nIdxShadow;						// �e�̔ԍ�
Model g_Model[MAX_MODEL];				// ���f���̏��

//=============================================================================
// ����������
//=============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	int nCntModel;

	// �ʒu�E�����̏����ݒ�
	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_Model[nCntModel].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
		g_Model[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Model[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Model[nCntModel].fDestAngle = 0.0f;
		g_Model[nCntModel].nType = nCntModel;

		g_Model[nCntModel].vtxMin = D3DXVECTOR3(10000, 10000, 10000);
		g_Model[nCntModel].vtxMax = D3DXVECTOR3(1, 1, 1);

		g_Model[nCntModel].bUse = false;
	}

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODELNAME00,		// ��
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[0],
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel[0]);

	D3DXLoadMeshFromX(MODELNAME01,		// ��
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[1],
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel[1]);

	D3DXLoadMeshFromX(MODELNAME02,		// �g�t
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[2],
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel[2]);

	D3DXLoadMeshFromX(MODELNAME03,		// �~�̖�
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[3],
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel[3]);

	D3DXLoadMeshFromX(MODELNAME04,		// �A�C�e��
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[4],
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel[4]);


	for (nCntModel = 0; nCntModel < MAX_TYPE; nCntModel++)
	{
		pMat = (D3DXMATERIAL*)g_pBuffMatModel[nCntModel]->GetBufferPointer();

		D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &g_pBuffTexModel[nCntModel]);
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitModel(void)
{
	int nCntModel;

	for (nCntModel = 0; nCntModel < MAX_TYPE; nCntModel++)
	{
		// ���b�V���̊J��
		if (g_pMeshModel[nCntModel] != NULL)
		{
			g_pMeshModel[nCntModel]->Release();
			g_pMeshModel[nCntModel] = NULL;
		}

		// �}�e���A���̊J��
		if (g_pBuffMatModel[nCntModel] != NULL)
		{
			g_pBuffMatModel[nCntModel]->Release();
			g_pBuffMatModel[nCntModel] = NULL;
		}
	}

	for (int nCntTex = 0; nCntTex < MAX_TYPE; nCntTex++)
	{
		if (g_pBuffTexModel[nCntTex] != NULL)
		{
			g_pBuffTexModel[nCntTex]->Release();
			g_pBuffTexModel[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModel(void)
{
	int nCntModel;

	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_Model[nCntModel].bUse == true && g_Model[nCntModel].type == MODELTYPE_ITEM)
		{
			g_Model[nCntModel].rot.y += 0.1f;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	int nCntModel;

	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Model[nCntModel].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model[nCntModel].rot.y, g_Model[nCntModel].rot.x, g_Model[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Model[nCntModel].pos.x, g_Model[nCntModel].pos.y, g_Model[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Model[nCntModel].mtxWorld);

		if (g_Model[nCntModel].bUse == true)
		{
			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatModel[g_Model[nCntModel].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pBuffTexModel[g_Model[nCntModel].nType]);

				// ���f��(�p�[�c)�̕`��
				g_pMeshModel[g_Model[nCntModel].nType]->DrawSubset(nCntMat);
			}

		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �����蔻��̏���
//=============================================================================
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange)
{
	int nCount;			// for���̃J�E���^
	bool bHit = false;	// �ڐG���Ă�����true

	for (nCount = 0; nCount < MAX_MODEL; nCount++)
	{// �S�Ă̓G����
		if (true == g_Model[nCount].bUse)
		{// �g�p����Ă���G�̂�
			float fLengthOneself;				// ����̉~��͈�

			fLengthOneself = powf(fRange, 2);	// �����XZ�͈̔�

			float fLengthX = pPos->x - g_Model[nCount].mtxWorld._41;		// X�̍�
			float fLengthZ = pPos->z - g_Model[nCount].mtxWorld._43;		// Z�̍�
			float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);		// XZ�̍��̓��

			if (fLengthOneself >= fLengthTotal)
			{// ����Ǝ����̔���
				if (MODELTYPE_OBJECT == g_Model[nCount].type)
				{// ��Q���̏ꍇ
					*pPos = *pPosold;							// �ʒu��߂�
					*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ��ʂ�0�ɂ���
				}
				else if (MODELTYPE_ITEM == g_Model[nCount].type)
				{// �A�C�e���̏ꍇ
					DeleteShadow(g_Model[nCount].nIdxShadow);	// �e������
					g_Model[nCount].bUse = false;				// ���f�����g�p���Ă��Ȃ���Ԃɂ���
				}
				bHit = true;
			}
		}
	}
	return bHit;
}

//=============================================================================
// ���f���̐ݒ�
//=============================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, MODELTYPE type)
{
	int nCntModel;

	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_Model[nCntModel].bUse == false)
		{
			g_Model[nCntModel].pos = pos;
			g_Model[nCntModel].rot = rot;
			g_Model[nCntModel].nType = nType;
			g_Model[nCntModel].type = type;

			g_Model[nCntModel].bUse = true;

			g_Model[nCntModel].nIdxShadow = SetShadow(pos, rot, 10.0f, 10.0f);

			int nNumVtx;		// ���_��
			DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
			BYTE *pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

			// ���_�����擾
			nNumVtx = g_pMeshModel[g_Model[nCntModel].nType]->GetNumVertices();

			// ���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(g_pMeshModel[g_Model[nCntModel].nType]->GetFVF());

			// ���_�o�b�t�@�����b�N
			g_pMeshModel[g_Model[nCntModel].nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// ���_���W�̔�r

				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

				// X�̒l�̔�r
				if (g_Model[nCntModel].vtxMin.x > vtx.x)
				{// �ŏ��l�Ɣ�r
					g_Model[nCntModel].vtxMin.x = vtx.x;	// �ŏ��l��菬������Α��
				}

				if (g_Model[nCntModel].vtxMax.x < vtx.x)
				{// �ő�l�Ɣ�r
					g_Model[nCntModel].vtxMax.x = vtx.x;	// �ő�l���傫����Α��
				}

				// Y�̒l�̔�r
				if (g_Model[nCntModel].vtxMin.y > vtx.y)
				{// �ŏ��l�Ɣ�r
					g_Model[nCntModel].vtxMin.y = vtx.y;	// �ŏ��l��菬������Α��
				}

				if (g_Model[nCntModel].vtxMax.y < vtx.y)
				{// �ő�l�Ɣ�r
					g_Model[nCntModel].vtxMax.y = vtx.y;	// �ő�l���傫����Α��
				}

				// Z�̒l�̔�r
				if (g_Model[nCntModel].vtxMin.z > vtx.z)
				{// �ŏ��l�Ɣ�r
					g_Model[nCntModel].vtxMin.z = vtx.z;	// �ŏ��l��菬������Α��
				}

				if (g_Model[nCntModel].vtxMax.z < vtx.z)
				{// �ő�l�Ɣ�r
					g_Model[nCntModel].vtxMax.z = vtx.z;	// �ő�l���傫����Α��
				}
			}

			// ���_�o�b�t�@���A�����b�N
			g_pMeshModel[g_Model[nCntModel].nType]->UnlockVertexBuffer();

			break;
		}
	}
}