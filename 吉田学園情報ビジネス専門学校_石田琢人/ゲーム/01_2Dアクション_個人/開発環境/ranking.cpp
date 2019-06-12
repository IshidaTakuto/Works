//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "ranking.h"
#include "score.h"
#include "bg.h"
#include "block.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RANKING0	"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RANKING1	"data/TEXTURE/ranking_logo.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RANKING2	"data/TEXTURE/ranking_logo001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define NUM_RANKING	(14)	// �����L���O�̐�
#define MAX_RANKING	(NUM_RANKING + 2)
#define RANKING_SIZE	(50)
#define MAX_TEXTURE		(3)
#define RANKING_TIME	(600)
#define LOGO_SIZE_X		(250)
#define LOGO_SIZE_Y		(50)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRanking[MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
RANKING					g_aRankScore[MAX_RANKING];				// �����L���O
int g_aNumber[NUM_RANKING];
int g_nCntRanking;
int g_nRankScore = 0;

//=============================================================================
// ����������
//=============================================================================
void InitRanking(void)
{
	// �w�i����������
	InitBg();

	// �u���b�N����������
	InitBlock();

	int nCntRanking;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntRanking = 0; nCntRanking < NUM_RANKING / 2; nCntRanking++)
	{
		// �����L���O�̏�����
		g_aRankScore[nCntRanking].nScore = 0;
		g_aRankScore[nCntRanking].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		g_aRankScore[nCntRanking].pos = D3DXVECTOR3((SCREEN_WIDTH / 4 + SCREEN_WIDTH / 2) - (nCntRanking * RANKING_SIZE * 2.0f), SCREEN_HEIGHT / 2 - 50, 0.0f);
		g_aRankScore[nCntRanking].nType = 0;
	}
	for (; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		g_aRankScore[nCntRanking].nScore = 0;
		g_aRankScore[nCntRanking].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		g_aRankScore[nCntRanking].pos = D3DXVECTOR3((SCREEN_WIDTH / 4 + SCREEN_WIDTH / 2) - ((nCntRanking % (NUM_RANKING / 2)) * RANKING_SIZE * 2.0f), SCREEN_HEIGHT / 2 + 250, 0.0f);
		g_aRankScore[nCntRanking].nType = 0;
	}

	g_aRankScore[NUM_RANKING].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5, 0.0f);
	g_aRankScore[NUM_RANKING].nType = 1;

	g_aRankScore[NUM_RANKING + 1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 75, 0.0f);
	g_aRankScore[NUM_RANKING + 1].nType = 2;

	g_nCntRanking = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING0, &g_pTextureRanking[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING1, &g_pTextureRanking[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING2, &g_pTextureRanking[2]);

	// ���_���̍쐬
	MakeVertexRanking(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitRanking(void)
{
	// �w�i�I������
	UninitBg();

	// �u���b�N�I������
	UninitBlock();

	int nCntRanking;

	for (nCntRanking = 0; nCntRanking < MAX_TEXTURE; nCntRanking++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureRanking[nCntRanking] != NULL)
		{
			g_pTextureRanking[nCntRanking]->Release();
			g_pTextureRanking[nCntRanking] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateRanking(void)
{
	// �w�i�X�V����
	UpdateBg();

	// �u���b�N�X�V����
	UpdateBlock();

	FADE pFade;

	g_nCntRanking++;

	// �t�F�[�h�̎擾
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			// ���[�h�I��
			SetFade(MODE_TITLE);
		}
		else if (g_nCntRanking >= RANKING_TIME)
		{
			StopSound(SOUND_LABEL_TITLE000);
			//���[�h�I��
			SetFade(MODE_TITLE);
		}
	}

	if (rand() % 160 == 0)
	{// �����_���ɒ����o��
		SetBlock(D3DXVECTOR3(-50, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}
	else if (rand() % 120 == 0)
	{// �����_���ɒ����o��
		SetBlock(D3DXVECTOR3(50 + SCREEN_WIDTH, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}

	int nScore;
	nScore = GetScore();

	if (g_nRankScore < nScore)
	{// �n�C�X�R�A�X�V
		g_nRankScore = nScore;
	}

	int nCntRanking;

	g_aNumber[0] = g_nRankScore % 10;
	g_aNumber[1] = g_nRankScore % 100 / 10;
	g_aNumber[2] = g_nRankScore % 1000 / 100;
	g_aNumber[3] = g_nRankScore % 10000 / 1000;
	g_aNumber[4] = g_nRankScore % 100000 / 10000;
	g_aNumber[5] = g_nRankScore % 1000000 / 100000;
	g_aNumber[6] = g_nRankScore % 10000000 / 1000000;

	g_aNumber[7] = nScore % 10;
	g_aNumber[8] = nScore % 100 / 10;
	g_aNumber[9] = nScore % 1000 / 100;
	g_aNumber[10] = nScore % 10000 / 1000;
	g_aNumber[11] = nScore % 100000 / 10000;
	g_aNumber[12] = nScore % 1000000 / 100000;
	g_aNumber[13] = nScore % 10000000 / 1000000;

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntRanking] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + g_aNumber[nCntRanking] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntRanking] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + g_aNumber[nCntRanking] * 0.1f, 1.0f);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - RANKING_SIZE, g_aRankScore[nCntRanking].pos.y - RANKING_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + RANKING_SIZE, g_aRankScore[nCntRanking].pos.y - RANKING_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - RANKING_SIZE, g_aRankScore[nCntRanking].pos.y + RANKING_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + RANKING_SIZE, g_aRankScore[nCntRanking].pos.y + RANKING_SIZE, 0.0f);

		// ���_�J���[
		pVtx[0].col = g_aRankScore[nCntRanking].col;
		pVtx[1].col = g_aRankScore[nCntRanking].col;
		pVtx[2].col = g_aRankScore[nCntRanking].col;
		pVtx[3].col = g_aRankScore[nCntRanking].col;

		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRanking(void)
{
	// �w�i�`�揈��
	DrawBg();

	// �u���b�N�`�揈��
	DrawBlock();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRanking[g_aRankScore[nCntRanking].nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntRanking;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - RANKING_SIZE, g_aRankScore[nCntRanking].pos.y - RANKING_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + RANKING_SIZE, g_aRankScore[nCntRanking].pos.y - RANKING_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - RANKING_SIZE, g_aRankScore[nCntRanking].pos.y + RANKING_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + RANKING_SIZE, g_aRankScore[nCntRanking].pos.y + RANKING_SIZE, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}

	for (; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - LOGO_SIZE_X, g_aRankScore[nCntRanking].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + LOGO_SIZE_X, g_aRankScore[nCntRanking].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - LOGO_SIZE_X, g_aRankScore[nCntRanking].pos.y + LOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + LOGO_SIZE_X, g_aRankScore[nCntRanking].pos.y + LOGO_SIZE_Y, 0.0f);

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

		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}
