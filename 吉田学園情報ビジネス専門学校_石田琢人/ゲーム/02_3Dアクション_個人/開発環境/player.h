//=============================================================================
//
// �v���C���[���� [player.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// �\����
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_NONE = 0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_2D,
	PLAYERSTATE_DAETH,
	PLAYERSTATE_CLEAR,
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nIdxModelParent;	// �e���f���̃C���f�b�N�X
}PlayerModel;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;		// ���݂̊p�x
	D3DXVECTOR3 angle;		// �p�x
	D3DXMATRIX mtxWorld;
	PlayerModel aModel[2];	// �p�[�c��
	PLAYERSTATE state;
	float fSpeed;
	int nCntState;			// �X�e�[�g�̃J�E���^
	float fDestAngle;		// �ړI�̊p�x
	float fDiffAngle;		// �����̊p�x
	int nIdxShadow;			// �e�̔ԍ�
	int nCntParticle;		// �p�[�e�B�N���̕ύX
	bool bJump;				// �W�����v�����Ă��邩
	D3DXVECTOR3 vtxMin;		// �v���C���[�̍ŏ��l
	D3DXVECTOR3 vtxMax;		// �v���C���[�̍ő�l
}Player;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

Player *GetPlayer(void);

void PlayerMotion(void);
#endif
