//=============================================================================
//
// ���b�V���I�[�r�b�g���� [meshOrbit.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "meshOrbit.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MESHORBIT		"data/TEXTURE/bg000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_ORBIT	(8)		// �I�[�r�b�g�̍ő吔
#define MAX_VERTEX	(256)	// ���_�̍ő吔
#define MAX_TEX		(1)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexMeshOrbit(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshOrbit[MAX_TEX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshOrbit = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
MeshOrbit g_aMeshOrbit[MAX_ORBIT];
int g_nCntUseVertex;

//=============================================================================
// ����������
//=============================================================================
void InitMeshOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHORBIT, &g_pTextureMeshOrbit[0]);

	int nCntOrbit;

	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_aMeshOrbit[nCntOrbit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshOrbit[nCntOrbit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshOrbit[nCntOrbit].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aMeshOrbit[nCntOrbit].bUse = false;
	}

	g_nCntUseVertex = 0;

	// ���_���̍쐬
	MakeVertexMeshOrbit(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshOrbit(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureMeshOrbit[nCntTex] != NULL)
		{
			g_pTextureMeshOrbit[nCntTex]->Release();
			g_pTextureMeshOrbit[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshOrbit != NULL)
	{
		g_pVtxBuffMeshOrbit->Release();
		g_pVtxBuffMeshOrbit = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshOrbit(void)
{
	int nCntOrbit;
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMeshOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_aMeshOrbit[nCntOrbit].bUse == false)
		{
			// ���������Ƀf�[�^���ڂ�
			g_aMeshOrbit[nCntOrbit].pos = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].pos;
			g_aMeshOrbit[nCntOrbit].start = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].start;
			g_aMeshOrbit[nCntOrbit].end = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].end;
			g_aMeshOrbit[nCntOrbit].col = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].col;
			g_aMeshOrbit[nCntOrbit].nTime = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].nTime;
			g_aMeshOrbit[nCntOrbit].nType = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].nType;
			g_aMeshOrbit[nCntOrbit].bUse = true;

			if (nCntOrbit % 2 == 0)
			{
				// ���_���W�̎w��
				pVtx[nCntOrbit - 1].pos = g_aMeshOrbit[nCntOrbit - 1].start;
				pVtx[nCntOrbit].pos = g_aMeshOrbit[nCntOrbit].end;

				pVtx[nCntOrbit - 1].col = g_aMeshOrbit[nCntOrbit - 1].col;
				pVtx[nCntOrbit].col = g_aMeshOrbit[nCntOrbit].col;

				break;
			}
		}
	}

	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_aMeshOrbit[nCntOrbit].bUse == true)
		{
			g_aMeshOrbit[nCntOrbit].nTime--;	// ���Ԃ����炷

			if (g_aMeshOrbit[nCntOrbit].nTime <= 0)
			{// ���Ԃ�0�ȉ��ɂȂ���
				g_aMeshOrbit[nCntOrbit].col.a -= 0.02f;	// ���X�ɓ�����

				if (g_aMeshOrbit[nCntOrbit].col.a <= 0)
				{// �����ɂȂ�����false
					g_aMeshOrbit[nCntOrbit].bUse = false;
				}
				pVtx[nCntOrbit].col = g_aMeshOrbit[nCntOrbit].col;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshOrbit->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	int nCntMeshOrbit;

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		// �J�����O�𖳌���

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntMeshOrbit = 0; nCntMeshOrbit < MAX_ORBIT; nCntMeshOrbit++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMeshOrbit[nCntMeshOrbit].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshOrbit[nCntMeshOrbit].rot.y, g_aMeshOrbit[nCntMeshOrbit].rot.x, g_aMeshOrbit[nCntMeshOrbit].rot.z);
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntMeshOrbit].mtxWorld, &g_aMeshOrbit[nCntMeshOrbit].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aMeshOrbit[nCntMeshOrbit].pos.x, g_aMeshOrbit[nCntMeshOrbit].pos.y, g_aMeshOrbit[nCntMeshOrbit].pos.z);
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntMeshOrbit].mtxWorld, &g_aMeshOrbit[nCntMeshOrbit].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMeshOrbit[nCntMeshOrbit].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffMeshOrbit, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMeshOrbit[g_aMeshOrbit[nCntMeshOrbit].nType]);

		if (g_aMeshOrbit[nCntMeshOrbit].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, MAX_VERTEX - 2);
		}
	}

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// ���ʂ��J�����O
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshOrbit(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshOrbit,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMeshOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_VERTEX / 2; nCntOrbit++)
	{
		// ���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(nCntOrbit * 1.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(nCntOrbit * 1.0f, 0.0f);

		pVtx += 2;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshOrbit->Unlock();
}

//=============================================================================
// �I�[�r�b�g�̐ݒ�
//=============================================================================
void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col, int nTime, int nType)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMeshOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_aMeshOrbit[nCntOrbit].pos = pos;
		g_aMeshOrbit[nCntOrbit].start = start;
		g_aMeshOrbit[nCntOrbit].end = end;
		g_aMeshOrbit[nCntOrbit].col = col;
		g_aMeshOrbit[nCntOrbit].nTime = nTime;
		g_aMeshOrbit[nCntOrbit].nType = nType;
	}

	// ���_�̍��W
	pVtx[MAX_ORBIT - 1].pos = start;
	pVtx[MAX_ORBIT - 2].pos = end;

	pVtx[MAX_ORBIT - 1].col = col;
	pVtx[MAX_ORBIT - 2].col = col;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshOrbit->Unlock();
}