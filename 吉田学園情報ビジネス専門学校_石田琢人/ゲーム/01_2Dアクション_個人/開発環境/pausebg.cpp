//=============================================================================
//
// �|�[�Y���� [Pausebg.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "pausebg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PAUSEBG_TEXTURENAME	"data/TEXTURE/pause100.png"	//�|�[�Y�̃e�N�X�`����
#define TEX_UPDATE	(g_Pausebg.nCountAnim * 0.005f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePausebg = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPausebg = NULL;	//���_�o�b�t�@�ւ̃|�C���^
PAUSEBG g_Pausebg;

//=============================================================================
// ����������
//=============================================================================
void InitPausebg(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�w�i�̏��̏�����
	g_Pausebg.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);	//�w�i�̈ʒu
	g_Pausebg.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	g_Pausebg.nCountAnim = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PAUSEBG_TEXTURENAME, &g_pTexturePausebg);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPausebg,
		NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

					//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPausebg->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_Pausebg.pos.x - 200, g_Pausebg.pos.y - 200, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Pausebg.pos.x + 200, g_Pausebg.pos.y - 200, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Pausebg.pos.x - 200, g_Pausebg.pos.y + 200, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Pausebg.pos.x + 200, g_Pausebg.pos.y + 200, 0.0f);

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

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPausebg->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitPausebg(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePausebg != NULL)
	{
		g_pTexturePausebg->Release();
		g_pTexturePausebg = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPausebg != NULL)
	{
		g_pVtxBuffPausebg->Release();
		g_pVtxBuffPausebg = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePausebg(void)
{
	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPausebg->Lock(0, 0, (void**)&pVtx, 0);

	g_Pausebg.nCountAnim++;

	// �e�N�X�`�����W���X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f - TEX_UPDATE, 0.0f - TEX_UPDATE);
	pVtx[1].tex = D3DXVECTOR2(1.0f - TEX_UPDATE, 0.0f - TEX_UPDATE);
	pVtx[2].tex = D3DXVECTOR2(0.0f - TEX_UPDATE, 1.0f - TEX_UPDATE);
	pVtx[3].tex = D3DXVECTOR2(1.0f - TEX_UPDATE, 1.0f - TEX_UPDATE);

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);

	if (pVtx[0].tex.x > 1.0f)
	{
		// ���_�̍��W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	}

	if (pVtx[1].tex.x > 1.0f)
	{
		// ���_�̍��W
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPausebg->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPausebg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPausebg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePausebg);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
