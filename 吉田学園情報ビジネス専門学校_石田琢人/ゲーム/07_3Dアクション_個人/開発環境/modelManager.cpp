//=============================================================================
//
// ���f������ [model.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "modelManager.h"
#include "manager.h"
#include "renderer.h"
#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_FILE "data/TEXT/PLAYER/model.txt"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int	CModelManager::m_NumModel = 0;		// ���f����
char **CModelManager::m_pStr = NULL;

//=========================================
// ���f���̓ǂݍ���
//=========================================
HRESULT CModelManager::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntModel = 0;

	// �t�@�C�����J��
	pFile = fopen(MODEL_FILE, "r");

	if (NULL != pFile)
	{// �t�@�C����������
		char aStr[128];

		while (1)
		{// �X�N���v�g��ǂݏI���܂Ń��[�v
			fscanf(pFile, "%s", aStr);

			if (strcmp(aStr, "SCRIPT") == 0)
			{// �X�N���v�g�J�n
				while (strcmp(aStr, "END_SCRIPT") != 0)
				{// �X�N���v�g�I�����b�Z�[�W�܂Ń��[�v
					fscanf(pFile, "%s", aStr);

					if (strcmp(aStr, "NUM_MODEL") == 0)
					{// ���f����
						fscanf(pFile, " = %d", &m_NumModel);
						m_pStr = new char*[m_NumModel];

						//m_pMesh = new LPD3DXMESH[m_NumModel];
						//m_pBuffMat = new LPD3DXBUFFER[m_NumModel];
						//m_nNumMat = new DWORD[m_NumModel];
					}
					else if (strcmp(aStr, "MODEL_FILENAME") == 0)
					{// X�t�@�C���̓ǂݍ���
						m_pStr[nCntModel] = new char[64];
						fscanf(pFile, " = %s", &m_pStr[nCntModel][0]);

						//if (nCntModel < m_NumModel)
						//{// ���������m�ۂ��������̂�
						//	D3DXLoadMeshFromX(aStr,
						//		D3DXMESH_SYSTEMMEM,
						//		pDevice,
						//		NULL,
						//		&m_pBuffMat[nCntModel],
						//		NULL,
						//		&m_nNumMat[nCntModel],
						//		&m_pMesh[nCntModel]);
						//}
						nCntModel++;	// �J�E���^��i�߂�
					}
				}
				break;
			}
		}
		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����Ȃ��Ƃ�
		MessageBox(0, "�Ȃ�", "model.txt", MB_OK);
	}
	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CModelManager::Unload(void)
{
	if (m_pStr != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_NumModel; nCntModel++)
		{
			if (m_pStr[nCntModel] != NULL)
			{// �����̔j��
				delete m_pStr[nCntModel];
				m_pStr[nCntModel] = NULL;
			}
		}

		delete[] m_pStr;
		m_pStr = NULL;
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CModelManager::CModelManager()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CModelManager::~CModelManager()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModelManager::Init(void)
{

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModelManager::Uninit(void)
{
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CModelManager::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CModelManager::Draw(void)
{
}

//=============================================================================
// ���f�����̎擾
//=============================================================================
void CModelManager::GetInfo(int nIdx, LPD3DXMESH &pMesh, LPD3DXBUFFER &pBuffMat, DWORD *nNumMat)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXLoadMeshFromX(m_pStr[nIdx],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pBuffMat,
		NULL,
		nNumMat,
		&pMesh);

	//pMesh = m_pMesh[nIdx];
	//pBuffMat = m_pBuffMat[nIdx];
	//*nNumMat = m_nNumMat[nIdx];
}