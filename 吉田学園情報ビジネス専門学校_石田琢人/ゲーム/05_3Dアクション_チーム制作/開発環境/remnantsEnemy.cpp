//=============================================================================
//
// �G�c���\������ [remnantsEnemy.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "remnantsEnemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_REMNANTS_00		"data/TEXTURE/number.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_REMNANTS_01		"data/TEXTURE/title_icon.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define NUM_REMNANTS			(3)								// �X�R�A�̍ő包��
#define SIZE_REMNANTS			(25)							// �X�R�A�̑傫��
#define MAX_TEX_REMNANTS		(2)								// �e�N�X�`���̐�
#define MAX_SIZE_REMNANTS		(40)							// �傫���̍ő�l
#define SIZE_LARGE_REMNANTS		(20)							// �ǂꂭ�炢�傫�����邩
#define SIZE_SMALL_REMNANTS		(0.1f)							// ����������l
#define MAX_REMNANTS_POLYGON	(NUM_REMNANTS + 1)				// �|���S����
#define REMNANTS_POSITION		(1180)							// X���W�̈ʒu

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexRemnants(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRemnants[MAX_TEX_REMNANTS] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRemnants = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
Remnants				g_aRemnants[MAX_REMNANTS_POLYGON];		// �G�̎c���������ƂɊi�[
int g_nNumRemnants;												// �G�̎c��
int g_aNumLogRemnants[NUM_REMNANTS];							// �����Ƃ̓G�̎c����ۑ�
D3DXVECTOR3 g_aDiffSizeRemnants[NUM_REMNANTS][4];				// ���̃T�C�Y

//=============================================================================
// ����������
//=============================================================================
void InitRemnants(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntScore = 0; nCntScore < NUM_REMNANTS; nCntScore++)
	{
		// �X�R�A�̏�����
		g_aRemnants[nCntScore].pos = D3DXVECTOR3(REMNANTS_POSITION - (nCntScore * 50.0f), 50, 0.0f);
		g_aRemnants[nCntScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aRemnants[nCntScore].nType = 0;

		g_aNumLogRemnants[nCntScore] = 0;
	}

	g_aRemnants[NUM_REMNANTS].pos = D3DXVECTOR3(REMNANTS_POSITION - (NUM_REMNANTS * 50.0f), 50, 0.0f);
	g_aRemnants[NUM_REMNANTS].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aRemnants[NUM_REMNANTS].nType = 1;

	g_nNumRemnants = 30;		// �G�̎c���̏�����

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_REMNANTS_00, &g_pTextureRemnants[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_REMNANTS_01, &g_pTextureRemnants[1]);

	// ���_���̍쐬
	MakeVertexRemnants(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitRemnants(void)
{
	for (int nCntScore = 0; nCntScore < MAX_TEX_REMNANTS; nCntScore++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureRemnants[nCntScore] != NULL)
		{
			g_pTextureRemnants[nCntScore]->Release();
			g_pTextureRemnants[nCntScore] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRemnants != NULL)
	{
		g_pVtxBuffRemnants->Release();
		g_pVtxBuffRemnants = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateRemnants(void)
{
	D3DXVECTOR3 aDestSize[4];	// �T�C�Y�̍��������߂�̂ɕK�v
	D3DXCOLOR col[4];

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRemnants->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nNumRemnants < 10)
	{
		g_aRemnants[0].pos.x = REMNANTS_POSITION - ((NUM_REMNANTS - 1) * 50);
	}
	else if (g_nNumRemnants < 100)
	{
		g_aRemnants[0].pos.x = REMNANTS_POSITION - ((NUM_REMNANTS - 2) * 50);
		g_aRemnants[1].pos.x = REMNANTS_POSITION - ((NUM_REMNANTS - 1) * 50);
	}
	else
	{
		g_aRemnants[0].pos.x = REMNANTS_POSITION - ((NUM_REMNANTS - 3) * 50);
		g_aRemnants[1].pos.x = REMNANTS_POSITION - ((NUM_REMNANTS - 2) * 50);
	}

	for (int nCntVtx = 0; nCntVtx < (int)log10f((float)g_nNumRemnants) + 1; nCntVtx++)
	{// ���݂̌�������
		//���_���W�̐ݒ�
		pVtx[(nCntVtx * 4) + 0].pos = D3DXVECTOR3(g_aRemnants[nCntVtx].pos.x - SIZE_REMNANTS, g_aRemnants[nCntVtx].pos.y - SIZE_REMNANTS, 0.0f);
		pVtx[(nCntVtx * 4) + 1].pos = D3DXVECTOR3(g_aRemnants[nCntVtx].pos.x + SIZE_REMNANTS, g_aRemnants[nCntVtx].pos.y - SIZE_REMNANTS, 0.0f);
		pVtx[(nCntVtx * 4) + 2].pos = D3DXVECTOR3(g_aRemnants[nCntVtx].pos.x - SIZE_REMNANTS, g_aRemnants[nCntVtx].pos.y + SIZE_REMNANTS, 0.0f);
		pVtx[(nCntVtx * 4) + 3].pos = D3DXVECTOR3(g_aRemnants[nCntVtx].pos.x + SIZE_REMNANTS, g_aRemnants[nCntVtx].pos.y + SIZE_REMNANTS, 0.0f);

		col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (pVtx[(nCntVtx * 4) + 0].pos.x < g_aDiffSizeRemnants[nCntVtx][0].x)
		{// �T�C�Y��߂�
			aDestSize[0].x = g_aDiffSizeRemnants[nCntVtx][0].x - pVtx[(nCntVtx * 4) + 0].pos.x;
			aDestSize[0].y = g_aDiffSizeRemnants[nCntVtx][0].y - pVtx[(nCntVtx * 4) + 0].pos.y;
			aDestSize[1].x = g_aDiffSizeRemnants[nCntVtx][1].x - pVtx[(nCntVtx * 4) + 1].pos.x;
			aDestSize[1].y = g_aDiffSizeRemnants[nCntVtx][1].y - pVtx[(nCntVtx * 4) + 1].pos.y;
			aDestSize[2].x = g_aDiffSizeRemnants[nCntVtx][2].x - pVtx[(nCntVtx * 4) + 2].pos.x;
			aDestSize[2].y = g_aDiffSizeRemnants[nCntVtx][2].y - pVtx[(nCntVtx * 4) + 2].pos.y;
			aDestSize[3].x = g_aDiffSizeRemnants[nCntVtx][3].x - pVtx[(nCntVtx * 4) + 3].pos.x;
			aDestSize[3].y = g_aDiffSizeRemnants[nCntVtx][3].y - pVtx[(nCntVtx * 4) + 3].pos.y;

			//���_���W�̐ݒ�
			pVtx[(nCntVtx * 4) + 0].pos.x += aDestSize[0].x * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 0].pos.y += aDestSize[0].y * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 1].pos.x += aDestSize[1].x * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 1].pos.y += aDestSize[1].y * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 2].pos.x += aDestSize[2].x * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 2].pos.y += aDestSize[2].y * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 3].pos.x += aDestSize[3].x * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 3].pos.y += aDestSize[3].y * SIZE_SMALL_REMNANTS;

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

		int aNumber[3];

		aNumber[0] = g_nNumRemnants % 10;				// �P����
		aNumber[1] = g_nNumRemnants % 100 / 10;			// �Q����
		aNumber[2] = g_nNumRemnants % 1000 / 100;		// �R����

		for (int nCntScore = 0; nCntScore < NUM_REMNANTS; nCntScore++)
		{
			//�e�N�X�`�����W
			pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
			pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 0.0f);
			pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 1.0f);

			g_aNumLogRemnants[nCntScore] = aNumber[nCntScore];	// �V�����f�[�^��ۑ�
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRemnants->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRemnants(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRemnants, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < MAX_REMNANTS_POLYGON; nCntScore++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRemnants[g_aRemnants[nCntScore].nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

//=============================================================================
// �G�c���̐ݒ�
//=============================================================================
void SetRemnants(int nRemnant)
{
	int nCntScore;
	int aNumber[NUM_REMNANTS];	// �����Ƃ̓G�̎c��

	g_nNumRemnants = nRemnant;	// �G�̎c�����X�V

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRemnants->Lock(0, 0, (void**)&pVtx, 0);

	aNumber[0] = g_nNumRemnants % 10;				// �P����
	aNumber[1] = g_nNumRemnants % 100 / 10;			// �Q����
	aNumber[2] = g_nNumRemnants % 1000 / 100;		// �R����

	for (nCntScore = 0; nCntScore < NUM_REMNANTS; nCntScore++)
	{
		//�e�N�X�`�����W
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 1.0f);

		if (g_aNumLogRemnants[nCntScore] != aNumber[nCntScore])
		{
			if ((g_aRemnants[nCntScore].pos.x - SIZE_REMNANTS) - MAX_SIZE_REMNANTS < pVtx[(nCntScore * 4) + 0].pos.x)
			{// �傫���̌��x
				//���_���W�̐ݒ�
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE_REMNANTS;
			}
		}

		g_aNumLogRemnants[nCntScore] = aNumber[nCntScore];	// �V�����f�[�^��ۑ�
	}

	if (g_nNumRemnants < 10)
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
	else if (g_nNumRemnants < 100)
	{// �X�X�ȉ�����3���ڂ𓧖���
		pVtx[8].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[9].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[10].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[11].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRemnants->Unlock();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexRemnants(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntScore;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_REMNANTS_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRemnants,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRemnants->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_REMNANTS; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[(nCntScore * 4) + 0].pos = D3DXVECTOR3(g_aRemnants[nCntScore].pos.x - SIZE_REMNANTS, g_aRemnants[nCntScore].pos.y - SIZE_REMNANTS, 0.0f);
		pVtx[(nCntScore * 4) + 1].pos = D3DXVECTOR3(g_aRemnants[nCntScore].pos.x + SIZE_REMNANTS, g_aRemnants[nCntScore].pos.y - SIZE_REMNANTS, 0.0f);
		pVtx[(nCntScore * 4) + 2].pos = D3DXVECTOR3(g_aRemnants[nCntScore].pos.x - SIZE_REMNANTS, g_aRemnants[nCntScore].pos.y + SIZE_REMNANTS, 0.0f);
		pVtx[(nCntScore * 4) + 3].pos = D3DXVECTOR3(g_aRemnants[nCntScore].pos.x + SIZE_REMNANTS, g_aRemnants[nCntScore].pos.y + SIZE_REMNANTS, 0.0f);

		// �T�C�Y�̋L��
		g_aDiffSizeRemnants[nCntScore][0] = pVtx[(nCntScore * 4) + 0].pos;
		g_aDiffSizeRemnants[nCntScore][1] = pVtx[(nCntScore * 4) + 1].pos;
		g_aDiffSizeRemnants[nCntScore][2] = pVtx[(nCntScore * 4) + 2].pos;
		g_aDiffSizeRemnants[nCntScore][3] = pVtx[(nCntScore * 4) + 3].pos;

		//1.0f�ŌŒ�
		pVtx[(nCntScore * 4) + 0].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 1].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 2].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 3].rhw = 1.0f;

		//���_�J���[
		pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//���_���W�̐ݒ�
	pVtx[(NUM_REMNANTS * 4) + 0].pos = D3DXVECTOR3(g_aRemnants[NUM_REMNANTS].pos.x - SIZE_REMNANTS, g_aRemnants[NUM_REMNANTS].pos.y - SIZE_REMNANTS, 0.0f);
	pVtx[(NUM_REMNANTS * 4) + 1].pos = D3DXVECTOR3(g_aRemnants[NUM_REMNANTS].pos.x + SIZE_REMNANTS, g_aRemnants[NUM_REMNANTS].pos.y - SIZE_REMNANTS, 0.0f);
	pVtx[(NUM_REMNANTS * 4) + 2].pos = D3DXVECTOR3(g_aRemnants[NUM_REMNANTS].pos.x - SIZE_REMNANTS, g_aRemnants[NUM_REMNANTS].pos.y + SIZE_REMNANTS, 0.0f);
	pVtx[(NUM_REMNANTS * 4) + 3].pos = D3DXVECTOR3(g_aRemnants[NUM_REMNANTS].pos.x + SIZE_REMNANTS, g_aRemnants[NUM_REMNANTS].pos.y + SIZE_REMNANTS, 0.0f);

	//1.0f�ŌŒ�
	pVtx[(NUM_REMNANTS * 4) + 0].rhw = 1.0f;
	pVtx[(NUM_REMNANTS * 4) + 1].rhw = 1.0f;
	pVtx[(NUM_REMNANTS * 4) + 2].rhw = 1.0f;
	pVtx[(NUM_REMNANTS * 4) + 3].rhw = 1.0f;

	//���_�J���[
	pVtx[(NUM_REMNANTS * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_REMNANTS * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_REMNANTS * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_REMNANTS * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[(NUM_REMNANTS * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[(NUM_REMNANTS * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[(NUM_REMNANTS * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[(NUM_REMNANTS * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRemnants->Unlock();
}
