//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "particle.h"
#include "renderer.h"
#include "manager.h"

#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PARTICLE_FILENAME	"data/TEXT/particle.txt"				// �p�[�e�B�N���f�[�^
#define TEXTURE_FILENAME	"data/TEXT/particle_texture.txt"		// �e�N�X�`����

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CParticlePolygon::m_pTexture[MAX_TEXTURE] = {};
CParData CParData::m_apParticleData[MAX_EMITTER_TYPE] = {};

//=========================================
// �R���X�g���N�^
//=========================================
CParticle::CParticle(int nPriority) : CScene(nPriority)
{
	m_nType = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// �f�X�g���N�^
//=========================================
CParticle::~CParticle()
{
}

//==================================
// ��������
//==================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, int nType)
{
	CParticle *pParticle;

	pParticle = new CParEmitter;	// ���������m��
	pParticle->m_pos = pos;
	pParticle->m_nType = nType;
	pParticle->Init();

	return pParticle;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticle::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_PARTICLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticle::Uninit(void)
{
	Release();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		�p�[�e�B�N���̃G�~�b�^
//
//=========================================
// �R���X�g���N�^
//=========================================
CParEmitter::CParEmitter()
{
	m_nCntParticle = 0;
	m_nAppear = 0;
	m_nCntAngle = 0;

	m_nLife = 0;
	m_nType = 0;
	m_nAppear = 0;
	m_nLap = 0;
	m_bLoop = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CParEmitter::~CParEmitter()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParEmitter::Init(void)
{
	CParticle::Init();
	Set(m_pos, m_nType);	// �G�~�b�^�̐ݒ�

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParEmitter::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParEmitter::Update(void)
{
	bool bDelete = false;
	m_nLife--;	// �G�~�b�^�̕\�����Ԃ��J�E���g

	if (m_nLife <= 0 && m_bLoop == false)
	{// ��莞�Ԍo�߂ŃG�~�b�^������
		bDelete = true;
	}

	// �o��������Ԋu�̃J�E���g
	m_nCntParticle = (m_nCntParticle + 1) % m_nAppear;

	if (m_nCntParticle == 0)
	{// �o��������
		CParticlePolygon::Create(m_pos, m_nType, m_nTexture);

		m_nCntAngle = (m_nCntAngle + 1) % m_nLap;
	}

	if (true == bDelete)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CParEmitter::Draw(void)
{
}

//=============================================================================
// �G�~�b�^�̐ݒ�
//=============================================================================
void CParEmitter::Set(D3DXVECTOR3 pos, int nType)
{
	m_pos = pos;
	m_nType = nType;
	m_nAppear = CParData::GetAppear(m_nType);
	m_nLife = CParData::GetLife(m_nType);
	m_nTexture = CParData::GetTextureIdx(m_nType);
	m_nLap = CParData::GetLap(m_nType);
}

//=============================================================================
// �G�~�b�^�̈ʒu�̐ݒ�
//=============================================================================
void CParEmitter::SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos)
{
	// �ʒu���X�V
	m_pos = pos;
}

//=============================================================================
// �G�~�b�^�̍폜
//=============================================================================
void CParEmitter::DestroyParticle(int nIdx)
{
	// ����
	CParEmitter::Uninit();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		�p�[�e�B�N���̃|���S��
//
//==================================
// ��������
//==================================
CParticlePolygon *CParticlePolygon::Create(D3DXVECTOR3 pos, int nType, int nTexture)
{
	CParticlePolygon *pParticlePlygon;

	pParticlePlygon = new CParticlePolygon;	// ���������m��
	pParticlePlygon->m_nType = nType;
	pParticlePlygon->Init(pos);
	pParticlePlygon->BindTexture(m_pTexture[nTexture]);
	return pParticlePlygon;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CParticlePolygon::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	char aFileName[MAX_TEXTURE][64];		// �e�N�X�`���̖��O
	int nCntTexture;
	int nNumTexture;

	pFile = fopen(TEXTURE_FILENAME, "r");

	if (NULL != pFile)
	{// �J����
		fscanf(pFile, "%d", &nNumTexture);		// �e�N�X�`����

		for (nCntTexture = 0; nCntTexture < nNumTexture; nCntTexture++)
		{
			fscanf(pFile, "%s\n", &aFileName[nCntTexture][0]);		// �e�N�X�`����
		}
		fclose(pFile);
	}
	else
	{// ���s
		MessageBox(0, "�t�@�C�����J���܂���ł���", "texture.txt", MB_OK);
	}

	// �e�N�X�`���̓ǂݍ���
	for (nCntTexture = 0; nCntTexture < nNumTexture; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, &aFileName[nCntTexture][0], &m_pTexture[nCntTexture]);
	}

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CParticlePolygon::Unload(void)
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
CParticlePolygon::CParticlePolygon(int nPriority) : CScene3DBill(nPriority)
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CParticlePolygon::~CParticlePolygon()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticlePolygon::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_PARTICLE);

	// ����������
	CScene3DBill::Init();
	CScene3DBill::SetPos(pos);

	int nLap = CParData::GetLap(m_nType);				// ����̐�
	int *nLife = CParData::GetParLife(m_nType);
	int *nRadius = CParData::GetRadius(m_nType);
	int *nWidth = CParData::GetWidth(m_nType);
	int *nHeight = CParData::GetHeight(m_nType);
	int *nDepth = CParData::GetDepth(m_nType);
	float fSpread = CParData::GetSpread(m_nType);
	D3DXCOLOR col = CParData::GetColor(m_nType);
	bool bDraw = CParData::GetDraw(m_nType);
	float fGravity = CParData::GetGravity(m_nType);

	m_fAngle = 1.0f / (nLap / 2);
	if (0 != nHeight[1] && 0 != nHeight[0])
	{// Y���ړ��ʂ������O�ȊO
		m_move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nWidth[0] + (rand() % (1 + nWidth[1]))) - ((rand() % 10) * 0.1f)) * fSpread,
			((rand() % nHeight[1]) + (rand() % nHeight[0])) * fSpread,
			cosf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nDepth[0] + (rand() % (1 + nDepth[1]))) - ((rand() % 10) * 0.1f)) * fSpread);
	}
	else
	{// Y���̈ړ��ʂ��O������ꍇ
		m_move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nWidth[0] + (rand() % (1 + nWidth[1]))) - ((rand() % 10) * 0.1f)) * fSpread,
			(nHeight[1] + nHeight[0]) * fSpread,
			cosf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nDepth[0] + (rand() % (1 + nDepth[1]))) - ((rand() % 10) * 0.1f)) * fSpread);
	}
	m_col = col;
	CScene3DBill::SetColor(m_col);
	m_fRadius = nRadius[0] + (rand() % (1 + (nRadius[1] - nRadius[0])) - 1) + ((rand() % 10) * 0.1f);
	m_nLife = nLife[0] + (rand() % (1 + (nLife[1] - nLife[0])) - 1);
	m_fGravity = fGravity;
	m_bDraw = bDraw;

	// ���_�̊p�x
	m_fRotAngle = atan2f(pos.x - (pos.x + m_fRadius), pos.y - (pos.y + m_fRadius));

	// ���S���璸�_�܂ł̋���
	m_fLength = sqrtf((m_fRadius * m_fRadius) + (m_fRadius * m_fRadius));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticlePolygon::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticlePolygon::Update(void)
{
	m_nLife--;		// �\�����Ԃ��J�E���g

	if (m_nLife <= 0)
	{// �\�����Ԃ�0�ȉ��ɂȂ���
		Uninit();	// �I������
	}
	else
	{
		D3DXVECTOR3 pos = CScene3DBill::GetPos();
		D3DXCOLOR colFlame = CParData::GetFlameColor(m_nType);
		float fFlameRadius = CParData::GetFlameRadius(m_nType);
		bool bBouncing = CParData::GetBouncing(m_nType);
		int nRotType = CParData::GetRotType(m_nType);
		float fRotSpeed = CParData::GetRotSpeed(m_nType);
		m_move.y -= m_fGravity;		// �d�͂�������

		m_fBouncing = pos.y;		// �l���L��

		pos += m_move;				// �ʒu�̍X�V
		m_col += colFlame;			// �F�̍X�V
		m_fRadius += fFlameRadius;	// �傫���̍X�V

		if (pos.y <= 0 && bBouncing == true)
		{// �o�E���h
			m_move.y += (m_fBouncing - pos.y) * 2.0f;
		}

		if (nRotType == 1)
		{// ���v���
			m_fCntRot -= fRotSpeed;			// ��]�̃J�E���^
		}
		else if (nRotType == 2)
		{// �����v���
			m_fCntRot += fRotSpeed;			// ��]�̃J�E���^
		}
		else if (nRotType == 3)
		{// �����_��
			if (rand() % 2 == 0)
			{// ���v���
				m_fCntRot += fRotSpeed;		// ��]�̃J�E���^
			}
			else
			{// �����v���
				m_fCntRot -= fRotSpeed;		// ��]�̃J�E���^
			}
		}

		D3DXVECTOR3 vtxPos[4];

		//���_���W�̐ݒ�
		vtxPos[0].x = sinf(m_fRotAngle + m_fCntRot) * -(m_fLength + m_fRadius);
		vtxPos[0].y = cosf(m_fRotAngle + m_fCntRot) * (m_fLength + m_fRadius);
		vtxPos[0].z = 0.0f;
		vtxPos[1].x = sinf(m_fRotAngle - m_fCntRot) * (m_fLength + m_fRadius);
		vtxPos[1].y = cosf(m_fRotAngle - m_fCntRot) * (m_fLength + m_fRadius);
		vtxPos[1].z = 0.0f;
		vtxPos[2].x = sinf(m_fRotAngle - m_fCntRot) * -(m_fLength + m_fRadius);
		vtxPos[2].y = cosf(m_fRotAngle - m_fCntRot) * -(m_fLength + m_fRadius);
		vtxPos[2].z = 0.0f;
		vtxPos[3].x = sinf(m_fRotAngle + m_fCntRot) * (m_fLength + m_fRadius);
		vtxPos[3].y = cosf(m_fRotAngle + m_fCntRot) * -(m_fLength + m_fRadius);
		vtxPos[3].z = 0.0f;

		//���_�J���[
		CScene3DBill::SetColor(m_col);

		CScene3DBill::SetPos(pos);

		CScene3DBill::SetVtxPos(&vtxPos[0]);
	}
}

//=============================================================================
// �|���S���`�揈��
//=============================================================================
void CParticlePolygon::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (m_bDraw == true)
	{ // ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CScene3DBill::Draw();

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�o�b�t�@�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		�p�[�e�B�N���̃f�[�^
//
//=========================================
// �R���X�g���N�^
//=========================================
CParData::CParData()
{
	for (int nCntParticle = 0; nCntParticle < 2; nCntParticle++)
	{
		m_nWidth[nCntParticle] = 0;
		m_nHeight[nCntParticle] = 0;
		m_nDepth[nCntParticle] = 0;
		m_nRadius[nCntParticle] = 0;
		m_nParLife[nCntParticle] = 0;
	}

	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fFlameRadius = 0.0f;
	m_nLife = 0;
	m_nTexture = 0;
	m_fSpread = 0.0f;
	m_nAppear = 0;
	m_fGravity = 0.0f;
	m_nLap = 0;
	m_nRot = 0;
	m_fRotSpeed = 0.0f;
	m_bLoop = false;
	m_bBouncing = false;
	m_bDraw = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CParData::~CParData()
{
}

//=============================================================================
// �t�@�C������f�[�^���擾
//=============================================================================
HRESULT CParData::Load(void)
{
	FILE *pFile;
	int nCntEffect;

	pFile = fopen(PARTICLE_FILENAME, "r");

	if (NULL != pFile)
	{
		for (nCntEffect = 0; nCntEffect < MAX_EMITTER_TYPE; nCntEffect++)
		{// �f�[�^���܂킷
			fscanf(pFile, "%d %d %d", &m_apParticleData[nCntEffect].m_nWidth[1], &m_apParticleData[nCntEffect].m_nHeight[1], &m_apParticleData[nCntEffect].m_nDepth[1]);	// �ړ��ʂ̍ő�
			fscanf(pFile, "%d %d %d", &m_apParticleData[nCntEffect].m_nWidth[0], &m_apParticleData[nCntEffect].m_nHeight[0], &m_apParticleData[nCntEffect].m_nDepth[0]);	// �ړ��ʂ̍ŏ�
			fscanf(pFile, "%f %f %f %f", &m_apParticleData[nCntEffect].m_col.r, &m_apParticleData[nCntEffect].m_col.g, &m_apParticleData[nCntEffect].m_col.b, &m_apParticleData[nCntEffect].m_col.a);	// �F
			fscanf(pFile, "%f %f %f %f", &m_apParticleData[nCntEffect].m_flameCol.r, &m_apParticleData[nCntEffect].m_flameCol.g, &m_apParticleData[nCntEffect].m_flameCol.b, &m_apParticleData[nCntEffect].m_flameCol.a);	// �F
			fscanf(pFile, "%d %d", &m_apParticleData[nCntEffect].m_nRadius[0], &m_apParticleData[nCntEffect].m_nRadius[1]);			// �T�C�Y
			fscanf(pFile, "%f", &m_apParticleData[nCntEffect].m_fFlameRadius);	// �T�C�Y�̉��Z
			fscanf(pFile, "%d", &m_apParticleData[nCntEffect].m_nLife);		// �\������(����)
			fscanf(pFile, "%d %d", &m_apParticleData[nCntEffect].m_nParLife[0], &m_apParticleData[nCntEffect].m_nParLife[1]);		// �p�[�e�B�N���̕\������
			fscanf(pFile, "%d", &m_apParticleData[nCntEffect].m_nAppear);		// �o��Ԋu
			fscanf(pFile, "%f", &m_apParticleData[nCntEffect].m_fSpread);		// �L����
			fscanf(pFile, "%d", &m_apParticleData[nCntEffect].m_nLap);			// 1���ɏo����
			fscanf(pFile, "%f", &m_apParticleData[nCntEffect].m_fGravity);		// �d��
			fscanf(pFile, "%d", &m_apParticleData[nCntEffect].m_nRot);			// ��]�̃p�^�[��
			fscanf(pFile, "%f", &m_apParticleData[nCntEffect].m_fRotSpeed);	// ��]�̑��x
			fscanf(pFile, "%d", (int*)&m_apParticleData[nCntEffect].m_bLoop);		// ���[�v���邩�ǂ���
			fscanf(pFile, "%d", (int*)&m_apParticleData[nCntEffect].m_bBouncing);	// �o�E���h���邩�ǂ���
			fscanf(pFile, "%d", &m_apParticleData[nCntEffect].m_nTexture);		// �e�N�X�`���̎��
			fscanf(pFile, "%d", (int*)&m_apParticleData[nCntEffect].m_bDraw);		// ���Z�����̗L��
		}

		fclose(pFile);		// �t�@�C�������
	}
	else
	{// ���s
		MessageBox(0, "�t�@�C�����J���܂���ł���", "particle.txt", MB_OK);
	}

	return S_OK;
}

//=============================================================================
// �f�[�^�̊J��
//=============================================================================
void CParData::Unload(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_EMITTER_TYPE; nCntParticle++)
	{// �f�[�^���܂킷
		for (int nCntParticle = 0; nCntParticle < 2; nCntParticle++)
		{
			m_apParticleData[nCntParticle].m_nWidth[nCntParticle] = 0;
			m_apParticleData[nCntParticle].m_nHeight[nCntParticle] = 0;
			m_apParticleData[nCntParticle].m_nDepth[nCntParticle] = 0;
			m_apParticleData[nCntParticle].m_nRadius[nCntParticle] = 0;
			m_apParticleData[nCntParticle].m_nParLife[nCntParticle] = 0;
		}

		m_apParticleData[nCntParticle].m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_apParticleData[nCntParticle].m_flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_apParticleData[nCntParticle].m_fFlameRadius = 0.0f;
		m_apParticleData[nCntParticle].m_nLife = 0;
		m_apParticleData[nCntParticle].m_nTexture = 0;
		m_apParticleData[nCntParticle].m_fSpread = 0.0f;
		m_apParticleData[nCntParticle].m_nAppear = 0;
		m_apParticleData[nCntParticle].m_fGravity = 0.0f;
		m_apParticleData[nCntParticle].m_nLap = 0;
		m_apParticleData[nCntParticle].m_nRot = 0;
		m_apParticleData[nCntParticle].m_fRotSpeed = 0.0f;
		m_apParticleData[nCntParticle].m_bLoop = false;
		m_apParticleData[nCntParticle].m_bBouncing = false;
		m_apParticleData[nCntParticle].m_bDraw = false;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParData::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParData::Uninit(void)
{
}
