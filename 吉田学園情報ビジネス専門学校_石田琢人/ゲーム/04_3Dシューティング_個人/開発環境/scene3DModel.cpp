//=============================================================================
//
// 3D���f���I�u�W�F�N�g���� [scene3DModel.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3DModel.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "light.h"

//==================================
// �}�N����`
//==================================

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//=========================================
// ���f���������蓖�Ă�
//=========================================
const void CScene3DModel::BindInfo(LPDIRECT3DTEXTURE9 pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat)
{
	m_pTexture = pTexture;
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = nNumMat;
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene3DModel::CScene3DModel(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pBuffMat = NULL;		// �}�e���A���ւ̃|�C���^
	m_pMesh = NULL;
	m_nIdxShadow = 0;			// �e�̔ԍ�
	m_vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	m_vtxMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̈ʒu
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene3DModel::~CScene3DModel()
{
}

//=========================================
// �|���S���̏���������
//=========================================
HRESULT CScene3DModel::Init(void)
{
	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

	// X�̒l�̔�r
	if (m_vtxMin.x > vtx.x)
	{// �ŏ��l�Ɣ�r
		m_vtxMin.x = vtx.x;	// �ŏ��l��菬������Α��
	}

	if (m_vtxMax.x < vtx.x)
	{// �ő�l�Ɣ�r
		m_vtxMax.x = vtx.x;	// �ő�l���傫����Α��
	}

	// Y�̒l�̔�r
	if (m_vtxMin.y > vtx.y)
	{// �ŏ��l�Ɣ�r
		m_vtxMin.y = vtx.y;	// �ŏ��l��菬������Α��
	}

	if (m_vtxMax.y < vtx.y)
	{// �ő�l�Ɣ�r
		m_vtxMax.y = vtx.y;	// �ő�l���傫����Α��
	}

	// Z�̒l�̔�r
	if (m_vtxMin.z > vtx.z)
	{// �ŏ��l�Ɣ�r
		m_vtxMin.z = vtx.z;	// �ŏ��l��菬������Α��
	}

	if (m_vtxMax.z < vtx.z)
	{// �ő�l�Ɣ�r
		m_vtxMax.z = vtx.z;	// �ő�l���傫����Α��
	}

	// ���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=========================================
// �|���S���̏I������
//=========================================
void CScene3DModel::Uninit(void)
{
	Release();
}

//=========================================
// �|���S���̍X�V����
//=========================================
void CScene3DModel::Update(void)
{
}

//=========================================
// �|���S���̕`�揈��
//=========================================
void CScene3DModel::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

		//�I�u�W�F�N�g�̃e�N�X�`���[���X�e�[�W�O�ɃZ�b�g����
		pDevice->SetTexture(0, m_pTexture);

		// ���f���̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=========================================================================================================================
// �����蔻��
//=========================================================================================================================
bool CScene3DModel::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange)
{
	bool bTouch = false;

	float fLengthOneself;		// ����̉~��͈�
	float fRange0 = fRange;		// ����͈̔͂̔��a�̒���

	fLengthOneself = powf(fRange0, 2);		// �����XZ�͈̔�

	float fLengthX = pPos->x - m_mtxWorld._41;	// X�̍�
	float fLengthZ = pPos->z - m_mtxWorld._43;	// Z�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);		// XZ�̍��̓��

	if (fLengthOneself >= fLengthTotal)
	{// ����Ǝ����̔���
		*pPos = *pPosold;							// �ʒu��߂�
		*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ��ʂ�0�ɂ���

		bTouch = true;
	}
	return bTouch;
}

//=========================================================================================================================
// �U���̓����蔻��
//=========================================================================================================================
bool CScene3DModel::CollisionAttack(D3DXVECTOR3 pos, float fRange)
{
	bool bAttack = false;

	float fLengthOneself;		// ����̉~��͈�
	float fRange0 = fRange;		// ����͈̔͂̔��a�̒���

	fLengthOneself = powf(fRange0, 2);		// �����XZ�͈̔�

	float fLengthX = pos.x - m_mtxWorld._41;	// X�̍�
	float fLengthZ = pos.z - m_mtxWorld._43;	// Z�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);		// XZ�̍��̓��

	if (pos.y + fRange >= m_mtxWorld._42 + (fLengthTotal / 2) && pos.y - fRange <= m_mtxWorld._42 - (fLengthTotal / 2))
	{
		if (fLengthOneself >= fLengthTotal)
		{// ����Ǝ����̔���
			bAttack = true;
		}
	}
	return bAttack;
}

//=========================================
// �ʒu�̐ݒ�
//=========================================
const void CScene3DModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=========================================
// ���C�t�̐ݒ�
//=========================================
const int CScene3DModel::SubtractLife(int nDamage)
{
	m_nLife -= nDamage;	// ���C�t������
	return m_nLife;		// �v�Z��̃��C�t��Ԃ�
};

//=========================================
// �F�̐ݒ�
//=========================================
const void CScene3DModel::SetColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �F��t����
	m_pMat->MatD3D.Diffuse = col;
	m_pMat->MatD3D.Ambient = col;

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=========================================
// �F�̉��Z
//=========================================
const void CScene3DModel::AddColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
									// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �F��t����
	m_pMat->MatD3D.Diffuse.r += col.r;
	m_pMat->MatD3D.Diffuse.g += col.g;
	m_pMat->MatD3D.Diffuse.b += col.b;
	m_pMat->MatD3D.Diffuse.a += col.a;

	m_pMat->MatD3D.Ambient.r += col.r;
	m_pMat->MatD3D.Ambient.g += col.g;
	m_pMat->MatD3D.Ambient.b += col.b;
	m_pMat->MatD3D.Ambient.a += col.a;

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}
