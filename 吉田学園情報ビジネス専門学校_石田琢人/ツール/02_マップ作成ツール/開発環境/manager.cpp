//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : Ishida Takuto
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

#include "setPlayer.h"
#include "meshField.h"
#include "scene3DIndex.h"
#include "GUIManager.h"

#include <stdio.h>

//=========================================
// �}�N����`
//=========================================
#define SYSTEM_FILE "data/TEXT/system.ini"

//=========================================
// �ÓI�����o�ϐ�
//=========================================
CRenderer *CManager::m_pRenderer = NULL;		// �����_���[�N���X�̃|�C���^�ϐ�
CInputKeyboard *CManager::m_pInput = NULL;		// �C���v�b�g�N���X�̃|�C���^�ϐ�
CCamera *CManager::m_pCamera = NULL;
CLight * CManager::m_pLight = NULL;
CDebugProc *CManager::m_pDebugProc = NULL;
CSetPlayer *CManager::m_pSetPlayer = NULL;
CMeshField *CManager::m_pMeshField = NULL;

int	 CManager::m_nNumStage = 0;
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

//=============================================================================
// �I�u�W�F�N�g���`�F�b�N
//=============================================================================
int CManager::SetNumType(void)
{
	for (int nCntType = 0; nCntType < CModelObjectManager::TYPE_MAX; nCntType++)
	{
		if (0 < CModelObjectManager::GetNumType(m_type))
		{
			return nCntType;
		}
		m_type = (CModelObjectManager::TYPE)(1 + m_type);
	}

	return -1;
}

//=========================================
// �R���X�g���N�^
//=========================================
CManager::CManager()
{
	m_type = CModelObjectManager::TYPE_MODEL;
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
	// �V�X�e���t�@�C���̓ǂݍ���
	Load();

	// �����_�����O�N���X�̐���
	if (NULL == m_pRenderer)
	{// ����������
		m_pRenderer = new CRenderer;	// �������m��

		if (NULL != m_pRenderer)
		{// ���������m�ۂł���
			if (FAILED(m_pRenderer->Init(hInstance, hWnd, TRUE)))	//FALSE�Ńt���X�N���[��
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

	// �C���v�b�g�N���X�̐���
	if (NULL == m_pInput)
	{// ����������
		m_pInput = new CInputKeyboard;	// �L�[�{�[�h�̓��͗p

		if (NULL != m_pInput)
		{// �������m��
			if (FAILED(m_pInput->Init(hInstance, hWnd)))
			{// ����������
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�C���v�b�g�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// ��łȂ������Ƃ�
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

	// �f�o�b�O�����N���X�̐���
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

	m_type = (CModelObjectManager::TYPE)SetNumType();
	if (m_type >= 0)
	{// �v���C���[�̐���
		m_pSetPlayer = CSetPlayer::Create(m_type);
		CGuiManager::SetObjectType(m_type);
	}

	// ���b�V���t�B�[���h����
	m_pMeshField = CMeshField::Create();

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CManager::Uninit(void)
{
	CModelObjectManager::Unload();

	if (m_pSetPlayer != NULL)
	{// �v���C���[�̔j��
		m_pSetPlayer->Uninit();
		m_pSetPlayer = NULL;
	}

	CScene::ReleaseAll();	// �S�ẴI�u�W�F�N�g�̔j��

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

	// �C���v�b�g�N���X�̔j��
	if (NULL != m_pInput)
	{
		// �I������
		m_pInput->Uninit();
		delete m_pInput;
		m_pInput = NULL;
	}
	else
	{
		MessageBox(0, "�C���v�b�g�N���X�̔j���Ɏ��s���܂���", "���s", MB_OK);
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

	// �f�o�b�O�����N���X�̔j��
	if (NULL != m_pDebugProc)
	{
		// �I������
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
	else
	{
		MessageBox(0, "�f�o�b�O�����N���X�̔j���Ɏ��s���܂���", "���s", MB_OK);
	}
}

//=========================================
// �X�V����
//=========================================
void CManager::Update(void)
{
	CDebugProc::Print("-------------------------------");
	CDebugProc::Print("�\���E��\���@�@[F1]");
	CDebugProc::Print("���C���[�t���[��[F8]");
	CDebugProc::Print("�e�N�X�`���@�@�@[F7]");
	CDebugProc::Print("�J���������NUM�p�b�h�̐����łł��܂�");
	CDebugProc::Print("-------------------------------\n\n");

	if (m_pInput->GetTrigger(DIK_F1))
	{// �f�o�b�O�\���E��\��
		m_pDebugProc->DebugDisp();
	}

	m_pCamera->Update();	// �J�����N���X�̍X�V
	m_pInput->Update();		// �C���v�b�g�N���X�̍X�V
	m_pRenderer->Update();	// �����_���[�N���X�̍X�V
	m_pLight->Update();		// ���C�g�N���X�̍X�V
}

//=========================================
// �`�揈��
//=========================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();	// �`��
}
