//=============================================================================
//
// �Ǐ��� [wall.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "wall.h"
#include "scene3DModel.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define WALL_MODELNAME00	"data/model/rock.x"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9	CWall::m_pTexture = NULL;	// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH			CWall::m_pMesh = NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		CWall::m_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^
DWORD				CWall::m_nNumMat = NULL;	// �}�e���A�����̐�

//==================================
// ��������
//==================================
CWall *CWall::Create(int nPos)
{
	CWall *pWall;

	pWall = new CWall;	// ���������m��

	if (pWall != NULL)
	{// ���������m�ۂł���
		pWall->m_nPos = nPos;	// ���X���W�̐ݒ�
		pWall->Init();			// ����������
	}

	return pWall;
}

//=========================================
// ���f�����̓ǂݍ���
//=========================================
HRESULT CWall::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(WALL_MODELNAME00,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// �e�N�X�`���̐���
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture);

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CWall::Unload(void)
{
	if (NULL != m_pTexture)
	{// �e�N�X�`���̔j��
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (NULL != m_pMesh)
	{// ���b�V�����̔j��
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (NULL != m_pBuffMat)
	{// �}�e���A�����̔j��
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	if (NULL != m_nNumMat)
	{// �}�e���A����������������
		m_nNumMat = 0;
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CWall::CWall(int nPriority) : CScene(nPriority)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL_ROCK; nCntWall++)
	{// ���f���N���X�ւ̃|�C���^�ϐ�
		m_pModel[nCntWall] = NULL;
	}

	m_nPos = 0;		// ���X���W
}

//=========================================
// �f�X�g���N�^
//=========================================
CWall::~CWall()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CWall::Init()
{
	for (int nCntWall = 0; nCntWall < MAX_WALL_ROCK; nCntWall++)
	{
		m_pModel[nCntWall] = new CScene3DModel;		// �������̊m��

		if (m_pModel[nCntWall] != NULL)
		{// ���������m�ۂł���
			m_pModel[nCntWall]->BindInfo(m_pTexture, m_pMesh, m_pBuffMat, m_nNumMat);	// ���f�����̐ݒ�

			// ����������
			m_pModel[nCntWall]->Init();

			//_�ʒu�̐ݒ�
			m_pModel[nCntWall]->SetPos(D3DXVECTOR3((float)m_nPos + (rand() % 200 + 1.0f) + (nCntWall * 100.0f), 0.0f, (nCntWall % 2) * -1000.0f + 500.0f));

			// �����̐ݒ�
			m_pModel[nCntWall]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI - (rand() % 6) - (rand() % 10 * 0.01f), 0.0f));
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWall::Uninit(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL_ROCK; nCntWall++)
	{// ���f�������[�v
		if (m_pModel[nCntWall] != NULL)
		{// ���f���̔j��
			m_pModel[nCntWall]->Uninit();
			m_pModel[nCntWall] = NULL;
		}
	}

	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CWall::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CWall::Draw(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL_ROCK; nCntWall++)
	{// ���f�������[�v
		m_pModel[nCntWall]->Draw();
	}
}
