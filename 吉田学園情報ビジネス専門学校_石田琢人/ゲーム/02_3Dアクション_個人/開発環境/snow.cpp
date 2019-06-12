//=============================================================================
//
// �Ꮘ�� [snow.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "snow.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SNOW_TEXTURENAME	"data/TEXTURE/shadow000.jpg"
#define MAX_SNOW		(1020)	// ��̍ő吔
#define SNOW_FIELD		(1250)	// ���S��0�Ƃ����͈�
#define TITLE_SNOW_FIELD	(150)
#define SNOW_APPEAR		(3)	// �o������Ԋu

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSnow = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSnow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
SNOW g_aSnow[MAX_SNOW];
int g_nCntSnow;

//=============================================================================
// �|���S������������
//=============================================================================
void InitSnow(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �W�F�l���[�^�̏���������
	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		g_aSnow[nCntSnow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSnow[nCntSnow].fRadius = 0;
		g_aSnow[nCntSnow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSnow[nCntSnow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSnow[nCntSnow].bUse = false;
	}

	g_nCntSnow = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, SNOW_TEXTURENAME, &g_pTextureSnow);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SNOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSnow,
		NULL);

	VERTEX_3D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffSnow->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);

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
	g_pVtxBuffSnow->Unlock();
}

//=============================================================================
// �|���S���I������
//=============================================================================
void UninitSnow(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureSnow != NULL)
	{
		g_pTextureSnow->Release();
		g_pTextureSnow = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffSnow != NULL)
	{
		g_pVtxBuffSnow->Release();
		g_pVtxBuffSnow = NULL;
	}
}

//=============================================================================
// �|���S���X�V����
//=============================================================================
void UpdateSnow(void)
{
	int nCntSnow;
	VERTEX_3D*pVtx;	// ���_���ւ̃|�C���^

	Player *pPlayer = GetPlayer();
	MODE mode = GetMode();

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffSnow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		switch (mode)
		{
		case MODE_TITLE:
			g_nCntSnow = (g_nCntSnow + 1) % SNOW_APPEAR;	// ����o���Ԋu�̃J�E���^

			if (g_aSnow[nCntSnow].bUse == true)
			{// �g���Ă���
				if (g_aSnow[nCntSnow].pos.y <= 50)
				{
					g_aSnow[nCntSnow].bUse = false;
				}

				g_aSnow[nCntSnow].pos += g_aSnow[nCntSnow].move;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
			}
			else
			{
				if (g_nCntSnow == 0)
				{// ��莞�Ԍo�߂Ő���~�炷
					g_aSnow[nCntSnow].pos = D3DXVECTOR3(-TITLE_SNOW_FIELD + (rand() % (TITLE_SNOW_FIELD * 2)) * 1.0f, 200.0f, ((-50 / 2) + (rand() % 50)) * 1.0f);
					g_aSnow[nCntSnow].move = D3DXVECTOR3(rand() % 3 * 0.1f, -rand() % 10 * 0.1f, rand() % 3 * 0.1f);
					g_aSnow[nCntSnow].fRadius = 0.7f;
					g_aSnow[nCntSnow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
					g_aSnow[nCntSnow].bUse = true;

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);

					// ���_�J���[
					pVtx[0].col = g_aSnow[nCntSnow].col;
					pVtx[1].col = g_aSnow[nCntSnow].col;
					pVtx[2].col = g_aSnow[nCntSnow].col;
					pVtx[3].col = g_aSnow[nCntSnow].col;

					//break;
				}
			}
			break;

		case MODE_GAME:
			g_nCntSnow = (g_nCntSnow + 1) % SNOW_APPEAR;	// ����o���Ԋu�̃J�E���^

			if (g_aSnow[nCntSnow].bUse == true)
			{// �g���Ă���
				if (g_aSnow[nCntSnow].pos.y <= 0)
				{
					g_aSnow[nCntSnow].bUse = false;
				}

				g_aSnow[nCntSnow].pos += g_aSnow[nCntSnow].move;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
			}
			else
			{
				if (g_nCntSnow == 0 && pPlayer->pos.x > SNOW_FIELD * 4)
				{// ��莞�Ԍo�߂Ő���~�炷
					g_aSnow[nCntSnow].pos = D3DXVECTOR3((SNOW_FIELD * 5 + rand() % (SNOW_FIELD * 2)) * 1.0f, 150.0f, (-200.0f + (rand() % 400)) * 1.0f);
					g_aSnow[nCntSnow].move = D3DXVECTOR3(rand() % 3 * 0.1f, -rand() % 10 * 0.1f, rand() % 3 * 0.1f);
					g_aSnow[nCntSnow].fRadius = 2.0f;
					g_aSnow[nCntSnow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
					g_aSnow[nCntSnow].bUse = true;

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);

					// ���_�J���[
					pVtx[0].col = g_aSnow[nCntSnow].col;
					pVtx[1].col = g_aSnow[nCntSnow].col;
					pVtx[2].col = g_aSnow[nCntSnow].col;
					pVtx[3].col = g_aSnow[nCntSnow].col;

					break;
				}
			}
			break;
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffSnow->Unlock();
}

//=============================================================================
// �|���S���`�揈��
//=============================================================================
void DrawSnow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	int nCntSnow;

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		if (g_aSnow[nCntSnow].bUse == true)
		{// �Ⴊ�g���Ă����
		 // ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aSnow[nCntSnow].mtxWorld);

			// ��]�𔽉f
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			g_aSnow[nCntSnow].mtxWorld._11 = mtxView._11;
			g_aSnow[nCntSnow].mtxWorld._12 = mtxView._21;
			g_aSnow[nCntSnow].mtxWorld._13 = mtxView._31;
			g_aSnow[nCntSnow].mtxWorld._21 = mtxView._12;
			g_aSnow[nCntSnow].mtxWorld._22 = mtxView._22;
			g_aSnow[nCntSnow].mtxWorld._23 = mtxView._32;
			g_aSnow[nCntSnow].mtxWorld._31 = mtxView._13;
			g_aSnow[nCntSnow].mtxWorld._32 = mtxView._23;
			g_aSnow[nCntSnow].mtxWorld._33 = mtxView._33;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aSnow[nCntSnow].pos.x, g_aSnow[nCntSnow].pos.y, g_aSnow[nCntSnow].pos.z);
			D3DXMatrixMultiply(&g_aSnow[nCntSnow].mtxWorld, &g_aSnow[nCntSnow].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aSnow[nCntSnow].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffSnow, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureSnow);

			// �e�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSnow, 2);
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
