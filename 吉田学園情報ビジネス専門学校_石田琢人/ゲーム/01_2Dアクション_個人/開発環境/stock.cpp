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
#define	TEXTURE_STOCK0		"data/TEXTURE/stock.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_STOCK1		"data/TEXTURE/frame_stock.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define STOCK_SIZE	(25)	// �c�@�̑傫��
#define MAX_STOCK	(NUM_STOCK + 1)
#define MAX_TEX		(2)
#define FRAME_SIZE_X	(200)
#define FRAME_SIZE_Y	(150)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexStock(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureStock[MAX_TEX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStock = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
STOCK					g_Stock[NUM_STOCK];			// �X�g�b�N

//=============================================================================
// ����������
//=============================================================================
void InitStock(void)
{
	int nCntStock;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntStock = 0; nCntStock < NUM_STOCK; nCntStock++)
	{// �c�@�̏�����
		g_Stock[nCntStock].pos = D3DXVECTOR3(100 + (50.0f * nCntStock), 65, 0.0f);	// �c�@�̈ʒu
		g_Stock[nCntStock].nCntStock = NUM_STOCK;
		g_Stock[nCntStock].nType = 0;
		g_Stock[nCntStock].bUse = true;
	}
	g_Stock[NUM_STOCK].pos = D3DXVECTOR3(150, 90, 0.0f);	// �c�@�̈ʒu
	g_Stock[NUM_STOCK].nType = 1;
	g_Stock[NUM_STOCK].bUse = true;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_STOCK0, &g_pTextureStock[0]);	// �e�N�X�`���ւ̃|�C���^
	D3DXCreateTextureFromFile(pDevice, TEXTURE_STOCK1, &g_pTextureStock[1]);	// �e�N�X�`���ւ̃|�C���^

	// ���_���̍쐬
	MakeVertexStock(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitStock(void)
{
	for (int nCntStock = 0; nCntStock < MAX_TEX; nCntStock++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureStock[nCntStock] != NULL)
		{
			g_pTextureStock[nCntStock]->Release();
			g_pTextureStock[nCntStock] = NULL;
		}
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

	for (int nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{
		if (g_Stock[nCntStock].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureStock[g_Stock[nCntStock].nType]);

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

	for (nCntStock = 0; nCntStock < NUM_STOCK; nCntStock++)
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
	}
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x - FRAME_SIZE_X, g_Stock[nCntStock].pos.y - FRAME_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x + FRAME_SIZE_X, g_Stock[nCntStock].pos.y - FRAME_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x - FRAME_SIZE_X, g_Stock[nCntStock].pos.y + FRAME_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x + FRAME_SIZE_X, g_Stock[nCntStock].pos.y + FRAME_SIZE_Y, 0.0f);

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