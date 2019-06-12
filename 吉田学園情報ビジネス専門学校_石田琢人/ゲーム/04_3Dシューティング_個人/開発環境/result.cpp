//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "result.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "polygon.h"
#include "number.h"
#include "ranking.h"
#include "meshCylinder.h"
#include "scene3DModel.h"
#include "particle.h"
#include "field.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_TIME			(420)			// �^�C�g����ʂ܂ł̎���
#define RESULT_NUMBER_SIZE	(40.0f)			// �����̑傫��

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CResult::m_nScore = 0;		// �X�R�A
int CResult::m_nNumKill = 0;	// �|�����G�̐�

//=========================================
// �R���X�g���N�^
//=========================================
CResult::CResult()
{
	// �i���o�[��������
	for (int nCntNumber = 0; nCntNumber < RESULT_SCORE_DIGITS; nCntNumber++)
	{// �X�R�A
		m_apNumScore[nCntNumber] = NULL;
	}
	for (int nCntNumber = 0; nCntNumber < RESULT_KILL_DIGITS; nCntNumber++)
	{// �|�����G�̐�
		m_apNumKill[nCntNumber] = NULL;
	}

	// �|���S����������
	for (int nCntPolygon = 0; nCntPolygon < RESULT_POLYGON; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon] = NULL;
	}

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{// �t�B�[���h��������
		m_pField[nCntField] = NULL;
	}

	m_model = NULL;
	m_nCntResult = 0;
	m_nScoreDigit = 0;
	m_nKillDigit = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CResult::~CResult()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	// �e�N�X�`�����擾
	CPolygon::Load();
	CNumber::Load();
	CParData::Load();
	CParticlePolygon::Load();
	CField::Load();
	CCylinder::Load();

	// ���S
	m_apPolygon[0] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 150, 0), 400, 100);
	m_apPolygon[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ���͑҂�
	m_apPolygon[1] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 100, 0), 250, 50, CPolygon::TYPE_PRESS_ANY);

	m_nScoreDigit = (int)log10f((float)m_nScore) + 1;		// �X�R�A�̌���
	if (m_nScoreDigit == 0) { m_nScoreDigit = 1; }			// 0�������Ƃ�
	for (int nCntDigit = 0; nCntDigit < m_nScoreDigit; nCntDigit++)
	{// �X�R�A
		m_apNumScore[nCntDigit] = CNumber::Create(D3DXVECTOR3((700 + (RESULT_NUMBER_SIZE * m_nScoreDigit) - RESULT_NUMBER_SIZE) - (nCntDigit * RESULT_NUMBER_SIZE * 2), 400, 0.0f), RESULT_NUMBER_SIZE);
	}

	m_nKillDigit = (int)log10f((float)m_nNumKill) + 1;		// �|�����G�̐��̌���
	if (m_nKillDigit == 0) { m_nKillDigit = 1; }			// 0�������Ƃ�
	for (int nCntDigit = 0; nCntDigit < m_nKillDigit; nCntDigit++)
	{// �|�����G�̐�
		m_apNumKill[nCntDigit] = CNumber::Create(D3DXVECTOR3((700 + (RESULT_NUMBER_SIZE * m_nKillDigit) - RESULT_NUMBER_SIZE) - (nCntDigit * RESULT_NUMBER_SIZE * 2), 500, 0.0f), RESULT_NUMBER_SIZE);
	}

	// �V�����_�[�̐���
	CCylinder::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), 8000, 7000, 8000, 20, 1, 0, 0);

	// �t�B�[���h�̐���
	CField::Create(CField::TYPE_GROUND);

	// ���f�����o��
	m_model = new CScene3DModel;

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	LPDIRECT3DTEXTURE9	pTexture;		// ���L�e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			pMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		pBuffMat;		// �}�e���A�����ւ̃|�C���^
	DWORD				nNumMat;		// �}�e���A�����̐�

										// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/aircraft.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pBuffMat,
		NULL,
		&nNumMat,
		&pMesh);

	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// �e�N�X�`���̐���
	pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &pTexture);

	// ���f������n��
	m_model->BindInfo(pTexture, pMesh, pBuffMat, nNumMat);

	// ���݂̃��[�h���擾
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_RESULT)
	{// �Q�[���N���A
		m_apPolygon[0]->TextureChange(CPolygon::TYPE_RESULT_LOGO);

		// ���f���̈ʒu�Ɗp�x
		m_model->SetPos(D3DXVECTOR3(0.0f, 500.0f, 0.0f));
		m_model->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		CCamera *pCamera = CManager::GetCamera();			// �J�����̏����擾
		pCamera->SetPosV(D3DXVECTOR3(0.0f, 500.0f, -100.0f));		// �J�����̈ʒu�̐ݒ�
	}
	else if (mode == CManager::MODE_GAMEOVER)
	{// �Q�[���I�[�o�[
		m_apPolygon[0]->TextureChange(CPolygon::TYPE_GAMEOVER_LOGO);

		// ���f���̈ʒu�Ɗp�x
		m_model->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_model->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.25f, D3DX_PI * 0.25f));

		// �t�B�[���h�̐���
		m_pField[0]->Create(CField::TYPE_GROUND);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	CPolygon::Unload();	// �|���S���̃e�N�X�`����j��
	CNumber::Unload();	// �i���o�[�̃e�N�X�`����j��
	CParData::Unload();
	CParticlePolygon::Unload();
	CField::Unload();
	CCylinder::Unload();

	// �ÓI�ϐ��������l�ɂ���
	m_nNumKill = 0;
	m_nScore = 0;

	// �|���S���N���X�̔j��
	for (int nCntPolygon = 0; nCntPolygon < RESULT_POLYGON; nCntPolygon++)
	{
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			m_apPolygon[nCntPolygon]->Uninit();
			delete m_apPolygon[nCntPolygon];
			m_apPolygon[nCntPolygon] = NULL;
		}
	}

	// �i���o�[�̊J��
	for (int nCntNumber = 0; nCntNumber < RESULT_SCORE_DIGITS; nCntNumber++)
	{// �X�R�A
		if (m_apNumScore[nCntNumber] != NULL)
		{// �i���o�[�̏I������
			m_apNumScore[nCntNumber]->Uninit();
			delete m_apNumScore[nCntNumber];
			m_apNumScore[nCntNumber] = NULL;
		}
	}
	for (int nCntNumber = 0; nCntNumber < RESULT_KILL_DIGITS; nCntNumber++)
	{// �|�����G�̐�
		if (m_apNumKill[nCntNumber] != NULL)
		{// �i���o�[�̏I������
			m_apNumKill[nCntNumber]->Uninit();
			delete m_apNumKill[nCntNumber];
			m_apNumKill[nCntNumber] = NULL;
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
void CResult::Update(void)
{
	// ���U���g�̌o�ߎ��Ԃ��J�E���g
	m_nCntResult++;

	// �_��
	m_apPolygon[1]->Flashing(60);

	int nNumber;

	for (int nCntDigits = 0; nCntDigits < m_nKillDigit; nCntDigits++)
	{// �|�����G�̐�
		nNumber = m_nNumKill % (int)powf(10.0f, (nCntDigits + 1) * 1.0f) / (int)powf(10.0f, nCntDigits * 1.0f);

		m_apNumKill[nCntDigits]->SetNumber(nNumber);
	}
	for (int nCntDigits = 0; nCntDigits < m_nScoreDigit; nCntDigits++)
	{// �X�R�A
		nNumber = m_nScore % (int)powf(10.0f, (nCntDigits + 1) * 1.0f) / (int)powf(10.0f, nCntDigits * 1.0f);

		m_apNumScore[nCntDigits]->SetNumber(nNumber);
	}

	// ���݂̃��[�h���擾
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_RESULT)
	{// �Q�[���N���A
		if (m_model != NULL)
		{
			D3DXVECTOR3 pos = m_model->GetPos();					// �ʒu�̎擾
			pos.x += 2;												// ���f���𓮂���
			m_model->SetPos(pos);									// �ʒu�̐ݒ�
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, pos.y, pos.z), 2);	// �ړ��̃G�t�F�N�g
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
	}
	else if (mode == CManager::MODE_GAMEOVER)
	{// �Q�[���I�[�o�[
		if (m_nCntResult % 3 == 0)
		{// ��莞�Ԍo�߂��ƂɃ��f���̈ʒu���獕�����o��
			CParticle::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
		}
	}

	// ���͂̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	CRenderer *pRenderer = CManager::GetRenderer();
	pRenderer->Update();

	if (pKeyboard->GetAnyKey() || pJoypad->GetAnyTrigger(0))
	{// ��ʑJ��
		CFade::Create(CManager::MODE_RANKING);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{
	// �|���S���̕`��
	for (int nCntPolygon = 0; nCntPolygon < RESULT_POLYGON; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon]->Draw();
	}

	// �i���o�[�̕`��
	//for (int nCntNumber = 0; nCntNumber < m_nScoreDigit; nCntNumber++)
	//{// �X�R�A
	//	m_apNumScore[nCntNumber]->Draw();
	//}
	//for (int nCntNumber = 0; nCntNumber < m_nKillDigit; nCntNumber++)
	//{// �|�����G�̐�
	//	m_apNumKill[nCntNumber]->Draw();
	//}
}
