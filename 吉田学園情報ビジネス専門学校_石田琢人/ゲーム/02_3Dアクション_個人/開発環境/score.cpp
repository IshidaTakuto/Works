//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "score.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SCORE0		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_SCORE1		"data/TEXTURE/frame_score.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define NUM_SCORE	(7)		// �X�R�A�̍ő包��
#define SCORE_SIZE	(25)	// �X�R�A�̑傫��
#define MAX_TEX		(2)
#define MAX_SCORE	(NUM_SCORE + 1)
#define FRAME_SIZE_X	(370)
#define FRAME_SIZE_Y	(150)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureScore[MAX_TEX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
SCORE					g_aScore[MAX_SCORE];					// �X�R�A
int g_nScore;

//=============================================================================
// ����������
//=============================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		// �X�R�A�̏�����
		g_aScore[nCntScore].pos = D3DXVECTOR3(1190 - (nCntScore * 50.0f), 80, 0.0f);
		g_aScore[nCntScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aScore[nCntScore].nSize = SCORE_SIZE;
		g_aScore[nCntScore].nType = 0;
	}

	g_aScore[NUM_SCORE].pos = D3DXVECTOR3(1045, 100, 0.0f);
	g_aScore[NUM_SCORE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aScore[NUM_SCORE].nSize = 150;
	g_aScore[NUM_SCORE].nType = 1;

	g_nScore = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SCORE0, &g_pTextureScore[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SCORE1, &g_pTextureScore[1]);

	// ���_���̍쐬
	MakeVertexScore(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	for (int nCntScore = 0; nCntScore < MAX_TEX; nCntScore++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureScore[nCntScore] != NULL)
		{
			g_pTextureScore[nCntScore]->Release();
			g_pTextureScore[nCntScore] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore[g_aScore[nCntScore].nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntScore;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x - SCORE_SIZE, g_aScore[nCntScore].pos.y - SCORE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x + SCORE_SIZE, g_aScore[nCntScore].pos.y - SCORE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x - SCORE_SIZE, g_aScore[nCntScore].pos.y + SCORE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x + SCORE_SIZE, g_aScore[nCntScore].pos.y + SCORE_SIZE, 0.0f);

		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aScore[NUM_SCORE].pos.x - FRAME_SIZE_X, g_aScore[NUM_SCORE].pos.y - FRAME_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aScore[NUM_SCORE].pos.x + FRAME_SIZE_X, g_aScore[NUM_SCORE].pos.y - FRAME_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aScore[NUM_SCORE].pos.x - FRAME_SIZE_X, g_aScore[NUM_SCORE].pos.y + FRAME_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aScore[NUM_SCORE].pos.x + FRAME_SIZE_X, g_aScore[NUM_SCORE].pos.y + FRAME_SIZE_Y, 0.0f);

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
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void AddScore(int nValue)
{
	PlaySound(SOUND_LABEL_COIN);

	int nCntScore;
	int aNumber[NUM_SCORE];

	g_nScore += nValue;

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	aNumber[0] = g_nScore % 10;
	aNumber[1] = g_nScore % 100 / 10;
	aNumber[2] = g_nScore % 1000 / 100;
	aNumber[3] = g_nScore % 10000 / 1000;
	aNumber[4] = g_nScore % 100000 / 10000;
	aNumber[5] = g_nScore % 1000000 / 100000;
	aNumber[6] = g_nScore % 10000000 / 1000000;

	for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 1.0f);

		if (g_nScore <= 0)
		{// 0�ȉ��ɂȂ����Ƃ�
			//���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{// �ʏ펞
			//���_�J���[
			pVtx[0].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
		}

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int GetScore(void)
{
	return g_nScore;
}