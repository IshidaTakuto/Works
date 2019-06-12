//=============================================================================
//
// �Ԃт珈�� [petals.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "petals.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PETALS_TEXTURENAME00	"data/TEXTURE/petal00.jpg"
#define PETALS_TEXTURENAME01	"data/TEXTURE/petal01.jpg"
#define MAX_PETALS			(1020)		// �Ԃт�̍ő吔
#define PETALS_FIELD		(2500 / 2)		// ���S��0�Ƃ����͈�
#define TITLE_PETALS_FIELD	(200)		// ���S��0�Ƃ����͈�
#define PETALS_APPEAR		(8)			// �o������Ԋu
#define MAX_TEXTURE			(2)			// �e�N�X�`���̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePetals[MAX_TEXTURE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPetals = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
PETALS g_aPetals[MAX_PETALS];
int g_nCntPetals;	// �Ԃт���o���Ԋu�̃J�E���^

//=============================================================================
// �|���S������������
//=============================================================================
void InitPetals(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �W�F�l���[�^�̏���������
	for (int nCntPetals = 0; nCntPetals < MAX_PETALS; nCntPetals++)
	{
		g_aPetals[nCntPetals].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].fRadius = 0;
		g_aPetals[nCntPetals].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aPetals[nCntPetals].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].bUse = false;
	}

	g_nCntPetals = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PETALS_TEXTURENAME00, &g_pTexturePetals[0]);
	D3DXCreateTextureFromFile(pDevice, PETALS_TEXTURENAME01, &g_pTexturePetals[1]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PETALS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPetals,
		NULL);

	VERTEX_3D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPetals->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	for (int nCntPetals = 0; nCntPetals < MAX_PETALS; nCntPetals++)
	{
		pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		
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
	g_pVtxBuffPetals->Unlock();
}

//=============================================================================
// �|���S���I������
//=============================================================================
void UninitPetals(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		// �e�N�X�`���̔j��
		if (g_pTexturePetals[nCntTex] != NULL)
		{
			g_pTexturePetals[nCntTex]->Release();
			g_pTexturePetals[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPetals != NULL)
	{
		g_pVtxBuffPetals->Release();
		g_pVtxBuffPetals = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePetals(PETALSTYPE type)
{
	int nCntPetals;
	Player *pPlayer = GetPlayer();
	MODE mode = GetMode();

	VERTEX_3D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPetals->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPetals = 0; nCntPetals < MAX_PETALS; nCntPetals++)
	{
		switch (mode)
		{
		case MODE_TITLE:
			switch (type)
			{
			case PETALSTYPE_SAKURA:
				g_nCntPetals = (g_nCntPetals + 1) % 2;	// �Ԃт���o���Ԋu

				if (g_aPetals[nCntPetals].bUse == true)
				{// �g���Ă���
					if (g_aPetals[nCntPetals].pos.y <= 0)
					{// �n�ʂɂ��������
						g_aPetals[nCntPetals].bUse = false;
					}

					// ��]������
					g_aPetals[nCntPetals].rot += g_aPetals[nCntPetals].angle;

					g_aPetals[nCntPetals].pos += g_aPetals[nCntPetals].move;

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
				}
				else
				{
					if (g_nCntPetals == 0)
					{// ��莞�Ԍo�߂ŉԂт���~�炷
						g_aPetals[nCntPetals].pos = D3DXVECTOR3((-TITLE_PETALS_FIELD + (rand() % (TITLE_PETALS_FIELD * 2))) * 1.0f, 200.0f, ((-100 / 2) + (rand() % 100)) * 1.0f);
						g_aPetals[nCntPetals].move = D3DXVECTOR3(rand() % 3 * 0.1f, -rand() % 3 * 0.1f, rand() % 3 * 0.1f);
						g_aPetals[nCntPetals].fRadius = rand() % 3 * 1.0f;
						g_aPetals[nCntPetals].col = D3DXCOLOR(1.0f, 0.5f, 0.8f, 1.0f);
						g_aPetals[nCntPetals].angle = D3DXVECTOR3((-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f);
						g_aPetals[nCntPetals].bUse = true;
						g_aPetals[nCntPetals].nType = 1;

						//���_���W�̐ݒ�
						pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
						pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);

						// ���_�J���[
						pVtx[0].col = g_aPetals[nCntPetals].col;
						pVtx[1].col = g_aPetals[nCntPetals].col;
						pVtx[2].col = g_aPetals[nCntPetals].col;
						pVtx[3].col = g_aPetals[nCntPetals].col;

						break;
					}
				}
				break;

			case PETALSTYPE_MOMIJI:
				g_nCntPetals = (g_nCntPetals + 1) % 2;	// �Ԃт���o���Ԋu

				if (g_aPetals[nCntPetals].bUse == true)
				{// �g���Ă���
					if (g_aPetals[nCntPetals].pos.y <= 0)
					{// �n�ʂɂ��������
						g_aPetals[nCntPetals].bUse = false;
					}

					// ��]������
					g_aPetals[nCntPetals].rot += g_aPetals[nCntPetals].angle;

					g_aPetals[nCntPetals].pos += g_aPetals[nCntPetals].move;

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
				}
				else
				{
					if (g_nCntPetals == 0)
					{// ��莞�Ԍo�߂ŉԂт���~�炷
						g_aPetals[nCntPetals].pos = D3DXVECTOR3((-TITLE_PETALS_FIELD + (rand() % (TITLE_PETALS_FIELD * 2))) * 1.0f, 200.0f, ((-100 / 2) + (rand() % 100)) * 1.0f);
						g_aPetals[nCntPetals].move = D3DXVECTOR3(rand() % 3 * 0.1f, -rand() % 3 * 0.1f, rand() % 3 * 0.1f);
						g_aPetals[nCntPetals].fRadius = rand() % 3 * 1.0f;
						g_aPetals[nCntPetals].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
						g_aPetals[nCntPetals].angle = D3DXVECTOR3((-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f);
						g_aPetals[nCntPetals].bUse = true;
						g_aPetals[nCntPetals].nType = 0;

						//���_���W�̐ݒ�
						pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
						pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);

						// ���_�J���[
						pVtx[0].col = g_aPetals[nCntPetals].col;
						pVtx[1].col = g_aPetals[nCntPetals].col;
						pVtx[2].col = g_aPetals[nCntPetals].col;
						pVtx[3].col = g_aPetals[nCntPetals].col;

						break;
					}
				}
				break;
			}
			break;

		case MODE_GAME:
			g_nCntPetals = (g_nCntPetals + 1) % PETALS_APPEAR;	// �Ԃт���o���Ԋu

			if (g_aPetals[nCntPetals].bUse == true)
			{// �g���Ă���
				if (g_aPetals[nCntPetals].pos.y <= 0)
				{// �n�ʂɂ��������
					g_aPetals[nCntPetals].bUse = false;
				}

				// ��]������
				g_aPetals[nCntPetals].rot += g_aPetals[nCntPetals].angle;

				g_aPetals[nCntPetals].pos += g_aPetals[nCntPetals].move;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
			}
			else
			{
				if (g_nCntPetals == 0 && pPlayer->pos.x > 2500 && pPlayer->pos.x < PETALS_FIELD * 3 + 2500)
				{// ��莞�Ԍo�߂ōg�t���~�炷
					g_aPetals[nCntPetals].pos = D3DXVECTOR3(((PETALS_FIELD * 3) + (rand() % PETALS_FIELD * 2)) * 1.0f, 100.0f, (-200.0f + (rand() % (200 * 2))) * 1.0f);
					g_aPetals[nCntPetals].move = D3DXVECTOR3(rand() % 3 * 0.1f, -rand() % 4 * 0.1f - 0.1f, rand() % 3 * 0.1f);
					g_aPetals[nCntPetals].fRadius = rand() % 20 * 1.0f;
					g_aPetals[nCntPetals].col = D3DXCOLOR(10.0f, 0.5f, 0.0f, 1.0f);
					g_aPetals[nCntPetals].angle = D3DXVECTOR3((-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f);
					g_aPetals[nCntPetals].nType = 0;
					g_aPetals[nCntPetals].bUse = true;

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);

					// ���_�J���[
					pVtx[0].col = g_aPetals[nCntPetals].col;
					pVtx[1].col = g_aPetals[nCntPetals].col;
					pVtx[2].col = g_aPetals[nCntPetals].col;
					pVtx[3].col = g_aPetals[nCntPetals].col;

					break;
				}
				if (g_nCntPetals == 1 && pPlayer->pos.x < PETALS_FIELD)
				{// ��莞�Ԍo�߂ō����~�炷
					g_aPetals[nCntPetals].pos = D3DXVECTOR3((-PETALS_FIELD + (rand() % (PETALS_FIELD * 2))) * 1.0f, 100.0f, (-200 + (rand() % 400)) * 1.0f);
					g_aPetals[nCntPetals].move = D3DXVECTOR3(rand() % 3 * 0.1f, -rand() % 10 * 0.1f, rand() % 3 * 0.1f);
					g_aPetals[nCntPetals].fRadius = rand() % 10 * 1.0f;
					g_aPetals[nCntPetals].col = D3DXCOLOR(1.0f, 0.5f, 0.8f, 1.0f);
					g_aPetals[nCntPetals].angle = D3DXVECTOR3((-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f);
					g_aPetals[nCntPetals].bUse = true;
					g_aPetals[nCntPetals].nType = 1;

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);

					// ���_�J���[
					pVtx[0].col = g_aPetals[nCntPetals].col;
					pVtx[1].col = g_aPetals[nCntPetals].col;
					pVtx[2].col = g_aPetals[nCntPetals].col;
					pVtx[3].col = g_aPetals[nCntPetals].col;

					break;
				}
			}
			break;
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPetals->Unlock();
}

//=============================================================================
// �|���S���`�揈��
//=============================================================================
void DrawPetals(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	int nCntPetals;

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		// �J�����O�𖳌���

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 220);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (nCntPetals = 0; nCntPetals < MAX_PETALS; nCntPetals++)
	{
		if (g_aPetals[nCntPetals].bUse == true)
		{// �Ԃт炪�g���Ă����
		 // ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPetals[nCntPetals].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPetals[nCntPetals].rot.y, g_aPetals[nCntPetals].rot.x, g_aPetals[nCntPetals].rot.z);
			D3DXMatrixMultiply(&g_aPetals[nCntPetals].mtxWorld, &g_aPetals[nCntPetals].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aPetals[nCntPetals].pos.x, g_aPetals[nCntPetals].pos.y, g_aPetals[nCntPetals].pos.z);
			D3DXMatrixMultiply(&g_aPetals[nCntPetals].mtxWorld, &g_aPetals[nCntPetals].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPetals[nCntPetals].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffPetals, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePetals[g_aPetals[nCntPetals].nType]);

			// �e�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntPetals, 2);
		}
	}

	// Z�o�b�t�@�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// ���ʂ��J�����O
}
