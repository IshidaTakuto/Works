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
#define BG_TEXTURENAME0	"data/TEXTURE/bg100.png"	//�w�i�̃e�N�X�`����1
#define BG_TEXTURENAME1	"data/TEXTURE/bg101.png"	//�w�i�̃e�N�X�`����2
#define BG_TEXTURENAME2	"data/TEXTURE/bg102.png"	//�w�i�̃e�N�X�`����3
#define BG_POS_X	(0)				//�w�i�̍���X���W
#define BG_POS_Y	(0)				//�w�i�̍���Y���W
#define BG_WIDTH	(SCREEN_WIDTH)	//�w�i�̕�
#define BG_HEIGHT	(SCREEN_HEIGHT)	//�w�i�̍���
#define TEX_Y_UPDATE	(g_Bg.nCountAnim * 0.005f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBg = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;	//���_�o�b�t�@�ւ̃|�C���^
BG g_Bg;

//=============================================================================
// �|���S������������
//=============================================================================
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�w�i�̏��̏�����
	g_Bg.pos = D3DXVECTOR3(BG_WIDTH / 2, BG_HEIGHT / 2, 0.0f);	//�w�i�̈ʒu
	g_Bg.nCountAnim = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME0, &g_pTextureBg);
	//D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME1, &g_apTextureBg[1]);
	//D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME2, &g_apTextureBg[2]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, 
								D3DUSAGE_WRITEONLY, 
								FVF_VERTEX_2D, 
								D3DPOOL_MANAGED, 
								&g_pVtxBuffBg, 
								NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, BG_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);

	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// �|���S���I������
//=============================================================================
void UninitBg(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}

	//���_�o�b�t�@�̔j��
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
	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	g_Bg.nCountAnim++;

	//�e�N�X�`�����W���X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f - TEX_Y_UPDATE);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f - TEX_Y_UPDATE);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f - TEX_Y_UPDATE);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f - TEX_Y_UPDATE);

	if (pVtx[0].tex.y > 1.0f)
	{
		//���_�̍��W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	if (pVtx[2].tex.y > 1.0f)
	{
		//���_�̍��W
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// �|���S���`�揈��
//=============================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);	

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBg);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	
}
