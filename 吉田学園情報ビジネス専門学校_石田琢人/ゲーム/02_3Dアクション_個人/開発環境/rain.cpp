//=============================================================================
//
// �J���� [rain.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "rain.h"
#include "effect.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RAIN		"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_RAIN	(300)	// �J�̍ő吔
#define RAIN_FIELD	(2500 / 2)	//
#define TITLE_FIELD	(150)	//

#define RAIN_APPEAR	(10)	//
#define RAIN_SIZE	(0.3f)	// �J�̑傫��
#define RAIN_SPEED	(2.0f)	// �J�̑���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRain = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRain = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
RAIN g_aRain[MAX_RAIN];
int g_nCntRain;

//=============================================================================
// �|���S������������
//=============================================================================
void InitRain(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �W�F�l���[�^�̏���������
	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		g_aRain[nCntRain].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRain[nCntRain].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aRain[nCntRain].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRain[nCntRain].bUse = false;
	}

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

	VERTEX_3D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRain->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		pVtx[0].pos = D3DXVECTOR3(-RAIN_SIZE, RAIN_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(RAIN_SIZE, RAIN_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-RAIN_SIZE, -RAIN_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RAIN_SIZE, -RAIN_SIZE, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

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
	int nCntRain;
	VERTEX_3D*pVtx;	// ���_���ւ̃|�C���^

	Player *pPlayer = GetPlayer();
	MODE mode = GetMode();

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRain->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		g_nCntRain++;	// �J���o���Ԋu�̃J�E���^

		switch (mode)
		{
		case MODE_TITLE:
			if (g_aRain[nCntRain].bUse == true)
			{// �g���Ă���
				if (g_aRain[nCntRain].pos.y <= 0)
				{
					g_aRain[nCntRain].bUse = false;
				}

				g_aRain[nCntRain].pos += g_aRain[nCntRain].move;
			}
			else
			{
				if (g_nCntRain % RAIN_APPEAR == 0)
				{// ��莞�Ԍo�߂ŉJ���~�炷
					if (g_aRain[nCntRain].bUse == false)
					{
						g_aRain[nCntRain].pos = D3DXVECTOR3(-TITLE_FIELD + (rand() % (TITLE_FIELD * 2)) * 1.0f, 200.0f, ((-100 / 2) + (rand() % 100)) * 1.0f);
						g_aRain[nCntRain].move = D3DXVECTOR3(0.0f, -RAIN_SPEED * (rand() % 3 + 2.0f), 0.0f);
						g_aRain[nCntRain].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						g_aRain[nCntRain].bUse = true;

						//���_���W�̐ݒ�
						pVtx[0].pos = D3DXVECTOR3(-RAIN_SIZE, RAIN_SIZE + (rand() % 20 + 20.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(RAIN_SIZE, RAIN_SIZE + (rand() % 20 + 20.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(-RAIN_SIZE, -RAIN_SIZE, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(RAIN_SIZE, -RAIN_SIZE, 0.0f);

						// ���_�J���[
						pVtx[0].col = g_aRain[nCntRain].col;
						pVtx[1].col = g_aRain[nCntRain].col;
						pVtx[2].col = g_aRain[nCntRain].col;
						pVtx[3].col = g_aRain[nCntRain].col;

						break;
					}
				}
			}
			break;

		case MODE_GAME:
			if (g_aRain[nCntRain].bUse == true)
			{// �g���Ă���
				if (g_aRain[nCntRain].pos.y <= 0)
				{
					g_aRain[nCntRain].bUse = false;
				}

				g_aRain[nCntRain].pos += g_aRain[nCntRain].move;
			}
			else
			{
				if (g_nCntRain % RAIN_APPEAR == 0 && pPlayer->pos.x > 0 && pPlayer->pos.x < RAIN_FIELD * 4)
				{// ��莞�Ԍo�߂ŉJ���~�炷
					if (g_aRain[nCntRain].bUse == false)
					{
						g_aRain[nCntRain].pos = D3DXVECTOR3((RAIN_FIELD + (rand() % (RAIN_FIELD * 2))) * 1.0f, 150.0f, -200.0f + (rand() % (200 * 2)) * 1.0f);
						g_aRain[nCntRain].move = D3DXVECTOR3(0.0f, -RAIN_SPEED * (rand() % 3 + 2.0f), 0.0f);
						g_aRain[nCntRain].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						g_aRain[nCntRain].bUse = true;

						//���_���W�̐ݒ�
						pVtx[0].pos = D3DXVECTOR3(-RAIN_SIZE, RAIN_SIZE + (rand() % 20 + 20.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(RAIN_SIZE, RAIN_SIZE + (rand() % 20 + 20.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(-RAIN_SIZE, -RAIN_SIZE, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(RAIN_SIZE, -RAIN_SIZE, 0.0f);

						// ���_�J���[
						pVtx[0].col = g_aRain[nCntRain].col;
						pVtx[1].col = g_aRain[nCntRain].col;
						pVtx[2].col = g_aRain[nCntRain].col;
						pVtx[3].col = g_aRain[nCntRain].col;

						break;
					}
				}
			}
			break;
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
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
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	int nCntRain;

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		if (g_aRain[nCntRain].bUse == true)
		{// �J���g���Ă����
		 // ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aRain[nCntRain].mtxWorld);

			// ��]�𔽉f
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			g_aRain[nCntRain].mtxWorld._11 = mtxView._11;
			g_aRain[nCntRain].mtxWorld._12 = mtxView._21;
			g_aRain[nCntRain].mtxWorld._13 = mtxView._31;
			//g_aRain[nCntRain].mtxWorld._21 = mtxView._12;
			//g_aRain[nCntRain].mtxWorld._22 = mtxView._22;
			//g_aRain[nCntRain].mtxWorld._23 = mtxView._32;
			g_aRain[nCntRain].mtxWorld._31 = mtxView._13;
			g_aRain[nCntRain].mtxWorld._32 = mtxView._23;
			g_aRain[nCntRain].mtxWorld._33 = mtxView._33;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aRain[nCntRain].pos.x, g_aRain[nCntRain].pos.y, g_aRain[nCntRain].pos.z);
			D3DXMatrixMultiply(&g_aRain[nCntRain].mtxWorld, &g_aRain[nCntRain].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aRain[nCntRain].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffRain, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureRain);

			// �e�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRain, 2);
		}
	}

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
