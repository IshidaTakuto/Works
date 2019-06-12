//=============================================================================
//
// �J���� [rain.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "rain.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RAIN		"data/TEXTURE/rain01.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_RAIN		(2)		// �J�̍ő吔
#define RAIN_SPEED		(2.0f)	// �J�̑���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRain = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRain = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
int g_nCntRain;

//=============================================================================
// �|���S������������
//=============================================================================
void InitRain(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	g_nCntRain = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RAIN, &g_pTextureRain);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_RAIN,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRain,
		NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRain->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//1.0f�ŌŒ�
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
		pVtx[1].tex = D3DXVECTOR2(1.0f + (nCntRain * 1.5f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f + (nCntRain * 1.5f));
		pVtx[3].tex = D3DXVECTOR2(1.0f + (nCntRain * 1.5f), 1.0f + (nCntRain * 1.5f));

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffRain->Unlock();
}

//=============================================================================
// �|���S���I������
//=============================================================================
void UninitRain(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureRain != NULL)
	{
		g_pTextureRain->Release();
		g_pTextureRain = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffRain != NULL)
	{
		g_pVtxBuffRain->Release();
		g_pVtxBuffRain = NULL;
	}
}

//=============================================================================
// �|���S���X�V����
//=============================================================================
void UpdateRain(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRain->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		// �e�N�X�`�����W
		pVtx[0].tex -= D3DXVECTOR2(0.0f, 0.015f);
		pVtx[1].tex -= D3DXVECTOR2(0.0f, 0.015f);
		pVtx[2].tex -= D3DXVECTOR2(0.0f, 0.015f);
		pVtx[3].tex -= D3DXVECTOR2(0.0f, 0.015f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffRain->Unlock();
}

//=============================================================================
// �|���S���`�揈��
//=============================================================================
void DrawRain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRain, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRain);

	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRain, 2);
	}

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}
