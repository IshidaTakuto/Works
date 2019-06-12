//=============================================================================
//
// ���f������ [player.h]
// Author : �L�n�@���u
//
//=============================================================================
#ifndef _DRAGON_H_
#define _DRAGON_H_

#include "main.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
#define MAX_TEXT_NAME			(255)							//�t�@�C�����̖��O�̍ő吔
#define FILE_DRAGON_NAME		"data\\TEXT\\Player2.txt"		//�ǂݍ���txt�t�@�C���̖��O
#define MAX_DRAGON_PARTS		(22)							//�p�[�c��
#define MODEL_SPEED				(1.0f)							//�v���C���[�X�s�[�h
#define MAX_DRAGONANIM			(8)								//�A�j���[�V������
#define MAX_DRAGONKEY			(30)							//�L�[�t���[����

//*****************************************************************************
// ���f���̍\����
//*****************************************************************************
typedef enum
{
	DRAGONANIM_NEUTRAL = 0,		//�j���[�g�������[�V����
	DRAGONANIM_WALK,			//�������[�V����
	DRAGONANIM_ATTACK,			//�U�����[�V����
	DRAGON_MAX					//���[�V�����̍ő吔
}DragonAnim;

typedef struct
{
	float fRadius;		// �����蔻��̒c�q�̑傫��
	int nNumJudgment;	// �c�q�̐�
}DragonAttack;

typedef struct
{
	char			FileName[MAX_TEXT_NAME];						//X�t�@�C����
	LPD3DXMESH		pMeshModel = NULL;								//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	pBuffMatModel = NULL;							//�}�e���A���̏��ւ̃|�C���^
	DWORD			nNumMatModel = 0;								//�}�e���A���̏��
	D3DXMATRIX		mtxWorldModel;									//���[���h�}�g���b�N�X
	D3DXVECTOR3		posModel;										//�ʒu
	D3DXVECTOR3		InitPos;										//�����ʒu
	D3DXVECTOR3		posAnim[MAX_DRAGONANIM][MAX_DRAGONKEY];			//�A�j���[�V����pos
	D3DXVECTOR3		rotModel;										//����
	D3DXVECTOR3		InitRot;										//��������
	D3DXVECTOR3		rotAnim[MAX_DRAGONANIM][MAX_DRAGONKEY];			//�A�j���[�V����rot
	D3DXVECTOR3		rotDest;										//�ړI�̈ʒu
	int				nIdxModelParent = 0;							//�e���f���̃C���f�b�N�X�@-1���e
	DragonAttack	playerAttack;									// �����蔻��
}DragonModel;

typedef struct
{

	D3DXMATRIX		mtxWorldDragon;									//���[���h�}�g���b�N�X
	LPD3DXMESH		pMeshDragon = NULL;								//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	pBuffMatDragon = NULL;							//�}�e���A���̏��ւ̃|�C���^
	DWORD			nNumMatDragon = 0;								//�}�e���A���̏��
	D3DXVECTOR3		posOld;											//�ߋ��̈ʒu
	D3DXVECTOR3		pos;											//�ʒu
	D3DXVECTOR3		move;											//�ړ�
	D3DXVECTOR3		rot;											//����
	int				nMaxPartsNum;									//�p�[�c�̍ő吔
	DragonModel		aModel[MAX_DRAGON_PARTS];								//[�Z]�����ɂ̓p�[�c��������
	int				nMaxAnimNum;									//�A�j���[�V�����̍ő吔
	int				nAnimloop[MAX_DRAGONANIM];						//���[�v
	int				nAnimKey[MAX_DRAGONANIM];						//�L�[��
	int				nAnimfram[MAX_DRAGONANIM][MAX_DRAGONKEY];		//�t���[����
	DragonAnim		nAnimnow;										//���݂̃A�j���[�V����
	int				nAnimKeynow;									//���݂̃L�[
	bool			bAttack;										//�g�p���Ă��邩�ǂ���
	bool			bUse;											//�g�p���Ă��邩�ǂ���
	bool			bLand;											//�߂܂��Ă��邩�ǂ���
	int				nIdxShadow;									//�ǂ̉e���g���Ă��邩
	int				nCntDragon;
}Dragon;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitDragon(void);
void UninitDragon(void);
void UpdateDragon(void);
void DrawDragon(void);
Dragon *GetDragon(void);
void DeleteDragon(void);
void SetDragon(D3DXVECTOR3 pos);
D3DXVECTOR3 GetPosDragon(void);

D3DXVECTOR3 GetNeckRot(void);

bool CollisionDragonAttack(D3DXVECTOR3 pos, float vtxMax);
bool CollisionDragon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange);		// �v���C���[�Ƒ��̃I�u�W�F�N�g

#endif
