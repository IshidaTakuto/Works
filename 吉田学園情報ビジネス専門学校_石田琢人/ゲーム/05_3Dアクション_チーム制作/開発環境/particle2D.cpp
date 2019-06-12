//=============================================================================
//
// �p�[�e�B�N������ [particle2D.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "particle2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PARTICLE_TEXTURENAME	"data/TEXTURE/blood0.jpg"

#define MAX_EMITTER		(16)		// �G�~�b�^�̍ő吔
#define MAX_PARTICLE	(500)		// �p�[�e�B�N���̍ő吔
#define PARTICLE_APPEAR	(2)			// �p�[�e�B�N���̏o��Ԋu
#define NORMAL_TIME		(15)

#define WIDTH_MOVE		(1)		// X���ړ���
#define HEIGHT_MOVE		(1)		// Y���ړ���
#define SPREAD			(0.7f)	// �L����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexParticle2D(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureParticle2D = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle2D = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Particle2D g_aParticle2D[MAX_PARTICLE];
Emitter g_aEmitter2D[MAX_EMITTER];
int g_nCntParticle2D;
int g_nCntAngle2D;

//=============================================================================
// �|���S������������
//=============================================================================
void InitParticle2D(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �p�[�e�B�N���̏���������
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle2D[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2D[nCntParticle].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2D[nCntParticle].fRadius = 0;
		g_aParticle2D[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle2D[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2D[nCntParticle].type = PARTICLETYPE_NONE;
		g_aParticle2D[nCntParticle].nLife = 0;
		g_aParticle2D[nCntParticle].bUse = false;
	}

	// �G�~�b�^�̏���������
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		g_aEmitter2D[nCntEmitter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEmitter2D[nCntEmitter].fRadius = 0;
		g_aEmitter2D[nCntEmitter].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEmitter2D[nCntEmitter].type = PARTICLETYPE_NONE;
		g_aEmitter2D[nCntEmitter].bUse = false;
	}

	g_nCntParticle2D = 0;
	g_nCntAngle2D = 0;

	// �p�[�e�B�N���̒��_���̍쐬
	MakeVertexParticle2D(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURENAME, &g_pTextureParticle2D);
}

//=============================================================================
// �|���S���I������
//=============================================================================
void UninitParticle2D(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureParticle2D != NULL)
	{
		g_pTextureParticle2D->Release();
		g_pTextureParticle2D = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffParticle2D != NULL)
	{
		g_pVtxBuffParticle2D->Release();
		g_pVtxBuffParticle2D = NULL;
	}
}

//=============================================================================
// �|���S���X�V����
//=============================================================================
void UpdateParticle2D(void)
{
	int nCntParticle;	// for���p�̃J�E���^

	// �o��������Ԋu�̃J�E���g
	g_nCntParticle2D = (g_nCntParticle2D + 1) % PARTICLE_APPEAR;

	if (g_nCntParticle2D == 0)
	{// �o��������
		for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
		{
			if (g_aEmitter2D[nCntEmitter].bUse == true)
			{// �g�p����Ă���G�~�b�^

				g_aEmitter2D[nCntEmitter].nLife--;	// �G�~�b�^�̕\�����Ԃ��J�E���g

				if (g_aEmitter2D[nCntEmitter].nLife <= 0)
				{// ��莞�Ԍo�߂ŃG�~�b�^������
					g_aEmitter2D[nCntEmitter].bUse = false;
				}

				for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
				{// �p�[�e�B�N��������
					if (g_aParticle2D[nCntParticle].bUse == false)
					{// �g���Ă��Ȃ�
						g_aParticle2D[nCntParticle].pos = g_aEmitter2D[nCntEmitter].pos;
						g_aParticle2D[nCntParticle].fAngle = 1.0f / (13 / 2);
						if (nCntParticle % 2 == 0)
						{
							g_aParticle2D[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle2D[nCntParticle].fAngle * g_nCntAngle2D))) * (WIDTH_MOVE + ((rand() % 10) * 0.1f)) * SPREAD,
								cosf(D3DX_PI * (0.0f + (g_aParticle2D[nCntParticle].fAngle * g_nCntAngle2D))) * (HEIGHT_MOVE + ((rand() % 10) * 0.1f)) * SPREAD,
								0.0f);
						}
						else
						{
							g_aParticle2D[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (1.0f + (g_aParticle2D[nCntParticle].fAngle * g_nCntAngle2D))) * (WIDTH_MOVE + ((rand() % 10) * 0.1f)) * SPREAD,
								cosf(D3DX_PI * (1.0f + (g_aParticle2D[nCntParticle].fAngle * g_nCntAngle2D))) * (HEIGHT_MOVE + ((rand() % 10) * 0.1f)) * SPREAD,
								0.0f);
						}

						g_aParticle2D[nCntParticle].nIdx = nCntEmitter;
						g_aParticle2D[nCntParticle].bUse = true;
						g_aParticle2D[nCntParticle].nLife = NORMAL_TIME;
						g_aParticle2D[nCntParticle].col = g_aEmitter2D[nCntEmitter].col;
						g_aParticle2D[nCntParticle].fRadius = g_aEmitter2D[nCntEmitter].fRadius;

						g_nCntAngle2D = (g_nCntAngle2D + 1) % 13;
						break;
					}
				}
			}
		}
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �p�[�e�B�N���̍X�V
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffParticle2D->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle2D[nCntParticle].bUse == true)
		{// �g���Ă���
			g_aParticle2D[nCntParticle].nLife--;		// �\�����Ԃ��J�E���g

			// �\�����Ԃ�0�ȉ��ɂȂ���
			if (g_aParticle2D[nCntParticle].nLife <= 0)
			{// �g�p���Ă��Ȃ���Ԃɂ���
				g_aParticle2D[nCntParticle].bUse = false;
			}

			g_aParticle2D[nCntParticle].pos += g_aParticle2D[nCntParticle].move;					// �ʒu�̍X�V
			g_aParticle2D[nCntParticle].col += D3DXCOLOR(0.01f, 0.01f, 0.01f, 0.0f);				// �F�̍X�V
			g_aParticle2D[nCntParticle].fRadius += 0.01f;		// �傫���̍X�V

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle2D[nCntParticle].fRadius, -g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aParticle2D[nCntParticle].fRadius, -g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle2D[nCntParticle].fRadius, g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aParticle2D[nCntParticle].fRadius, g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;

			//���_�J���[
			pVtx[0].col = g_aParticle2D[nCntParticle].col;
			pVtx[1].col = g_aParticle2D[nCntParticle].col;
			pVtx[2].col = g_aParticle2D[nCntParticle].col;
			pVtx[3].col = g_aParticle2D[nCntParticle].col;
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffParticle2D->Unlock();
}

//=============================================================================
// �|���S���`�揈��
//=============================================================================
void DrawParticle2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	int nCntParticle;

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffParticle2D, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureParticle2D);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �|���S���̕`��
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle2D[nCntParticle].bUse == true)	//�g�p����Ă���
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// �p�[�e�B�N���̐ݒ�
//=============================================================================
void SetParticle2D(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius,int nLife, PARTICLETYPE type)
{
	int nCntEmitter;

	for (nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter2D[nCntEmitter].bUse == false)
		{
			g_aEmitter2D[nCntEmitter].pos = pos;
			g_aEmitter2D[nCntEmitter].col = col;
			g_aEmitter2D[nCntEmitter].fRadius = fRadius;
			g_aEmitter2D[nCntEmitter].nLife = nLife;
			g_aEmitter2D[nCntEmitter].type = type;

			g_aEmitter2D[nCntEmitter].bUse = true;
			break;
		}
	}
}

//=============================================================================
// �p�[�e�B�N���̒��_���̍쐬
//=============================================================================
void MakeVertexParticle2D(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle2D,
		NULL);

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffParticle2D->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle2D[nCntParticle].fRadius, -g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aParticle2D[nCntParticle].fRadius, -g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle2D[nCntParticle].fRadius, g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aParticle2D[nCntParticle].fRadius, g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;

		// �@���̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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
	g_pVtxBuffParticle2D->Unlock();
}
