//=============================================================================
//
// �̗͏��� [life.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "life.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BAR_WIDTH_LIFE		(500)
#define BAR_HEIGHT_LIFE		(60)
#define NUM_TEXTURE_LIFE	(1)
#define NUM_POLYGON_LIFE	(4)
#define SUBTRACT_TIME		(25)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureLife[NUM_TEXTURE_LIFE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
Life g_Life[NUM_POLYGON_LIFE];		// ���C�t�̍\����
int g_nCntSubtraction;				// �o�[�����炵�n�߂�J�E���^

//=============================================================================
// ����������
//=============================================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �g
	g_Life[0].pos = D3DXVECTOR3(20.0f, 630.0f, 0.0f);
	g_Life[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	g_Life[0].nLife = MAX_LIFE;				// ���C�t���ő�ɂ���
	g_Life[0].fLength = BAR_WIDTH_LIFE;		// �o�[�̒�����ݒ�
	g_Life[0].nType = -1;

	// �o�[
	for (int nCntLife = 1; nCntLife < NUM_POLYGON_LIFE; nCntLife++)
	{
		g_Life[nCntLife].pos = D3DXVECTOR3(20.0f, 630.0f, 0.0f);
		g_Life[nCntLife].nLife = MAX_LIFE;				// ���C�t���ő�ɂ���
		g_Life[nCntLife].fLength = BAR_WIDTH_LIFE;		// �o�[�̒�����ݒ�
		g_Life[nCntLife].nType = -1;
	}

	g_Life[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_Life[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	g_Life[3].col = D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f);

	// �o�[�����炵�n�߂�J�E���^
	g_nCntSubtraction = 0;

	// ���_���̍쐬
	MakeVertexLife(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitLife(void)
{
	// �e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < NUM_TEXTURE_LIFE; nCntTexture++)
	{
		if (g_pTextureLife[nCntTexture] != NULL)
		{
			g_pTextureLife[nCntTexture]->Release();
			g_pTextureLife[nCntTexture] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLife(void)
{
	/*	g_Life[3].fLength == ���݂̃��C�t					�i�őO�ʂɗ���o�[�j			*/
	/*	g_Life[2].fLength == �_���[�W���󂯂Č��������̃o�[	�i�őO�ʂ������ɗ���o�[�j*/

	if (g_Life[2].fLength > g_Life[3].fLength)
	{// ���C�t�Ɠ����l�ɂȂ�܂Ō��̃Q�[�W�����炷
		g_nCntSubtraction++;								// �_���[�W�o�[�����炵�n�߂邽�߂̃J�E���g

		if (g_nCntSubtraction >= SUBTRACT_TIME)
		{// ��莞�Ԍo��
			g_Life[2].fLength -= BAR_WIDTH_LIFE * 0.01f;	// �_���[�W�o�[�̒�����1�������炷

			VERTEX_2D *pVtx;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�
			pVtx[(2 * 4) + 1].pos = D3DXVECTOR3(g_Life[2].pos.x + g_Life[2].fLength, g_Life[2].pos.y, 0.0f);
			pVtx[(2 * 4) + 3].pos = D3DXVECTOR3(g_Life[2].pos.x + g_Life[2].fLength, g_Life[2].pos.y + BAR_HEIGHT_LIFE, 0.0f);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffLife->Unlock();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLife(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLife = 0; nCntLife < NUM_POLYGON_LIFE; nCntLife++)
	{
		// �e�N�X�`���̐ݒ�
		if (g_Life[nCntLife].nType >= 0)
		{// �e�N�X�`���̎w��
			pDevice->SetTexture(0, g_pTextureLife[g_Life[nCntLife].nType]);
		}
		else
		{// 0���Ⴂ�ƃe�N�X�`���𒣂�Ȃ�
			pDevice->SetTexture(0, NULL);

		}

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntLife, 2);
	}
}

//=============================================================================
// �̗͂̑���
//=============================================================================
void AddSubtractLife(int nValue)
{
	g_Life[3].nLife += nValue;		// ���C�t�𑝌�������
	float fLife;

	if (g_Life[3].nLife > 0)
	{// ���C�t��0���傫���Ƃ�
		fLife = (BAR_WIDTH_LIFE * 1.0f) / ((MAX_LIFE * 1.0f) / g_Life[3].nLife);	// �o�[�̌��̒�����100%�Ƃ��ă��C�t�𑝌�������
	}
	else
	{// ���C�t��0�ȉ��̂Ƃ�
		fLife = 0;
	}

	if (g_Life[3].nLife >= MAX_LIFE)
	{// �ő�l�𒴂�����߂�
		g_Life[3].nLife = MAX_LIFE;
		fLife = BAR_WIDTH_LIFE;
	}

	g_Life[3].fLength = fLife;	// ���݂̃��C�t�o�[�̒���

	g_nCntSubtraction = 0;		// �_���[�W�o�[�̒��������炵�n�߂邽�߂̃J�E���^��������

	// ���_���̐ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	if (nValue > 0)
	{// �񕜂����Ƃ����̃o�[�����₷
		g_Life[2].fLength = fLife;

		//���_���W�̐ݒ�
		pVtx[(2 * 4) + 1].pos = D3DXVECTOR3(g_Life[2].pos.x + g_Life[2].fLength, g_Life[2].pos.y, 0.0f);
		pVtx[(2 * 4) + 3].pos = D3DXVECTOR3(g_Life[2].pos.x + g_Life[2].fLength, g_Life[2].pos.y + BAR_HEIGHT_LIFE, 0.0f);
	}

	// ���݂̃��C�t
	pVtx[(3 * 4) + 1].pos = D3DXVECTOR3(g_Life[3].pos.x + g_Life[3].fLength, g_Life[3].pos.y, 0.0f);
	pVtx[(3 * 4) + 3].pos = D3DXVECTOR3(g_Life[3].pos.x + g_Life[3].fLength, g_Life[3].pos.y + BAR_HEIGHT_LIFE, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// ���݂̃��C�t�̎擾
//=============================================================================
int GetLife(void)
{
	return g_Life[3].nLife;
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntLife;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_POLYGON_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntLife = 0; nCntLife < NUM_POLYGON_LIFE; nCntLife++)
	{
		//���_���W�̐ݒ�
		pVtx[(nCntLife * 4) + 0].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x,					g_Life[nCntLife].pos.y, 0.0f);
		pVtx[(nCntLife * 4) + 1].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x + BAR_WIDTH_LIFE,	g_Life[nCntLife].pos.y, 0.0f);
		pVtx[(nCntLife * 4) + 2].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x,					g_Life[nCntLife].pos.y + BAR_HEIGHT_LIFE, 0.0f);
		pVtx[(nCntLife * 4) + 3].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x + BAR_WIDTH_LIFE,	g_Life[nCntLife].pos.y + BAR_HEIGHT_LIFE, 0.0f);

		//1.0f�ŌŒ�
		pVtx[(nCntLife * 4) + 0].rhw = 1.0f;
		pVtx[(nCntLife * 4) + 1].rhw = 1.0f;
		pVtx[(nCntLife * 4) + 2].rhw = 1.0f;
		pVtx[(nCntLife * 4) + 3].rhw = 1.0f;

		//���_�J���[
		pVtx[(nCntLife * 4) + 0].col = g_Life[nCntLife].col;
		pVtx[(nCntLife * 4) + 1].col = g_Life[nCntLife].col;
		pVtx[(nCntLife * 4) + 2].col = g_Life[nCntLife].col;
		pVtx[(nCntLife * 4) + 3].col = g_Life[nCntLife].col;

		//�e�N�X�`�����W
		pVtx[(nCntLife * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntLife * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntLife * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntLife * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
}
