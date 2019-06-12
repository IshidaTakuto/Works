//=============================================================================
//
// �r�[������ [Beam.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "beam.h"
#include "player.h"
#include "effect.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BEAM		"data/TEXTURE/bar.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_BEAM	(2)	//�`���[�W�̍ő吔
#define BEAM_GAUGE_SIZE	(100)
#define MAX_GAUGE	(210)	//�Q�[�W�̍ő�l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexBeam(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBeam[MAX_BEAM] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBeam = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
BEAM					g_aBeam[MAX_BEAM];					// �r�[��

//=============================================================================
// ����������
//=============================================================================
void InitBeam(void)
{
	int nCntBeam;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �r�[���̏�����
	for (nCntBeam = 0; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		g_aBeam[nCntBeam].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBeam[nCntBeam].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30, 0.0f);
		g_aBeam[nCntBeam].nCntGauge = 0;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BEAM, &g_pTextureBeam[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BEAM, &g_pTextureBeam[1]);

	// ���_���̍쐬
	MakeVertexBeam(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitBeam(void)
{
	int nCntBeam;

	for (nCntBeam = 0; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureBeam[nCntBeam] != NULL)
		{
			g_pTextureBeam[nCntBeam]->Release();
			g_pTextureBeam[nCntBeam] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBeam != NULL)
	{
		g_pVtxBuffBeam->Release();
		g_pVtxBuffBeam = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBeam(void)
{
	Player *pPlayer;
	pPlayer = GetPlayer();

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBeam->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->state == PLAYERSTATE_BEAM)
	{
		g_aBeam[1].nCntGauge = 0;

		//���_�J���[
		pVtx[4].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	}

	if (g_aBeam[1].nCntGauge <= MAX_GAUGE)
	{
		//���_���W�̐ݒ�
		pVtx[4].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE, g_aBeam[1].pos.y - BEAM_GAUGE_SIZE, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE + g_aBeam[1].nCntGauge, g_aBeam[1].pos.y - BEAM_GAUGE_SIZE, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE, g_aBeam[1].pos.y + BEAM_GAUGE_SIZE, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE + g_aBeam[1].nCntGauge, g_aBeam[1].pos.y + BEAM_GAUGE_SIZE, 0.0f);
	}
	if (g_aBeam[1].nCntGauge == MAX_GAUGE)
	{
		//���_�J���[
		pVtx[4].col = D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f);
	}


	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBeam->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBeam(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBeam, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBeam = 0; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBeam[nCntBeam]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBeam, 2);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexBeam(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BEAM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBeam,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBeam->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aBeam[0].pos.x - BEAM_GAUGE_SIZE, g_aBeam[0].pos.y - BEAM_GAUGE_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBeam[0].pos.x + BEAM_GAUGE_SIZE, g_aBeam[0].pos.y - BEAM_GAUGE_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBeam[0].pos.x - BEAM_GAUGE_SIZE, g_aBeam[0].pos.y + BEAM_GAUGE_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBeam[0].pos.x + BEAM_GAUGE_SIZE, g_aBeam[0].pos.y + BEAM_GAUGE_SIZE, 0.0f);

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

	pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE, g_aBeam[1].pos.y - BEAM_GAUGE_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE + 1, g_aBeam[1].pos.y - BEAM_GAUGE_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE, g_aBeam[1].pos.y + BEAM_GAUGE_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE + 1, g_aBeam[1].pos.y + BEAM_GAUGE_SIZE, 0.0f);

	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBeam->Unlock();
}

//=============================================================================
// �r�[���̉��Z
//=============================================================================
void AddBeam(int nValue)
{
	g_aBeam[1].nCntGauge += nValue;
}

//=============================================================================
// �r�[���̎擾
//=============================================================================
BEAM *GetBeam(void)
{
	return &g_aBeam[1];
}