//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debugProc.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "pause.h"
#include "particle.h"
#include "meshField.h"
#include "sound.h"

#include <stdio.h>

//=========================================
// �}�N����`
//=========================================
#define SYSTEM_FILE	"data/TEXT/system.ini"		// �n�ʂ̏��e�L�X�g

//=========================================
// �ÓI�����o�ϐ�
//=========================================
CRenderer			*CManager::m_pRenderer = NULL;			// �����_���[�N���X�̃|�C���^�ϐ�
CInputKeyboard		*CManager::m_pInputKeyboard = NULL;		// �C���v�b�g�N���X�̃|�C���^�ϐ�
CInputJoyStick		*CManager::m_pJoyStick = NULL;			// �W���C�X�e�B�b�N�N���X�^�̃|�C���^
CXInput				*CManager::m_pXInput = NULL;			// XInput�N���X�^�̃|�C���^
CCamera				*CManager::m_pCamera = NULL;			// �J�����N���X�̃|�C���^�ϐ�
CLight				*CManager::m_pLight = NULL;				// ���C�g�N���X�̃|�C���^�ϐ�
CDebugProc			*CManager::m_pDebugProc = NULL;			// �f�o�b�O�����N���X�̃|�C���^�ϐ�
CSound				*CManager::m_pSound = NULL;				// �T�E���h�N���X�̃|�C���^�ϐ�
CTitle				*CManager::m_pTitle = NULL;				// �^�C�g���N���X�̃|�C���^�ϐ�
CTutorial			*CManager::m_pTutorial = NULL;			// �`���[�g���A���N���X�̃|�C���^�ϐ�
CGame				*CManager::m_pGame = NULL;				// �Q�[���N���X�̃|�C���^�ϐ�
CResult				*CManager::m_pResult = NULL;			// ���U���g�N���X�̃|�C���^�ϐ�
CRanking			*CManager::m_pRanking = NULL;			// �����L���O�N���X�̃|�C���^�ϐ�
CManager::MODE		CManager::m_mode = CManager::MODE_TITLE;

int  CManager::m_nNumStage = 0;
char **CManager::m_pFieldFileName = NULL;
char **CManager::m_pSetFileName = NULL;

//=========================================
// �V�X�e���X�N���v�g�̓ǂݍ���
//=========================================
HRESULT CManager::Load(void)
{
	FILE *pFile;	// �t�@�C���̃|�C���^�ϐ�
	char aStr[128];	// �擾���镶����
	int nCntField = 0;
	int nCntSet = 0;

	// �t�@�C�����J��
	pFile = fopen(SYSTEM_FILE, "r");

	if (NULL != pFile)
	{// �t�@�C����������
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", aStr);

			if (strcmp(aStr, "NUM_STAGE") == 0)
			{// �ǂݍ��ރX�N���v�g�t�@�C����
				fscanf(pFile, " = %d", &m_nNumStage);

				// �X�N���v�g���̃������m��
				m_pFieldFileName = new char*[m_nNumStage];
				m_pSetFileName = new char*[m_nNumStage];
			}
			if (strcmp(aStr, "FIELD_FILENAME") == 0)
			{// �ǂݍ��ރX�N���v�g�t�@�C��
				m_pFieldFileName[nCntField] = new char[64];
				fscanf(pFile, " = %s", &m_pFieldFileName[nCntField][0]);
				nCntField++;
			}
			if (strcmp(aStr, "SET_FILENAME") == 0)
			{// �ǂݍ��ރX�N���v�g�t�@�C��
				m_pSetFileName[nCntSet] = new char[64];
				fscanf(pFile, " = %s", &m_pSetFileName[nCntSet][0]);
				nCntSet++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I", "system.ini", MB_OK);
	}

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃f�[�^�̉��
//=========================================
void CManager::Unload(void)
{
	for (int nCntStage = 0; nCntStage < m_nNumStage; nCntStage++)
	{// �������̊J��
		if (NULL != m_pFieldFileName[nCntStage])
		{// ������̔j��
			delete m_pFieldFileName[nCntStage];
			m_pFieldFileName[nCntStage] = NULL;
		}

		if (NULL != m_pSetFileName[nCntStage])
		{// ������̔j��
			delete m_pSetFileName[nCntStage];
			m_pSetFileName[nCntStage] = NULL;
		}
	}

	if (NULL != m_pFieldFileName)
	{// �������̊J��
		delete m_pFieldFileName;
		m_pFieldFileName = NULL;
	}

	if (NULL != m_pSetFileName)
	{// �������̊J��
		delete m_pSetFileName;
		m_pSetFileName = NULL;
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CManager::CManager()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CManager::~CManager()
{
}

//=========================================
// ����������
//=========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �V�X�e���t�@�C����ǂݍ���
	Load();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �����_�����O�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pRenderer)
	{// ����������
		m_pRenderer = new CRenderer;	// �������m��

		if (NULL != m_pRenderer)
		{// ���������m�ۂł���
			if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))	//FALSE�Ńt���X�N���[��
			{// ����������
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�����_�����O�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// ��łȂ������Ƃ�
		MessageBox(0, "�����_�����O�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �C���v�b�g(�L�[�{�[�h)�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pInputKeyboard)
	{// ����������
		m_pInputKeyboard = new CInputKeyboard;	// �L�[�{�[�h�̓��͗p

		if (NULL != m_pInputKeyboard)
		{// �������m��
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{// ����������
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�C���v�b�g(�L�[�{�[�h)�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// ��łȂ������Ƃ�
		MessageBox(0, "�C���v�b�g(�L�[�{�[�h)�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �C���v�b�g(DirectInput)�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pJoyStick)
	{// ����������
		m_pJoyStick = new CInputJoyStick;	// �L�[�{�[�h�̓��͗p

		if (NULL != m_pJoyStick)
		{// �������m��
			if (FAILED(m_pJoyStick->Init(hInstance, hWnd)))
			{// ����������
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�C���v�b�g(DirectInput)�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// ��łȂ������Ƃ�
		MessageBox(0, "�C���v�b�g(DirectInput)�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �C���v�b�g(XInput)�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pXInput)
	{// ����������
		m_pXInput = new CXInput;	// �L�[�{�[�h�̓��͗p

		if (NULL != m_pXInput)
		{// �������m��
			if (FAILED(m_pXInput->Init()))
			{// ����������
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�C���v�b�g(XInput)�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// ��łȂ������Ƃ�
		MessageBox(0, "�C���v�b�g(XInput)�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �J�����N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pCamera)
	{// �k���`�F�b�N
		m_pCamera = new CCamera;	// �������m��

		if (NULL != m_pCamera)
		{// ���������m�ۂł������ǂ���
			m_pCamera->Init();
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�J�����N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// NULL�łȂ������Ƃ�
		MessageBox(0, "�J�����N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ���C�g�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pLight)
	{// �k���`�F�b�N
		m_pLight = new CLight;	// �������m��

		if (NULL != m_pLight)
		{// ���������m�ۂł������ǂ���
			m_pLight->Init();
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "���C�g�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// NULL�łȂ������Ƃ�
		MessageBox(0, "���C�g�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

#ifdef _DEBUG
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �f�o�b�O�����N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pDebugProc)
	{// �k���`�F�b�N
		m_pDebugProc = new CDebugProc;	// �������m��

		if (NULL != m_pDebugProc)
		{// ���������m�ۂł������ǂ���
			m_pDebugProc->Init();
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�f�o�b�O�����N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// NULL�łȂ������Ƃ�
		MessageBox(0, "�f�o�b�O�����N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}
#endif

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �T�E���h�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pSound)
	{// �k���`�F�b�N
		m_pSound = new CSound;	// �������m��

		if (NULL != m_pSound)
		{// ���������m�ۂł������ǂ���
			m_pSound->Init(hWnd);
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�T�E���h�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// NULL�łȂ������Ƃ�
		MessageBox(0, "�T�E���h�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	// ���݂̃��[�h��ݒ�
	SetMode(m_mode);

	// �t�@�C������f�[�^��ǂݎ��
	CParData::Load();

	// �f�o�b�O�����̕\��
	m_bDebugProc = true;

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CManager::Uninit(void)
{
	// �f�[�^�̔j��
	Unload();
	CMeshField::Unload();
	CParData::Unload();

	CScene::ReleaseAll();	// �S�ẴI�u�W�F�N�g�̔j��

	if (NULL != m_pRenderer)
	{// �����_�����O�N���X�̔j��
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (NULL != m_pInputKeyboard)
	{// �C���v�b�g(�L�[�{�[�h)�N���X�̔j��
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (NULL != m_pJoyStick)
	{// �C���v�b�g(DirectInput)�N���X�̔j��
		m_pJoyStick->Uninit();
		delete m_pJoyStick;
		m_pJoyStick = NULL;
	}

	if (NULL != m_pXInput)
	{// �C���v�b�g(XInput)�N���X�̔j��
		m_pXInput->Uninit();
		delete m_pXInput;
		m_pXInput = NULL;
	}

	if (NULL != m_pCamera)
	{// �J�����N���X�̔j��
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (NULL != m_pLight)
	{// ���C�g�N���X�̔j��
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

#ifdef _DEBUG
	if (NULL != m_pDebugProc)
	{// �f�o�b�O�����N���X�̔j��
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
#endif

	if (NULL != m_pSound)
	{// �T�E���h�N���X�̔j��
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
}

//=========================================
// �X�V����
//=========================================
void CManager::Update(void)
{
	m_pInputKeyboard->Update();		// �C���v�b�g�N���X�̍X�V
	m_pJoyStick->Update();
	m_pXInput->Update();

	if (m_pGame != NULL)
	{// �Q�[����
		if (!m_pGame->GetPauseBool())
		{// �|�[�Y���łȂ��Ƃ�
			m_pCamera->Update();			// �J�����N���X�̍X�V
			m_pRenderer->Update();			// �����_���[�N���X�̍X�V
			m_pLight->Update();				// ���C�g�N���X�̍X�V
		}
		else
		{// �|�[�Y��
			CPause *pPause = m_pGame->GetPause();
			if (pPause != NULL)
			{
				pPause->Update();
			}

			m_pGame->Update();
		}
	}
	else
	{// �Q�[���ȊO
		m_pCamera->Update();			// �J�����N���X�̍X�V
		m_pRenderer->Update();			// �����_���[�N���X�̍X�V
		m_pLight->Update();				// ���C�g�N���X�̍X�V
	}

#ifdef _DEBUG
	if (m_pInputKeyboard->GetTrigger(DIK_F1))
	{
		m_bDebugProc = m_bDebugProc ? false : true;
		m_pDebugProc->DebugDisp(m_bDebugProc);
	}
#endif
}

//=========================================
// �`�揈��
//=========================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();	// �`��
}

//=============================================================================
// ���[�h�̏I���E����������
//=============================================================================
const void CManager::SetMode(MODE mode)
{
	// �t�F�[�h�ȊO�̃I�u�W�F�N�g��j��
	CScene::ModeUninit();

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// �^�C�g��
			m_pTitle = NULL;
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// �`���[�g���A��
			m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{// �Q�[��
			m_pSound->StopSound(CSound::SOUND_LABEL_GAME);
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{// �N���A
			m_pResult = NULL;
		}
		break;

	case MODE_GAMEOVER:
		if (m_pResult != NULL)
		{// �Q�[���I�[�o�[
			m_pResult = NULL;
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{// �����L���O
			m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			m_pSound->StopSound(CSound::SOUND_LABEL_CLEAR);
			m_pRanking = NULL;
		}
		break;
	}

	m_mode = mode;

	//�V�������(���[�h)�̏���������
	switch (mode)
	{
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{// �^�C�g��
			m_pSound->PlaySound(CSound::SOUND_LABEL_TITLE);
			m_pCamera->Init();
			m_pTitle = new CTitle;
			m_pTitle->Init();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// �^�C�g��
			m_pTutorial = new CTutorial;
			m_pTutorial->Init();
		}
		break;

	case MODE_GAME:
		if (m_pGame == NULL)
		{// �Q�[��
			m_pSound->PlaySound(CSound::SOUND_LABEL_GAME);
			m_pCamera->Init();
			m_pGame = new CGame;
			m_pGame->Init();
		}
		break;

	case MODE_RESULT:
		if (m_pResult == NULL)
		{// �N���A
			m_pSound->PlaySound(CSound::SOUND_LABEL_CLEAR);
			m_pResult = new CResult;
			m_pResult->Init();
		}
		break;

	case MODE_GAMEOVER:
		if (m_pResult == NULL)
		{// �Q�[���I�[�o�[
			//m_pSound->PlaySound(CSound::SOUND_LABEL_OVER);
			m_pResult = new CResult;
			m_pResult->Init();
		}
		break;

	case MODE_RANKING:
		if (m_pRanking == NULL)
		{// �����L���O
			m_pRanking = new CRanking;
			m_pRanking->Init();
		}
		break;
	}
}