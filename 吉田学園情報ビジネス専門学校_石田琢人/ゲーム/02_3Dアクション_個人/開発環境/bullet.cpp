//=============================================================================
//
// �e���� [bullet.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "bullet.h"
#include "shadow.h"
#include "input.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_BULLET		"data/TEXTURE/bullet000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_BULLET		(128)
#define BULLET_LIMIT	(150)
#define BULLET_SPEED	(1.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];						// �ǂ̏��

//=============================================================================
// ����������
//=============================================================================
void InitBullet(void)
{
	int nCntBullet;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &g_pTextureBullet);

	// ���_���̍쐬
	MakeVertexBullet(pDevice);

	// �ǂ̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̏�����
		g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̏�����
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̏�����
		g_aBullet[nCntBullet].fHeight = 0.0f;
		g_aBullet[nCntBullet].fWidth = 0.0f;
		g_aBullet[nCntBullet].bUse = false;
	}

}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	int nCntBullet;

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//�G�t�F�N�g
			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 0.7f), 5.0f, 60, EFFECTSTATE_BULLET);

			// �r���{�[�h�̈ړ�
			g_aBullet[nCntBullet].pos.x -= sinf((D3DX_PI * 0.0f) + g_aBullet[nCntBullet].rot.y) * BULLET_SPEED;
			g_aBullet[nCntBullet].pos.z -= cosf((D3DX_PI * 0.0f) + g_aBullet[nCntBullet].rot.y) * BULLET_SPEED;

			if (g_aBullet[nCntBullet].pos.x < -BULLET_LIMIT
				|| g_aBullet[nCntBullet].pos.x > BULLET_LIMIT
				|| g_aBullet[nCntBullet].pos.z < -BULLET_LIMIT
				|| g_aBullet[nCntBullet].pos.z > BULLET_LIMIT)
			{// �r���{�[�h�폜
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0));	// ����������
				//SetParticle(g_aBullet[nCntBullet].pos,
				//	D3DXCOLOR((rand() % 10) * 0.1f, (rand() % 10) * 0.1f, (rand() % 10) * 0.1f, 1.0f),
				//	(rand() % 3) * 1.5f,
				//	15,
				//	PARTICLETYPE_SPARK);				
				g_aBullet[nCntBullet].bUse = false;					// �g�p���Ă��Ȃ���Ԃɂ���
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);		// �e������
			}


			// �e�̒��_���W
			SetVtxShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos, 0.7f);

			// �e�̈ʒu
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	int nCntBullet;

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

		// ��]�𔽉f
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		g_aBullet[nCntBullet].mtxWorld._11 = mtxView._11;
		g_aBullet[nCntBullet].mtxWorld._12 = mtxView._21;
		g_aBullet[nCntBullet].mtxWorld._13 = mtxView._31;
		g_aBullet[nCntBullet].mtxWorld._21 = mtxView._12;
		g_aBullet[nCntBullet].mtxWorld._22 = mtxView._22;
		g_aBullet[nCntBullet].mtxWorld._23 = mtxView._32;
		g_aBullet[nCntBullet].mtxWorld._31 = mtxView._13;
		g_aBullet[nCntBullet].mtxWorld._32 = mtxView._23;
		g_aBullet[nCntBullet].mtxWorld._33 = mtxView._33;

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
		D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBullet);

		if (g_aBullet[nCntBullet].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);
		}
	}

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntBullet;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// ���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, 0.0f, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 rot)
{
	int nCntBullet;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].fWidth = fWidth;
			g_aBullet[nCntBullet].fHeight = fHeight;
			g_aBullet[nCntBullet].rot = rot;

			// ���_�̍��W
			pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, 0.0f, 0.0f);

			g_aBullet[nCntBullet].bUse = true;

			// �e������
			g_aBullet[nCntBullet].nIdxShadow = SetShadow(g_aBullet[nCntBullet].pos, 
															 g_aBullet[nCntBullet].rot, 
															 g_aBullet[nCntBullet].fWidth, 
															 g_aBullet[nCntBullet].fHeight);

			break;
		}
		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// �����蔻��̏���
//=============================================================================
bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax)
{
	bool bHit = false;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (pPos->x + vtxMin.x < g_aBullet[nCntBullet].fWidth + g_aBullet[nCntBullet].pos.x && pPos->x + vtxMax.x > g_aBullet[nCntBullet].pos.x  - g_aBullet[nCntBullet].fWidth &&
				pPos->y + vtxMin.y < g_aBullet[nCntBullet].fHeight + g_aBullet[nCntBullet].pos.y && pPos->y + vtxMax.y > g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight &&
				pPos->z + vtxMin.z < g_aBullet[nCntBullet].fHeight + g_aBullet[nCntBullet].pos.z && pPos->z + vtxMax.z > g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fHeight)
			{// �e�͈̔�
				bHit = true;
			}
		}
	}
	return bHit;
}