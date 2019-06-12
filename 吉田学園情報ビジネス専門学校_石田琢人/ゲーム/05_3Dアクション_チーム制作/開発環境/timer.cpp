//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "timer.h"
#include "game.h"
#include "fade.h"
#include "load.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TIMER_00	"data/TEXTURE/number.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TIMER_01	"data/TEXTURE/icon_timer.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define NUM_TIMER	(3)		// �^�C�}�[�̍ő包��
#define TIMER_SIZE	(25)	// �^�C�}�[�̕����̑傫��
#define MAX_TEX		(2)
#define MAX_TIMER	(NUM_TIMER + 2)
#define FRAME_SIZE_X	(120)	// �t���[���̑傫��
#define FRAME_SIZE_Y	(100)	// �t���[���̑傫��
#define TIMER_TIME		(105)	// ����
#define TIMER_POSITION	(935.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTimer[MAX_TEX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
TIMER g_aTimer[MAX_TIMER];		// �^�C�}�[
int g_nTimer;		// ��������
int g_nCntSecond;	// �b�̃J�E���g

//=============================================================================
// ����������
//=============================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTimer = 0; nCntTimer < NUM_TIMER; nCntTimer++)
	{
		g_aTimer[nCntTimer].pos = D3DXVECTOR3(TIMER_POSITION - (nCntTimer * TIMER_SIZE * 2.0f), 50.0f, 0.0f);
		g_aTimer[nCntTimer].nType = 0;
	}

	g_aTimer[NUM_TIMER].pos = D3DXVECTOR3(940 - (NUM_TIMER * TIMER_SIZE * 2), 50, 0.0f);
	g_aTimer[NUM_TIMER].nType = 1;

	g_aTimer[NUM_TIMER + 1].pos = D3DXVECTOR3(770 - TIMER_SIZE + (NUM_TIMER * TIMER_SIZE * 2.0f), 100, 0.0f);
	g_aTimer[NUM_TIMER + 1].nType = -1;

	g_nTimer = TIMER_TIME;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TIMER_00, &g_pTextureTimer[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TIMER_01, &g_pTextureTimer[1]);

	// ���_���̍쐬
	MakeVertexTimer(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitTimer(void)
{
	for (int nCntTimer = 0; nCntTimer < MAX_TEX; nCntTimer++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureTimer[nCntTimer] != NULL)
		{
			g_pTextureTimer[nCntTimer]->Release();
			g_pTextureTimer[nCntTimer] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTimer(void)
{
	bool bSwap = GetStageSwat();
	bool bFade = GetFadeNow();
	LOADFADE loadFade = GetLoadFade();

	// �t�F�[�h�ƃN���A���b�Z�[�W�̂Ƃ��͓����Ȃ�����
	if (false == bSwap && false == bFade && LOADFADE_NONE == loadFade)
	{
		g_nCntSecond++;

		if (g_nCntSecond % 60 == 0 && g_nTimer > 0)
		{// ���Ԃ����炷
			g_nTimer--;	// �J�E���g�_�E��
		}

		if (g_nTimer <= 0 && g_nCntSecond % 180 == 0)
		{// �^�C���I�[�o�[
			SetGameState(GAMESTATE_END);
		}
	}

		int aNumber[NUM_TIMER];

		aNumber[0] = g_nTimer % 10 / 1;
		aNumber[1] = g_nTimer % 100 / 10;
		aNumber[2] = g_nTimer % 1000 / 100;

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// ����
	int nDigit = (int)log10f((float)g_nTimer) + 1;
	if (nDigit <= 0) { nDigit = 1; }

	for (int nCntTimer = 0; nCntTimer < nDigit; nCntTimer++)
	{
		//�e�N�X�`�����W
		pVtx[(nCntTimer * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[(nCntTimer * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[(nCntTimer * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntTimer] * 0.1f, 1.0f);
		pVtx[(nCntTimer * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntTimer] * 0.1f, 1.0f);

		// ���_�J���[
		pVtx[(nCntTimer * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		// �����ɂ���Ĉʒu��ς���
		if (g_nTimer < 10)
		{// �ꌅ
			g_aTimer[0].pos.x = TIMER_POSITION - ((NUM_TIMER - 1) * 50);

			// �񌅖ڂ𓧖��ɂ���
			pVtx[(1 * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(1 * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(1 * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(1 * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		else if (g_nTimer < 100)
		{// ��
			g_aTimer[0].pos.x = TIMER_POSITION - ((NUM_TIMER - 2) * 50);
			g_aTimer[1].pos.x = TIMER_POSITION - ((NUM_TIMER - 1) * 50);

			// �O���ڂ𓧖��ɂ���
			pVtx[(nDigit * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nDigit * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nDigit * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nDigit * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		else
		{// �O��
			g_aTimer[0].pos.x = TIMER_POSITION - ((NUM_TIMER - 3) * 50);
			g_aTimer[1].pos.x = TIMER_POSITION - ((NUM_TIMER - 2) * 50);
		}

		//���_���W�̐ݒ�
		pVtx[(nCntTimer * 4) + 0].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - TIMER_SIZE, g_aTimer[nCntTimer].pos.y - TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 1].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + TIMER_SIZE, g_aTimer[nCntTimer].pos.y - TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 2].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - TIMER_SIZE, g_aTimer[nCntTimer].pos.y + TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 3].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + TIMER_SIZE, g_aTimer[nCntTimer].pos.y + TIMER_SIZE, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimer->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTimer = MAX_TIMER - 1; nCntTimer >= 0; nCntTimer--)
	{
		// �e�N�X�`���̐ݒ�
		if (g_aTimer[nCntTimer].nType >= 0)
		{// �e�N�X�`���ԍ��ʂ�ɂ���
			pDevice->SetTexture(0, g_pTextureTimer[g_aTimer[nCntTimer].nType]);
		}
		else
		{// nType���}�C�i�X�̒l�̓e�N�X�`���Ȃ�
			pDevice->SetTexture(0, NULL);
		}
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTimer, 2);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntTimer;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < NUM_TIMER; nCntTimer++)
	{
		// ���_���W�̐ݒ�
		pVtx[(nCntTimer * 4) + 0].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - TIMER_SIZE, g_aTimer[nCntTimer].pos.y - TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 1].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + TIMER_SIZE, g_aTimer[nCntTimer].pos.y - TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 2].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - TIMER_SIZE, g_aTimer[nCntTimer].pos.y + TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 3].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + TIMER_SIZE, g_aTimer[nCntTimer].pos.y + TIMER_SIZE, 0.0f);

		// 1.0f�ŌŒ�
		pVtx[(nCntTimer * 4) + 0].rhw = 1.0f;
		pVtx[(nCntTimer * 4) + 1].rhw = 1.0f;
		pVtx[(nCntTimer * 4) + 2].rhw = 1.0f;
		pVtx[(nCntTimer * 4) + 3].rhw = 1.0f;

		// ���_�J���[
		pVtx[(nCntTimer * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W
		pVtx[(nCntTimer * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntTimer * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntTimer * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//----------------------------------------------------------------
	/*							�A�C�R��							*/
	//----------------------------------------------------------------
	// ���_���W�̐ݒ�
	pVtx[(NUM_TIMER * 4) + 0].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x - TIMER_SIZE, g_aTimer[NUM_TIMER].pos.y - TIMER_SIZE, 0.0f);
	pVtx[(NUM_TIMER * 4) + 1].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x + TIMER_SIZE, g_aTimer[NUM_TIMER].pos.y - TIMER_SIZE, 0.0f);
	pVtx[(NUM_TIMER * 4) + 2].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x - TIMER_SIZE, g_aTimer[NUM_TIMER].pos.y + TIMER_SIZE, 0.0f);
	pVtx[(NUM_TIMER * 4) + 3].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x + TIMER_SIZE, g_aTimer[NUM_TIMER].pos.y + TIMER_SIZE, 0.0f);

	// 1.0f�ŌŒ�
	pVtx[(NUM_TIMER * 4) + 0].rhw = 1.0f;
	pVtx[(NUM_TIMER * 4) + 1].rhw = 1.0f;
	pVtx[(NUM_TIMER * 4) + 2].rhw = 1.0f;
	pVtx[(NUM_TIMER * 4) + 3].rhw = 1.0f;

	// ���_�J���[
	pVtx[(NUM_TIMER * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_TIMER * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_TIMER * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_TIMER * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	pVtx[(NUM_TIMER * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[(NUM_TIMER * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[(NUM_TIMER * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[(NUM_TIMER * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//----------------------------------------------------------------
	/*							  ������							*/
	//----------------------------------------------------------------
	// ���_���W�̐ݒ�
	pVtx[((NUM_TIMER + 1) * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 2].pos = D3DXVECTOR3(0.0f, FRAME_SIZE_Y, 0.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 3].pos = D3DXVECTOR3(SCREEN_WIDTH, FRAME_SIZE_Y, 0.0f);

	// 1.0f�ŌŒ�
	pVtx[((NUM_TIMER + 1) * 4) + 0].rhw = 1.0f;
	pVtx[((NUM_TIMER + 1) * 4) + 1].rhw = 1.0f;
	pVtx[((NUM_TIMER + 1) * 4) + 2].rhw = 1.0f;
	pVtx[((NUM_TIMER + 1) * 4) + 3].rhw = 1.0f;

	// ���_�J���[
	pVtx[((NUM_TIMER + 1) * 4) + 0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[((NUM_TIMER + 1) * 4) + 1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[((NUM_TIMER + 1) * 4) + 2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[((NUM_TIMER + 1) * 4) + 3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

	// �e�N�X�`�����W
	pVtx[((NUM_TIMER + 1) * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimer->Unlock();
}

//=============================================================================
// �^�C���̎擾
//=============================================================================
int *GetTimer(void)
{
	return &g_nTimer;
}

//=============================================================================
// �������Ԃ�߂�
//=============================================================================
void ResetTimer(void)
{
	g_nTimer = TIMER_TIME;
}