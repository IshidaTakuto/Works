//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "bg.h"
#include "renderer.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_TEXTURE00	"data/TEXTURE/sky.jpg"		// �w�i�̃e�N�X�`����
#define BG_TEXTURE01	"data/TEXTURE/cloud.jpg"	// �w�i�̃e�N�X�`����
#define BG_TEXTURE02	"data/TEXTURE/mountain000.png"	// �w�i�̃e�N�X�`����

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9 CBg::m_pTexture[MAX_BG] = {};
int CBg::m_nPriority = 0;

//==================================
// ��������
//==================================
CBg* CBg::Create(int nTexture, bool bAdd, int nPriority)
{
	CBg *pBg;

	// �D��ԍ��̐ݒ�
	m_nPriority = nPriority;

	pBg = new CBg;	// ���������m��

	if (pBg != NULL)
	{// �������m�ې���
		pBg->Init();								// ����������
		pBg->BindTexture(m_pTexture[nTexture]);		// �e�N�X�`���̐ݒ�
		pBg->m_bAdd = bAdd;							// ���Z�������邩�ǂ���
	}

	return pBg;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CBg::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE01, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE02, &m_pTexture[2]);

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CBg::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_BG; nCntTex++)
	{
		if (NULL != m_pTexture[nCntTex])
		{// �e�N�X�`���̔j��
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CBg::CBg(CScene::OBJTYPE objType) : CScene2D(m_nPriority, objType)
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CBg::~CBg()
{
}

//=========================================
// ����������
//=========================================
HRESULT CBg::Init()
{
	CScene2D::Init();

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene2D::GetVtxBuff();

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// ���_�o�b�t�@���A�����b�N
	vtxBuff->Unlock();

	// �|���S���̈ʒu��ݒ�
	CScene2D::SetVtxBuff(vtxBuff);

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CBg::Uninit(void)
{
	// ������j��
	CScene2D::Release();
}

//=========================================
// �X�V����
//=========================================
void CBg::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CBg::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	if (m_bAdd)
	{// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	CScene2D::Draw();

	if (m_bAdd)
	{// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=========================================
// �e�N�X�`�����W�ړ�
//=========================================
void CBg::TexMove(float fMove)
{
	CScene::SetObjType(CScene::OBJTYPE_FADE);		// �I�u�W�F�N�g�^�C�v���t�F�[�h�ɐݒ�

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene2D::GetVtxBuff();

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W���X�V
	pVtx[0].tex.x += fMove;
	pVtx[1].tex.x += fMove;
	pVtx[2].tex.x += fMove;
	pVtx[3].tex.x += fMove;

	// ���_�o�b�t�@���A�����b�N
	vtxBuff->Unlock();

	// �|���S���̈ʒu��ݒ�
	CScene2D::SetVtxBuff(vtxBuff);
}