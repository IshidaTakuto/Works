//=============================================================================
//
// �|�[�Y���j���[���� [menu.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "menu.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MENU_TEXTURENAME1	"data/TEXTURE/pause000.png"	//�|�[�Y�̃e�N�X�`����
#define MENU_TEXTURENAME2	"data/TEXTURE/pause001.png"	//�|�[�Y�̃e�N�X�`����
#define MENU_TEXTURENAME3	"data/TEXTURE/pause002.png"	//�|�[�Y�̃e�N�X�`����
#define MAX_SELECTMENU	(3)
#define MENU_SIZE_X	(200.0f)		//���j���[�̑傫��(X)
#define MENU_SIZE_Y	(45.0f)			//���j���[�̑傫��(Y)
#define MENU_FLASHING	(10)		//���j���[�̓_��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMenu[MAX_MENU] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenu = NULL;	//���_�o�b�t�@�ւ̃|�C���^
MENU g_Menu[MAX_MENU];
int nSelect;	//���݂̏��
int g_nNumSelect;

//=============================================================================
// ����������
//=============================================================================
void InitMenu(void)
{
	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���̏�����
	g_Menu[CONTINUE].pos = D3DXVECTOR3(640, 250, 0.0f);	
	g_Menu[RETRY].pos = D3DXVECTOR3(640, 350, 0.0f);
	g_Menu[QUIT].pos = D3DXVECTOR3(640, 450, 0.0f);

	g_Menu[CONTINUE].nType = 0;
	g_Menu[RETRY].nType = 1;
	g_Menu[QUIT].nType = 2;

	g_nNumSelect = CONTINUE;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME1, &g_pTextureMenu[CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME2, &g_pTextureMenu[RETRY]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME3, &g_pTextureMenu[QUIT]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenu,
		NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < MAX_MENU; nCntMenu++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x - MENU_SIZE_X, g_Menu[nCntMenu].pos.y - MENU_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x + MENU_SIZE_X, g_Menu[nCntMenu].pos.y - MENU_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x - MENU_SIZE_X, g_Menu[nCntMenu].pos.y + MENU_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x + MENU_SIZE_X, g_Menu[nCntMenu].pos.y + MENU_SIZE_Y, 0.0f);

		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMenu->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitMenu(void)
{
	int nCntMenu;

	for (nCntMenu = 0; nCntMenu < MAX_MENU; nCntMenu++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureMenu[nCntMenu] != NULL)
		{
			g_pTextureMenu[nCntMenu]->Release();
			g_pTextureMenu[nCntMenu] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMenu != NULL)
	{
		g_pVtxBuffMenu->Release();
		g_pVtxBuffMenu = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMenu(void)
{
	SelectMenu();

	if (GetKeyboardRepeat(DIK_UP) == true)
	{
		g_nNumSelect = (g_nNumSelect + (MAX_MENU - 1)) % MAX_MENU;	//�I����Ԃ��P�グ��
		SelectMenu();
	}

	if (GetKeyboardRepeat(DIK_DOWN) == true)
	{
		g_nNumSelect = (g_nNumSelect + 1) % MAX_MENU;				//�I����Ԃ��P������
		SelectMenu();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMenu(void)
{
	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntMenu = 0; nCntMenu < MAX_MENU; nCntMenu++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMenu[g_Menu[nCntMenu].nType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMenu, 2);
	}
}

//=============================================================================
// �I������
//=============================================================================
void SelectMenu(void)
{
	PAUSECHECK *pPause;
	pPause = GetPauseMenu();

	FADE pFade;
	pFade = *GetFade();

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_nNumSelect)
		{
		case CONTINUE:
			pPause->bPause = false;
			break;

		case RETRY:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{

					SetFade(MODE_GAME);
				}
			}
			break;

		case QUIT:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					SetFade(MODE_TITLE);
				}
			}
			break;
		}
	}

	if (g_nNumSelect == CONTINUE)
	{
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (CONTINUE * 4);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelect != CONTINUE)
	{
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (CONTINUE * 4);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}

	if (g_nNumSelect == RETRY)
	{
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (RETRY * 4);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelect != RETRY)
	{
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (RETRY * 4);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}

	if (g_nNumSelect == QUIT)
	{
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (QUIT * 4);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelect != QUIT)
	{
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (QUIT * 4);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}
}

