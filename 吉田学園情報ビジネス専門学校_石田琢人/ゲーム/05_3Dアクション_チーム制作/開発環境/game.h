//=============================================================================
//
// �Q�[������ [game.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

#define SCREEN_HALF (SCREEN_WIDTH / 2)	// ��ʂ̔���
#define GRAVITY					(3)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{//�e�X�e�[�W
	STAGE_NONE = 0,			//�������ĂȂ����
	STAGENUM_1,				//�X�e�[�W1
	STAGENUM_2,				//�X�e�[�W2
	STAGENUM_3,				//�X�e�[�W3
}STAGENUM;

typedef enum
{
	GAMESTATE_NONE = 0,	// �������Ă��Ȃ����
	GAMESTATE_NORMAL,	// �ʏ���
	GAMESTATE_END,		// �Q�[���I�[�o�[
	GAMESTATE_CLEAR,	// �Q�[���N���A
	GAMESTATE_MAX
}GAMESTATE;

// �|�[�YON/OFF�̍\����
typedef struct
{
	bool bPause;
}PAUSECHECK;

// �G�̏��̍\����
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	int nType;			// �G�̎��
	bool bUse;			// �����g�p�������ǂ���
}EnemyInfo;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetStage(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);
STAGENUM GetNumState(void);
PAUSECHECK *GetPauseMenu(void);
void SetStageState(STAGENUM stage);				// �X�e�[�W�ݒ菈��
void StageStateSwat(bool bSwat);
bool GetStageSwat(void);

#endif