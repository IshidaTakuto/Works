//=============================================================================
//
// ���쐧�䏈�� [operation.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	MODE_EFECT = 0,
	MODE_PARTICLE,
	//MODE_TEXANIM,
	MODE_MAX
}MODE;

typedef enum
{
	TYPE_MODE = 0,
	TYPE_SAVE_FILE,
	TYPE_NUM,
	TYPE_SELECT,
	TYPE_MAX
}STRIG_TYPE;

//***************************
// �L�[���͂̃}�N����`
//***************************
typedef struct
{
	char aKeyNamy[16];	// ��ʂɕ\������L�[�̖��O
	int nDIK;			// �L�[��DIK
}Information;

const Information MODE_CHANGE = { "[ �e�Q ]", DIK_F2 };		// ���[�h�̐؂�ւ� [�G�t�F�N�g or �p�[�e�B�N��]

const Information FILE_OUTPUT = { "[ �e�X ]", DIK_F9 };		// �f�[�^���t�@�C���ɏo��

const Information NUM_UP		 = { "[ �� ]", DIK_RIGHT };		// �I�𒆂̒l�𑝂₷
const Information NUM_DOWN	 = { "[ �� ]", DIK_LEFT };		// �I�𒆂̒l�𑝂₷

const Information SELECT_UP	 = { "[ �� ]", DIK_UP };		// �I���J�[�\�����グ��
const Information SELECT_DOWN = { "[ �� ]", DIK_DOWN };		// �I���J�[�\����������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitOperation(void);
void UninitOperation(void);
void UpdateOperation(void);
void DrawOperation(void);

#endif