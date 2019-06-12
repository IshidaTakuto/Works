//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "title.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "polygon.h"
#include "meshCylinder.h"
#include "camera.h"
#include "enemy.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_TIME	(1200)		// �����L���O��ʂ܂ł̎���

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CPolygon *CTitle::m_apPolygon[TITLE_POLYGON] = {};	// �|���S���N���X�̃|�C���^�ϐ�
CField *CTitle::m_pField[MAX_FIELD] = {};

//=========================================
// �R���X�g���N�^
//=========================================
CTitle::CTitle()
{
	m_nCntEnemyAppear = 0;
	m_nCntTitle = 0;
	m_nCntEnemyAppear = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CTitle::~CTitle()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	CPolygon::Load();	// �|���S���̃e�N�X�`�����擾
	CCylinder::Load();
	CField::Load();
	CEnemy::Load();
	CShadow::Load();

	CCylinder::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), 8000, 7000, 8000, 20, 1, 0, 0);

	m_pField[0] = CField::Create(CField::TYPE_GROUND);
	m_pField[1] = CField::Create(CField::TYPE_GROUND);
	m_pField[2] = CField::Create(CField::TYPE_SEA);

	m_apPolygon[0] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0),300,150, CPolygon::TYPE_TITLE_LOGO);
	m_apPolygon[1] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 600, 0), 250, 50, CPolygon::TYPE_PRESS_ANY);

	// �����ɂ���
	m_apPolygon[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apPolygon[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	m_nCntEnemyAppear = 1;

	CCamera *pCamera = CManager::GetCamera();			// �J�����̏����擾
	pCamera->SetPosV(D3DXVECTOR3(700, 0.0f, 0.0f));		// �J�����̈ʒu�̐ݒ�

	// �G�𐶐�
	CEnemy::Create(D3DXVECTOR3(1450.0f, 0.0f, -50.0f), CEnemy::TYPE_TANK);
	CEnemy::Create(D3DXVECTOR3(1500.0f, 0.0f, 50.0f), CEnemy::TYPE_TANK);
	CEnemy::Create(D3DXVECTOR3(1400.0f, 0.0f, 100.0f), CEnemy::TYPE_TANK);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	CPolygon::Unload();	// �|���S���̃e�N�X�`����j��
	CCylinder::Unload();
	CField::Unload();
	CEnemy::Unload();
	CShadow::Unload();

	// �t�B�[���h�N���X�̔j��
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (NULL != m_pField[nCntField])
		{
			m_pField[nCntField] = NULL;
		}
	}

	// �|���S���N���X�̔j��
	for (int nCntPolygon = 0; nCntPolygon < TITLE_POLYGON; nCntPolygon++)
	{
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			m_apPolygon[nCntPolygon]->Uninit();
			delete m_apPolygon[nCntPolygon];
			m_apPolygon[nCntPolygon] = NULL;
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
void CTitle::Update(void)
{
	// �^�C�g���̌o�ߎ���
	m_nCntTitle++;

	CCamera *pCamera = CManager::GetCamera();				// �J�����̏����擾
	D3DXVECTOR3 posCamera = pCamera->GetPosV();				// �J�����̈ʒu���擾
	D3DXVECTOR3 posRCamera = pCamera->GetPosR();			// �J�����̒����_

	// �t�B�[���h�̎�������
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (NULL != m_pField[nCntField])
		{
			D3DXVECTOR3 pos = m_pField[nCntField]->GetPos();		// �t�B�[���h�̈ʒu���擾
			D3DXVECTOR3 length = m_pField[nCntField]->GetLength();	// �t�B�[���h�̒������擾

			if (pos.x + length.x < posCamera.x - 500)
			{// �t�B�[���h�̊J��
				if (NULL != m_pField[nCntField])
				{
					m_pField[nCntField]->Uninit();
					m_pField[nCntField] = NULL;
				}
			}
		}
		else
		{
			// �t�B�[���h�������_������
			if (rand() % 2 == 0)
			{// �n��
				m_pField[nCntField] = CField::Create(CField::TYPE_GROUND);
			}
			else
			{// �C
				m_pField[nCntField] = CField::Create(CField::TYPE_SEA);
			}
		}
	}

	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	D3DXCOLOR col = m_apPolygon[0]->GetColor();			// �^�C�g�����S�̐F���擾

	// �J�����̒����_�����܂ł����ƃ��S���o��
	if (posRCamera.y >= posCamera.y)
	{
		if (col.a >= 1.0f)
		{// �����x�����ȏ�
			col.a = 1.0f;
		}
		else if (col.a < 1.0f)
		{// �����x�����Z
			col.a += 0.02f;
		}
	}

	if (col.a >= 1.0f)
	{// �����łȂ����
		// ���͑҂��e�N�X�`����_��
		m_apPolygon[1]->Flashing(60);

		if (pKeyboard->GetAnyKey() || pJoypad->GetAnyTrigger(0))
		{// ��ʑJ��
			CFade::Create(CManager::MODE_TUTORIAL);
		}
		else if (m_nCntTitle >= TITLE_TIME)
		{// ��莞�Ԍo��
			CFade::Create(CManager::MODE_RANKING);
		}
	}
	if (pKeyboard->GetAnyKey() || pJoypad->GetAnyTrigger(0))
	{// �{�^���������Ƃ����ɕ\�������
		col.a = 1.0f;
	}

	// �F�̐ݒ�
	m_apPolygon[0]->SetColor(col);

	// �G���o��������
	if (posCamera.x >= m_nCntEnemyAppear * 50)
	{// �G�̐���
		if (CEnemy::GetNumEnemy() < 40)
		{// ��萔��ۂ�
			m_nCntEnemyAppear++;
			CEnemy::Create(D3DXVECTOR3(rand() % 400 + (300.0f * m_nCntEnemyAppear), rand() % 400 + 100.0f, -1000.0f + (rand() % 2000)), CEnemy::TYPE_AIRCRAFT);

			for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
			{// �t�B�[���h�𒲂ׂ�
				if (m_pField[nCntField] != NULL)
				{
					if (m_pField[nCntField]->Collision(&D3DXVECTOR3(posCamera.x + 3000.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_SEA == m_pField[nCntField]->GetType())
					{// ���
						CEnemy::Create(D3DXVECTOR3(rand() % 1000 + (2500.0f + posCamera.x), 0.0f, -1000.0f + (rand() % 2000)), CEnemy::TYPE_SHIP);
					}
					else if (m_pField[nCntField]->Collision(&D3DXVECTOR3(posCamera.x + 3000.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_GROUND == m_pField[nCntField]->GetType())
					{// ���
						CEnemy::Create(D3DXVECTOR3(rand() % 1000 + (2500.0f + posCamera.x), 0.0f, -2000.0f + (rand() % 4000)), CEnemy::TYPE_TANK);
					}
				}
			}
		}
	}

	CRenderer *pRenderer = CManager::GetRenderer();
	pRenderer->Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < TITLE_POLYGON; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon]->Draw();
	}
}
