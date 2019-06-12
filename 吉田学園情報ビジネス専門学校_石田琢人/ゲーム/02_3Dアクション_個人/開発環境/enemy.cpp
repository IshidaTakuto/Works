//=============================================================================
//
// �G���� [enemy.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "shadow.h"
#include "explosion.h"
#include "particle.h"
#include "bullet.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMYNAME		"data/MODEL/airplane000.x"		// �ǂݍ���x�t�@�C����
#define MAX_ENEMY		(64)
#define DAMAGE_TIME		(60)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPD3DXMESH g_pMeshEnemy = NULL;				// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatEnemy = NULL;		// �}�e���A�����ւ̃|�C���^
DWORD g_nNumMatEnemy = 0;					// �}�e���A�����̐�
Enemy g_aEnemy[MAX_ENEMY];					// �G�̏��
int g_nNumEnemy;							// �G�̑���
D3DXVECTOR3 g_vtxMinEnemy, g_vtxMaxEnemy;	// ���_�̍ŏ��l�A�ő�l

//=============================================================================
// ����������
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(ENEMYNAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatEnemy,
		NULL,
		&g_nNumMatEnemy,
		&g_pMeshEnemy);

	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����擾
	nNumVtx = g_pMeshEnemy->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshEnemy->GetFVF());

	// ���_�o�b�t�@�����b�N
	g_pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// ���_���W�̔�r
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

		// X�̒l�̔�r
		if (g_vtxMinEnemy.x > vtx.x)
		{// �ŏ��l�Ɣ�r
			g_vtxMinEnemy.x = vtx.x;	// �ŏ��l��菬������Α��
		}

		if (g_vtxMaxEnemy.x < vtx.x)
		{// �ő�l�Ɣ�r
			g_vtxMaxEnemy.x = vtx.x;	// �ő�l���傫����Α��
		}

		// Y�̒l�̔�r
		if (g_vtxMinEnemy.y > vtx.y)
		{// �ŏ��l�Ɣ�r
			g_vtxMinEnemy.y = vtx.y;	// �ŏ��l��菬������Α��
		}

		if (g_vtxMaxEnemy.y < vtx.y)
		{// �ő�l�Ɣ�r
			g_vtxMaxEnemy.y = vtx.y;	// �ő�l���傫����Α��
		}

		// Z�̒l�̔�r
		if (g_vtxMinEnemy.z > vtx.z)
		{// �ŏ��l�Ɣ�r
			g_vtxMinEnemy.z = vtx.z;	// �ŏ��l��菬������Α��
		}

		if (g_vtxMaxEnemy.z < vtx.z)
		{// �ő�l�Ɣ�r
			g_vtxMaxEnemy.z = vtx.z;	// �ő�l���傫����Α��
		}

		pVtxBuff += sizeFVF;	// �T�C�Y���|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pMeshEnemy->UnlockVertexBuffer();

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G�̏����ݒ�
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].fDestAngle = 0.0f;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].bUse = false;
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	// ���b�V���̊J��
	if (g_pMeshEnemy != NULL)
	{
		g_pMeshEnemy->Release();
		g_pMeshEnemy = NULL;
	}

	// �}�e���A���̊J��
	if (g_pBuffMatEnemy != NULL)
	{
		g_pBuffMatEnemy->Release();
		g_pBuffMatEnemy = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState >= DAMAGE_TIME)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				}
				break;
			}

			if (CollisionBullet(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, g_vtxMinEnemy, g_vtxMaxEnemy) == true)
			{
				HitEnemy(nCntEnemy, 1);
			}
		}
	}


	//// ���X�ɕ�����ς���
	//g_fAngle = g_aEnemy.fDestAngle - g_aEnemy.rot.y;

	//if (g_fAngle < -D3DX_PI)
	//{
	//	g_fAngle += D3DX_PI * 2;
	//}
	//else if (g_fAngle > D3DX_PI)
	//{
	//	g_fAngle -= D3DX_PI * 2;
	//}

	//g_aEnemy.rot.y += g_fAngle * 0.1f;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// �����ɂ��e�̒��_���W
			SetVtxShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos,0.7f);

			// �e�̈ʒu
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy; nCntMat++)
			{

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);

				// �G(�p�[�c)�̕`��
				g_pMeshEnemy->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �G�̐ݒ菈��
//=============================================================================
void SetEnenmy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;
			g_aEnemy[nCntEnemy].nLife = 10;

			// �e������
			g_aEnemy[nCntEnemy].nIdxShadow = SetShadow(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot, 15.0f, 15.0f);
			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
	}
	g_nNumEnemy++;
}

//=============================================================================
// �G�̃_���[�W����
//=============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	g_aEnemy[nCntEnemy].nLife -= nDamage;
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(5.0f, 0.0f, 0.0f, 10.0f), (rand() % 3) * 1.5f, 60, PARTICLETYPE_BLOOD);
		g_aEnemy[nCntEnemy].bUse = false;
		g_nNumEnemy--;
		DeleteShadow(g_aEnemy[nCntEnemy].nIdxShadow);
		if (g_nNumEnemy <= 0)
		{//�G���S�ł�����
			SetGameState(GAMESTATE_CLEAR);
		}
	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
	}
}

//=============================================================================
// �G�̏��̎擾
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}