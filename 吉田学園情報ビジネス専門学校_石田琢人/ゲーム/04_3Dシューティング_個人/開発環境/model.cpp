//=============================================================================
//
// ���f������ [model.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODELNAME_00		"data/MODEL/item.x"		// �ǂݍ���x�t�@�C����

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		CModel::m_pTexture[MAX_MODEL] = {};		// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH				CModel::m_pMesh[MAX_MODEL] = {};		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			CModel::m_pBuffMat[MAX_MODEL] = {};		// �}�e���A�����ւ̃|�C���^
DWORD					CModel::m_nNumMat[MAX_MODEL] = {};		// �}�e���A�����̐�

//=========================================
// ��������
//=========================================
CModel* CModel::Create(D3DXVECTOR3 pos, TYPE type)
{
	CModel *pModel;

	pModel = new CModel;	// ���������m��
	pModel->BindInfo(m_pTexture[type], m_pMesh[type], m_pBuffMat[type], m_nNumMat[type]);	// ���f���̏���CScene3DModel�ɓn��
	pModel->Init(pos, type);
	return pModel;
}

//=========================================
// ���f���̓ǂݍ���
//=========================================
HRESULT CModel::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < TYPE_MAX; nCntModel++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(MODELNAME_00,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMat[nCntModel],
			NULL,
			&m_nNumMat[nCntModel],
			&m_pMesh[nCntModel]);

		pMat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
		D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[nCntModel]);
	}

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃��f���̉��
//=========================================
void CModel::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		// �e�N�X�`���̔j��
		if (NULL != m_pTexture[nCntModel])
		{
			m_pTexture[nCntModel]->Release();
			m_pTexture[nCntModel] = NULL;
		}

		if (NULL != m_pMesh[nCntModel])
		{
			m_pMesh[nCntModel]->Release();
			m_pMesh[nCntModel] = NULL;
		}

		if (NULL != m_pBuffMat[nCntModel])
		{
			m_pBuffMat[nCntModel]->Release();
			m_pBuffMat[nCntModel] = NULL;
		}

		if (NULL != m_nNumMat[nCntModel])
		{
			m_nNumMat[nCntModel] = NULL;
		}
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CModel::CModel()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CModel::~CModel()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, TYPE type)
{
	CScene3DModel::Init();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{
	CScene3DModel::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void)
{
	CScene3DModel::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel::Draw(void)
{
	CScene3DModel::Draw();
}
