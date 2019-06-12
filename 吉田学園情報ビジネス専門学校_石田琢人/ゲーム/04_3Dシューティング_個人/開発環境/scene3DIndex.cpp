//=============================================================================
//
// �C���f�b�N�X�o�b�t�@3D�I�u�W�F�N�g���� [scene3DIndex.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3DIndex.h"
#include "renderer.h"
#include "manager.h"

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
void CScene3DIndex::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene3DIndex::CScene3DIndex(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̈ʒu
	m_nLengthX = 0;
	m_nLengthY = 0;
	m_nLengthZ = 0;
	m_nMeshX = 0;
	m_nMeshY = 0;
	m_nMeshZ = 0;
	m_type = TYPE_NONE;
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene3DIndex::~CScene3DIndex()
{
}

//=========================================
// �|���S���̏���������
//=========================================
HRESULT CScene3DIndex::Init(void)
{
	return S_OK;
}

//=========================================
// �|���S���̏I������
//=========================================
void CScene3DIndex::Uninit(void)
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
void CScene3DIndex::Update(void)
{
}

//=========================================
// �|���S���̕`�揈��
//=========================================
void CScene3DIndex::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// ���[���h�}�g���b�N�X�̕ۑ��p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);
}

//=========================================================================================================================
// �����蔻��
//=========================================================================================================================
bool CScene3DIndex::Collision(D3DXVECTOR3 *pos, float fRadius)
{
	bool bLand = false;

	if (this != NULL)
	{
		if (pos->x - fRadius > m_pos.x - m_nLengthX
			&& pos->x + fRadius < m_pos.x + m_nLengthX
			&& pos->z - fRadius > m_pos.z - m_nLengthZ
			&& pos->z + fRadius < m_pos.z + m_nLengthZ)
		{// ����Ǝ����̔���
			if (false == bLand)
			{// �U�������L���ɂ���
				bLand = true;
				pos->y = m_pos.y;
			}
		}
	}
	return bLand;
}

//=========================================================================================================================
// ���_���̍쐬
//=========================================================================================================================
void CScene3DIndex::MakeVertex(int meshX, int meshY, int meshZ, int nLengthX, int nLengthY, int nLengthZ, TYPE type)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	int nCntMeshYZ;
	int nCntMeshX;
	int nCntIdx;

	m_nLengthX = nLengthX;
	m_nLengthY = nLengthY;
	m_nLengthZ = nLengthZ;

	m_nMeshX = meshX;
	m_nMeshY = meshY;
	m_nMeshZ = meshZ;

	m_type = type;

	m_nNumVtx = (meshX + 1) * ((meshZ + meshY) + 1);
	m_nNumPolygon = meshX * (meshZ + meshY) * 2 + (4 * ((meshZ + meshY) - 1));

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * (meshX * (meshZ + meshY) * 2 + (4 * ((meshZ + meshY) - 1)) + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshYZ = 0; nCntMeshYZ <= (meshZ + meshY); nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX <= meshX; nCntMeshX++)
		{
			// ���_�̍��W
			switch(type)
			{
			case TYPE_FIELD:		// �n��
				pVtx[0].pos = D3DXVECTOR3(((nLengthX / 2.0f) - ((nLengthX * nCntMeshX) / meshX)) * 1.0f, 0.0f, ((nLengthZ * nCntMeshYZ) / meshZ) * 1.0f);
				break;

			case TYPE_WALL:			// ��
				pVtx[0].pos = D3DXVECTOR3((nLengthX / 2.0f) - ((nLengthX * nCntMeshX) / meshX), (nLengthY * nCntMeshYZ * 1.0f) / meshY, 0);
				break;

			case TYPE_CYLINDER:		// �V�����_�[
				float fAngle = 1.0f / (meshX / 2);	// �p�x

				pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * nLengthX;		// �����
				pVtx[0].pos.y = nLengthY - (nCntMeshYZ * nLengthY) * 1.0f;								// Y�������ւ��炷
				pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * nLengthZ;		// �����
				break;
			}

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshYZ * 1.0f);

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	if (TYPE_CYLINDER == type)
	{// �V�����_�[�̂Ƃ�
		for (int nCntMeshH = 0; nCntMeshH <= meshX; nCntMeshH++)
		{
			pIdx[0] = (meshX + 1) + nCntMeshH;
			pIdx[1] = 0 + nCntMeshH;

			pIdx += 2;
		}
	}
	else
	{// �n�ʂƕ�
		for (nCntMeshYZ = 0, nCntIdx = 0; nCntMeshYZ < (meshZ + meshY); nCntMeshYZ++)
		{
			for (nCntMeshX = 0; nCntMeshX <= meshX; nCntMeshX++, nCntIdx++)
			{
				pIdx[0] = (meshX + 1) + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				pIdx += 2;

				if (nCntMeshYZ < (meshZ + meshY) - 1 && nCntMeshX == meshX)
				{// �܂�Ԃ���
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = (meshX + 1) + nCntIdx + 1;

					pIdx += 2;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
}

//=========================================================================================================================
// �ʒu�̐ݒ�
//=========================================================================================================================
const void CScene3DIndex::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshYZ = 0; nCntMeshYZ <= (m_nMeshZ + m_nMeshY); nCntMeshYZ++)
	{
		for (int nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
		{
			// ���_�̍��W
			switch (m_type)
			{
			case TYPE_FIELD:		// �n��
				pVtx[0].pos = D3DXVECTOR3(((m_nLengthX / 2.0f) - ((m_nLengthX * nCntMeshX) / m_nMeshX)) * 1.0f, 0.0f, ((m_nLengthZ * nCntMeshYZ) / m_nMeshZ) * 1.0f) + m_pos;
				break;

			case TYPE_WALL:			// ��
				pVtx[0].pos = D3DXVECTOR3((m_nLengthX / 2.0f) - ((m_nLengthX * nCntMeshX) / m_nMeshX), (m_nLengthY * nCntMeshYZ * 1.0f) / m_nMeshY, 0) + m_pos;
				break;

			case TYPE_CYLINDER:		// �V�����_�[
				float fAngle = 1.0f / (m_nMeshX / 2);	// �p�x

				pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * m_nLengthX;		// �����
				pVtx[0].pos.y = m_nLengthY - (nCntMeshYZ * m_nLengthY) + m_pos.y;				// Y�������ւ��炷
				pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * m_nLengthZ;		// �����
				break;
			}

			pVtx++;
		}
	}
}
