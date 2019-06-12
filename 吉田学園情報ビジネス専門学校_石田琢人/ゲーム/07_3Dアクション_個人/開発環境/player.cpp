//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "meshField.h"
#include "camera.h"
#include "model.h"
#include "motion.h"
#include "shadow.h"
#include "game.h"
#include "enemy.h"
#include "stage.h"
#include "modelSet.h"
#include "fade.h"
#include "life.h"
#include "particle.h"
#include "sound.h"

#include <stdio.h>

//==================================
// �}�N����`
//==================================
#define MODEL_FILE "data/TEXT/PLAYER/model.txt"
#define MOVE_SPEED			(4.0f)		// �ړ��̑��x
#define PLAYER_SLIP_ANGLE	(0.8f)		// ����悤�ɂȂ�p�x
#define JUMP_SPEED			(4.0f)		// �W�����v�̑��x
#define SLIP_ENABLE_SPEED	(1.0f)		// ����悤�ɂȂ鑬�x
#define PLAYER_LIFE			(100)		// �̗�
#define	PLAYER_LIFE_WIDTH	(500.0f)	// �̗̓o�[�̕�
#define PLAYER_LIFE_HEIGHT	(50.0f)		// �̗̓o�[�̍���
#define PLAYER_DAMAGE_TIME	(60)		// �_���[�W���󂯂����̖��G����
#define PLAYER_DAMAGE_MOVE	(40)		// �_���[�W���󂯂Ă��瓮����悤�ɂȂ�܂ł̎���
#define PLAYER_CLIMB_SPEED	(1.1f)		// �o��Ƃ��̑��x�̌�����
#define PLAYER_SLIP_SPEED	(1.7f)		// ���鑬�x
#define PLAYER_SLIP_JUMP	(0.45f)		// ������уW�����v���x�␳
#define PLAYER_JUMP_DASH	(25.0f)		// �W�����v���̋󒆃_�b�V���̑��x�␳

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// ��������
//==================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// ���������m��

	if (pPlayer != NULL)
	{// �������m�ې���
		pPlayer->Init();
		pPlayer->m_pos = pos;
	}

	return pPlayer;
}

//=========================================
// �R���X�g���N�^
//=========================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntState = 0;
	m_rotDest = 0.0f;
	m_fSpeed = 0.0f;
	m_fSlipSpeed = 0.0f;
	m_bJump = false;
	m_bSlip = false;
	m_fJumpSpeed = 0.0f;
	m_pModel = NULL;
	m_nNumParts = 0;
	m_fAngleSlip = 0.0f;
	m_pMotion = NULL;
	m_pLife = NULL;
	m_bDamage = false;
	m_bMove = false;
	m_nCntParticle = 0;
	m_bDash = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CPlayer::~CPlayer()
{
}

//=========================================
// ����������
//=========================================
HRESULT CPlayer::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntParts = 0;		// �p�[�c���̃J�E���^
	int nCntModel = 0;
	float fX, fY, fZ;
	int nId;
	int nParent;
	char **pModelName = NULL;

	// �t�@�C�����J��
	pFile = fopen(MODEL_FILE, "r");

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

						pModelName = new char*[nNumModel];		// �������m��
					}
					if (strcmp(aStr, "MODEL_FILENAME") == 0)
					{// �L�����N�^�[���
						fscanf(pFile, " = %s", &aStr);

						pModelName[nCntModel] = new char[64];		// �������m��
						strcpy(pModelName[nCntModel], aStr);
						nCntModel++;
					}
					if (strcmp(aStr, "CHARACTERSET") == 0)
					{// �L�����N�^�[���
						while (strcmp(aStr, "END_CHARACTERSET") != 0)
						{// �L�����N�^�[���I�����b�Z�[�W�܂Ń��[�v
							fscanf(pFile, "%s", aStr);

							if (strcmp(aStr, "NUM_PARTS") == 0)
							{// �p�[�c��
								fscanf(pFile, " = %d", &m_nNumParts);
								m_pModel = new CModel*[m_nNumParts];

								for (int nCntModelParts = 0; nCntModelParts < m_nNumParts; nCntModelParts++)
								{// �p�[�c�������[�v
									m_pModel[nCntModelParts] = CModel::Create();
								}
							}
							else if (strcmp(aStr, "PARTSSET") == 0)
							{// �p�[�c�̐ݒ�
								while (strcmp(aStr, "END_PARTSSET") != 0)
								{// �p�[�c�̐ݒ�I�����b�Z�[�W�܂Ń��[�v
									if (m_pModel[nCntParts] != NULL)
									{// �������m�ې���
										fscanf(pFile, "%s", &aStr);

										if (strcmp(aStr, "INDEX") == 0)
										{// ���f���ԍ�
											fscanf(pFile, " = %d", &nId);

											// �g�����f�����w��
											m_pModel[nCntParts]->SetModel(pModelName[nId]);
										}
										else if (strcmp(aStr, "PARENT") == 0)
										{// �e�����߂�
											fscanf(pFile, " = %d", &nParent);

											if (nParent >= 0)
											{// �e�̃|�C���^��ݒ�
												m_pModel[nCntParts]->SetParent(m_pModel[nParent]);
											}
											else
											{// �e�����Ȃ��ꍇ
												m_pModel[nCntParts]->SetParent(NULL);
											}
										}
										else if (strcmp(aStr, "POS") == 0)
										{// �ʒu�̐ݒ�
											fscanf(pFile, " = %f %f %f", &fX, &fY, &fZ);

											// �ʒu�̐ݒ�
											m_pModel[nCntParts]->SetPos(D3DXVECTOR3(fX, fY, fZ));
										}
										else if (strcmp(aStr, "ROT") == 0)
										{// �����̐ݒ�
											fscanf(pFile, " = %f %f %f", &fX, &fY, &fZ);

											// �����̐ݒ�
											m_pModel[nCntParts]->SetRot(D3DXVECTOR3(fX, fY, fZ));
										}
									}
								}
								nCntParts++;
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
		MessageBox(0, "�t�@�C��������܂���", "player/model.txt", MB_YESNO);
	}

	if (NULL != pModelName)
	{// �������̊J��
		for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
		{// �������̊J��
			delete[] pModelName[nCnt];
		}

		// �������̊J��
		delete[] pModelName;
	}

	// �|���S���̈ʒu��ݒ�
	m_fSpeed = MOVE_SPEED;	// ���x�̐ݒ�

	if (NULL == m_pMotion)
	{// ���[�V�����N���X�̐���
		m_pMotion = CMotion::Create(0, m_nNumParts, m_pModel);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// �j���[�g�������[�V������ݒ�
	}

	if (NULL == m_pShadow)
	{// �e�̐���
		m_pShadow = CShadow::Create(&m_pos);
	}

	if (NULL == m_pLife && CManager::MODE_GAME == CManager::GetMode())
	{// �̗͂̐���
		m_pLife = CLife2D::Create(PLAYER_LIFE, D3DXVECTOR3(10.0f, 0.0f, 0.0f), PLAYER_LIFE_WIDTH, PLAYER_LIFE_HEIGHT);
	}

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CPlayer::Uninit(void)
{
	if (NULL != m_pMotion)
	{// ���[�V�����N���X�̔j��
		m_pMotion->Uninit();
		m_pMotion = NULL;
	}

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// ���f���̔j��
		if (NULL != m_pModel[nCntModel])
		{
			m_pModel[nCntModel]->Uninit();
			delete m_pModel[nCntModel];
			m_pModel[nCntModel] = NULL;
		}
	}

	if (NULL != m_pModel)
	{// �������̊J��
		delete[] m_pModel;
		m_pModel = NULL;
	}

	if (NULL != m_pLife)
	{// ���C�t�̔j��
		m_pLife->Uninit();
		m_pLife = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();						// �T�E���h�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CXInput *pXInput = CManager::GetXInput();					// XInput�̓��͂��擾

	D3DXVECTOR3 posOld = m_pos;		// �ʒu�̕ۑ�
	float fDiffAngle = 0.0f;		// ��]�̍���
	bool bJumpOld = m_bJump;		// �W�����v�t���O�̋L��
	bool bMoveKey = false;			// �ړ��L�[�����t���O
	float fAngleOld = m_fAngle;		// �n�ʂƂ̌�_�̊p�x��ۑ�

	// ���x������
	m_fSpeed = MOVE_SPEED;

	// �X�΂̏���
	Tilt();

	if (m_bDamage)
	{// ���G����
		m_nCntState = (1 + m_nCntState) % PLAYER_DAMAGE_TIME;
		m_bMove = false;

		if (m_nCntState == 0)
		{// ��莞�Ԍo��
			m_bDamage = false;	// ���G���Ԃ�����
		}
	}

	if (m_nCntState > PLAYER_DAMAGE_MOVE || m_nCntState == 0)
	{// ��莞�Ԍo��
		m_bMove = true;		// ������悤�ɂ���
	}

	// �J�����̊p�x���擾
	float rotCamera = CManager::GetCamera()->GetRotY();
	if ((!m_bSlip && m_fSlipSpeed > -3.0f && m_fSlipSpeed < 3.0f) && m_nCntState == 0 || m_nCntState > PLAYER_DAMAGE_TIME * 0.5f)
	{// �L�[���͂ɂ��ړ�
		if (pKeyboard->GetPress(DIK_A) || pXInput->GetThumbLX(0) <= -MIN_GAMEPAD_LEFT_THUMB_X)
		{// �������ֈړ�
			m_rotDest = D3DX_PI * 0.5f + rotCamera;

			// �ړ��t���O�𗧂Ă�
			bMoveKey = true;
		}
		else if (pKeyboard->GetPress(DIK_D) || pXInput->GetThumbLX(0) >= MIN_GAMEPAD_LEFT_THUMB_X)
		{// �������ֈړ�
			m_rotDest = D3DX_PI * -0.5f + rotCamera;

			// �ړ��t���O�𗧂Ă�
			bMoveKey = true;
		}

		if (bMoveKey)
		{// �ړ��L�[�������ꂽ
			if (m_bJump)
			{// �W�����v��
				m_move += D3DXVECTOR3(sinf(m_rotDest), 0.0f, 0.0f) * m_fJumpSpeed;
			}
			else if (m_fSlipSpeed == 0)
			{// �����Ă���p�x�ւ̈ړ���
				m_move += D3DXVECTOR3(sinf(m_rotDest), 0.0f, 0.0f) * m_fSpeed;
			}
			else
			{// 0�ɂȂ�܂Ō��炷
				m_fSlipSpeed += (0 - m_fSlipSpeed) * 0.3f;
			}

			if (((0 == m_pMotion->GetCurKey() && 0 == m_pMotion->GetCurFlame()) || (1 == m_pMotion->GetCurKey() && 0 == m_pMotion->GetCurFlame())) && !m_bJump)
			{// �������Ƃ��̃p�[�e�B�N��
				CParticle::Create(m_pos, 1);
			}
		}
	}
	else if (m_bSlip && !m_bJump)
	{// �����Ă���
		float fRot = 0;				// �ړ��ʂ𑫂��p�x
		float fMoveX = m_move.x;	// �ړ��ʂ̕ۑ�
		bool bBefore = false;		// �L�[���͑O�̈ړ�����
		bool bAfter = false;		// �L�[���͌�̈ړ�����

		m_pMotion->SetMotion(CMotionManager::TYPE_SLIP);
		if (m_move.x > 0)
		{// X�̈ړ��ʂ���
			bBefore = true;
		}

		// �L�[���͂ɂ��ړ�---------------------------------------------------------------------------------------------
		if (pKeyboard->GetPress(DIK_A) || pXInput->GetThumbLX(0) <= -MIN_GAMEPAD_LEFT_THUMB_X)
		{// �������ֈړ�
			fRot = D3DX_PI * 0.5f + rotCamera;

			// �����Ă���p�x�ւ̈ړ���
			m_move += D3DXVECTOR3(sinf(fRot), 0.0f, 0.0f) * (m_fSlipSpeed * 0.2f);
		}
		else if (pKeyboard->GetPress(DIK_D) || pXInput->GetThumbLX(0) >= MIN_GAMEPAD_LEFT_THUMB_X)
		{// �������ֈړ�
			fRot = D3DX_PI * -0.5f + rotCamera;

			// �����Ă���p�x�ւ̈ړ���
			m_move += D3DXVECTOR3(sinf(fRot), 0.0f, 0.0f) * (m_fSlipSpeed * 0.2f);
		}

		if (m_move.x > 0)
		{// X�̈ړ��ʂ���
			bAfter = true;
		}

		if (bAfter != bBefore)
		{// �������ς���Ă���
			m_move.x = fMoveX * 0.5f;	// ���C�̒l�𒲐�
		}
	}

	CDebugProc::Print("���x�F%.2f", m_fSpeed);
	if (m_pMotion)
	{// ���[�V�����N���X���g���Ă���
		if (bMoveKey && !m_bJump && m_bMove)
		{// �ړ����[�V����
			m_pMotion->SetMotion(CMotionManager::TYPE_WALK);
		}
		else if (!bMoveKey && m_bMove && (CMotionManager::TYPE_WALK == m_pMotion->GetType() || CMotionManager::TYPE_SLIP == m_pMotion->GetType() && !m_bSlip))
		{// �j���[�g�������[�V����
			m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
		}
	}

	// ���]���Ȃ��悤�Ɋp�x�𒲐�
	if (m_rotDest > D3DX_PI)
	{
		m_rotDest -= D3DX_PI * 2.0f;
	}
	if (m_rotDest < -D3DX_PI)
	{
		m_rotDest += D3DX_PI * 2.0f;
	}

	// ���������߂�
	fDiffAngle = (m_rotDest + D3DX_PI) - m_rot.y;

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
	m_rot.y += fDiffAngle * (fabsf(((float)m_bSlip * -0.97f + 1.0f)) * 0.1f + 0.1f);

	// ���]���Ȃ��悤�Ɋp�x�𒲐�
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	// �d��
	m_move.y -= GRAVITY;

	CDebugProc::Print("�X���b�v : %s", m_bSlip ? "���Ă�" : "���ĂȂ�");

	// �ړ��ʂ̊���
	if (!m_bJump) { m_move.x += (0 - m_move.x) * 0.6f; }
	else { m_move.x += (0 - m_move.x) * 0.005f; }

	CGame::MoveMountain(m_move.x);

	float fHeight = 0.0f;	// �N���̍��������߂�

	if (m_fAngle < 0.0f && fAngleOld >= 0.0f && !m_bJump && m_bSlip)
	{// ����Ă���Ƃ��Œn�ʂɂ��Ă��Ȃ��Ƃ�
		m_move.y = fAngleOld * m_fSlipSpeed * PLAYER_SLIP_JUMP;
		if (m_move.y > 0.0f)
		{// ��ɔ�񂾎��W�����v��Ԃɂ���
			m_bJump = true;
			m_bDash = false;
			pSound->PlaySound(CSound::SOUND_LABEL_JUMP);
		}
		m_move.x = sinf(m_rotDest) * m_fSlipSpeed * PLAYER_SLIP_JUMP;
		m_pMotion->SetMotion(CMotionManager::TYPE_JUMP);
	}

	if (m_bJump && !m_bDash)
	{// ���[�V�����N���X���g���Ă���
		if ((pKeyboard->GetTrigger(DIK_SPACE) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_10) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_12) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_13)) && m_bMove)
		{// �󒆃_�b�V��
			m_move.x = sinf(m_rotDest) * PLAYER_JUMP_DASH;
			m_move.y += PLAYER_JUMP_DASH * 0.1f;
			m_bDash = true;
		}
	}

	if (m_bDash)
	{// �󒆃_�b�V����
		CParticle::Create(m_pos, 4);
	}

	// �ړ�������
	m_pos += m_move;

	// �n�ʃI�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);
	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// �n�ʂ������Ƃ�
			CMeshField *pMeshField = (CMeshField*)pScene;

			fHeight = pMeshField->GetHeight(m_pos);		// �n�ʂ̍����̎擾
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	// �I�u�W�F�N�g���f����T��
	pScene = CScene::GetSceneTop(MODEL_PRIORITY);
	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_MODELOBJECT)
		{// �z�u���������Ƃ�
			CModelObject *pModelObject = (CModelObject*)pScene;

			for (int nCntHeight = 0; nCntHeight < 10; nCntHeight++)
			{
				if (pModelObject->Collision(&m_pos, 45.0f))
				{// �����蔻��
					m_move.x = 0;
					m_fJumpSpeed = 0.0f;
					m_fSlipSpeed = 0.0f;
					m_fSpeed = 0.0f;
					break;
				}
			}
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	if (m_pos.y <= fHeight || !m_bJump)
	{// �n�ʈȉ��ɂȂ����Ƃ�
		m_pos.y = fHeight;	// ���������W�ɓ����
		m_bJump = false;	// ���n��Ԃɂ���
		m_move.y = 0.0f;	// Y���̈ړ��ʂ𖳂���
		m_bDash = false;	// �󒆃_�b�V���t���O�𕉂ɂ���
	}

	if (!m_bJump && bJumpOld)
	{// ���n�����Ƃ�
		m_fSlipSpeed *= 0.2f;	// ����ړ��ʂ����炷

		if (m_pMotion)
		{// ���[�V�����N���X���g���Ă���
			m_pMotion->SetMotion(CMotionManager::TYPE_LANDING);
		}
	}

	if (m_pMotion)
	{// ���[�V�����N���X���g���Ă���
		pScene = CScene::GetSceneTop(ENEMY_PRIORITY);		// �G�I�u�W�F�N�g��T��

		while (pScene != NULL)
		{// NULL�ɂȂ�܂Ń��[�v
			CScene *pSceneNext = pScene->GetSceneNext();
			CScene::OBJTYPE objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE_ENEMY)
			{// �G�������Ƃ�
				CEnemy *pEnemy = (CEnemy*)pScene;
				D3DXVECTOR3 posEnemy = pEnemy->GetPos();

				if (m_bSlip)
				{// �����Ă���
					if (m_pModel[6]->Collision(posEnemy, 50.0f))
					{// �����蔻��
						pEnemy->Damage(-10, -1);

						// �q�b�g�G�t�F�N�g
						CParticle::Create(posEnemy, 3);
					}
				}
			}
			// ���̃I�u�W�F�N�g������
			pScene = pSceneNext;
		}
	}

	if (Collision(CModelSetManager::GetGoalPos(), 70.0f) && CManager::MODE_GAME == CManager::GetMode() && CFade::FADE_NONE == CFade::GetFade())
	{// �S�[�������Ƃ�
		m_bMove = false;
		m_bSlip = false;
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_fSlipSpeed = 0.0f;

		if (!m_bStageFade)
		{// �X�e�[�W�ړ��̐ݒ�
			CGame::NextStage();
		}
		m_bStageFade = true;
	}
	else if(m_bStageFade)
	{// �X�e�[�W�J�ڂ��I�����
		m_bStageFade = false;
	}

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// �p�[�c���f���̍X�V
		m_pModel[nCntModel]->Update();
	}
}

//=========================================
// �`�揈��
//=========================================
void CPlayer::Draw(void)
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

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// �p�[�c���f���̕`��
		if (m_bDamage)
		{// �_���[�W���󂯂Ă�����
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.7f, 0.0f, 0.0f, -0.5f));
		}

		// ���f���̕`�揈��
		m_pModel[nCntModel]->Draw();

		if (m_bDamage)
		{// �_���[�W���󂯂Ă�����
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(-0.7f, 0.0f, 0.0f, 0.5f));
		}
	}
}

//=========================================
// �X�΂̏���
//=========================================
void CPlayer::Tilt(void)
{
	CSound *pSound = CManager::GetSound();			// �T�E���h�̎擾
	float fCosTheta = 0.0f;							// �v�Z����
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f,0.0f,0.0f);	// �@��

	// �n�ʃI�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// �n�ʂ������Ƃ�
			CMeshField *pMeshField = (CMeshField*)pScene;

			m_fAngle = pMeshField->GetAngle(m_pos, m_rot.y);	// �X�΂̊p�x�̎擾
			nor = pMeshField->GetNor(m_pos);					// �n�ʂ̖@���̎擾

			if (!m_bJump)
			{// ��̉���������擾
				m_fAngleSlip = pMeshField->GetSlipAngle(m_pos);
			}
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	CDebugProc::Print("�n�ʂƂ�cos�ƁF%.2f", m_fAngle);

	// �J�����̎擾
	CCamera *pCamera = CManager::GetCamera();

	if (!m_bSlip)
	{// �����Ă��Ȃ�
		if (!m_bJump && nor.y < PLAYER_SLIP_ANGLE && nor.y != 0.0f && 0 > m_fAngle)
		{// �W�����v���ĂȂ����p�x�����ȉ��Ŋ���
			m_bSlip = true;
			pSound->PlaySound(CSound::SOUND_LABEL_SLIP);

			if (m_fAngle < 0.0f)
			{// ���艺���Ƃ����x������
				m_fSlipSpeed = m_fSpeed;
			}
		}

		if (!m_bJump)
		{// �n��
			if (m_fAngle < 0.0f)
			{// �����Ƃ��ɑ��x���グ��
				m_fSpeed += MOVE_SPEED * (1.0f - nor.y);
			}
			else if (m_fAngle > 0.0f)
			{// ���Ƃ��ɑ��x��������
				m_fSpeed -= MOVE_SPEED * (1.0f - nor.y) * PLAYER_CLIMB_SPEED;
			}
		}

		// �����𗣂�
		pCamera->SetType(false);
	}
	else
	{// �����Ă���
		if(!m_bJump)
		{// ����ړ��ʂ𑝂₷
			m_fSlipSpeed += PLAYER_SLIP_SPEED * (1.0f - nor.y) * -m_fAngle;

			// ���藎����Ƃ��̃p�[�e�B�N��
			CParticle::Create(m_pos, 2);
		}
		else
		{// �W�����v��
			pSound->StopSound(CSound::SOUND_LABEL_SLIP);
		}

		// �������k�߂�
		pCamera->SetType(true);
	}

	if (m_fSlipSpeed > -3.0f && m_fSlipSpeed < 3.0f)
	{// ���x�����ȉ��Œʏ��
		m_bSlip = false;
		pSound->StopSound(CSound::SOUND_LABEL_SLIP);
		m_fSlipSpeed = 0;
	}

	if (!m_bJump)
	{// ����ړ���
		m_move.x += sinf(m_fAngleSlip) * m_fSlipSpeed;

		if (m_bSlip)
		{// �����Ă���Ƃ�
			if (0.0 <= m_rot.y && 0 < m_move.x)
			{// �ړ��ʂ��t�ɂ���
				m_move.x *= -1.0f;
			}
			else if (0.0 >= m_rot.y && 0 > m_move.x)
			{// �ړ��ʂ��t�ɂ���
				m_move.x *= -1.0f;
			}
		}
	}

	// �ړ��ʂ̃x�N�g��
	D3DXVECTOR3 vecMove = (m_pos + m_move) - m_pos;

	// �ړ��ʃx�N�g���Ɩ@����cos�Ƃ����߂�
	fCosTheta = (vecMove.x * nor.x) + (vecMove.y * nor.y) + (vecMove.z * nor.z);
	fCosTheta = fCosTheta / (sqrtf(powf(vecMove.x, 2) + powf(vecMove.y, 2) + powf(vecMove.z, 2)) * (sqrtf(powf(nor.x, 2) + powf(nor.y, 2) + powf(nor.z, 2))));

	if (isnan(fCosTheta))
	{// nan�`�F�b�N
		fCosTheta = 0.0f;
	}

	// �p�x�ɂ���ăp�[�e�B�N����ς���
	if (nor.y < PLAYER_SLIP_ANGLE && 0 < m_fAngle && (m_move.x >= 0.01f || m_move.x <= -0.01f))
	{// �����o��
		m_nCntParticle = (1 + m_nCntParticle) % 15;

		if (m_nCntParticle == 0)
		{// �p�[�e�B�N�����o��
			CParticle::Create(m_pos + D3DXVECTOR3(0.0f, 40.0f, 0.0f), 0);
		}

		// �������k�߂�
		pCamera->SetType(true);
	}

	CDebugProc::Print("���鑬�x�F%.2f", m_fSlipSpeed);
	CDebugProc::Print("�W�����v���x�F%.2f", m_fJumpSpeed);
}

//=========================================
// �����蔻��
//=========================================
bool CPlayer::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// �����XY�͈̔�
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_mtxWorld._41;	// X�̍�
	float fLengthY = pos.y - m_mtxWorld._42;	// Y�̍�
	float fLengthZ = pos.z - m_mtxWorld._43;	// Z�̍�

	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);		// XYZ�̍��̓��

	if (fLength >= fLengthTotal)
	{// ����Ǝ����̔���
		bHit = true;	// �����L���ɂ���
	}

	return bHit;	// ���茋�ʂ�Ԃ�
}

//=========================================
// �_���[�W���󂯂��Ƃ��̏���
//=========================================
void CPlayer::Damage(int nDamage)
{
	if (m_pMotion)
	{// ���[�V�����N���X���g���Ă���
		if (NULL != m_pLife && m_pMotion->GetType() != CMotionManager::TYPE_DAMAGE && !m_bDamage)
		{// ���C�t�N���X���g���Ă���
			m_pLife->AddSubtract(nDamage);
			m_pMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// �_���[�W���[�V�������Đ�
			m_bDamage = true;									// �_���[�W���󂯂Ă����Ԃɂ���
			CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
			pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// �_���[�W�����Đ�

			if (0 >= m_pLife->GetLife())
			{// �̗͂�0�ȉ��ɂȂ���
				Uninit();
				CGame::SetGameState(CGame::STATE_END);
			}
		}
	}
}