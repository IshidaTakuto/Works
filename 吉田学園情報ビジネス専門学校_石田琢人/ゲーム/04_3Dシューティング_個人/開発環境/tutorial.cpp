//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "tutorial.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "field.h"
#include "model.h"
#include "polygon.h"
#include "effect.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "string.h"
#include "gauge.h"
#include "stock.h"
#include "camera.h"
#include "number.h"
#include "particle.h"
#include "meshCylinder.h"
#include "communicateWindow.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_STRING	(6)			// ������̐�
#define TIME_STRING	(6 * 60)	// �\�����鎞��
#define TUTORIAL_ENEMY_APPEAR	(6 * 60)	// �G�̏o���Ԋu
#define TUTORIAL_FIELD_APPEAR	(9 * 60)	// �G�̏o���Ԋu

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CField *CTutorial::m_pField[MAX_FIELD] = {};

//=========================================
// �R���X�g���N�^
//=========================================
CTutorial::CTutorial()
{
	m_nCntString = 0;
	m_nCntEnemyAppear = 0;
	m_nCntTutorialTime = 0;
	m_pCommunicateWindow = NULL;
	m_pString[0] = NULL;
	m_pString[1] = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CTutorial::~CTutorial()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorial::Init(void)
{
	CBullet::Load();
	CEnemy::Load();
	CPlayer::Load();
	CEffect::Load();
	CField::Load();
	CModel::Load();
	CPolygon::Load();
	CString::Load();
	CNumber::Load();
	CParData::Load();
	CParticlePolygon::Load();
	CCylinder::Load();
	CShadow::Load();

	m_pString[0] = CString::Create();
	m_pString[1] = CString::Create();

	CCylinder::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), 8000, 7000, 8000, 20, 1, 0, 0);

	m_pField[0] = CField::Create(CField::TYPE_GROUND);
	m_pField[1] = CField::Create(CField::TYPE_SEA);
	m_pField[2] = CField::Create(CField::TYPE_GROUND);

	// �I�u�W�F�N�g�N���X�̐���
	CPlayer::Create(D3DXVECTOR3(0, 200, 0), CPlayer::TYPE_AIRCRAFT);

	CStock::Create();
	CGauge::Create();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	// �e�N�X�`���̔j��
	CBullet::Unload();
	CPlayer::Unload();
	CEnemy::Unload();
	CField::Unload();
	CModel::Unload();
	CPolygon::Unload();
	CString::Unload();
	CNumber::Unload();
	CParData::Unload();
	CParticlePolygon::Unload();
	CCylinder::Unload();
	CShadow::Unload();

	if (NULL != m_pCommunicateWindow)
	{// �����̕\���g�̔j��
		m_pCommunicateWindow = NULL;
	}

	for (int nCntString = 0; nCntString < 2; nCntString++)
	{
		if (m_pString[nCntString] != NULL)
		{// �����̔j��
			m_pString[nCntString]->Uninit();
			delete m_pString[nCntString];
			m_pString[nCntString] = NULL;
		}
	}

	// �t�B�[���h�N���X�̔j��
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (NULL != m_pField[nCntField])
		{
			m_pField[nCntField] = NULL;
		}
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
// �X�V����
//=============================================================================
void CTutorial::Update(void)
{
	m_nCntTutorialTime++;

	CCamera *pCamera = CManager::GetCamera();				// �J�����̏����擾
	D3DXVECTOR3 posCamera = pCamera->GetPosV();				// �J�����̈ʒu���擾

	// �t�B�[���h�̎�������
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (NULL != m_pField[nCntField])
		{
			D3DXVECTOR3 pos = m_pField[nCntField]->GetPos();		// �t�B�[���h�̈ʒu���擾
			D3DXVECTOR3 length = m_pField[nCntField]->GetLength();	// �t�B�[���h�̒������擾

			if (pos.x + length.x < posCamera.x - 5000)
			{// �t�B�[���h�̊J��
				m_pField[nCntField]->Uninit();
				m_pField[nCntField] = NULL;
			}
		}
		else
		{// �t�B�[���h�𐶐�����
			if (rand() % 2 == 0)
			{//	�n��
				m_pField[nCntField] = CField::Create(CField::TYPE_GROUND);

				D3DXVECTOR3 posField = m_pField[nCntField]->GetPos();		// �t�B�[���h�̈ʒu�̎擾
				D3DXVECTOR3 length = m_pField[nCntField]->GetLength();		// �t�B�[���h�̒������擾
				CEnemy::Create(D3DXVECTOR3(posField.x, 0.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_TANK);
				CEnemy::Create(D3DXVECTOR3(posField.x + (length.x / 2), 0.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_TANK);
			}
			else
			{// �C
				m_pField[nCntField] = CField::Create(CField::TYPE_SEA);

				D3DXVECTOR3 posField = m_pField[nCntField]->GetPos();		// �t�B�[���h�̈ʒu�̎擾
				D3DXVECTOR3 length = m_pField[nCntField]->GetLength();		// �t�B�[���h�̒������擾
				CEnemy::Create(D3DXVECTOR3(posField.x, 0.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_TANK);
				CEnemy::Create(D3DXVECTOR3(posField.x + (length.x / 2), 0.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_TANK);
			}
		}
	}

	// �G���o��������
	if (posCamera.x >= m_nCntEnemyAppear * 800)
	{// �G�̐���
		m_nCntEnemyAppear++;
		CEnemy::Create(D3DXVECTOR3(800.0f * m_nCntEnemyAppear, 200.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_AIRCRAFT);

		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{// �t�B�[���h�𒲂ׂ�
			if (m_pField[nCntField] != NULL)
			{
				if (m_pField[nCntField]->Collision(&D3DXVECTOR3(m_nCntEnemyAppear * 800.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_SEA == m_pField[nCntField]->GetType())
				{// ���
					CEnemy::Create(D3DXVECTOR3(800.0f * m_nCntEnemyAppear + 200.0f, 0.0f, -400.0f + (rand() % 800)), CEnemy::TYPE_SHIP);
				}
				else if (m_pField[nCntField]->Collision(&D3DXVECTOR3(m_nCntEnemyAppear * 800.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_GROUND == m_pField[nCntField]->GetType())
				{// ���
					CEnemy::Create(D3DXVECTOR3(800.0f * m_nCntEnemyAppear + 200.0f, 0.0f, -400.0f + (rand() % 800)), CEnemy::TYPE_TANK);
				}
			}
		}
	}


	// �ʐM�E�B���h�E���o��
	if (m_nCntTutorialTime == 60)
	{
		if (NULL == m_pCommunicateWindow)
		{
			m_pCommunicateWindow = CCommunicateWindow::Create();
		}
	}

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// �W���C�p�b�h�̎擾

	if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_7) || pKeyboard->GetTrigger(DIK_P))
	{// ��ʑJ��
		CFade::Create(CManager::MODE_GAME);
	}

	// �I�u�W�F�N�g�̍X�V
	CRenderer *pRenderer = CManager::GetRenderer();
	pRenderer->Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{
	if (NULL != m_pCommunicateWindow)
	{
		if (m_pCommunicateWindow->GetCompleted())
		{// �ʐM�E�B���h�E�������ł���
			bool bComp = false;
			CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
			CInputJoypad *pJoypad = CManager::GetInputJoypad();			// �W���C�p�b�h�̎擾

			m_pString[0]->Set("[�r�s�`�q�s]�Ń`���[�g���A�����X�L�b�v\n[�a�{�^��]����", RECT{ 940,70,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1,1);

			if (m_nCntString == 0)
			{// �J�n���b�Z�[�W
				bComp = m_pString[1]->Set("������@�̐��������܂��B", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 1)
			{// �ړ����@�̕����\��
				bComp = m_pString[1]->Set("�m���X�e�B�b�N�n�ňړ����ł��܂��B", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 2)
			{// �U�����@�̕����\��
				bComp = m_pString[1]->Set("�m�w�{�^���n�ōU�����ł��܂��B", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 3)
			{// �n�`�̕����\��
				bComp = m_pString[1]->Set("�n��Ő�͂ɕό`�܂��� \n����Ő�ԂɂȂ�ƃ_���[�W���󂯑����܂��B", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 4)
			{// �ό`���@�̕����\��
				bComp = m_pString[1]->Set("�\���L�[�ŕό`���ł��܂��B�ł����A�ēx�ό`�ł���悤�ɂȂ�܂Ő��b������܂��B \n���F�퓬�@�@���F��ԁ@���F���", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 5)
			{// ����U�����@�̕����\��
				bComp = m_pString[1]->Set("�m�x�{�^���n�ł��ꂼ��̃t�H�[���̋��͂ȍU�����ł��܂��B \n�������A�Q�[�W�����܂��Ă����ԂłȂ��Ǝg���܂���B", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 6)
			{
				bComp = m_pString[1]->Set("�m�r�s�`�q�s�n�Ń`���[�g���A�����I�����܂��B \n�m�`�{�^���n�ł�����x�ŏ�����������܂��B", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 3);

				if (bComp)
				{// ���͂��S���\������Ă���
					if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_0) || pKeyboard->GetTrigger(DIK_RETURN))
					{// ���͂��ŏ��ɖ߂�
						m_nCntString = 0;
					}
				}
			}

			if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_1) || pKeyboard->GetTrigger(DIK_RETURN))
			{// ���͂𑗂�
				if (m_nCntString < 6 && bComp)
				{
					m_nCntString++;
				}
			}
		}
	}
}
