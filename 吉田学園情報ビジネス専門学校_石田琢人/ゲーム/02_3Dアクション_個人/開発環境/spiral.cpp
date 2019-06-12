//=============================================================================
//
// �����̏��� [spiral.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "spiral.h"
#include "effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SPIRAL		"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define SORT_TEXTURE		(1)
#define	MAX_SPIRAL			(256)							// �G�t�F�N�g�̍ő吔

#define ANGLE_RING			(1.0f / (SPIRAL_RING / 2))
#define ANGLE_POWERUP		(1.0f / (SPIRAL_POWERUP / 2))
#define ANGLE_MAGIC			(1.0f / (SPIRAL_MAGIC / 2))

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureSpiral = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpiral = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpiralEmitter = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Spiral					g_aSpiral[MAX_SPIRAL];		// �G�t�F�N�g�̏��
int g_nCntMoveAngle;
float g_fSpiralHeight;

//=============================================================================
// ����������
//=============================================================================
void InitSpiral(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntSpiral;

	for (nCntSpiral = 0; nCntSpiral < MAX_SPIRAL; nCntSpiral++)
	{
		g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSpiral[nCntSpiral].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSpiral[nCntSpiral].type = SPIRALTYPE_NONE;
		g_aSpiral[nCntSpiral].fRadius = 0.0f;
		g_aSpiral[nCntSpiral].bUse = false;
	}

	g_nCntMoveAngle = 0;
	g_aSpiral[nCntSpiral].fRadius = 20.0f;
	g_fSpiralHeight = 0.0f;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SPIRAL, &g_pTextureSpiral);

	// ���_���̍쐬
	MakeVertexSpiral(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitSpiral(void)
{
		// �e�N�X�`���̊J��
		if (g_pTextureSpiral != NULL)
		{
			g_pTextureSpiral->Release();
			g_pTextureSpiral = NULL;
		}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffSpiral != NULL)
	{
		g_pVtxBuffSpiral->Release();
		g_pVtxBuffSpiral = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSpiral(void)
{
	int nCntSpiral;
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSpiral->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSpiral = 0; nCntSpiral < MAX_SPIRAL; nCntSpiral++)
	{
		if (g_aSpiral[nCntSpiral].bUse == true)
		{
			switch (g_aSpiral[nCntSpiral].type)
			{
			case SPIRALTYPE_RING:
				g_aSpiral[nCntSpiral].move = D3DXVECTOR3(0.0f, sinf(D3DX_PI * (0.0f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * 2.0f, cosf(D3DX_PI * (0.0f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * 2.0f);
				g_aSpiral[nCntSpiral].nCntAngle = (g_aSpiral[nCntSpiral].nCntAngle + 1) % SPIRAL_RING;

				g_aSpiral[nCntSpiral].pos += g_aSpiral[nCntSpiral].move;

				SetEffect(g_aSpiral[nCntSpiral].pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.1f), 3.0f, 5, EFFECTSTATE_GATHER);
				break;

			case SPIRALTYPE_POWERUP:
				if (nCntSpiral % 2 == 0)
				{
					g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].PlayerPos.x + sinf(D3DX_PI * (0.5f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius, 20.0f, g_aSpiral[nCntSpiral].PlayerPos.z + cosf(D3DX_PI * (0.5f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius);
				}
				else
				{
					g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].PlayerPos.x + -sinf(D3DX_PI * (0.5f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius, 20.0f, g_aSpiral[nCntSpiral].PlayerPos.z + -cosf(D3DX_PI * (0.5f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius);
				}
				//g_aSpiral[nCntSpiral].move.y += 0.5f;
				g_aSpiral[nCntSpiral].nCntAngle = (g_aSpiral[nCntSpiral].nCntAngle + 1) % SPIRAL_RING;

				g_aSpiral[nCntSpiral].pos += g_aSpiral[nCntSpiral].move;

				//g_aSpiral[nCntSpiral].fRadius -= 0.25f;

				SetEffect(g_aSpiral[nCntSpiral].pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2.0f, 10, EFFECTSTATE_GATHER);

				if (g_aSpiral[nCntSpiral].fRadius <= 0)
				{
					g_aSpiral[nCntSpiral].bUse = false;
				}
				break;

			case SPIRALTYPE_MAGIC:
				g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].PlayerPos.x + sinf(D3DX_PI * (0.0f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius, g_aSpiral[nCntSpiral].fHeight, g_aSpiral[nCntSpiral].PlayerPos.z + cosf(D3DX_PI * (0.0f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius);
				g_aSpiral[nCntSpiral].nCntAngle = (g_aSpiral[nCntSpiral].nCntAngle + 1) % SPIRAL_RING;
				g_aSpiral[nCntSpiral].fRadius += 0.2f;
				g_aSpiral[nCntSpiral].fSize += 0.02f;
				g_aSpiral[nCntSpiral].fHeight += 0.2f;

				SetEffect(g_aSpiral[nCntSpiral].pos + D3DXVECTOR3(0.0f,2.0f,0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.1f), 2.0f, 20, EFFECTSTATE_GATHER);

				if (g_aSpiral[nCntSpiral].fHeight >= 30)
				{
					g_aSpiral[nCntSpiral].bUse = false;
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);

				break;

			}
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSpiral->Unlock();
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawSpiral(void)
{
	int nCntSpiral;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X

												// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (nCntSpiral = 0; nCntSpiral < MAX_SPIRAL; nCntSpiral++)
	{
		if (g_aSpiral[nCntSpiral].bUse == true)	//�e���g�p����Ă���
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aSpiral[nCntSpiral].mtxWorld);

			// ��]�𔽉f
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			g_aSpiral[nCntSpiral].mtxWorld._11 = mtxView._11;
			g_aSpiral[nCntSpiral].mtxWorld._12 = mtxView._21;
			g_aSpiral[nCntSpiral].mtxWorld._13 = mtxView._31;
			g_aSpiral[nCntSpiral].mtxWorld._21 = mtxView._12;
			g_aSpiral[nCntSpiral].mtxWorld._22 = mtxView._22;
			g_aSpiral[nCntSpiral].mtxWorld._23 = mtxView._32;
			g_aSpiral[nCntSpiral].mtxWorld._31 = mtxView._13;
			g_aSpiral[nCntSpiral].mtxWorld._32 = mtxView._23;
			g_aSpiral[nCntSpiral].mtxWorld._33 = mtxView._33;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aSpiral[nCntSpiral].pos.x, g_aSpiral[nCntSpiral].pos.y, g_aSpiral[nCntSpiral].pos.z);
			D3DXMatrixMultiply(&g_aSpiral[nCntSpiral].mtxWorld, &g_aSpiral[nCntSpiral].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aSpiral[nCntSpiral].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffSpiral, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureSpiral);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSpiral, 2);
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
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexSpiral(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntSpiral;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SPIRAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSpiral,
		NULL);

	// ���_�o�b�t�@�̏���ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSpiral->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSpiral = 0; nCntSpiral < MAX_SPIRAL; nCntSpiral++)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-1, 1, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1, 1, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-1, -1, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1, -1, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	g_pVtxBuffSpiral->Unlock();
}


//=============================================================================
// �ݒ�
//=============================================================================
void SetSpiral(D3DXVECTOR3 pos, D3DXCOLOR col, SPIRALTYPE type)
{
	int nCntSpiral;

	// ���_�o�b�t�@�̏���ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSpiral->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSpiral = 0; nCntSpiral < MAX_SPIRAL; nCntSpiral++)
	{
		if (g_aSpiral[nCntSpiral].bUse == false)
		{
			switch (type)
			{
			case SPIRALTYPE_RING:
				g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(pos.x, pos.y - sinf(D3DX_PI * (0.5f + (ANGLE_RING * g_nCntMoveAngle))) * 20.0f, pos.z - cosf(D3DX_PI * (0.5f + (ANGLE_RING * g_nCntMoveAngle))) * 20.0f);
				g_aSpiral[nCntSpiral].col = col;
				g_aSpiral[nCntSpiral].PlayerPos = pos;
				g_aSpiral[nCntSpiral].type = type;
				g_aSpiral[nCntSpiral].fSize = 2.0f;

				g_aSpiral[nCntSpiral].nCntAngle = (g_nCntMoveAngle + 1) % SPIRAL_RING;
				g_nCntMoveAngle = g_aSpiral[nCntSpiral].nCntAngle;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);

				break;

			case SPIRALTYPE_POWERUP:
				//g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(pos.x - sinf(D3DX_PI * (0.5f + (ANGLE_RING * g_nCntMoveAngle))) * 20.0f, 0.0f, pos.z - cosf(D3DX_PI * (0.5f + (ANGLE_RING * g_nCntMoveAngle))) * 20.0f);
				g_aSpiral[nCntSpiral].pos = pos;
				g_aSpiral[nCntSpiral].move = D3DXVECTOR3(0.0f,0.0f,0.0f);
				g_aSpiral[nCntSpiral].col = col;
				g_aSpiral[nCntSpiral].PlayerPos = pos;
				g_aSpiral[nCntSpiral].type = type;
				g_aSpiral[nCntSpiral].fRadius = 20.0f;
				g_aSpiral[nCntSpiral].fSize = 1.5f;
				g_aSpiral[nCntSpiral].nCntAngle = (g_nCntMoveAngle + 1 * (SPIRAL_RING / SPIRAL_POWERUP)) % SPIRAL_RING;
				g_nCntMoveAngle = g_aSpiral[nCntSpiral].nCntAngle;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);

				break;

			case SPIRALTYPE_MAGIC:
				g_aSpiral[nCntSpiral].pos = pos;
				g_aSpiral[nCntSpiral].col = col;
				g_aSpiral[nCntSpiral].PlayerPos = pos;
				g_aSpiral[nCntSpiral].type = type;
				g_aSpiral[nCntSpiral].fRadius = 0.0f;
				g_aSpiral[nCntSpiral].fSize = 1.5f;
				g_aSpiral[nCntSpiral].fHeight = 0.0f;
				g_aSpiral[nCntSpiral].nCntAngle = (g_nCntMoveAngle + 1 * (SPIRAL_RING / SPIRAL_MAGIC)) % SPIRAL_RING;
				g_nCntMoveAngle = g_aSpiral[nCntSpiral].nCntAngle;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);

				break;

			}

			//���_�J���[
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aSpiral[nCntSpiral].bUse = true;
			break;
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSpiral->Unlock();
}
