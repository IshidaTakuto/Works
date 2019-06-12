//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "GUIManager.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "modelObject.h"
#include "setPlayer.h"
#include "manager.h"
#include "enemy.h"
#include "meshField.h"

#include <stdio.h>

//=========================================
// �}�N����`
//=========================================
#define IMGUI_TEXT_SIZE	(15.0f)		// ImGui�̃e�L�X�g�T�C�Y

//=========================================
// �ÓI�����o�ϐ�
//=========================================
int CGuiManager::m_nType = 0;
CModelObjectManager::TYPE CGuiManager::m_type = CModelObjectManager::TYPE_MODEL;
float CGuiManager::m_fHeight = 0.0f;				// �グ�鍂��
float CGuiManager::m_fRange = 0.001f;				// �I��͈�
float CGuiManager::m_aLength[2] = { 0.0f,0.0f };	// ����
int CGuiManager::m_aSplit[2] = { 0,0 };				// ������

//=========================================
// ��������
//=========================================
CGuiManager *CGuiManager::Create(void)
{
	CGuiManager *pGUIManager = NULL;
	pGUIManager = new CGuiManager;		// ���������m��

	if (pGUIManager != NULL)
	{// ���������m�ۂł���
	}

	return pGUIManager;
}

//=========================================
// �R���X�g���N�^
//=========================================
CGuiManager::CGuiManager()
{
	m_showDemoWindow = false;
	m_showFieldWindow = false;
	m_clearColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	m_bRepeat = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CGuiManager::~CGuiManager()
{
}

//=========================================
// ����������
//=========================================
HRESULT CGuiManager::Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// �X�^�C���̃Z�b�g�A�b�v
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	// �v���b�g�t�H�[��/�����_���̃Z�b�g�A�b�v
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

#if 0	// �S�p�����t�H���g�ς���Ƃ��ɕK�v
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
#endif

	//�t�H���g�̐ݒ�
	LoadFont();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, m_glyphRangesJapanese);
	//io.Fonts->AddFontFromFileTTF("C:/Users/game104/AppData/Local/Microsoft/Windows/Fonts/TakaoGothic.ttf", IMGUI_TEXT_SIZE, NULL, m_glyphRangesJapanese);

	// �ϐ��̏����l�����߂�
	m_showDemoWindow = false;
	m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// ���f���t�@�C�����̕ۑ�
	m_aModelFileName[CModelObjectManager::TYPE_MODEL] = CModelObjectManager::GetModelFileName(CModelObjectManager::TYPE_MODEL);
	m_aModelFileName[CModelObjectManager::TYPE_START] = CModelObjectManager::GetModelFileName(CModelObjectManager::TYPE_START);
	m_aModelFileName[CModelObjectManager::TYPE_ENEMY] = CModelObjectManager::GetModelFileName(CModelObjectManager::TYPE_ENEMY);
	m_aModelFileName[CModelObjectManager::TYPE_ITEM] = CModelObjectManager::GetModelFileName(CModelObjectManager::TYPE_ITEM);

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CGuiManager::Uninit(void)
{
	// �j��
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=========================================
// �X�V����
//=========================================
void CGuiManager::Update(void)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (m_showDemoWindow)
	{// �f���E�B���h�E��\��
		ImGui::ShowDemoWindow(&m_showDemoWindow);
	}

	// �ʒu
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���C���E�B���h�E��\��
	{
		ImGui::Begin(u8"���C���E�C���h�E");							// �E�C���h�E�̃L���v�V������

		ImGui::Text(u8"�E�C���h�E�̕\��");							// �e�L�X�g�\��
		//ImGui::Checkbox(u8"�f���E�C���h�E", &m_showDemoWindow);		// �f���E�C���h�E�̃`�F�b�N�{�b�N�X
		ImGui::Checkbox(u8"�n�ʃE�C���h�E", &m_showFieldWindow);	// �n�ʃE�C���h�E�̃`�F�b�N�{�b�N�X

		// �d�؂������
		SpaceLine(true);

		//------------------------------------------------
		// �I�u�W�F�N�g�̎�ނ�I������
		//------------------------------------------------
		int nType = 0;		// combo�֐��ɕK�v
		int nTypeOld = m_type;
		nType = (int)m_type;
		if (ImGui::Combo(u8"�I�u�W�F�N�g�^�C�v", &nType, "MODEL\0START\0ENEMY\0ITEM\0"))
		{// �I�u�W�F�N�g�^�C�v���I�����ꂽ
			int nNumType = CModelObjectManager::GetNumType((CModelObjectManager::TYPE)nType);
			if (nNumType <= 0)
			{// �I�������I�u�W�F�N�g�̎�ނ��Ȃ������Ƃ�
				nType = nTypeOld;	// �I����߂�

				MessageBox(NULL, "�I�������I�u�W�F�N�g�͂���܂���B", "�x��", MB_OK);
			}
			m_type = (CModelObjectManager::TYPE)nType;	// �ς�����l������

			m_nType = 0;
		}

		//------------------------------------------------
		// ���f���̎�ނ̑I��
		//------------------------------------------------
		if (CModelObjectManager::TYPE_START == m_type)
		{// �X�^�[�g
			ImGui::Combo(u8"���f���̎��", &m_nType, u8"�X�^�[�g\0�S�[��\0", CModelObjectManager::GetNumType(m_type));
		}
		else
		{// �X�^�[�g�ȊO
			ImGui::Combo(u8"���f���̎��", &m_nType, m_aModelFileName[m_type], CModelObjectManager::GetNumType(m_type));
		}

		//------------------------------------------------
		// ���f���̈ʒu
		//------------------------------------------------
		if (CManager::GetSetPlayer() != NULL)
		{// �g�p���Ă���Ƃ�
			pos = CManager::GetSetPlayer()->GetPos();
		}

		ImGui::DragFloat3(u8"�ʒu", pos);

		if (CManager::GetSetPlayer() != NULL)
		{// �g�p���Ă���Ƃ�
			CManager::GetSetPlayer()->SetPos(pos);
		}


		//------------------------------------------------
		// ���f���̌���
		//------------------------------------------------
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (CManager::GetSetPlayer() != NULL)
		{// �g�p���Ă���Ƃ�
			rot = CManager::GetSetPlayer()->GetRot();
		}

		ImGui::DragFloat3(u8"����", rot,D3DX_PI * 0.01f);

		// ���]���Ȃ��悤�Ɋp�x�𒲐�
		if (rot.x > D3DX_PI) { rot.x -= D3DX_PI * 2.0f; }
		if (rot.x < -D3DX_PI) { rot.x += D3DX_PI * 2.0f; }

		if (rot.y > D3DX_PI) { rot.y -= D3DX_PI * 2.0f; }
		if (rot.y < -D3DX_PI) { rot.y += D3DX_PI * 2.0f; }

		if (rot.z > D3DX_PI) { rot.z -= D3DX_PI * 2.0f; }
		if (rot.z < -D3DX_PI) { rot.z += D3DX_PI * 2.0f; }

		if (CManager::GetSetPlayer() != NULL)
		{// �g�p���Ă���Ƃ�
			CManager::GetSetPlayer()->SetRot(rot);
		}

		//------------------------------------------------
		// �I��͈͂̐ݒ�
		//------------------------------------------------
		ImGui::SliderFloat(u8"�I��͈�", &m_fRange, 0.001f, 1000.0f);

		// ���s������
		SpaceLine();

		//------------------------------------------------
		// ���f���̐ݒu
		//------------------------------------------------
		if (ImGui::Button(u8"�ݒu"))
		{// ���f���ݒu
			if (CModelObjectManager::TYPE_ENEMY == m_type)
			{// �G
				CEnemy::Create(pos, m_nType);
			}
			else
			{// �G�ȊO
				CModelObject::Create(pos, rot, m_type, m_nType);
			}
		}

		//------------------------------------------------
		// ���f���̍폜
		//------------------------------------------------
		if (ImGui::Button(u8"�폜"))
		{// ���f���ݒu
			CScene::RangeDelete();
		}

		// �d�؂������
		SpaceLine(true);

		//------------------------------------------------
		// �z�u�f�[�^�̕ۑ�
		//------------------------------------------------
		if (ImGui::Button(u8"�Z�[�u"))
		{// �z�u�f�[�^���Z�[�u����
			int nID = MessageBox(NULL, "�ۑ����܂����H", "�z�u�f�[�^", MB_YESNO);
			if (nID == IDYES)
			{// �Z�[�u����
				CModelObjectManager::Save();
			}
		}

		//------------------------------------------------
		// �z�u�f�[�^�̓ǂݍ���
		//------------------------------------------------
		if (ImGui::Button(u8"���[�h"))
		{// �z�u�f�[�^�����[�h����
			int nID = MessageBox(NULL, "�ǂݍ��݂܂����H", "�z�u�f�[�^", MB_YESNO);
			if (nID == IDYES)
			{// ���[�h����
				CModelObjectManager::Load();
				MessageBox(0, "�ǂݍ��݂��I���܂����B", "�z�u�f�[�^", MB_OK);
			}
		}

		ImGui::End();
	}

	if (m_showFieldWindow)
	{// �n�ʃE�C���h�E�̕\��
		ImGui::Begin(u8"�n�ʃE�C���h�E");	// �E�C���h�E�̃L���v�V������

		//------------------------------------------------
		// �グ�鍂���̐ݒ�
		//------------------------------------------------
		ImGui::DragFloat(u8"�グ�鍂��", &m_fHeight, 0.1f, -10.0f, 10.0f);

		//------------------------------------------------
		// �N���̐ݒ�
		//------------------------------------------------
		ImGui::Button(u8"�N��");
		if (ImGui::IsItemClicked(0))
		{
			m_bRepeat = true;
		}
		else
		{
			m_bRepeat = false;
		}

		if (m_bRepeat)
		{// �N�������
			CManager::GetMeshField()->CMeshField::SetHeight(pos, m_fHeight, m_fRange);
		}

		// �d�؂������
		SpaceLine(true);

		//------------------------------------------------
		// �������̐ݒ�
		//------------------------------------------------
		ImGui::DragInt2(u8"������(X,Z)", m_aSplit, 1, 1, 1000);

		//------------------------------------------------
		// �����̐ݒ�
		//------------------------------------------------
		ImGui::DragFloat2(u8"����(X,Z)", m_aLength, 0.1f, 0.001f, 1000.0f);

		// �d�؂������
		SpaceLine(true);

		//------------------------------------------------
		// �n�ʃf�[�^�̕ۑ�
		//------------------------------------------------
		if (ImGui::Button(u8"�Z�[�u"))
		{// �z�u�f�[�^���Z�[�u����
			int nID = MessageBox(NULL, "�ۑ����܂����H", "�n�ʃf�[�^", MB_YESNO);
			if (nID == IDYES)
			{// �Z�[�u����
				CManager::GetMeshField()->Save();
			}
		}

		//------------------------------------------------
		// �n�ʃf�[�^�̓ǂݍ���
		//------------------------------------------------
		if (ImGui::Button(u8"���[�h"))
		{// �z�u�f�[�^���Z�[�u����
			int nID = MessageBox(NULL, "�ǂݍ��݂܂����H", "�n�ʃf�[�^", MB_YESNO);
			if (nID == IDYES)
			{// ���[�h����
				CManager::GetMeshField()->Load();
				CManager::GetMeshField()->LoadVtx();
				MessageBox(0, "�ǂݍ��݂��I���܂����B", "�n�ʃf�[�^", MB_OK);
			}
		}

		ImGui::End();
	}

	ImGui::EndFrame();
}

//=========================================
// �`�揈��
//=========================================
void CGuiManager::Draw(void)
{
	// ImGui�̕`��
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//=========================================
// D3D9�f�o�C�X�̃n���h���������̏���
//=========================================
void CGuiManager::SetPresent(HRESULT result, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS d3dpp)
{
	if (result == D3DERR_DEVICELOST && pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		pDevice->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

//=========================================
// �t�H���g�̓ǂݎ��
//=========================================
void CGuiManager::LoadFont(void)
{
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/font.bin", "rb");

	if (NULL != pFile)
	{// �t�@�C����������

		// ���_�����t�@�C���ɏ�������
		fread((void*)m_glyphRangesJapanese, sizeof(ImWchar), 8253, pFile);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I\n�t�@�C�����쐬���܂��B", "font.bin", MB_OK);
	}
}

//=========================================
// �����̐ݒ�
//=========================================
void CGuiManager::SetLength(float nLengthX, float nLengthZ)
{
	m_aLength[0] = nLengthX;
	m_aLength[1] = nLengthZ;
}

//=========================================
// �������̐ݒ�
//=========================================
void CGuiManager::SetSplit(int nSplitX, int nSplitZ)
{
	m_aSplit[0] = nSplitX;
	m_aSplit[1] = nSplitZ;
}

//=========================================
// �C�ӂ̐������s������
//=========================================
void CGuiManager::SpaceLine(bool bPartition, int nLine)
{
	int nCntLine = 0;
	while (nCntLine < nLine)
	{// �V�����s������
		ImGui::NewLine();
		nCntLine++;
	}

	if (bPartition)
	{// �d�؂��
		ImGui::Separator();
	}
}