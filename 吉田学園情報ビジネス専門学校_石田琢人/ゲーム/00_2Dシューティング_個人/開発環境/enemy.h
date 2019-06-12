//=============================================================================
//
// �G���� [enemy.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_ENEMY	(128)	// �G�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NOMAL = 0,	//�ʏ���
	ENEMYSTATE_DAMAGE,		//�_���[�W���
	ENEMYSTATE_MAX			//�G�̏�Ԃ̑���
}ENEMYSTATE;

typedef struct
{
	int nCounterAnim;	//�A�j���[�V�����J�E���^�[
	int nPatternAnim;	//�A�j���[�V�����p�^�[��No.
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ�
	int nType;			//���
	ENEMYSTATE state;	//�G�̏��
	int nCounterState;	//��ԊǗ��̃J�E���^�[
	int nLife;			//�G�̗̑�
	int nPoint;			//�G�̌��j�|�C���g
	bool bUse;			//�g�p���Ă��邩�ǂ���
}Enemy;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);

#endif
