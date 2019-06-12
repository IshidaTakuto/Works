//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "ranking.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "meshField.h"
#include "joypad.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RANKING			"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RANKING_LOGO	"data/TEXTURE/ranking_logo.png"		// �ǂݍ��ރe�N�X�`���t�@�C����

#define RANKING_TIME	(300)	// �^�C�g����ʂ܂ł̎���

#define MAX_TEXTURE		(2)		// �e�N�X�`����
#define RANKING_DIGIT	(7)		// ����
#define MAX_RANKING		(5)		// �����L���O��

#define RANKING_X (60.0f)		//�����̑傫��_X
#define RANKING_Y (80.0f)		//�����̑傫��_Y
#define RANK_CHARACTER_X	(250)
#define RANK_CHARACTER_Y	(100)

#define RANKING_POS			(D3DXVECTOR3(SCREEN_WIDTH / 2 + (RANKING_X * (RANKING_DIGIT / 2.0f)) - RANKING_X / 2, 125.0f + RANKING_Y, 0.0f))	// �����L���O�̈ʒu
#define RANKING_LOGO_POS	(D3DXVECTOR3(SCREEN_WIDTH / 2, 80.0f , 0.0f))										// ���S�̈ʒu

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRanking[MAX_TEXTURE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
static int				g_nRanking[MAX_RANKING] = { 7000, 6000, 5000, 0000, 0000 };	// �X�R�A
D3DXCOLOR				g_nScoreCol[MAX_RANKING + 1];								// ���̃X�R�A�̐F
int g_nYourScore;

bool bScore;
int nCntHighScore;
int g_nCntFade;		// �t�F�[�h����܂ł̎���

//=============================================================================
// ����������
//=============================================================================
void InitRanking(void)
{
	InitMeshField();

	g_nCntFade = 0;
	nCntHighScore = 0;
	for (int nCnt = 0; nCnt < MAX_RANKING + 1; nCnt++)
	{
		g_nScoreCol[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//bScore = false;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING, &g_pTextureRanking[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING_LOGO, &g_pTextureRanking[1]);

	// ���_���̍쐬
	MakeVertexRanking(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitRanking(void)
{
	UninitMeshField();

	for (int nCntRankingTex = 0; nCntRankingTex < MAX_TEXTURE; nCntRankingTex++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureRanking[nCntRankingTex] != NULL)
		{
			g_pTextureRanking[nCntRankingTex]->Release();
			g_pTextureRanking[nCntRankingTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
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
	UpdateMeshField();

	XINPUT_STATE state;
	state = GetJoyPad();

	// ���_���̍쐬
	VERTEX_2D *pVtx;
	FADE pFade;
	pFade = *GetFade();

	// ���_���̍쐬
	//VERTEX_2D *pVtx;

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_DECIDE);
			//���[�h�I��
			SetFade(MODE_TITLE);
			bScore = false;
		}
		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			PlaySound(SOUND_LABEL_DECIDE);
			//���[�h�I��
			SetFade(MODE_TITLE);
			bScore = false;
		}

	}
	g_nCntFade++;
	if (g_nCntFade >= RANKING_TIME)
	{
		if (pFade == FADE_NONE)
		{
			//���[�h�I��
			SetFade(MODE_TITLE);
			bScore = false;
		}
	}

	nCntHighScore++;
	nCntHighScore = nCntHighScore % 4;

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
	if (bScore == true)
	{
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			if (g_nRanking[nCntRanking] == g_nYourScore)
			{
				for (int nCnt = 0; nCnt < RANKING_DIGIT; nCnt++, pVtx += 4)
				{
					if (nCntHighScore < 2)
					{
						g_nScoreCol[nCntRanking] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					}
					else if (nCntHighScore >= 2)
					{
						g_nScoreCol[nCntRanking] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
					}

					// ���_�J���[�̐ݒ�
					pVtx[0].col = g_nScoreCol[nCntRanking];
					pVtx[1].col = g_nScoreCol[nCntRanking];
					pVtx[2].col = g_nScoreCol[nCntRanking];
					pVtx[3].col = g_nScoreCol[nCntRanking];
				}
				break;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRanking(void)
{
	DrawMeshField();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �����L���O����--------------------------------------------------------
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRanking[1]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (RANKING_DIGIT * MAX_RANKING), 2);

	// �����L���O����--------------------------------------------------------
	// �e�N�X�`���̐ݒ�
	for (int nCntRanking = 0; nCntRanking < (RANKING_DIGIT * MAX_RANKING); nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRanking[0]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RANKING_DIGIT * (MAX_RANKING + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// �����L���O�̃X�R�A(����)
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntDigit = 0; nCntDigit < RANKING_DIGIT; nCntDigit++)
		{
			pVtx[0].pos = D3DXVECTOR3(-(RANKING_X / 2) - (RANKING_X * nCntDigit), -(RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;
			pVtx[1].pos = D3DXVECTOR3((RANKING_X / 2) - (RANKING_X * nCntDigit), -(RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;
			pVtx[2].pos = D3DXVECTOR3(-(RANKING_X / 2) - (RANKING_X * nCntDigit), (RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;
			pVtx[3].pos = D3DXVECTOR3((RANKING_X / 2) - (RANKING_X * nCntDigit), (RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;

			//1.0f�ŌŒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			if (nCntRanking == 0)
			{
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else if (nCntRanking == 1)
			{
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			}
			else if (nCntRanking == 2)
			{
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
			}
			else
			{
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			int nData = 1;

			for (int nCntData = nCntDigit; nCntData != 0; nCntData--)
			{
				nData *= 10;
			}

			pVtx[0].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);

			pVtx += 4;
		}
	}

	// �����L���O�̕���-------------------------------------------------------------------------------------------											    	   
	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(-RANK_CHARACTER_X / 2, -RANK_CHARACTER_Y / 2, 0.0f) + RANKING_LOGO_POS;
	pVtx[1].pos = D3DXVECTOR3(RANK_CHARACTER_X / 2, -RANK_CHARACTER_Y / 2, 0.0f) + RANKING_LOGO_POS;
	pVtx[2].pos = D3DXVECTOR3(-RANK_CHARACTER_X / 2, RANK_CHARACTER_Y / 2, 0.0f) + RANKING_LOGO_POS;
	pVtx[3].pos = D3DXVECTOR3(RANK_CHARACTER_X / 2, RANK_CHARACTER_Y / 2, 0.0f) + RANKING_LOGO_POS;

	// rhw�̐ݒ�
	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
	{
		pVtx[nCntrhw].rhw = 1.0f;
	}

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
}


//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetRanking(int nScore)
{
	int nRank;
	int aEmptyData[MAX_RANKING + 1];
	aEmptyData[MAX_RANKING] = nScore;

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{

		aEmptyData[nCntNum] = g_nRanking[nCntNum];

	}

	for (int nCntNum = 1; nCntNum < MAX_RANKING + 1; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aEmptyData[nCntNum1 - 1] <= aEmptyData[nCntNum1])
			{
				nRank = aEmptyData[nCntNum1];
				aEmptyData[nCntNum1] = aEmptyData[nCntNum1 - 1];
				aEmptyData[nCntNum1 - 1] = nRank;
			}
		}
	}

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{
		g_nRanking[nCntNum] = aEmptyData[nCntNum];

	}
	bScore = true;

	g_nYourScore = nScore;
}
