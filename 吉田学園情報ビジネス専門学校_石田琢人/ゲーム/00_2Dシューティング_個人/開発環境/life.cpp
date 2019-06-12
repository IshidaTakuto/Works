//=============================================================================
//
// �̗͏��� [life.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "life.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_LIFE		"data/TEXTURE/life000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_LIFE	(5)		//�̗͂̍ő包��
#define LIFE_SIZE	(25)	//�̗͂̑傫��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureLife = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LIFE					g_Life[MAX_LIFE];					// �̗�

//=============================================================================
// ����������
//=============================================================================
void InitLife(void)
{
	int nCntLife;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{// �̗͂̏�����
		g_Life[nCntLife].pos = D3DXVECTOR3(40, 40, 0.0f);	// �̗͂̈ʒu
		g_Life[nCntLife].nCntLife = MAX_LIFE;
		g_Life[nCntLife].bUse = true;
	}


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_LIFE,		// �t�@�C���̖��O
								&g_pTextureLife);	// �e�N�X�`���ւ̃|�C���^

	// ���_���̍쐬
	MakeVertexLife(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitLife(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLife(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		if (g_Life[nCntLife].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntLife, 2);
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntLife;
	int nCntLife2;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x - LIFE_SIZE, g_Life[nCntLife].pos.y - LIFE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x + LIFE_SIZE, g_Life[nCntLife].pos.y - LIFE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x - LIFE_SIZE, g_Life[nCntLife].pos.y + LIFE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x + LIFE_SIZE, g_Life[nCntLife].pos.y + LIFE_SIZE, 0.0f);

		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
		for (nCntLife2 = 0; nCntLife2 < MAX_LIFE; nCntLife2++)
		{
			g_Life[nCntLife2].pos.x += 50;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// �̗͂̌��Z
//=============================================================================
void SubtractionLife(int nValue)
{
	if (g_Life[nValue - 1].bUse == true)
	{
		g_Life[nValue - 1].bUse = false;
	}
}

//=============================================================================
// �̗͂̎擾
//=============================================================================
LIFE *GetLife(void)
{
	return &g_Life[0];
}