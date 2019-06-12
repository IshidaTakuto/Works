//=============================================================================
//
// �|�[�Y���j���[���� [menu.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "menu.h"
#include "bg.h"
#include "generator.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MENU_TEXTURENAME1	"data/TEXTURE/menu_game.png"		// �e�N�X�`����
#define MENU_TEXTURENAME2	"data/TEXTURE/menu_tutorial.png"	// �e�N�X�`����
#define MENU_TEXTURENAME3	"data/TEXTURE/menu_title.png"		// �e�N�X�`����
#define MENU_SIZE_X	(300.0f)	// ���j���[�̑傫��(X)
#define MENU_SIZE_Y	(50.0f)		// ���j���[�̑傫��(Y)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMenu[MENUTYPE_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenu = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
PAUSEMENU g_Menu[MENUTYPE_MAX];
int g_nNumSelectMenu;

//=============================================================================
// ����������
//=============================================================================
void InitMenu(void)
{
	// �w�i����������
	InitBg();

	// �u���b�N����������
	InitBlock();

	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���̏�����
	g_Menu[MENUTYPE_GAME].pos = D3DXVECTOR3(640, 100, 0.0f);
	g_Menu[MENUTYPE_TUTORIAL].pos = D3DXVECTOR3(640, 350, 0.0f);
	g_Menu[MENUTYPE_TITLE].pos = D3DXVECTOR3(640, 600, 0.0f);

	g_Menu[MENUTYPE_GAME].nType = 0;
	g_Menu[MENUTYPE_TUTORIAL].nType = 1;
	g_Menu[MENUTYPE_TITLE].nType = 2;

	g_nNumSelectMenu = MENUTYPE_GAME;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME1, &g_pTextureMenu[MENUTYPE_GAME]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME2, &g_pTextureMenu[MENUTYPE_TUTORIAL]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME3, &g_pTextureMenu[MENUTYPE_TITLE]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MENUTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenu,
		NULL);

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < MENUTYPE_MAX; nCntMenu++)
	{
		// ���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x - MENU_SIZE_X, g_Menu[nCntMenu].pos.y - MENU_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x + MENU_SIZE_X, g_Menu[nCntMenu].pos.y - MENU_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x - MENU_SIZE_X, g_Menu[nCntMenu].pos.y + MENU_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x + MENU_SIZE_X, g_Menu[nCntMenu].pos.y + MENU_SIZE_Y, 0.0f);

		// 1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMenu->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitMenu(void)
{
	// �w�i�I������
	UninitBg();

	// �u���b�N�I������
	UninitBlock();

	int nCntMenu;

	for (nCntMenu = 0; nCntMenu < MENUTYPE_MAX; nCntMenu++)
	{
		// �e�N�X�`���̔j��
		if (g_pTextureMenu[nCntMenu] != NULL)
		{
			g_pTextureMenu[nCntMenu]->Release();
			g_pTextureMenu[nCntMenu] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
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
	// �w�i�X�V����
	UpdateBg();

	// �u���b�N�X�V����
	UpdateBlock();

	SelectMenu();

	if (GetKeyboardRepeat(DIK_W) == true || GetKeyboardRepeat(DIK_UP) == true)
	{
		PlaySound(SOUND_LABEL_SE_CURSOR);
		g_nNumSelectMenu = (g_nNumSelectMenu + (MENUTYPE_MAX - 1)) % MENUTYPE_MAX;	// �I����Ԃ��P�グ��
		SelectMenu();
	}

	if (GetKeyboardRepeat(DIK_S) == true || GetKeyboardRepeat(DIK_DOWN) == true)
	{
		PlaySound(SOUND_LABEL_SE_CURSOR);
		g_nNumSelectMenu = (g_nNumSelectMenu + 1) % MENUTYPE_MAX;				// �I����Ԃ��P������
		SelectMenu();
	}

	if (rand() % 740 == 0)
	{// �����_���ɒ����o��
		SetBlock(D3DXVECTOR3(-50, (rand() % 700) * 1.0f, 0.0f), 50.0f,50.0f, D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f), BLOCKTYPE_TITLE);
	}
	else if(rand() % 640 == 0)
	{// �����_���ɒ����o��
		SetBlock(D3DXVECTOR3(50 + SCREEN_WIDTH, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMenu(void)
{
	// �w�i�`�揈��
	DrawBg();

	// �u���b�N�`�揈��
	DrawBlock();

	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntMenu = 0; nCntMenu < MENUTYPE_MAX; nCntMenu++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMenu[g_Menu[nCntMenu].nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMenu, 2);
	}
}

//=============================================================================
// �I������
//=============================================================================
void SelectMenu(void)
{
	FADE pFade;
	pFade = *GetFade();

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_nNumSelectMenu)
		{
		case MENUTYPE_GAME:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);

					SetFade(MODE_GAME);
				}
			}
			break;

		case MENUTYPE_TUTORIAL:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);

					SetFade(MODE_TUTORIAL);
				}
			}
			break;

		case MENUTYPE_TITLE:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);

					SetFade(MODE_TITLE);
				}
			}
			break;
		}
	}

	if (g_nNumSelectMenu == MENUTYPE_GAME)
	{
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_GAME * 4);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelectMenu != MENUTYPE_GAME)
	{
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_GAME * 4);

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// ���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}

	if (g_nNumSelectMenu == MENUTYPE_TUTORIAL)
	{
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_TUTORIAL * 4);

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelectMenu != MENUTYPE_TUTORIAL)
	{
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_TUTORIAL * 4);

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// ���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}

	if (g_nNumSelectMenu == MENUTYPE_TITLE)
	{
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_TITLE * 4);

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelectMenu != MENUTYPE_TITLE)
	{
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_TITLE * 4);

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// ���_�o�b�t�@���A�����b�N
		g_pVtxBuffMenu->Unlock();
	}
}

