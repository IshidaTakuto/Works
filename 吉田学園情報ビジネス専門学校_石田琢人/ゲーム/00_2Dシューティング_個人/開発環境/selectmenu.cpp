//=============================================================================
//
// �|�[�Y���j���[���� [selectmenu.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "selectmenu.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SELECTMENU_TEXTURENAME0	"data/TEXTURE/pause000.png"	//�|�[�Y�̃e�N�X�`����
#define SELECTMENU_TEXTURENAME1	"data/TEXTURE/pause001.png"	//�|�[�Y�̃e�N�X�`����
#define SELECTMENU_TEXTURENAME2	"data/TEXTURE/pause002.png"	//�|�[�Y�̃e�N�X�`����
#define TEX_UPDATE	(g_aSelectmenu.nCountAnim * 0.005f)
#define MAX_MENU	(3)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureSelectmenu[MAX_MENU];		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectmenu = NULL;	//���_�o�b�t�@�ւ̃|�C���^
SELECTMENU g_aSelectmenu[MAX_MENU];

//=============================================================================
// ����������
//=============================================================================
void InitSelectmenu(void)
{
	int nCntSelectmenu;

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (nCntSelectmenu = 0; nCntSelectmenu < MAX_MENU; nCntSelectmenu++)
	{
		//���̏�����
		g_aSelectmenu[nCntSelectmenu].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�w�i�̈ʒu
		g_aSelectmenu[nCntSelectmenu].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSelectmenu[nCntSelectmenu].state = SELECTMENUSTATE_CONTINUE;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, SELECTMENU_TEXTURENAME0, &g_apTextureSelectmenu[0]);
	D3DXCreateTextureFromFile(pDevice, SELECTMENU_TEXTURENAME1, &g_apTextureSelectmenu[1]);
	D3DXCreateTextureFromFile(pDevice, SELECTMENU_TEXTURENAME2, &g_apTextureSelectmenu[2]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectmenu,
		NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffSelectmenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSelectmenu = 0; nCntSelectmenu < MAX_MENU; nCntSelectmenu++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x - 50, g_aSelectmenu[nCntSelectmenu].pos.y - 50, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x + 50, g_aSelectmenu[nCntSelectmenu].pos.y - 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x - 50, g_aSelectmenu[nCntSelectmenu].pos.y + 50, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x + 50, g_aSelectmenu[nCntSelectmenu].pos.y + 50, 0.0f);

		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffSelectmenu->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitSelectmenu(void)
{
	int nCntSelectmenu;

	//�e�N�X�`���̔j��
	for (nCntSelectmenu = 0; nCntSelectmenu < MAX_MENU; nCntSelectmenu++)
	{
		if (g_apTextureSelectmenu[nCntSelectmenu] != NULL)
		{
			g_apTextureSelectmenu[nCntSelectmenu]->Release();
			g_apTextureSelectmenu[nCntSelectmenu] = NULL;
		}

	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSelectmenu != NULL)
	{
		g_pVtxBuffSelectmenu->Release();
		g_pVtxBuffSelectmenu = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSelectmenu(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSelectmenu(void)
{
	int nCntSelectmenu;

	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSelectmenu, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	//�|���S���̕`��
	for (nCntSelectmenu = 0; nCntSelectmenu < MAX_MENU; nCntSelectmenu++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureSelectmenu[g_aSelectmenu[nCntSelectmenu].state]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSelectmenu, 2);
	}
}

//=============================================================================
// �Z���N�g���j���[�̐ݒ�
//=============================================================================
void SetSelectmenu(D3DXVECTOR3 pos, D3DXCOLOR col, SELECTMENUSTATE state)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntSelectmenu;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffSelectmenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSelectmenu = 0; nCntSelectmenu < MAX_MENU; nCntSelectmenu++)
	{
		g_aSelectmenu[nCntSelectmenu].pos = pos;
		g_aSelectmenu[nCntSelectmenu].col = col;
		g_aSelectmenu[nCntSelectmenu].state = state;

		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x - 50, g_aSelectmenu[nCntSelectmenu].pos.y - 50, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x + 50, g_aSelectmenu[nCntSelectmenu].pos.y - 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x - 50, g_aSelectmenu[nCntSelectmenu].pos.y + 50, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x + 50, g_aSelectmenu[nCntSelectmenu].pos.y + 50, 0.0f);

		break;
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffSelectmenu->Unlock();
}