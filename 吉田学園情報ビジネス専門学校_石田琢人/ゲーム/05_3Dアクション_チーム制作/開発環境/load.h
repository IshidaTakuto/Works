//=============================================================================
//
// �G���A�ړ����� [load.h]
// Author : shotaro kanamaru
//
//=============================================================================
#ifndef _LOAD_H_
#define _LOAD_H_

#include "main.h"

//*****************************************************************************
// �񋓑̂̒�`
//*****************************************************************************
typedef enum
{// ���
	LOADTYPE_BACK = 0,		// �h��Ԃ�
	LOADTYPE_BG,			// �w�i
	LOADTYPE_LINE,			// �X�e�[�W�̐�
	LOADTYPE_PLAYER,		// �v���C���[�A�C�R��
	LOADTYPE_FADE,			// �t�F�[�h
	LOADTYPE_MAX,			// �ő吔
}LOADTYPE;

typedef enum
{// ���
	LOADSTATE_NONE = 0,		// �ҋ@
	LOADSTATE_START,		// �N��
	LOADSTATE_OUT,			// �I��
	LOADSTATE_MAX,
}LOADSTATE;

typedef enum
{// ���[�h�p�t�F�[�h
	LOADFADE_NONE = 0,
	LOADFADE_IN,		//�t�F�[�h�C��
	LOADFADE_OUT,		//�t�F�[�h�A�E�g
	LOADFADE_MAX		// �ő吔
}LOADFADE;

//*****************************************************************************
//�\���̂̒�`
//*****************************************************************************
typedef struct
{// �G���A�ړ����
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 size;			// �傫��
	D3DXCOLOR	col;			// �F
	LOADTYPE	type;			// ���
	bool		bUse;			// �g�p
	int			nNumPattern;	// ���݂̃A�j���[�V�����ԍ�
}Load;

typedef struct
{// �ݒ���
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 size;			// �傫��
	int			nAnimPattern;	// �A�j���[�V������
}LoadInfo;

typedef struct
{// ���̑��ݒ���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		pTexture[LOADTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
	LOADSTATE				state;							// ���
	int						nNextStage;						// ���̃X�e�[�W
	int						nCntAll;						// ���ʃJ�E���^
	bool					bUse;							// �S�̂̕`��
}LoadOther;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitLoad(void);
void UninitLoad(void);
void UpdateLoad(void);
void DrawLoad(void);
void SetLoad(void);

LOADFADE GetLoadFade(void);

#endif
