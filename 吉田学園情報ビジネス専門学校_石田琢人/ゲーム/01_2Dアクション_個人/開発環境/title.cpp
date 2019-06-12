//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"
#include "block.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_TEXTURENAME0	"DATA/TEXTURE/title_logo.png"	// �^�C�g���̃e�N�X�`����
#define TITLE_TEXTURENAME1	"DATA/TEXTURE/press_enter.png"	// �^�C�g���̃e�N�X�`����
#define TITLE_SIZE_X	(450.0f)	// �^�C�g���̑傫��(X)
#define TITLE_SIZE_Y	(225.0f)	// �^�C�g���̑傫��(Y)
#define ENTER_SIZE_X	(150.0f)	// �uPRESS ENTER�v�̑傫��(X)
#define ENTER_SIZE_Y	(50.0f)		// �uPRESS ENTER�v�̑傫��(Y)
#define MAX_TITLE		(2)			// �^�C�g���̃e�N�X�`����
#define TITLE_TIME		(900)		// �����L���O��ʂ܂ł̎���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
TITLE g_aTitle[MAX_TITLE];
D3DXCOLOR g_colorTitle;	// �uPRESS ENTER�v�̓_��
int g_nTransparent;
int g_nCntTitle;

//=============================================================================
// �^�C�g������������
//=============================================================================
void InitTitle(void)
{
	// �w�i����������
	InitBg();

	// �u���b�N����������
	InitBlock();

	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();


	g_aTitle[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, -55, 0.0f);
	g_aTitle[0].nType = 0;

	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f);
	g_aTitle[1].nType = 1;

	g_colorTitle = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	g_nTransparent = 0;
	g_nCntTitle = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME0, &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME1, &g_pTextureTitle[1]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_aTitle[0].pos.x - TITLE_SIZE_X, g_aTitle[0].pos.y - TITLE_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTitle[0].pos.x + TITLE_SIZE_X, g_aTitle[0].pos.y - TITLE_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTitle[0].pos.x - TITLE_SIZE_X, g_aTitle[0].pos.y + TITLE_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTitle[0].pos.x + TITLE_SIZE_X, g_aTitle[0].pos.y + TITLE_SIZE_Y, 0.0f);

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
	pVtx[4].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_SIZE_X, g_aTitle[1].pos.y - ENTER_SIZE_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_SIZE_X, g_aTitle[1].pos.y - ENTER_SIZE_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_SIZE_X, g_aTitle[1].pos.y + ENTER_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_SIZE_X, g_aTitle[1].pos.y + ENTER_SIZE_Y, 0.0f);

	// 1.0f�ŌŒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	// �e�N�X�`�����W
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}

//=============================================================================
// �^�C�g���I������
//=============================================================================
void UninitTitle(void)
{
	// �w�i�I������
	UninitBg();

	// �u���b�N�I������
	UninitBlock();

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{
		// �e�N�X�`���̔j��
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//=============================================================================
// �^�C�g���X�V����
//=============================================================================
void UpdateTitle(void)
{
	// �w�i�X�V����
	UpdateBg();

	// �u���b�N�X�V����
	UpdateBlock();

	if (g_aTitle[0].pos.y < SCREEN_HEIGHT / 2 - 100)
	{// �ォ��
		g_aTitle[0].pos.y++;

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// �X�L�b�v
			g_aTitle[0].pos.y = SCREEN_HEIGHT / 2 - 100;
		}
	}
	else
	{// ���n�_�ɂ����Ƃ�
		g_nCntTitle++;	// �����L���O��ʂ܂ł̃J�E���^

		FADE pFade;

		// �t�F�[�h�̎擾
		pFade = *GetFade();
		if (pFade == FADE_NONE)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{// ���j���[��ʂ�
				PlaySound(SOUND_LABEL_SE_DECIDE);
				// ���[�h�I��
				SetFade(MODE_MENU);
			}
			else if (g_nCntTitle >= TITLE_TIME)
			{// ���Ԍo�߂Ń����L���O��ʂ�
				//���[�h�I��
				SetFade(MODE_RANKING);
			}
		}

		if (g_colorTitle.a <= 1.0f && g_nTransparent == 0)
		{//	�����x��1.0f�̎�
			g_colorTitle.a -= 0.02f;
			if (g_colorTitle.a <= 0.0f)
			{// 0.0f�ȉ��ɂȂ����Ƃ�
				g_colorTitle.a = 0.0f;
				g_nTransparent = 1;
			}
		}
		else if (g_colorTitle.a >= 0.0f && g_nTransparent == 1)
		{//	�����x��0.0f�̎�
			g_colorTitle.a += 0.02f;
			if (g_colorTitle.a >= 1.0f)
			{// 1.0f�ȏ�ɂȂ����Ƃ�
				g_colorTitle.a = 1.0f;
				g_nTransparent = 0;
			}
		}

		if (g_nCntTitle % 450 == 0)
		{
			SetBlock(D3DXVECTOR3(-100, SCREEN_HEIGHT - 80.0f, 0.0f), 100.0f, 100.0f, D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 70.0f, 0.0f), BLOCKTYPE_SHARK);
		}
		else if (g_nCntTitle % 700 == 0)
		{
			SetBlock(D3DXVECTOR3(100 + SCREEN_WIDTH, SCREEN_HEIGHT - 80.0f, 0.0f), 100.0f, 100.0f, D3DXVECTOR3(0, SCREEN_HEIGHT - 70.0f, 0.0f), BLOCKTYPE_SHARK);
		}
		//else if (g_nCntTitle % 100 == 0)
		//{
		//	SetParticle(D3DXVECTOR3(rand() % SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT, 0.0f),D3DXCOLOR(0.0f,0.0f,1.0f,1.0f), rand() % 20 + (rand() % 10 * 0.2f), 80, PARTICLETYPE_NORMAL);
		//}
	}

	if (rand() % 740 == 0)
	{// �����_���ɒ����o��
		SetBlock(D3DXVECTOR3(-50, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}
	else if (rand() % 640 == 0)
	{// �����_���ɒ����o��
		SetBlock(D3DXVECTOR3(50 + SCREEN_WIDTH, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_aTitle[0].pos.x - TITLE_SIZE_X, g_aTitle[0].pos.y - TITLE_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTitle[0].pos.x + TITLE_SIZE_X, g_aTitle[0].pos.y - TITLE_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTitle[0].pos.x - TITLE_SIZE_X, g_aTitle[0].pos.y + TITLE_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTitle[0].pos.x + TITLE_SIZE_X, g_aTitle[0].pos.y + TITLE_SIZE_Y, 0.0f);

	// ���_�̍��W
	pVtx[4].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_SIZE_X, g_aTitle[1].pos.y - ENTER_SIZE_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_SIZE_X, g_aTitle[1].pos.y - ENTER_SIZE_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_SIZE_X, g_aTitle[1].pos.y + ENTER_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_SIZE_X, g_aTitle[1].pos.y + ENTER_SIZE_Y, 0.0f);

	// ���_�J���[
	pVtx[4].col = g_colorTitle;
	pVtx[5].col = g_colorTitle;
	pVtx[6].col = g_colorTitle;
	pVtx[7].col = g_colorTitle;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}

//=============================================================================
// �^�C�g���`�揈��
//=============================================================================
void DrawTitle(void)
{
	// �w�i�`�揈��
	DrawBg();

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[g_aTitle[nCntTitle].nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTitle, 2);
	}

	// �u���b�N�`�揈��
	DrawBlock();
}
