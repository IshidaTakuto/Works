//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debugProc.h"

#include "player.h"
#include "meshField.h"
#include "meshCylinder.h"
#include "scene3DIndex.h"

//=========================================
// �}�N����`
//=========================================

//=========================================
// �ÓI�����o�ϐ�
//=========================================
CRenderer *CManager::m_pRenderer = NULL;		// �����_���[�N���X�̃|�C���^�ϐ�
CInputKeyboard *CManager::m_pInput = NULL;		// �C���v�b�g�N���X�̃|�C���^�ϐ�
CCamera *CManager::m_pCamera = NULL;
CLight * CManager::m_pLight = NULL;
CDebugProc *CManager::m_pDebugProc = NULL;
CPlayer *CManager::m_pPlayer = NULL;
CMeshField *CManager::m_pMeshField = NULL;
CCylinder *CManager::m_pCylinder = NULL;

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

	CScene3DIndex::Load();
	CMeshField::Load();	// ���b�V���t�B�[���h�e�N�X�`���̓ǂݍ���
	CPlayer::Load();	// �v���C���[�̓ǂݍ���

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create();

	// ���b�V���t�B�[���h����
	m_pMeshField = CMeshField::Create();

	// ���b�V���V�����_�[����
	m_pCylinder = CCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CManager::Uninit(void)
{
	CMeshField::Unload();	// �t�B�[���h�e�N�X�`���̔j��
	CPlayer::Unload();	// �v���C���[���f���̔j��

	if (m_pPlayer != NULL)
	{// �v���C���[�̔j��
		m_pPlayer->Uninit();
		m_pPlayer = NULL;
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
	CDebugProc::Print("�t�@�C���o�́@�@[F9]");
	CDebugProc::Print("�t�@�C���ǂݍ���[F3]");
	CDebugProc::Print("-------------------------------");
	CDebugProc::Print("���C���[�t���[��[F8]");
	CDebugProc::Print("�e�N�X�`���@�@�@[F7]");
	CDebugProc::Print("�@���̍Čv�Z�@�@[F5]");
	CDebugProc::Print("���_���グ��@�@[Y]");
	CDebugProc::Print("�w���������@�@�@[X+I][X+K]");
	CDebugProc::Print("�y���������@�@�@[Z+I][Z+K]");
	CDebugProc::Print("���@�@�@�@�@�@�@[X+U][X+J]");
	CDebugProc::Print("���s�@�@�@�@�@�@[Z+U][Z+J]");
	CDebugProc::Print("�I��͈́@�@�@�@[��][��]");
	CDebugProc::Print("�グ��l�@�@�@�@[��][��]");
	CDebugProc::Print("SHIFT�Ő��l�𑝉�");
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
