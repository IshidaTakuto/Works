//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "ranking.h"
#include "score.h"
#include "bg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RANKING		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RANKING1	"data/TEXTURE/high_score.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_RANKING	(8)	//�����L���O�̍ő包��
#define RANKING_SIZE	(50)
#define MAX_TEXTURE		(2)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexHighscore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRanking[MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighscore = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
int						g_nRankScore;							// �����L���O
int g_aNumber[8];
D3DXVECTOR3 g_posRanking;

//=============================================================================
// ����������
//=============================================================================
void InitRanking(void)
{
	//�w�i����������
	InitBg();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posRanking = D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 0.0f);

	// �����L���O�̏�����
	g_nRankScore = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING, &g_pTextureRanking[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING1, &g_pTextureRanking[1]);

	// ���_���̍쐬
	MakeVertexRanking(pDevice);

	// ���_���̍쐬
	MakeVertexHighscore(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitRanking(void)
{
	//�w�i�I������
	UninitBg();

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

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHighscore != NULL)
	{
		g_pVtxBuffHighscore->Release();
		g_pVtxBuffHighscore = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateRanking(void)
{
	//�w�i�X�V����
	UpdateBg();

	FADE pFade;

	//�t�F�[�h�̎擾
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			//���[�h�I��
			SetFade(MODE_TITLE);
		}
	}
	int nCntRanking;

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntRanking] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + g_aNumber[nCntRanking] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntRanking] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + g_aNumber[nCntRanking] * 0.1f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRanking(void)
{
	//�w�i�`�揈��
	DrawBg();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRanking[0]);

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);
	}

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHighscore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRanking[1]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
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

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posRanking.x - RANKING_SIZE, g_posRanking.y - RANKING_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posRanking.x + RANKING_SIZE, g_posRanking.y - RANKING_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posRanking.x - RANKING_SIZE, g_posRanking.y + RANKING_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posRanking.x + RANKING_SIZE, g_posRanking.y + RANKING_SIZE, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
		g_posRanking.x += 80;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexHighscore(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighscore,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHighscore->Lock(0, 0, (void**)&pVtx, 0);

	////���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHighscore->Unlock();
}

//=============================================================================
// �����L���O�̐ݒ�
//=============================================================================
void SetRankScore(int nScore)
{
	if (nScore > g_nRankScore)
	{// �n�C�X�R�A�X�V
		g_nRankScore = nScore;

		g_aNumber[0] = g_nRankScore / 10000000;
		g_aNumber[1] = g_nRankScore % 10000000 / 1000000;
		g_aNumber[2] = g_nRankScore % 1000000 / 100000;
		g_aNumber[3] = g_nRankScore % 100000 / 10000;
		g_aNumber[4] = g_nRankScore % 10000 / 1000;
		g_aNumber[5] = g_nRankScore % 1000 / 100;
		g_aNumber[6] = g_nRankScore % 100 / 10;
		g_aNumber[7] = g_nRankScore % 10;

	}
}
