//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_EMITTER			(16)							// �G�~�b�^�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	PARTICLE_SELEMITTER = 0,	// �I�𒆂̃G�~�b�^
	PARTICLE_MAXMOVE,			// �ړ��ʂ̍ő�l
	PARTICLE_MINMOVE,			// �ړ��ʂ̍ŏ��l
	PARTICLE_COLOR,				// �F
	PARTICLE_FLAMECOLOR,		// ���t���[���̐F�̕ω�
	PARTICLE_RADIUS,			// ���a(�傫��)
	PARTICLE_FLAMERADIUS,		// ���t���[���̔��a�̊g�k
	PARTICLE_LIFE,				// ����
	PARTICLE_PARLIFE,			// �p�[�e�B�N���̕\������
	PARTICLE_APPEAR,			// �o��Ԋu
	PARTICLE_SPREAD,			// �L����
	PARTICLE_LAP,				// 1���ɏo����
	PARTICLE_LOOP,				// ���[�v�����邩
	PARTICLE_GRAVITY,			// �d��
	PARTICLE_BOUNCING,			// �o�E���h�����邩
	PARTICLE_ROTATE,			// ��]�����邩�ǂ���
	PARTICLE_ROTATE_SPEED,		// ��]���鑬�x
	PARTICLE_TEXTURE,			// �e�N�X�`���̎��
	PARTICLE_DRAW,				// ���Z�������ǂ���
	PARTICLE_MAX
}PARTICLE_TYPE;

typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 rot;		// ���݂̊p�x
	D3DXCOLOR col;			// �G�~�b�^�̈ʒu
	float fLength;			// ���_�̋���
	float fCntRot;			// �p�x�̍X�V
	float fRotAngle;		// ��]����Ƃ��̊p�x
	float fAngle;			// �p�x
	float fRadius;			// �傫��
	int nLife;				// �\������
	float fGravity;			// �d��
	int nType;				// �e�N�X�`���̎��
	bool bUse;				// �g�p����Ă��邩
	float fBouncing;		// �o�E���h����l
	int nIdx;				// �G�~�b�^�̔ԍ����L��
	bool bDraw;				// ���Z�������ǂ���
	D3DXMATRIX mtxWorld;
}Particle;

typedef struct
{
	D3DXVECTOR3 pos;		// �G�~�b�^�̈ʒu
	int nWidth[2];			// ��(�ŏ��l�ƍő�l)
	int nHeight[2];			// ����(�ŏ��l�ƍő�l)
	int nDepth[2];			// ���s(�ŏ��l�ƍő�l)
	D3DXCOLOR col;			// �F
	D3DXCOLOR flameCol;		// ���t���[���̐F�̕ω�
	int nRadius[2];			// �傫��(�ŏ��l�ƍő�l)
	float fFlameRadius;		// ���t���[���̔��a�̊g�k
	int nLife;				// �G�~�b�^�̎���
	int nParLife[2];		// �\������(�ŏ��l�ƍő�l)
	int nType;				// �e�N�X�`���̎��
	float fSpread;			// �L����
	int nAppear;			// �o��Ԋu
	int nLap;				// 1���ɏo����
	float fGravity;			// �d��
	int nRot;				// ��]���邩�ǂ���(0:�Ȃ��@1:���v���@2:�����v��� 3:�����_��)
	float fRotSpeed;		// ��]���鑬�x
	bool bLoop;				// ���[�v�����邩 0:�Ȃ� 1:����
	bool bBouncing;			// �o�E���h�����邩 0:�Ȃ� 1:����
	bool bUse;				// �g�p����Ă��邩
	bool bDraw;				// ���Z�������ǂ��� 0:���� 1:���Z
}ParEmitter;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticleEmitter(D3DXVECTOR3 pos);
void SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos);
void DestroyParticle(int nIdx);

#endif