//=============================================================================
//
// ���b�V���h�[������ [meshDome.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "meshDome.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MESHDOME		"data/TEXTURE/river000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MESH_HORIZONTAL	(8)				// ���������̕�����
#define MESH_VERTICAL	(4)				// ���������̕�����
#define MESH_HEIGHT		(50.0f)			// ����
#define MESH_RADIUS		(50.0f)			// ���a�̒���
#define MAX_VERTEX		((MESH_HORIZONTAL + 1) * (MESH_VERTICAL + 1) + (MESH_HORIZONTAL + 1))			// ���_��
#define MAX_INDEX		(MESH_HORIZONTAL * MESH_VERTICAL * 2 + (4 * (MESH_VERTICAL - 1)) + 2 + (MESH_HORIZONTAL + 2))	// �C���f�b�N�X�̐�
#define MAX_POLYGON		((MESH_HORIZONTAL * MESH_VERTICAL * 2) + (4 * (MESH_VERTICAL - 1)) + MESH_HORIZONTAL)		// �|���S����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexMeshDome(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshDome = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshDome = NULL;	// �C���f�b�N�X�ւ̃|�C���^
MeshDome g_MeshDome;
int u = MAX_POLYGON;
//=============================================================================
// ����������
//=============================================================================
void InitMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHDOME, &g_pTextureMeshDome);

	g_MeshDome.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu�̏�����
	g_MeshDome.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����̏�����
	g_MeshDome.fAngle = 1.0f / (MESH_HORIZONTAL / 2);	// �p�x
	g_MeshDome.fRadius = MESH_RADIUS;					// ���a

															// ���_���̍쐬
	MakeVertexMeshDome(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshDome(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshDome != NULL)
	{
		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (g_pIdxBuffMeshDome != NULL)
	{
		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshDome(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

												// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_MeshDome.mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshDome.rot.y, g_MeshDome.rot.x, g_MeshDome.rot.z);
	D3DXMatrixMultiply(&g_MeshDome.mtxWorld, &g_MeshDome.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_MeshDome.pos.x, g_MeshDome.pos.y, g_MeshDome.pos.z);
	D3DXMatrixMultiply(&g_MeshDome.mtxWorld, &g_MeshDome.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshDome.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshDome, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshDome);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshDome);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESH_HORIZONTAL + 1, 0, MESH_HORIZONTAL);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_VERTEX - (MESH_HORIZONTAL + 1), 0, MAX_POLYGON - MESH_HORIZONTAL);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshDome(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMeshV;
	int nCntMeshH;
	int nCntIdx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshDome,
		NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshDome,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos.x = 0.0f;
	pVtx[0].pos.y = MESH_HEIGHT * 2;	// Y�������ւ��炷
	pVtx[0].pos.z = 0.0f;

	// �@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);	// ���K������

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	// ���_�̐ݒ�
	for (nCntMeshV = 0; nCntMeshV < MESH_VERTICAL; nCntMeshV++)
	{
		for (nCntMeshH = 0; nCntMeshH < MESH_HORIZONTAL; nCntMeshH++)
		{
			// ���_�̍��W
			//pVtx[1].pos.x = sinf(D3DX_PI * (0.0f + (g_MeshDome.fAngle * nCntMeshH))) * sinf(nCntMeshH * g_MeshDome.fRadius) * g_MeshDome.fRadius;		// 45�x�������
			//pVtx[1].pos.y = MESH_HEIGHT - (nCntMeshV * MESH_HEIGHT);									// Y�������ւ��炷
			//pVtx[1].pos.z = cosf(D3DX_PI * (0.0f + (g_MeshDome.fAngle * nCntMeshH))) * sinf(nCntMeshH * g_MeshDome.fRadius) * g_MeshDome.fRadius;		// 45�x�������

			pVtx[1].pos.x = sinf(D3DX_PI * (0.0f + (g_MeshDome.fAngle * nCntMeshH))) * g_MeshDome.fRadius;		// 45�x�������
			pVtx[1].pos.y = MESH_HEIGHT - (nCntMeshV * MESH_HEIGHT);									// Y�������ւ��炷
			pVtx[1].pos.z = cosf(D3DX_PI * (0.0f + (g_MeshDome.fAngle * nCntMeshH))) * g_MeshDome.fRadius;		// 45�x�������


			// �@���̐ݒ�
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);	// ���K������

			// ���_�J���[
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W
			pVtx[1].tex = D3DXVECTOR2(0.0f + (nCntMeshH * 1.0f), 0.0f + (nCntMeshV * 1.0f));

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshDome->Unlock();

	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	for (nCntMeshV = 0, nCntIdx = 0; nCntMeshV < MESH_VERTICAL; nCntMeshV++)
	{// ��������
		for (nCntMeshH = 0; nCntMeshH <= MESH_HORIZONTAL; nCntMeshH++, nCntIdx++)
		{// ��������
			if (nCntMeshV == 0)
			{// FAN�����̐ݒ�
				pIdx[0] = 1 + nCntIdx;
				pIdx[1] = 0;

				pIdx += 2;

				if (nCntMeshH % 2 == 0)
				{// Fan�̐܂�Ԃ�
					pIdx[0] = 0;
					pIdx[1] = 1 + nCntIdx + 1;

					pIdx += 2;
				}
			}
			else
			{// STRIP�����̐ݒ�
				pIdx[0] = 1 + MESH_HORIZONTAL + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				pIdx += 2;

				if (nCntMeshV < MESH_VERTICAL - 1 && nCntMeshH == MESH_HORIZONTAL)
				{// �܂�Ԃ���
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = 1 + MESH_HORIZONTAL + nCntIdx + 1;

					pIdx += 2;
				}
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N
	g_pIdxBuffMeshDome->Unlock();
}
