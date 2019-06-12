//=============================================================================
//
// ���b�V���E�H�[������ [meshWall.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "meshWall.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MESHWALL00		"data/TEXTURE/sakura_wall.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHWALL01		"data/TEXTURE/summer_wall.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHWALL02		"data/TEXTURE/fall_wall.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHWALL03		"data/TEXTURE/winter_wall.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHWALL04		"data/TEXTURE/sky.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C����

#define MAX_TEXTURE		(5)

#define SPLITX			(5)
#define SPLITY			(1)

#define MAX_VERTEX		((SPLITX + 1) * (SPLITY + 1))			// ���_��
#define MAX_INDEX		(SPLITX * SPLITY * 2 + (4 * (SPLITY - 1)) + 2)	// �C���f�b�N�X�̐�
#define MAX_POLYGON		(SPLITX * SPLITY * 2) + (4 * (SPLITY - 1))		// �|���S����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice, int nIndex);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshWall[MAX_TEXTURE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshWall = NULL;	// �C���f�b�N�X�ւ̃|�C���^
MeshWall g_aMeshWall[MAX_WALL];

//=============================================================================
// ����������
//=============================================================================
void InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHWALL00, &g_pTextureMeshWall[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHWALL01, &g_pTextureMeshWall[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHWALL02, &g_pTextureMeshWall[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHWALL03, &g_pTextureMeshWall[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHWALL04, &g_pTextureMeshWall[4]);

	int nCntWall;

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{// ���f���̏���������
		if (g_aMeshWall[nCntWall].bUse == false)
		{
			g_aMeshWall[nCntWall].pos = pos;
			g_aMeshWall[nCntWall].rot = rot;
			g_aMeshWall[nCntWall].fWidth = fWidth;
			g_aMeshWall[nCntWall].fHeight = fHeight;
			g_aMeshWall[nCntWall].nType = nType;
			g_aMeshWall[nCntWall].fTexPos = 1.0f;
			g_aMeshWall[nCntWall].bUse = true;
			break;
		}
	}

	// ���_���̍쐬
	MakeVertexMeshWall(pDevice, nCntWall);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshWall(void)
{
	for (int nCntWall = 0; nCntWall < MAX_TEXTURE; nCntWall++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureMeshWall[nCntWall] != NULL)
		{
			g_pTextureMeshWall[nCntWall]->Release();
			g_pTextureMeshWall[nCntWall] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshWall(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	int nCntMeshWall;

	for (nCntMeshWall = 0; nCntMeshWall < MAX_WALL; nCntMeshWall++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMeshWall[nCntMeshWall].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntMeshWall].rot.y, g_aMeshWall[nCntMeshWall].rot.x, g_aMeshWall[nCntMeshWall].rot.z);
		D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntMeshWall].pos.x, g_aMeshWall[nCntMeshWall].pos.y, g_aMeshWall[nCntMeshWall].pos.z);
		D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntMeshWall].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffMeshWall);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMeshWall[g_aMeshWall[nCntMeshWall].nType]);

		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_VERTEX, 0, MAX_POLYGON);
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice, int nIndex)
{
	int nCntMeshY;
	int nCntMeshX;
	int nCntIdx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshWall,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshY = 0; nCntMeshY <= SPLITY; nCntMeshY++)
	{
		for (nCntMeshX = 0; nCntMeshX <= SPLITX; nCntMeshX++)
		{
			// ���_�̍��W
			pVtx[0].pos = D3DXVECTOR3((-g_aMeshWall[nIndex].fWidth / 2) + ((g_aMeshWall[nIndex].fWidth * nCntMeshX) / SPLITX), g_aMeshWall[nIndex].fHeight - ((g_aMeshWall[nIndex].fHeight * nCntMeshY) / SPLITY), 0.0f);

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshY * 1.0f);

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshWall->Unlock();

	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshY = 0, nCntIdx = 0; nCntMeshY < SPLITY; nCntMeshY++)
	{
		for (nCntMeshX = 0; nCntMeshX <= SPLITX; nCntMeshX++, nCntIdx++)
		{
			pIdx[0] = (SPLITX + 1) + nCntIdx;
			pIdx[1] = 0 + nCntIdx;

			pIdx += 2;

			if (nCntMeshY < SPLITY - 1 && nCntMeshX == SPLITX)
			{// �܂�Ԃ���
				pIdx[0] = 0 + nCntIdx;
				pIdx[1] = (SPLITX + 1) + nCntIdx + 1;

				pIdx += 2;
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	g_pIdxBuffMeshWall->Unlock();
}
