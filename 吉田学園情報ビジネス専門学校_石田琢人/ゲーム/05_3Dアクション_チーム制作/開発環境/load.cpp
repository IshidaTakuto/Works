//=============================================================================
//
// �G���A�ړ����� [load.cpp]
// Author : shotaro kanamaru
//
//=============================================================================
#include "main.h"		// ���C��
#include "load.h"		// �G���A�ړ�
#include "input.h"		// ����
#include "sound.h"		// ��
#include "fade.h"		// �t�F�[�h
#include "clear.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define LOAD_TEX_PLAYER			"data/TEXTURE/load_player.png"	// �v���C���[�A�C�R��
#define LOAD_TEX_BG				"data/TEXTURE/load_bg.png"		// �w�i
#define LOAD_TEX_LINE			"data/TEXTURE/load_line.png"	// �X�e�[�W���C��
#define LOAD_MAX_STAGE			(3)								// �X�e�[�W��
#define LOAD_LINE_WIDTH			(1000.0f)						// �X�e�[�W���C���̕�
#define LOAD_LINE_HEIGHT		(100.0f)						// �X�e�[�W���C���̍���
#define LOAD_ICON_DEF			(LOAD_LINE_WIDTH / (LOAD_MAX_STAGE - 1))	// �v���C���[�A�C�R���̖ړI�ʒu
#define LOAD_START				(60)							// �S�̓I�ȏ����̊J�n����

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************
void LoadMakeVtx(void);						// ���_���̍쐬
void LoadPlayerIcon(void);					// �v���C���[�A�C�R���̏���
void LoadFade(void);						// �t�F�[�h�̏���
void LoadBg(void);							// �w�i�̏���
void LoadColAExtent(void);					// �����x�͈̔͐���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Load			g_aLoad[LOADTYPE_MAX];		// �G���A�ړ����
LoadOther		g_loadOther;				// ���̑��ݒ���
LOADFADE		g_loadFade;					//�t�F�[�h�̏��
// �p�[�c�̐ݒ���
LoadInfo		g_aLoadInfo[LOADTYPE_MAX] =
{// [�ʒu]:[�傫��]:[�A�j���[�V������]
	/*�h��Ԃ�*/		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),1,
	/*�w�i*/			D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),3,
	/*�X�e�[�W���C��*/	D3DXVECTOR3(SCREEN_WIDTH / 2, 500.0f, 0.0f),				D3DXVECTOR3(LOAD_LINE_WIDTH, LOAD_LINE_HEIGHT, 0.0f),1,
	/*�v���C���[*/		D3DXVECTOR3(-LOAD_LINE_WIDTH / 2, -LOAD_LINE_HEIGHT,0.0f) + g_aLoadInfo[LOADTYPE_LINE].pos	,D3DXVECTOR3(70.0f, 100.0f, 0.0f),6,
	/*�t�F�[�h*/		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),1,
};

//=============================================================================
//�G���A�ړ��̏���������
//=============================================================================
void InitLoad(void)
{
	// �ݒ���̏�����
	g_loadOther.nNextStage = 0;					// ���̃X�e�[�W
	g_loadOther.nCntAll = 0;					// ���ʃJ�E���^
	g_loadOther.state = LOADSTATE_NONE;			// ���
	g_loadOther.bUse = false;					// �`����
	g_loadFade = LOADFADE_NONE;					// �t�F�[�h

	for (int nCntLoad = 0; nCntLoad < LOADTYPE_MAX; nCntLoad++)
	{// ���̏�����
		// �ʒu
		g_aLoad[nCntLoad].pos = g_aLoadInfo[nCntLoad].pos;
		// �傫��
		g_aLoad[nCntLoad].size = g_aLoadInfo[nCntLoad].size / 2;
		// �ړ���
		g_aLoad[nCntLoad].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �F
		g_aLoad[nCntLoad].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// �g�p
		g_aLoad[nCntLoad].bUse = true;
		// ���݂̃A�j���[�V����
		g_aLoad[nCntLoad].nNumPattern = 0;
	}

	// �h��Ԃ��F�̐ݒ�
	g_aLoad[LOADTYPE_BACK].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	// �t�F�[�h�̐ݒ�
	g_aLoad[LOADTYPE_FADE].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	LoadMakeVtx();		// ���_���̍쐬
}

//=============================================================================
//�G���A�ړ��̏I������
//=============================================================================
void UninitLoad(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_loadOther.pVtxBuff != NULL)
	{
		g_loadOther.pVtxBuff->Release();
		g_loadOther.pVtxBuff = NULL;
	}
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < LOADTYPE_MAX; nCntTex++)
	{
		if (g_loadOther.pTexture[nCntTex] != NULL)
		{
			g_loadOther.pTexture[nCntTex]->Release();
			g_loadOther.pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//�G���A�ړ��̍X�V����
//=============================================================================
void UpdateLoad(void)
{
	VERTEX_2D*pVtx;  //���_���ւ̃|�C���^

	// �t�F�[�h
	//LoadFade();

	if (g_loadOther.pVtxBuff != NULL)
	{
		if (g_loadOther.state == LOADSTATE_START)
		{// �������J�n���ꂽ��
			// �v���C���[�A�C�R���̏���
			LoadPlayerIcon();
			// �w�i�̏���
			LoadBg();

			g_loadOther.nCntAll++;		// ���ʃJ�E���^�̉��Z
		}
		else
		{
			g_loadOther.nCntAll = 0;		// ���ʃJ�E���^�����Z�b�g
		}

		// �t�F�[�h����
		LoadFade();
		// �����x�͈̔͐���
		LoadColAExtent();

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_loadOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntLoad = 0; nCntLoad < LOADTYPE_MAX; nCntLoad++, pVtx += 4)
		{
			// ���������e�N�X�`���̕��̎Z�o
			float fTexWidth = 1.0f / g_aLoadInfo[nCntLoad].nAnimPattern;

			// �ړ��ʂ̍X�V
			g_aLoad[nCntLoad].pos += g_aLoad[nCntLoad].move;

			// ���_���W
			pVtx[0].pos = D3DXVECTOR3(-g_aLoad[nCntLoad].size.x, -g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aLoad[nCntLoad].size.x, -g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aLoad[nCntLoad].size.x, g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aLoad[nCntLoad].size.x, g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;

			// ���_�J���[
			pVtx[0].col = g_aLoad[nCntLoad].col;
			pVtx[1].col = g_aLoad[nCntLoad].col;
			pVtx[2].col = g_aLoad[nCntLoad].col;
			pVtx[3].col = g_aLoad[nCntLoad].col;

			// �e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(fTexWidth * g_aLoad[nCntLoad].nNumPattern, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fTexWidth * g_aLoad[nCntLoad].nNumPattern + fTexWidth, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(fTexWidth * g_aLoad[nCntLoad].nNumPattern, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(fTexWidth * g_aLoad[nCntLoad].nNumPattern + fTexWidth, 1.0f);
		}

		//���_�o�b�t�@���A�����b�N
		g_loadOther.pVtxBuff->Unlock();
	}
}

//=============================================================================
// �v���C���[�A�C�R���̏���
//=============================================================================
void LoadPlayerIcon(void)
{
	if (g_aLoad[LOADTYPE_PLAYER].pos.x < LOAD_ICON_DEF * g_loadOther.nNextStage - LOAD_ICON_DEF + g_aLoad[LOADTYPE_LINE].pos.x)
	{// �w��ʒu�ɗ��Ă��Ȃ�������
		if (g_loadOther.nCntAll > LOAD_START)
		{
		 // �A�C�R�������̏ꏊ�ֈړ�
			g_aLoad[LOADTYPE_PLAYER].pos.x += 5.0f;
		}

		if (g_loadOther.nCntAll % 15 == 0)
		{// ���s�A�j���[�V�����̍X�V
			g_aLoad[LOADTYPE_PLAYER].nNumPattern = (g_aLoad[LOADTYPE_PLAYER].nNumPattern + 1) % 4;
		}
	}
	else
	{// �w��ʒu�ɗ�����
		// �w��ʒu�Œ�~
		g_aLoad[LOADTYPE_PLAYER].pos.x = LOAD_ICON_DEF * g_loadOther.nNextStage - LOAD_ICON_DEF + g_aLoad[LOADTYPE_LINE].pos.x;

		// �X�e�[�W�ړ������~�߂�
		StopSound(SOUND_LABEL_NEXTSTAGE);

		if (g_loadOther.nCntAll % 15 == 0)
		{// ����A�j���[�V�����̍X�V
			if (g_aLoad[LOADTYPE_PLAYER].nNumPattern != 5)
			{
				g_aLoad[LOADTYPE_PLAYER].nNumPattern = (g_aLoad[LOADTYPE_PLAYER].nNumPattern + 1) % 6;
			}
		}

		if (g_loadOther.nCntAll > 300)
		{// �����ԂɂȂ�����
			// �I����Ԃ�
			g_loadOther.state = LOADSTATE_OUT;
			// �t�F�[�h�J�n
			g_loadFade = LOADFADE_OUT;
		}
		if (g_loadOther.nCntAll == 250)
		{// �����o��
			PlaySound(SOUND_LABEL_HUNDUP);
		}
	}
}

//=============================================================================
// �w�i�̏���
//=============================================================================
void LoadBg(void)
{
	if (g_aLoad[LOADTYPE_BG].col.a == 0)
	{
		//static int nNowStage = 0;

		if (g_loadOther.nCntAll == LOAD_START + 30 - 1)
		{
			g_aLoad[LOADTYPE_BG].nNumPattern++;
		}
	}

	if (g_loadOther.nCntAll > LOAD_START + 30)
	{
		// �w�莞�Ԍo�ߌ�
		g_aLoad[LOADTYPE_BG].col.a += 0.02f;
	}
	else if (g_loadOther.nCntAll > LOAD_START)
	{
		// �w�莞�Ԍo�ߌ�
		g_aLoad[LOADTYPE_BG].col.a -= 0.06f;
	}
}

//=============================================================================
// �t�F�[�h
//=============================================================================
void LoadFade(void)
{
	if (g_loadFade != LOADFADE_NONE)
	{//�A�Ŗh�~
		if (g_loadFade == LOADFADE_IN)
		{//�t�F�[�h�C����Ԃ̎�
			g_aLoad[LOADTYPE_FADE].col.a -= 0.02f;	//��ʂ𓧖���

			if (g_aLoad[LOADTYPE_FADE].col.a <= 0.0f)
			{
				g_aLoad[LOADTYPE_FADE].col.a = 0.0f;
				g_loadFade = LOADFADE_NONE;			//�������Ă��Ȃ���Ԃ�

				if (g_loadOther.state == LOADSTATE_NONE)
				{// �I����Ԃ�������
					SetClearMessage(TYPE_NORMAL);
				}
				else
				{// �X�e�[�W�ړ���
					PlaySound(SOUND_LABEL_NEXTSTAGE);
				}
			}
		}
		else if (g_loadFade == LOADFADE_OUT)
		{//�t�F�[�h�A�E�g��Ԃ̎�
			g_aLoad[LOADTYPE_FADE].col.a += 0.02f;	//��ʂ�s������

			if (g_aLoad[LOADTYPE_FADE].col.a >= 1.0f)
			{
				g_aLoad[LOADTYPE_FADE].col.a = 1.0f;
				g_loadFade = LOADFADE_IN;			//�t�F�[�h�C����Ԃ�

				if (g_loadOther.state == LOADSTATE_NONE)
				{// �ҋ@��Ԃ�������
					// �`��J�n
					g_loadOther.bUse = true;
					// �����̊J�n
					g_loadOther.state = LOADSTATE_START;
				}
				else if (g_loadOther.state == LOADSTATE_OUT)
				{// �I����Ԃ�������
					// �`��I��
					g_loadOther.bUse = false;
					// �ҋ@��Ԃ�
					g_loadOther.state = LOADSTATE_NONE;

					STAGENUM stage = GetNumState();
					if (stage == STAGENUM_1)
					{
						SetStageState(STAGENUM_2);
					}
					else if (stage == STAGENUM_2)
					{
						SetStageState(STAGENUM_3);
					}
				}
			}
		}
	}
}

//=============================================================================
// �����x�͈̔͐���
//=============================================================================
void LoadColAExtent(void)
{
	for (int nCntLoad = 0; nCntLoad < LOADTYPE_MAX; nCntLoad++)
	{
		if (g_aLoad[nCntLoad].col.a < 0.0f)
		{// 0�����ɂȂ�����
			g_aLoad[nCntLoad].col.a = 0.0f;
		}
		if (g_aLoad[nCntLoad].col.a > 1.0f)
		{// 1�𒴉߂�����
			g_aLoad[nCntLoad].col.a = 1.0f;
		}
	}
}

//=============================================================================
// �G���A�ړ��̋N��
//=============================================================================
void SetLoad(void)
{
	if (g_loadOther.state == LOADSTATE_NONE &&
		g_loadFade == LOADFADE_NONE)
	{// �ҋ@��Ԃ��t�F�[�h���Ȃ����
		// �ړ��n�_�̍X�V
		g_loadOther.nNextStage++;
		// �t�F�[�h�J�n
		g_loadFade = LOADFADE_OUT;

		StageStateSwat(true);
	}
}

//=============================================================================
//�G���A�ړ��̕`�揈��
//=============================================================================
void DrawLoad(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_loadOther.pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLoad = 0; nCntLoad < LOADTYPE_MAX; nCntLoad++)
	{
		if (g_loadOther.bUse ||
			nCntLoad == LOADTYPE_FADE)
		{// �`���Ԃ������̓t�F�[�h
			 // �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_loadOther.pTexture[nCntLoad]);
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntLoad, 2);
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void LoadMakeVtx(void)
{
	VERTEX_2D*pVtx;								// ���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	//if (g_loadOther.pVtxBuff == NULL)
	{
		// �e�N�X�`���̓ǂݍ���
		// �w�i
		D3DXCreateTextureFromFile(pDevice,
			LOAD_TEX_BG,
			&g_loadOther.pTexture[LOADTYPE_BG]);
		// �v���C���[�A�C�R��
		D3DXCreateTextureFromFile(pDevice,
			LOAD_TEX_PLAYER,
			&g_loadOther.pTexture[LOADTYPE_PLAYER]);
		// �X�e�[�W���C��
		D3DXCreateTextureFromFile(pDevice,
			LOAD_TEX_LINE,
			&g_loadOther.pTexture[LOADTYPE_LINE]);

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LOADTYPE_MAX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_loadOther.pVtxBuff,
			NULL);

		//���_�o�b�t�@�����b�N���Ē��_�f�[�^�ւ̃|�C���^���擾
		g_loadOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���̐ݒ�
		for (int nCntLoad = 0; nCntLoad < LOADTYPE_MAX; nCntLoad++, pVtx += 4)
		{
			// ���_���W
			pVtx[0].pos = D3DXVECTOR3(-g_aLoad[nCntLoad].size.x, -g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aLoad[nCntLoad].size.x, -g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aLoad[nCntLoad].size.x, g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aLoad[nCntLoad].size.x, g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;

			// ���_�J���[
			pVtx[0].col = g_aLoad[nCntLoad].col;
			pVtx[1].col = g_aLoad[nCntLoad].col;
			pVtx[2].col = g_aLoad[nCntLoad].col;
			pVtx[3].col = g_aLoad[nCntLoad].col;

			// rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// �e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		//���_�o�b�t�@���A�����b�N
		g_loadOther.pVtxBuff->Unlock();
	}
}

//=============================================================================
// ���[�h�̃t�F�[�h�����擾
//=============================================================================
LOADFADE GetLoadFade(void)
{
	return g_loadFade;
}