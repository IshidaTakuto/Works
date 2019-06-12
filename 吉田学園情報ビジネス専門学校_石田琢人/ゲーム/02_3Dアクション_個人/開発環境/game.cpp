//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "game.h"
#include "fade.h"
#include "input.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "meshField.h"
#include "meshCylinder.h"
#include "snow.h"
#include "rain.h"
#include "particle.h"
#include "pause.h"
#include "pausemenu.h"
#include "spiral.h"
#include "magic.h"
#include "petals.h"
#include "timer.h"
#include "score.h"
#include "ranking.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMYNUMBER	(10)		// �G�̈��ނ̐�
#define BLOCK_NUM		(15)		// �u���b�N�̐�
#define ITEM_NUM		(17)		// �A�C�e���̐�
#define GENERATOR_NUM	(5)

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;	// �Q�[���̏��
int g_nCounterGameState;				// ��ԊǗ��̃J�E���^�[
PAUSECHECK g_bPause;

MeshWallInfo g_aMeshWallInfo[12]
{
	{ D3DXVECTOR3(0.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), 0, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(0.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 0, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(2500.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), 1, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(2500.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 1, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(5000.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), 2, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(5000.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 2, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(7500.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), 3, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(7500.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 3, WALL_WIDTH, WALL_HEIGHT },

	{ D3DXVECTOR3(0.0f, 300.0f, 200.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), 4, WALL_WIDTH, WALL_HEIGHT * 2 },
	{ D3DXVECTOR3(2500.0f, 300.0f, 200.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), 4, WALL_WIDTH, WALL_HEIGHT * 2 },
	{ D3DXVECTOR3(5000.0f, 300.0f, 200.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), 4, WALL_WIDTH, WALL_HEIGHT * 2 },
	{ D3DXVECTOR3(7500.0f, 300.0f, 200.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), 4, WALL_WIDTH, WALL_HEIGHT * 2 }
};

ModelInfo g_aModelInfo[61]
{
	{ D3DXVECTOR3(-1250.0f,0.0f,-100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(-1250.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(-1250.0f,0.0f,100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(-900.0f,0.0f,-100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(-550.0f,0.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(-50.0f,0.0f,50.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(550.0f,0.0f,100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(1050.0f,0.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },

	{ D3DXVECTOR3(1550.0f,0.0f,-120.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(1850.0f,0.0f,70.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(2300.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(2850.0f,0.0f,-90.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(3300.0f,0.0f,150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(3600.0f,0.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },

	{ D3DXVECTOR3(4000.0f,0.0f,100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(4750.0f,0.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(5300.0f,0.0f,150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(5600.0f,0.0f,10.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(6100.0f,0.0f,-130.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2, MODELTYPE_OBJECT },

	{ D3DXVECTOR3(6800.0f,0.0f,30.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 3, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(7600.0f,0.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 3, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(8300.0f,0.0f,100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 3, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(8750.0f,0.0f,150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 3, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(8750.0f,0.0f,-150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 3, MODELTYPE_OBJECT },

	{ D3DXVECTOR3(-500.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(-100.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(100.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(260.0f,60.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(530.0f,25.0f,50.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(950.0f,25.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(1250.0f,25.0f,120.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(1500.0f,60.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(1750.0f,25.0f,30.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(1980.0f,60.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(2250.0f,25.0f,150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(2500.0f,60.0f,50.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(2750.0f,25.0f,-130.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(2980.0f,25.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(3180.0f,25.0f,10.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(3360.0f,25.0f,90.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(3650.0f,60.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(3920.0f,25.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(4240.0f,25.0f,40.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(4590.0f,25.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(4750.0f,60.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(4960.0f,25.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(5350.0f,25.0f,30.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(5507.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(5950.0f,60.0f,-100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(6350.0f,25.0f,-150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(6590.0f,25.0f,10.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(6750.0f,25.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(6900.0f,25.0f,80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(7050.0f,25.0f,60.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(7350.0f,25.0f,-40.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(7500.0f,60.0f,-120.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(7850.0f,25.0f,70.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(8050.0f,60.0f,90.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(8200.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(8400.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(8600.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM }

};

//=============================================================================
// �Q�[������������
//=============================================================================
void InitGame(void)
{
	// ���b�V���V�����_�[�̏���������
	//InitMeshCylinder();

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// ���b�V���E�H�[���̏���������
	for (int nCntWall = 0; nCntWall < sizeof g_aMeshWallInfo / sizeof(MeshWallInfo); nCntWall++)
	{
		InitMeshWall(g_aMeshWallInfo[nCntWall].pos, g_aMeshWallInfo[nCntWall].rot, g_aMeshWallInfo[nCntWall].nType, g_aMeshWallInfo[nCntWall].fWidth, g_aMeshWallInfo[nCntWall].fHeight);
	}

	// �e����������
	InitBullet();

	InitSpiral();
	for (int nCntSpiral = 0; nCntSpiral < SPIRAL_RING; nCntSpiral++)
	{
		SetSpiral(D3DXVECTOR3(8750.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SPIRALTYPE_RING);
	}

	// �e�̏���������
	InitShadow();

	// �p�[�e�B�N���̏���������
	InitParticle();

	// �v���C���[�̏���������
	InitPlayer();

	// ���f���̏���������
	InitModel();
	for (int nCntModelInfo = 0; nCntModelInfo < sizeof g_aModelInfo / sizeof(ModelInfo); nCntModelInfo++)
	{
		SetModel(g_aModelInfo[nCntModelInfo].pos, g_aModelInfo[nCntModelInfo].rot, g_aModelInfo[nCntModelInfo].nType, g_aModelInfo[nCntModelInfo].type);
	}

	// ��������������
	InitExplosion();

	// �G�t�F�N�g����������
	InitEffect();
	SetEffect(D3DXVECTOR3(0.0f, 60.0f, 400.0f), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), 100.0f, 0, EFFECTSTATE_MOON);
	SetEffect(D3DXVECTOR3(0.0f, 60.0f, 400.0f), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), 130.0f, 0, EFFECTSTATE_NONE);

	// ���@����������
	InitMagic();

	// �Ԃт珉��������
	InitPetals();

	// ��̏���������
	InitSnow();

	// �J�̏���������
	InitRain();

	// �^�C�}�[����������
	InitTimer();

	// �X�R�A����������
	InitScore();

	// �|�[�Y����������
	InitPause();

	// �|�[�Y���j���[����������
	InitPmenu();

	g_gameState = GAMESTATE_NORMAL;	// �ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;

	g_bPause.bPause = false;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// ���b�V���V�����_�[�̏I������
	//UninitMeshCylinder();

	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// ���b�V���E�H�[���̏I������
	UninitMeshWall();

	// �e�I������
	UninitBullet();

	// �v���C���[�̏I������
	UninitPlayer();

	// ���f���̏I������
	UninitModel();

	// �r���{�[�h�̏I������
	UninitBullet();

	// �e�̏I������
	UninitShadow();

	// �����I������
	UninitExplosion();

	// �G�t�F�N�g�I������
	UninitEffect();

	// ���@�̏I������
	UninitMagic();

	// �p�[�e�B�N���̏I������
	UninitParticle();

	UninitSpiral();

	// �Ԃт�I������
	UninitPetals();

	// ��̏I������
	UninitSnow();

	// �J�̏I������
	UninitRain();

	// �^�C�}�[�I������
	UninitTimer();

	// �X�R�A�I������
	UninitScore();

	// �|�[�Y�I������
	UninitPause();

	// �|�[�Y���j���[�I������
	UninitPmenu();
}

//=============================================================================
// �Q�[���X�V����
//=============================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true)
	{// �|�[�YON/OFF
		g_bPause.bPause = g_bPause.bPause ? false : true;
	}

	if (g_bPause.bPause == false)
	{
		// ���b�V���V�����_�[�̍X�V����
		//UpdateMeshCylinder();

		// ���b�V���t�B�[���h�̍X�V����
		UpdateMeshField();

		// ���b�V���E�H�[���̍X�V����
		UpdateMeshWall();

		// �e�X�V����
		UpdateBullet();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		UpdateModel();

		// �r���{�[�h�̍X�V����
		UpdateBullet();

		// �e�̍X�V����
		UpdateShadow();

		// �����X�V����
		UpdateExplosion();

		// �G�t�F�N�g�X�V����
		UpdateEffect();

		// ���@�̍X�V����
		UpdateMagic();

		// �����̍X�V����
		UpdateSpiral();

		// �Ԃт�X�V����
		UpdatePetals(PETALSTYPE_SAKURA);

		// �^�C�}�[�X�V����
		UpdateTimer();

		// �X�R�A�X�V����
		UpdateScore();

		// ��̍X�V����
		UpdateSnow();

		// �J�̍X�V����
		UpdateRain();

		// �p�[�e�B�N���̍X�V����
		UpdateParticle();
	}
	else
	{
		// �|�[�Y�X�V����
		UpdatePause();

		// �|�[�Y���j���[�X�V����
		UpdatePmenu();
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		break;

	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;

			int nScore = GetScore();

			SetRanking(nScore);

			// ���(���[�h)�̐ݒ�
			SetFade(MODE_GAMEOVER);
		}
		break;

	case GAMESTATE_CLEAR:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			int nScore = GetScore();

			SetRanking(nScore);

			g_gameState = GAMESTATE_NONE;

			// ���(���[�h)�̐ݒ�
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//=============================================================================
// �Q�[���`�揈��
//=============================================================================
void DrawGame(void)
{
	// ���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// ���b�V���E�H�[���̕`�揈��
	DrawMeshWall();

	// ���b�V���V�����_�[�̕`�揈��
	//DrawMeshCylinder();

	// �e�̕`�揈��
	DrawShadow();

	// �e�`�揈��
	DrawBullet();

	// �����`�揈��
	DrawExplosion();

	// �G�t�F�N�g�`�揈��
	DrawEffect();

	// �����̕`�揈��
	DrawSpiral();

	// ���@�̕`�揈��
	DrawMagic();

	// ���f���̕`�揈��
	DrawModel();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �r���{�[�h�̕`�揈��
	DrawBullet();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	// �Ԃт�`�揈��
	DrawPetals();

	// ��̍X�V����
	DrawSnow();

	// �J�̍X�V����
	DrawRain();

	// �^�C�}�[�`�揈��
	DrawTimer();

	// �X�R�A�`�揈��
	DrawScore();

	if (g_bPause.bPause == true)
	{
		// �|�[�Y�`�揈��
		DrawPause();

		// �|�[�Y���j���[�`�揈��
		DrawPmenu();
	}
}

//=============================================================================
// �Q�[���̏�Ԃ̐ݒ�
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//=============================================================================
// �Q�[���̏�Ԃ̎擾
//=============================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//=============================================================================
// �|�[�Y��Ԃ̎擾
//=============================================================================
PAUSECHECK *GetPauseMenu(void)
{
	return &g_bPause;
}
