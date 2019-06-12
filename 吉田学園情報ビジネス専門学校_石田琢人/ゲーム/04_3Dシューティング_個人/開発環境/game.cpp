//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "camera.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "field.h"
#include "model.h"
#include "number.h"
#include "score.h"
#include "timer.h"
#include "polygon.h"
#include "effect.h"
#include "stock.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "pause.h"
#include "gauge.h"
#include "scene.h"
#include "camera.h"
#include "particle.h"
#include "meshCylinder.h"
#include "communicateWindow.h"
#include "shadow.h"
#include "sound.h"
#include "string.h"

#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMYINFO_FILENAME	"data/TEXT/enemy_info.txt"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CPlayer *CGame::m_pPlayer = NULL;
CField *CGame::m_pField[MAX_FIELD] = {};
bool CGame::m_bPause = false;
CGame::State CGame::m_state = CGame::STATE_NONE;
int CGame::m_stateCounter = 0;
CEnemyInfo CEnemyInfo::m_aEnemyInfo[MAX_ENEMY] = {};	// �G�̏��

//=========================================
// �R���X�g���N�^
//=========================================
CGame::CGame()
{
	m_apPolygon = NULL;
	m_pause = NULL;
	m_nCntEnemyAppear = 0;
	m_pString = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CGame::~CGame()
{
}

//=============================================================================
// �Q�[������������
//=============================================================================
HRESULT CGame::Init(void)
{
	CBullet::Load();
	CPlayer::Load();
	CEnemy::Load();
	CEffect::Load();
	CField::Load();
	CModel::Load();
	CNumber::Load();
	CPolygon::Load();
	CParData::Load();
	CParticlePolygon::Load();
	CEnemyInfo::LoadEnemy();
	CCylinder::Load();
	CShadow::Load();
	CString::Load();

	// �I�u�W�F�N�g�N���X�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0, 200, 0),CPlayer::TYPE_AIRCRAFT);

	CCylinder::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), 8000, 7000, 8000, 20, 1, 0, 0);

	m_pField[0] = CField::Create(CField::TYPE_GROUND);
	m_pField[1] = CField::Create(CField::TYPE_SEA);
	m_pField[2] = CField::Create(CField::TYPE_GROUND);

	m_apPolygon = CPolygon::Create(1, D3DXVECTOR3(0.0f, 620.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT - 620);
	m_apPolygon->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f));

	m_pString = CString::Create();

	CScore::Create(D3DXVECTOR3(800, 670, 0));
	CTimer::Create(D3DXVECTOR3(1200, 670, 0));
	CStock::Create();
	CGauge::Create();

	m_bPause = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
	// �e�N�X�`���̔j��
	CBullet::Unload();
	CPlayer::Unload();
	CEnemy::Unload();
	CField::Unload();
	CModel::Unload();
	CNumber::Unload();
	CPolygon::Unload();
	CParData::Unload();
	CParticlePolygon::Unload();
	CCylinder::Unload();
	CShadow::Unload();
	CString::Unload();

	// �v���C���[�N���X�̔j��
	if (NULL != m_pPlayer)
	{
		// �I������
		m_pPlayer = NULL;
	}

	// �|�[�Y�N���X�̔j��
	if (NULL != m_pause)
	{
		// �I������
		m_pause->Uninit();
		delete m_pause;
		m_pause = NULL;
	}

	// �|�[�Y�N���X�̔j��
	if (NULL != m_apPolygon)
	{
		// �I������
		m_apPolygon->Uninit();
		delete m_apPolygon;
		m_apPolygon = NULL;
	}

	if (m_pString != NULL)
	{// �����̔j��
		m_pString->Uninit();
		delete m_pString;
		m_pString = NULL;
	}

	// �t�F�[�h�ȊO�̃I�u�W�F�N�g��j��
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			CScene *pScene = CScene::GetScene(nCntPriority, nCnt);

			if (pScene != NULL)
			{
				CScene::OBJTYPE objType = pScene->GetObjType();		// �I�u�W�F�N�g�^�C�v�̎擾
				if (objType != CScene::OBJTYPE_FADE)
				{// �t�F�[�h�ȊO�̃I�u�W�F�N�g��j��
					pScene->Uninit();
				}
			}
		}
	}
}

//=============================================================================
// �Q�[���X�V����
//=============================================================================
void CGame::Update(void)
{
	// �t�B�[���h�̎�������
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (NULL != m_pField[nCntField])
		{
			CCamera *pCamera = CManager::GetCamera();				// �J�����̏����擾
			D3DXVECTOR3 posCamera = pCamera->GetPosV();				// �J�����̈ʒu���擾

			D3DXVECTOR3 pos = m_pField[nCntField]->GetPos();		// �t�B�[���h�̈ʒu���擾
			D3DXVECTOR3 length = m_pField[nCntField]->GetLength();	// �t�B�[���h�̒������擾

			if (pos.x + length.x < posCamera.x - 5000)
			{// �t�B�[���h�̐���
				m_pField[nCntField]->Uninit();
				m_pField[nCntField] = NULL;
			}
		}
		else
		{
			m_pField[nCntField] = CField::Create(CField::TYPE_GROUND);
		}
	}

	// �G�̎�������
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		CEnemyInfo *pEnemyInfo = CEnemyInfo::GetInfo(nCntEnemy);
		int nNowTime = CTimer::GetTime();				// ���݂̎���

		if (pEnemyInfo != NULL)
		{// ��łȂ����ǂ����̊m�F
			bool bUse = pEnemyInfo->GetUse();

			if (!bUse)
			{// �܂��g�p����Ă��Ȃ��f�[�^
				int nAppearTime = pEnemyInfo->GetAppearTime();	// �G�̏o�����鎞��

				if (nNowTime == nAppearTime)
				{// �G�𐶐�
					pEnemyInfo->SetUse(true);
					CEnemy::Create(pEnemyInfo->GetPos(), pEnemyInfo->GeType());
				}
			}
		}
	}

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// �W���C�p�b�h�̎擾
	CSound *pSound = CManager::GetSound();

	CFade::FADE fade = CFade::GetFade();

	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���̓|�[�Y�ɂł��Ȃ�����
		if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_7) || pKeyboard->GetTrigger(DIK_P))
		{// �|�[�YON/OFF
			m_bPause = m_bPause ? false : true;

			if (true == m_bPause)
			{// �|�[�Y�ɂȂ����Ƃ�
				if (m_pause == NULL)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_PAUSE);
					m_pause = CPause::Create();	// �|�[�Y�N���X�𐶐�
				}
			}
		}
	}

	if (m_bPause == false)
	{// �|�[�Y�łȂ��Ƃ�
		if (m_pause != NULL)
		{// �|�[�Y�łȂ��Ȃ�����j��
			pSound->PlaySound(CSound::SOUND_LABEL_CANCEL);
			m_pause->Uninit();
			delete m_pause;
			m_pause = NULL;
		}

		CCamera *pCamera = CManager::GetCamera();				// �J�����̏����擾
		D3DXVECTOR3 posCamera = pCamera->GetPosV();				// �J�����̈ʒu���擾

		// �G���o��������
		if (posCamera.x >= m_nCntEnemyAppear * 600)
		{// �G�̐���
			m_nCntEnemyAppear++;
			CEnemy::Create(D3DXVECTOR3(600.0f * m_nCntEnemyAppear, 200.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_AIRCRAFT);

			for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
			{// �t�B�[���h�𒲂ׂ�
				if (m_pField[nCntField] != NULL)
				{
					if (m_pField[nCntField]->Collision(&D3DXVECTOR3(m_nCntEnemyAppear * 600.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_SEA == m_pField[nCntField]->GetType())
					{// ���
						CEnemy::Create(D3DXVECTOR3(800.0f * m_nCntEnemyAppear + 200.0f, 0.0f, -400.0f + (rand() % 800)), CEnemy::TYPE_SHIP);
					}
					else if (m_pField[nCntField]->Collision(&D3DXVECTOR3(m_nCntEnemyAppear * 600.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_GROUND == m_pField[nCntField]->GetType())
					{// ���
						CEnemy::Create(D3DXVECTOR3(800.0f * m_nCntEnemyAppear + 200.0f, 0.0f, -400.0f + (rand() % 800)), CEnemy::TYPE_TANK);
					}
				}
			}
		}

		// �����_���[�N���X�̍X�V
		CRenderer *pRenderer = CManager::GetRenderer();
		pRenderer->Update();
	}
	else
	{// �|�[�Y��
		m_pause->Update();
	}

	switch (m_state)
	{
	case STATE_NORMAL:
		break;

	case STATE_END:
		m_stateCounter++;
		if (m_stateCounter >= 60)
		{
			m_state = STATE_NONE;

			// ���(���[�h)�̐ݒ�
			CFade::Create(CManager::MODE_GAMEOVER);
		}
		break;

	case STATE_CLEAR:
		m_stateCounter++;
		if (m_stateCounter >= 60)
		{
			m_state = STATE_NONE;

			// ���(���[�h)�̐ݒ�
			CFade::Create(CManager::MODE_RESULT);
		}
		break;
	}
}

//=============================================================================
// �Q�[���`�揈��
//=============================================================================
void CGame::Draw(void)
{
	if (m_apPolygon != NULL)
	{// �|���S���̕`��
		m_apPolygon->Draw();
	}

	if (m_pString != NULL)
	{// �����̕`��
		m_pString->Set("[�w]�F�U��\n[�x]�F����e\n[��]�F�퓬�@\n[��]�F���\n[��]�F���", RECT{ 10,10,0,0 }, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), -1);
	}

	if (NULL != m_pause)
	{// �|�[�Y��ʂ̕`��
		m_pause->Draw();
	}
}

//=============================================================================
// �Q�[���̏�Ԃ̐ݒ�
//=============================================================================
const void CGame::SetGameState(State state)
{
	m_state = state;
	m_stateCounter = 0;
}

//=========================================
// �v���C���[�Ƀk��������
//=========================================
void CGame::NullPlayer(void)
{
	if (NULL != m_pPlayer)
	{
		m_pPlayer = NULL;
	}
}

//=========================================
// �v���C���[�̐ݒ�
//=========================================
const void CGame::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

//=========================================
// �R���X�g���N�^
//=========================================
CEnemyInfo::CEnemyInfo()
{
	m_nAppearTime = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = CEnemy::TYPE_AIRCRAFT;
	m_bUse = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CEnemyInfo::~CEnemyInfo()
{
}

//=========================================
// �G�̏���ǂݍ���
//=========================================
HRESULT CEnemyInfo::LoadEnemy(void)
{
	FILE *pFile;
	int nNumEnemy = 0;

	pFile = fopen(ENEMYINFO_FILENAME, "r");

	if (NULL != pFile)
	{
		while (1)
		{
			if (nNumEnemy >= MAX_ENEMY)
			{// �G�̍ő吔
				break;
			}
			else
			{
				char aStr[256];				// ��������i�[
				fgets(aStr, 256, pFile);	// ������̎擾

				if (strcmp(aStr, "SETENEMY\n") == 0)
				{// �G�̏���ǂݎ��
					int nType;	// �G�̎��

					fscanf(pFile, "%f %f %f", &m_aEnemyInfo[nNumEnemy].m_pos.x, &m_aEnemyInfo[nNumEnemy].m_pos.y, &m_aEnemyInfo[nNumEnemy].m_pos.z);
					fscanf(pFile, "%d", &m_aEnemyInfo[nNumEnemy].m_nAppearTime);
					fscanf(pFile, "%d", &nType);

					if (nType == 0)
					{// �퓬�@�^
						m_aEnemyInfo[nNumEnemy].m_type = CEnemy::TYPE_AIRCRAFT;
					}
					else if (nType == 1)
					{// ��͌^
						m_aEnemyInfo[nNumEnemy].m_type = CEnemy::TYPE_SHIP;
					}
					else if (nType == 2)
					{// ��Ԍ^
						m_aEnemyInfo[nNumEnemy].m_type = CEnemy::TYPE_TANK;
					}

					m_aEnemyInfo[nNumEnemy].m_bUse = false;
					nNumEnemy++;
				}
				else if (strcmp(aStr, "END") == 0)
				{
					break;
				}
			}
		}

		fclose(pFile);		// �t�@�C�������
	}
	else
	{// ���s
		MessageBox(0, "�t�@�C�����J���܂���ł���", "particle.txt", MB_OK);
	}

	return S_OK;
}

