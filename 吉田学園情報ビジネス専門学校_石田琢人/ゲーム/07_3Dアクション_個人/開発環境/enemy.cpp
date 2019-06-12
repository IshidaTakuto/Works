//=============================================================================
//
// �G���� [enemy.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "enemy.h"
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "player.h"
#include "meshField.h"
#include "game.h"
#include "modelSet.h"
#include "shadow.h"
#include "life.h"
#include "sound.h"

#include <stdio.h>

//==================================
// �}�N����`
//==================================
#define ENEMY_MODEL_FILE	"data/TEXT/ENEMY/model.txt"
#define ENEMY_SPEED		(0.4f)	// �ړ��̑��x
#define ENEMY_LIFE		(10)		// ���C�t
#define ENEMY_SHOOT		(120)	// �U���Ԋu

#define ENEMY_LIFE_WIDTH	(50.0f)		// �̗̓o�[�̕�
#define ENEMY_LIFE_HEIGHT	(10.0f)		// �̗̓o�[�̍���

#define ENEMY_WIDTH			(30.0f)		// �����蔻��̕�
#define ENEMY_DAMAGE_TIME	(30)		// �_���[�W��Ԃ̎���

// ���[�V�����ԍ�
#define ENEMY_NEUTRAL		(0)		// �j���[�g����
#define ENEMY_WALK			(1)		// ���s
#define ENEMY_JUMP			(2)		// �W�����v
#define ENEMY_LAND			(3)		// ���n
#define ENEMY_DAMAGE		(4)		// �_���[�W
#define ENEMY_ATTACK		(5)		// �U��

//==================================
// �ÓI�����o�ϐ��錾
//==================================
int				*CEnemy::m_pNumParts = {};				// �p�[�c��
char			***CEnemy::m_pModelName = NULL;			// ���f����
CEnemy::DATA	**CEnemy::m_pFileData = NULL;			// �t�@�C���ǂݎ��
int				CEnemy::m_nNumType = 0;					// ��ނ̐�

//==================================
// ��������
//==================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, int nType)
{
	CEnemy *pEnemy;

	pEnemy = new CEnemy;	// ���������m��

	if (pEnemy != NULL)
	{// �������m�ې���
		pEnemy->m_pos = pos;	// �ʒu
		pEnemy->m_nType = nType;	// ���
		pEnemy->Init();			// ����������
	}

	return pEnemy;
}

//=========================================
// ���f���̓ǂݍ���
//=========================================
HRESULT CEnemy::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntParts = 0;		// �p�[�c���̃J�E���^
	char **pModelFileName = CModelSetManager::GetModelFileName(CModelSetManager::TYPE_ENEMY);	// ���f�����̎擾
	int nCntModel = 0;
	m_nNumType = CModelSetManager::GetNumType(CModelSetManager::TYPE_ENEMY);

	// ���f�������������m��
	m_pNumParts = new int[m_nNumType];
	m_pModelName = new char**[m_nNumType];
	m_pFileData = new DATA*[m_nNumType];

	for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
	{
		// �t�@�C�����J��
		pFile = fopen(pModelFileName[nCntType], "r");

		if (NULL != pFile)
		{// �t�@�C����������
			char aStr[128];

			while (1)
			{// �X�N���v�g��ǂݏI���܂Ń��[�v
				fscanf(pFile, "%s", aStr);

				if (strcmp(aStr, "SCRIPT") == 0)
				{// �X�N���v�g�J�n
					while (strcmp(aStr, "END_SCRIPT") != 0)
					{// �X�N���v�g�I�����b�Z�[�W�܂Ń��[�v
						fscanf(pFile, "%s", aStr);

						if (strcmp(aStr, "NUM_MODEL") == 0)
						{// ���f����
							int nNumModel;
							fscanf(pFile, " = %d", &nNumModel);

							m_pModelName[nCntType] = new char*[nNumModel];		// �������m��
						}
						if (strcmp(aStr, "MODEL_FILENAME") == 0)
						{// �L�����N�^�[���
							m_pModelName[nCntType][nCntModel] = new char[64];		// �������m��
							fscanf(pFile, " = %s", &m_pModelName[nCntType][nCntModel][0]);

							nCntModel++;
						}
						if (strcmp(aStr, "CHARACTERSET") == 0)
						{// �L�����N�^�[���
							while (strcmp(aStr, "END_CHARACTERSET") != 0)
							{// �L�����N�^�[���I�����b�Z�[�W�܂Ń��[�v
								fscanf(pFile, "%s", aStr);

								if (strcmp(aStr, "NUM_PARTS") == 0)
								{// �p�[�c��
									fscanf(pFile, " = %d", &m_pNumParts[nCntType]);
									m_pFileData[nCntType] = new DATA[m_pNumParts[nCntType]];
								}
								else if (strcmp(aStr, "PARTSSET") == 0)
								{// �p�[�c�̐ݒ�
									while (strcmp(aStr, "END_PARTSSET") != 0)
									{// �p�[�c�̐ݒ�I�����b�Z�[�W�܂Ń��[�v
										fscanf(pFile, "%s", &aStr);

										if (strcmp(aStr, "INDEX") == 0)
										{// ���f���ԍ�
											fscanf(pFile, " = %d", &m_pFileData[nCntType][nCntParts].nIdx);
										}
										else if (strcmp(aStr, "PARENT") == 0)
										{// �e�����߂�
											fscanf(pFile, " = %d", &m_pFileData[nCntType][nCntParts].nParent);
										}
										else if (strcmp(aStr, "POS") == 0)
										{// �ʒu�̐ݒ�
											fscanf(pFile, " = %f %f %f", &m_pFileData[nCntType][nCntParts].pos.x, &m_pFileData[nCntType][nCntParts].pos.y, &m_pFileData[nCntType][nCntParts].pos.z);
										}
										else if (strcmp(aStr, "ROT") == 0)
										{// �����̐ݒ�
											fscanf(pFile, " = %f %f %f", &m_pFileData[nCntType][nCntParts].rot.x, &m_pFileData[nCntType][nCntParts].rot.y, &m_pFileData[nCntType][nCntParts].rot.z);
										}
									}
									nCntParts++;	// �p�[�c���̃J�E���g
								}
							}
						}
					}
					break;
				}
			}
			fclose(pFile);	// �t�@�C�������
		}
		else
		{// �t�@�C�����Ȃ��Ƃ�
			MessageBox(0, "�e�L�X�g�t�@�C��������܂���B", "ENEMY/model.txt", MB_YESNO);
		}
	}

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃f�[�^�̉��
//=========================================
void CEnemy::Unload(void)
{
	if (NULL != m_pFileData)
	{
		for (int nCntType = 0; nCntType < CModelSetManager::GetNumType(CModelSetManager::TYPE_ENEMY); nCntType++)
		{// ��ޕ����[�v
			if (NULL != m_pFileData[nCntType])
			{// �������̊J��
				delete[] m_pFileData[nCntType];
				m_pFileData[nCntType] = NULL;
			}
		}

		if (NULL != m_pFileData)
		{// �������̊J��
			delete[] m_pFileData;
			m_pFileData = NULL;
		}
	}

	if (NULL != m_pModelName)
	{// �p�[�c���̔j��
		for (int nCntType = 0; nCntType < CModelSetManager::GetNumType(CModelSetManager::TYPE_ENEMY); nCntType++)
		{// ��ޕ����[�v
			for (int nCntParts = 0; nCntParts < m_pNumParts[nCntType]; nCntParts++)
			{
				if (NULL != m_pModelName[nCntType][nCntParts])
				{// �j��
					delete[] m_pModelName[nCntType][nCntParts];
					m_pModelName[nCntType][nCntParts] = NULL;
				}
			}

			if (NULL != m_pModelName[nCntType])
			{// �j��
				delete[] m_pModelName[nCntType];
				m_pModelName[nCntType] = NULL;
			}
		}
		if (NULL != m_pModelName)
		{// �j��
			delete[] m_pModelName;
			m_pModelName = NULL;
		}
	}

	if (NULL != m_pNumParts)
	{// �p�[�c���̔j��
		delete[] m_pNumParts;
		m_pNumParts = NULL;
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CEnemy::CEnemy(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_pShadow = NULL;
	m_fAttackRange = 0.0f;
	m_fWalkRange = 0.0f;
	m_bAttack = false;
	m_nIntervalAttack = 0;
	m_fRotDest = 0.0f;
	m_pLife = NULL;
	m_fAlpha = 0.0f;

	for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
	{// �U�����[�V���������[�v
		m_nCntDamage[nCntDamage] = 0;
		m_bDamage[nCntDamage] = false;
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CEnemy::~CEnemy()
{
}

//=========================================
// ����������
//=========================================
HRESULT CEnemy::Init(void)
{
	if (NULL == m_pModel)
	{// ���f���N���X�̐���
		m_pModel = new CModel*[m_pNumParts[m_nType]];	// �p�[�c���̃��������m��

		for (int nCntModel = 0; nCntModel < m_pNumParts[m_nType]; nCntModel++)
		{// �������̊m��
			m_pModel[nCntModel] = CModel::Create();

			// ���f�����̎󂯓n��
			m_pModel[nCntModel]->SetModel(m_pModelName[m_nType][nCntModel]);

			if (m_pFileData[m_nType][nCntModel].nParent >= 0)
			{// �e�̃|�C���^��ݒ�
				m_pModel[nCntModel]->SetParent(m_pModel[m_pFileData[m_nType][nCntModel].nParent]);
			}
			else
			{// �e�����Ȃ��ꍇ
				m_pModel[nCntModel]->SetParent(NULL);
			}

			m_pModel[nCntModel]->Init();
			m_pModel[nCntModel]->SetPos(m_pFileData[m_nType][nCntModel].pos);
			m_pModel[nCntModel]->SetRot(m_pFileData[m_nType][nCntModel].rot);
		}
	}

	if (NULL == m_pMotion)
	{// ���[�V�����N���X�̐���
		m_pMotion = CMotion::Create(1, m_pNumParts[m_nType], &m_pModel[m_nType]);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);		// �j���[�g�������[�V������ݒ�
	}

	if (NULL == m_pShadow)
	{// �e�̐���
		m_pShadow = CShadow::Create(&m_pos);
	}

	if (NULL == m_pLife)
	{// �̗͂̐���
		float fHeight = 0.0f;
		int nMostHeight = 0;
		for (int nCntParts = 0; nCntParts < m_pNumParts[m_nType]; nCntParts++)
		{
			if (fHeight < m_pFileData[m_nType][nCntParts].pos.y)
			{// ���Y���������p�[�c��������
				fHeight = m_pFileData[m_nType][nCntParts].pos.y;
				nMostHeight = nCntParts;
			}
		}

		float fModelHeight = m_pModel[nMostHeight]->GetVtxMax().y;
		m_pLife = CLife3D::Create(ENEMY_LIFE, D3DXVECTOR3(-ENEMY_LIFE_WIDTH * 0.5f, fHeight + fModelHeight + ENEMY_LIFE_HEIGHT + 10.0f, 0.0f), ENEMY_LIFE_WIDTH, ENEMY_LIFE_HEIGHT, &m_pos);
	}

	// �U������͈̔�
	m_fAttackRange = 60.0f;
	m_fWalkRange = 400.0f;

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CEnemy::Uninit(void)
{
	if (m_pShadow != NULL)
	{// �e�̔j��
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	if (NULL != m_pLife)
	{// �̗̓N���X�̔j��
		m_pLife->Uninit();
		m_pLife = NULL;
	}

	if (m_pMotion != NULL)
	{// ���[�V�����N���X�̔j��
		m_pMotion->Uninit();
		m_pMotion = NULL;
	}

	for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
	{
		for (int nCntModel = 0; nCntModel < m_pNumParts[nCntType]; nCntModel++)
		{// ���f���̔j��
			if (NULL != m_pModel)
			{
				m_pModel[nCntModel]->Uninit();
				delete m_pModel[nCntModel];
				m_pModel[nCntModel] = NULL;
			}
		}
	}

	if (NULL != m_pModel)
	{// �������̊J��
		delete m_pModel;
		m_pModel = NULL;
	}

	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CEnemy::Update(void)
{
	CMotionManager::TYPE type = CMotionManager::TYPE_NEUTRAL;		// ���[�V�����̎��
	float fDiffAngle;												// �p�x�̍���
	CScene *pScene = NULL;											// �I�u�W�F�N�g�Ǘ��N���X�̃|�C���^�ϐ�

	if (m_pMotion && !m_bDeath)
	{// �g���Ă���
		type = m_pMotion->GetType();	// ���[�V�����̎��

		if (type != CMotionManager::TYPE_ATTACK01 && type != CMotionManager::TYPE_DAMAGE)
		{// �U�����[�V�����łȂ���
			if (m_move.x > 0.1f || m_move.x < -0.1f)
			{// �ړ����[�V������ݒ�
				m_pMotion->SetMotion(CMotionManager::TYPE_WALK);		// ���[�V�����̐ݒ�
			}
			else
			{// �j���[�g�������[�V������ݒ�
				m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);		// ���[�V�����̐ݒ�
			}
		}

		// �v���C���[�I�u�W�F�N�g��T��
		pScene = CScene::GetSceneTop(PLAYER_PRIORITY);

		while (pScene != NULL)
		{// NULL�ɂȂ�܂Ń��[�v
			CScene *pSceneNext = pScene->GetSceneNext();		// ���̃I�u�W�F�N�g��ۊ�
			CScene::OBJTYPE objType = pScene->GetObjType();		// �I�u�W�F�N�g�̎�ނ��擾

			if (objType == CScene::OBJTYPE_PLAYER)
			{// �v���C���[�������Ƃ�
				CPlayer *pPlayer = (CPlayer*)pScene;
				D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// �v���C���[�̍��W�̎擾

				if (m_pModel[0]->Collision(posPlayer, m_fAttackRange) && !m_bAttack)
				{// �U���͈͂Ƀv���C���[������
					m_pMotion->SetMotion(CMotionManager::TYPE_ATTACK01);	// ���[�V�����̐ݒ�
					m_bAttack = true;
				}
				else if (m_pModel[0]->Collision(posPlayer, m_fWalkRange) && CMotionManager::TYPE_ATTACK01 != type && !m_pModel[0]->Collision(posPlayer, m_fAttackRange))
				{// �v���C���[�ɋ߂Â�
					float fAngle = atan2f(posPlayer.x - m_pos.x, posPlayer.y - m_pos.y);
					m_move.x = sinf(fAngle) * 2.0f;
				}

				if (CMotionManager::TYPE_ATTACK01 == m_pMotion->GetType())
				{
					for (int nCntHit = 0; nCntHit < 5; nCntHit++)
					{// �����蔻���c�q��ɂ���
						if (NULL != m_pMotion)
						{// ���[�V�����N���X���g���Ă���Ƃ��̂�
							if (m_pMotion->Collision(D3DXVECTOR3(posPlayer.x, posPlayer.y + (nCntHit * 10), posPlayer.z)))
							{// �����蔻��
								pPlayer->Damage(-10);
								break;
							}
						}
					}
				}

				if (Collision(posPlayer, ENEMY_WIDTH))
				{// �v���C���[�ƂԂ�����
					m_move.x = 0.0f;

					// �v���C���[�̈ړ��ʂ����炷
					D3DXVECTOR3 move = pPlayer->GetMove();
					move *= 0.3f;
					pPlayer->SetMove(move);
				}

				// �v���C���[�̕���������
				if (posPlayer.x - m_pos.x <= 0)
				{
					m_fRotDest = D3DX_PI * -0.5f;
				}
				else
				{
					m_fRotDest = D3DX_PI * 0.5f;
				}
			}

			// ���̃I�u�W�F�N�g������
			pScene = pSceneNext;
		}
	}

	// ���]���Ȃ��悤�Ɋp�x�𒲐�
	if (m_fRotDest > D3DX_PI)
	{
		m_fRotDest -= D3DX_PI * 2.0f;
	}
	if (m_fRotDest < -D3DX_PI)
	{
		m_fRotDest += D3DX_PI * 2.0f;
	}

	// ���������߂�
	fDiffAngle = (m_fRotDest + D3DX_PI) - m_rot.y;

	// ���]���Ȃ��悤�Ɋp�x�𒲐�
	if (fDiffAngle > D3DX_PI)
	{
		fDiffAngle -= D3DX_PI * 2.0f;
	}
	if (fDiffAngle < -D3DX_PI)
	{
		fDiffAngle += D3DX_PI * 2.0f;
	}

	// ��]
	m_rot.y += fDiffAngle * 0.2f;

	// ���]���Ȃ��悤�Ɋp�x�𒲐�
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	if (m_bAttack)
	{// �U������Ԋu
		m_nIntervalAttack = (1 + m_nIntervalAttack) % 120;

		if (0 == m_nIntervalAttack)
		{// ��莞�Ԍo�߂ōU���t���O��false�ɂ���
			m_bAttack = false;
		}
	}

	// �d��
	m_move.y -= GRAVITY;

	// �ړ��ʂ̊���
	m_move.x += (0 - m_move.x) * 0.4f;
	m_pos += m_move;

	float fHeight = 0.0f;	// �N���̍��������߂�

	// �n�ʃI�u�W�F�N�g��T��
	pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// �n�ʂ������Ƃ�
			CMeshField *pMeshField = (CMeshField*)pScene;	// �n�ʂ̃|�C���^���擾

			fHeight = pMeshField->GetHeight(m_pos);			// �n�ʂ̍����̎擾
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	if (m_pos.y <= fHeight)
	{// �n�ʈȉ��ɂȂ����Ƃ�
		m_pos.y = fHeight;	// ���������W�ɓ����
		m_move.y = 0.0f;	// Y���̈ړ��ʂ𖳂���
	}

	// �G�I�u�W�F�N�g��T��
	pScene = CScene::GetSceneTop(ENEMY_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_ENEMY)
		{// �G�������Ƃ�
			CEnemy *pEnemy = (CEnemy*)pScene;	// �|�C���^���擾

			// �G���m�̓����蔻��
			pEnemy->Collision(m_pos, ENEMY_WIDTH);
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
	{// �U�����[�V���������[�v
		if (m_bDamage[nCntDamage] && !m_bDeath)
		{// �_���[�W���
			m_nCntDamage[nCntDamage] = (1 + m_nCntDamage[nCntDamage]) % ENEMY_DAMAGE_TIME;

			if (m_nCntDamage[nCntDamage] == 0)
			{// ��莞�Ԍo��
				m_bDamage[nCntDamage] = false;
			}
		}
	}

	if (m_bDeath)
	{// ���S���
		if (m_pos.y == fHeight)
		{// �n�ʂƓ�������
			if (m_pMotion)
			{// ���S���[�V����
				m_pMotion->SetMotion(CMotionManager::TYPE_WALK);
			}

			m_move.z = 5.0f;
			m_rot.y = D3DX_PI;

			m_fAlpha += 0.01f;
			if (m_fAlpha > 1.0f)
			{// �����ɂȂ����Ƃ��폜����
				Uninit();
			}
		}
	}
}

//=========================================
// �`�揈��
//=========================================
void CEnemy::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_pNumParts[0]; nCntModel++)
	{// �p�[�c���f���̕`��
		if (m_bDeath)
		{// ���S���
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, -m_fAlpha));
		}
		for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
		{// �U�����[�V���������[�v
			if (m_bDamage[nCntDamage])
			{// �_���[�W���
				m_pModel[nCntModel]->AddColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
				break;
			}
		}

		// ���f���̕`��
		m_pModel[nCntModel]->Draw();

		if (m_bDeath)
		{// ���S���
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fAlpha));
		}
		for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
		{// �U�����[�V���������[�v
			if (m_bDamage[nCntDamage])
			{// �_���[�W���
				m_pModel[nCntModel]->AddColor(D3DXCOLOR(-1.0f, 0.0f, 0.0f, 0.0f));
				break;
			}
		}
	}
}

//=========================================
// �����蔻��
//=========================================
bool CEnemy::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// �����XY�͈̔�
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_pos.x;						// X�̍�
	float fLengthY = pos.y - m_pos.y;						// Y�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);		// XY�̍��̓��

	if (fLength >= fLengthTotal)
	{// ����Ǝ����̔���
		if (fLengthX > 0)
		{// �E���ɂ���
			m_pos.x += fLengthX - fRange;
		}
		else if(fLengthX < 0)
		{// �����ɂ���
			m_pos.x -= fabsf(fLengthX) - fRange;
		}
		bHit = true;	// �����L���ɂ���
	}

	return bHit;	// ���茋�ʂ�Ԃ�
}

//=========================================
// ���f�����̎󂯓n��
//=========================================
void CEnemy::GetModelData(CModel**& pModel)
{
	m_pModel = pModel;
}

//=========================================
// �_���[�W���󂯂��Ƃ��̏���
//=========================================
void CEnemy::Damage(int nDamage, int nMotion)
{
	if (0 < m_pLife->GetLife())
	{
		if (nMotion < ATTACK_MOTION && nMotion >= 0)
		{// �z����z���Ȃ��悤�ɂ���
			if (NULL != m_pLife && !m_bDamage[nMotion])
			{// ���C�t�N���X���g���Ă���
				m_pLife->AddSubtract(nDamage);

				// �_���[�W��Ԃɂ���
				m_pMotion->SetMotion(CMotionManager::TYPE_DAMAGE);
				m_bDamage[nMotion] = true;
				CSound *pSound = CManager::GetSound();			// �T�E���h�̎擾
				pSound->PlaySound(CSound::SOUND_LABEL_HIT);		// �U���q�b�g���̍Đ�

				if (0 >= m_pLife->GetLife())
				{// �̗͂�0�ȉ��ɂȂ���
					m_bDeath = true;
					m_nCntDamage[nMotion] = 0;
					pSound->PlaySound(CSound::SOUND_LABEL_ENEMYDEATH);	// �G���S���̍Đ�
					m_move.y = 10.0f;

					if (m_pMotion)
					{// ���S���[�V����
						m_pMotion->SetMotion(CMotionManager::TYPE_DEATH);
					}
				}
			}
		}
		else if (-1 == nMotion)
		{// ���[�V������-1�̂Ƃ�
			CSound *pSound = CManager::GetSound();			// �T�E���h�̎擾
			pSound->PlaySound(CSound::SOUND_LABEL_HIT);		// �U���q�b�g���̍Đ�

			m_pLife->AddSubtract(nDamage);	// ���C�t��0�ɂ���

			if (0 >= m_pLife->GetLife())
			{// �̗͂�0�ȉ��ɂȂ���
				m_bDeath = true;
				pSound->PlaySound(CSound::SOUND_LABEL_ENEMYDEATH);	// �G���S���̍Đ�

				m_move.y = 15.0f;

				if (m_pMotion)
				{// ���S���[�V����
					m_pMotion->SetMotion(CMotionManager::TYPE_DEATH);
				}
			}
		}
	}
}