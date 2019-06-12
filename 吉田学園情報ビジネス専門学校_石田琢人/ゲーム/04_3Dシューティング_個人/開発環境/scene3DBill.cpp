//=============================================================================
//
// 3D�r���{�[�h�I�u�W�F�N�g���� [scene3DBill.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3DBill.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "game.h"
#include "player.h"

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
void CScene3DBill::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene3DBill::CScene3DBill(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �|���S���̈ʒu
	m_fLengthX = 0.0f;							// X���̑傫��
	m_fLengthY = 0.0f;							// Z���̑傫��
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene3DBill::~CScene3DBill()
{
}

//=========================================
// �|���S���̏���������
//=========================================
HRESULT CScene3DBill::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(SIZE_X, SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SIZE_X, SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SIZE_X, SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SIZE_X, SIZE_Y, 0.0f);

	// �@�����J�����̈ʒu�ŕς���
	CPlayer *pPlayer = CGame::GetPlayer();	// �v���C���[�̏����擾
	if (NULL != pPlayer)
	{// �v���C���[�̏��̎擾
		CPlayer::TYPE type = pPlayer->GetType();
		if (type == CPlayer::TYPE_AIRCRAFT || type == CPlayer::TYPE_TANK)
		{// �퓬�@
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
		else if (type == CPlayer::TYPE_SHIP)
		{// ���
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
	}
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
// �|���S���̏I������
//=========================================
void CScene3DBill::Uninit(void)
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
void CScene3DBill::Update(void)
{
}

//=========================================
// �|���S���̕`�揈��
//=========================================
void CScene3DBill::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//�t�H�O���g�p����
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	D3DXMATRIX mtxTrans, mtxView;	// ���[���h�}�g���b�N�X�̕ۑ��p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�t�H�O���g�p����
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================================================================================================
// �����蔻��
//=========================================================================================================================
bool CScene3DBill::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	if (this != NULL)
	{
		float fLengthOneself;
		float fRange0 = fRange;

		fLengthOneself = powf(fRange0 * 3, 2);		// �����XYZ�͈̔�

		float fLengthX = pos.x - m_mtxWorld._41;	// X�̍�
		float fLengthY = pos.y - m_mtxWorld._42;	// Y�̍�
		float fLengthZ = pos.z - m_mtxWorld._43;	// Z�̍�
		float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);			// XYZ�̓��̍��v

		if (fLengthOneself >= fLengthTotal)
		{// ����Ǝ����̔���
			if (false == bHit)
			{// �����L���ɂ���
				bHit = true;
			}
		}
	}
	return bHit;
}

//=========================================================================================================================
// �����蔻��(XY)
//=========================================================================================================================
bool CScene3DBill::CollisionXY(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	if (this != NULL)
	{
		float fLengthOneself;
		float fRange0 = fRange;

		fLengthOneself = powf(fRange0 * 2, 2);		// �����XY�͈̔�

		float fLengthX = pos.x - m_mtxWorld._41;	// X�̍�
		float fLengthY = pos.y - m_mtxWorld._42;	// Y�̍�
		float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);			// XY�̍��̓��

		if (fLengthOneself >= fLengthTotal)
		{// ����Ǝ����̔���
			if (false == bHit)
			{// �����L���ɂ���
				bHit = true;
			}
		}
	}
	return bHit;
}

//=========================================================================================================================
// �����蔻��(XZ)
//=========================================================================================================================
bool CScene3DBill::CollisionXZ(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	if (this != NULL)
	{
		float fLengthOneself;
		float fRange0 = fRange;

		fLengthOneself = powf(fRange0 * 2, 2);		// �����XZ�͈̔�

		float fLengthX = pos.x - m_mtxWorld._41;	// X�̍�
		float fLengthZ = pos.z - m_mtxWorld._43;	// Z�̍�
		float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);			// XZ�̍��̓��

		if (fLengthOneself >= fLengthTotal)
		{// ����Ǝ����̔���
			if (false == bHit)
			{// �����L���ɂ���
				bHit = true;
			}
		}
	}
	return bHit;
}

//=========================================================================================================================
// �����蔻��(Y)
//=========================================================================================================================
bool CScene3DBill::CollisionY(float posY, float fRange)
{
	bool bHit = false;	// �Ԃ��l

	if (this != NULL)
	{
		if (posY - fRange <= m_pos.y + m_fLengthY && posY + fRange >= m_pos.y - m_fLengthY)
		{// Y�����͈͓�
			if (false == bHit)
			{// �����L���ɂ���
				bHit = true;
			}
		}
	}
	return bHit;
}

//=========================================
// �ʒu�̐ݒ�
//=========================================
const void CScene3DBill::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=========================================
// ���C�t�̐ݒ�
//=========================================
const int CScene3DBill::SubtractLife(int nDamage)
{
	m_nLife -= nDamage;	// ���C�t������
	return m_nLife;		// �v�Z��̃��C�t��Ԃ�
};

//=========================================
// �F�̐ݒ�
//=========================================
const void CScene3DBill::SetColor(D3DXCOLOR col)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

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
const void CScene3DBill::AddColor(D3DXCOLOR col)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

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

//=========================================
// �傫���̐ݒ�
//=========================================
const void CScene3DBill::SetLength(float fLength0, float fLength1)
{
	m_fLengthX = fLength0;
	m_fLengthY = fLength1;

	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].pos = D3DXVECTOR3(-fLength0, fLength1,0.0f);
	pVtx[1].pos = D3DXVECTOR3(fLength0, fLength1, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fLength0, -fLength1, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fLength0, -fLength1, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// ���_���W�̐ݒ�
//=========================================
const void CScene3DBill::SetVtxPos(D3DXVECTOR3 *vtxPos)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].pos = vtxPos[0];
	pVtx[1].pos = vtxPos[1];
	pVtx[2].pos = vtxPos[2];
	pVtx[3].pos = vtxPos[3];

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �e�N�X�`�����W�̐ݒ�
//=========================================
const void CScene3DBill::SetTexPos(D3DXVECTOR2 *tex)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

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
