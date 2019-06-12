//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "ranking.h"
#include "fade.h"
#include "input.h"
#include "polygon.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "meshCylinder.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKING_TIME			(420)			// �^�C�g����ʂ܂ł̎���
#define RANKINGNUM_SIZE			(40.0f)			// �����̑傫��

//==================================
// �ÓI�����o�ϐ��錾
//==================================
int CRanking::m_nRanking[MAX_RANKING] = { 15000, 13000, 10000, 5000, 1000 };
int CRanking::m_nScore = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �i���o�[��������
		for (int nCntNumber = 0; nCntNumber < RANKING_DIGITS; nCntNumber++)
		{
			m_apNumber[nCntRanking][nCntNumber] = NULL;
			m_aDestPos[nCntRanking][nCntNumber] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		m_bArrive[nCntRanking] = false;
	}

	for (int nCntPolygon = 0; nCntPolygon < NUM_POLYGON; nCntPolygon++)
	{// �|���S����������
		m_apPolygon[nCntPolygon] = NULL;
	}

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{// �t�B�[���h��������
		m_pField[nCntField] = NULL;
	}

	m_nCntFade = 0;
	m_nCntHighScore = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRanking::Init(void)
{
	// �e�N�X�`���̓ǂݍ���
	CNumber::Load();
	CPolygon::Load();
	CField::Load();
	CCylinder::Load();

	// �����L���O�̃��S
	m_apPolygon[0] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 60, 0.0f), 200, 160, CPolygon::TYPE::TYPE_RANKING_LOGO);

	// �����L���O�̃X�R�A(����)
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐�
		for (int nCntDigit = 0; nCntDigit < RANKING_DIGITS; nCntDigit++)
		{// �����L���O�X�R�A�̌���
			m_apNumber[nCntRanking][nCntDigit] = CNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 2.5f + (RANKINGNUM_SIZE * RANKING_DIGITS) - RANKINGNUM_SIZE) - (nCntDigit * (RANKINGNUM_SIZE * 2)), (125.0f + (RANKINGNUM_SIZE * 2)) + (nCntRanking * RANKINGNUM_SIZE * (3.3f - (nCntRanking * 0.1f))), 0.0f), RANKINGNUM_SIZE - (nCntRanking * 4));

			// �ړI�̈ʒu
			m_aDestPos[nCntRanking][nCntDigit] = D3DXVECTOR3((SCREEN_WIDTH / 2 + (RANKINGNUM_SIZE * RANKING_DIGITS) - RANKINGNUM_SIZE) - (nCntDigit * (RANKINGNUM_SIZE * 2)), (125.0f + (RANKINGNUM_SIZE * 2)) + (nCntRanking * RANKINGNUM_SIZE * (3.3f - (nCntRanking * 0.1f))), 0.0f);
		}
	}

	CCamera *pCamera = CManager::GetCamera();			// �J�����̏����擾
	pCamera->SetPosV(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �J�����̈ʒu�̐ݒ�

	// �V�����_�[�̐���
	CCylinder::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), 8000, 7000, 8000, 20, 1, 0, 0);

	// �t�B�[���h�̐���
	m_pField[0] = CField::Create(CField::TYPE_SEA);
	m_pField[1] = CField::Create(CField::TYPE_SEA);
	m_pField[2] = CField::Create(CField::TYPE_GROUND);

	m_bArrive[MAX_RANKING - 1] = true;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRanking::Uninit(void)
{
	// �e�N�X�`���̔j��
	CNumber::Unload();
	CPolygon::Unload();
	CField::Unload();
	CCylinder::Unload();

	// �|���S���̊J��
	for (int nCntPolygon = 0; nCntPolygon < NUM_POLYGON; nCntPolygon++)
	{
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			m_apPolygon[nCntPolygon]->Uninit();	// �|���S���̏I������
			delete m_apPolygon[nCntPolygon];
			m_apPolygon[nCntPolygon] = NULL;
		}
	}

	// �i���o�[�̊J��
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntNumber = 0; nCntNumber < RANKING_DIGITS; nCntNumber++)
		{
			if (m_apNumber[nCntRanking][nCntNumber] != NULL)
			{
				m_apNumber[nCntRanking][nCntNumber]->Uninit();	// �i���o�[�̏I������
				delete m_apNumber[nCntRanking][nCntNumber];
				m_apNumber[nCntRanking][nCntNumber] = NULL;
			}
		}
	}

	// �t�B�[���h�N���X�̊J��
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
void CRanking::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	// �_�ŗp�J�E���g�A�b�v
	m_nCntHighScore++;
	m_nCntHighScore = m_nCntHighScore % 4;

	// �����̎擾�����X�R�A��_�ł�����
	if (m_nScore > -1)
	{// �X�R�A�������Ă���
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			if (m_nRanking[nCntRanking] == m_nScore)
			{// �����̃X�R�A��_�ł�����
				for (int nCntNumber = 0; nCntNumber < RANKING_DIGITS; nCntNumber++)
				{// �������܂킷
					if (m_nCntHighScore < 2)
					{
						m_apNumber[nCntRanking][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					}
					else if (m_nCntHighScore >= 2)
					{
						m_apNumber[nCntRanking][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
					}
				}
				break;	// �����X�R�A���������ꍇ�̕ی�
			}
		}
	}

	D3DXVECTOR3 deffPos;	// ����
	D3DXVECTOR3 pos;		// ����
	int nCntNumber;

	// �����L���O�X�R�A��ړI�̈ʒu�܂ňړ�
	for (int nCntRanking = MAX_RANKING - 1; nCntRanking >= 0; nCntRanking--)
	{
		if (m_bArrive[nCntRanking] == true)
		{// ��O�̃X�R�A���ړI�̈ʒu�ɂ�����
			for (nCntNumber = 0; nCntNumber < RANKING_DIGITS; nCntNumber++)
			{// �������܂킷
				pos = m_apNumber[nCntRanking][nCntNumber]->GetPos();

				if (pos.x >= m_aDestPos[nCntRanking][nCntNumber].x)
				{// ���̈ʒu�܂œ�����
					deffPos = m_aDestPos[nCntRanking][nCntNumber] - pos;	// �����̌v�Z

					pos.x += deffPos.x * 0.4f;
					pos.y += deffPos.y * 0.4f;
					pos.z += deffPos.z * 0.4f;

					m_apNumber[nCntRanking][nCntNumber]->SetPos(pos);
				}

				if (pos.x - 0.1f <= m_aDestPos[nCntRanking][nCntNumber].x)
				{// �X�R�A���ړI�̈ʒu�ɂ���
					if (nCntRanking > 0)
					{// ���̃X�R�A�𓮂�����悤�ɂ���
						m_bArrive[nCntRanking - 1] = true;
					}
				}
			}
		}
	}

	// �����L���O�X�R�A�̐ݒ�
	int aNumber;

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐���
		for (int nCntDigits = 0; nCntDigits < RANKING_DIGITS; nCntDigits++)
		{// ���̏�����������ݒ�
			aNumber = m_nRanking[nCntRanking] % (int)powf(10.0f, (nCntDigits + 1) * 1.0f) / (int)powf(10.0f, nCntDigits * 1.0f);
			m_apNumber[nCntRanking][nCntDigits]->SetNumber(aNumber);
		}
	}

	CCamera *pCamera = CManager::GetCamera();	// �J�����̏����擾
	D3DXVECTOR3 posCamera = pCamera->GetPosV();	// �J�����̈ʒu���擾

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
		{// �t�B�[���h�̐���
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

	// �����_���[�̍X�V���Ă�
	CRenderer *pRenderer = CManager::GetRenderer();
	pRenderer->Update();

	if (pKeyboard->GetAnyKey() || pJoypad->GetAnyTrigger(0))
	{
		//���[�h�I��
		CFade::Create(CManager::MODE_TITLE);
	}

	m_nCntFade++;
	if (m_nCntFade >= RANKING_TIME)
	{
		//���[�h�I��
		CFade::Create(CManager::MODE_TITLE);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRanking::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < NUM_POLYGON; nCntPolygon++)
	{// �|���S���̕`��
		m_apPolygon[nCntPolygon]->Draw();
	}

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �i���o�[�̕`��
		for (int nCntNumber = 0; nCntNumber < RANKING_DIGITS; nCntNumber++)
		{
			m_apNumber[nCntRanking][nCntNumber]->Draw();
		}
	}
}

//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void CRanking::Set(int nScore)
{
	int nSwap;							// ����ւ��p
	int aEmptyData[MAX_RANKING + 1];	// ����ւ��p

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{// ���܂ł̋L�^����̃f�[�^�ɓ����
		aEmptyData[nCntNum] = m_nRanking[nCntNum];
	}

	aEmptyData[MAX_RANKING] = nScore;	// �Ō�̔z��ɍŐV�̃X�R�A������

	// ����ւ�
	for (int nCntNum = 1; nCntNum < MAX_RANKING + 1; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aEmptyData[nCntNum1 - 1] <= aEmptyData[nCntNum1])
			{
				nSwap = aEmptyData[nCntNum1];
				aEmptyData[nCntNum1] = aEmptyData[nCntNum1 - 1];
				aEmptyData[nCntNum1 - 1] = nSwap;
			}
		}
	}

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{// ����ւ����I������f�[�^�������L���O�ɔ��f����
		m_nRanking[nCntNum] = aEmptyData[nCntNum];

	}

	m_nScore = nScore;	// �ŐV�̃X�R�A���X�V����
}
