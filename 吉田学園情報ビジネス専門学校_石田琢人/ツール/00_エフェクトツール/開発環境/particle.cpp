//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "particle.h"
#include "string.h"
#include "input.h"
#include "operation.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PARTICLE_FILENAME	"data/TEXT/EFFECT/particle.txt"				// �p�[�e�B�N���f�[�^
#define TEXTURE_FILENAME	"data/TEXT/EFFECT/particle_texture.txt"		// �e�N�X�`����
#define PARTICLE_SAVEFILE	"data/TEXT/EFFECT/save_particle.txt"

#define MAX_EMITTER_TYPE	(32)							// �p�[�e�B�N���̎�ނ̍ő吔
#define MAX_PARTICLE		(1024)							// �p�[�e�B�N���̍ő吔

#define MAX_TEXTURE			(32)								// �e�N�X�`���̍ő吔
#define MAX_TYPE			(32)								// ���ڂ̍ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);

void EditParticle(void);

void FileInputParticle(void);
void FileOutputParticle(void);
void TextureInputParticle(void);

void StringParticle(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureParticle[MAX_TEXTURE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Particle g_aParticle[MAX_PARTICLE];				// �p�[�e�B�N��
ParEmitter g_aEmitter[MAX_EMITTER];				// �\���p�̃G�~�b�^
ParEmitter g_aEditEmitter[MAX_EMITTER_TYPE];		// �G�f�B�b�g�p�̃G�~�b�^

int g_nCntParticle;		// �o��������Ԋu�̃J�E���^

int g_nSelectParticleEmitter;				// �I�𒆂̃G�~�b�^
int g_nSelectParticleType;					// �I�𒆂̍���

int g_nNumParticleData;						// �p�[�e�B�N���̎�ނ̐�
int g_nNumParticleTexture;					// �e�N�X�`���̐�

int g_nCntAngle;							// �p�[�e�B�N���̊p�x�v�Z�p�J�E���^

//=============================================================================
// ����������
//=============================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �p�[�e�B�N���̏���������
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 0;
		g_aParticle[nCntParticle].nType = 0;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].fAngle = 0.0f;
		g_aParticle[nCntParticle].fGravity = 0.0f;
		g_aParticle[nCntParticle].fBouncing = 0.0f;
		g_aParticle[nCntParticle].nIdx = 0;
		g_aParticle[nCntParticle].bUse = false;
		g_aParticle[nCntParticle].bDraw = false;
	}

	// �G�~�b�^�̏���������
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		g_aEmitter[nCntEmitter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEmitter[nCntEmitter].nWidth[0] = 0;
		g_aEmitter[nCntEmitter].nWidth[1] = 0;
		g_aEmitter[nCntEmitter].nHeight[0] = 0;
		g_aEmitter[nCntEmitter].nHeight[1] = 0;
		g_aEmitter[nCntEmitter].nDepth[0] = 0;
		g_aEmitter[nCntEmitter].nDepth[1] = 0;
		g_aEmitter[nCntEmitter].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEmitter[nCntEmitter].flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEmitter[nCntEmitter].nRadius[0] = 0;
		g_aEmitter[nCntEmitter].nRadius[1] = 0;
		g_aEmitter[nCntEmitter].fFlameRadius = 0;
		g_aEmitter[nCntEmitter].fSpread = 0.0f;
		g_aEmitter[nCntEmitter].nAppear = 1;
		g_aEmitter[nCntEmitter].nParLife[0] = 0;
		g_aEmitter[nCntEmitter].nParLife[1] = 0;
		g_aEmitter[nCntEmitter].nLife = 0;
		g_aEmitter[nCntEmitter].nType = 0;
		g_aEmitter[nCntEmitter].nLap = 0;
		g_aEmitter[nCntEmitter].fGravity = 0.0f;
		g_aEmitter[nCntEmitter].nRot = 0;
		g_aEmitter[nCntEmitter].fRotSpeed = 0.0f;
		g_aEmitter[nCntEmitter].bLoop = false;
		g_aEmitter[nCntEmitter].bBouncing = false;
		g_aEmitter[nCntEmitter].bUse = false;
		g_aEmitter[nCntEmitter].bDraw = false;
	}
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER_TYPE; nCntEmitter++)
	{
		g_aEditEmitter[nCntEmitter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEditEmitter[nCntEmitter].nWidth[0] = 0;
		g_aEditEmitter[nCntEmitter].nWidth[1] = 0;
		g_aEditEmitter[nCntEmitter].nHeight[0] = 0;
		g_aEditEmitter[nCntEmitter].nHeight[1] = 0;
		g_aEditEmitter[nCntEmitter].nDepth[0] = 0;
		g_aEditEmitter[nCntEmitter].nDepth[1] = 0;
		g_aEditEmitter[nCntEmitter].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEditEmitter[nCntEmitter].flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEditEmitter[nCntEmitter].nRadius[0] = 0;
		g_aEditEmitter[nCntEmitter].nRadius[1] = 0;
		g_aEditEmitter[nCntEmitter].fFlameRadius = 0;
		g_aEditEmitter[nCntEmitter].fSpread = 0.0f;
		g_aEditEmitter[nCntEmitter].nAppear = 1;
		g_aEditEmitter[nCntEmitter].nParLife[0] = 0;
		g_aEditEmitter[nCntEmitter].nParLife[1] = 0;
		g_aEditEmitter[nCntEmitter].nLife = 0;
		g_aEditEmitter[nCntEmitter].nLap = 1;
		g_aEditEmitter[nCntEmitter].fGravity = 0.0f;
		g_aEditEmitter[nCntEmitter].nRot = 0;
		g_aEditEmitter[nCntEmitter].fRotSpeed = 0.0f;
		g_aEditEmitter[nCntEmitter].nType = 0;
		g_aEditEmitter[nCntEmitter].bLoop = false;
		g_aEditEmitter[nCntEmitter].bBouncing = false;
		g_aEditEmitter[nCntEmitter].bUse = false;
		g_aEditEmitter[nCntEmitter].bDraw = false;
	}

	g_nCntParticle = 0;
	g_nNumParticleTexture = 1;
	g_nCntAngle = 0;

	FileInputParticle();
	TextureInputParticle();

	// �p�[�e�B�N���̒��_���̍쐬
	MakeVertexParticle(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureParticle[nCntTexture] != NULL)
		{
			g_pTextureParticle[nCntTexture]->Release();
			g_pTextureParticle[nCntTexture] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{
	int nCntParticle;	// for���p�̃J�E���^

	EditParticle();

	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter[nCntEmitter].bUse == true)
		{// �g�p����Ă���G�~�b�^

			g_aEmitter[nCntEmitter].nLife--;	// �G�~�b�^�̕\�����Ԃ��J�E���g

			if (g_aEmitter[nCntEmitter].nLife <= 0 && g_aEmitter[nCntEmitter].bLoop == false)
			{// ��莞�Ԍo�߂ŃG�~�b�^������
				g_aEmitter[nCntEmitter].bUse = false;
			}

			// �o��������Ԋu�̃J�E���g
			g_nCntParticle = (g_nCntParticle + 1) % g_aEmitter[nCntEmitter].nAppear;

			if (g_nCntParticle == 0)
			{// �o��������
				for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
				{// �p�[�e�B�N��������
					if (g_aParticle[nCntParticle].bUse == false)
					{// �g���Ă��Ȃ�
						g_aParticle[nCntParticle].fAngle = 1.0f / (g_aEmitter[nCntEmitter].nLap / 2);
						g_aParticle[nCntParticle].pos = g_aEmitter[nCntEmitter].pos;
						//g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nWidth[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nWidth[1] * 2))) / 2) + ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread,
						//	(rand() % g_aEmitter[nCntEmitter].nHeight[1]) + (rand() % g_aEmitter[nCntEmitter].nHeight[0]),
						//	cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nDepth[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nDepth[1] * 2))) / 2) + ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread);
						if(0 != g_aEmitter[nCntEmitter].nHeight[1] && 0 != g_aEmitter[nCntEmitter].nHeight[0])
						{
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nWidth[0] + (rand() % (1 + g_aEmitter[nCntEmitter].nWidth[1])) - 1) - ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread,
								((rand() % g_aEmitter[nCntEmitter].nHeight[1]) + (rand() % g_aEmitter[nCntEmitter].nHeight[0])) * g_aEmitter[nCntEmitter].fSpread,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nDepth[0] + (rand() % (1 + g_aEmitter[nCntEmitter].nDepth[1]) - 1)) - ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread);
						}
						else
						{
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nWidth[0] + (rand() % (1 + g_aEmitter[nCntEmitter].nWidth[1])) - 1) - ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread,
								(g_aEmitter[nCntEmitter].nHeight[1] + g_aEmitter[nCntEmitter].nHeight[0]) * g_aEmitter[nCntEmitter].fSpread,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nDepth[0] + (rand() % (1 + g_aEmitter[nCntEmitter].nDepth[1]) - 1)) - ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread);
						}

						g_aParticle[nCntParticle].col = g_aEmitter[nCntEmitter].col;
						g_aParticle[nCntParticle].fRadius = g_aEmitter[nCntEmitter].nRadius[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nRadius[1] - g_aEmitter[nCntEmitter].nRadius[0])) - 1) + ((rand() % 10) * 0.1f);
						g_aParticle[nCntParticle].nLife = g_aEmitter[nCntEmitter].nParLife[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nParLife[1] - g_aEmitter[nCntEmitter].nParLife[0])) - 1);
						g_aParticle[nCntParticle].nType = g_aEmitter[nCntEmitter].nType;
						g_aParticle[nCntParticle].fGravity = g_aEmitter[nCntEmitter].fGravity;
						g_aParticle[nCntParticle].nIdx = nCntEmitter;
						g_aParticle[nCntParticle].bDraw = g_aEmitter[nCntEmitter].bDraw;
						g_aParticle[nCntParticle].bUse = true;

						g_nCntAngle = (g_nCntAngle + 1) % g_aEmitter[nCntEmitter].nLap;
						break;
					}
				}
			}
		}
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �p�[�e�B�N���̍X�V
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	VERTEX_3D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{// �g���Ă���
			g_aParticle[nCntParticle].nLife--;		// �\�����Ԃ��J�E���g

			// �\�����Ԃ�0�ȉ��ɂȂ���
			if (g_aParticle[nCntParticle].nLife <= 0)
			{// �g�p���Ă��Ȃ���Ԃɂ���
				g_aParticle[nCntParticle].bUse = false;
			}

			g_aParticle[nCntParticle].move.y -= g_aParticle[nCntParticle].fGravity;		// �d�͂�������

			g_aParticle[nCntParticle].fBouncing = g_aParticle[nCntParticle].pos.y;		// �l���L��

			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;								// �ʒu�̍X�V
			g_aParticle[nCntParticle].col += g_aEmitter[g_aParticle[nCntParticle].nIdx].flameCol;			// �F�̍X�V
			g_aParticle[nCntParticle].fRadius += g_aEmitter[g_aParticle[nCntParticle].nIdx].fFlameRadius;	// �傫���̍X�V

			if (g_aParticle[nCntParticle].pos.y <= 0 && g_aEmitter[g_aParticle[nCntParticle].nIdx].bBouncing == true)
			{// �o�E���h
				g_aParticle[nCntParticle].move.y += (g_aParticle[nCntParticle].fBouncing - g_aParticle[nCntParticle].pos.y) * 2.0f;
			}

			// ���_�̊p�x
			g_aParticle[nCntParticle].fRotAngle = atan2f(g_aParticle[nCntParticle].pos.x - (g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius), g_aParticle[nCntParticle].pos.y - (g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius));

			// ���S���璸�_�܂ł̋���
			g_aParticle[nCntParticle].fLength = sqrtf((g_aParticle[nCntParticle].fRadius * g_aParticle[nCntParticle].fRadius) + (g_aParticle[nCntParticle].fRadius * g_aParticle[nCntParticle].fRadius));

			if (g_aEmitter[g_aParticle[nCntParticle].nIdx].nRot == 1)
			{// ���v���
				g_aParticle[nCntParticle].fCntRot -= g_aEmitter[g_aParticle[nCntParticle].nIdx].fRotSpeed;			// ��]�̃J�E���^
			}
			else if (g_aEmitter[g_aParticle[nCntParticle].nIdx].nRot == 2)
			{// �����v���
				g_aParticle[nCntParticle].fCntRot += g_aEmitter[g_aParticle[nCntParticle].nIdx].fRotSpeed;			// ��]�̃J�E���^
			}
			else if (g_aEmitter[g_aParticle[nCntParticle].nIdx].nRot == 3)
			{// �����_��
				if (nCntParticle % 2 == 0)
				{// ���v���
					g_aParticle[nCntParticle].fCntRot += g_aEmitter[g_aParticle[nCntParticle].nIdx].fRotSpeed;		// ��]�̃J�E���^
				}
				else
				{// �����v���
					g_aParticle[nCntParticle].fCntRot -= g_aEmitter[g_aParticle[nCntParticle].nIdx].fRotSpeed;		// ��]�̃J�E���^
				}
			}

			//���_���W�̐ݒ�
			pVtx[0].pos.x = sinf(g_aParticle[nCntParticle].fRotAngle + g_aParticle[nCntParticle].fCntRot) * -(g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[0].pos.y = cosf(g_aParticle[nCntParticle].fRotAngle + g_aParticle[nCntParticle].fCntRot) * (g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[1].pos.x = sinf(g_aParticle[nCntParticle].fRotAngle - g_aParticle[nCntParticle].fCntRot) * (g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[1].pos.y = cosf(g_aParticle[nCntParticle].fRotAngle - g_aParticle[nCntParticle].fCntRot) * (g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[2].pos.x = sinf(g_aParticle[nCntParticle].fRotAngle - g_aParticle[nCntParticle].fCntRot) * -(g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[2].pos.y = cosf(g_aParticle[nCntParticle].fRotAngle - g_aParticle[nCntParticle].fCntRot) * -(g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[3].pos.x = sinf(g_aParticle[nCntParticle].fRotAngle + g_aParticle[nCntParticle].fCntRot) * (g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[3].pos.y = cosf(g_aParticle[nCntParticle].fRotAngle + g_aParticle[nCntParticle].fCntRot) * -(g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);

			//���_���W�̐ݒ�
			//pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			//pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			//pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
			//pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

			//���_�J���[
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}


	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffParticle->Unlock();

	// �t�@�C���ɏo��
	if (GetKeyboardTrigger(DIK_F9) == true)
	{
		FileOutputParticle();
	}
}

//=============================================================================
// �|���S���`�揈��
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	int nCntParticle;

	// �p�[�e�B�N���̃f�[�^�𕶎���ɂ���
	StringParticle();

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bDraw == true)
		{ // ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		else
		{// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		if (g_aParticle[nCntParticle].bUse == true)
		{// �p�[�e�B�N�����g���Ă����
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aParticle[nCntParticle].mtxWorld);

			// ��]�𔽉f
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			g_aParticle[nCntParticle].mtxWorld._11 = mtxView._11;
			g_aParticle[nCntParticle].mtxWorld._12 = mtxView._21;
			g_aParticle[nCntParticle].mtxWorld._13 = mtxView._31;
			g_aParticle[nCntParticle].mtxWorld._21 = mtxView._12;
			g_aParticle[nCntParticle].mtxWorld._22 = mtxView._22;
			g_aParticle[nCntParticle].mtxWorld._23 = mtxView._32;
			g_aParticle[nCntParticle].mtxWorld._31 = mtxView._13;
			g_aParticle[nCntParticle].mtxWorld._32 = mtxView._23;
			g_aParticle[nCntParticle].mtxWorld._33 = mtxView._33;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_aParticle[nCntParticle].mtxWorld, &g_aParticle[nCntParticle].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCntParticle].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureParticle[g_aParticle[nCntParticle].nType]);

			// �p�[�e�B�N���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
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
// �G�t�F�N�g�̃f�[�^�𕶎���ɂ���
//=============================================================================
void StringParticle(void)
{
	char aStrParticle[PARTICLE_MAX][MAX_STRING];			// �\�����镶����@���l
	char aStrParticleType[PARTICLE_MAX][MAX_STRING];		// �\�����镶����@����

	// ����
	sprintf(&aStrParticleType[PARTICLE_SELEMITTER][0], "[ �G�~�b�^�ԍ� ]");			// �I�𒆂̃G�~�b�^
	sprintf(&aStrParticleType[PARTICLE_MAXMOVE][0], "[ �ړ��ʂ̍ő� ]");			// �ړ��ʂ̍ő�l
	sprintf(&aStrParticleType[PARTICLE_MINMOVE][0], "[ �ړ��ʂ̍ŏ� ]");			// �ړ��ʂ̍ŏ��l
	sprintf(&aStrParticleType[PARTICLE_COLOR][0], "[ �J���[ ]");					// �F
	sprintf(&aStrParticleType[PARTICLE_FLAMECOLOR][0], "[ �J���[�̕ω� ]");			// �F�̕ω�
	sprintf(&aStrParticleType[PARTICLE_RADIUS][0], "[ �T�C�Y ]");					// �T�C�Y
	sprintf(&aStrParticleType[PARTICLE_FLAMERADIUS][0], "[ �T�C�Y�̊g�k ]");		// ���t���[���̃T�C�Y�̊g�k
	sprintf(&aStrParticleType[PARTICLE_LIFE][0], "[ �G�~�b�^�̎��� ]");				// �G�~�b�^�̕\������
	sprintf(&aStrParticleType[PARTICLE_PARLIFE][0], "[ �\���t���[�� ]");			// �p�[�e�B�N���̕\������
	sprintf(&aStrParticleType[PARTICLE_APPEAR][0], "[ �o��Ԋu ]");					// �o��Ԋu
	sprintf(&aStrParticleType[PARTICLE_SPREAD][0], "[ �g�U ]");						// �g�U����x��
	sprintf(&aStrParticleType[PARTICLE_LAP][0], "[ 1���̌� ]");					// 1���ɏo����
	sprintf(&aStrParticleType[PARTICLE_LOOP][0], "[ ���[�v�̗L�� ]");				// ���[�v�̗L��
	sprintf(&aStrParticleType[PARTICLE_GRAVITY][0], "[ �d�� ]");					// �d��
	sprintf(&aStrParticleType[PARTICLE_BOUNCING][0], "[ �o�E���h�̗L�� ]");			// �o�E���h�̗L��
	sprintf(&aStrParticleType[PARTICLE_ROTATE][0], "[ ��]�̃p�^�[�� ]");			// ��]�̃p�^�[��
	sprintf(&aStrParticleType[PARTICLE_ROTATE_SPEED][0], "[ ��]�̑��x ]");			// ��]�̑��x
	sprintf(&aStrParticleType[PARTICLE_TEXTURE][0], "[ �e�N�X�`�� ]");				// �e�N�X�`��
	sprintf(&aStrParticleType[PARTICLE_DRAW][0], "[ ���Z�����̗L�� ]");				// ���Z�����̗L��

	// ���l
	sprintf(&aStrParticle[PARTICLE_SELEMITTER][0], "[ %d ]", g_nSelectParticleEmitter);								// �I�𒆂̃G�~�b�^
	sprintf(&aStrParticle[PARTICLE_MAXMOVE][0], "[ %d %d %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1], g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1], g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1]);	// �ړ��ʂ̍ő�
	sprintf(&aStrParticle[PARTICLE_MINMOVE][0], "[ %d %d %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0], g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0], g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0]);	// �ړ��ʂ̍ŏ�
	sprintf(&aStrParticle[PARTICLE_COLOR][0], "[ %.2f %.2f %.2f %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].col.r, g_aEditEmitter[g_nSelectParticleEmitter].col.g, g_aEditEmitter[g_nSelectParticleEmitter].col.b, g_aEditEmitter[g_nSelectParticleEmitter].col.a);	// �F
	sprintf(&aStrParticle[PARTICLE_FLAMECOLOR][0], "[ %.2f %.2f %.2f %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r, g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g, g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b, g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a);	// �F
	sprintf(&aStrParticle[PARTICLE_RADIUS][0], "[ %d %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0], g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1]);			// �T�C�Y
	sprintf(&aStrParticle[PARTICLE_FLAMERADIUS][0], "[ %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius);		// ���t���[���̃T�C�Y�̊g�k
	sprintf(&aStrParticle[PARTICLE_LIFE][0], "[ %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nLife);						// ����
	sprintf(&aStrParticle[PARTICLE_PARLIFE][0], "[ %d %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0], g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1]);			// �\������
	sprintf(&aStrParticle[PARTICLE_APPEAR][0], "[ %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nAppear);					// �o��Ԋu
	sprintf(&aStrParticle[PARTICLE_SPREAD][0], "[ %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].fSpread);					// �g�U����x��
	sprintf(&aStrParticle[PARTICLE_LAP][0], "[ %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nLap);						// 1���ɏo����
	sprintf(&aStrParticle[PARTICLE_GRAVITY][0], "[ %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].fGravity);					// �d�͂̓x��
	sprintf(&aStrParticle[PARTICLE_LOOP][0], "[ %s ]", g_aEditEmitter[g_nSelectParticleEmitter].bLoop ? "����" : "�Ȃ�");		// ���[�v�����邩
	sprintf(&aStrParticle[PARTICLE_BOUNCING][0], "[ %s ]", g_aEditEmitter[g_nSelectParticleEmitter].bBouncing ? "����" : "�Ȃ�");	// �o�E���h�����邩
	sprintf(&aStrParticle[PARTICLE_ROTATE][0], "[ %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nRot);		// ��]�̃p�^�[��
	sprintf(&aStrParticle[PARTICLE_ROTATE_SPEED][0], "[ %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed);	// ��]�̑��x
	sprintf(&aStrParticle[PARTICLE_TEXTURE][0], "[ %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nType);					// �e�N�X�`��
	sprintf(&aStrParticle[PARTICLE_DRAW][0], "[ %s ]", g_aEditEmitter[g_nSelectParticleEmitter].bDraw ? "���Z����" : "����");		// ���Z�����̗L��

	// �\������
	RECT rect;

	for (int nCntString = 0; nCntString < PARTICLE_MAX; nCntString++)
	{	// ���ڂ̕`��
		rect = { 10,230 + (nCntString * 20),0,0 };
		SetString(&aStrParticleType[nCntString][0], rect, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �F������
		rect = { 170,230 + (nCntString * 20),0,0 };
		SetString(":", rect, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ���l�̕`��
		if (nCntString == g_nSelectParticleType)
		{// �I��
			rect = { 200,230 + (nCntString * 20),0,0 };
			SetString(&aStrParticle[nCntString][0], rect, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{// �I�𒆂łȂ�
			rect = { 190,230 + (nCntString * 20),0,0 };
			SetString(&aStrParticle[nCntString][0], rect, 0, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
		}
	}
}

//=============================================================================
// �p�[�e�B�N���̍쐬
//=============================================================================
void EditParticle(void)
{
	// ���ڂ̑I��
	if (GetKeyboardRepeat(SELECT_UP.nDIK) == true)
	{// �J�[�\�����グ��
		g_nSelectParticleType--;

		if (g_nSelectParticleType < 0)
		{// ���[�v������
			g_nSelectParticleType = PARTICLE_MAX - 1;
		}
	}
	else if (GetKeyboardRepeat(SELECT_DOWN.nDIK) == true)
	{// �J�[�\����������
		g_nSelectParticleType = (g_nSelectParticleType + 1) % PARTICLE_MAX;
	}

	// ���l�̐ݒ�
	switch (g_nSelectParticleType)
	{
	case PARTICLE_SELEMITTER:		// �I�𒆂̃G�~�b�^------------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_nSelectParticleEmitter = 0;
		}

		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// ���Z����
			if (g_nSelectParticleEmitter < MAX_EMITTER_TYPE)
			{// �ő���z���Ȃ�
				g_nSelectParticleEmitter++;
			}
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// ���Z����
			if (g_nSelectParticleEmitter > 0)
			{// 0��艺���Ȃ�
				g_nSelectParticleEmitter--;
			}
		}

		break;

	case PARTICLE_MAXMOVE:				// �ړ��ʂ̍ő�-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1] = g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0];
			g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1] = g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0];
			g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1] = g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0];
		}

		// ���l�ύX
		if (GetKeyboardPress(DIK_X) == true)
		{// X��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1]++;
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1] > g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0])
				{// �ŏ��l��菬�������Ȃ�
					g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1]--;
				}
			}
		}
		if (GetKeyboardPress(DIK_Y) == true)
		{// Y��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1]++;
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1] > g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0])
				{// �ŏ��l��菬�������Ȃ�
					g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1]--;
				}
			}
		}
		if (GetKeyboardPress(DIK_Z) == true)
		{// Z��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1]++;
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1] > g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0])
				{// �ŏ��l��菬�������Ȃ�
					g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1]--;
				}
			}
		}
		break;

	case PARTICLE_MINMOVE:				// �ړ��ʂ̍ŏ�-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0] = 0;
			g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0] = 0;
			g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0] = 0;
		}

		// ���l�ύX
		if (GetKeyboardPress(DIK_X) == true)
		{// X��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0] < g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1])
				{// �ő�l���傫�����Ȃ�
					g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0]++;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0]--;
			}
		}
		if (GetKeyboardPress(DIK_Y) == true)
		{// Y��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0] < g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1])
				{// �ő�l���傫�����Ȃ�
					g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0]++;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0]--;
			}
		}
		if (GetKeyboardPress(DIK_Z) == true)
		{// Z��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0] < g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1])
				{// �ő�l���傫�����Ȃ�
					g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0]++;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0]--;
			}
		}
		break;

	case PARTICLE_COLOR:			// �F-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// ���l�ύX
		if (GetKeyboardPress(DIK_R) == true)
		{// ��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].col.r += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.r > 1.0f)
				{// �ő���z����0�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].col.r = 0.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].col.r -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.r < 0.0f)
				{// 0���z���ƍő�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].col.r = 1.0f;
				}
			}
		}
		else if (GetKeyboardPress(DIK_G) == true)
		{// ��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].col.g += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.g > 1.0f)
				{// �ő���z����0�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].col.g = 0.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].col.g -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.g < 0.0f)
				{// 0���z���ƍő�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].col.g = 1.0f;
				}
			}
		}
		else if (GetKeyboardPress(DIK_B) == true)
		{// ��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].col.b += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.b > 1.0f)
				{// �ő���z����0�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].col.b = 0.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].col.b -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.b < 0.0f)
				{// 0���z���ƍő�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].col.b = 1.0f;
				}
			}
		}
		if (GetKeyboardPress(DIK_T) == true)
		{// �����x
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].col.a += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.a > 1.0f)
				{// �ő���z����0�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].col.a = 0.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].col.a -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.a < 0.0f)
				{// 0���z���ƍő�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].col.a = 1.0f;
				}
			}
		}
		break;

	case PARTICLE_FLAMECOLOR:			// �F�̉��Z-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// ���l�ύX
		if (GetKeyboardPress(DIK_R) == true)
		{// ��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r > 1.0f)
				{// �ő���z����-1�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r = -1.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r < -1.0f)
				{// -1���z���ƍő�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r = 1.0f;
				}
			}
		}
		else if (GetKeyboardPress(DIK_G) == true)
		{// ��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g > 1.0f)
				{// �ő���z����-1�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g = -1.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g < -1.0f)
				{// -1���z���ƍő�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g = 1.0f;
				}
			}
		}
		else if (GetKeyboardPress(DIK_B) == true)
		{// ��
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b > 1.0f)
				{// �ő���z����-1�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b = -1.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b < -1.0f)
				{// -1���z���ƍő�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b = 1.0f;
				}
			}
		}
		else if (GetKeyboardPress(DIK_T) == true)
		{// �����x
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a > 1.0f)
				{// �ő���z����-1�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a = -1.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a < -1.0f)
				{// -1���z���ƍő�ɂ���
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a = 1.0f;
				}
			}
		}
		break;

	case PARTICLE_RADIUS:				// �T�C�Y-------------------------------------------------------------------------------
		if (GetKeyboardPress(DIK_G) == true)
		{// �ŏ��l
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0] < g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1])
				{
					g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0]++;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0] > 0)
				{
					g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0]--;
				}
			}
		}
		if (GetKeyboardPress(DIK_T) == true)
		{// �ő�l
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1]++;
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1] > g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0])
				{
					g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1]--;
				}
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0] = 5;
			g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1] = 10;
		}

		break;

	case PARTICLE_FLAMERADIUS:		// �傫���̉��Z-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// ���Z����
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// ���Z����ʂ𑝂₷
				g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius += 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius += 0.01f;
			}
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// ���Z����
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// ���Z����ʂ𑝂₷
				g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius -= 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius -= 0.01f;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius = 5.0f;
		}

		break;

	case PARTICLE_LIFE:				// ����-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// ���Z����
			g_aEditEmitter[g_nSelectParticleEmitter].nLife++;
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// ���Z����
			if (g_aEditEmitter[g_nSelectParticleEmitter].nLife > 1)
			{// 0��艺���Ȃ�
				g_aEditEmitter[g_nSelectParticleEmitter].nLife--;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].nLife = 0;
		}

		break;

	case PARTICLE_PARLIFE:			// �\������-------------------------------------------------------------------------------
		if (GetKeyboardPress(DIK_G) == true)
		{
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0] < g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1])
				{// �ő�l���傫�����Ȃ�
					g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0]++;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0] > 0)
				{// 0��艺���Ȃ�
					g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0]--;
				}
			}
		}
		if (GetKeyboardPress(DIK_T) == true)
		{
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// ���Z����
				g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1]++;
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// ���Z����
				if (g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1] > g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0])
				{// �ŏ��l��艺���Ȃ�
					g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1]--;
				}
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0] = 0;
			g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1] = 0;
		}

		break;

	case PARTICLE_APPEAR:			// �o��Ԋu-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// ���Z����
			g_aEditEmitter[g_nSelectParticleEmitter].nAppear++;
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// ���Z����
			if (g_aEditEmitter[g_nSelectParticleEmitter].nAppear > 1)
			{// 0��艺���Ȃ�
				g_aEditEmitter[g_nSelectParticleEmitter].nAppear--;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].nAppear = 0;
		}

		break;

	case PARTICLE_SPREAD:			// �L����-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// ���Z����
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// ���Z����ʂ𑝂₷
				g_aEditEmitter[g_nSelectParticleEmitter].fSpread += 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fSpread += 0.01f;
			}
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// ���Z����
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// ���Z����ʂ𑝂₷
				g_aEditEmitter[g_nSelectParticleEmitter].fSpread -= 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fSpread -= 0.01f;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].fSpread = 0;
		}

		break;

	case PARTICLE_LAP:				// 1���ɏo����-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// ���Z����
			g_aEditEmitter[g_nSelectParticleEmitter].nLap++;
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// ���Z����
			if (g_aEditEmitter[g_nSelectParticleEmitter].nLap > 1)
			{// 0��艺���Ȃ�
				g_aEditEmitter[g_nSelectParticleEmitter].nLap--;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].nLap = 0;
		}

		break;

	case PARTICLE_GRAVITY:			// �d��---------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// ���Z����
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// ���Z����ʂ𑝂₷
				g_aEditEmitter[g_nSelectParticleEmitter].fGravity += 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fGravity += 0.01f;
			}
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// ���Z����
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// ���Z����ʂ𑝂₷
				g_aEditEmitter[g_nSelectParticleEmitter].fGravity -= 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fGravity -= 0.01f;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].fGravity = 0;
		}

		break;

	case PARTICLE_ROTATE:			// ��]�̃p�^�[��-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// ���Z����
			g_aEditEmitter[g_nSelectParticleEmitter].nRot = (g_aEditEmitter[g_nSelectParticleEmitter].nRot + 1) % 4;
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// ���Z����
			g_aEditEmitter[g_nSelectParticleEmitter].nRot--;

			if (g_aEditEmitter[g_nSelectParticleEmitter].nRot < 0)
			{// 0��艺���Ȃ�
				g_aEditEmitter[g_nSelectParticleEmitter].nRot = 3;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].nRot = 0;
		}

		break;

	case PARTICLE_ROTATE_SPEED:		// ��]���鑬�x-----------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// ���Z����
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// ���Z����ʂ𑝂₷
				g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed += 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed += 0.01f;
			}
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// ���Z����
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// ���Z����ʂ𑝂₷
				g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed -= 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed -= 0.01f;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed = 0;
		}

		break;

	case PARTICLE_LOOP:				// ���[�v���邩�ǂ���-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(NUM_UP.nDIK) == true || GetKeyboardTrigger(NUM_DOWN.nDIK) == true)
		{
			g_aEditEmitter[g_nSelectParticleEmitter].bLoop = g_aEditEmitter[g_nSelectParticleEmitter].bLoop ? false : true;
		}

		break;

	case PARTICLE_BOUNCING:			// �o�E���h���邩�ǂ���-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(NUM_UP.nDIK) == true || GetKeyboardTrigger(NUM_DOWN.nDIK) == true)
		{
			g_aEditEmitter[g_nSelectParticleEmitter].bBouncing = g_aEditEmitter[g_nSelectParticleEmitter].bBouncing ? false : true;
		}

		break;

	case PARTICLE_TEXTURE:			// �e�N�X�`��-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// ���Z����
			g_aEditEmitter[g_nSelectParticleEmitter].nType = (g_aEditEmitter[g_nSelectParticleEmitter].nType + 1) % g_nNumParticleTexture;
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// ���Z����
			g_aEditEmitter[g_nSelectParticleEmitter].nType--;

			if (g_aEditEmitter[g_nSelectParticleEmitter].nType < 0)
			{// 0���z���ƍő�ɂ���
				g_aEditEmitter[g_nSelectParticleEmitter].nType = g_nNumParticleTexture - 1;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ���Z�b�g
			g_aEditEmitter[g_nSelectParticleEmitter].nType = 0;
		}

		break;

	case PARTICLE_DRAW:			// ���Z�������邩���Ȃ���-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(NUM_UP.nDIK) == true || GetKeyboardTrigger(NUM_DOWN.nDIK) == true)
		{
			g_aEditEmitter[g_nSelectParticleEmitter].bDraw = g_aEditEmitter[g_nSelectParticleEmitter].bDraw ? false : true;
		}

		break;

	}
}

//=============================================================================
// �G�~�b�^�̐ݒ�
//=============================================================================
void SetParticleEmitter(D3DXVECTOR3 pos)
{
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter[nCntEmitter].bUse == false)
		{// �g�p����Ă����Ԃɂ���
			g_aEmitter[nCntEmitter].pos = pos;
			g_aEmitter[nCntEmitter].nWidth[0] = g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0];
			g_aEmitter[nCntEmitter].nWidth[1] = g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1];
			g_aEmitter[nCntEmitter].nHeight[0] = g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0];
			g_aEmitter[nCntEmitter].nHeight[1] = g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1];
			g_aEmitter[nCntEmitter].nDepth[0] = g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0];
			g_aEmitter[nCntEmitter].nDepth[1] = g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1];
			g_aEmitter[nCntEmitter].col = g_aEditEmitter[g_nSelectParticleEmitter].col;
			g_aEmitter[nCntEmitter].flameCol = g_aEditEmitter[g_nSelectParticleEmitter].flameCol;
			g_aEmitter[nCntEmitter].nRadius[0] = g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0];
			g_aEmitter[nCntEmitter].nRadius[1] = g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1];
			g_aEmitter[nCntEmitter].fFlameRadius = g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius;
			g_aEmitter[nCntEmitter].fSpread = g_aEditEmitter[g_nSelectParticleEmitter].fSpread;
			g_aEmitter[nCntEmitter].nAppear = g_aEditEmitter[g_nSelectParticleEmitter].nAppear;
			g_aEmitter[nCntEmitter].nParLife[0] = g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0];
			g_aEmitter[nCntEmitter].nParLife[1] = g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1];
			g_aEmitter[nCntEmitter].nLap = g_aEditEmitter[g_nSelectParticleEmitter].nLap;
			g_aEmitter[nCntEmitter].nLife = g_aEditEmitter[g_nSelectParticleEmitter].nLife;
			g_aEmitter[nCntEmitter].nRot = g_aEditEmitter[g_nSelectParticleEmitter].nRot;
			g_aEmitter[nCntEmitter].fRotSpeed = g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed;
			g_aEmitter[nCntEmitter].nType = g_aEditEmitter[g_nSelectParticleEmitter].nType;
			g_aEmitter[nCntEmitter].bLoop = g_aEditEmitter[g_nSelectParticleEmitter].bLoop;
			g_aEmitter[nCntEmitter].bBouncing = g_aEditEmitter[g_nSelectParticleEmitter].bBouncing;
			g_aEmitter[nCntEmitter].fGravity = g_aEditEmitter[g_nSelectParticleEmitter].fGravity;
			g_aEmitter[nCntEmitter].bDraw = g_aEditEmitter[g_nSelectParticleEmitter].bDraw;

			g_aEmitter[nCntEmitter].bUse = true;
			break;
		}
	}
}

//=============================================================================
// �G�~�b�^�̈ʒu�̐ݒ�
//=============================================================================
void SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos)
{
	if (g_aEmitter[nIdx].bUse == true)
	{// �ʒu���X�V
		g_aEmitter[nIdx].pos = pos;
	}
}

//=============================================================================
// �G�~�b�^�̍폜
//=============================================================================
void DestroyParticle(int nIdx)
{
	if (g_aEmitter[nIdx].bUse == true)
	{// �g���Ă��Ȃ���Ԃɂ���
		g_aEmitter[nIdx].bUse = false;
	}
}

//=============================================================================
// �t�@�C������f�[�^���擾
//=============================================================================
void FileInputParticle(void)
{
	FILE *pFile;
	int nCntEffect = 0;
	char aStr[128];

	pFile = fopen(PARTICLE_FILENAME, "r");

	if (NULL != pFile)
	{
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{// �I�����b�Z�[�W�܂Ń��[�v
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, "PARTICLESET") == 0)
			{// �G�~�b�^�̏��
				while (strcmp(aStr, "END_PARTICLESET") != 0)
				{// �I�����b�Z�[�W�܂Ń��[�v
					fscanf(pFile, "%s", &aStr);

					if (strcmp(aStr, "MAXMOVE") == 0)
					{// �ړ��ʂ̍ő�
						fscanf(pFile, " = %d %d %d", &g_aEditEmitter[nCntEffect].nWidth[1], &g_aEditEmitter[nCntEffect].nHeight[1], &g_aEditEmitter[nCntEffect].nDepth[1]);
					}
					else if (strcmp(aStr, "MINMOVE") == 0)
					{// �ړ��ʂ̍ŏ�
						fscanf(pFile, " = %d %d %d", &g_aEditEmitter[nCntEffect].nWidth[0], &g_aEditEmitter[nCntEffect].nHeight[0], &g_aEditEmitter[nCntEffect].nDepth[0]);
					}
					else if (strcmp(aStr, "INITCOL") == 0)
					{// �����J���[
						fscanf(pFile, " = %f %f %f %f", &g_aEditEmitter[nCntEffect].col.r, &g_aEditEmitter[nCntEffect].col.g, &g_aEditEmitter[nCntEffect].col.b, &g_aEditEmitter[nCntEffect].col.a);
					}
					else if (strcmp(aStr, "FLAMECOL") == 0)
					{// �t���[���ɂ��J���[�̕ω�
						fscanf(pFile, " = %f %f %f %f", &g_aEditEmitter[nCntEffect].flameCol.r, &g_aEditEmitter[nCntEffect].flameCol.g, &g_aEditEmitter[nCntEffect].flameCol.b, &g_aEditEmitter[nCntEffect].flameCol.a);
					}
					else if (strcmp(aStr, "INITSIZE") == 0)
					{// �����T�C�Y
						fscanf(pFile, " = %d %d", &g_aEditEmitter[nCntEffect].nRadius[0], &g_aEditEmitter[nCntEffect].nRadius[1]);
					}
					else if (strcmp(aStr, "FLAMESIZE") == 0)
					{// �t���[���ɂ��T�C�Y�̕ω�
						fscanf(pFile, " = %f", &g_aEditEmitter[nCntEffect].fFlameRadius);
					}
					else if (strcmp(aStr, "EMITTERLIFE") == 0)
					{// �\������(����)
						fscanf(pFile, " = %d", &g_aEditEmitter[nCntEffect].nLife);
					}
					else if (strcmp(aStr, "PARLIFE") == 0)
					{// �p�[�e�B�N���̕\������
						fscanf(pFile, " = %d %d", &g_aEditEmitter[nCntEffect].nParLife[0], &g_aEditEmitter[nCntEffect].nParLife[1]);
					}
					else if (strcmp(aStr, "APPEAR") == 0)
					{// �o��Ԋu
						fscanf(pFile, " = %d", &g_aEditEmitter[nCntEffect].nAppear);
					}
					else if (strcmp(aStr, "SPREAD") == 0)
					{// �L����
						fscanf(pFile, " = %f", &g_aEditEmitter[nCntEffect].fSpread);
					}
					else if (strcmp(aStr, "LAP") == 0)
					{// 1���ɏo����
						fscanf(pFile, " = %d", &g_aEditEmitter[nCntEffect].nLap);
					}
					else if (strcmp(aStr, "GRAVITY") == 0)
					{// �d��
						fscanf(pFile, " = %f", &g_aEditEmitter[nCntEffect].fGravity);
					}
					else if (strcmp(aStr, "ROTPATTERN") == 0)
					{// ��]�̃p�^�[��
						fscanf(pFile, " = %d", &g_aEditEmitter[nCntEffect].nRot);
					}
					else if (strcmp(aStr, "ROTSPEED") == 0)
					{// ��]�̑��x
						fscanf(pFile, " = %f", &g_aEditEmitter[nCntEffect].fRotSpeed);
					}
					else if (strcmp(aStr, "LOOP") == 0)
					{// ���[�v���邩�ǂ���
						fscanf(pFile, " = %d", (int*)&g_aEditEmitter[nCntEffect].bLoop);
					}
					else if (strcmp(aStr, "BOUND") == 0)
					{// �o�E���h���邩�ǂ���
						fscanf(pFile, " = %d", (int*)&g_aEditEmitter[nCntEffect].bBouncing);
					}
					else if (strcmp(aStr, "TEXTURE") == 0)
					{// �e�N�X�`���̎��
						fscanf(pFile, " = %d", &g_aEditEmitter[nCntEffect].nType);
					}
					else if (strcmp(aStr, "DRAW") == 0)
					{// ���Z�����̗L��
						fscanf(pFile, " = %d", (int*)&g_aEditEmitter[nCntEffect].bDraw);
					}
				}
				nCntEffect++;	// �G�~�b�^�ԍ��̃J�E���g
			}
		}

		fclose(pFile);
	}
	else
	{// ���s
		MessageBox(0, "�t�@�C�����J���܂���ł���", "effect.txt", MB_OK);
	}
}

//=============================================================================
// �t�@�C���Ƀf�[�^����������
//=============================================================================
void FileOutputParticle(void)
{
	FILE *pFile;
	int nCntEffect;

	pFile = fopen(PARTICLE_SAVEFILE, "w");

	if (NULL != pFile)
	{
		fprintf(pFile, "#********************************************************************\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#    �yEffectCreater�z�X�N���v�g�t�@�C��	[save_particle.txt]\n");
		fprintf(pFile, "#    Auther : Ishida Takuto\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#********************************************************************\n");

		fprintf(pFile, "SCRIPT\n\n");
		fprintf(pFile, "#******************************************\n");
		fprintf(pFile, "#	�p�[�e�B�N���̐ݒ�\n");
		fprintf(pFile, "#******************************************\n");

		for (nCntEffect = 0; nCntEffect < MAX_EMITTER_TYPE; nCntEffect++)
		{// �f�[�^���܂킷
			fprintf(pFile, "PARTICLESET		[ %d ]\n", nCntEffect);
			fprintf(pFile, "	MAXMOVE = %d %d %d					���ړ��ʂ̍ő�\n", g_aEditEmitter[nCntEffect].nWidth[1], g_aEditEmitter[nCntEffect].nHeight[1], g_aEditEmitter[nCntEffect].nDepth[1]);	// �ړ��ʂ̍ő�
			fprintf(pFile, "	MINMOVE = %d %d %d					���ړ��ʂ̍ŏ�\n", g_aEditEmitter[nCntEffect].nWidth[0], g_aEditEmitter[nCntEffect].nHeight[0], g_aEditEmitter[nCntEffect].nDepth[0]);	// �ړ��ʂ̍ŏ�
			fprintf(pFile, "	INITCOL = %.2f %.2f %.2f %.2f	�������J���[\n", g_aEditEmitter[nCntEffect].col.r, g_aEditEmitter[nCntEffect].col.g, g_aEditEmitter[nCntEffect].col.b, g_aEditEmitter[nCntEffect].col.a);	// �F
			fprintf(pFile, "	FLAMECOL = %.2f %.2f %.2f %.2f	���t���[�����̃J���[�̕ω�\n", g_aEditEmitter[nCntEffect].flameCol.r, g_aEditEmitter[nCntEffect].flameCol.g, g_aEditEmitter[nCntEffect].flameCol.b, g_aEditEmitter[nCntEffect].flameCol.a);	// �F
			fprintf(pFile, "	INITSIZE = %d %d					�������T�C�Y\n", g_aEditEmitter[nCntEffect].nRadius[0], g_aEditEmitter[nCntEffect].nRadius[1]);			// �T�C�Y
			fprintf(pFile, "	FLAMESIZE = %f			���t���[�����̃T�C�Y�̕ω�\n", g_aEditEmitter[nCntEffect].fFlameRadius);	// �T�C�Y�̉��Z
			fprintf(pFile, "	EMITTERLIFE = %d					���G�~�b�^�̎���\n", g_aEditEmitter[nCntEffect].nLife);			// �\������(����)
			fprintf(pFile, "	PARLIFE = %d %d					���|���S���̎���\n", g_aEditEmitter[nCntEffect].nParLife[0], g_aEditEmitter[nCntEffect].nParLife[1]);		// �p�[�e�B�N���̕\������
			fprintf(pFile, "	APPEAR = %d						���o���Ԋu\n", g_aEditEmitter[nCntEffect].nAppear);			// �o��Ԋu
			fprintf(pFile, "	SPREAD = %.2f					���g�U\n", g_aEditEmitter[nCntEffect].fSpread);		// �L����
			fprintf(pFile, "	LAP = %d							��1���ɏo����\n", g_aEditEmitter[nCntEffect].nLap);			// 1���ɏo����
			fprintf(pFile, "	GRAVITY = %f				���d��\n", g_aEditEmitter[nCntEffect].fGravity);		// �d��
			fprintf(pFile, "	ROTPATTERN = %d					����]�̃p�^�[��\n", g_aEditEmitter[nCntEffect].nRot);			// ��]�̃p�^�[��
			fprintf(pFile, "	ROTSPEED = %f				����]�̑��x\n", g_aEditEmitter[nCntEffect].fRotSpeed);		// ��]�̑��x
			fprintf(pFile, "	LOOP = %d						�����[�v[0:�Ȃ��@1:����]\n", g_aEditEmitter[nCntEffect].bLoop);			// ���[�v���邩�ǂ���
			fprintf(pFile, "	BOUND = %d						���o�E���h[0:�Ȃ��@1:����]\n", g_aEditEmitter[nCntEffect].bBouncing);		// �o�E���h���邩�ǂ���
			fprintf(pFile, "	TEXTURE = %d						���e�N�X�`���̎��\n", g_aEditEmitter[nCntEffect].nType);			// �e�N�X�`���̎��
			fprintf(pFile, "	DRAW = %d						�����Z����[0:�Ȃ��@1:����]\n", g_aEditEmitter[nCntEffect].bDraw);			// ���Z�����̗L��
			fprintf(pFile, "END_PARTICLESET");
			fprintf(pFile, "\n\n");
		}
		fprintf(pFile, "\nEND_SCRIPT\n");

		fclose(pFile);
	}
	else
	{// ���s
		MessageBox(0, "�t�@�C�����J���܂���ł���", "particle.txt", MB_OK);
	}
}

//=============================================================================
// �t�@�C������e�N�X�`�����擾
//=============================================================================
void TextureInputParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	char aFileName[MAX_TEXTURE][64];		// �e�N�X�`���̖��O
	int nCntTexture;

	pFile = fopen(TEXTURE_FILENAME, "r");

	if (NULL != pFile)
	{// �J����
		fscanf(pFile, "%d", &g_nNumParticleTexture);		// �e�N�X�`����

		for (nCntTexture = 0; nCntTexture < g_nNumParticleTexture; nCntTexture++)
		{
			fscanf(pFile, "%s\n", &aFileName[nCntTexture][0]);		// �e�N�X�`����
		}
		fclose(pFile);
	}
	else
	{// ���s
		MessageBox(0, "�t�@�C�����J���܂���ł���", "texture.txt", MB_OK);
	}

	// �e�N�X�`���̓ǂݍ���
	for (nCntTexture = 0; nCntTexture < g_nNumParticleTexture; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, &aFileName[nCntTexture][0], &g_pTextureParticle[nCntTexture]);
	}
}

//=============================================================================
// �p�[�e�B�N���̒��_���̍쐬
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	VERTEX_3D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

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
	g_pVtxBuffParticle->Unlock();
}
