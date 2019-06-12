//=============================================================================
//
// ���b�V���t�B�[���h���� [meshField.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME	"data/TEXT/field_texture.txt"
#define MESH_X			(6)			// x�������̃}�X��
#define MESH_Z			(6)			// z�������̃}�X��
#define MESH_WIDTH		(400.0f)	// ��
#define MESH_DEPTH		(400.0f)	// ���s
#define MAX_POLYGON		((MESH_X * MESH_Z * 2) + (4 * (MESH_Z - 1)))		// �|���S����

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 *CMeshField::m_pTexture = NULL;	// �e�N�X�`�����ւ̃|�C���^
int CMeshField::m_nNumTex = 0;						// �e�N�X�`����

//==================================
// ��������
//==================================
CMeshField* CMeshField::Create(void)
{
	CMeshField *pMeshField;

	pMeshField = new CMeshField;	// ���������m��
	pMeshField->Init();

	return pMeshField;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CMeshField::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;			// �t�@�C���̃|�C���^�ϐ�
	char aTexName[64];		// �e�N�X�`���̖��O
	int nCntTexture;

	// �t�@�C�����J��
	pFile = fopen(TEXTURE_NAME, "r");

	if (NULL != pFile)
	{// �t�@�C����������
		fscanf(pFile, "%d", &m_nNumTex);		// �e�N�X�`����

		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumTex];	// �e�N�X�`�������������m��

		for (nCntTexture = 0; nCntTexture < m_nNumTex; nCntTexture++)
		{
			// �e�N�X�`���̃p�X���L��
			fscanf(pFile, "%s\n", &aTexName[0]);

			if (&m_pTexture[nCntTexture] != NULL)
			{// �e�N�X�`���̐���
				D3DXCreateTextureFromFile(pDevice, &aTexName[0], &m_pTexture[nCntTexture]);
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I", "field_texture.txt", MB_OK);
	}

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CMeshField::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < m_nNumTex; nCntTexture++)
	{
		if (NULL != m_pTexture[nCntTexture])
		{// �e�N�X�`���̔j��
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}

	// �������̊J��
	delete m_pTexture;

	// �e�N�X�`�����̏�����
	m_nNumTex = 0;
}

//=========================================
// �R���X�g���N�^
//=========================================
CMeshField::CMeshField()
{
	m_nWire = 0;
	m_nType = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CMeshField::~CMeshField()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshField::Init(void)
{
	CScene3DIndex::SetObjType(CScene::OBJTYPE_FIELD);
	CScene3DIndex::Init();
	CScene3DIndex::MakeVertex();
	CScene3DIndex::BindTexture(m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMeshField::Uninit(void)
{
	CScene3DIndex::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshField::Update(void)
{
	CScene3DIndex::Update();

	// �L�[�{�[�h�̓��͂��擾
	CInputKeyboard *pKeyboard = CManager::GetInput();

	if (pKeyboard->GetTrigger(DIK_F8) == true)
	{// ���C���[�t���[���\��
		m_nWire = (2 + m_nWire) % 4;
	}

	if (pKeyboard->GetTrigger(DIK_F7) == true)
	{// �e�N�X�`���\��
		m_nType = (1 + m_nType) % (m_nNumTex + 1);

		if (m_nType < m_nNumTex)
		{// �e�N�X�`�����w��
			CScene3DIndex::BindTexture(m_pTexture[m_nType]);
		}
		else
		{// �e�N�X�`���𒣂�Ȃ�
			CScene3DIndex::BindTexture(NULL);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshField::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���C���[�t���[����\��
	pDevice->SetRenderState(D3DRS_FILLMODE, m_nWire);

	CScene3DIndex::Draw();

	// ���C���[�t���[�����\��
	pDevice->SetRenderState(D3DRS_FILLMODE, 0);
}
