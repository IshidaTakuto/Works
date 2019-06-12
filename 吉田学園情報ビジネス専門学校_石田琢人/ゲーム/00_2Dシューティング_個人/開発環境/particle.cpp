//=============================================================================
//
// �G�t�F�N�g�̏��� [Particle.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "Particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PARTICLE		"data/TEXTURE/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	MAX_PARTICLE			(5096)							// �G�t�F�N�g�̍ő吔
#define PARTICLE_MOVE	(50)

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
PARTICLE				g_aParticle[MAX_PARTICLE];	// �G�t�F�N�g�̏��

//=============================================================================
// ����������
//=============================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].nParticles = 0;
		g_aParticle[nCntParticle].state = PARTICLESTATE_NONE;
		g_aParticle[nCntParticle].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PARTICLE, &g_pTextureParticle);

	// ���_���̍쐬
	MakeVertexParticle(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{
	int nCntParticle;
	int nCntParticle2;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		switch (g_aParticle[nCntParticle].state)
		{
		case PARTICLESTATE_NONE:
			break;

		case PARTICLESTATE_BULLET:
			//g_aParticle[nCntParticle].nLife--;
			//g_aParticle[nCntParticle].fRadius -= 0.3f;

			//if (g_aParticle[nCntParticle].nLife <= 0 || g_aParticle[nCntParticle].fRadius <= 0)
			//{
			//	g_aParticle[nCntParticle].state = PARTICLESTATE_NONE;
			//	g_aParticle[nCntParticle].bUse = false;
			//}

			////���_���W�̐ݒ�
			//pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			//pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			//pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			//pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;

			break;

		case PARTICLESTATE_EXPLOSION:
			for (nCntParticle2 = 0; nCntParticle2 <= g_aParticle[nCntParticle].nParticles; nCntParticle2++)
			{
				g_aParticle[nCntParticle2].nLife--;
				g_aParticle[nCntParticle2].fRadius -= 0.1f;

				g_aParticle[nCntParticle2].pos.x += g_aParticle[nCntParticle2].move.x;
				g_aParticle[nCntParticle2].pos.y += g_aParticle[nCntParticle2].move.y;

				if (g_aParticle[nCntParticle2].nLife <= 0 || g_aParticle[nCntParticle2].fRadius <= 0)
				{
					g_aParticle[nCntParticle2].state = PARTICLESTATE_NONE;
					g_aParticle[nCntParticle2].bUse = false;
				}

			}
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;

			break;
		}

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffParticle->Unlock();
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	int nCntParticle;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureParticle);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �|���S���̕`��
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)	//�g�p����Ă���
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntParticle;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	// ���_�o�b�t�@�̏���ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;

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

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffParticle->Unlock();
}


//=============================================================================
// �p�[�e�B�N���ݒ�
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nParticles, PARTICLESTATE state)
{
	int nCntParticle;
	int nCntParticle2;

	// ���_�o�b�t�@�̏���ݒ�
	//VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle2 = 0; nCntParticle2 < nParticles; nCntParticle2++)
	{
		for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
		{
			if (g_aParticle[nCntParticle].bUse == false)
			{
				g_aParticle[nCntParticle2].pos = pos;
				g_aParticle[nCntParticle2].move = move;
				g_aParticle[nCntParticle2].col = col;
				g_aParticle[nCntParticle2].fRadius = fRadius;
				g_aParticle[nCntParticle2].nLife = nLife;
				g_aParticle[nCntParticle2].nParticles = nParticles;
				g_aParticle[nCntParticle2].state = state;

				//���_���W�̐ݒ�
				//pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
				//pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
				//pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
				//pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;

				//���_�J���[
				//pVtx[0].col = col;
				//pVtx[1].col = col;
				//pVtx[2].col = col;
				//pVtx[3].col = col;

				g_aParticle[nCntParticle].bUse = true;
				break;
			}
			//pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffParticle->Unlock();
}
