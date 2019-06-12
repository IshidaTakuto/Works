//=============================================================================
//
// �L�����O���� [killLog.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "killLog.h"
#include "particle2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_KILLLOG_00	"data/TEXTURE/number.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_KILLLOG_01	"data/TEXTURE/game_kill.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define NUM_KILLLOG			(3)								// �X�R�A�̍ő包��
#define SIZE_KILLLOG		(25)							// �X�R�A�̑傫��
#define MAX_TEX				(2)								// �e�N�X�`���̐�
#define MAX_SIZE			(40)							// �傫���̍ő�l
#define SIZE_LARGE			(20)							// �ǂꂭ�炢�傫�����邩
#define SIZE_SMALL			(0.1f)							// ����������l
#define MAX_KILLLOG_POLYGON	(NUM_KILLLOG + 1)
#define KILLLOG_POSITION	(275)
#define KILLING_VOID_SIZE	(50.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexKillLog(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureKillLog[MAX_TEX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffKillLog = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
KILLLOG					g_aKillLog[MAX_KILLLOG_POLYGON];		// �L�����������ƂɊi�[
int g_nNumKill;													// �L����
int g_aNumLog[NUM_KILLLOG];										// �����Ƃ̃L������ۑ�
D3DXVECTOR3 g_aDiffSize[NUM_KILLLOG][4];						// ���̃T�C�Y

//=============================================================================
// ����������
//=============================================================================
void InitKillLog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntScore = 0; nCntScore < NUM_KILLLOG; nCntScore++)
	{
		// �X�R�A�̏�����
		g_aKillLog[nCntScore].pos = D3DXVECTOR3(KILLLOG_POSITION - (nCntScore * KILLING_VOID_SIZE), 50, 0.0f);
		g_aKillLog[nCntScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aKillLog[nCntScore].nType = 0;

		g_aNumLog[nCntScore] = 0;
	}

	// �X�R�A�̏�����
	g_aKillLog[NUM_KILLLOG].pos = D3DXVECTOR3(KILLLOG_POSITION - (NUM_KILLLOG * KILLING_VOID_SIZE) - 20.0f, 50, 0.0f);
	g_aKillLog[NUM_KILLLOG].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aKillLog[NUM_KILLLOG].nType = 1;

	g_nNumKill = 0;		// �L�����̏�����

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_KILLLOG_00, &g_pTextureKillLog[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_KILLLOG_01, &g_pTextureKillLog[1]);

	// ���_���̍쐬
	MakeVertexKillLog(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitKillLog(void)
{
	for (int nCntScore = 0; nCntScore < MAX_TEX; nCntScore++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureKillLog[nCntScore] != NULL)
		{
			g_pTextureKillLog[nCntScore]->Release();
			g_pTextureKillLog[nCntScore] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffKillLog != NULL)
	{
		g_pVtxBuffKillLog->Release();
		g_pVtxBuffKillLog = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateKillLog(void)
{
	D3DXVECTOR3 aDestSize[4];	// �T�C�Y�̍��������߂�̂ɕK�v
	D3DXCOLOR col[4];

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffKillLog->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nNumKill < 10)
	{
		g_aKillLog[0].pos.x = KILLLOG_POSITION - ((NUM_KILLLOG - 1) * KILLING_VOID_SIZE);
	}
	else if (g_nNumKill < 100)
	{
		g_aKillLog[0].pos.x = KILLLOG_POSITION - ((NUM_KILLLOG - 2) * KILLING_VOID_SIZE);
		g_aKillLog[1].pos.x = KILLLOG_POSITION - ((NUM_KILLLOG - 1) * KILLING_VOID_SIZE);
	}
	else
	{
		g_aKillLog[0].pos.x = KILLLOG_POSITION - ((NUM_KILLLOG - 3) * KILLING_VOID_SIZE);
		g_aKillLog[1].pos.x = KILLLOG_POSITION - ((NUM_KILLLOG - 2) * KILLING_VOID_SIZE);
	}

	for (int nCntVtx = 0; nCntVtx < NUM_KILLLOG; nCntVtx++)
	{
		//���_���W�̐ݒ�
		pVtx[(nCntVtx * 4) + 0].pos = D3DXVECTOR3(g_aKillLog[nCntVtx].pos.x - SIZE_KILLLOG, g_aKillLog[nCntVtx].pos.y - SIZE_KILLLOG, 0.0f);
		pVtx[(nCntVtx * 4) + 1].pos = D3DXVECTOR3(g_aKillLog[nCntVtx].pos.x + SIZE_KILLLOG, g_aKillLog[nCntVtx].pos.y - SIZE_KILLLOG, 0.0f);
		pVtx[(nCntVtx * 4) + 2].pos = D3DXVECTOR3(g_aKillLog[nCntVtx].pos.x - SIZE_KILLLOG, g_aKillLog[nCntVtx].pos.y + SIZE_KILLLOG, 0.0f);
		pVtx[(nCntVtx * 4) + 3].pos = D3DXVECTOR3(g_aKillLog[nCntVtx].pos.x + SIZE_KILLLOG, g_aKillLog[nCntVtx].pos.y + SIZE_KILLLOG, 0.0f);

		col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (pVtx[(nCntVtx * 4) + 0].pos.x < g_aDiffSize[nCntVtx][0].x)
		{// �T�C�Y��߂�
			aDestSize[0].x = g_aDiffSize[nCntVtx][0].x - pVtx[(nCntVtx * 4) + 0].pos.x;
			aDestSize[0].y = g_aDiffSize[nCntVtx][0].y - pVtx[(nCntVtx * 4) + 0].pos.y;
			aDestSize[1].x = g_aDiffSize[nCntVtx][1].x - pVtx[(nCntVtx * 4) + 1].pos.x;
			aDestSize[1].y = g_aDiffSize[nCntVtx][1].y - pVtx[(nCntVtx * 4) + 1].pos.y;
			aDestSize[2].x = g_aDiffSize[nCntVtx][2].x - pVtx[(nCntVtx * 4) + 2].pos.x;
			aDestSize[2].y = g_aDiffSize[nCntVtx][2].y - pVtx[(nCntVtx * 4) + 2].pos.y;
			aDestSize[3].x = g_aDiffSize[nCntVtx][3].x - pVtx[(nCntVtx * 4) + 3].pos.x;
			aDestSize[3].y = g_aDiffSize[nCntVtx][3].y - pVtx[(nCntVtx * 4) + 3].pos.y;

			//���_���W�̐ݒ�
			pVtx[(nCntVtx * 4) + 0].pos.x += aDestSize[0].x * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 0].pos.y += aDestSize[0].y * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 1].pos.x += aDestSize[1].x * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 1].pos.y += aDestSize[1].y * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 2].pos.x += aDestSize[2].x * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 2].pos.y += aDestSize[2].y * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 3].pos.x += aDestSize[3].x * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 3].pos.y += aDestSize[3].y * SIZE_SMALL;

			col[0] -= pVtx[(nCntVtx * 4) + 0].col;
			col[1] -= pVtx[(nCntVtx * 4) + 1].col;
			col[2] -= pVtx[(nCntVtx * 4) + 2].col;
			col[3] -= pVtx[(nCntVtx * 4) + 3].col;

			//���_�J���[
			pVtx[(nCntVtx * 4) + 0].col += col[0] * 0.02f;
			pVtx[(nCntVtx * 4) + 1].col += col[1] * 0.02f;
			pVtx[(nCntVtx * 4) + 2].col += col[2] * 0.02f;
			pVtx[(nCntVtx * 4) + 3].col += col[3] * 0.02f;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffKillLog->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawKillLog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffKillLog, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < MAX_KILLLOG_POLYGON; nCntScore++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureKillLog[g_aKillLog[nCntScore].nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void AddKill(void)
{
	/***********************************************************
	*
	*	�G��|�����Ƃ��ɌĂ�ł�������
	*
	***********************************************************/

	int nCntScore;
	int aNumber[NUM_KILLLOG];	// �����Ƃ̃L����

	g_nNumKill++;	// �L�����𑝂₷

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffKillLog->Lock(0, 0, (void**)&pVtx, 0);

	aNumber[0] = g_nNumKill % 10;				// �P����
	aNumber[1] = g_nNumKill % 100 / 10;			// �Q����
	aNumber[2] = g_nNumKill % 1000 / 100;		// �R����

	for (nCntScore = 0; nCntScore < NUM_KILLLOG; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 1.0f);

		if (g_nNumKill < 100)
		{// 100��菬�����Ɖ��F
			//���_�J���[
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{// 100�ȏ�ŃJ���t��
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(rand() % 11 * 0.1f, rand() % 11 * 0.1f, rand() % 11 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(rand() % 11 * 0.1f, rand() % 11 * 0.1f, rand() % 11 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(rand() % 11 * 0.1f, rand() % 11 * 0.1f, rand() % 11 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(rand() % 11 * 0.1f, rand() % 11 * 0.1f, rand() % 11 * 0.1f, 1.0f);

			if ((g_aKillLog[nCntScore].pos.x - SIZE_KILLLOG) - (MAX_SIZE * 1.1f) < pVtx[(nCntScore * 4) + 0].pos.x)
			{// �傫���̌��x
			 //���_���W�̐ݒ�
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE * 1.1f;
			}
		}

		if (g_aNumLog[nCntScore] != aNumber[nCntScore])
		{
			if ((g_aKillLog[nCntScore].pos.x - SIZE_KILLLOG) - MAX_SIZE < pVtx[(nCntScore * 4) + 0].pos.x)
			{// �傫���̌��x
				//���_���W�̐ݒ�
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE;
			}
		}

		g_aNumLog[nCntScore] = aNumber[nCntScore];	// �V�����f�[�^��ۑ�
	}

	if (g_nNumKill < 10)
	{// �X�ȉ�����2,3���ڂ𓧖���
		//���_�J���[
		pVtx[4].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[5].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[6].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[7].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pVtx[8].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[9].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[10].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[11].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
	}
	else if (g_nNumKill < 100)
	{// �X�X�ȉ�����3���ڂ𓧖���
		pVtx[8].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[9].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[10].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[11].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffKillLog->Unlock();

	// �p�[�e�B�N��������
	if (g_nNumKill < 10)
	{// �X�ȉ�
		SetParticle2D(g_aKillLog[0].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
	}
	else if (g_nNumKill < 100)
	{// �X�X�ȉ�
		SetParticle2D(g_aKillLog[0].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
		SetParticle2D(g_aKillLog[1].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
	}
	else
	{
		SetParticle2D(g_aKillLog[0].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
		SetParticle2D(g_aKillLog[1].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
		SetParticle2D(g_aKillLog[2].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexKillLog(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntScore;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_KILLLOG_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffKillLog,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffKillLog->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_KILLLOG; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[(nCntScore * 4) + 0].pos = D3DXVECTOR3(g_aKillLog[nCntScore].pos.x - SIZE_KILLLOG, g_aKillLog[nCntScore].pos.y - SIZE_KILLLOG, 0.0f);
		pVtx[(nCntScore * 4) + 1].pos = D3DXVECTOR3(g_aKillLog[nCntScore].pos.x + SIZE_KILLLOG, g_aKillLog[nCntScore].pos.y - SIZE_KILLLOG, 0.0f);
		pVtx[(nCntScore * 4) + 2].pos = D3DXVECTOR3(g_aKillLog[nCntScore].pos.x - SIZE_KILLLOG, g_aKillLog[nCntScore].pos.y + SIZE_KILLLOG, 0.0f);
		pVtx[(nCntScore * 4) + 3].pos = D3DXVECTOR3(g_aKillLog[nCntScore].pos.x + SIZE_KILLLOG, g_aKillLog[nCntScore].pos.y + SIZE_KILLLOG, 0.0f);

		// �T�C�Y�̋L��
		g_aDiffSize[nCntScore][0] = pVtx[(nCntScore * 4) + 0].pos;
		g_aDiffSize[nCntScore][1] = pVtx[(nCntScore * 4) + 1].pos;
		g_aDiffSize[nCntScore][2] = pVtx[(nCntScore * 4) + 2].pos;
		g_aDiffSize[nCntScore][3] = pVtx[(nCntScore * 4) + 3].pos;

		//1.0f�ŌŒ�
		pVtx[(nCntScore * 4) + 0].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 1].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 2].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 3].rhw = 1.0f;

		if (nCntScore == 0)
		{// �P���ڂ͍ŏ�����o��
			//���_�J���[
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{// �Q���ڈȍ~�͉B���Ă���
			//���_�J���[
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}

		//�e�N�X�`�����W
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//���_���W�̐ݒ�
	pVtx[(NUM_KILLLOG * 4) + 0].pos = D3DXVECTOR3(g_aKillLog[NUM_KILLLOG].pos.x - SIZE_KILLLOG, g_aKillLog[NUM_KILLLOG].pos.y - SIZE_KILLLOG, 0.0f);
	pVtx[(NUM_KILLLOG * 4) + 1].pos = D3DXVECTOR3(g_aKillLog[NUM_KILLLOG].pos.x + SIZE_KILLLOG, g_aKillLog[NUM_KILLLOG].pos.y - SIZE_KILLLOG, 0.0f);
	pVtx[(NUM_KILLLOG * 4) + 2].pos = D3DXVECTOR3(g_aKillLog[NUM_KILLLOG].pos.x - SIZE_KILLLOG, g_aKillLog[NUM_KILLLOG].pos.y + SIZE_KILLLOG, 0.0f);
	pVtx[(NUM_KILLLOG * 4) + 3].pos = D3DXVECTOR3(g_aKillLog[NUM_KILLLOG].pos.x + SIZE_KILLLOG, g_aKillLog[NUM_KILLLOG].pos.y + SIZE_KILLLOG, 0.0f);

	//1.0f�ŌŒ�
	pVtx[(NUM_KILLLOG * 4) + 0].rhw = 1.0f;
	pVtx[(NUM_KILLLOG * 4) + 1].rhw = 1.0f;
	pVtx[(NUM_KILLLOG * 4) + 2].rhw = 1.0f;
	pVtx[(NUM_KILLLOG * 4) + 3].rhw = 1.0f;

	//���_�J���[
	pVtx[(NUM_KILLLOG * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_KILLLOG * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_KILLLOG * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_KILLLOG * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[(NUM_KILLLOG * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[(NUM_KILLLOG * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[(NUM_KILLLOG * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[(NUM_KILLLOG * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffKillLog->Unlock();
}
