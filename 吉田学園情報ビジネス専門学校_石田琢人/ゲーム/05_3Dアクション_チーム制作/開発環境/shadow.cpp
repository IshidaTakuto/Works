//=============================================================================
//
// �e�̏��� [shadow.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define SHADOW_WIDTH		(10.0f)							// x���̑傫��
#define SHADOW_DEPTH		(10.0f)							// z���̑傫��
#define MAX_SHADOW			(900)							// �e�̍ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Shadow g_aShadow[MAX_SHADOW];						// �e�̏��

//=============================================================================
// ����������
//=============================================================================
void InitShadow(void)
{
	int nCntShadow;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SHADOW, &g_pTextureShadow);

	// ���_���̍쐬
	MakeVertexShadow(pDevice);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̏�����
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̏�����
		g_aShadow[nCntShadow].fWidth = 0.0f;
		g_aShadow[nCntShadow].fDepth = 0.0f;
		g_aShadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aShadow[nCntShadow].bUse = false;
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	int nCntShadow;

	//�t�H�O���g�p���Ȃ�
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{// �e���g���Ă����
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			// �e�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);
		}
	}

	// Z�o�b�t�@�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�t�H�O���g�p����
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntShadow;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// ���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].fDepth);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].fDepth);
		pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth, 0.0f, -g_aShadow[nCntShadow].fDepth);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fWidth, 0.0f, -g_aShadow[nCntShadow].fDepth);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepth)
{
	int nCntShadow;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			g_aShadow[nCntShadow].pos.x = pos.x;
			g_aShadow[nCntShadow].pos.z = pos.z;
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].fWidth = fWidth;
			g_aShadow[nCntShadow].fDepth = fDepth;

			// ���_�̍��W
			pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].fDepth);
			pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].fDepth);
			pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth, 0.0f, -g_aShadow[nCntShadow].fDepth);
			pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fWidth, 0.0f, -g_aShadow[nCntShadow].fDepth);

			g_aShadow[nCntShadow].bUse = true;
			break;
		}
		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//=============================================================================
// �e�̈ʒu��ݒ�
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].pos.x = pos.x;
	g_aShadow[nIdxShadow].pos.z = pos.z;
}

//=============================================================================
// �e�̍����ɂ�钸�_��ݒ�
//=============================================================================
void SetVtxShadow(int nIdxShadow, D3DXVECTOR3 pos, float fAlpha)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aShadow[nIdxShadow].bUse == true)
	{
		g_aShadow[nIdxShadow].fHeightWidth = 0.08f * pos.y;
		g_aShadow[nIdxShadow].fHeightDepth = 0.08f * pos.y;

		g_aShadow[nIdxShadow].col.a = fAlpha - (0.005f * pos.y);
	}

	// ���_�̍��W
	pVtx[0 + (nIdxShadow * 4)].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fWidth - g_aShadow[nIdxShadow].fHeightWidth, 0.0f, g_aShadow[nIdxShadow].fDepth + g_aShadow[nIdxShadow].fHeightDepth);
	pVtx[1 + (nIdxShadow * 4)].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].fWidth + g_aShadow[nIdxShadow].fHeightWidth, 0.0f, g_aShadow[nIdxShadow].fDepth + g_aShadow[nIdxShadow].fHeightDepth);
	pVtx[2 + (nIdxShadow * 4)].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fWidth - g_aShadow[nIdxShadow].fHeightWidth, 0.0f, -g_aShadow[nIdxShadow].fDepth - g_aShadow[nIdxShadow].fHeightDepth);
	pVtx[3 + (nIdxShadow * 4)].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].fWidth + g_aShadow[nIdxShadow].fHeightWidth, 0.0f, -g_aShadow[nIdxShadow].fDepth - g_aShadow[nIdxShadow].fHeightDepth);

	// ���_�J���[
	pVtx[0 + (nIdxShadow * 4)].col = g_aShadow[nIdxShadow].col;
	pVtx[1 + (nIdxShadow * 4)].col = g_aShadow[nIdxShadow].col;
	pVtx[2 + (nIdxShadow * 4)].col = g_aShadow[nIdxShadow].col;
	pVtx[3 + (nIdxShadow * 4)].col = g_aShadow[nIdxShadow].col;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// �e�̍폜
//=============================================================================
void DeleteShadow(int nIdxShadow)
{
	if (g_aShadow[nIdxShadow].bUse == true)
	{
		g_aShadow[nIdxShadow].bUse = false;
	}
}

//=============================================================================
// �e�̎擾
//=============================================================================
Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}