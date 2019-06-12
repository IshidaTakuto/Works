//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMYNUMBER	(10)		//�G�̈��ނ̐�

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;	//�Q�[���̏��
int g_nCounterGameState;				//��ԊǗ��̃J�E���^�[
PAUSECHECK g_bPause;					//

//=============================================================================
// �Q�[������������
//=============================================================================
void InitGame(void)
{
	int nCntEnemy;

	//�w�i����������
	InitBg();

	//�X�R�A����������
	InitScore();

	//�^�C�}�[����������
	InitTimer();

	//�̗͏���������
	InitLife();

	//�c�@����������
	InitStock();

	//�r�[������������
	InitBeam();

	//�e����������
	InitBullet();

	//��������������
	InitExplosion();

	//�p�[�e�B�N������������
	InitParticle();

	//�G�t�F�N�g����������
	InitEffect();

	//�v���C���[����������
	InitPlayer();

	//�G����������
	InitEnemy();
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMYNUMBER; nCntEnemy++)
	{
		SetEnemy(D3DXVECTOR3(nCntEnemy * 100 + 200.0f, 100.0f, 0.0f), 0);
		SetEnemy(D3DXVECTOR3(nCntEnemy * 100 + 200.0f, 150.0f, 0.0f), 1);
		SetEnemy(D3DXVECTOR3(nCntEnemy * 100 + 200.0f, 200.0f, 0.0f), 2);
		SetEnemy(D3DXVECTOR3(nCntEnemy * 100 + 200.0f, 250.0f, 0.0f), 3);
	}

	//�|�[�Y����������
	InitPause();

	//�|�[�Y�w�i����������
	InitPausebg();

	//�|�[�Y���j���[����������
	InitMenu();

	g_gameState = GAMESTATE_NORMAL;	//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;

	g_bPause.bPause = false;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	//�w�i�I������
	UninitBg();

	//�X�R�A�I������
	UninitScore();

	//�^�C�}�[�I������
	UninitTimer();

	//�̗͏I������
	UninitLife();

	//�c�@�I������
	UninitStock();

	//�r�[���I������
	UninitBeam();

	//�e�I������
	UninitBullet();

	//�����I������
	UninitExplosion();

	//�p�[�e�B�N���I������
	UninitParticle();

	//�G�t�F�N�g�I������
	UninitEffect();

	//�v���C���[�I������
	UninitPlayer();

	//�G�I������
	UninitEnemy();

	//�|�[�Y�I������
	UninitPause();

	//�|�[�Y�w�i�I������
	UninitPausebg();

	//�|�[�Y���j���[�I������
	UninitMenu();
}

//=============================================================================
// �Q�[���X�V����
//=============================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true)
	{//�|�[�YON/OFF
		g_bPause.bPause = g_bPause.bPause ? false : true;
	}

	if (g_bPause.bPause == false)
	{
		//�w�i�X�V����
		UpdateBg();

		//�X�R�A�X�V����
		UpdateScore();

		//�^�C�}�[�X�V����
		UpdateTimer();

		//�̗͍X�V����
		UpdateLife();

		//�c�@�X�V����
		UpdateStock();

		//�r�[���X�V����
		UpdateBeam();

		//�����X�V����
		UpdateExplosion();

		//�p�[�e�B�N���X�V����
		UpdateParticle();

		//�e�X�V����
		UpdateBullet();

		//�G�t�F�N�g�X�V����
		UpdateEffect();

		//�v���C���[�X�V����
		UpdatePlayer();

		//�G�X�V����
		UpdateEnemy();
	}
	else
	{
		//�|�[�Y�X�V����
		UpdatePause();

		//�|�[�Y�w�i�X�V����
		UpdatePausebg();
		
		//�|�[�Y���j���[�X�V����
		UpdateMenu();
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

			int *pScore;
			pScore = GetScore();

			int *pTimer;
			pTimer = GetTimer();

			// �c��^�C�����X�R�A�ɉ��Z
			*pScore += *pTimer * 12;

			SetRankScore(*pScore);

			//���(���[�h)�̐ݒ�
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
	//�w�i�`�揈��
	DrawBg();

	//�X�R�A�`�揈��
	DrawScore();

	//�^�C�}�[�`�揈��
	DrawTimer();

	//�̗͕`�揈��
	DrawLife();

	//�c�@�`�揈��
	DrawStock();

	//�r�[���`�揈��
	DrawBeam();

	//�e�`�揈��
	DrawBullet();

	//�����`�揈��
	DrawExplosion();

	//�p�[�e�B�N���`�揈��
	DrawParticle();

	//�G�t�F�N�g�`�揈��
	DrawEffect();

	//�v���C���[�`�揈��
	DrawPlayer();

	//�G�`�揈��
	DrawEnemy();

	if (g_bPause.bPause == true)
	{
		//�|�[�Y�`�揈��
		DrawPause();

		//�|�[�Y�w�i�`�揈��
		DrawPausebg();

		//�|�[�Y���j���[�`�揈��
		DrawMenu();
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