//=============================================================================
//
// �������� [number.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"

//==================================
// �}�N����`
//==================================
#define TEXTURE_NAME	"data/TEXTURE/number000.png"

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//==================================
// ��������
//==================================
CNumber* CNumber::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CNumber *pNumber = NULL;

	pNumber = new CNumber;	// ���������m��

	if (pNumber)
	{// �������m�ې���
		pNumber->Init(pos, fWidth, fHeight);	// ����������
	}

	return pNumber;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CNumber::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CNumber::Unload(void)
{
	if (NULL != m_pTexture)
	{// �e�N�X�`���̔j��
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CNumber::CNumber(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_nNumber = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CNumber::~CNumber()
{
}

//=========================================
// ����������
//=========================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// ����������
	CScene2D::Init();

	// �e�N�X�`���̐ݒ�
	CScene2D::BindTexture(m_pTexture);

	// �ʒu�̐ݒ�
	CScene2D::SetPos(pos);

	// �T�C�Y�̐ݒ�
	CScene2D::SetSize(fWidth, fHeight);

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CNumber::Uninit(void)
{
	// �I������
	CScene2D::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CNumber::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CNumber::Draw(void)
{
	// �`�揈��
	CScene2D::Draw();
}

//=========================================
// �����̐ݒ�
//=========================================
const void CNumber::SetNumber(int nNumber)
{
	// �l�̕ۑ�
	m_nNumber = nNumber;

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene2D::GetVtxBuff();

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	vtxBuff->Unlock();
}

//=========================================
// �T�C�Y�̎擾
//=========================================
void CNumber::GetSize(float *pWidth, float *pHeight)
{
	if (pWidth)
	{// NULL�łȂ��Ƃ��l������
		*pWidth = CScene2D::GetWidth();
	}

	if (pHeight)
	{// NULL�łȂ��Ƃ��l������
		*pHeight = CScene2D::GetHeight();
	}
}
