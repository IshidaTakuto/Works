//=============================================================================
//
// �G���� [enemy.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "player.h"
#include "game.h"
#include "camera.h"
#include "gauge.h"
#include "score.h"
#include "field.h"
#include "tutorial.h"
#include "result.h"
#include "title.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

//==================================
// �}�N����`
//==================================
#define ENEMY_MODEL_AIRCRAFT	"data/MODEL/aircraft.x"
#define ENEMY_MODEL_SHIP		"data/MODEL/ship.x"
#define ENEMY_MODEL_TANK		"data/MODEL/tank.x"
#define ENEMY_SPEED		(0.4f)	// �ړ��̑��x
#define ENEMY_LIFE		(2)		// ���C�t
#define ENEMY_SHOOT		(120)	// ���Ԋu

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9	CEnemy::m_pTexture[TYPE_MAX] = {};
LPD3DXMESH			CEnemy::m_pMesh[TYPE_MAX] = {};	// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		CEnemy::m_pBuffMat[TYPE_MAX] = {};	// �}�e���A�����ւ̃|�C���^
DWORD				CEnemy::m_nNumMat[TYPE_MAX] = {};	// �}�e���A�����̐�
int					CEnemy::m_nNumEnemy = 0;

//==================================
// ��������
//==================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, TYPE type)
{
	CEnemy *pEnemy;

	pEnemy = new CEnemy;	// ���������m��
	pEnemy->BindInfo(m_pTexture[type], m_pMesh[type], m_pBuffMat[type], m_nNumMat[type]);	// ���f������n��
	pEnemy->m_type = type;
	pEnemy->Init(pos);

	return pEnemy;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CEnemy::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(ENEMY_MODEL_AIRCRAFT,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[TYPE_AIRCRAFT],
		NULL,
		&m_nNumMat[TYPE_AIRCRAFT],
		&m_pMesh[TYPE_AIRCRAFT]);

	pMat = (D3DXMATERIAL*)m_pBuffMat[TYPE_AIRCRAFT]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[TYPE_AIRCRAFT]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(ENEMY_MODEL_SHIP,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[TYPE_SHIP],
		NULL,
		&m_nNumMat[TYPE_SHIP],
		&m_pMesh[TYPE_SHIP]);

	// �e�N�X�`���̐���
	pMat = (D3DXMATERIAL*)m_pBuffMat[TYPE_SHIP]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[TYPE_SHIP]);


	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(ENEMY_MODEL_TANK,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[TYPE_TANK],
		NULL,
		&m_nNumMat[TYPE_TANK],
		&m_pMesh[TYPE_TANK]);

	// �e�N�X�`���̐���
	pMat = (D3DXMATERIAL*)m_pBuffMat[TYPE_TANK]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[TYPE_TANK]);

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CEnemy::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		if (NULL != m_pTexture[nCntModel])
		{// �e�N�X�`���̔j��
			m_pTexture[nCntModel]->Release();
			m_pTexture[nCntModel] = NULL;
		}

		if (NULL != m_pMesh[nCntModel])
		{// ���b�V�����̔j��
			m_pMesh[nCntModel]->Release();
			m_pMesh[nCntModel] = NULL;
		}

		if (NULL != m_pBuffMat[nCntModel])
		{// �}�e���A�����̔j��
			m_pBuffMat[nCntModel]->Release();
			m_pBuffMat[nCntModel] = NULL;
		}

		if (NULL != m_nNumMat[nCntModel])
		{// �}�e���A�����̏�����
			m_nNumMat[nCntModel] = 0;
		}
	}

	// �G�̑�����������
	m_nNumEnemy = 0;
}

//=========================================
// �R���X�g���N�^
//=========================================
CEnemy::CEnemy(int nPriority) : CScene3DModel(nPriority)
{
	m_posOriginalZ = 0.0f;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_state = STATE_NORMAL;
	m_nCntState = 0;
	m_bMoveAngle = true;
	m_nNumEnemy++;
	m_pShadow = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CEnemy::~CEnemy()
{
}

//=========================================
// ����������
//=========================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	CScene::SetObjType(CScene::OBJTYPE_ENEMY);
	CScene3DModel::Init();

	CScene3DModel::SetPos(pos);		// �|���S���̈ʒu��ݒ�
	CScene3DModel::SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	CScene3DModel::SetLife(ENEMY_LIFE);
	CScene3DModel::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if (m_pShadow == NULL && m_type == TYPE_AIRCRAFT)
	{// �e������
		m_pShadow = CShadow::Create(pos);
	}

	m_posOriginalZ = pos.z;
	m_move.x = -ENEMY_SPEED;			// ��Ƀv���C���[�̔��Ε����ɐi�܂���

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CEnemy::Uninit(void)
{
	CScene3DModel::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CEnemy::Update(void)
{
	D3DXVECTOR3 thisPos = CScene3DModel::GetPos();	// �ʒu�̎擾
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 posCamera = pCamera->GetPosV();
	float vtxMinY = CScene3DModel::GetMinVtxY();

	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_TITLE)
	{// �^�C�g���̂Ƃ�
		m_move.x = -(rand() % 3 + 0.5f);

		CField *pField[MAX_FIELD];		// �t�B�[���h�̃|�C���^
		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{
			pField[nCntField] = CTitle::GetField(nCntField);

			if (pField[nCntField] != NULL)
			{
				CField::TYPE type = pField[nCntField]->GetType();		// �t�B�[���h�̎�ނ̎擾

				if (pField[nCntField]->Collision(&thisPos, vtxMinY) && type == CField::TYPE_GROUND && m_type == TYPE_SHIP)
				{// ����ɂ���
					m_state = STATE_DEATH;	// ���S��Ԃɂ���
				}
				else if (pField[nCntField]->Collision(&thisPos, -vtxMinY) && type == CField::TYPE_SEA && m_type == TYPE_TANK)
				{// �C��ɂ���
					m_state = STATE_DEATH;	// ���S��Ԃɂ���
				}
			}
		}
		if (m_pShadow != NULL)
		{// �e�̈ʒu�̐ݒ�
			m_pShadow->SetPos(D3DXVECTOR3(thisPos.x, 0.0f, thisPos.z));
		}
	}
	else
	{// �^�C�g���ȊO
		CField *pField[MAX_FIELD];		// �t�B�[���h�̃|�C���^

		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{// �G�𓮂���
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// ���[�h���Q�[���̂Ƃ�
				pField[nCntField] = CGame::GetField(nCntField);		// �t�B�[���h�̏����擾
			}
			else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
			{// ���[�h���`���[�g���A���̂Ƃ�
				pField[nCntField] = CTutorial::GetField(nCntField);	// �t�B�[���h�̏����擾
			}

			if (NULL != pField[nCntField])
			{// ��񂪂�����̂̂�
				CField::TYPE type = pField[nCntField]->GetType();	// �t�B�[���h�̎�ނ̎擾
				D3DXVECTOR3 length;
				switch (m_type)
				{// ��ނɂ���ĕς�鏈��
				case TYPE_AIRCRAFT:		// �퓬�@
					if (m_move.y < 0.8f && m_bMoveAngle)
					{// �㏸
						m_move.y += 0.002f;

						if (m_move.y >= 0.8f)
						{// ���܂ŏ㏸����
							m_bMoveAngle = false;
						}
					}
					else if (m_move.y > -0.8f && !m_bMoveAngle)
					{// ���~
						m_move.y -= 0.002f;

						if (m_move.y <= -0.8f)
						{// ���܂ŉ��~����
							m_bMoveAngle = true;
						}
					}

					if (m_pShadow != NULL)
					{// �e�̑傫���������ŕς���
						length = m_pShadow->GetLength();
						length.x = (0.5f * thisPos.y) - length.x;
						length.z = (0.5f * thisPos.y) - length.z;

						m_pShadow->SetLength(length);	// �傫���̐ݒ�

						m_pShadow->SetPos(D3DXVECTOR3(thisPos.x, 0.0f, thisPos.z));		// �e�̈ʒu�̐ݒ�
					}

					break;

				case TYPE_SHIP:			// ���
					if (pField[nCntField]->Collision(&thisPos, vtxMinY) && type == CField::TYPE_GROUND)
					{// ����ɂ���
						m_state = STATE_DEATH;
					}
					break;

				case TYPE_TANK:			// ���
					if (pField[nCntField]->Collision(&thisPos, -vtxMinY) && type == CField::TYPE_SEA)
					{// �C��ɂ���
						m_state = STATE_DEATH;
					}
					break;
				}
			}
		}

		CPlayer *pPlayer = CGame::GetPlayer();				// �v���C���[�̏����擾
		if (pPlayer != NULL)
		{
			// �v���C���[�̎�ނɂ���ĕς�鏈��
			D3DXVECTOR3 posPlayer = pPlayer->GetPos();			// �v���C���[�̈ʒu���擾
			CPlayer::TYPE typePlayer = pPlayer->GetType();		// �v���C���[�̎�ނ��擾
			m_nCntBullet = (m_nCntBullet + 1) % ENEMY_SHOOT;	// �e�����Ԋu
			CSound *pSound = CManager::GetSound();				// �T�E���h�̏����擾

			if (typePlayer == CPlayer::TYPE_AIRCRAFT)
			{// �v���C���[���퓬�@
				if (posPlayer.x + 50.0f < thisPos.x)
				{// ���֒e�������Ȃ�
					if (thisPos.x > posCamera.x - 310 && thisPos.x < posCamera.x + 310)
					{// ��ʓ��ɂ���Ƃ�
						if (m_nCntBullet == 0)
						{// �e������
							pSound->PlaySound(CSound::SOUND_LABEL_BULLET000);
							CBullet::Create(thisPos, CBullet::TYPE_ENEMY);
						}
					}
				}

				thisPos.z = 0;
				m_move.z = 0;
			}
			else if (typePlayer == CPlayer::TYPE_SHIP)
			{// �v���C���[�����
				if (thisPos.x > posCamera.x - 250 && thisPos.x < posCamera.x + 250)
				{// ��ʓ��ɂ���Ƃ�
					if (m_nCntBullet == 0)
					{// �e������
						pSound->PlaySound(CSound::SOUND_LABEL_BULLET000);
						CBullet::Create(thisPos, CBullet::TYPE_ENEMY);
					}
				}

				thisPos.z = m_posOriginalZ;		// Z�������̈ʒu�ɖ߂�
			}
			else if (typePlayer == CPlayer::TYPE_TANK)
			{// �v���C���[�����
				if (posPlayer.x + 50.0f < thisPos.x)
				{// ���֒e�������Ȃ�
					if (m_nCntBullet == 0)
					{// �e������
						pSound->PlaySound(CSound::SOUND_LABEL_BULLET000);
						CBullet::Create(D3DXVECTOR3(thisPos.x, thisPos.y + 20.0f, thisPos.z), CBullet::TYPE_ENEMY);
					}
				}

				thisPos.z = m_posOriginalZ;		// Z�������̈ʒu�ɖ߂�
			}
		}
	}

	// �G�̈ʒu�̐ݒ�
	thisPos += m_move;
	CScene3DModel::SetPos(thisPos);
	CSound *pSound = CManager::GetSound();				// �T�E���h�̏����擾

	if (thisPos.x < posCamera.x - 400)
	{// ��ʂ����������
		m_state = STATE_DEATH;	// ���S��Ԃɂ���
	}

	int nLife = CScene3DModel::GetLife();		// ���C�t���擾

	switch (m_state)
	{
	case STATE_NORMAL:		// �ʏ���
		break;

	case STATE_DAMAGE:		// �_���[�W���
		m_nCntState++;		// �X�e�[�g���Ԃ��J�E���g����
		if (m_nCntState % 10 == 0 && nLife > 0)
		{// �ʏ펞�ɖ߂�
			if (mode == CManager::MODE_GAME)
			{// ���[�h���Q�[���̂Ƃ��̂�
				CScore::AddScore(300);		// �X�R�A�l��
			}
			m_state = STATE_NORMAL;
			m_nCntState = 0;
		}
		else if (nLife <= 0)
		{// ���C�t���Ȃ��Ȃ����Ƃ�
			if (mode == CManager::MODE_GAME)
			{// ���[�h���Q�[���̂Ƃ��̂�
				CScore::AddScore(1000);		// �X�R�A�l��
				CResult::AddKillEnemy();	// ���U���g�̓|�����G�������Z
			}

			// �����ƍ���
			CParticle::Create(thisPos, 6);
			CParticle::Create(thisPos, 1);
			pSound->PlaySound(CSound::SOUND_LABEL_EXPLOSION);

			CGauge::AddSubtract(10);
			m_state = STATE_DEATH;	// ���S��Ԃɂ���
		}
		break;

	case STATE_DEATH:		// ���S���
		if (m_pShadow != NULL)
		{
			m_pShadow->Uninit();
		}
		Uninit();					// �I������
		m_nNumEnemy--;
		break;
	}
}

//=========================================
// �`�揈��
//=========================================
void CEnemy::Draw(void)
{
	CScene3DModel::Draw();
}
