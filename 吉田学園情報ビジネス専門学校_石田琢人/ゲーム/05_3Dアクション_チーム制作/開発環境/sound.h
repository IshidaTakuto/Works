//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// �T�E���h�t�@�C��
//*************************************
typedef enum
{
	SOUND_LABEL_TITLE = 0,		//�^�C�g��
	SOUND_LABEL_STAGE1,			//�X�e�[�W1
	SOUND_LABEL_STAGE2,			//�X�e�[�W2
	SOUND_LABEL_STAGE3,			//�X�e�[�W3
	SOUND_LABEL_DIERANKING,		//���Ƀ����L���O
	SOUND_LABEL_RANKING,		//�����L���O
	SOUND_LABEL_ENEMYVOICE,		//�G�l�~�[�{�C�X
	SOUND_LABEL_HIT,			//�G�l�~�[�q�b�g
	SOUND_LABEL_RAIN,			//�J
	SOUND_LABEL_SELECT,			//���艹
	SOUND_LABEL_SELECT2,		//���艹
	SOUND_LABEL_SWORDGESTURE,	//�\�[�h��U��
	SOUND_LABEL_COMBO,			//�R���{��
	SOUND_LABEL_SWORDHIT,		//�\�[�h�q�b�g
	SOUND_LABEL_PAUSE,			//�|�[�Y
	SOUND_LABEL_CANCEL,			//�L�����Z��
	SOUND_LABEL_SENTOU,			//�퓬�J�n
	SOUND_LABEL_SEIATU,			//��������
	SOUND_LABEL_HUNDUP,			//���������
	SOUND_LABEL_BREATH,			// �h���S���̍U��
	SOUND_LABEL_NEXTSTAGE,		// �X�e�[�W�ړ�
	SOUND_LABEL_FOOTSTEPS,		// ����
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
