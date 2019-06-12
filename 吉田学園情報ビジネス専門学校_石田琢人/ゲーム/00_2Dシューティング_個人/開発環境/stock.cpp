//=============================================================================
//
// �c�@���� [stock.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "stock.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_STOCK		"data/TEXTURE/player000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_STOCK	(3)		//�̗͂̍ő包��
#define STOCK_SIZE	(25)	//�̗͂̑傫��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexStock(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureStock = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStock = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
STOCK					g_Stock[MAX_STOCK];			// �X�g�b�N

//=============================================================================
// ����������
//=============================================================================
void InitStock(void)
{
	int nCntStock;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{// �̗͂̏�����
		g_Stock[nCntStock].pos = D3DXVECTOR3(40, 80, 0.0f);	// �̗͂̈ʒu
		g_Stock[nCntStock].nCntStock = MAX_STOCK;
		g_Stock[nCntStock].bUse = true;
	}


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_STOCK,		// �t�@�C���̖��O
		&g_pTextureStock);	// �e�N�X�`���ւ̃|�C���^

	// ���_���̍쐬
	MakeVertexStock(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitStock(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureStock != NULL)
	{
		g_pTextureStock->Release();
		g_pTextureStock = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffStock != NULL)
	{
		g_pVtxBuffStock->Release();
		g_pVtxBuffStock = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateStock(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawStock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffStock, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureStock);

	for (int nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{
		if (g_Stock[nCntStock].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntStock, 2);
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexStock(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntStock;
	int nCntStock2;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_STOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStock,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x - STOCK_SIZE, g_Stock[nCntStock].pos.y - STOCK_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x + STOCK_SIZE, g_Stock[nCntStock].pos.y - STOCK_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x - STOCK_SIZE, g_Stock[nCntStock].pos.y + STOCK_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x + STOCK_SIZE, g_Stock[nCntStock].pos.y + STOCK_SIZE, 0.0f);

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
		for (nCntStock2 = 0; nCntStock2 < MAX_STOCK; nCntStock2++)
		{
			g_Stock[nCntStock2].pos.x += 50;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStock->Unlock();
}

//=============================================================================
// �c�@�̌��Z
//=============================================================================
void SubtractionStock(int nValue)
{
	if (g_Stock[nValue - 1].bUse == true)
	{
		g_Stock[nValue - 1].bUse = false;
	}
}

//=============================================================================
// �c�@�̎擾
//=============================================================================
STOCK *GetStock(void)
{
	return &g_Stock[0];
}