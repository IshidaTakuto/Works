//=============================================================================
//
// �������� [explosion.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "explosion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_EXPLOSION	(128)	// �����̍ő吔
#define EXPLOSION_TEXTURENAME	"data/TEXTURE/explosion000.png"
#define UPDATE_ANIM ((g_aExplosion[nCntExplosion].nPatternAnim % 8) * 0.125f)	//�A�j���[�V�����p�^�[��
#define TEX_PATTERN	(8)
#define WIDTH_SIZE	(15.0f)
#define HEIGHT_SIZE	(15.0f)

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];	//�����̏��

//=============================================================================
// ����������
//=============================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �����̏��̏�����
	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].nCountAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TEXTURENAME, &g_pTextureExplosion);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-WIDTH_SIZE, HEIGHT_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(WIDTH_SIZE, HEIGHT_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-WIDTH_SIZE, -HEIGHT_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(WIDTH_SIZE, -HEIGHT_SIZE, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateExplosion(void)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^
	int nCntExplosion;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].nCountAnim++;

		if ((g_aExplosion[nCntExplosion].nCountAnim % 5) == 0)
		{
			if (g_aExplosion[nCntExplosion].bUse == true)	
			{//�������g�p����Ă���
				//�p�^�[��No.�̍X�V
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 8;

				//�e�N�X�`�����W���X�V
				pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 1.0f);

				if (g_aExplosion[nCntExplosion].nPatternAnim == 0)
				{//�p�^�[�����Ō�܂ŏI�����
					g_aExplosion[nCntExplosion].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
				}
			}
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	int nCntExplosion;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 120);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)	//�e���g�p����Ă���
		{

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtxWorld);

			// ��]�𔽉f
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			g_aExplosion[nCntExplosion].mtxWorld._11 = mtxView._11;
			g_aExplosion[nCntExplosion].mtxWorld._12 = mtxView._21;
			g_aExplosion[nCntExplosion].mtxWorld._13 = mtxView._31;
			g_aExplosion[nCntExplosion].mtxWorld._21 = mtxView._12;
			g_aExplosion[nCntExplosion].mtxWorld._22 = mtxView._22;
			g_aExplosion[nCntExplosion].mtxWorld._23 = mtxView._32;
			g_aExplosion[nCntExplosion].mtxWorld._31 = mtxView._13;
			g_aExplosion[nCntExplosion].mtxWorld._32 = mtxView._23;
			g_aExplosion[nCntExplosion].mtxWorld._33 = mtxView._33;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntExplosion, 2);
		}
	}

	// Z�o�b�t�@�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=============================================================================
// �����̐ݒ菈��
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^
	int nCntExplosion;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)	//�e���g�p����Ă��Ȃ�
		{
			g_aExplosion[nCntExplosion].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-WIDTH_SIZE, HEIGHT_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(WIDTH_SIZE, HEIGHT_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-WIDTH_SIZE, -HEIGHT_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(WIDTH_SIZE, -HEIGHT_SIZE, 0.0f);

			g_aExplosion[nCntExplosion].col = col;

			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aExplosion[nCntExplosion].nCountAnim = 0;

			g_aExplosion[nCntExplosion].nPatternAnim = 0;

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f * g_aExplosion[nCntExplosion].nPatternAnim), 1.0f);

			g_aExplosion[nCntExplosion].bUse = true;	//�g�p���Ă����Ԃɂ���
			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}