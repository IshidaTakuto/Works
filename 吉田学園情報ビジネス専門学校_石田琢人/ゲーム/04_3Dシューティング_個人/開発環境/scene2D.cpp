//=============================================================================
//
// 2D�I�u�W�F�N�g���� [scene2D.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//==================================
// �}�N����`
//==================================
#define SIZE_X	(50.0f)	// �|���S���̉��̒���
#define SIZE_Y	(50.0f)	// �|���S���̏c�̒���

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//=========================================
// �e�N�X�`�������蓖�Ă�
//=========================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̈ʒu
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene2D::~CScene2D()
{
}

//=========================================
// �|���S���̏���������
//=========================================
HRESULT CScene2D::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//// ���_�̊p�x
	//m_fAngle = atan2f(m_pos.x - (m_pos.x + SIZE_X), m_pos.y - (m_pos.y + SIZE_Y));

	//// ���S���璸�_�܂ł̋���
	//m_fLength = sqrtf((SIZE_X * SIZE_X) + (SIZE_Y * SIZE_Y));

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

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - SIZE_X, m_pos.y - SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + SIZE_X, m_pos.y - SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - SIZE_X, m_pos.y + SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + SIZE_X, m_pos.y + SIZE_Y, 0.0f);

	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// �|���S���̏I������
//=========================================
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (NULL != m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//=========================================
// �|���S���̍X�V����
//=========================================
void CScene2D::Update(void)
{
#if 0	// �|���S���̉�]
	m_fCntRot += 0.01f;		// ��]�̃J�E���^

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = (m_pos.x) - sinf(m_fAngle + m_fCntRot) * m_fLength;
	pVtx[0].pos.y = (m_pos.y) - cosf(m_fAngle + m_fCntRot) * m_fLength;
	pVtx[1].pos.x = (m_pos.x) + sinf(m_fAngle - m_fCntRot) * m_fLength;
	pVtx[1].pos.y = (m_pos.y) - cosf(m_fAngle - m_fCntRot) * m_fLength;
	pVtx[2].pos.x = (m_pos.x) - sinf(m_fAngle - m_fCntRot) * m_fLength;
	pVtx[2].pos.y = (m_pos.y) + cosf(m_fAngle - m_fCntRot) * m_fLength;
	pVtx[3].pos.x = (m_pos.x) + sinf(m_fAngle + m_fCntRot) * m_fLength;
	pVtx[3].pos.y = (m_pos.y) + cosf(m_fAngle + m_fCntRot) * m_fLength;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
#endif
}

//=========================================
// �|���S���̕`�揈��
//=========================================
void CScene2D::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=========================================================================================================================
// �����蔻��
//=========================================================================================================================
bool CScene2D::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	if (this != NULL)
	{
		if (pos.x - fRange < m_pos.x
			&& pos.x + fRange > m_pos.x
			&& pos.y - fRange < m_pos.y
			&& pos.y + fRange > m_pos.y)
		{// ����Ǝ����̔���
			if (false == bHit)
			{// �U�������L���ɂ���
				bHit = true;
			}
		}
	}
	return bHit;
}

//=========================================
// �ʒu�̐ݒ�
//=========================================
const void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + m_fLength, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �T�C�Y�̐ݒ�
//=========================================
const void CScene2D::SetLength(float fLength)
{
	m_fLength = fLength;

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + m_fLength, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �e�N�X�`�����W�̐ݒ�
//=========================================
const void CScene2D::SetTexPos(D3DXVECTOR2 *tex)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = tex[0];
	pVtx[1].tex = tex[1];
	pVtx[2].tex = tex[2];
	pVtx[3].tex = tex[3];

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

}

//=========================================
// ���_���W�̐ݒ�
//=========================================
const void CScene2D::SetVtxPos(D3DXVECTOR3 *pos)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].pos = pos[0];
	pVtx[1].pos = pos[1];
	pVtx[2].pos = pos[2];
	pVtx[3].pos = pos[3];

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// ���C�t�̐ݒ�
//=========================================
const int CScene2D::SubtractLife(int nDamage)
{
	m_nLife -= nDamage;	// ���C�t������
	return m_nLife;		// �v�Z��̃��C�t��Ԃ�
};

//=========================================
// �F�̐ݒ�
//=========================================
const void CScene2D::SetColor(D3DXCOLOR col)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �F�̉��Z
//=========================================
const void CScene2D::AddColor(D3DXCOLOR col)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].col += col;
	pVtx[1].col += col;
	pVtx[2].col += col;
	pVtx[3].col += col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
