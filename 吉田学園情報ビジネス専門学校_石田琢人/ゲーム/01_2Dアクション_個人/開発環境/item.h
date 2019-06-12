//=============================================================================
//
// �A�C�e������ [item.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_ITEM	(128)	// �A�C�e���̍ő吔

//=============================================================================
// �\���̒�`
//=============================================================================
typedef enum
{
	ITEMTYPE_NONE = 0,
	ITEMTYPE_EGG,		// �R�C��
	ITEMTYPE_DIRTY,		// ����
	ITEMTYPE_MAX		// �A�C�e����ނ̑���
}ITEMTYPE;

typedef enum
{
	ITEMSTATE_APPEAR = 0,
	ITEMSTATE_NORMAL,
	ITEMSTATE_GET,
	ITEMSTATE_MOVE,
	ITEMSTATE_MAX
}ITEMSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 move;	// �ړ���
	float fWidth;		// ��
	float fHeight;		// ����
	float fAngle;		// �p�x
	float fLength;		// ���_�܂ł̒���
	int nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
	int nPatternAnim;	// �A�j���[�V�����p�^�[��No.
	ITEMTYPE type;		// �A�C�e���̎��
	ITEMSTATE state;	// ���
	int nCntState;		// ��Ԃ̃J�E���^�[
	bool bUse;			// �g�p����Ă��邩�ǂ���
}ITEM;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, ITEMTYPE state);
ITEM *GetItem(void);

#endif