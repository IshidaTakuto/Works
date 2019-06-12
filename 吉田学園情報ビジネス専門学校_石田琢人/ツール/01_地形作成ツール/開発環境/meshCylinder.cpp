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
#define TEXTURE_MESHCYLINDER		"data/TEXTURE/circle.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MESH_HORIZONTAL	(20)			// ���������̕�����
#define MESH_VERTICAL	(1)				// ���������̕�����
#define CYLINDER_HEIGHT	(200.0f)		// ����

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
CCylinder* CCylinder::Create(D3DXVECTOR3 pos)
{
	CCylinder *pCylinder;

	pCylinder = new CCylinder;	// ���������m��
	if (pCylinder != NULL)
	{// �������m�ۂł���
		pCylinder->Init(pos);
		pCylinder->BindTexture(m_pTexture[0]);
	}

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
	m_fRange = 0.0f;
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
HRESULT CCylinder::Init(D3DXVECTOR3 pos)
{
	CScene3DIndex::Init();
	CScene::SetObjType(CScene::OBJTYPE_CYLINDER);
	CScene3DIndex::MakeVertex();
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
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshYZ = 0; nCntMeshYZ <= MESH_VERTICAL; nCntMeshYZ++)
	{
		for (int nCntMeshX = 0; nCntMeshX <= MESH_HORIZONTAL; nCntMeshX++)
		{

			float fAngle = 1.0f / (MESH_HORIZONTAL / 2);	// �p�x

			pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * m_fRange;		// �����
			pVtx[0].pos.y = CYLINDER_HEIGHT - (nCntMeshYZ * CYLINDER_HEIGHT) * 1.0f;		// Y�������ւ��炷
			pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * m_fRange;		// �����
		}
	}

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();
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
