//=============================================================================
//
// �R���{���� [combo.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "combo.h"
#include "score.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_COMBO_00 	"data/TEXTURE/number.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_COMBO_01	"data/TEXTURE/game_combo.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define NUM_COMBO				(3)							// �R���{�̍ő包��
#define SIZE_COMBO				(50)						// �R���{�̑傫��
#define MAX_SIZE_COMBO			(40)						// �傫���̍ő�l
#define MAX_TEX_COMBO			(2)							// �e�N�X�`���̐�
#define SIZE_LARGE_COMBO		(20)						// �ǂꂭ�炢�傫�����邩
#define SIZE_SMALL_COMBO		(0.1f)						// �����ɂ�����l
#define TRANSPARENT_TIME_COMBO	(120)						// ����������܂ł̎���
#define MAX_COMBO_POLYGON		(NUM_COMBO + 1)				// �\������|���S����
#define LOGO_SIZE_COMBO			(60)						// �R���{�̕����̑傫��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexCombo(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureCombo[MAX_TEX_COMBO] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
Combo					g_aCombo[MAX_COMBO_POLYGON];			// �R���{���������ƂɊi�[
int g_nNumCombo;												// �R���{��
int g_aNumLogCombo[NUM_COMBO];									// �����Ƃ̃R���{����ۑ�
D3DXVECTOR3 g_aDiffSizeComco[NUM_COMBO][4];						// ���̃T�C�Y
int g_nCntComboTime;											// ������������܂ł̃J�E���^
int g_nCntKillCombo;

//=============================================================================
// ����������
//=============================================================================
void InitCombo(D3DXVECTOR3 pos)
{
	/***********************************************************
	*
	*	Init���ĂԂƂ��ɕ\��������W�������ɂ���Ă�������
	*
	************************************************************/

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntScore = 0; nCntScore < NUM_COMBO; nCntScore++)
	{
		// �R���{�̏�����
		g_aCombo[nCntScore].pos = D3DXVECTOR3(pos.x - (nCntScore * 70.0f), pos.y, 0.0f);
		g_aCombo[nCntScore].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_aCombo[nCntScore].nType = 0;

		g_aNumLogCombo[nCntScore] = 0;
	}

	g_aCombo[NUM_COMBO].pos = D3DXVECTOR3(pos.x - 70.0f, pos.y - 60.0f, 0.0f);
	g_aCombo[NUM_COMBO].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_aCombo[NUM_COMBO].nType = 1;

	g_nCntComboTime = 0;	// �R���{������������܂ł̎���
	g_nNumCombo = 0;			// �R���{���̏�����
	g_nCntKillCombo = 0;	// �L���R���{�̏�����

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_COMBO_00, &g_pTextureCombo[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_COMBO_01, &g_pTextureCombo[1]);

	// ���_���̍쐬
	MakeVertexCombo(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitCombo(void)
{
	for (int nCntScore = 0; nCntScore < MAX_TEX_COMBO; nCntScore++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureCombo[nCntScore] != NULL)
		{
			g_pTextureCombo[nCntScore]->Release();
			g_pTextureCombo[nCntScore] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCombo != NULL)
	{
		g_pVtxBuffCombo->Release();
		g_pVtxBuffCombo = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCombo(void)
{
	D3DXVECTOR3 aDestSize[4];	// �T�C�Y�̍��������߂�̂ɕK�v
	D3DXCOLOR col[4];

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nNumCombo > 0)
	{// �R���{�������Ă���Ƃ�
		for (int nCntVtx = 0; nCntVtx < NUM_COMBO; nCntVtx++)
		{
			col[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			col[1] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			col[2] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			col[3] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			if (g_nNumCombo >= pow(10, nCntVtx))
			{// �K�v�Ȍ���������
				if (pVtx[(nCntVtx * 4) + 0].pos.x < g_aDiffSizeComco[nCntVtx][0].x)
				{// �T�C�Y��߂�
					aDestSize[0].x = g_aDiffSizeComco[nCntVtx][0].x - pVtx[(nCntVtx * 4) + 0].pos.x;
					aDestSize[0].y = g_aDiffSizeComco[nCntVtx][0].y - pVtx[(nCntVtx * 4) + 0].pos.y;
					aDestSize[1].x = g_aDiffSizeComco[nCntVtx][1].x - pVtx[(nCntVtx * 4) + 1].pos.x;
					aDestSize[1].y = g_aDiffSizeComco[nCntVtx][1].y - pVtx[(nCntVtx * 4) + 1].pos.y;
					aDestSize[2].x = g_aDiffSizeComco[nCntVtx][2].x - pVtx[(nCntVtx * 4) + 2].pos.x;
					aDestSize[2].y = g_aDiffSizeComco[nCntVtx][2].y - pVtx[(nCntVtx * 4) + 2].pos.y;
					aDestSize[3].x = g_aDiffSizeComco[nCntVtx][3].x - pVtx[(nCntVtx * 4) + 3].pos.x;
					aDestSize[3].y = g_aDiffSizeComco[nCntVtx][3].y - pVtx[(nCntVtx * 4) + 3].pos.y;

					//���_���W�̐ݒ�
					pVtx[(nCntVtx * 4) + 0].pos.x += aDestSize[0].x * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 0].pos.y += aDestSize[0].y * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 1].pos.x += aDestSize[1].x * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 1].pos.y += aDestSize[1].y * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 2].pos.x += aDestSize[2].x * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 2].pos.y += aDestSize[2].y * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 3].pos.x += aDestSize[3].x * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 3].pos.y += aDestSize[3].y * SIZE_SMALL_COMBO;

					col[0] -= pVtx[(nCntVtx * 4) + 0].col;
					col[1] -= pVtx[(nCntVtx * 4) + 1].col;
					col[2] -= pVtx[(nCntVtx * 4) + 2].col;
					col[3] -= pVtx[(nCntVtx * 4) + 3].col;
				}

				//���_�J���[
				pVtx[(nCntVtx * 4) + 0].col += col[0] * 0.02f;
				pVtx[(nCntVtx * 4) + 1].col += col[1] * 0.02f;
				pVtx[(nCntVtx * 4) + 2].col += col[2] * 0.02f;
				pVtx[(nCntVtx * 4) + 3].col += col[3] * 0.02f;

				// ������������
				if (g_nCntComboTime > TRANSPARENT_TIME_COMBO)
				{// ��莞�Ԍo��
						//���_�J���[
					pVtx[(nCntVtx * 4) + 0].col -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.03f);
					pVtx[(nCntVtx * 4) + 1].col -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.03f);
					pVtx[(nCntVtx * 4) + 2].col -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.03f);
					pVtx[(nCntVtx * 4) + 3].col -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.03f);
				}
			}
		}
	}

	D3DXCOLOR vtxColor = pVtx[0].col;

	 //���_�J���[
	pVtx[(NUM_COMBO * 4) + 0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, vtxColor.a);
	pVtx[(NUM_COMBO * 4) + 1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, vtxColor.a);
	pVtx[(NUM_COMBO * 4) + 2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, vtxColor.a);
	pVtx[(NUM_COMBO * 4) + 3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, vtxColor.a);

	if (vtxColor.a <= 0.02f)
	{// �����ɂȂ����Ƃ�
		int nScore;
		float fScore;

		fScore = g_nNumCombo % 100 / 10.0f;
		fScore = fScore * 0.01f + 1.0f;

		// �R���{�ƃL�����ŃX�R�A����o��
		nScore = ((g_nCntKillCombo * 1000) + (g_nNumCombo * 100)) * (int)fScore;

		AddScore(nScore);
		g_nNumCombo = 0;
		g_nCntKillCombo = 0;

		if (nScore > 0)
		{// �X�R�A���f�����ʉ�
			PlaySound(SOUND_LABEL_COMBO);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCombo->Unlock();

	g_nCntComboTime++;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCombo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = NUM_COMBO; nCntScore >= 0; nCntScore--)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureCombo[g_aCombo[nCntScore].nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

//=============================================================================
// �L���R���{�̉��Z
//=============================================================================
void AddKillCombo(void)
{
	g_nCntKillCombo++;
}

//=============================================================================
// �R���{�̉��Z
//=============================================================================
void AddCombo(void)
{
	/***********************************************************
	*
	*	�G��|�����Ƃ��ɌĂ�ł�������
	*
	***********************************************************/

	int nCntScore;
	int aNumber[NUM_COMBO];	// �����Ƃ̃R���{��

	g_nNumCombo++;	// �R���{���𑝂₷

	g_nCntComboTime = 0;	// ����������܂ł̎��Ԃ�0�ɖ߂�

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[
	pVtx[(NUM_COMBO * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_COMBO * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_COMBO * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_COMBO * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	aNumber[0] = g_nNumCombo % 10;				// �P����
	aNumber[1] = g_nNumCombo % 100 / 10;		// �Q����
	aNumber[2] = g_nNumCombo % 1000 / 100;		// �R����

	for (nCntScore = 0; nCntScore < NUM_COMBO; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 1.0f);

		if (g_nNumCombo < 100)
		{// 100��菬����
			//���_�J���[
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{// 100�ȏ�
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, rand() % 3 * 0.1f, rand() % 3 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, rand() % 3 * 0.1f, rand() % 3 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, rand() % 3 * 0.1f, rand() % 3 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, rand() % 3 * 0.1f, rand() % 3 * 0.1f, 1.0f);

			if ((g_aCombo[nCntScore].pos.x - SIZE_COMBO) - (MAX_SIZE_COMBO * 1.1f) < pVtx[(nCntScore * 4) + 0].pos.x)
			{// �傫���̌��x
				//���_���W�̐ݒ�
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE_COMBO * 1.1f;
			}
		}

		if (g_aNumLogCombo[nCntScore] != aNumber[nCntScore])
		{
			if ((g_aCombo[nCntScore].pos.x - SIZE_COMBO) - MAX_SIZE_COMBO < pVtx[(nCntScore * 4) + 0].pos.x)
			{// �傫���̌��x
			 //���_���W�̐ݒ�
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE_COMBO;
			}
		}

		g_aNumLogCombo[nCntScore] = aNumber[nCntScore];	// �V�����f�[�^��ۑ�
	}

	if (g_nNumCombo < 10)
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
	else if (g_nNumCombo < 100)
	{// �X�X�ȉ�����3���ڂ𓧖���
		pVtx[8].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[9].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[10].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[11].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCombo->Unlock();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexCombo(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntScore;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COMBO_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCombo,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_COMBO; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[(nCntScore * 4) + 0].pos = D3DXVECTOR3(g_aCombo[nCntScore].pos.x - SIZE_COMBO, g_aCombo[nCntScore].pos.y - SIZE_COMBO, 0.0f);
		pVtx[(nCntScore * 4) + 1].pos = D3DXVECTOR3(g_aCombo[nCntScore].pos.x + SIZE_COMBO, g_aCombo[nCntScore].pos.y - SIZE_COMBO, 0.0f);
		pVtx[(nCntScore * 4) + 2].pos = D3DXVECTOR3(g_aCombo[nCntScore].pos.x - SIZE_COMBO, g_aCombo[nCntScore].pos.y + SIZE_COMBO, 0.0f);
		pVtx[(nCntScore * 4) + 3].pos = D3DXVECTOR3(g_aCombo[nCntScore].pos.x + SIZE_COMBO, g_aCombo[nCntScore].pos.y + SIZE_COMBO, 0.0f);

		// �T�C�Y�̋L��
		g_aDiffSizeComco[nCntScore][0] = pVtx[(nCntScore * 4) + 0].pos;
		g_aDiffSizeComco[nCntScore][1] = pVtx[(nCntScore * 4) + 1].pos;
		g_aDiffSizeComco[nCntScore][2] = pVtx[(nCntScore * 4) + 2].pos;
		g_aDiffSizeComco[nCntScore][3] = pVtx[(nCntScore * 4) + 3].pos;

		//1.0f�ŌŒ�
		pVtx[(nCntScore * 4) + 0].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 1].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 2].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 3].rhw = 1.0f;

		//���_�J���[
		pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//�e�N�X�`�����W
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//���_���W�̐ݒ�
	pVtx[(NUM_COMBO * 4) + 0].pos = D3DXVECTOR3(g_aCombo[NUM_COMBO].pos.x - LOGO_SIZE_COMBO * 1.5f, g_aCombo[NUM_COMBO].pos.y - LOGO_SIZE_COMBO, 0.0f);
	pVtx[(NUM_COMBO * 4) + 1].pos = D3DXVECTOR3(g_aCombo[NUM_COMBO].pos.x + LOGO_SIZE_COMBO * 1.5f, g_aCombo[NUM_COMBO].pos.y - LOGO_SIZE_COMBO, 0.0f);
	pVtx[(NUM_COMBO * 4) + 2].pos = D3DXVECTOR3(g_aCombo[NUM_COMBO].pos.x - LOGO_SIZE_COMBO * 1.5f, g_aCombo[NUM_COMBO].pos.y + LOGO_SIZE_COMBO, 0.0f);
	pVtx[(NUM_COMBO * 4) + 3].pos = D3DXVECTOR3(g_aCombo[NUM_COMBO].pos.x + LOGO_SIZE_COMBO * 1.5f, g_aCombo[NUM_COMBO].pos.y + LOGO_SIZE_COMBO, 0.0f);

	//1.0f�ŌŒ�
	pVtx[(NUM_COMBO * 4) + 0].rhw = 1.0f;
	pVtx[(NUM_COMBO * 4) + 1].rhw = 1.0f;
	pVtx[(NUM_COMBO * 4) + 2].rhw = 1.0f;
	pVtx[(NUM_COMBO * 4) + 3].rhw = 1.0f;

	//���_�J���[
	pVtx[(NUM_COMBO * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[(NUM_COMBO * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[(NUM_COMBO * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[(NUM_COMBO * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//�e�N�X�`�����W
	pVtx[(NUM_COMBO * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[(NUM_COMBO * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[(NUM_COMBO * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[(NUM_COMBO * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCombo->Unlock();
}
