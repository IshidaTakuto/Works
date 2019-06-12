//=============================================================================
//
// �v���C���[���� [player.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "block.h"
#include "item.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE_SPEED		(5.0f)	// �v���C���[�̈ړ���
#define JUMP_HEIGHT		(-30)	// �W�����v�̍���

//=============================================================================
// �\���̒�`
//=============================================================================
typedef enum
{
	PLAYERSTATE_NONE = 0,	// �������Ă��Ȃ����
	PLAYERSTATE_APPEAR,		// �o�����
	PLAYERSTATE_NORMAL,		// �ʏ���
	PLAYERSTATE_DEATH,		// ���S���
	PLAYERSTATE_CLEAR,		// �Q�[���N���A���
	PLAYERSTATE_MAX			// ��Ԃ̑���
}PLAYERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;		// ���݂̈ʒu
	D3DXVECTOR3 posOld;		// �O��̈ʒu
	D3DXVECTOR3 move;		// �ړ���
	float fHeight;			// ����
	float fWidth;			// ��
	int nCounterAnim;		// �A�j���[�V�����̃J�E���^�[
	int nPatternAnim;		// �A�j���[�V�����p�^�[��No.
	int nDirectionMove;		// �v���C���[�̌����Ă����
	bool bJump;				// �W�����v�����ǂ���
	bool bDisp;				// �`�悳��Ă��邩���Ȃ���
	int nCounterState;		// �X�e�[�g�̃J�E���^�[
	BLOCK BlockOld;			// �Ō�ɐG�����u���b�N
	PLAYERSTATE state;		// ���
	BLOCK *pBlock;			// �Ώۂ̃u���b�N�ւ̃|�C���^
}PLAYER;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER GetPlayer(void);
void CollisionItem(void);

#endif