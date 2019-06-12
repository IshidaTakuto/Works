//=============================================================================
//
// �G�t�F�N�g�̏��� [effect.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EFFECT0		"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_EFFECT1		"data/TEXTURE/effect001.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_EFFECT2		"data/TEXTURE/moon.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_EFFECT3		"data/TEXTURE/spark.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_EFFECT4		"data/TEXTURE/flower.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_EFFECT5		"data/TEXTURE/ripples.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����

#define	MAX_EFFECT		(5096)							// �G�t�F�N�g�̍ő吔
#define MAX_TEXTURE		(6)

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect[MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
EFFECT					g_aEffect[MAX_EFFECT];		// �G�t�F�N�g�̏��

//=============================================================================
// ����������
//=============================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect;

	for(nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
		g_aEffect[nCntEffect].nType = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT0, &g_pTextureEffect[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT1, &g_pTextureEffect[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT2, &g_pTextureEffect[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT3, &g_pTextureEffect[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT4, &g_pTextureEffect[4]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT5, &g_pTextureEffect[5]);

	// ���_���̍쐬
	MakeVertexEffect(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	int nCntEffect;

	for (nCntEffect = 0; nCntEffect < MAX_TEXTURE; nCntEffect++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureEffect[nCntEffect] != NULL)
		{
			g_pTextureEffect[nCntEffect]->Release();
			g_pTextureEffect[nCntEffect] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	int nCntEffect;
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		switch (g_aEffect[nCntEffect].state)
		{
		case EFFECTSTATE_NONE:
			g_aEffect[nCntEffect].nType = 0;
			break;

		case EFFECTSTATE_BULLET:
			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].fRadius -= 0.3f;
			g_aEffect[nCntEffect].nType = 0;

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_RAIN:
			g_aEffect[nCntEffect].nType = 0;
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_HIPFIRE:
			g_aEffect[nCntEffect].fRadius -= 0.3f;
			g_aEffect[nCntEffect].nType = 0;

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_SPARK:
			g_aEffect[nCntEffect].fRadius -= 0.05f;
			g_aEffect[nCntEffect].nType = 1;

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_BLOOD:
			g_aEffect[nCntEffect].nLife--;
				g_aEffect[nCntEffect].fRadius -= 0.08f;

			g_aEffect[nCntEffect].nType = 3;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_MOON:
			g_aEffect[nCntEffect].nType = 2;
			break;

		case EFFECTSTATE_GATHER:
			g_aEffect[nCntEffect].fRadius -= 0.1f;
			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].nType = 0;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_FOUNTAIN:
			g_aEffect[nCntEffect].fRadius -= 0.05f;
			g_aEffect[nCntEffect].nType = 0;

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_RIPPLES:
			g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEffect[nCntEffect].fRadius += 2.0f;
			g_aEffect[nCntEffect].col.a -= 0.05f;
			g_aEffect[nCntEffect].nType = 5;

			if (g_aEffect[nCntEffect].col.a <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, 0.0f, g_aEffect[nCntEffect].fRadius);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, 0.0f, g_aEffect[nCntEffect].fRadius);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, 0.0f, -g_aEffect[nCntEffect].fRadius);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, 0.0f, -g_aEffect[nCntEffect].fRadius);

			//���_�J���[
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			break;
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	int nCntEffect;
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

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)	//�e���g�p����Ă���
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			if (g_aEffect[nCntEffect].state != EFFECTSTATE_RIPPLES)
			{
				// ��]�𔽉f
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);
				g_aEffect[nCntEffect].mtxWorld._11 = mtxView._11;
				g_aEffect[nCntEffect].mtxWorld._12 = mtxView._21;
				g_aEffect[nCntEffect].mtxWorld._13 = mtxView._31;
				g_aEffect[nCntEffect].mtxWorld._21 = mtxView._12;
				g_aEffect[nCntEffect].mtxWorld._22 = mtxView._22;
				g_aEffect[nCntEffect].mtxWorld._23 = mtxView._32;
				g_aEffect[nCntEffect].mtxWorld._31 = mtxView._13;
				g_aEffect[nCntEffect].mtxWorld._32 = mtxView._23;
				g_aEffect[nCntEffect].mtxWorld._33 = mtxView._33;
			}
			else
			{
				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffect[nCntEffect].rot.y, g_aEffect[nCntEffect].rot.x, g_aEffect[nCntEffect].rot.z);
				D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxRot);
			}

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEffect, 2);
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
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntEffect;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	// ���_�o�b�t�@�̏���ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

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
	g_pVtxBuffEffect->Unlock();
}


//=============================================================================
// �G�t�F�N�g�ݒ�
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, EFFECTSTATE state)
{
	int nCntEffect;

	// ���_�o�b�t�@�̏���ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].state = state;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			//���_�J���[
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aEffect[nCntEffect].bUse = true;
			break;

		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}
