//=============================================================================
//
// �e���� [bullet.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	BULLETTYPE_PLAYER = 0,		//�v���C���[�̒e
	BULLETTYPE_ENEMY,			//�G�̒e
	BULLETTYPE_PLAYER_BEAM,		//�v���C���[�̃r�[��
	BULLETTYPE_MAX
}BULLETTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	BULLETTYPE type;	//�e�̎��
	D3DXCOLOR col;		//�F
	int nLife;			//����
	bool bUse;			//�g�p���Ă��邩�ǂ���
} Bullet;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type);

Bullet *GetBullet(void);
#endif
