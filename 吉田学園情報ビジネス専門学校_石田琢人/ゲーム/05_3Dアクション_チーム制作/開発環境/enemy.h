//=============================================================================
//
// �G�̏��� [enemy.h]
// Author : �L�n�@���u
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
#define MAX_TEXT_NAME			(255)							//�t�@�C�����̖��O�̍ő吔
#define TEXT_NAME				"data\\TEXT\\Enemy1.txt"		//�ǂݍ���txt�t�@�C���̖��O
#define MAX_PARTS				(15)							//�p�[�c��
#define MODEL_SPEED				(1.0f)							//�v���C���[�X�s�[�h
#define MAX_ENEMYANIM			(8)								//�A�j���[�V������
#define MAX_ENEMYKEY			(15)								//�L�[�t���[����

//*****************************************************************************
// ���f���̍\����
//*****************************************************************************
typedef enum
{
	ENEMYANIM_NEUTRAL = 0,		//�j���[�g�������[�V����
	ENEMYANIM_WALK,				//�������[�V����
	ENEMYANIM_ATTACK,			//�U�����[�V����
	ENEMYANIM_DAMAGE,			// �_���[�W���[�V����
	ENEMY_MAX					//���[�V�����̍ő吔
}EnemyAnim;

typedef struct
{
	char			FileName[MAX_TEXT_NAME];						//X�t�@�C����
	LPD3DXMESH		pMeshModel = NULL;								//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	pBuffMatModel = NULL;							//�}�e���A���̏��ւ̃|�C���^
	DWORD			nNumMatModel = 0;								//�}�e���A���̏��
	D3DXMATRIX		mtxWorldModel;									//���[���h�}�g���b�N�X
	D3DXVECTOR3		posModel;										//�ʒu
	D3DXVECTOR3		InitPos;										//�����ʒu
	D3DXVECTOR3		posAnim[MAX_ENEMYANIM][MAX_ENEMYKEY];			//�A�j���[�V����pos
	D3DXVECTOR3		rotModel;										//����
	D3DXVECTOR3		InitRot;										//��������
	D3DXVECTOR3		rotAnim[MAX_ENEMYANIM][MAX_ENEMYKEY];			//�A�j���[�V����rot
	D3DXVECTOR3		rotDest;										//�ړI�̈ʒu
	int				nIdxModelParent = 0;							//�e���f���̃C���f�b�N�X�@-1���e
}EnemyModel;

typedef struct
{

	D3DXMATRIX		mtxWorldEnemy;									//���[���h�}�g���b�N�X
	LPD3DXMESH		pMeshEnemy = NULL;								//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	pBuffMatEnemy = NULL;							//�}�e���A���̏��ւ̃|�C���^
	DWORD			nNumMatEnemy = 0;								//�}�e���A���̏��
	D3DXVECTOR3		posOld;											//�ߋ��̈ʒu
	D3DXVECTOR3		pos;											//�ʒu
	D3DXVECTOR3		move;											//�ړ�
	D3DXVECTOR3		rot;											//����
	int				nMaxPartsNum;									//�p�[�c�̍ő吔
	EnemyModel		aModel[MAX_PARTS];								//[�Z]�����ɂ̓p�[�c��������
	int				nMaxAnimNum;									//�A�j���[�V�����̍ő吔
	int				nAnimloop[MAX_ENEMYANIM];						//���[�v
	int				nAnimKey[MAX_ENEMYANIM];						//�L�[��
	int				nAnimfram[MAX_ENEMYANIM][MAX_ENEMYKEY];			//�t���[����
	EnemyAnim		nAnimnow;										//���݂̃A�j���[�V����
	int				nAnimKeynow;									//���݂̃L�[
	bool			bAttack;										//�U�����Ă��邩�ǂ���
	bool			bUse;											//�g�p���Ă��邩�ǂ���
	bool			bLand;											//�߂܂��Ă��邩�ǂ���
	int				nIdxShadow;										//�ǂ̉e���g���Ă��邩
	int				nAttackCount;									//�U���̎��Ԃ̃J�E���g
	int				nCntTimeEnemy;									//�G�J�E���^�[
	D3DXVECTOR3		VtxMin, VtxMax;									//���f���̍ŏ��l�E�ő�l
	int				nLife;											//���C�t

	D3DXVECTOR3     blowMove;										//������Ԉړ���
	bool			bDamage;										// �_���[�W���󂯂���
	int				nCntDamage;										// �_���[�W��Ԃ̃J�E���^
}Enemy;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
void DeleteEnemy(void);
void SetEnemy(D3DXVECTOR3 pos,int nType);	// �ʒu�ƓG�̎��
int GetNumEnemy(void);
void SetNumEnemy(int nNumEnemy);

bool CollisionEnemies(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange, int nIdx);	// �G���m
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange);				// �G�Ƒ��̃I�u�W�F�N�g
bool CollisionAttackEnemy(D3DXVECTOR3 pos, float vtxMaxX);

#endif
