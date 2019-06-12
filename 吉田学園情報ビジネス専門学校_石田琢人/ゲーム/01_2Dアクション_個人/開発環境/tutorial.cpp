//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"
#include "block.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TUTORIAL_TEXTURENAME0	"DATA/TEXTURE/tutorial000.png"	// �^�C�g���̃e�N�X�`����
#define TUTORIAL_TEXTURENAME1	"DATA/TEXTURE/press_a.png"	// �^�C�g���̃e�N�X�`����
#define TUTORIAL_TEXTURENAME2	"DATA/TEXTURE/press_d.png"	// �^�C�g���̃e�N�X�`����
#define TUTORIAL_TEXTURENAME3	"DATA/TEXTURE/press_enter.png"	// �^�C�g���̃e�N�X�`����
#define MAX_TUTORIAL	(4)
#define LOGO_SIZE_X		(150)
#define LOGO_SIZE_Y		(50)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTutorial[MAX_TUTORIAL] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
TUTORIALSTATE g_State;
TUTORIAL g_aTutorial[MAX_TUTORIAL];
float g_fCntMove;

//=============================================================================
// �^�C�g������������
//=============================================================================
void InitTutorial(void)
{
	// �w�i����������
	InitBg();

	// �u���b�N����������
	InitBlock();

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	// ������
	g_aTutorial[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aTutorial[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTutorial[0].nType = 0;
	g_aTutorial[0].nTransparent = 0;

	// a
	g_aTutorial[1].pos = D3DXVECTOR3(LOGO_SIZE_X + 20, SCREEN_HEIGHT - (LOGO_SIZE_Y + 20), 0.0f);
	g_aTutorial[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTutorial[1].nType = 1;
	g_aTutorial[1].nTransparent = 0;

	// d
	g_aTutorial[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (LOGO_SIZE_X + 20), SCREEN_HEIGHT - (LOGO_SIZE_Y + 20), 0.0f);
	g_aTutorial[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTutorial[2].nType = 2;
	g_aTutorial[2].nTransparent = 0;

	// enter
	g_aTutorial[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (LOGO_SIZE_X + 20), SCREEN_HEIGHT - (LOGO_SIZE_Y + 20), 0.0f);
	g_aTutorial[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTutorial[3].nType = 3;
	g_aTutorial[3].nTransparent = 0;

	// �`���[�g���A���̏��
	g_State = TUTORIALSTATE_NORMAL;

	g_fCntMove = 0.0f;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TUTORIAL_TEXTURENAME0, &g_pTextureTutorial[0]);
	D3DXCreateTextureFromFile(pDevice, TUTORIAL_TEXTURENAME1, &g_pTextureTutorial[1]);
	D3DXCreateTextureFromFile(pDevice, TUTORIAL_TEXTURENAME2, &g_pTextureTutorial[2]);
	D3DXCreateTextureFromFile(pDevice, TUTORIAL_TEXTURENAME3, &g_pTextureTutorial[3]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TUTORIAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

	for (int nCntTutorial = 1; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�

		// ���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x - LOGO_SIZE_X, g_aTutorial[nCntTutorial].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x + LOGO_SIZE_X, g_aTutorial[nCntTutorial].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x - LOGO_SIZE_X, g_aTutorial[nCntTutorial].pos.y + LOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x + LOGO_SIZE_X, g_aTutorial[nCntTutorial].pos.y + LOGO_SIZE_Y, 0.0f);

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

	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTutorial->Unlock();
}

//=============================================================================
// �^�C�g���I������
//=============================================================================
void UninitTutorial(void)
{
	// �w�i�I������
	UninitBg();

	// �u���b�N�I������
	UninitBlock();

	for (int nCntTexture = 0; nCntTexture < MAX_TUTORIAL; nCntTexture++)
	{
		// �e�N�X�`���̔j��
		if (g_pTextureTutorial[nCntTexture] != NULL)
		{
			g_pTextureTutorial[nCntTexture]->Release();
			g_pTextureTutorial[nCntTexture] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//=============================================================================
// �^�C�g���X�V����
//=============================================================================
void UpdateTutorial(void)
{
	int nCntTutorial;

	// �w�i�X�V����
	UpdateBg();

	// �u���b�N�X�V����
	UpdateBlock();

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		if (nCntTutorial != 0)
		{
			if (g_aTutorial[nCntTutorial].col.a <= 1.0f && g_aTutorial[nCntTutorial].nTransparent == 0)
			{//	�����x��1.0f�̎�
				g_aTutorial[nCntTutorial].col.a -= 0.02f;
				if (g_aTutorial[nCntTutorial].col.a <= 0.0f)
				{// 0.0f�ȉ��ɂȂ����Ƃ�
					g_aTutorial[nCntTutorial].col.a = 0.0f;
					g_aTutorial[nCntTutorial].nTransparent = 1;
				}
			}
			else if (g_aTutorial[nCntTutorial].col.a >= 0.0f && g_aTutorial[nCntTutorial].nTransparent == 1)
			{//	�����x��0.0f�̎�
				g_aTutorial[nCntTutorial].col.a += 0.02f;
				if (g_aTutorial[nCntTutorial].col.a >= 1.0f)
				{// 1.0f�ȏ�ɂȂ����Ƃ�
					g_aTutorial[nCntTutorial].col.a = 1.0f;
					g_aTutorial[nCntTutorial].nTransparent = 0;
				}
			}
		}
	}

	switch (g_State)
	{
	case TUTORIALSTATE_NORMAL:
		// enter�ƍ�������
		g_aTutorial[1].col.a = 0.0f;
		g_aTutorial[3].col.a = 0.0f;
		break;

	case TUTORIALSTATE_MOVE_L:
		// �e�N�X�`�����W��ύX
		pVtx[0].tex.x = g_fCntMove;
		pVtx[1].tex.x = g_fCntMove + 0.25f;
		pVtx[2].tex.x = g_fCntMove;
		pVtx[3].tex.x = g_fCntMove + 0.25f;

		break;

	case TUTORIALSTATE_MOVE_R:
		// �e�N�X�`�����W��ύX
		pVtx[0].tex.x = g_fCntMove;
		pVtx[1].tex.x = g_fCntMove + 0.25f;
		pVtx[2].tex.x = g_fCntMove;
		pVtx[3].tex.x = g_fCntMove + 0.25f;
		break;

	case TUTORIALSTATE_MIDDLE:
		// enter������
		g_aTutorial[3].col.a = 0.0f;
		break;

	case TUTORIALSTATE_END:
		// �E������
		g_aTutorial[2].col.a = 0.0f;
		break;
	}

		if (pVtx[0].tex.x <= 0.0f)
		{// �X�e�[�g�ύX
			g_State = TUTORIALSTATE_NORMAL;
		}
		else if(pVtx[0].tex.x >= 0.75f)
		{// �X�e�[�g�ύX
			g_State = TUTORIALSTATE_END;
		}
		else if(pVtx[0].tex.x == 0.25f || pVtx[0].tex.x == 0.5f)
		{// �X�e�[�g�ύX
			g_State = TUTORIALSTATE_MIDDLE;
		}

		if (pVtx[0].tex.x >= 0.0f && pVtx[0].tex.x <= 0.75f)
		{
			if (GetKeyboardTrigger(DIK_A) == true)
			{// �X�e�[�g�ύX
				if (g_State != TUTORIALSTATE_NORMAL)
				{
					PlaySound(SOUND_LABEL_SE_TURN);
					g_fCntMove -= 0.25f;
					g_State = TUTORIALSTATE_MOVE_L;
				}
			}
			if (GetKeyboardTrigger(DIK_D) == true)
			{// �X�e�[�g�ύX
				if (g_State != TUTORIALSTATE_END)
				{
					PlaySound(SOUND_LABEL_SE_TURN);
					g_fCntMove += 0.25f;
					g_State = TUTORIALSTATE_MOVE_R;
				}
			}
		}


	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTutorial->Unlock();

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		// ���_�J���[
		pVtx[0].col = g_aTutorial[nCntTutorial].col;
		pVtx[1].col = g_aTutorial[nCntTutorial].col;
		pVtx[2].col = g_aTutorial[nCntTutorial].col;
		pVtx[3].col = g_aTutorial[nCntTutorial].col;

		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTutorial->Unlock();

	FADE pFade;

	// �t�F�[�h�̎擾
	pFade = *GetFade();

	if (g_State == TUTORIALSTATE_END)
	{
		if (pFade == FADE_NONE)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);
				// ���[�h�I��
				SetFade(MODE_GAME);
			}
		}
	}

	if (rand() % 740 == 0)
	{// �����_���ɒ����o��
		SetBlock(D3DXVECTOR3(-50, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}
	else if (rand() % 640 == 0)
	{// �����_���ɒ����o��
		SetBlock(D3DXVECTOR3(50 + SCREEN_WIDTH, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}
}

//=============================================================================
// �`���[�g���A���`�揈��
//=============================================================================
void DrawTutorial(void)
{
	// �w�i�`�揈��
	DrawBg();

	// �u���b�N�`�揈��
	DrawBlock();

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorial[g_aTutorial[nCntTutorial].nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTutorial, 2);
	}
}
