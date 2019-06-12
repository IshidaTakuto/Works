//=============================================================================
//
// �N���A���b�Z�[�W���� [clear.h]
// Author : shotaro kanamaru
//
//=============================================================================
#ifndef _CLEAR_H_
#define _CLEAR_H_

#include "main.h"

//*****************************************************************************
// �񋓑̂̒�`
//*****************************************************************************
typedef enum
{// ���b�Z�[�W�^�C�v
	TYPE_NORMAL = 0,	// �m�[�}��
	TYPE_COMPLETE,		// �R���v���[�g
	TYPE_EMPTY,			// ���̑�
	TYPE_MAX,
}CLEARTYPE;

typedef enum
{// ���
	STATE_NONE = 0,		// �ҋ@
	STATE_RESET,		// ���Z�b�g
	STATE_START,		// �N��
	STATE_MAX,
}CLEARSTATE;

typedef enum
{//	���b�Z�[�W�̕���
	MESSAGE_BAND = 0,	// ��
	MESSAGE_UP,			// �㕔
	MESSAGE_DOWN,		// ����
	MESSAGE_ONE,		// 1������
	MESSAGE_TWO,		// 2������
	MESSAGE_THREE,		// 3������
	MESSAGE_FOUR,		// 4������
	MESSAGE_MAX,		// �ő吔
}CLEARMESSAGE;

//*****************************************************************************
//�\���̂̒�`
//*****************************************************************************
typedef struct
{// ���b�Z�[�W���
	D3DXVECTOR3 pos;		// ���݈ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 size;		// �傫��
	D3DXCOLOR	col;		// �F
	CLEARTYPE	type;		// ���b�Z�[�W�^�C�v
	bool		bUse;		// �g�p
}Clear;

typedef struct
{// ���̑��ݒ���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		pTexture[MESSAGE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
	CLEARTYPE				type;				// ���b�Z�[�W�^�C�v
	CLEARSTATE				state;				// ���
	int						nCntAll;			// ���ʃJ�E���^
	int						nCntDraw;			// �\���J�E���^
	bool					bSwitch;			// �ړ��̐ؑ�
}ClearOther;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitClear(void);
void UninitClear(void);
void UpdateClear(void);
void DrawClear(void);
void SetClearMessage(const CLEARTYPE type);

#endif
