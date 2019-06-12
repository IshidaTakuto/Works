//=============================================================================
//
// �|���S������ [polygon.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "polygon.h"
#include "manager.h"
#include "renderer.h"

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9 CPolygon::m_pTexture[TYPE_MAX] = {};

//==================================
// ��������
//==================================
CPolygon* CPolygon::Create(int nType, D3DXVECTOR3 pos, float fWidth, float fHeight, int nTexType)
{
	CPolygon *pPolygon;

	pPolygon = new CPolygon;	// ���������m��
	pPolygon->Init(pos, fWidth, fHeight, nType);	// ����������
	pPolygon->m_nTexType = nTexType;				// �e�N�X�`���̎��

	return pPolygon;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CPolygon::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	char aTexName[TYPE_MAX][128] =				// �e�N�X�`���̖��O
	{
		"data/TEXTURE/press_any.png",			// PRESS_ANY
		"data/TEXTURE/continue.png",			// �|�[�Y���̃R���e�j���[
		"data/TEXTURE/retry.png",			// �|�[�Y���̃��g���C
		"data/TEXTURE/quit.png",			// �|�[�Y���̃^�C�g���ɖ߂�
		"data/TEXTURE/title_logo.png",			// �^�C�g���̃��S
		"data/TEXTURE/clear_logo.png",			// �Q�[���N���A�̃��S
		"data/TEXTURE/over_logo.png",			// �Q�[���I�[�o�[�̃��S
		"data/TEXTURE/ranking_logo.png",		// �����L���O�̃��S
		"data/TEXTURE/communicate_window.jpg",	// �ʐM�E�B���h�E
		"data/TEXTURE/stock00.png",				// �X�g�b�N�A�C�R��
		"data/TEXTURE/stock01.png",				// �X�g�b�N�A�C�R��
		"data/TEXTURE/stock02.png",				// �X�g�b�N�A�C�R��
	};

	// �e�N�X�`���̐���
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, aTexName[nCntTex], &m_pTexture[nCntTex]);
	}

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CPolygon::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
	{
		if (NULL != m_pTexture[nCntPolygon])
		{
			m_pTexture[nCntPolygon]->Release();
			m_pTexture[nCntPolygon] = NULL;
		}
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CPolygon::CPolygon()
{
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nTexType = 0;
	m_nCntColor = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CPolygon::~CPolygon()
{
}

//=========================================
// ����������
//=========================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	// �|���S���̐ݒ�
	m_pos = pos;				// �ʒu
	m_fWidth = fWidth;			// ��
	m_fHeight = fHeight;		// ����
	m_nType = nType;			// �����̎��
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F�̏����l

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

	// ���_���W
	if (nType == 0)
	{// �^�񒆂����S
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}
	else if (nType == 1)
	{// ���オ���S
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CPolygon::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{// �e�N�X�`���̃|�C���^���J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=========================================
// �X�V����
//=========================================
void CPolygon::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CPolygon::Draw(void)
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
	if (m_nTexType == -1)
	{
		pDevice->SetTexture(0, NULL);
	}
	else
	{
		pDevice->SetTexture(0, m_pTexture[m_nTexType]);
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// �ʒu�̐ݒ�
//=========================================
const void CPolygon::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	if (m_nType == 0)
	{// �^�񒆂����S
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}
	else if (m_nType == 1)
	{// ���オ���S
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �w���T�C�Y�̐ݒ�
//=========================================
const void CPolygon::SetWidth(float fWidth)
{
	m_fWidth = fWidth;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	if (m_nType == 0)
	{// �^�񒆂����S
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}
	else if (m_nType == 1)
	{// ���オ���S
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �x���T�C�Y�̐ݒ�
//=========================================
const void CPolygon::SetHeight(float fHeight)
{
	m_fHeight = fHeight;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	if (m_nType == 0)
	{// �^�񒆂����S
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}
	else if (m_nType == 1)
	{// ���オ���S
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �F�̐ݒ�
//=========================================
const void CPolygon::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �F�̉��Z
//=========================================
const void CPolygon::AddColor(D3DXCOLOR col)
{
	m_col += col;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �_�ŏ���
//=========================================
void CPolygon::Flashing(int nFlame)
{
	// �w�肵���t���[����
	if (m_nCntColor < nFlame)
	{// �����ɂ���
		m_col.a -= 1.0f / nFlame;
	}
	else
	{// �s�����ɂ���
		m_col.a += 1.0f / nFlame;
	}

	m_nCntColor = (1 + m_nCntColor) % (nFlame * 2);		// �J�E���g��i�߂�

	if (m_col.a <= 0)
	{// �A���t�@�l���O�ȉ��̂Ƃ�
		m_col.a = 0;
		m_nCntColor = nFlame;
	}
	else if (m_col.a >= 1)
	{// �A���t�@�l���P�ȏ�̂Ƃ�
		m_col.a = 1;
		m_nCntColor = 0;
	}

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}