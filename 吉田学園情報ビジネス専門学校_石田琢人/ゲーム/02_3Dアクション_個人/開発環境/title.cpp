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
#include "camera.h"
#include "rain.h"
#include "effect.h"
#include "petals.h"
#include "snow.h"
#include "joypad.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_TEXTURENAME0	"DATA/TEXTURE/title_logo.png"	// �^�C�g���̃e�N�X�`����
#define TITLE_TEXTURENAME1	"DATA/TEXTURE/press_enter.png"	// �^�C�g���̃e�N�X�`����
#define TITLE_SIZE_X	(380.0f)	// �^�C�g���̑傫��(X)
#define TITLE_SIZE_Y	(125.0f)	// �^�C�g���̑傫��(Y)
#define ENTER_WIDTH		(150.0f)	// �uPRESS ENTER�v�̕�
#define ENTER_HEIGHT	(50.0f)		// �uPRESS ENTER�v�̍���
#define MAX_TITLE		(2)			// �^�C�g���̃e�N�X�`����
#define TITLE_TIME		(400)		// �����L���O��ʂ܂ł̎���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
TITLE g_aTitle[MAX_TITLE];
int g_nTransparent;
int g_nCntTitle;
int g_nTitleSeason = 3;

//=============================================================================
// �^�C�g������������
//=============================================================================
void InitTitle(void)
{
	// �J�����̏���������
	InitCamera();

	// �J�̏���������
	InitRain();

	InitSnow();

	InitPetals();

	// �G�t�F�N�g����������
	InitEffect();
	SetEffect(D3DXVECTOR3(200.0f, 0.0f, 500.0f), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), 100.0f, 0, EFFECTSTATE_MOON);
	SetEffect(D3DXVECTOR3(200.0f, 0.0f, 500.0f), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), 120.0f, 0, EFFECTSTATE_NONE);

	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();


	g_aTitle[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f, 0.0f);
	g_aTitle[0].nType = 0;
	g_aTitle[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f);
	g_aTitle[1].nType = 1;
	g_aTitle[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	g_nTransparent = 0;
	g_nCntTitle = 0;

	g_nTitleSeason = (g_nTitleSeason + 1) % 4;

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�̍��W
	pVtx[4].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_WIDTH, g_aTitle[1].pos.y - ENTER_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_WIDTH, g_aTitle[1].pos.y - ENTER_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_WIDTH, g_aTitle[1].pos.y + ENTER_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_WIDTH, g_aTitle[1].pos.y + ENTER_HEIGHT, 0.0f);

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
	// �J�����̏I������
	UninitCamera();

	// �J�̏I������
	UninitRain();

	UninitSnow();

	UninitPetals();

	// �G�t�F�N�g�̏I������
	UninitEffect();

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
	XINPUT_STATE state;
	state = GetJoyPad();

	// �J�����̍X�V����
	UpdateCamera();

	switch (g_nTitleSeason)
	{
	case 0:
		UpdatePetals(PETALSTYPE_SAKURA);
		break;

	case 1:
		// �J�̍X�V����
		UpdateRain();
		break;

	case 2:
		UpdatePetals(PETALSTYPE_MOMIJI);
		break;

	case 3:
		UpdateSnow();
		break;
	}

	// �G�t�F�N�g�̍X�V����
	UpdateEffect();

	//if (g_aTitle[0].pos.y < SCREEN_HEIGHT / 2 - 100)
	//{// �ォ��
	//	g_aTitle[0].pos.y++;

	//	if (GetKeyboardTrigger(DIK_RETURN) == true)
	//	{// �X�L�b�v
	//		g_aTitle[0].pos.y = SCREEN_HEIGHT / 2 - 100;
	//	}
	//}
	if (g_aTitle[0].col.a < 1.0f)
	{
		g_aTitle[0].col.a += 0.005f;

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// �X�L�b�v
			g_aTitle[0].col.a = 1.0f;
		}
	}
	else
	{
		g_nCntTitle++;	// �����L���O��ʂ܂ł̃J�E���^

		FADE pFade;

		// �t�F�[�h�̎擾
		pFade = *GetFade();
		if (pFade == FADE_NONE)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{// ���j���[��ʂ�
				PlaySound(SOUND_LABEL_DECIDE);
				// ���[�h�I��
				SetFade(MODE_MENU);
			}
			else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				PlaySound(SOUND_LABEL_DECIDE);
				// ���[�h�I��
				SetFade(MODE_MENU);
			}
			else if (g_nCntTitle >= TITLE_TIME)
			{// ���Ԍo�߂Ń����L���O��ʂ�
				//���[�h�I��
				SetFade(MODE_RANKING);
			}
		}

		if (g_aTitle[1].col.a <= 1.0f && g_nTransparent == 0)
		{//	�����x��1.0f�̎�
			g_aTitle[1].col.a -= 0.02f;
			if (g_aTitle[1].col.a <= 0.0f)
			{// 0.0f�ȉ��ɂȂ����Ƃ�
				g_aTitle[1].col.a = 0.0f;
				g_nTransparent = 1;
			}
		}
		else if (g_aTitle[1].col.a >= 0.0f && g_nTransparent == 1)
		{//	�����x��0.0f�̎�
			g_aTitle[1].col.a += 0.02f;
			if (g_aTitle[1].col.a >= 1.0f)
			{// 1.0f�ȏ�ɂȂ����Ƃ�
				g_aTitle[1].col.a = 1.0f;
				g_nTransparent = 0;
			}
		}
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
	pVtx[4].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_WIDTH, g_aTitle[1].pos.y - ENTER_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_WIDTH, g_aTitle[1].pos.y - ENTER_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_WIDTH, g_aTitle[1].pos.y + ENTER_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_WIDTH, g_aTitle[1].pos.y + ENTER_HEIGHT, 0.0f);

	// ���_�J���[
	pVtx[0].col = g_aTitle[0].col;
	pVtx[1].col = g_aTitle[0].col;
	pVtx[2].col = g_aTitle[0].col;
	pVtx[3].col = g_aTitle[0].col;

	pVtx[4].col = g_aTitle[1].col;
	pVtx[5].col = g_aTitle[1].col;
	pVtx[6].col = g_aTitle[1].col;
	pVtx[7].col = g_aTitle[1].col;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}

//=============================================================================
// �^�C�g���`�揈��
//=============================================================================
void DrawTitle(void)
{
	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	switch (g_nTitleSeason)
	{
	case 0:
		DrawPetals();
		break;

	case 1:
		// �J�̍X�V����
		DrawRain();
		break;

	case 2:
		DrawPetals();
		break;

	case 3:
		DrawSnow();
		break;
	}

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
}
