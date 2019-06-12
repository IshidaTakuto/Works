//=============================================================================
//
// ���b�V���V�����_�[���� [meshCylinder.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "meshCylinder.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MESHCYLINDER		"data/TEXTURE/river000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MESH_HORIZONTAL	(20)			// ���������̕�����
#define MESH_VERTICAL	(1)				// ���������̕�����
#define MESH_HEIGHT		(50.0f)			// ����
#define MESH_RADIUS		(100.0f)			// ���a�̒���
#define MAX_VERTEX		((MESH_HORIZONTAL + 1) * (MESH_VERTICAL + 1))							// ���_��
#define MAX_INDEX		(MESH_HORIZONTAL * MESH_VERTICAL * 2 + (4 * (MESH_VERTICAL - 1)) + 2)	// �C���f�b�N�X�̐�
#define MAX_POLYGON		((MESH_HORIZONTAL * MESH_VERTICAL * 2) + (4 * (MESH_VERTICAL - 1)))		// �|���S����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;	// �C���f�b�N�X�ւ̃|�C���^
MeshCylinder g_MeshCylinder;

//=============================================================================
// ����������
//=============================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHCYLINDER, &g_pTextureMeshCylinder);

	g_MeshCylinder.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu�̏�����
	g_MeshCylinder.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����̏�����
	g_MeshCylinder.fAngle = 1.0f / (MESH_HORIZONTAL / 2);	// �p�x
	g_MeshCylinder.fRadius = MESH_RADIUS;					// ���a

	// ���_���̍쐬
	MakeVertexMeshCylinder(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshCylinder(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshCylinder(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_MeshCylinder.mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshCylinder.rot.y, g_MeshCylinder.rot.x, g_MeshCylinder.rot.z);
	D3DXMatrixMultiply(&g_MeshCylinder.mtxWorld, &g_MeshCylinder.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_MeshCylinder.pos.x, g_MeshCylinder.pos.y, g_MeshCylinder.pos.z);
	D3DXMatrixMultiply(&g_MeshCylinder.mtxWorld, &g_MeshCylinder.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshCylinder.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_VERTEX, 0, MAX_POLYGON);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMeshV;
	int nCntMeshH;
	int nCntIdx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̐ݒ�

	for (nCntMeshV = 0; nCntMeshV <= MESH_VERTICAL; nCntMeshV++)
	{
		for (nCntMeshH = 0; nCntMeshH <= MESH_HORIZONTAL; nCntMeshH++)
		{

			// ���_�̍��W
			pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (g_MeshCylinder.fAngle * nCntMeshH))) * g_MeshCylinder.fRadius;		// �����
			pVtx[0].pos.y = MESH_HEIGHT - (nCntMeshV * MESH_HEIGHT);													// Y�������ւ��炷
			pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (g_MeshCylinder.fAngle * nCntMeshH))) * g_MeshCylinder.fRadius;		// �����

			//if (g_MeshCylinder.fAngle == D3DX_PI)
			//{
			//	g_MeshCylinder.fAngle = g_MeshCylinder.fAngle * -1.0f;
			//}

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);	// ���K������

			// ���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(nCntMeshH * 1.0f, nCntMeshV * 1.0f);

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshCylinder->Unlock();

	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshH = 0, nCntIdx = 0; nCntMeshH <= MESH_HORIZONTAL; nCntMeshH++, nCntIdx++)
		{
			pIdx[0] = (MESH_HORIZONTAL + 1) + nCntIdx;
			pIdx[1] = 0 + nCntIdx;

			pIdx += 2;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pIdxBuffMeshCylinder->Unlock();
}
