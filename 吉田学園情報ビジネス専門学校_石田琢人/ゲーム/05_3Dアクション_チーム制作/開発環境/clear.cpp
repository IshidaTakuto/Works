//=============================================================================
//
// �N���A���b�Z�[�W���� [clear.cpp]
// Author : shotaro kanamaru
//
//=============================================================================
#include "main.h"		// ���C��
#include "clear.h"		// �N���A���b�Z�[�W
#include "input.h"		// ����
#include "sound.h"		// ��
#include "fade.h"		// �t�F�[�h
#include "load.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_CLEAR				(10)							// �ő吔
// �m�[�}���N���A
#define CLEAR_NOR_TEX			"data/TEXTURE/�퓬�J�n.png"// �e�N�X�`��
#define CLEAR_NOR_START			(30)							// �����J�n�̎���
#define CLEAR_NORBAND_HEIGHT	(200.0f)						// �т̑���
#define CLEAR_NOR_BAND_COL		(D3DXCOLOR(0.3f,0.3f,0.3f,0.8f))// �т̐F
#define CLEAR_NOR_SPEED			(37.0f)							// ���x
#define CLEAR_NOR_CONNECT		(60)							// ���b�Z�[�W�\���̎���
#define CLEAR_NOR_WIDTH			(600.0f)						// ���b�Z�[�W�̕�
#define CLEAR_NOR_HEIGHT		(200.0f)						// ���b�Z�[�W�̍���
#define CLEAR_NOR_DESTPOS_Y		(SCREEN_HEIGHT / 2)				// y�ʒu
// �R���v���[�g�N���A
#define CLEAR_COMP_TEX_0		"data/TEXTURE/��.png"// 1������
#define CLEAR_COMP_TEX_1		"data/TEXTURE/��.png"// 2������
#define CLEAR_COMP_TEX_2		"data/TEXTURE/��.png"// 3������
#define CLEAR_COMP_TEX_3		"data/TEXTURE/��.png"// 4������

#define CLEAR_COMP_START		(50)				// �����J�n�̎���
#define CLEAR_COMP_NUM			(4)					// ������
#define CLEAR_COMP_SPACE		(30.0f)				// �����̊Ԋu
#define CLEAR_COMP_CONNECT		(220)				// ���b�Z�[�W�\���̎���
#define CLEAR_COMP_INITSIZE		(700.0f)			// �o�����̑傫��
#define CLEAR_COMP_DESTSIZE		(270.0f)			// �ړI�̑傫��
#define CLEAR_COMP_INTERVAL		(40)				// �\���Ԋu
#define CLEAR_COMP_REDUCT		(7.0f)				// �k�����x

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************
void ClearMakeVtx(void);						// ���_���̍쐬
void ClearBand(void);							// �т̏���
void NormalMessage(void);						// �m�[�}�����b�Z�[�W�̏���
void CompleteMessage(void);						// �R���v���[�g���b�Z�[�W�̏���
void ClearReset(void);							// ���̃��Z�b�g

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Clear			g_aClear[MAX_CLEAR];			// �N���A���b�Z�[�W���
ClearOther		g_titleOther;					// ���̑��ݒ���

//=============================================================================
//�N���A���b�Z�[�W�̏���������
//=============================================================================
void InitClear(void)
{
	// �N���A���b�Z�[�W�̐ݒ���̏�����
	g_titleOther.nCntAll = 0;			// ���ʃJ�E���^
	g_titleOther.nCntDraw = 0;			// �\���J�E���^
	g_titleOther.bSwitch = false;		// �ړ��̐ؑ�
	g_titleOther.type = TYPE_NORMAL;	// ���b�Z�[�W�^�C�v
	g_titleOther.state = STATE_NONE;	// ���

	for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++)
	{// ���̏�����
		// �ʒu
		g_aClear[nCntClear].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �ړ���
		g_aClear[nCntClear].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �傫��
		g_aClear[nCntClear].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �F
		g_aClear[nCntClear].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// �g�p
		g_aClear[nCntClear].bUse = false;
		// �^�C�v
		g_aClear[nCntClear].type = TYPE_EMPTY;
	}

	// �т̐ݒ�(��ʒ���)
	// �ʒu
	g_aClear[MESSAGE_BAND].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, CLEAR_NOR_DESTPOS_Y, 0.0f);
	// �傫��
	g_aClear[MESSAGE_BAND].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// �F
	g_aClear[MESSAGE_BAND].col = CLEAR_NOR_BAND_COL;
	// �g�p
	g_aClear[MESSAGE_BAND].bUse = true;

	// ���b�Z�[�W�㕔�̐ݒ�
	// �ʒu(��)
	g_aClear[MESSAGE_UP].pos = D3DXVECTOR3(-CLEAR_NOR_WIDTH, CLEAR_NOR_DESTPOS_Y - CLEAR_NOR_HEIGHT / 4,0.0f);
	// �傫��
	g_aClear[MESSAGE_UP].size = D3DXVECTOR3(CLEAR_NOR_WIDTH, CLEAR_NOR_HEIGHT / 4, 0.0f);
	// �^�C�v
	g_aClear[MESSAGE_UP].type = TYPE_NORMAL;
	// ���b�Z�[�W�����̐ݒ�
	// �ʒu(�E)
	g_aClear[MESSAGE_DOWN].pos = D3DXVECTOR3(SCREEN_WIDTH + CLEAR_NOR_WIDTH, CLEAR_NOR_DESTPOS_Y + CLEAR_NOR_HEIGHT / 4, 0.0f);
	// �傫��
	g_aClear[MESSAGE_DOWN].size = D3DXVECTOR3(CLEAR_NOR_WIDTH, CLEAR_NOR_HEIGHT / 4, 0.0f);
	// �^�C�v
	g_aClear[MESSAGE_DOWN].type = TYPE_NORMAL;

	for (int nCntComplete = MESSAGE_ONE,nCntEquality = 0; nCntComplete < MESSAGE_ONE + CLEAR_COMP_NUM; nCntComplete++, nCntEquality++)
	{// �R���v���[�g���b�Z�[�W�̐ݒ�
		// �ʒu�̐ݒ�
		g_aClear[nCntComplete].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		// ���ɕ����𓙊Ԋu�Ŕz�u
		g_aClear[nCntComplete].pos.x += -((CLEAR_COMP_SPACE / 2) * (CLEAR_COMP_NUM - 1) + (CLEAR_COMP_DESTSIZE / 2) * (CLEAR_COMP_NUM - 1)) + nCntEquality * (CLEAR_COMP_DESTSIZE + CLEAR_COMP_SPACE);
		// �傫���̐ݒ�
		g_aClear[nCntComplete].size = D3DXVECTOR3(CLEAR_COMP_INITSIZE / 2, CLEAR_COMP_INITSIZE / 2, 0.0f);
		// �^�C�v
		g_aClear[nCntComplete].type = TYPE_COMPLETE;
	}

	ClearMakeVtx();		// ���_���̍쐬
}

//=============================================================================
//�N���A���b�Z�[�W�̏I������
//=============================================================================
void UninitClear(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_titleOther.pVtxBuff != NULL)
	{
		g_titleOther.pVtxBuff->Release();
		g_titleOther.pVtxBuff = NULL;
	}
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < MESSAGE_MAX; nCntTex++)
	{
		if (g_titleOther.pTexture[nCntTex] != NULL)
		{
			g_titleOther.pTexture[nCntTex]->Release();
			g_titleOther.pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//�N���A���b�Z�[�W�̍X�V����
//=============================================================================
void UpdateClear(void)
{
	VERTEX_2D*pVtx;  //���_���ւ̃|�C���^

	if (g_titleOther.pVtxBuff != NULL)
	{
		if (g_titleOther.state == STATE_START)
		{
			ClearBand();				// �т̏���

			if (g_titleOther.type == TYPE_NORMAL)
			{// �m�[�}�����b�Z�[�W�̏���
				NormalMessage();
			}
			else if (g_titleOther.type == TYPE_COMPLETE)
			{// �R���v���[�g���b�Z�[�W�̏���
				CompleteMessage();
			}

			// ���̃��Z�b�g(������g�p��)
			ClearReset();

			g_titleOther.nCntAll++;		// ���ʃJ�E���^�̉��Z

			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_titleOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++, pVtx += 4)
			{
				// �ړ��ʂ̍X�V
				g_aClear[nCntClear].pos += g_aClear[nCntClear].move;

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(-g_aClear[nCntClear].size.x, -g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
				pVtx[1].pos = D3DXVECTOR3(g_aClear[nCntClear].size.x, -g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
				pVtx[2].pos = D3DXVECTOR3(-g_aClear[nCntClear].size.x, g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
				pVtx[3].pos = D3DXVECTOR3(g_aClear[nCntClear].size.x, g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;

				// ���_�J���[
				pVtx[0].col = g_aClear[nCntClear].col;
				pVtx[1].col = g_aClear[nCntClear].col;
				pVtx[2].col = g_aClear[nCntClear].col;
				pVtx[3].col = g_aClear[nCntClear].col;
			}

			//���_�o�b�t�@���A�����b�N
			g_titleOther.pVtxBuff->Unlock();
		}
	}
}

//=============================================================================
// �т̏���
//=============================================================================
void ClearBand(void)
{
	if (g_titleOther.type == TYPE_NORMAL)
	{// �m�[�}���N���A�̎�
		// �o��
		if (g_titleOther.nCntAll > CLEAR_NOR_START &&
			g_titleOther.nCntAll < CLEAR_NOR_START + CLEAR_NOR_CONNECT + 60)
		{// �w��̎��ԂɂȂ�����
			if (g_aClear[MESSAGE_BAND].size.y < CLEAR_NORBAND_HEIGHT / 2)
			{// �w��̍����܂ő�������
				g_aClear[MESSAGE_BAND].size.y += 5.0f;
				// ������ʕ���
				g_aClear[MESSAGE_BAND].size.x = SCREEN_WIDTH;
			}
		}

		// �k��
		if (g_titleOther.nCntAll > CLEAR_NOR_START + CLEAR_NOR_CONNECT + 60)
		{// �w��̎��ԂɂȂ�����
			// �k��������
			g_aClear[MESSAGE_BAND].size.y -= 5.0f;
		}
	}
	else if (g_titleOther.type == TYPE_COMPLETE)
	{// �R���v���[�g�N���A�̎�
		if (g_titleOther.nCntDraw > CLEAR_COMP_CONNECT)
		{// �w��̎��ԂɂȂ�����
			g_aClear[MESSAGE_BAND].size.y -= 7.0f;
		}
		else if (g_aClear[MESSAGE_BAND].size.x < SCREEN_WIDTH)
		{// ��ʕ��܂ŐL�΂�
			g_aClear[MESSAGE_BAND].size.x += 35.0f;
			// �������ׂ��ݒ�
			g_aClear[MESSAGE_BAND].size.y = 3.0f;
		}
		else
		{// ��ʕ��ɂȂ�����
			if (g_aClear[MESSAGE_BAND].size.y < 240.0f)
			{// �w��̍����܂ō�������
				g_aClear[MESSAGE_BAND].size.y += 10.0f;
			}
		}
	}
}

//=============================================================================
// �m�[�}�����b�Z�[�W�̏���
//=============================================================================
void NormalMessage(void)
{
	if (!g_titleOther.bSwitch)
	{// �ړ��ؑւ�false�̎�
		if (g_aClear[MESSAGE_UP].pos.x > SCREEN_WIDTH / 2)
		{// �㕔�������ɗ�����
			// ���ꂼ��𒆉��Œ�~
			g_aClear[MESSAGE_UP].pos.x = SCREEN_WIDTH / 2;
			g_aClear[MESSAGE_DOWN].pos.x = SCREEN_WIDTH / 2;
			// ���ꂼ��̈ړ��ʂ�0��
			g_aClear[MESSAGE_UP].move.x = 0.0f;
			g_aClear[MESSAGE_DOWN].move.x = 0.0f;
			// �ړ��ؑւ�true��
			g_titleOther.bSwitch = true;

			// �����o��
			PlaySound(SOUND_LABEL_SENTOU);
		}
		else
		{
			if (g_titleOther.nCntAll > CLEAR_NOR_START + 6)
			{// �J�n���ԂɂȂ�����
				// �ړ��ʂ���
				g_aClear[MESSAGE_UP].move.x = CLEAR_NOR_SPEED;
				g_aClear[MESSAGE_DOWN].move.x = -CLEAR_NOR_SPEED;
			}
		}
	}
	else
	{
		// �\���J�E���^�����Z
		g_titleOther.nCntDraw++;

		if (g_titleOther.nCntDraw > CLEAR_NOR_CONNECT)
		{// �w�莞�ԕ\��������
		 // �ړ��ʂ���
			g_aClear[MESSAGE_UP].move.x = CLEAR_NOR_SPEED;
			g_aClear[MESSAGE_DOWN].move.x = -CLEAR_NOR_SPEED;

			if (g_aClear[MESSAGE_UP].pos.x > SCREEN_WIDTH + g_aClear[MESSAGE_UP].size.x / 2 &&
				g_aClear[MESSAGE_BAND].size.y <= 0.0f)
			{
				g_titleOther.state = STATE_RESET;
				StageStateSwat(false);
			}
		}
	}
}

//=============================================================================
// �R���v���[�g���b�Z�[�W�̏���
//=============================================================================
void CompleteMessage(void)
{
	if (g_titleOther.nCntAll > CLEAR_COMP_START)
	{
		for (int nCntComp = MESSAGE_ONE; nCntComp < MESSAGE_ONE + CLEAR_COMP_NUM; nCntComp++)
		{
			if (g_titleOther.nCntAll % CLEAR_COMP_INTERVAL == 0)
			{// �w��̊Ԋu�ɂȂ�����
				if (!g_aClear[nCntComp].bUse)
				{// �\������Ă��Ȃ����������ɕ\��
					g_aClear[nCntComp].bUse = true;
					PlaySound(SOUND_LABEL_SEIATU);
					break;
				}
			}

			if (g_aClear[nCntComp].bUse)
			{// �g�p����Ă���
				if (g_aClear[nCntComp].size.x > CLEAR_COMP_DESTSIZE / 2)
				{// �ړI�̑傫�����傫�����
					// �k��������
					g_aClear[nCntComp].size -= D3DXVECTOR3(CLEAR_COMP_REDUCT, CLEAR_COMP_REDUCT, 0.0f);
				}
			}

			if (g_titleOther.nCntDraw > CLEAR_COMP_CONNECT)
			{// �\�����Ԃ��߂�����
				// �k�������Ă���
				g_aClear[nCntComp].size.y -= 5.0f;

				if (g_aClear[nCntComp].size.y <= 0.0f &&
					g_aClear[MESSAGE_BAND].size.y <= 0.0f)
				{// �����A�тƂ��ɂ݂��Ȃ��Ȃ�����
					// �ҋ@��Ԃ�
					g_titleOther.state = STATE_RESET;

					STAGENUM stage = GetNumState();
					if (stage <= STAGENUM_2)
					{
						SetLoad();
					}
					else if (stage == STAGENUM_3)
					{// �N���A
						SetGameState(GAMESTATE_CLEAR);
					}
				}
			}
		}

		// �\���J�E���^�����Z
		g_titleOther.nCntDraw++;
	}
}

//=============================================================================
// ���b�Z�[�W�̋N��
//=============================================================================
void SetClearMessage(CLEARTYPE type)
{
	if (g_titleOther.state == STATE_NONE)
	{
		StageStateSwat(true);

		g_titleOther.type = type;			// ���b�Z�[�W�^�C�v�̐ݒ�
		g_titleOther.state = STATE_START;	// �����J�n

		if (type == TYPE_NORMAL)
		{
			for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++)
			{
				if (g_aClear[nCntClear].type == type)
				{
					g_aClear[nCntClear].bUse = true;
				}
			}
		}
	}
}

//=============================================================================
// ���̃��Z�b�g
//=============================================================================
void ClearReset(void)
{
	if (g_titleOther.state == STATE_RESET)
	{
		// �N���A���b�Z�[�W�̐ݒ���̏�����
		g_titleOther.nCntAll = 0;			// ���ʃJ�E���^
		g_titleOther.nCntDraw = 0;			// �\���J�E���^
		g_titleOther.bSwitch = false;		// �ړ��̐ؑ�
		g_titleOther.type = TYPE_NORMAL;	// ���b�Z�[�W�^�C�v
		g_titleOther.state = STATE_NONE;	// ���

		for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++)
		{// ���̏�����
		 // �ʒu
			g_aClear[nCntClear].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// �ړ���
			g_aClear[nCntClear].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// �傫��
			g_aClear[nCntClear].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// �F
			g_aClear[nCntClear].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �g�p
			g_aClear[nCntClear].bUse = false;
			// �^�C�v
			g_aClear[nCntClear].type = TYPE_EMPTY;
		}

		// �т̐ݒ�(��ʒ���)
		// �ʒu
		g_aClear[MESSAGE_BAND].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, CLEAR_NOR_DESTPOS_Y, 0.0f);
		// �傫��
		g_aClear[MESSAGE_BAND].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �F
		g_aClear[MESSAGE_BAND].col = CLEAR_NOR_BAND_COL;
		// �g�p
		g_aClear[MESSAGE_BAND].bUse = true;

		// ���b�Z�[�W�㕔�̐ݒ�
		// �ʒu(��)
		g_aClear[MESSAGE_UP].pos = D3DXVECTOR3(-CLEAR_NOR_WIDTH, CLEAR_NOR_DESTPOS_Y - CLEAR_NOR_HEIGHT / 4, 0.0f);
		// �傫��
		g_aClear[MESSAGE_UP].size = D3DXVECTOR3(CLEAR_NOR_WIDTH, CLEAR_NOR_HEIGHT / 4, 0.0f);
		// �^�C�v
		g_aClear[MESSAGE_UP].type = TYPE_NORMAL;
		// ���b�Z�[�W�����̐ݒ�
		// �ʒu(�E)
		g_aClear[MESSAGE_DOWN].pos = D3DXVECTOR3(SCREEN_WIDTH + CLEAR_NOR_WIDTH, CLEAR_NOR_DESTPOS_Y + CLEAR_NOR_HEIGHT / 4, 0.0f);
		// �傫��
		g_aClear[MESSAGE_DOWN].size = D3DXVECTOR3(CLEAR_NOR_WIDTH, CLEAR_NOR_HEIGHT / 4, 0.0f);
		// �^�C�v
		g_aClear[MESSAGE_DOWN].type = TYPE_NORMAL;

		for (int nCntComplete = MESSAGE_ONE, nCntEquality = 0; nCntComplete < MESSAGE_ONE + CLEAR_COMP_NUM; nCntComplete++, nCntEquality++)
		{// �R���v���[�g���b�Z�[�W�̐ݒ�
		 // �ʒu�̐ݒ�
			g_aClear[nCntComplete].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
			// ���ɕ����𓙊Ԋu�Ŕz�u
			g_aClear[nCntComplete].pos.x += -((CLEAR_COMP_SPACE / 2) * (CLEAR_COMP_NUM - 1) + (CLEAR_COMP_DESTSIZE / 2) * (CLEAR_COMP_NUM - 1)) + nCntEquality * (CLEAR_COMP_DESTSIZE + CLEAR_COMP_SPACE);
			// �傫���̐ݒ�
			g_aClear[nCntComplete].size = D3DXVECTOR3(CLEAR_COMP_INITSIZE / 2, CLEAR_COMP_INITSIZE / 2, 0.0f);
			// �^�C�v
			g_aClear[nCntComplete].type = TYPE_COMPLETE;
		}
	}
}

//=============================================================================
//�N���A���b�Z�[�W�̕`�揈��
//=============================================================================
void DrawClear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_titleOther.pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++)
	{
		if (g_aClear[nCntClear].bUse)
		{// �g�p����Ă��鎞
			// �e�N�X�`���̐ݒ�
			if (nCntClear != MESSAGE_BAND)
			{// �шȊO�̂�
				pDevice->SetTexture(0, g_titleOther.pTexture[nCntClear]);
			}
			else
			{// ��
				pDevice->SetTexture(0, NULL);
			}

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntClear, 2);
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void ClearMakeVtx(void)
{
	VERTEX_2D*pVtx;								// ���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	if (g_titleOther.pVtxBuff == NULL)
	{
		// �e�N�X�`���̓ǂݍ���
		// �m�[�}���N���A(�ǂ����������e�N�X�`���ł��B������Ƃ��邭�Ă����Ȃ�܂���)
		// ��
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_NOR_TEX,
			&g_titleOther.pTexture[MESSAGE_UP]);
		// ��
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_NOR_TEX,
			&g_titleOther.pTexture[MESSAGE_DOWN]);
		// �R���v���[�g�N���A
		// 1������
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_COMP_TEX_0,
			&g_titleOther.pTexture[MESSAGE_ONE]);
		// 2������
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_COMP_TEX_1,
			&g_titleOther.pTexture[MESSAGE_TWO]);
		// 3������
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_COMP_TEX_2,
			&g_titleOther.pTexture[MESSAGE_THREE]);
		// 4������
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_COMP_TEX_3,
			&g_titleOther.pTexture[MESSAGE_FOUR]);

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_CLEAR,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_titleOther.pVtxBuff,
			NULL);

		//���_�o�b�t�@�����b�N���Ē��_�f�[�^�ւ̃|�C���^���擾
		g_titleOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���̐ݒ�
		for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++, pVtx += 4)
		{
			// ���_���W
			pVtx[0].pos = D3DXVECTOR3(-g_aClear[nCntClear].size.x, -g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aClear[nCntClear].size.x, -g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aClear[nCntClear].size.x, g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aClear[nCntClear].size.x, g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;

			// ���_�J���[
			pVtx[0].col = g_aClear[nCntClear].col;
			pVtx[1].col = g_aClear[nCntClear].col;
			pVtx[2].col = g_aClear[nCntClear].col;
			pVtx[3].col = g_aClear[nCntClear].col;

			// rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// �e�N�X�`�����W
			if (nCntClear == MESSAGE_UP ||
				nCntClear == MESSAGE_DOWN)
			{// �m�[�}�����b�Z�[�W�̎�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f * nCntClear - 0.5f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f * nCntClear - 0.5f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, (float)nCntClear / 2);
				pVtx[3].tex = D3DXVECTOR2(1.0f, (float)nCntClear / 2);
			}
			else
			{// ���̑�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}

		//���_�o�b�t�@���A�����b�N
		g_titleOther.pVtxBuff->Unlock();
	}
}

