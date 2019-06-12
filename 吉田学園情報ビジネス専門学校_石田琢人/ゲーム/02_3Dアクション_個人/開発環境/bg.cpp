//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "bg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_TEXTURENAME0	"data/TEXTURE/bg000.jpg"	// �w�i�̃e�N�X�`����
#define BG_TEXTURENAME1	"data/TEXTURE/river000.png"	// �w�i�̃e�N�X�`����
#define BG_POS_X	(0)				// �w�i�̍���X���W
#define BG_POS_Y	(0)				// �w�i�̍���Y���W
#define BG_WIDTH	(SCREEN_WIDTH)	// �w�i�̕�
#define BG_HEIGHT	(SCREEN_HEIGHT)	// �w�i�̍���
#define TEX_UPDATE	(g_Bg.nCountAnim * 0.0001f)
#define MAX_BG		(2)		// �w�i�̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBg[MAX_BG] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
BG g_aBg[MAX_BG];

//=============================================================================
// �|���S������������
//=============================================================================
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �w�i�̏��̏�����
	g_aBg[0].pos = D3DXVECTOR3(BG_WIDTH / 2, BG_HEIGHT / 2, 0.0f);	// �w�i�̈ʒu
	g_aBg[1].pos = D3DXVECTOR3(BG_WIDTH / 2, BG_HEIGHT / 4 - BG_HEIGHT, 0.0f);	// �w�i�̈ʒu

	g_aBg[0].nCountAnim = 0;
	g_aBg[1].nCountAnim = 0;

	g_aBg[0].nType = 0;
	g_aBg[1].nType = 1;


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME0, &g_pTextureBg[0]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME1, &g_pTextureBg[1]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG, 
								D3DUSAGE_WRITEONLY, 
								FVF_VERTEX_2D, 
								D3DPOOL_MANAGED, 
								&g_pVtxBuffBg, 
								NULL);

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, BG_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);

	// 1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�̍��W
	pVtx[4].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT / 2, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT / 2, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);

	// 1.0f�ŌŒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// �|���S���I������
//=============================================================================
void UninitBg(void)
{
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		// �e�N�X�`���̔j��
		if (g_pTextureBg[nCntBg] != NULL)
		{
			g_pTextureBg[nCntBg]->Release();
			g_pTextureBg[nCntBg] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//=============================================================================
// �|���S���X�V����
//=============================================================================
void UpdateBg(void)
{
	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	g_aBg[0].nCountAnim++;
	g_aBg[1].nCountAnim++;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W���X�V
	pVtx[0].tex = D3DXVECTOR2(0.0002f * g_aBg[0].nCountAnim + 0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0002f * g_aBg[0].nCountAnim + 1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0002f * g_aBg[0].nCountAnim + 0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0002f * g_aBg[0].nCountAnim + 1.0f, 1.0f);

	// �e�N�X�`�����W���X�V
	pVtx[4].tex = D3DXVECTOR2(0.0005f * g_aBg[1].nCountAnim - 0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.0005f * g_aBg[1].nCountAnim - 1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0005f * g_aBg[1].nCountAnim - 0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.0005f * g_aBg[1].nCountAnim - 1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// �|���S���`�揈��
//=============================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);	

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBg[g_aBg[nCntBg].nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBg, 2);
	}
}
