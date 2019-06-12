//=============================================================================
//
// �C���f�b�N�X3D�I�u�W�F�N�g���� [scene3DIndex.h]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "scene3DIndex.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "GUIManager.h"

#include <stdio.h>

//==================================
// �}�N����`
//==================================
#define BIN_SAVE_FILENAME	"data/TEXT/FIELD/field_save.bin"
#define TEXT_SAVE_FILENAME	"data/TEXT/FIELD/field_save.txt"

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

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �|���S���̈ʒu
	m_fLengthX = 0;								// ��̃u���b�N�̕�
	m_fLengthY = 0;								// ��̃u���b�N�̍���
	m_fLengthZ = 0;								// ��̃u���b�N�̉��s
	m_nSplitX = 0;								// X���̕�����
	m_nSplitY = 0;								// Y���̕�����
	m_nSplitZ = 0;								// Z���̕�����
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene3DIndex::~CScene3DIndex()
{
}

//=========================================
// ����������
//=========================================
HRESULT CScene3DIndex::Init(void)
{
	// �t�@�C���ǂݍ���
	Load();

	// �����̐ݒ�
	CGuiManager::SetLength(m_fLengthX, m_fLengthZ);

	// �������̐ݒ�
	CGuiManager::SetSplit(m_nSplitX, m_nSplitZ);

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CScene3DIndex::Uninit(void)
{
	if (NULL != m_pVtxBuff)
	{// ���_�o�b�t�@�̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (NULL != m_pIdxBuff)
	{// �C���f�b�N�X�o�b�t�@�̔j��
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// ���S�t���O
	Release();
}

//=========================================
// �X�V����
//=========================================
void CScene3DIndex::Update(void)
{
	float fLengthX = 0.0f;
	float fLengthZ = 0.0f;
	int nMeshX = 0;
	int nMeshZ = 0;

	// �L�[�{�[�h�̓��͂��擾
	CInputKeyboard *pKeyboard = CManager::GetInput();

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pKeyboard->GetTrigger(DIK_F5))
	{// �@���̍Čv�Z
		SetNor(pVtx);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	if (pKeyboard->GetTrigger(DIK_F9))
	{// �t�@�C���ɏo��
		Save();
	}

	if (pKeyboard->GetTrigger(DIK_F3))
	{// �t�@�C������ǂݍ���
		if (NULL != m_pVtxBuff)
		{// ���_�o�b�t�@�̔j��
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}

		if (NULL != m_pIdxBuff)
		{// �C���f�b�N�X�o�b�t�@�̔j��
			m_pIdxBuff->Release();
			m_pIdxBuff = NULL;
		}

		// �����t�@�C������ǂݍ���
		Load();

		// ���_���̍쐬
		MakeVertex();

		// ���_�����t�@�C������ǂݍ���
		LoadVtx();
	}

	// �������̐ݒ�
	nMeshX = CGuiManager::GetSplit(0);
	if (nMeshX != m_nSplitX) { SetSplitX(nMeshX); }

	nMeshZ = CGuiManager::GetSplit(1);
	if (nMeshZ != m_nSplitZ) { SetSplitZ(nMeshZ); }

	// �����̐ݒ�
	fLengthX = CGuiManager::GetLength(0);
	if (fLengthX != m_fLengthX) { SetLengthX(fLengthX); }

	fLengthZ = CGuiManager::GetLength(1);
	if (fLengthZ != m_fLengthZ)
	{
		SetLengthZ(fLengthZ);
	}
}

//=========================================
// �`�揈��
//=========================================
void CScene3DIndex::Draw(void)
{
	// �f�o�C�X�̎擾
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

//=========================================
// X�̕������̕ύX
//=========================================
void CScene3DIndex::SetSplitX(int nValue)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	if ((m_nSplitX > 1 || nValue > 0) && m_nSplitX + nValue > 0)
	{// �P���傫�����̂�
		VERTEX_3D *pVtx;					// ���_���̃|�C���^
		VERTEX_3D *pVtxInfo;				// ���_���̕ۑ�
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// �ۑ��p���_�o�b�t�@
		int nCntAdd = 0;					// ���_�J�E���g�p�J�E���^
		int nCntMeshX, nCntMeshZ, nCntVtx;	// for���p�J�E���^

		// ���_�o�b�t�@�𐶐�
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (((m_nSplitX + 1) + nValue) * (m_nSplitZ + 1)),
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pVtxBuff,
			NULL);

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �ۑ��p���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtxInfo, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nSplitZ; nCntMeshZ++)
		{// Z�̕����������[�v
			for (nCntMeshX = 0; nCntMeshX <= m_nSplitX; nCntMeshX++, nCntVtx++)
			{// X�̕����������[�v
				if (nValue < 0)
				{// �����������炷�Ƃ�
					if (nCntMeshX != m_nSplitX)
					{// X���������̂Ƃ��ȊO��ۑ�
						pVtxInfo[nCntAdd] = pVtx[nCntVtx];
						nCntAdd++;
					}
				}
				else
				{// �������𑝂₷�Ƃ�
					pVtxInfo[nCntVtx + nCntAdd] = pVtx[nCntVtx];

					if (nCntMeshX == m_nSplitX)
					{// X���������̂Ƃ��ȊO��ۑ�
						for (int nCount = 0; nCount < nValue; nCount++)
						{// X���������ȏ���ۑ�
							pVtxInfo[nCntVtx + nCntAdd].pos = pVtx[nCntVtx].pos;
							pVtxInfo[nCntVtx + nCntAdd].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
							pVtxInfo[nCntVtx + nCntAdd].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
							pVtxInfo[nCntVtx + nCntAdd].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshZ * 1.0f);

							nCntAdd++;
						}
					}
				}
			}
		}

		// �ۑ��p���_�o�b�t�@���A�����b�N
		pVtxBuff->Unlock();

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		if (m_pVtxBuff != NULL)
		{// ���_�o�b�t�@���J��
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}

		if (m_pIdxBuff != NULL)
		{// �C���f�b�N�X�o�b�t�@���J��
			m_pIdxBuff->Release();
			m_pIdxBuff = NULL;
		}

		// ������
		m_nSplitX = nValue;

		if (m_nSplitX < 1)
		{// X�̕��������P��菬�����Ȃ���
			m_nSplitX = 1;
		}

		// ���_���̍쐬
		MakeVertex();

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �ۑ��p���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtxInfo, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nSplitZ; nCntMeshZ++)
		{// Z�̕����������[�v
			for (nCntMeshX = 0; nCntMeshX <= m_nSplitX; nCntMeshX++, nCntVtx++)
			{// X�̕����������[�v
				pVtx[nCntVtx].pos = pVtxInfo[nCntVtx].pos;

				pVtx[nCntVtx].pos.x = -(m_fLengthX * m_nSplitX * 0.5f) + (m_fLengthX * nCntMeshX) * 1.0f;
				pVtx[nCntVtx].pos.z = (m_fLengthZ * m_nSplitZ * 0.5f) - (m_fLengthZ * nCntMeshZ) * 1.0f;
			}
		}

		// �@���̍Čv�Z
		SetNor(pVtx);

		// �ۑ��p���_�o�b�t�@���A�����b�N
		pVtxBuff->Unlock();

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		if (pVtxBuff != NULL)
		{// �ۑ��p���_�o�b�t�@���J��
			pVtxBuff->Release();
			pVtxBuff = NULL;
		}
	}
}

//=========================================
// Z�̕������̕ύX
//=========================================
void CScene3DIndex::SetSplitZ(int nValue)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	if ((m_nSplitZ > 1 || nValue > 0) && m_nSplitZ + nValue > 0)
	{// �P���傫�����̂�
		VERTEX_3D *pVtx;					// ���_���̃|�C���^
		VERTEX_3D *pVtxInfo;				// ���_���̕ۑ�
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// �ۑ��p���_�o�b�t�@
		int nCntAdd = 0;					// ���_�J�E���g�p�J�E���^
		int nCntMeshX, nCntMeshZ, nCntVtx;	// for���p�J�E���^

		// ���_�o�b�t�@�𐶐�
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nSplitX + 1) * (m_nSplitZ + 1 + nValue)),
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pVtxBuff,
			NULL);

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �ۑ��p���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtxInfo, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nSplitZ; nCntMeshZ++)
		{// Z�̕����������[�v
			for (nCntMeshX = 0; nCntMeshX <= m_nSplitX; nCntMeshX++, nCntVtx++)
			{// X�̕����������[�v
				if (nValue < 0)
				{// �����������炷�Ƃ�
					if (nCntMeshZ != m_nSplitZ)
					{// X���������̂Ƃ��ȊO��ۑ�
						pVtxInfo[nCntAdd] = pVtx[nCntVtx];
						nCntAdd++;
					}
				}
				else
				{// �������𑝂₷�Ƃ�
					pVtxInfo[nCntVtx + nCntAdd] = pVtx[nCntVtx];

					if (nCntMeshZ == m_nSplitZ)
					{// X���������̂Ƃ��ȊO��ۑ�
						for (int nCount = 0; nCount < nValue; nCount++)
						{// X���������ȏ���ۑ�
							pVtxInfo[nCntVtx + nCntAdd].pos = pVtx[nCntVtx].pos;
							pVtxInfo[nCntVtx + nCntAdd].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
							pVtxInfo[nCntVtx + nCntAdd].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
							pVtxInfo[nCntVtx + nCntAdd].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshZ * 1.0f);

							nCntAdd++;
						}
					}
				}
			}
		}

		// �ۑ��p���_�o�b�t�@���A�����b�N
		pVtxBuff->Unlock();

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		if (m_pVtxBuff != NULL)
		{// ���_�o�b�t�@���J��
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}

		if (m_pIdxBuff != NULL)
		{// �C���f�b�N�X�o�b�t�@���J��
			m_pIdxBuff->Release();
			m_pIdxBuff = NULL;
		}

		// ������
		m_nSplitZ = nValue;

		if (m_nSplitZ < 1)
		{// Z�̕��������P��菬�����Ȃ���
			m_nSplitZ = 1;
		}

		// ���_���̍쐬
		MakeVertex();

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �ۑ��p���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtxInfo, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nSplitZ; nCntMeshZ++)
		{// Z�̕����������[�v
			for (nCntMeshX = 0; nCntMeshX <= m_nSplitX; nCntMeshX++, nCntVtx++)
			{// X�̕����������[�v
				pVtx[nCntVtx].pos = pVtxInfo[nCntVtx].pos;

				pVtx[nCntVtx].pos.x = -(m_fLengthX * m_nSplitX * 0.5f) + (m_fLengthX * nCntMeshX) * 1.0f;
				pVtx[nCntVtx].pos.z = (m_fLengthZ * m_nSplitZ * 0.5f) - (m_fLengthZ * nCntMeshZ) * 1.0f;
			}
		}

		// �@���̍Čv�Z
		SetNor(pVtx);

		// �ۑ��p���_�o�b�t�@���A�����b�N
		pVtxBuff->Unlock();

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		if (pVtxBuff != NULL)
		{// �ۑ��p���_�o�b�t�@���J��
			pVtxBuff->Release();
			pVtxBuff = NULL;
		}
	}
}

//=========================================
// X�������̕ύX
//=========================================
void CScene3DIndex::SetLengthX(float fValue)
{
	if (m_fLengthX > 1 || fValue > 0)
	{// �P���傫�����̂�
		// �T�C�Y
		m_fLengthX = fValue;

		if (m_fLengthX < 5.0f)
		{// ������1��菬�����Ȃ����Ƃ�
			m_fLengthX = 5.0f;
		}

		int nCntMeshZ, nCntMeshX, nCntVtx;	// for���p�J�E���^
		VERTEX_3D *pVtx;	// ���_���̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nSplitZ; nCntMeshZ++)
		{// Z�̕����������[�v
			for (nCntMeshX = 0; nCntMeshX <= m_nSplitX; nCntMeshX++, nCntVtx++)
			{// X�̕����������[�v
				pVtx[nCntVtx].pos.x = -(m_fLengthX * m_nSplitX * 0.5f) + (m_fLengthX * nCntMeshX) * 1.0f;
			}
		}

		// �@���̍Čv�Z
		SetNor(pVtx);

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();
	}
}

//=========================================
// Z�������̕ύX
//=========================================
void CScene3DIndex::SetLengthZ(float fValue)
{
	if (m_fLengthZ > 1 || fValue > 0)
	{// �P���傫�����̂�
		// �T�C�Y
		m_fLengthZ = fValue;

		if (m_fLengthZ < 5.0f)
		{// ������1��菬�����Ȃ����Ƃ�
			m_fLengthZ = 5.0f;
		}

		int nCntMeshZ, nCntMeshX, nCntVtx;	// for���p�J�E���^
		VERTEX_3D *pVtx;	// ���_���̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nSplitZ; nCntMeshZ++)
		{// Z�̕����������[�v
			for (nCntMeshX = 0; nCntMeshX <= m_nSplitX; nCntMeshX++, nCntVtx++)
			{// X�̕����������[�v
				pVtx[nCntVtx].pos.z = (m_fLengthZ * m_nSplitZ * 0.5f) - (m_fLengthZ * nCntMeshZ) * 1.0f;
			}
		}

		// �@���̍Čv�Z
		SetNor(pVtx);

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();
	}
}

//=========================================
// �n�ʂ̍����̎擾
//=========================================
float CScene3DIndex::GetHeight(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;	// �Ԃ�����
	float fDiff = 0.0f;		// ����p��Y������������ۑ�
	int nPosBrock[2];		// �v���C���[�̂���|���S���̃u���b�N
	int nVtxNumber[4];		// �u���b�N�̒��_�ԍ�

	// �v���C���[�̃x�N�g�������߂�
	D3DXVECTOR3 vecPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecA, vecB, collisionNor, collisionPos[3];

	// �v���C���[�̂���|���S������
	nPosBrock[0] = (int)(pos.x + (m_fLengthX * m_nSplitX * 0.5f)) / (int)m_fLengthX;		// X���̃u���b�N����o��
	nPosBrock[1] = (int)(pos.z - (m_fLengthZ * m_nSplitZ * 0.5f)) / (int)m_fLengthZ * -1;	// Z���̃u���b�N����o��

	if (nPosBrock[0] >= m_nSplitX || (int)(pos.x + (m_fLengthX * m_nSplitX * 0.5f)) < 0 ||
		nPosBrock[1] >= m_nSplitZ || (int)(pos.z - (m_fLengthZ * m_nSplitZ * 0.5f)) > 0)
	{// �|���S���ɏ���Ă��Ȃ��Ƃ�
		return 0;
	}

	// ���_�ԍ����v�Z
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (m_nSplitX + 1));			// �����̃u���b�N�̍����ԍ�
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (m_nSplitX + 1));					// �����̃u���b�N�̍���ԍ�
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (m_nSplitX + 1));		// �����̃u���b�N�̉E���ԍ�
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (m_nSplitX + 1));			// �����̃u���b�N�̉E��ԍ�

	// �O�ςō��E�ǂ���̃|���S��������
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// ���_����ړI�̒��_�ւ̕���
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// �v���C���[�@�|�@vecA�̎n�܂�̒��_

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// ����
		// ����p���_���W
		collisionPos[0] = pVtx[nVtxNumber[0]].pos;
		collisionPos[1] = pVtx[nVtxNumber[1]].pos;
		collisionPos[2] = pVtx[nVtxNumber[2]].pos;
	}
	else
	{// �E��
		// ����p���_���W
		collisionPos[0] = pVtx[nVtxNumber[3]].pos;
		collisionPos[1] = pVtx[nVtxNumber[2]].pos;
		collisionPos[2] = pVtx[nVtxNumber[1]].pos;
	}

	// ����p�ɉ�����l��ۑ�
	fDiff = collisionPos[0].y;

	// ���S�̒��_�̍�����0�ɂȂ�悤�Ɉ���
	collisionPos[0].y -= fDiff;
	collisionPos[1].y -= fDiff;
	collisionPos[2].y -= fDiff;

	// ����p�@��
	vecA = collisionPos[1] - collisionPos[0];	// ����
	vecB = collisionPos[2] - collisionPos[0];	// �E��
	D3DXVec3Cross(&collisionNor, &vecA, &vecB);			// �O�ς����߂�
	D3DXVec3Normalize(&collisionNor, &collisionNor);	// ���K��

	// �v���C���[�̃x�N�g��
	vecPlayer = pos - collisionPos[0];

	// �v���C���[��Y�������߂�
	fHeight = (-(collisionNor.x * vecPlayer.x) - (collisionNor.z * vecPlayer.z)) / collisionNor.y;

	// ����p�ɉ���������␳
	fHeight += fDiff;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return fHeight;
}

//=========================================================================================================================
// ���_���グ��
//=========================================================================================================================
void CScene3DIndex::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{// �S���_�̋������v�Z
		float fLength = powf(pVtx[nCntVtx].pos.x - pos.x, 2.0f) + powf(pVtx[nCntVtx].pos.z - pos.z, 2.0f);

		if (fLength <= powf(fRange, 2.0f))
		{// �͈͓��ɂ��钸�_
			float fHeight = cosf((D3DX_PI * 0.5f) / (fRange / sqrtf(fLength))) * fValue;

			pVtx[nCntVtx].pos.y += fHeight;

#if 0	// sin,cos���g��Ȃ�
			float fDiff = powf(fRange, 2) - (powf(pVtx[nCntVtx].pos.x - pos.x, 2) + powf(pVtx[nCntVtx].pos.z - pos.z, 2));
			pVtx[nCntVtx].pos.y += fDiff / powf(fRange, 2);
#endif
		}
	}

	// �@���̍Čv�Z
	SetNor(pVtx);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================================================================================================
// ���_���̐ݒ�
//=========================================================================================================================
void CScene3DIndex::SetVtx(void)
{
	// �|���S����
	m_nNumPolygon = m_nSplitX * m_nSplitZ * 2 + (4 * (m_nSplitZ - 1));

	// �J�E���g�p
	int nCntMeshYZ, nCntMeshX, nCntIdx;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshYZ = 0; nCntMeshYZ <= m_nSplitZ; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX <= m_nSplitX; nCntMeshX++)
		{
			// ���_���W
			switch (CScene::GetObjType())
			{
			case CScene::OBJTYPE_FIELD:		// �t�B�[���h
				pVtx[0 + (nCntMeshX + (nCntMeshYZ * (m_nSplitX + 1)))].pos = D3DXVECTOR3(-(m_fLengthX * m_nSplitX * 0.5f) + (m_fLengthX * nCntMeshX) * 1.0f,
					0.0f,
					(m_fLengthZ * m_nSplitZ * 0.5f) - (m_fLengthZ * nCntMeshYZ) * 1.0f);
				break;
			}

			// ���_�J���[
			pVtx[0 + (nCntMeshX + (nCntMeshYZ * (m_nSplitX + 1)))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W
			pVtx[0 + (nCntMeshX + (nCntMeshYZ * (m_nSplitX + 1)))].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshYZ * 1.0f);
		}
	}

	// �@���̌v�Z
	SetNor(pVtx);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �n��
	switch (CScene::GetObjType())
	{
	case CScene::OBJTYPE_FIELD:		// �t�B�[���h
		for (nCntMeshYZ = 0, nCntIdx = 0; nCntMeshYZ < m_nSplitZ; nCntMeshYZ++)
		{
			for (nCntMeshX = 0; nCntMeshX <= m_nSplitX; nCntMeshX++, nCntIdx++)
			{

				pIdx[0] = (m_nSplitX + 1) + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				pIdx += 2;

				if (nCntMeshYZ < m_nSplitZ - 1 && nCntMeshX == m_nSplitX)
				{// �܂�Ԃ���
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = (m_nSplitX + 1) + nCntIdx + 1;

					pIdx += 2;
				}
			}
		}
		break;
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
}

//=========================================================================================================================
// �@���̌v�Z
//=========================================================================================================================
void CScene3DIndex::SetNor(VERTEX_3D *pVtx)
{
	// �J�E���g�p
	int nCntMeshYZ, nCntMeshX, nCntVtx;

	for (nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{// �S���_�̖@����������
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (nCntMeshYZ = 0; nCntMeshYZ < m_nSplitZ; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX < m_nSplitX; nCntMeshX++)
		{// �@��
			D3DXVECTOR3 vecA;	// ���_�O���璸�_�P�ւ̕���
			D3DXVECTOR3 vecB;	// ���_�O���璸�_�Q�ւ̕���
			D3DXVECTOR3 nor[2];	// �@���̌v�Z����
			int nVtxNumber[4];	// �S���_�̔ԍ�

			nVtxNumber[0] = nCntMeshX + ((nCntMeshYZ + 1) * (m_nSplitX + 1));			// �����̃u���b�N�̍����ԍ�
			nVtxNumber[1] = nCntMeshX + (nCntMeshYZ * (m_nSplitX + 1));					// �����̃u���b�N�̍���ԍ�
			nVtxNumber[2] = (nCntMeshX + 1) + ((nCntMeshYZ + 1) * (m_nSplitX + 1));		// �����̃u���b�N�̉E���ԍ�
			nVtxNumber[3] = (nCntMeshX + 1) + (nCntMeshYZ * (m_nSplitX + 1));			// �����̃u���b�N�̉E��ԍ�

			// �e�|���S���̒��_�����߂�
			// ����-------------------------------------------------------------------------------------------
			// 0
			vecA = pVtx[nVtxNumber[1]].pos - pVtx[nVtxNumber[0]].pos;	// ����
			vecB = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[0]].pos;	// �E��

			D3DXVec3Cross(&nor[0], &vecA, &vecB);	// �O�ς����߂�
			D3DXVec3Normalize(&nor[0], &nor[0]);	// ���K��

			// �E��-------------------------------------------------------------------------------------------
			// 3
			vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[3]].pos;	// ����
			vecB = pVtx[nVtxNumber[1]].pos - pVtx[nVtxNumber[3]].pos;	// �E��

			D3DXVec3Cross(&nor[1], &vecA, &vecB);	// �O�ς����߂�
			D3DXVec3Normalize(&nor[1], &nor[1]);	// ���K��

													// �e���_�̖@�����Z�o
			pVtx[nVtxNumber[0]].nor += nor[0];
			pVtx[nVtxNumber[1]].nor += nor[0] + nor[1];
			pVtx[nVtxNumber[2]].nor += nor[0] + nor[1];
			pVtx[nVtxNumber[3]].nor += nor[1];
		}
	}

	for (nCntMeshYZ = 0; nCntMeshYZ < m_nSplitZ; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX < m_nSplitX; nCntMeshX++)
		{// �@���̕��ς��o��
			D3DXVECTOR3 vecA;	// ���_�O���璸�_�P�ւ̕���
			D3DXVECTOR3 vecB;	// ���_�O���璸�_�Q�ւ̕���
			int nVtxNumber[4];	// �S���_�̔ԍ�

			nVtxNumber[0] = nCntMeshX + ((nCntMeshYZ + 1) * (m_nSplitX + 1));			// �u���b�N�̍����ԍ�
			nVtxNumber[1] = nCntMeshX + (nCntMeshYZ * (m_nSplitX + 1));					// �u���b�N�̍���ԍ�
			nVtxNumber[2] = (nCntMeshX + 1) + ((nCntMeshYZ + 1) * (m_nSplitX + 1));		// �u���b�N�̉E���ԍ�
			nVtxNumber[3] = (nCntMeshX + 1) + (nCntMeshYZ * (m_nSplitX + 1));			// �u���b�N�̉E��ԍ�

			if (nCntMeshX == 0 && nCntMeshYZ == 0)
			{// ����[
				pVtx[nVtxNumber[0]].nor /= 3;
				pVtx[nVtxNumber[1]].nor /= 2;
				pVtx[nVtxNumber[2]].nor /= 6;
				pVtx[nVtxNumber[3]].nor /= 3;
			}
			else if (nCntMeshX == 0 && nCntMeshYZ > 0 && nCntMeshYZ < m_nSplitZ - 1)
			{// ���[
				pVtx[nVtxNumber[0]].nor /= 3;
				pVtx[nVtxNumber[2]].nor /= 6;
			}
			else if (nCntMeshX == m_nSplitX - 1 && nCntMeshYZ > 0 && nCntMeshYZ < m_nSplitZ - 1)
			{
				pVtx[nVtxNumber[2]].nor /= 3;
			}
			else if (nCntMeshYZ == 0)
			{// ��[
				if (nCntMeshX == m_nSplitX - 1)
				{// �E��[
					pVtx[nVtxNumber[2]].nor /= 3;
				}
				else
				{// �㒆
					pVtx[nVtxNumber[2]].nor /= 6;
					pVtx[nVtxNumber[3]].nor /= 3;
				}
			}
			else if (nCntMeshYZ == m_nSplitZ - 1)
			{// ���[
				if (nCntMeshX == m_nSplitX - 1)
				{// �E���[
					pVtx[nVtxNumber[2]].nor /= 2;
				}
				else
				{// ����
					pVtx[nVtxNumber[2]].nor /= 3;
				}
			}
			else if (nCntMeshX > 0 && nCntMeshYZ > 0 && nCntMeshX < m_nSplitX - 1 && nCntMeshYZ < m_nSplitZ - 1)
			{// ��
				pVtx[nVtxNumber[2]].nor /= 6;
			}
		}
	}
}

//=========================================================================================================================
// ���_���̍쐬
//=========================================================================================================================
void CScene3DIndex::MakeVertex(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���_���̌v�Z
	m_nNumVtx = (m_nSplitX + 1) * (m_nSplitZ + 1);

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nSplitX * m_nSplitZ * 2 + (4 * (m_nSplitZ - 1)) + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// ���_���̐ݒ�
	SetVtx();
}

//=========================================================================================================================
// ���_���̎擾
//=========================================================================================================================
void CScene3DIndex::GetInfo(float *pWidth, float *pHeight, float *pDepth, int *pMeshX, int *pMeshY, int *pMeshZ)
{
	if (pWidth != NULL) { *pWidth = m_fLengthX; }
	if (pHeight != NULL) { *pHeight = m_fLengthY; }
	if (pDepth != NULL) { *pDepth = m_fLengthZ; }
	if (pMeshX != NULL) { *pMeshX = m_nSplitX; }
	if (pMeshY != NULL) { *pMeshY = m_nSplitY; }
	if (pMeshZ != NULL) { *pMeshZ = m_nSplitZ; }
}

//=========================================================================================================================
// �ʒu�̐ݒ�
//=========================================================================================================================
const void CScene3DIndex::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=========================================================================================================================
// �t�@�C���ɏo��
//=========================================================================================================================
void CScene3DIndex::Save(void)
{
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen(TEXT_SAVE_FILENAME, "w");

	if (NULL != pFile)
	{// �t�@�C����������
		fprintf(pFile, "#********************************************************************\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#    �n�ʃX�N���v�g�t�@�C��[field.txt]\n");
		fprintf(pFile, "#    Auther : Ishida Takuto\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#********************************************************************\n");

		fprintf(pFile, "FIELDSET\n");

		fprintf(pFile, "	POS = %.1f %.1f %.1f	# �ʒu\n", m_pos.x, m_pos.y, m_pos.z);
		fprintf(pFile, "	ROT = %.1f %.1f %.1f	# ����\n", m_rot.x, m_rot.y, m_rot.z);
		fprintf(pFile, "	WIDTH = %.1f	# 1�u���b�N���̕��̑傫��\n", m_fLengthX);
		fprintf(pFile, "	DEPTH = %.1f	# 1�u���b�N���̉��s�̑傫��\n", m_fLengthZ);
		fprintf(pFile, "	XBLOCK = %d	# ���̕�����\n", m_nSplitX);
		fprintf(pFile, "	ZBLOCK = %d	# ���s�̕�����\n", m_nSplitZ);
		fprintf(pFile, "	VERTEX_FILENAME = %s	# �ǂݍ��ޒ��_���̃o�C�i���t�@�C����\n", &m_aFileName[0]);
		fprintf(pFile, "	TEXTURE = %s	# �e�N�X�`����\n", &m_aTexFileName[0]);

		fprintf(pFile, "END_FIELDSET\n");

		// �t�@�C�������
		fclose(pFile);

		// ���_���̕ۑ�
		SaveVtx();
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I\n�t�@�C�����쐬���܂��B", "field.txt", MB_OK);
	}
}

//=========================================================================================================================
// �t�@�C������ǂݍ���
//=========================================================================================================================
void CScene3DIndex::Load(void)
{
	FILE *pFile;	// �t�@�C���̃|�C���^�ϐ�
	char aStr[128];	// �擾���镶����

	char *FileName = CManager::GetFieldFileName(0);

	// �t�@�C�����J��
	pFile = fopen(&FileName[0], "r");

	if (NULL != pFile)
	{// �t�@�C����������
		while (strcmp(aStr, "END_FIELDSET") != 0)
		{
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, "POS") == 0)
			{// �ʒu
				fscanf(pFile, " = %f %f %f", &m_pos.x, &m_pos.y, &m_pos.z);
			}
			if (strcmp(aStr, "ROT") == 0)
			{// ����
				fscanf(pFile, " = %f %f %f", &m_rot.x, &m_rot.y, &m_rot.z);
			}
			if (strcmp(aStr, "WIDTH") == 0)
			{// ��
				fscanf(pFile, " = %f", &m_fLengthX);
			}
			if (strcmp(aStr, "DEPTH") == 0)
			{// ���s
				fscanf(pFile, " = %f", &m_fLengthZ);
			}
			if (strcmp(aStr, "XBLOCK") == 0)
			{// ���̕�����
				fscanf(pFile, " = %d", &m_nSplitX);
			}
			if (strcmp(aStr, "ZBLOCK") == 0)
			{// ���s�̕�����
				fscanf(pFile, " = %d", &m_nSplitZ);
			}
			if (strcmp(aStr, "VERTEX_FILENAME") == 0)
			{// �ǂݍ��ރo�C�i���t�@�C��
				fscanf(pFile, " = %s", &m_aFileName[0]);
			}
			if (strcmp(aStr, "TEXTURE") == 0)
			{// �e�N�X�`��
				fscanf(pFile, " = %s", &m_aTexFileName[0]);

				// �f�o�C�X�̎擾
				CRenderer *pRenderer = CManager::GetRenderer();
				LPDIRECT3DDEVICE9 pDevice;
				pDevice = pRenderer->GetDevice();

				// �e�N�X�`���̐���
				D3DXCreateTextureFromFile(pDevice, &m_aTexFileName[0], &m_pTexture);
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I", "field.txt", MB_OK);
	}
}

//=========================================================================================================================
// ���_�����t�@�C���ɏo��
//=========================================================================================================================
void CScene3DIndex::SaveVtx(void)
{
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen(BIN_SAVE_FILENAME, "wb");

	if (NULL != pFile)
	{// �t�@�C����������
		VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�����t�@�C���ɏ�������
		fwrite(pVtx, sizeof(VERTEX_3D), m_nNumVtx, pFile);

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		// �t�@�C�������
		fclose(pFile);

		MessageBox(NULL, "�Z�[�u���܂����B", "�n�ʃf�[�^", MB_OK);
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I\n�t�@�C�����쐬���܂��B", "field.bin", MB_OK);
	}
}

//=========================================================================================================================
// ���_�����t�@�C������ǂݍ���
//=========================================================================================================================
void CScene3DIndex::LoadVtx(void)
{
	// �t�@�C���̃|�C���^�ϐ�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen(m_aFileName, "rb");

	if (NULL != pFile)
	{// �t�@�C����������
		VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�����t�@�C���ɏ�������
		fread(pVtx, sizeof(VERTEX_3D), m_nNumVtx, pFile);

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I", "field.bin", MB_OK);
	}
}