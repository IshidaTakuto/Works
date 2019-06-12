//=============================================================================
//
// ���b�V���t�B�[���h���� [meshField.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "meshField.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MESHFIELD00		"data/TEXTURE/sakura_field.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD01		"data/TEXTURE/weed_field.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD02		"data/TEXTURE/momiji_field.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD03		"data/TEXTURE/snow_field.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define MESH_X			(6)			// x�������̃}�X��
#define MESH_Z			(3)			// z�������̃}�X��
#define MESH_WIDTH		(2500.0f)		// ��
#define MESH_DEPTH		(400.0f)		// ���s
#define MAX_VERTEX		((MESH_X + 1) * (MESH_Z + 1))						// ���_��
#define MAX_INDEX		(MESH_X * MESH_Z * 2 + (4 * (MESH_Z - 1)) + 2)		// �C���f�b�N�X�̐�
#define MAX_POLYGON		((MESH_X * MESH_Z * 2) + (4 * (MESH_Z - 1)))		// �|���S����

#define MAX_FIELD		(4)
#define MAX_TEXTURE		(4)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField[MAX_TEXTURE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;	// �C���f�b�N�X�ւ̃|�C���^
MeshField g_MeshField[MAX_FIELD];

//=============================================================================
// ����������
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD00, &g_pTextureMeshField[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD01, &g_pTextureMeshField[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD02, &g_pTextureMeshField[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD03, &g_pTextureMeshField[3]);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_MeshField[nCntField].pos = D3DXVECTOR3(MESH_WIDTH * nCntField, 0.0f, 0.0f);	// �ʒu�̏�����
		g_MeshField[nCntField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̏�����
		g_MeshField[nCntField].nType = nCntField;
	}

	// ���_���̍쐬
	MakeVertexMeshField(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureMeshField[nCntTex] != NULL)
		{
			g_pTextureMeshField[nCntTex]->Release();
			g_pTextureMeshField[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_MeshField[nCntField].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField[nCntField].rot.y, g_MeshField[nCntField].rot.x, g_MeshField[nCntField].rot.z);
		D3DXMatrixMultiply(&g_MeshField[nCntField].mtxWorld, &g_MeshField[nCntField].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_MeshField[nCntField].pos.x, g_MeshField[nCntField].pos.y, g_MeshField[nCntField].pos.z);
		D3DXMatrixMultiply(&g_MeshField[nCntField].mtxWorld, &g_MeshField[nCntField].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_MeshField[nCntField].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffMeshField);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMeshField[g_MeshField[nCntField].nType]);

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_VERTEX, 0, MAX_POLYGON);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMeshZ;
	int nCntMeshX;
	int nCntIdx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshZ = 0; nCntMeshZ <= MESH_Z; nCntMeshZ++)
	{
		for (nCntMeshX = 0; nCntMeshX <= MESH_X; nCntMeshX++)
		{
			// ���_�̍��W
			pVtx[0].pos = D3DXVECTOR3((MESH_WIDTH / 2) - ((MESH_WIDTH * nCntMeshX) / MESH_X), 0.0f, (-MESH_DEPTH / 2) + ((MESH_DEPTH * nCntMeshZ) / MESH_Z));
			
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshZ * 1.0f);

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshField->Unlock();

	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshZ = 0, nCntIdx = 0; nCntMeshZ < MESH_Z; nCntMeshZ++)
	{
		for (nCntMeshX = 0; nCntMeshX <= MESH_X; nCntMeshX++, nCntIdx++)
		{
			pIdx[0] = (MESH_X + 1) + nCntIdx;
			pIdx[1] = 0 + nCntIdx;

			pIdx += 2;

			if (nCntMeshZ < MESH_Z - 1 && nCntMeshX == MESH_X)
			{// �܂�Ԃ���
				pIdx[0] = 0 + nCntIdx;
				pIdx[1] = (MESH_X + 1) + nCntIdx + 1;

				pIdx += 2;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pIdxBuffMeshField->Unlock();
}

//=============================================================================
// �����蔻��̏���
//=============================================================================
bool CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (pPos->y < g_MeshField[nCntField].pos.y && pPosOld->y >= g_MeshField[nCntField].pos.y)
		{// �ォ�瓖��������
			bLand = true;
			pPos->y = g_MeshField[nCntField].pos.y;
			pMove->y = 0.0f;
		}
	}

	return bLand;
}