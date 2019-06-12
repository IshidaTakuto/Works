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
int g_nCntBlock;

BlockInfo g_aBlockInfo[BLOCK_NUM]
{
	{ D3DXVECTOR3(20.0f, 500.0f, 0.0f), BLOCK_SIZE * 2, BLOCK_SIZE * 1.6f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_NORMAL },
	{ D3DXVECTOR3(200.0f, 550.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_NORMAL },
	{ D3DXVECTOR3(350.0f, 500.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_NORMAL },
	{ D3DXVECTOR3(650.0f, 580.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_NORMAL },
	{ D3DXVECTOR3(1650.0f, 500.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_NORMAL },
	{ D3DXVECTOR3(2500, 480.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 300.0f, 0.0f), BLOCKTYPE_VERTICAL },
	{ D3DXVECTOR3(4500.0f, 480.0f, 0.0f), BLOCK_SIZE * 2, BLOCK_SIZE * 2, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_GOAL },

	{ D3DXVECTOR3(SCREEN_HALF - 100, 200.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 300.0f, 0.0f), BLOCKTYPE_VERTICAL },
	{ D3DXVECTOR3(3200 - (BLOCK_SIZE * 4), 200.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 380.0f, 0.0f), BLOCKTYPE_VERTICAL },
	{ D3DXVECTOR3(3500 - (BLOCK_SIZE * 4), 480.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 300.0f, 0.0f), BLOCKTYPE_VERTICAL },
	{ D3DXVECTOR3(3800 - (BLOCK_SIZE * 4), 200.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 380.0f, 0.0f), BLOCKTYPE_VERTICAL },
	{ D3DXVECTOR3(SCREEN_WIDTH - (4 * BLOCK_SIZE), 500.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 500.0f, 0.0f), BLOCKTYPE_FALL },
	{ D3DXVECTOR3(4000.0f, 250.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 250.0f, 0.0f), BLOCKTYPE_FALL },
	{ D3DXVECTOR3(2000, 250.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 250.0f, 0.0f), BLOCKTYPE_FALL },
	{ D3DXVECTOR3(1400.0f, 450.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 450.0f, 0.0f), BLOCKTYPE_FALL }
};

ItemInfo g_aItemInfo[ITEM_NUM]
{
	{ D3DXVECTOR3(150, 450.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(1500, 500.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(2500, 400.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(3600, 350.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(850, 450.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(2800, 500.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(4150, 450.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(3150, 550.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(1850, 500.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(3850, 450.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(2150, 400.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(3400, 450.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(1100.0f, 600.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(400.0f, 500.0f, 0.0f), ITEMTYPE_DIRTY },
	{ D3DXVECTOR3(1600.0f, 600.0f, 0.0f), ITEMTYPE_DIRTY },
	{ D3DXVECTOR3(2400.0f, 300.0f, 0.0f), ITEMTYPE_DIRTY },
	{ D3DXVECTOR3(4100.0f, 600.0f, 0.0f), ITEMTYPE_DIRTY },
};

GeneratorInfo g_aGeneratorInfo[GENERATOR_NUM]
{
	{ D3DXVECTOR3(SCREEN_WIDTH + 60, 660.0f, 0.0f), BLOCKTYPE_BELT_LEFT, 200.0f, 100.0f, D3DXVECTOR3(0.0f,658.0f,0.0f), 120 },
	{ D3DXVECTOR3(-150.0f, 125.0f, 0.0f), BLOCKTYPE_HORIZONTAL, BLOCK_SIZE + 20, BLOCK_SIZE, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), 270 },
	{ D3DXVECTOR3(-150.0f, 250.0f, 0.0f), BLOCKTYPE_HORIZONTAL, BLOCK_SIZE + 20, BLOCK_SIZE, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), 450 },
	{ D3DXVECTOR3(-150.0f, 375.0f, 0.0f), BLOCKTYPE_HORIZONTAL, BLOCK_SIZE + 20, BLOCK_SIZE, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), 400 },
	{ D3DXVECTOR3(-150.0f, 500.0f, 0.0f), BLOCKTYPE_HORIZONTAL, BLOCK_SIZE + 20, BLOCK_SIZE, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), 350 },
};
//=============================================================================
// �Q�[������������
//=============================================================================
void InitGame(void)
{
	g_nCntBlock = 0;

	// �w�i����������
	InitBg();

	// �u���b�N����������
	InitBlock();
	for (int nCntBlock = 0; nCntBlock < sizeof g_aBlockInfo / sizeof(BlockInfo); nCntBlock++)
	{
		SetBlock(g_aBlockInfo[nCntBlock].pos, g_aBlockInfo[nCntBlock].fWidth, g_aBlockInfo[nCntBlock].fHeight, g_aBlockInfo[nCntBlock].des, g_aBlockInfo[nCntBlock].type);
	}

	// �W�F�l���[�^����������
	InitGenerator();
	for (int nCntGenerator = 0; nCntGenerator < sizeof g_aGeneratorInfo / sizeof(GeneratorInfo); nCntGenerator++)
	{
		SetGenerator(g_aGeneratorInfo[nCntGenerator].pos, 
					g_aGeneratorInfo[nCntGenerator].type, 
					g_aGeneratorInfo[nCntGenerator].fWidth, 
					g_aGeneratorInfo[nCntGenerator].fHeight, 
					g_aGeneratorInfo[nCntGenerator].des, 
					g_aGeneratorInfo[nCntGenerator].interval);
	}

	// �A�C�e������������
	InitItem();
	for (int nCntItem = 0; nCntItem < sizeof g_aItemInfo / sizeof(ItemInfo); nCntItem++)
	{
		SetItem(g_aItemInfo[nCntItem].pos, g_aItemInfo[nCntItem].type);
	}

	// �X�R�A����������
	InitScore();

	// �^�C�}�[����������
	InitTimer();

	// �c�@����������
	InitStock();

	// �v���C���[����������
	InitPlayer();

	// �f�X�g���C���[����������
	InitDestroyer();

	// �G�t�F�N�g����������
	InitEffect();

	// ������������
	InitBlood();

	// �|�[�Y����������
	InitPause();

	// �|�[�Y�w�i����������
	InitPausebg();

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
	// �w�i�I������
	UninitBg();

	// �u���b�N�I������
	UninitBlock();

	// �W�F�l���[�^�I������
	UninitGenerator();

	//�X�R�A�I������
	UninitScore();

	// �A�C�e���I������
	UninitItem();

	// �^�C�}�[�I������
	UninitTimer();

	// �c�@�I������
	UninitStock();

	// �v���C���[�I������
	UninitPlayer();

	// �f�X�g���C���[�I������
	UninitDestroyer();

	// �G�t�F�N�g�I������
	UninitEffect();

	// ���I������
	UninitBlood();

	// �|�[�Y�I������
	UninitPause();

	// �|�[�Y�w�i�I������
	UninitPausebg();

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
		if (g_bPause.bPause == true)
		{
			PlaySound(SOUND_LABEL_SE_PAUSEON);
		}
		else
		{
			PlaySound(SOUND_LABEL_SE_PAUSEOFF);
		}
	}

	g_nCntBlock++;

	if (g_bPause.bPause == false)
	{
		// �w�i�X�V����
		UpdateBg();

		// �u���b�N�X�V����
		UpdateBlock();
		if (g_gameState == GAMESTATE_NORMAL && g_nCntBlock % 150 == 0)
		{
			SetBlock(D3DXVECTOR3((rand() % SCREEN_WIDTH) * 1.0f, -100.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_ENEMY);
		}

		// �W�F�l���[�^�X�V����
		UpdateGenerator();

		//�X�R�A�X�V����
		UpdateScore();

		// �A�C�e���X�V����
		UpdateItem();

		// �^�C�}�[�X�V����
		UpdateTimer();

		// �c�@�X�V����
		UpdateStock();

		// �v���C���[�X�V����
		UpdatePlayer();

		// �f�X�g���C���[�X�V����
		UpdateDestroyer();

		// �G�t�F�N�g�X�V����
		UpdateEffect();

		// ���X�V����
		UpdateBlood();
	}
	else
	{
		// �|�[�Y�X�V����
		UpdatePause();

		// �|�[�Y�w�i�X�V����
		UpdatePausebg();
		
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

			// ���(���[�h)�̐ݒ�
			SetFade(MODE_GAMEOVER);
		}
		break;

	case GAMESTATE_CLEAR:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;

			AddScore(30000);

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
	// �w�i�`�揈��
	DrawBg();

	// �u���b�N�`�揈��
	DrawBlock();

	// �W�F�l���[�^�`�揈��
	DrawGenerator();

	// �X�R�A�`�揈��
	DrawScore();

	// �A�C�e���`�揈��
	DrawItem();

	// �^�C�}�[�`�揈��
	DrawTimer();

	// �c�@�`�揈��
	DrawStock();

	// �v���C���[�`�揈��
	DrawPlayer();

	// �f�X�g���C���[�`�揈��
	DrawDestroyer();

	// �G�t�F�N�g�`�揈��
	DrawEffect();

	// ���`�揈��
	DrawBlood();

	if (g_bPause.bPause == true)
	{
		// �|�[�Y�`�揈��
		DrawPause();

		// �|�[�Y�w�i�`�揈��
		DrawPausebg();

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