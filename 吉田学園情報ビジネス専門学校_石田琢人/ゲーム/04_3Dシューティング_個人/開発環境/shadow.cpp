//=============================================================================
//
// �e���� [shadow.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_SHADOW	"data/TEXTURE/shadow001.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define SHADOW_WIDTH	(10.0f)		// x���̑傫��
#define SHADOW_DEPTH	(10.0f)		// z���̑傫��
#define MAX_SHADOW		(900)		// �e�̍ő吔

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CShadow::m_pTexture[NUM_SHADOW_TEXTURE] = {};

//==================================
// ��������
//==================================
CShadow *CShadow::Create(D3DXVECTOR3 pos)
{
	CShadow *pShadow;

	pShadow = new CShadow;	// ���������m��

	if (pShadow != NULL)
	{// �������m�ې���
		pShadow->Init(pos);
		pShadow->BindTexture(m_pTexture[0]);
	}

	return pShadow;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CShadow::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SHADOW, &m_pTexture[0]);

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CShadow::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < NUM_SHADOW_TEXTURE; nCntTexture++)
	{
		if (NULL != m_pTexture[nCntTexture])
		{// �e�N�X�`���̔j��
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CShadow::CShadow(int nPriority) : CScene3D(nPriority)
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CShadow::~CShadow()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos)
{
	CScene3D::Init();		// ����������
	CScene3D::SetPos(D3DXVECTOR3(pos.x, 0.001f, pos.z));		// �ʒu�̐ݒ�
	CScene3D::SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));	// �ʒu�̐ݒ�
	CScene3D::SetLength(D3DXVECTOR3(30.0f, 0.0f, 30.0f));
	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
	CScene::SetObjType(CScene::OBJTYPE_SHADOW);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShadow::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CShadow::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CShadow::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CScene3D::Draw();		// �`��

	// Z�o�b�t�@�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// �j��
//=============================================================================
void CShadow::Delelte(void)
{
	Uninit();
}