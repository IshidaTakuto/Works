//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "input.h"
#include "ranking.h"
#include "meshbg.h"
#include "score.h"
#include "killLog.h"
#include "meshField.h"
#include "shadow.h"
#include "player.h"
#include "enemy.h"
#include "object.h"
#include "object2.h"
#include "object3.h"
#include "objectnot.h"
#include "pause.h"
#include "petals.h"
#include "Blood.h"
#include "wall.h"
#include "particle.h"
#include "rain.h"
#include "particle2D.h"
#include "effect.h"
#include "clear.h"
#include "life.h"
#include "timer.h"
#include "combo.h"
#include "remnantsEnemy.h"
#include "load.h"
#include "gauge.h"
#include "dragon.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_NUM_APPEAR	(10)	// �o��������ő吔

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;		//�Q�[���̏��
int g_nCounterGameState;					//��ԊǗ��̃J�E���^�[
PAUSECHECK g_bPause;						//�|�[�Y
STAGENUM g_StageState;						//�X�e�[�W���
int	g_nCntSetStage;							//�X�e�[�W�Z�b�g�J�E���^
bool g_bStageSwat;							// �X�e�[�W���ς��n�߂ƏI���
EnemyInfo g_aEnemyInfo[30];					// �G�̏��

//=============================================================================
// �Q�[������������
//=============================================================================
void InitGame(void)
{
	g_nCounterGameState = 0;
	//�J�n�X�e�[�W
	g_StageState = STAGENUM_1;			// �X�e�[�W���

	//�J�����̏�����
	InitCamera();

	// �L�����O����������
	InitKillLog();

	//�w�i�̏���������
	InitMeshBg();

	// �t�B�[���h����������
	InitMeshField();

	// �p�[�e�B�N������������
	InitParticle();

	// �X�R�A����������
	InitScore();

	// �e����������
	InitShadow();

	//�v���C���[����������
	InitPlayer();
	InitDragon();

	//�G�̏���������
	InitEnemy();

	// ���̏���������
	InitBlood();

	//�I�u�W�F�N�g�̏���������
	InitObject();
	InitObject2();
	InitObject3();
	InitObjectNot();

	// �Ԃт珉��������
	InitPetals();

	// �Ǐ���������
	InitWall();

	// �|�[�Y����������
	InitPause();

	if (g_StageState == STAGENUM_1)
	{// �J����������
		InitRain();
	}

	// 2D�p�[�e�B�N������������
	InitParticle2D();

	// �G�t�F�N�g����������
	InitEffect();

	// �N���A���b�Z�[�W����������
	InitClear();

	// ���C�t����������
	InitLife();

	// �^�C�}�[����������
	InitTimer();

	// �R���{����������
	InitCombo(D3DXVECTOR3(200, 200, 0));

	// �G�c������������
	InitRemnants();

	// �ϐg�Q�[�W����������
	InitGauge();

	// �}�b�v�ړ�����������
	InitLoad();

	//�ǂ̔z�u
	//��
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 870.0f), D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f));
	//�E
	SetWall(D3DXVECTOR3(870.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f));
	//��
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -870.0f), D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f));
	//��
	SetWall(D3DXVECTOR3(-870.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f));

	//�ʏ��Ԃɐݒ�
	g_gameState = GAMESTATE_NORMAL;

	//�J�E���^������
	g_nCntSetStage = 0;

	g_bPause.bPause = false;
	g_bStageSwat = false;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	//�J�����̏I������
	UninitCamera();

	// �L�����O�I������
	UninitKillLog();

	//�w�i�̏I������
	UninitMeshBg();

	// �t�B�[���h�I������
	UninitMeshField();

	// �X�R�A�I������
	UninitScore();

	// �e�I������
	UninitShadow();

	// �v���C���[�I������
	UninitPlayer();
	UninitDragon();

	// �G�̏I������
	UninitEnemy();

	// �p�[�e�B�N���I������
	UninitParticle();

	// 2D�p�[�e�B�N���I������
	UninitParticle2D();

	//�I�u�W�F�N�g�̏I������
	UninitObject();
	UninitObject2();
	UninitObject3();
	UninitObjectNot();

	// �Ԃт�I������
	UninitPetals();

	// �ǏI������
	UninitWall();

	// ���f���I������
	UninitScore();

	// ���̏I������
	UninitBlood();

	// �|�[�Y�I������
	UninitPause();

	if (g_StageState == STAGENUM_1)
	{// �J�I������
		UninitRain();
	}

	// �G�t�F�N�g�I������
	UninitEffect();

	// �N���A���b�Z�[�W�I������
	UninitClear();

	// ���C�t�I������
	UninitLife();

	// �^�C�}�[�I������
	UninitTimer();

	// �R���{�I������
	UninitCombo();

	// �G�c���I������
	UninitRemnants();

	// �ϐg�Q�[�W�I������
	UninitGauge();

	// �}�b�v�ړ��I������
	UninitLoad();
}

//=============================================================================
// �Q�[���X�V����
//=============================================================================
void UpdateGame(void)
{
	g_bStageSwat;
	bool bFade = GetFadeNow();
	LOADFADE loadFade = GetLoadFade();

	if (false == g_bStageSwat && false == bFade && LOADFADE_NONE == loadFade)
	{// �X�e�[�W�؂�ւ����Ȃǂ̓|�[�Y����
		if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadTrigger(DIJS_BUTTON_7, 0) == true)
		{// �|�[�YON/OFF
			PlaySound(SOUND_LABEL_PAUSE);
			g_bPause.bPause = g_bPause.bPause ? false : true;
		}
	}

	if (g_bPause.bPause == false)
	{
		//�J�����̍X�V����
		UpdateCamera();

		// �L�����O�X�V����
		UpdateKillLog();

		// �t�B�[���h�X�V����
		UpdateMeshBg();

		// �t�B�[���h�X�V����
		UpdateMeshField();

		// �X�R�A�X�V����
		UpdateScore();

		// �e�X�V����
		UpdateShadow();

		// �X�e�[�W�ݒ�
		SetStage();

		// �v���C���[�X�V����
		UpdatePlayer();
		UpdateDragon();

		// �G�̍X�V����
		UpdateEnemy();

		// �p�[�e�B�N���X�V����
		UpdateParticle();

		// 2D�p�[�e�B�N���X�V����
		UpdateParticle2D();

		// ���̍X�V����
		UpdateBlood();

		//�I�u�W�F�N�g�̍X�V����
		UpdateObject();
		UpdateObject2();
		UpdateObject3();
		UpdateObjectNot();

		// �Ԃт�X�V����
		UpdatePetals();

		// �ǏI������
		UpdateWall();

		if (g_StageState == STAGENUM_1)
		{// �J�X�V����
			UpdateRain();
		}

		// �G�t�F�N�g�X�V����
		UpdateEffect();

		// �N���A���b�Z�[�W�X�V����
		UpdateClear();

		// �R���{�X�V����
		UpdateCombo();

		// �G�c���X�V����
		UpdateRemnants();

		// �^�C�}�[�X�V����
		UpdateTimer();

		// ���C�t�X�V����
		UpdateLife();

		// �ϐg�Q�[�W�X�V����
		UpdateGauge();

		// �}�b�v�ړ��X�V����
		UpdateLoad();

		// �G�̐���
		for (int nCntEnemyInfo = 0; nCntEnemyInfo < sizeof g_aEnemyInfo / sizeof(EnemyInfo); nCntEnemyInfo++)
		{
			int nNumEnemy = GetNumEnemy();	// �g���Ă���G�̐��̎擾
			if (ENEMY_NUM_APPEAR > nNumEnemy)
			{// ���̐��܂œG���o��������
				if (!g_aEnemyInfo[nCntEnemyInfo].bUse)
				{// �g���Ă��Ȃ������g�p
					SetEnemy(g_aEnemyInfo[nCntEnemyInfo].pos, g_aEnemyInfo[nCntEnemyInfo].nType);
					g_aEnemyInfo[nCntEnemyInfo].bUse = true;
				}
			}
			else
			{// ��萔�ɂȂ���
				break;
			}
		}
	}
	else
	{
		// �|�[�Y�X�V����
		UpdatePause();
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
			SetFadeMode(MODE_GAMEOVER);
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
			SetFadeMode(MODE_RESULT);
		}
		break;
	}
}

//=============================================================================
// �Q�[���`�揈��
//=============================================================================
void DrawGame(void)
{
	//�J�����̕`�揈��
	SetCamera(CAMERATYPE_GAME);

	// �t�B�[���h�`�揈��
	DrawMeshField();

	// �t�B�[���h�`�揈��
	//DrawMeshBg();

	// ���̕`�揈��
	DrawBlood();

	// �e�`�揈��
	DrawShadow();

	//�v���C���[�`�揈��
	PlayerType type = GetPlayerType();
	if (type == TYPE_HUMAN)
	{
		DrawPlayer();
	}
	else
	{
		DrawDragon();
	}

	// �G�̕`�揈��
	DrawEnemy();

	//�I�u�W�F�N�g�̕`�揈��
	DrawObject();
	DrawObject2();
	DrawObject3();
	DrawObjectNot();

	// �Ǖ`�揈��
	DrawWall();

	// �Ԃт�`�揈��
	DrawPetals();

	// �p�[�e�B�N���`�揈��
	DrawParticle();

	// 2D�p�[�e�B�N���`�揈��
	DrawParticle2D();

	// �G�t�F�N�g�`�揈��
	DrawEffect();

	if (g_StageState == STAGENUM_1)
	{// �J�`�揈��
		DrawRain();
	}

	// �^�C�}�[�`�揈��
	DrawTimer();

	// �X�R�A�`�揈��
	DrawScore();

	// �L�����O�`�揈��
	DrawKillLog();

	// �N���A���b�Z�[�W�`�揈��
	DrawClear();

	// ���C�t�`�揈��
	DrawLife();

	// �R���{�`�揈��
	DrawCombo();

	// �G�c���`�揈��
	DrawRemnants();

	// �ϐg�Q�[�W�`�揈��
	DrawGauge();

	// �}�b�v�ړ��`�揈��
	DrawLoad();

	if (g_bPause.bPause == true)
	{
		// �|�[�Y�`�揈��
		DrawPause();
	}
}

//=============================================================================
// �Q�[���̏�Ԃ̐ݒ�
//=============================================================================
void SetGameState(GAMESTATE state)
{
	if (g_StageState != state)
	{
		DeletePlayer();
	}
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
// �}�b�v�̎擾
//=============================================================================
STAGENUM GetNumState(void)
{
	return g_StageState;
}

//=============================================================================
// �|�[�Y��Ԃ̎擾
//=============================================================================
PAUSECHECK *GetPauseMenu(void)
{
	return &g_bPause;
}

//=========================================================================================================================
// �Q�[���ݒ菈��
//=========================================================================================================================
void SetStageState(STAGENUM stage)
{

	if (g_StageState != stage)
	{
		DeletePlayer();		//�v���C���[�̍폜
		DeleteEnemy();		//�G�̍폜
		DeleteObject();		//�؂̍폜
		DeleteObject2();	//��̍폜
		DeleteObject3();	//�r���̍폜
		DeleteNotObject();	//�؂̑��̍폜
		DeleteBlood();		//���̍폜
		DeleteCamera();


		// �G�t�F�N�g�̍폜
		DeletePetals();
		DeleteParticle();
		DeleteEffect();

		// ���Ԃ̃��Z�b�g
		ResetTimer();

		// �G�c���̃��Z�b�g
		SetRemnants(0);
	}

	// ��Ԃ��L��
	g_StageState = stage;
	g_nCntSetStage = 0;
}

//=========================================================================================================================
// �}�b�v�̐ݒ�
//=========================================================================================================================
void SetStage(void)
{
	if (g_nCntSetStage == 0)
	{
		//=====================================
		//	          �X�e�[�W�P
		//=====================================
		if (g_StageState == STAGENUM_1)
		{//�ݒu��
			// �O�̃X�e�[�W��BGM���~�߂�
			StopSound(SOUND_LABEL_STAGE2);
			StopSound(SOUND_LABEL_STAGE3);

			// �X�e�[�W1��BGM
			PlaySound(SOUND_LABEL_STAGE1);
			PlaySound(SOUND_LABEL_RAIN);

		 //================================================================================
		 // �����蔻��̂���I�u�W�F�N�g
		 //================================================================================
			for (int nCount = 0; nCount < 5; nCount++)
			{
				//�E��
				SetObject(D3DXVECTOR3(800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObject(D3DXVECTOR3(800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//����
				SetObject(D3DXVECTOR3(-800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObject(D3DXVECTOR3(-800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//���
				SetObject(D3DXVECTOR3(nCount * 200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObject(D3DXVECTOR3(nCount * -200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//����
				SetObject(D3DXVECTOR3(nCount * -200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObject(D3DXVECTOR3(nCount * 200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}//40

			 //�E��p
			SetObject(D3DXVECTOR3(640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(660.0f, 0.0f, 540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�����p
			SetObject(D3DXVECTOR3(-640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-660.0f, 0.0f, -540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�E���p
			SetObject(D3DXVECTOR3(640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(660.0f, 0.0f, -540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//����p
			SetObject(D3DXVECTOR3(-640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-660.0f, 0.0f, 540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//�}�b�v�ɎU��΂߂郂�f��
			SetObject(D3DXVECTOR3(-120.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-320.0f, 0.0f, -240.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(340.0f, 0.0f, 40.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(160.0f, 0.0f, -320.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//���݌� : 72

			//================================================================================
			//�����蔻��̂Ȃ��I�u�W�F�N�g
			//================================================================================
			for (int nCount = 0; nCount < 5; nCount++)
			{
				//�E��
				SetObjectNot(D3DXVECTOR3(800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObjectNot(D3DXVECTOR3(800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//����
				SetObjectNot(D3DXVECTOR3(-800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObjectNot(D3DXVECTOR3(-800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//���
				SetObjectNot(D3DXVECTOR3(nCount * 200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObjectNot(D3DXVECTOR3(nCount * -200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//����
				SetObjectNot(D3DXVECTOR3(nCount * -200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObjectNot(D3DXVECTOR3(nCount * 200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}//40

			 //�E��p
			SetObjectNot(D3DXVECTOR3(640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(660.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�����p
			SetObjectNot(D3DXVECTOR3(-640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-660.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�E���p
			SetObjectNot(D3DXVECTOR3(640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(660.0f, 0.0f, -540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//����p
			SetObjectNot(D3DXVECTOR3(-640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-660.0f, 0.0f, 540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//�}�b�v�ɎU��΂߂郂�f��
			SetObjectNot(D3DXVECTOR3(-120.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-320.0f, 0.0f, -240.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(340.0f, 0.0f, 40.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(160.0f, 0.0f, -320.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//���݌� : 72

			//�E��p
			//SetObject(D3DXVECTOR3(640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�����p
			//SetObject(D3DXVECTOR3(-640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�E���p
			//SetObject(D3DXVECTOR3(640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//����p
			//SetObject(D3DXVECTOR3(-640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//==================================
			//             �G�̔z�u
			//==================================
			g_aEnemyInfo[0]  = { D3DXVECTOR3(600.0f, 50.0f, 600.0f), 0		};
			g_aEnemyInfo[1]  = { D3DXVECTOR3(-600.0f, 50.0f, 600.0f), 0		};
			g_aEnemyInfo[2]  = { D3DXVECTOR3(600.0f, 50.0f, -600.0f), 0		};
			g_aEnemyInfo[3]  = { D3DXVECTOR3(-600.0f, 50.0f, -600.0f), 0	};
			g_aEnemyInfo[4]  = { D3DXVECTOR3(300.0f, 50.0f, -100.0f), 0		};
			g_aEnemyInfo[5]  = { D3DXVECTOR3(200.0f, 50.0f, 300.0f), 0		};
			g_aEnemyInfo[6]  = { D3DXVECTOR3(500.0f, 50.0f, -400.0f), 0		};
			g_aEnemyInfo[7]  = { D3DXVECTOR3(-150.0f, 50.0f, -200.0f), 0	};
			g_aEnemyInfo[8]  = { D3DXVECTOR3(-300.0f, 50.0f, 200.0f), 0		};
			g_aEnemyInfo[9]  = { D3DXVECTOR3(50.0f, 50.0f, -100.0f), 0		};
			g_aEnemyInfo[10] = { D3DXVECTOR3(-200.0f, 50.0f, 500.0f), 0		};
			g_aEnemyInfo[11] = { D3DXVECTOR3(-300.0f, 50.0f, 450.0f), 0		};
			g_aEnemyInfo[12] = { D3DXVECTOR3(0.0f, 50.0f, -300.0f), 0		};
			g_aEnemyInfo[13] = { D3DXVECTOR3(0.0f, 50.0f, 500.0f), 0		};
			g_aEnemyInfo[14] = { D3DXVECTOR3(500.0f, 50.0f, 0.0f), 0		};
			g_aEnemyInfo[15] = { D3DXVECTOR3(-200.0f, 50.0f, 50.0f), 0		};
			g_aEnemyInfo[16] = { D3DXVECTOR3(-500.0f, 50.0f, -100.0f), 0	};
			g_aEnemyInfo[17] = { D3DXVECTOR3(200.0f, 50.0f, 90.0f), 0		};
			g_aEnemyInfo[18] = { D3DXVECTOR3(20.0f, 50.0f, -400.0f), 0		};
			g_aEnemyInfo[19] = { D3DXVECTOR3(120.0f, 50.0f, -320.0f), 0		};
			g_aEnemyInfo[20] = { D3DXVECTOR3(-320.0f, 50.0f, -120.0f), 0	};
			g_aEnemyInfo[21] = { D3DXVECTOR3(-400.0f, 50.0f, 0.0f), 0		};
			g_aEnemyInfo[22] = { D3DXVECTOR3(-400.0f, 50.0f, 200.0f), 0		};
			g_aEnemyInfo[23] = { D3DXVECTOR3(300.0f, 50.0f, 600.0f), 0		};
			g_aEnemyInfo[24] = { D3DXVECTOR3(-300.0f, 50.0f, 600.0f), 0		};
			g_aEnemyInfo[25] = { D3DXVECTOR3(-600.0f, 50.0f, 300.0f), 0		};
			g_aEnemyInfo[26] = { D3DXVECTOR3(-50.0f, 50.0f, 400.0f), 0		};
			g_aEnemyInfo[27] = { D3DXVECTOR3(150.0f, 50.0f, -200.0f), 0		};
			g_aEnemyInfo[28] = { D3DXVECTOR3(250.0f, 50.0f, 0.0f), 0		};
			g_aEnemyInfo[29] = { D3DXVECTOR3(0.0f, 50.0f, 120.0f), 0		};

			for (int nCntEnemyInfo = 0; nCntEnemyInfo < 30; nCntEnemyInfo++)
			{// �g�p���Ă��Ȃ���Ԃɂ���
				g_aEnemyInfo[nCntEnemyInfo].bUse = false;
			}

			SetNumEnemy(30);	// �G�̑����̐ݒ�
			SetRemnants(30);	// �G�̎c���̐ݒ�
		}
		//=====================================
		//	          �X�e�[�W�Q
		//=====================================
		else if (g_StageState == STAGENUM_2)
		{//�ݒu��
		 //�؂̃I�u�W�F�N�g�������Ɣ��肪���������Ȃ�o�O����
			// �O�̃X�e�[�W��BGM���~�߂�
			StopSound(SOUND_LABEL_STAGE1);
			StopSound(SOUND_LABEL_RAIN);

			// �X�e�[�W2��BGM
			PlaySound(SOUND_LABEL_STAGE2);

		 //================================================================================
		 // �����蔻��̂���I�u�W�F�N�g
		 //================================================================================
			for (int nCount = 0; nCount < 9; nCount++)
			{
				//�E��
				SetObject2(D3DXVECTOR3(800.0f, 0.0f, nCount * 100.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				SetObject2(D3DXVECTOR3(800.0f, 0.0f, nCount * -100.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				//����
				SetObject2(D3DXVECTOR3(-800.0f, 0.0f, nCount * 100.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				SetObject2(D3DXVECTOR3(-800.0f, 0.0f, nCount * -100.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				//���
				SetObject2(D3DXVECTOR3(nCount * 100.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				SetObject2(D3DXVECTOR3(nCount * -100.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				//����
				SetObject2(D3DXVECTOR3(nCount * -100.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				SetObject2(D3DXVECTOR3(nCount * 100.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			}//72

			 //�}�b�v�ɎU��΂߂郂�f��
			SetObject2(D3DXVECTOR3(300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(-300.0f, 0.0f, 150.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(600.0f, 0.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(0.0f, 0.0f, -320.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(-560.0f, 0.0f, -560.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(-430.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(330.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(540.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(0.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(-580.0f, 0.0f, 320.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(680.0f, 0.0f, 680.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));

			//���݌� : 83

			//==================================
			//             �G�̔z�u
			//==================================
			g_aEnemyInfo[0]  = { D3DXVECTOR3(600.0f, 50.0f, 600.0f), 0 };
			g_aEnemyInfo[1] = { D3DXVECTOR3(600.0f, 50.0f, -600.0f), 0 };
			g_aEnemyInfo[2] = { D3DXVECTOR3(-600.0f, 50.0f, 600.0f), 0 };
			g_aEnemyInfo[3] = { D3DXVECTOR3(-480.0f, 50.0f, -580.0f), 0 };
			g_aEnemyInfo[4] = { D3DXVECTOR3(600.0f, 50.0f, 0.0f), 0 };
			g_aEnemyInfo[5] = { D3DXVECTOR3(-600.0f, 50.0f, 0.0f), 0 };
			g_aEnemyInfo[6] = { D3DXVECTOR3(0.0f, 50.0f, 600.0f), 0 };
			g_aEnemyInfo[7] = { D3DXVECTOR3(0.0f, 50.0f, -600.0f), 0 };
			g_aEnemyInfo[8] = { D3DXVECTOR3(-320.0f, 50.0f, 600.0f), 0 };
			g_aEnemyInfo[9] = { D3DXVECTOR3(-120.0f, 50.0f, -600.0f), 0 };
			g_aEnemyInfo[10] = { D3DXVECTOR3(600.0f, 50.0f, 300.0f), 0 };
			g_aEnemyInfo[11] = { D3DXVECTOR3(200.0f, 50.0f, -400.0f), 0 };
			g_aEnemyInfo[12] = { D3DXVECTOR3(-150.0f, 50.0f, 200.0f), 0 };
			g_aEnemyInfo[13] = { D3DXVECTOR3(-50.0f, 50.0f, -200.0f), 0 };
			g_aEnemyInfo[14] = { D3DXVECTOR3(400.0f, 50.0f, 280.0f), 0 };
			g_aEnemyInfo[15] = { D3DXVECTOR3(220.0f, 50.0f, 0.0f), 0 };
			g_aEnemyInfo[16] = { D3DXVECTOR3(220.0f, 50.0f, 220.0f), 0 };
			g_aEnemyInfo[17] = { D3DXVECTOR3(10, 50.0f, 100.0f), 0 };
			g_aEnemyInfo[18] = { D3DXVECTOR3(-100.0f, 50.0f, -100.0f), 0 };
			g_aEnemyInfo[19] = { D3DXVECTOR3(-510.0f, 50.0f, -300), 0 };
			g_aEnemyInfo[20] = { D3DXVECTOR3(30.0f, 50.0f, 50.0f), 0 };
			g_aEnemyInfo[21] = { D3DXVECTOR3(50.0f, 50.0f, -50.0f), 0 };
			g_aEnemyInfo[22] = { D3DXVECTOR3(-50.0f, 50.0f, 30.0f), 0 };
			g_aEnemyInfo[23] = { D3DXVECTOR3(-50.0f, 50.0f, -50.0f), 0 };
			g_aEnemyInfo[24] = { D3DXVECTOR3(150.0f, 50.0f, 20.0f), 0 };
			g_aEnemyInfo[25] = { D3DXVECTOR3(700.0f, 50.0f, 130.0f), 0 };
			g_aEnemyInfo[26] = { D3DXVECTOR3(0.0f, 50.0f, 220.0f), 0 };
			g_aEnemyInfo[27] = { D3DXVECTOR3(10, 50.0f, 100.0f), 0 };
			g_aEnemyInfo[28] = { D3DXVECTOR3(-100.0f, 50.0f, -190.0f), 0 };
			g_aEnemyInfo[29] = { D3DXVECTOR3(-380.0f, 50.0f, -460), 0 };

			for (int nCntEnemyInfo = 0; nCntEnemyInfo < 30; nCntEnemyInfo++)
			{// �g�p���Ă��Ȃ���Ԃɂ���
				g_aEnemyInfo[nCntEnemyInfo].bUse = false;
			}

			SetNumEnemy(30);	// �G�̑����̐ݒ�
			SetRemnants(30);	// �G�̎c���̐ݒ�
		}
		//=====================================
		//	          �X�e�[�W3
		//=====================================
		else if (g_StageState == STAGENUM_3)
		{//�ݒu��
			// �O�̃X�e�[�W��BGM���~�߂�
			StopSound(SOUND_LABEL_STAGE2);

			// �X�e�[�W3��BGM
			PlaySound(SOUND_LABEL_STAGE3);

		 //����E
			SetObject3(D3DXVECTOR3(750.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(550.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//���㍶
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-550.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-750.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�����E
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(550.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(750.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//������
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-550.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-750.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�c����
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, -550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, -750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�c��E
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, 550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, 750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�c��E
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, -550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, -750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//�c��E
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, 550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, 750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//==================================
			//             �G�̔z�u
			//==================================
			g_aEnemyInfo[0]  = { D3DXVECTOR3(620.0f, 50.0f, -100.0f), 0 };
			g_aEnemyInfo[1] = { D3DXVECTOR3(-620.0f, 50.0f, 230.0f), 0 };
			g_aEnemyInfo[2] = { D3DXVECTOR3(0.0f, 50.0f, 620.0f), 0 };
			g_aEnemyInfo[3] = { D3DXVECTOR3(0.0f, 50.0f, -620.0f), 0 };
			g_aEnemyInfo[4] = { D3DXVECTOR3(580.0f, 50.0f, 60.0f), 0 };
			g_aEnemyInfo[5] = { D3DXVECTOR3(-580.0f, 50.0f, -240.0f), 0 };
			g_aEnemyInfo[6] = { D3DXVECTOR3(120.0f, 50.0f, 580.0f), 0 };
			g_aEnemyInfo[7] = { D3DXVECTOR3(180.0f, 50.0f, -580.0f), 0 };
			g_aEnemyInfo[8] = { D3DXVECTOR3(-160.0f, 50.0f, 200.0f), 0 };
			g_aEnemyInfo[9] = { D3DXVECTOR3(100.0f, 50.0f, -360.0f), 0 };
			g_aEnemyInfo[10] = { D3DXVECTOR3(480.0f, 50.0f, 220.0f), 0 };
			g_aEnemyInfo[11] = { D3DXVECTOR3(-480.0f, 50.0f, 120.0f), 0 };
			g_aEnemyInfo[12] = { D3DXVECTOR3(-140.0f, 50.0f, 480.0f), 0 };
			g_aEnemyInfo[13] = { D3DXVECTOR3(180.0f, 50.0f, -480.0f), 0 };
			g_aEnemyInfo[14] = { D3DXVECTOR3(520.0f, 50.0f, 180.0f), 0 };
			g_aEnemyInfo[15] = { D3DXVECTOR3(-520.0f, 50.0f, 230.0f), 0 };
			g_aEnemyInfo[16] = { D3DXVECTOR3(-90.0f, 50.0f, 520.0f), 0 };
			g_aEnemyInfo[17] = { D3DXVECTOR3(-190.0f, 50.0f, -520.0f), 0 };
			g_aEnemyInfo[18] = { D3DXVECTOR3(350.0f, 50.0f, -70.0f), 0 };
			g_aEnemyInfo[19] = { D3DXVECTOR3(-350.0f, 50.0f, -120.0f), 0 };
			g_aEnemyInfo[20] = { D3DXVECTOR3(60.0f, 50.0f, 350.0f), 0 };
			g_aEnemyInfo[21] = { D3DXVECTOR3(60.0f, 50.0f, -350.0f), 0 };
			g_aEnemyInfo[22] = { D3DXVECTOR3(250.0f, 50.0f, -120.0f), 0 };
			g_aEnemyInfo[23] = { D3DXVECTOR3(-220.0f, 50.0f, 190.0f), 0 };
			g_aEnemyInfo[24] = { D3DXVECTOR3(120.0f, 50.0f, 250.0f), 0 };
			g_aEnemyInfo[25] = { D3DXVECTOR3(210.0f, 50.0f, -250.0f), 0 };
			g_aEnemyInfo[26] = { D3DXVECTOR3(-210.0f, 50.0f, 480.0f), 0 };
			g_aEnemyInfo[27] = { D3DXVECTOR3(-100.0f, 50.0f, -210.0f), 0 };

			for (int nCntEnemyInfo = 0; nCntEnemyInfo < 28; nCntEnemyInfo++)
			{// �g�p���Ă��Ȃ���Ԃɂ���
				g_aEnemyInfo[nCntEnemyInfo].bUse = false;
			}

			SetNumEnemy(28);	// �G�̑����̐ݒ�
			SetRemnants(28);	// �G�̎c���̐ݒ�
		}

		g_nCntSetStage = 1;
	}
}

void StageStateSwat(bool bSwat)
{
	g_bStageSwat = bSwat;
}

bool GetStageSwat(void)
{
	return g_bStageSwat;
}