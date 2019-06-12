//=============================================================================
//
// ���b�V���V�����_�[���� [meshCylinder.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "meshCylinder.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MESHCYLINDER		"data/TEXTURE/sky.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MESH_HORIZONTAL	(20)			// ���������̕�����
#define MESH_VERTICAL	(1)				// ���������̕�����
#define MAX_VERTEX		((MESH_HORIZONTAL + 1) * (MESH_VERTICAL + 1))							// ���_��
#define MAX_INDEX		(MESH_HORIZONTAL * MESH_VERTICAL * 2 + (4 * (MESH_VERTICAL - 1)) + 2)	// �C���f�b�N�X�̐�
#define MAX_POLYGON		((MESH_HORIZONTAL * MESH_VERTICAL * 2) + (4 * (MESH_VERTICAL - 1)))		// �|���S����

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CCylinder::m_pTexture[CYLINDER_TEXTURE] = {};

//=============================================================================
// ��������
//=============================================================================
CCylinder* CCylinder::Create(D3DXVECTOR3 pos, int nWidth, int nHeight, int nDepth, int nMeshX, int nMeshY, int nMeshZ, int nTexture)
{
	CCylinder *pCylinder;

	pCylinder = new CCylinder;	// ���������m��
	pCylinder->Init(pos, nWidth, nHeight, nDepth, nMeshX, nMeshY, nMeshZ);
	pCylinder->BindTexture(m_pTexture[nTexture]);

	return pCylinder;
}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CCylinder::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHCYLINDER, &m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=============================================================================
void CCylinder::Unload(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < CYLINDER_TEXTURE; nCntTexture++)
	{
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCylinder::CCylinder(int nPriority) : CScene3DIndex(nPriority)
{
	m_nTexture = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCylinder::~CCylinder()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCylinder::Init(D3DXVECTOR3 pos, int nWidth, int nHeight, int nDepth, int nMeshX, int nMeshY, int nMeshZ)
{
	CScene3DIndex::Init();
	CScene::SetObjType(CScene::OBJTYPE_CYLINDER);
	CScene3DIndex::MakeVertex(nMeshX, nMeshY, nMeshZ, nWidth, nHeight, nDepth, TYPE_CYLINDER);
	CScene3DIndex::SetPos(pos);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCylinder::Uninit(void)
{
	CScene3DIndex::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCylinder::Update(void)
{
	// ��]������
	D3DXVECTOR3 rot = CScene3DIndex::GetRot();
	rot.y -= 0.0001f;
	CScene3DIndex::SetRot(rot);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCylinder::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	CCamera *pCamera = CManager::GetCamera();

	D3DXMATRIX projection = pCamera->GetProjection();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&projection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&projection,
		D3DXToRadian(45.0f),							// ��p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// �A�X�y�N�g��
		10.0f,											// �J�����͈̔́i��O�j
		10000.0f);										// �J�����͈̔́i���s�j

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &projection);

	CScene3DIndex::Draw();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&projection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&projection,
		D3DXToRadian(45.0f),							// ��p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// �A�X�y�N�g��
		10.0f,											// �J�����͈̔́i��O�j
		8000.0f);										// �J�����͈̔́i���s�j

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &projection);
}
