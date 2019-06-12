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
CNumber* CNumber::Create(D3DXVECTOR3 pos, float fLength)
{
	CNumber *pNumber;

	pNumber = new CNumber;	// ���������m��
	pNumber->Init(pos, fLength);

	return pNumber;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CNumber::Load(void)
{
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
	// �e�N�X�`���̔j��
	if (NULL != m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CNumber::CNumber()
{
	m_nCountAnim = 0;
	m_nPatternAnim = 0;
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
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fLength)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	// �|���S���̈ʒu��ݒ�
	m_pos = pos;
	m_fLength = fLength;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���W
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - (m_fLength * 1.3f), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - (m_fLength * 1.3f), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + (m_fLength * 1.3f), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + (m_fLength * 1.3f), 0.0f);

	// 1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CNumber::Uninit(void)
{
	if (m_pTexture != NULL)
	{// �e�N�X�`���̃|�C���^���J��
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
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
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// �����̐ݒ�
//=========================================
const void CNumber::SetNumber(int nNumber)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + nNumber * 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + nNumber * 0.1f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �ʒu�̐ݒ�
//=========================================
const void CNumber::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - m_fLength, 0.0f);;
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + m_fLength, 0.0f);;
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + m_fLength, 0.0f);;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �T�C�Y�̐ݒ�
//=========================================
const void CNumber::SetLength(float fLength)
{
	m_fLength = fLength;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - m_fLength, 0.0f);;
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + m_fLength, 0.0f);;
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + m_fLength, 0.0f);;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �F�̐ݒ�
//=========================================
const void CNumber::SetColor(D3DXCOLOR col)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
