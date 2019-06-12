//=============================================================================
//
// �u���b�N���� [block.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_BLOCK	(255)	// �G�̍ő吔
#define BLOCK_SIZE	(100.0f)	// �u���b�N�̑傫��
#define SCROLL_SPEED	(1.5f)	// �X�N���[�����鑬��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	BLOCKTYPE_NORMAL = 0,
	BLOCKTYPE_VERTICAL,
	BLOCKTYPE_HORIZONTAL,
	BLOCKTYPE_TITLE,
	BLOCKTYPE_BELT_LEFT,
	BLOCKTYPE_BELT_RIGHT,
	BLOCKTYPE_FALL,
	BLOCKTYPE_GOAL,
	BLOCKTYPE_SHARK,
	BLOCKTYPE_ENEMY,
	BLOCKTYPE_MAX
}BLOCKTYPE;

typedef enum
{
	BLOCKSTATE_NONE = 0,
	BLOCKSTATE_NORMAL,
	BLOCKSTATE_MOVE,
	BLOCKSTATE_CLEAR,
	BLOCKSTATE_MAX
}BLOCKSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 posOld; // �O��̈ʒu
	D3DXVECTOR3 move;	// �ړ���
	D3DXVECTOR3 des;	// �ړI�n
	int nType;			// �e�N�X�`���^�C�v
	int nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
	int nPatternAnim;	// �A�j���[�V�����p�^�[��No.
	int nDirectionMove;	// �����Ă����
	float fWidth;		// ��
	float fHeight;		// ����
	int nCounterState;	// �X�e�[�g�̃J�E���^�[
	int nCntMove;		// �ǂ��Ă��鎞��
	BLOCKTYPE type;		// ���
	BLOCKSTATE state;	// ���
	int nVertical;		// �㉺�ړ�
	bool bScroll;		// �����X�N���[��
	bool bUse;			// �g�p���Ă��邩�ǂ���
}BLOCK;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 des, BLOCKTYPE type);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float *pHeight, float *pWidth);
BLOCK *GetBlock(void);

#endif
