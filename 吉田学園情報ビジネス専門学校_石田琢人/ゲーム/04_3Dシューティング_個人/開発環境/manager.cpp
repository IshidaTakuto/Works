//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "score.h"

//=========================================
// �}�N����`
//=========================================

//=========================================
// �ÓI�����o�ϐ�
//=========================================
CRenderer *CManager::m_pRenderer = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight * CManager::m_pLight = NULL;
CSound *CManager::m_pSound = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;
CTitle *CManager::m_pTitle = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CRanking *CManager::m_pRanking = NULL;

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
	// �����_�����O�N���X�̐���
	if (NULL == m_pRenderer)
	{
		m_pRenderer = new CRenderer;

		if (NULL != m_pRenderer)
		{
			// ����������
			if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))	//FALSE�Ńt���X�N���[��
			{
				return -1;
			}
		}
		else
		{
			MessageBox(0, "�����_�����O�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "�����_�����O�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	// �C���v�b�g�N���X�̐���
	if (NULL == m_pInputKeyboard)
	{// �L�[�{�[�h
		m_pInputKeyboard = new CInputKeyboard;	// �L�[�{�[�h�̓��͗p

		if (NULL != m_pInputKeyboard)
		{// �������m��
			// ����������
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�C���v�b�g�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// NULL�łȂ������Ƃ�
		MessageBox(0, "�C���v�b�g�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}
	if (NULL == m_pInputJoypad)
	{// �p�b�h
		m_pInputJoypad = new CInputJoypad;	// �L�[�{�[�h�̓��͗p

		if (NULL != m_pInputJoypad)
		{// �������m��
		 // ����������
			if (FAILED(m_pInputJoypad->Init(hInstance, hWnd)))
			{
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�C���v�b�g�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// NULL�łȂ������Ƃ�
		MessageBox(0, "�C���v�b�g�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}


	// �J�����N���X�̐���
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

	// ���C�g�N���X�̐���
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

	// �T�E���h�N���X�̐���
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

	// ���[�h�̐ݒ�
	SetMode(m_mode);

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CManager::Uninit(void)
{
	CScene::ReleaseAll();

	// �����_�����O�N���X�̔j��
	if (NULL != m_pRenderer)
	{
		// �I������
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	else
	{
		MessageBox(0, "�����_�����O�N���X�̔j���Ɏ��s���܂���", "���s", MB_OK);
	}

	// �J�����N���X�̔j��
	if (NULL != m_pCamera)
	{
		// �I������
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	else
	{
		MessageBox(0, "�J�����N���X�̔j���Ɏ��s���܂���", "���s", MB_OK);
	}

	// ���C�g�N���X�̔j��
	if (NULL != m_pLight)
	{
		// �I������
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	else
	{
		MessageBox(0, "���C�g�N���X�̔j���Ɏ��s���܂���", "���s", MB_OK);
	}

	// �T�E���h�N���X�̔j��
	if (NULL != m_pSound)
	{
		// �I������
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
	else
	{
		MessageBox(0, "���C�g�N���X�̔j���Ɏ��s���܂���", "���s", MB_OK);
	}

	// �C���v�b�g�N���X�̔j��
	if (NULL != m_pInputKeyboard)
	{// �L�[�{�[�h
		// �I������
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	else
	{
		MessageBox(0, "�C���v�b�g�N���X�̔j���Ɏ��s���܂���", "���s", MB_OK);
	}
	if (NULL != m_pInputJoypad)
	{// �p�b�h
		// �I������
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}
	else
	{
		MessageBox(0, "�C���v�b�g�N���X�̔j���Ɏ��s���܂���", "���s", MB_OK);
	}

	// �^�C�g���N���X�̔j��
	if (NULL != m_pTitle)
	{
		// �I������
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = NULL;
	}

	// �`���[�g���A���N���X�̔j��
	if (NULL != m_pTutorial)
	{
		// �I������
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = NULL;
	}

	// �Q�[���N���X�̔j��
	if (NULL != m_pGame)
	{
		// �I������
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = NULL;
	}

	// ���U���g�N���X�̔j��
	if (NULL != m_pResult)
	{
		// �I������
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = NULL;
	}

	// �����L���O�N���X�̔j��
	if (NULL != m_pRanking)
	{
		// �I������
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = NULL;
	}

}

//=========================================
// �X�V����
//=========================================
void CManager::Update(void)
{
	m_pInputKeyboard->Update();
	m_pInputJoypad->Update();
	m_pLight->Update();

	// �|�[�Y���̓J�������X�V���Ȃ�
	bool bPause = CGame::GetPause();
	if (bPause == false)
	{
		m_pCamera->Update();
	}

	switch (m_mode)
	{
	case MODE_TITLE:	// �^�C�g��
		if (m_pTitle != NULL)
		{// �^�C�g���̍X�V����
			m_pTitle->Update();
		}

		break;

	case MODE_TUTORIAL:	// �`���[�g���A��
		if (m_pTutorial != NULL)
		{// �`���[�g���A���̍X�V����
			m_pTutorial->Update();
		}

		break;

	case MODE_GAME:		// �Q�[��
		if (m_pGame != NULL)
		{// �Q�[���̍X�V����
			m_pGame->Update();
		}

		break;

	case MODE_RESULT:	// ���U���g
		if (m_pResult != NULL)
		{// �Q�[���̍X�V����
			m_pResult->Update();
		}

		break;

	case MODE_GAMEOVER:	// ���U���g
		if (m_pResult != NULL)
		{// �Q�[���̍X�V����
			m_pResult->Update();
		}

		break;

	case MODE_RANKING:	// �����L���O
		if (m_pRanking != NULL)
		{// �Q�[���̍X�V����
			m_pRanking->Update();
		}

		break;
	}
}

//=========================================
// �`�揈��
//=========================================
void CManager::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	pRenderer->Draw();
}

//=============================================================================
// ���[�h�̏I���E����������
//=============================================================================
const void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// �^�C�g��
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// �`���[�g���A��
			m_pSound->StopSound(CSound::SOUND_LABEL_TUTORIAL);
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{// �Q�[��
			m_pRanking->Set(CScore::GetScore());
			m_pSound->StopSound(CSound::SOUND_LABEL_GAME);
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{// �N���A
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;

	case MODE_GAMEOVER:
		if (m_pResult != NULL)
		{// �Q�[���I�[�o�[
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{// �����L���O
			m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			m_pSound->StopSound(CSound::SOUND_LABEL_CLEAR);
			m_pSound->StopSound(CSound::SOUND_LABEL_OVER);
			m_pRanking->Uninit();
			delete m_pRanking;
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
		{// �`���[�g���A��
			m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			m_pSound->PlaySound(CSound::SOUND_LABEL_TUTORIAL);
			m_pCamera->Init();
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
			m_pSound->PlaySound(CSound::SOUND_LABEL_OVER);
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

	//m_mode = mode;
}