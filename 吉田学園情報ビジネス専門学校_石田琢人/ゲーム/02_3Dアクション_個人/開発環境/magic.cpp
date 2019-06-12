//=============================================================================
//
// ���@�̏��� [magic.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "magic.h"
#include "effect.h"
#include "particle.h"
#include "spiral.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAGIC		"data/TEXTURE/magic.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_MAGIC		(64)
#define ANGLE_MAGIC		(1.0f / (MAGIC_PARTICLE / 2))
#define MAGIC_PARTICLE	(10)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexMagic(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMagic = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagic = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
MAGIC g_aMagic[MAX_MAGIC];							// �ǂ̏��
int g_nAngleMagic;

//=============================================================================
// ����������
//=============================================================================
void InitMagic(void)
{
	int nCntMagic;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MAGIC, &g_pTextureMagic);

	// ���_���̍쐬
	MakeVertexMagic(pDevice);

	// �ǂ̏��̏�����
	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		g_aMagic[nCntMagic].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̏�����
		g_aMagic[nCntMagic].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̏�����
		g_aMagic[nCntMagic].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aMagic[nCntMagic].bUse = false;
	}

	g_nAngleMagic = 0;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMagic(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMagic != NULL)
	{
		g_pTextureMagic->Release();
		g_pTextureMagic = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMagic != NULL)
	{
		g_pVtxBuffMagic->Release();
		g_pVtxBuffMagic = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMagic(void)
{
	for (int nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		if (g_aMagic[nCntMagic].bUse == true)
		{
			switch (g_aMagic[nCntMagic].type)
			{
			case MAGICTYPE_FIRE:
				g_aMagic[nCntMagic].rot.y += 0.02f;
				g_aMagic[nCntMagic].nLife--;

				
				SetParticle(D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + sinf(D3DX_PI * (0.0f + (ANGLE_MAGIC * g_nAngleMagic))) * g_aMagic[nCntMagic].fRadius,
							0.0f,
							g_aMagic[nCntMagic].pos.z + cosf(D3DX_PI * (0.0f + (ANGLE_MAGIC * g_nAngleMagic))) * g_aMagic[nCntMagic].fRadius), 
							g_aMagic[nCntMagic].col, 
							2.0f, 
							100, 
							PARTICLETYPE_NORMAL);

				//SetSpiral(g_aMagic[nCntMagic].pos, g_aMagic[nCntMagic].col, SPIRALTYPE_MAGIC);

				g_nAngleMagic = (g_nAngleMagic + 1) % MAGIC_PARTICLE;

				if (g_aMagic[nCntMagic].nLife <= 0)
				{
					g_aMagic[nCntMagic].bUse = false;
				}

				break;

			case MAGICTYPE_WATER:

				break;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMagic(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	int nCntMagic;

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMagic[nCntMagic].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMagic[nCntMagic].rot.y, g_aMagic[nCntMagic].rot.x, g_aMagic[nCntMagic].rot.z);
		D3DXMatrixMultiply(&g_aMagic[nCntMagic].mtxWorld, &g_aMagic[nCntMagic].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aMagic[nCntMagic].pos.x, g_aMagic[nCntMagic].pos.y, g_aMagic[nCntMagic].pos.z);
		D3DXMatrixMultiply(&g_aMagic[nCntMagic].mtxWorld, &g_aMagic[nCntMagic].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMagic[nCntMagic].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffMagic, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMagic);

		if (g_aMagic[nCntMagic].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMagic, 2);
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

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMagic(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMagic;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MAGIC,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMagic,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		// ���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(-1, 0.0f, 1);
		pVtx[1].pos = D3DXVECTOR3(1, 0.0f, 1);
		pVtx[2].pos = D3DXVECTOR3(-1, 0.0f, 1);
		pVtx[3].pos = D3DXVECTOR3(1, 0.0f, 1);

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

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMagic->Unlock();
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void SetMagic(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, int nCntNumber, MAGICTYPE type)
{
	int nCntMagic, nCntLoop;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntLoop = 0; nCntLoop < nCntNumber; nCntLoop++)
	{
		for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
		{
			if (g_aMagic[nCntMagic].bUse == false)
			{
				g_aMagic[nCntMagic].pos = pos + D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				g_aMagic[nCntMagic].col = col;
				g_aMagic[nCntMagic].fRadius = fRadius;
				g_aMagic[nCntMagic].nLife = nLife;
				g_aMagic[nCntMagic].type = type;

				// ���_�̍��W
				pVtx[0].pos = D3DXVECTOR3(-fRadius, 0.0f, fRadius);
				pVtx[1].pos = D3DXVECTOR3(fRadius, 0.0f, fRadius);
				pVtx[2].pos = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);
				pVtx[3].pos = D3DXVECTOR3(fRadius, 0.0f, -fRadius);

				// ���_�J���[
				pVtx[0].col = col;
				pVtx[1].col = col;
				pVtx[2].col = col;
				pVtx[3].col = col;

				for (int nCntGather = 0; nCntGather < SPIRAL_MAGIC; nCntGather++)
				{
					SetSpiral(g_aMagic[nCntMagic].pos, g_aMagic[nCntMagic].col, SPIRALTYPE_MAGIC);
				}

				g_aMagic[nCntMagic].bUse = true;
				break;
			}
			pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMagic->Unlock();
}
