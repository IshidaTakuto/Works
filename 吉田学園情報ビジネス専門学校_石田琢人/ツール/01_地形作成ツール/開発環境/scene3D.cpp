//=============================================================================
//
// 3D�I�u�W�F�N�g���� [scene3D.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugProc.h"

//==================================
// �}�N����`
//==================================
#define SIZE_X	(50.0f)	// �|���S���̉��̒���
#define SIZE_Y	(50.0f)	// �|���S���̏c�̒���
#define SIZE_Z	(50.0f)	// �|���S���̉��̒���

//==================================
// �ÓI�����o�ϐ��錾
//==================================
float g_fHieght = 80;

//=========================================
// �e�N�X�`�������蓖�Ă�
//=========================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene3D::CScene3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̈ʒu
	m_length = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene3D::~CScene3D()
{
}

//=========================================
// �|���S���̏���������
//=========================================
HRESULT CScene3D::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �F�̐ݒ�
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

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

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - SIZE_X, m_pos.y - SIZE_Y, m_pos.z - SIZE_Z);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + SIZE_X, m_pos.y - SIZE_Y, m_pos.z - SIZE_Z);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - SIZE_X, m_pos.y - SIZE_Y, m_pos.z + SIZE_Z);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + SIZE_X, m_pos.y - SIZE_Y, m_pos.z + SIZE_Z);

	// �@��
	pVtx[0].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

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
void CScene3D::Uninit(void)
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
void CScene3D::Update(void)
{
}

//=========================================
// �|���S���̕`�揈��
//=========================================
void CScene3D::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// �ʒu�̐ݒ�
//=========================================
const void CScene3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=========================================
// ��]�̐ݒ�
//=========================================
const void CScene3D::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot / 4;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;	// ���[���h�}�g���b�N�X�̕ۑ��p

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pVtx[nCntVtx].pos.x, pVtx[nCntVtx].pos.y, pVtx[nCntVtx].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &m_mtxWorld);

		pVtx[nCntVtx].pos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	}

	// �@��
	D3DXVECTOR3 vecA;	// ���_�O���璸�_�P�ւ̕���
	D3DXVECTOR3 vecB;	// ���_�O���璸�_�Q�ւ̕���
	D3DXVECTOR3 nor;	// �@���̌v�Z����

	// 0
	vecA = pVtx[1].pos - pVtx[0].pos;	// �v�Z�ɕK�v�ȃx�N�g��
	vecB = pVtx[2].pos - pVtx[0].pos;	// �v�Z�ɕK�v�ȃx�N�g��

	D3DXVec3Cross(&nor, &vecA, &vecB);	// �O�ς����߂�
	D3DXVec3Normalize(&nor, &nor);		// ���K��
	pVtx[0].nor = nor;					// �@����ݒ�

	// 3
	vecA = pVtx[2].pos - pVtx[3].pos;	// �v�Z�ɕK�v�ȃx�N�g��
	vecB = pVtx[1].pos - pVtx[3].pos;	// �v�Z�ɕK�v�ȃx�N�g��

	D3DXVec3Cross(&nor, &vecA, &vecB);	// �O�ς����߂�
	D3DXVec3Normalize(&nor, &nor);		// ���K��
	pVtx[3].nor = nor;					// �@����ݒ�

	// 1
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2;	// �@����ݒ�

	// 2
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2;	// �@����ݒ�

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �����̐ݒ�
//=========================================
const void CScene3D::SetLength(D3DXVECTOR3 length)
{
	m_length = length;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_length.x, 10, -m_length.z);
	pVtx[1].pos = D3DXVECTOR3(-m_length.x, 50, m_length.z);
	pVtx[2].pos = D3DXVECTOR3(m_length.x, 20, -m_length.z);
	pVtx[3].pos = D3DXVECTOR3(m_length.x, 40, m_length.z);

	// �@��
	D3DXVECTOR3 vecA;	// ���_�O���璸�_�P�ւ̕���
	D3DXVECTOR3 vecB;	// ���_�O���璸�_�Q�ւ̕���
	D3DXVECTOR3 nor;	// �@���̌v�Z����

	// 0
	vecA = pVtx[1].pos - pVtx[0].pos;	// �v�Z�ɕK�v�ȃx�N�g��
	vecB = pVtx[2].pos - pVtx[0].pos;	// �v�Z�ɕK�v�ȃx�N�g��

	D3DXVec3Cross(&nor, &vecA, &vecB);	// �O�ς����߂�
	D3DXVec3Normalize(&nor, &nor);		// ���K��
	pVtx[0].nor = nor;					// �@����ݒ�

	// 3
	vecA = pVtx[2].pos - pVtx[3].pos;	// �v�Z�ɕK�v�ȃx�N�g��
	vecB = pVtx[1].pos - pVtx[3].pos;	// �v�Z�ɕK�v�ȃx�N�g��

	D3DXVec3Cross(&nor, &vecA, &vecB);	// �O�ς����߂�
	D3DXVec3Normalize(&nor, &nor);		// ���K��
	pVtx[3].nor = nor;					// �@����ݒ�

	// 1
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2;	// �@����ݒ�

	// 2
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2;	// �@����ݒ�

	//pVtx[0].pos = D3DXVECTOR3(-m_length.x, m_length.y, m_length.z);
	//pVtx[1].pos = D3DXVECTOR3(m_length.x, m_length.y + 100.0f, m_length.z);
	//pVtx[2].pos = D3DXVECTOR3(-m_length.x, -m_length.y + 100.0f, -m_length.z);
	//pVtx[3].pos = D3DXVECTOR3(m_length.x, -m_length.y, -m_length.z);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �F�̐ݒ�
//=========================================
const void CScene3D::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �F�̉��Z
//=========================================
const void CScene3D::AddColor(D3DXCOLOR col)
{
	m_col += col;

	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �e�N�X�`�����W�̐ݒ�
//=========================================
const void CScene3D::SetTexPos(D3DXVECTOR2 *texPos)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = texPos[0];
	pVtx[1].tex = texPos[1];
	pVtx[2].tex = texPos[2];
	pVtx[3].tex = texPos[3];

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �e�N�X�`�����W�𓮂���
//=========================================
void CScene3D::MoveTexPos(D3DXVECTOR2 speed)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex += speed;
	pVtx[1].tex += speed;
	pVtx[2].tex += speed;
	pVtx[3].tex += speed;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
