//=============================================================================
//
// ���̏��� [Blood.cpp]
// Author : KOJI INAI
//
//=============================================================================
#include "Blood.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BLOOD_MAX			(64)							// �e�̍ő吔
#define	BLOOD_TEX			"data/TEXTURE/Blood.png"		// ���f���̖��O

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexBlood(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �e�̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ��
	//float fHeight;					// ����
	//float fWide;					// ��
	D3DXMATRIX mtxWorld;			// ���[���h�}�g���b�N�X
	bool bUse;						// �g�p���Ă��邩�ǂ���
}Blood;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBlood = NULL;					//�e�N�X�`���փ|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlood = NULL;			//���_�o�b�t�@�ւ̃|�C���^
Blood g_aBlood[BLOOD_MAX];								// �e�̍\���̂ւ̃|�C���^
float g_fHeightBlood;

//=============================================================================
// ����������
//=============================================================================
void InitBlood(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	//g_nWBlood = 0.000001f;
	g_fHeightBlood = 0.0f;
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BLOOD_TEX, &g_pTextureBlood);

	// ���_���̍쐬
	MakeVertexBlood(pDevice);

	for (int nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	{
		g_aBlood[nCntBlood].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlood[nCntBlood].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlood[nCntBlood].bUse = false;
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitBlood(void)
{
	// �e�N�X�`���̊J��;
	if (g_pTextureBlood != NULL)
	{
		g_pTextureBlood->Release();
		g_pTextureBlood = NULL;
	}

	// ���_�o�b�t�@�̊J��;
	if (g_pVtxBuffBlood != NULL)
	{
		g_pVtxBuffBlood->Release();
		g_pVtxBuffBlood = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBlood(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBlood(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	for (int nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aBlood[nCntBlood].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBlood[nCntBlood].rot.y, g_aBlood[nCntBlood].rot.x, g_aBlood[nCntBlood].rot.z);
		D3DXMatrixMultiply(&g_aBlood[nCntBlood].mtxWorld, &g_aBlood[nCntBlood].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aBlood[nCntBlood].pos.x, g_aBlood[nCntBlood].pos.y, g_aBlood[nCntBlood].pos.z);
		D3DXMatrixMultiply(&g_aBlood[nCntBlood].mtxWorld, &g_aBlood[nCntBlood].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aBlood[nCntBlood].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffBlood, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBlood);

		if (g_aBlood[nCntBlood].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlood * 4, 2);
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexBlood(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * BLOOD_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlood,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾6
	g_pVtxBuffBlood->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	{
		//���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-40.0f,0.0f, 40.0f);		//���_���W(X���W,Y���W)
		pVtx[1].pos = D3DXVECTOR3(40.0f, 0.0f, 40.0f);		//���_���W(X���W,Y���W)
		pVtx[2].pos = D3DXVECTOR3(-40.0f,0.0f, -40.0f);		//���_���W(X���W,Y���W)
		pVtx[3].pos = D3DXVECTOR3(40.0f, 0.0f, -40.0f);		//���_���W(X���W,Y���W)

		//�@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f);  //�F�̎w��i0.0f �` 1.0f�j&�����x�i0.0f�@�`�@1.0f�j
		pVtx[1].col = D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f);  //�F�̎w��i0.0f �` 1.0f�j&�����x�i0.0f�@�`�@1.0f)
		pVtx[2].col = D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f);  //�F�̎w��i0.0f �` 1.0f�j&�����x�i0.0f�@�`�@1.0f�j
		pVtx[3].col = D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f);  //�F�̎w��i0.0f �` 1.0f�j&�����x�i0.0f�@�`�@1.0f�j

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		//U�����AV�������@(0.0f�`1.0f�j
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);		//U�����AV�������@(0.0f�`1.0f�j
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		//U�����AV�������@(0.0f�`1.0f�j
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);		//U�����AV�������@(0.0f�`1.0f�j

		pVtx += 4;

	}
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBlood->Unlock();
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
int SetBlood(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWide, D3DXCOLOR col)
{
	int nCntBlood;

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾6
	g_pVtxBuffBlood->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	{
		if (g_aBlood[nCntBlood].bUse == false)
		{
			g_fHeightBlood += 0.0001f;

			g_aBlood[nCntBlood].pos = pos;
			g_aBlood[nCntBlood].rot = rot;
			g_aBlood[nCntBlood].bUse = true;

			//���_���̐ݒ�
			//pVtx[0].pos = D3DXVECTOR3(-fWide, g_fHeightBlood, fHeight);		//���_���W(X���W,Y���W)
			//pVtx[1].pos = D3DXVECTOR3(fWide, g_fHeightBlood, fHeight);		//���_���W(X���W,Y���W)
			//pVtx[2].pos = D3DXVECTOR3(-fWide, g_fHeightBlood, -fHeight);		//���_���W(X���W,Y���W)
			//pVtx[3].pos = D3DXVECTOR3(fWide, g_fHeightBlood, -fHeight);		//���_���W(X���W,Y���W)

			//���_�J���[�̐ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			break;
		}
		pVtx += 4;

	}
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBlood->Unlock();

	//for (nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	//{
	//	if (g_aBlood[nCntBlood].bUse == true)
	//	{
	//		float nPosBlood = 0.000001f;
	//		g_aBlood[nCntBlood].pos.y += nPosBlood;
	//	}
	//}
	return nCntBlood;
}

////=============================================================================
//// �e�̐ݒ�
////=============================================================================
//void SetPositionBlood(int nIdyShadow, D3DXVECTOR3 pos)
//{
//	g_aBlood[nIdyShadow].pos = pos;
//}
//=============================================================================
// ���f���̎擾
//=============================================================================
Blood *GetBlood(void)
{
	return &g_aBlood[0];
}
//
//
//
void DeleteBlood(void)
{
	for (int nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	{
		g_aBlood[nCntBlood].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlood[nCntBlood].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlood[nCntBlood].bUse = false;
	}
}
