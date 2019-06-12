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
#include "meshField.h"
#include "modelObject.h"
#include "shadow.h"
#include "setPlayer.h"

#include <stdio.h>

//==================================
// �}�N����`
//==================================
#define ENEMY_MODEL_FILE	"data/TEXT/ENEMY/model.txt"
#define ENEMY_SPEED		(0.4f)	// �ړ��̑��x
#define ENEMY_LIFE		(2)		// ���C�t
#define ENEMY_SHOOT		(120)	// ���Ԋu

//==================================
// �ÓI�����o�ϐ��錾
//==================================
int				*CEnemy::m_pNumParts = {};				// �p�[�c��
char			***CEnemy::m_pModelName = NULL;			// ���f����
CEnemy::DATA	**CEnemy::m_pFileData = NULL;			// �t�@�C���ǂݎ��

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
	char **pModelFileName = CModelObjectManager::GetModelFileName(CModelObjectManager::TYPE_ENEMY);	// ���f�����̎擾
	int nNumType = CModelObjectManager::GetNumType(CModelObjectManager::TYPE_ENEMY);
	int nCntModel = 0;

	// ���f�������������m��
	m_pNumParts = new int[nNumType];
	m_pModelName = new char**[nNumType];
	m_pFileData = new DATA*[nNumType];

	for (int nCntType = 0; nCntType < nNumType; nCntType++)
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
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CEnemy::Unload(void)
{
	if (m_pFileData != NULL)
	{
		for (int nCntType = 0; nCntType < CModelObjectManager::GetNumType(CModelObjectManager::TYPE_ENEMY); nCntType++)
		{// ��ޕ����[�v
			if (m_pFileData[nCntType] != NULL)
			{// �������̊J��
				delete[] m_pFileData[nCntType];
				m_pFileData[nCntType] = NULL;
			}
		}

		// �������̊J��
		delete m_pFileData;
		m_pFileData = NULL;
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CEnemy::CEnemy(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pShadow = NULL;
	m_pModel = NULL;
	m_pMotion = NULL;
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
	if (m_pModel == NULL)
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

			m_pModel[nCntModel]->SetPos(m_pFileData[m_nType][nCntModel].pos);
			m_pModel[nCntModel]->SetRot(m_pFileData[m_nType][nCntModel].rot);
		}
	}

	if (m_pMotion == NULL)
	{// ���[�V�����N���X�̐���
		m_pMotion = CMotion::Create(1, m_pNumParts[m_nType], m_pModel);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// �j���[�g�������[�V������ݒ�
	}

	if (m_pShadow == NULL)
	{// �e�̐���
		m_pShadow = CShadow::Create();
	}

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CEnemy::Uninit(void)
{
	if (m_pShadow != NULL)
	{// �e�̔j��
		m_pShadow = NULL;
	}

	if (m_pMotion != NULL)
	{// ���[�V�����N���X�̔j��
		m_pMotion->Uninit();
	}

	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CEnemy::Update(void)
{
	float fHeight = 0.0f;	// �N���̍��������߂�

	// �n�ʃI�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

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

	// �n�ʂ̍����ɍ��킹��
	m_pos.y = fHeight;

	// �͈͓����ǂ���
	CSetPlayer *pSetPlayer = CManager::GetSetPlayer();
	m_bRange = pSetPlayer->Range(m_pos);
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

	if (m_pShadow != NULL)
	{// �g���Ă���
		for (int nCntModel = 0; nCntModel < m_pNumParts[m_nType]; nCntModel++)
		{// �e�̕`��
			m_pShadow->Set(m_pModel[nCntModel]->GetMtxWorld(), m_pModel[nCntModel]->GetMesh(), m_pos);
		}
	}

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_pNumParts[0]; nCntModel++)
	{// �p�[�c���f���̕`��
		if (m_bRange)
		{
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, -0.3f));
		}

		m_pModel[nCntModel]->Draw();

		if (m_bRange)
		{
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(-1.0f, 0.0f, 0.0f, 0.3f));
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

	float fLengthX = pos.x - m_mtxWorld._41;						// X�̍�
	float fLengthY = pos.y - m_mtxWorld._42;						// Y�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);		// XY�̍��̓��

	if (fLength >= fLengthTotal)
	{// ����Ǝ����̔���
		if (fLengthX > 0)
		{// �v���C���[�̉E���ɂ���
			m_pos.x += fLengthX - fRange;
		}
		else
		{// �v���C���[�̍����ɂ���
			m_pos.x -= fabsf(fLengthX) - fRange;
		}
		bHit = true;	// �����L���ɂ���
	}

	return bHit;	// ���茋�ʂ�Ԃ�
}

//=========================================
// ���f�����̐ݒ�
//=========================================
void CEnemy::GetInfo(int nType, CModel **pModel)
{
	for (int nCntModel = 0; nCntModel < m_pNumParts[nType]; nCntModel++)
	{// �������̊m��
		// ���f�����̎󂯓n��
		pModel[nCntModel]->SetModel(m_pModelName[nType][nCntModel]);

		if (m_pFileData[nType][nCntModel].nParent >= 0)
		{// �e�̃|�C���^��ݒ�
			pModel[nCntModel]->SetParent(pModel[m_pFileData[nType][nCntModel].nParent]);
		}
		else
		{// �e�����Ȃ��ꍇ
			pModel[nCntModel]->SetParent(NULL);
		}

		pModel[nCntModel]->SetPos(m_pFileData[nType][nCntModel].pos);
		pModel[nCntModel]->SetRot(m_pFileData[nType][nCntModel].rot);
	}
}

//=========================================
// �͈͓��̓G���폜
//=========================================
void CEnemy::Range(void)
{
	if (m_bRange)
	{// �͈͓��̂Ƃ��폜
		Uninit();
	}
}