//=============================================================================
//
// �e���� [bullet.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "life.h"
#include "gauge.h"
#include "camera.h"
#include "particle.h"
#include "sound.h"

//==================================
// �}�N����`
//==================================
#define TEXTURE_NAME			"data/TEXTURE/bullet000.png"
#define BULLET_SPEED			(6.0f)	// �ړ��̑��x
#define BULLET_LIFE				(120)	// ����
#define BULLET_LENGTH			(3.0f)	// �傫��
#define BULLET_EFFECT_SIZE		(10.0f)	// �e�̃G�t�F�N�g�̑傫��
#define BULLET_EFFECT_LIFE		(10)	// �e�̃G�t�F�N�g�̎���
#define AIRCRAFT_SPECIAL_SIZE	(10.0f)	// �퓬�@�̓���e�̑傫��
#define SHIP_SPECIAL_SIZE		(20.0f)	// ��͂̓���e�̑傫��
#define TANK_SPECIAL_SIZE		(15.0f)	// ��Ԃ̓���e�̑傫��

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;
int CBulletPlayerSpecial::m_nCntAngle = 0;

//==================================
// ��������
//==================================
CBullet* CBullet::Create(D3DXVECTOR3 pos, TYPE type)
{
	CBullet *pBullet = NULL;

	switch (type)
	{
	case TYPE_PLAYERNORMAL:			// �v���C���[�̒ʏ�e
		pBullet = new CBulletPlayerNormal;	// ���������m��
		break;

	case TYPE_PLAYERSPECIAL:		// �v���C���[�̓���e
		pBullet = new CBulletPlayerSpecial;	// ���������m��
		break;

	case TYPE_ENEMY:				// �G�̒e
		pBullet = new CBulletEnemy;	// ���������m��
		break;
	}

	pBullet->Init(pos);
	pBullet->BindTexture(m_pTexture);

	return pBullet;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CBullet::Load(void)
{
	// �f�o�C�X�̎擾
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
void CBullet::Unload(void)
{
	if (NULL != m_pTexture)
	{// �e�N�X�`���̔j��
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CBullet::CBullet(int nPriority) : CScene3DBill(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
}

//=========================================
// �f�X�g���N�^
//=========================================
CBullet::~CBullet()
{
}

//=========================================
// �e�̏���������
//=========================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	CScene3DBill::Init();

	// �|���S���̈ʒu��ݒ�
	CScene3DBill::SetPos(pos);

	// �|���S���̃T�C�Y�̐ݒ�
	CScene3DBill::SetLength(BULLET_LENGTH, BULLET_LENGTH);

	// ���C�t�̐ݒ�
	CScene3DBill::SetLife(BULLET_LIFE);

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_BULLET);

	return S_OK;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �v���C���[�̒e
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=========================================
// �R���X�g���N�^
//=========================================
CBulletPlayer::CBulletPlayer()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CBulletPlayer::~CBulletPlayer()
{
}

//=========================================
// �v���C���[�̒e�̏���������
//=========================================
HRESULT CBulletPlayer::Init(D3DXVECTOR3 pos)
{
	CBullet::Init(pos);
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_move.x = BULLET_SPEED;

	return S_OK;
}

//=========================================
// �v���C���[�̒e�̏I������
//=========================================
void CBulletPlayer::Uninit(void)
{
	CScene3DBill::Uninit();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		�v���C���[�ʏ�e
//
//=========================================
// �R���X�g���N�^
//=========================================
CBulletPlayerNormal::CBulletPlayerNormal()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CBulletPlayerNormal::~CBulletPlayerNormal()
{
}

//=========================================
// �v���C���[�̒ʏ�e�̏���������
//=========================================
HRESULT CBulletPlayerNormal::Init(D3DXVECTOR3 pos)
{
	CBulletPlayer::Init(pos);
	return S_OK;
}

//=========================================
// �v���C���[�̒ʏ�e�̏I������
//=========================================
void CBulletPlayerNormal::Uninit(void)
{
	CBulletPlayer::Uninit();
}

//=========================================
// �v���C���[�̒ʏ�e�̍X�V����
//=========================================
void CBulletPlayerNormal::Update(void)
{
	D3DXVECTOR3 thisPos = CScene3DBill::GetPos();	// �e�̈ʒu

	thisPos += m_move;		// �ړ��ʂ��ʒu�ɉ��Z����

	// �ʒu�̍X�V
	CScene3DBill::SetPos(thisPos);

	int nLife;						// �e�̃��C�t���i�[
	nLife = CScene3DBill::GetLife();	// �e�̃��C�t���擾
	nLife--;
	CScene3DBill::SetLife(nLife);

	if (nLife <= 0)
	{// �������s����
		CBulletPlayer::Uninit();
	}
	else
	{// �����Ă�
		// �G�t�F�N�g������
		CEffect::Create(thisPos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), BULLET_EFFECT_SIZE, BULLET_EFFECT_LIFE);

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			CScene3DModel *pScene3DModel;								// 3D�I�u�W�F�N�g�̃|�C���^�ϐ�
			pScene3DModel = (CScene3DModel*)GetScene(ENEMY_PRIORITY, nCntScene);		// �C���f�b�N�X���Ɏ擾

			if (pScene3DModel != NULL)
			{// null�`�F�b�N
				CScene::OBJTYPE objType;					// �I�u�W�F�N�g�̎�ޔ��ʗp�̕ϐ�
				objType = pScene3DModel->GetObjType();			// �I�u�W�F�N�g�̎�ނ��擾

				if (objType == CScene::OBJTYPE_ENEMY)
				{// �I�u�W�F�N�g�̎�ނ��G�̂Ƃ�
					int nEnemyLife;								// �G�̃��C�t���i�[
					nEnemyLife = pScene3DModel->GetLife();		// �G�̃��C�t���擾

					CEnemy *pEnemy;												// �G�̏����i�[
					pEnemy = (CEnemy*)pScene3DModel->GetScene(ENEMY_PRIORITY, nCntScene);	// �G�̏����擾

					D3DXVECTOR3 pos = pEnemy->GetPos();
					float fLength = pEnemy->GetLength();
					bool bHit = false;			// �����蔻��̊m�F

					for (int nCntScene2 = 0; nCntScene2 < MAX_OBJECT; nCntScene2++)
					{
						CScene3DModel *pScene3DModel2;								// 3D�I�u�W�F�N�g�̃|�C���^�ϐ�
						pScene3DModel2 = (CScene3DModel*)GetScene(PLAYER_PRIORITY, nCntScene2);		// �C���f�b�N�X���Ɏ擾

						if (pScene3DModel2 != NULL)
						{// null�`�F�b�N
							CScene::OBJTYPE objType;					// �I�u�W�F�N�g�̎�ޔ��ʗp�̕ϐ�
							objType = pScene3DModel2->GetObjType();			// �I�u�W�F�N�g�̎�ނ��擾

							if (objType == CScene::OBJTYPE_PLAYER)
							{// �I�u�W�F�N�g�̎�ނ��v���C���[�̂Ƃ�
								CPlayer *pPlayer;															// �v���C���[�̏����i�[
								pPlayer = (CPlayer*)pScene3DModel2->GetScene(PLAYER_PRIORITY, nCntScene2);	// �v���C���[�̏����擾

								CPlayer::TYPE type;			// �v���C���[�̌��݂̌`�Ԃ��i�[
								type = pPlayer->GetType();	// �v���C���[�̌��݂̌`�Ԃ��擾

								// �J�����̏����擾
								CCamera *pCamera = CManager::GetCamera();
								D3DXVECTOR3 posCamera = pCamera->GetPosV();

								if (CPlayer::TYPE_AIRCRAFT == type)
								{// �v���C���[���퓬�@�̏ꍇ
									bHit = CScene3DBill::CollisionXY(pos, (fLength * 2.0f));	// Z���𔲂�������

									if (thisPos.x < posCamera.x - 370 || thisPos.x > posCamera.x + 370)
									{// ���E�̉�ʊO�ɂ���Ƃ�
										bHit = false;				// �����蔻��`�F�b�N�𖳌��ɂ���
										CBulletPlayer::Uninit();	// �e�̏I������
									}
								}
								else if (CPlayer::TYPE_SHIP == type)
								{// �v���C���[����͂̏ꍇ
									bHit = CScene3DBill::CollisionXZ(pos, (fLength * 2.5f));	// Y���𔲂�������

									if (thisPos.x < posCamera.x - 450)
									{// ���̉�ʊO�ɂ���Ƃ�
										bHit = false;				// �����蔻��`�F�b�N�𖳌��ɂ���
										CBulletPlayer::Uninit();	// �e�̏I������
									}
									else if (thisPos.x > posCamera.x + 450)
									{// ��̉�ʊO�ɂ���Ƃ�
										bHit = false;				// �����蔻��`�F�b�N�𖳌��ɂ���
										CBulletPlayer::Uninit();	// �e�̏I������
									}
								}
								else if (CPlayer::TYPE_TANK == type)
								{// �v���C���[����͂̏ꍇ
									bHit = CScene3DBill::Collision(pos, (fLength * 2.0f));		// �S���̓����蔻��
								}
							}
						}
					}

					if (true == bHit)
					{// �����蔻�肪�L���̏ꍇ
						if (pEnemy->GetState() == CEnemy::STATE_NORMAL)
						{// �ʏ펞�̂݃_���[�W
							CParticle::Create(pos, 5);
							nEnemyLife = pEnemy->SubtractLife(1);		// �G�̃��C�t�����炷
							CGauge::AddSubtract(5);
							pEnemy->SetState(CEnemy::STATE_DAMAGE);		// �G���_���[�W��Ԃɂ���

							CSound *pSound = CManager::GetSound();		// �T�E���h�̏����擾
							pSound->PlaySound(CSound::SOUND_LABEL_HIT);
						}

 						CBulletPlayer::Uninit();	// �e�̏I������
					}
				}
			}
		}
	}
}

//=========================================
// �v���C���[�̒ʏ�e�̕`�揈��
//=========================================
void CBulletPlayerNormal::Draw(void)
{
	CScene3DBill::Draw();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		�v���C���[����e
//
//=========================================
// �R���X�g���N�^
//=========================================
CBulletPlayerSpecial::CBulletPlayerSpecial()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CBulletPlayerSpecial::~CBulletPlayerSpecial()
{
}

//=========================================
// �v���C���[�̓���e�̏���������
//=========================================
HRESULT CBulletPlayerSpecial::Init(D3DXVECTOR3 pos)
{
	CBulletPlayer::Init(pos);

	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{// �e�̐ݒ�
		CScene3DModel *pScene3DModel;					// �I�u�W�F�N�g�̃|�C���^�ϐ�
		pScene3DModel = (CScene3DModel*)GetScene(PLAYER_PRIORITY, nCntScene);		// �C���f�b�N�X���Ɏ擾

		if (pScene3DModel != NULL)
		{// �g���Ă��邩�̃`�F�b�N
			CScene::OBJTYPE objType;					// �I�u�W�F�N�g�̎�ޔ��ʗp�̕ϐ�
			objType = pScene3DModel->GetObjType();		// �I�u�W�F�N�g�̎�ނ��擾

			if (objType == CScene::OBJTYPE_PLAYER)
			{// �I�u�W�F�N�g�̎�ނ��v���C���[�̂Ƃ�
				CPlayer *pPlayer = (CPlayer*)pScene3DModel->GetScene(PLAYER_PRIORITY, nCntScene);	// �v���C���[�̏����i�[
				CPlayer::TYPE type = pPlayer->GetType();		// �v���C���[�̌��݂̌`�̂��擾
				D3DXVECTOR3 thisPos = CScene3DBill::GetPos();	// �e�̈ʒu���擾

				int nBasicAngle = 20;						// �����̌�
				int nNumAngle = 6;							// [nBasicAngle]��[nNumAngle]�Ŋ��������̔{�̌���1���ɒu��	[(2)��nBasicAngle�̒l���ݒu]

				float fAngle = 1.0f / (nBasicAngle / nNumAngle);	// �Ԋu

				switch (type)
				{// �t�H�[���ʂ̓���e�̐ݒ�
				case CPlayer::TYPE_AIRCRAFT:	// �퓬�@
					CScene3DBill::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
					CScene3DBill::SetLife(120);		// �r�[�������������鎞��
					m_move.x = 2.0f;
					break;

				case CPlayer::TYPE_SHIP:		// ���
					CScene3DBill::SetLength(SHIP_SPECIAL_SIZE, SHIP_SPECIAL_SIZE);			// �e�̃T�C�Y�̐ݒ�
					CEffect::Create(thisPos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 30.0f, 60);	// �G�t�F�N�g������

					// �e���o���o���Ɍ���
					if (rand() % 2 == 0)
					{// ���v���
						m_move = D3DXVECTOR3(sinf(D3DX_PI * (1.0f + (fAngle * m_nCntAngle))) * 3.0f,
							0,
							cosf(D3DX_PI * (1.0f + (fAngle * m_nCntAngle))) * 3.0f);
					}
					else
					{// �����v���
						m_move = D3DXVECTOR3(-sinf(D3DX_PI * (0.5f + (fAngle * m_nCntAngle))) * 3.0f,
							0,
							-cosf(D3DX_PI * (0.5f + (fAngle * m_nCntAngle))) * 3.0f);
					}
					m_nCntAngle = (m_nCntAngle + 1) % nBasicAngle;
					break;

				case CPlayer::TYPE_TANK:		// ���
					CScene3DBill::SetLength(TANK_SPECIAL_SIZE, TANK_SPECIAL_SIZE);			// �e�̃T�C�Y�̐ݒ�
					CEffect::Create(thisPos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 30.0f, 60);	// �G�t�F�N�g������
					m_move = D3DXVECTOR3(10.0f, 3.0f, 0.0f);
					break;
				}

			}
		}
	}
	return S_OK;
}

//=========================================
// �v���C���[�̓���e�̏I������
//=========================================
void CBulletPlayerSpecial::Uninit(void)
{
	CBulletPlayer::Uninit();
}

//=========================================
// �v���C���[�̓���e�̍X�V����
//=========================================
void CBulletPlayerSpecial::Update(void)
{
	D3DXVECTOR3 thisPos = CScene3DBill::GetPos();	// �e�̈ʒu
	int nLife;							// �e�̃��C�t���i�[
	nLife = CScene3DBill::GetLife();	// �e�̃��C�t���擾
	nLife--;							// ���C�t�����炷
	CScene3DBill::SetLife(nLife);		// ���C�t�̓K�p

	if (nLife <= 0)
	{// �������s����
		CBulletPlayer::Uninit();
	}
	else
	{// �����Ă�
		thisPos += m_move;

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{// �G�̏��̒T��
			CScene3DModel *pScene3DModel;											// 3D�I�u�W�F�N�g�̃|�C���^�ϐ�
			pScene3DModel = (CScene3DModel*)GetScene(ENEMY_PRIORITY, nCntScene);	// �C���f�b�N�X���Ɏ擾

			if (pScene3DModel != NULL)
			{// null�`�F�b�N
				CScene::OBJTYPE objType;					// �I�u�W�F�N�g�̎�ޔ��ʗp�̕ϐ�
				objType = pScene3DModel->GetObjType();		// �I�u�W�F�N�g�̎�ނ��擾

				if (objType == CScene::OBJTYPE_ENEMY)
				{// �I�u�W�F�N�g�̎�ނ��G�̂Ƃ�
					CEnemy *pEnemy;															// �G�̏����i�[
					pEnemy = (CEnemy*)pScene3DModel->GetScene(ENEMY_PRIORITY, nCntScene);	// �G�̏����擾

					D3DXVECTOR3 pos = pEnemy->GetPos();
					float fLength = pEnemy->GetLength();
					bool bHit = false;			// �����蔻��̊m�F

					for (int nCntScene2 = 0; nCntScene2 < MAX_OBJECT; nCntScene2++)
					{
						CScene3DModel *pScene3DModel2;								// 3D�I�u�W�F�N�g�̃|�C���^�ϐ�
						pScene3DModel2 = (CScene3DModel*)GetScene(PLAYER_PRIORITY, nCntScene2);		// �C���f�b�N�X���Ɏ擾

						if (pScene3DModel2 != NULL)
						{// null�`�F�b�N
							CScene::OBJTYPE objType;					// �I�u�W�F�N�g�̎�ޔ��ʗp�̕ϐ�
							objType = pScene3DModel2->GetObjType();		// �I�u�W�F�N�g�̎�ނ��擾

							if (objType == CScene::OBJTYPE_PLAYER)
							{// �I�u�W�F�N�g�̎�ނ��v���C���[�̂Ƃ�
								CPlayer *pPlayer;															// �v���C���[�̏����i�[
								pPlayer = (CPlayer*)pScene3DModel2->GetScene(PLAYER_PRIORITY, nCntScene2);	// �v���C���[�̏����擾
								D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// �v���C���[�̈ʒu���擾

								CPlayer::TYPE type;			// �v���C���[�̌��݂̌`�Ԃ��i�[
								type = pPlayer->GetType();	// �v���C���[�̌��݂̌`�Ԃ��擾

								if (CPlayer::TYPE_AIRCRAFT == type)
								{// �v���C���[���퓬�@�̏ꍇ
									CEffect::Create(posPlayer, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), AIRCRAFT_SPECIAL_SIZE, 120, CEffect::TYPE_BIIM);	// �G�t�F�N�g������

									CSound *pSound = CManager::GetSound();				// �T�E���h�̏����擾
									pSound->PlaySound(CSound::SOUND_LABEL_BULLET001);
								}
								else if (CPlayer::TYPE_SHIP == type)
								{// �v���C���[����͂̏ꍇ
									bHit = CScene3DBill::CollisionXZ(pos, (fLength * 1.8f));	// Y���𔲂�������
								}
								else if (CPlayer::TYPE_TANK == type)
								{// �v���C���[����Ԃ̏ꍇ
									m_move.y -= 0.3f;
									bHit = CScene3DBill::Collision(pos, (fLength * 1.8f));		// �S���̓����蔻��

									if (nLife <= 1 || thisPos.y <= 0)
									{// ���C�t���Ȃ��Ȃ�Ɣ���������
										CParticle::Create(thisPos, 0);
										CSound *pSound = CManager::GetSound();				// �T�E���h�̏����擾
										pSound->PlaySound(CSound::SOUND_LABEL_BULLET003);

										if (CScene3DBill::Collision(thisPos, 30))
										{// �p�[�e�B�N���̓����蔻��
											if (pEnemy->GetState() == CEnemy::STATE_NORMAL)
											{// �ʏ펞�̂݃_���[�W
												pEnemy->SubtractLife(2);					// �G�̃��C�t�����炷

												pEnemy->SetState(CEnemy::STATE_DAMAGE);		// �G���_���[�W��Ԃɂ���
											}
										}
									}
								}
							}
						}
					}

					if (true == bHit)
					{// �����蔻�肪�L���̏ꍇ
						if (pEnemy->GetState() == CEnemy::STATE_NORMAL)
						{// �ʏ펞�̂݃_���[�W
							pEnemy->SubtractLife(2);					// �G�̃��C�t�����炷

							pEnemy->SetState(CEnemy::STATE_DAMAGE);		// �G���_���[�W��Ԃɂ���

							CSound *pSound = CManager::GetSound();		// �T�E���h�̏����擾
							pSound->PlaySound(CSound::SOUND_LABEL_HIT);
						}
					}
				}
			}
		}

		// �ʒu�̍X�V
		CScene3DBill::SetPos(thisPos);
	}
}

//=========================================
// �v���C���[�̓���e�̕`�揈��
//=========================================
void CBulletPlayerSpecial::Draw(void)
{
	CScene3DBill::Draw();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �G�̒e
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=========================================
// �R���X�g���N�^
//=========================================
CBulletEnemy::CBulletEnemy()
{
	m_nCntAngle = 0;
	m_fAngle = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CBulletEnemy::~CBulletEnemy()
{
}

//=========================================
// �G�̒e�̏���������
//=========================================
HRESULT CBulletEnemy::Init(D3DXVECTOR3 pos)
{
	CBullet::Init(pos);
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_move.x = -BULLET_SPEED;

	return S_OK;
}

//=========================================
// �G�̒e�̏I������
//=========================================
void CBulletEnemy::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=========================================
// �G�̒e�̍X�V����
//=========================================
void CBulletEnemy::Update(void)
{
	bool bDelete = false;				// �폜��������𖞂��������ǂ���
	int nLife;							// �e�̃��C�t���i�[
	nLife = CScene3DBill::GetLife();	// �e�̃��C�t���擾

	m_nCntAngle++;	// �ǔ����鎞�Ԃ̃J�E���g

	nLife--;
	CScene3DBill::SetLife(nLife);

	if (nLife <= 0)
	{// �������s����
		bDelete = true;
	}
	else
	{// �����Ă�
		D3DXVECTOR3 thisPos = CScene3DBill::GetPos();	// �e�̈ʒu���擾
		CEffect::Create(thisPos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), BULLET_EFFECT_SIZE, BULLET_EFFECT_LIFE);	// �G�t�F�N�g������

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			CScene3DModel *pScene3DModel;					// �I�u�W�F�N�g�̃|�C���^�ϐ�
			pScene3DModel = (CScene3DModel*)GetScene(PLAYER_PRIORITY, nCntScene);		// �C���f�b�N�X���Ɏ擾

			if (pScene3DModel != NULL)
			{// �g���Ă��邩�̃`�F�b�N
				CScene::OBJTYPE objType;					// �I�u�W�F�N�g�̎�ޔ��ʗp�̕ϐ�
				objType = pScene3DModel->GetObjType();			// �I�u�W�F�N�g�̎�ނ��擾

				if (objType == CScene::OBJTYPE_PLAYER)
				{// �I�u�W�F�N�g�̎�ނ��v���C���[�̂Ƃ�
					CPlayer *pPlayer;	// �v���C���[�̏����i�[
					pPlayer = (CPlayer*)pScene3DModel->GetScene(PLAYER_PRIORITY, nCntScene);	// �v���C���[�̏����擾
					D3DXVECTOR3 posPlayer = pPlayer->GetPos();					// �v���C���[�̈ʒu���擾
					CPlayer::STATE statePlayer = pPlayer->GetState();			// �v���C���[�̏�Ԃ��擾
					CPlayer::TYPE typePlayer = pPlayer->GetType();				// �v���C���[�̌`�Ԃ��擾
					float fLengthPlayer = pPlayer->GetLength();					// �v���C���[�̃T�C�Y���擾
					D3DXVECTOR3 BulletMove(0, 0, 0);							// �ړI�̊p�x�ւ̈ړ���

					// �v���C���[�̌`�Ԃɂ���Ēe��ς���
					if (CPlayer::TYPE_AIRCRAFT == typePlayer)
					{// �v���C���[�̌`�Ԃ��퓬�@�̏ꍇ
						if (m_nCntAngle < 5)
						{// �v���C���[�ւ̊p�x���v�Z
							m_fAngle = atan2f(posPlayer.x - thisPos.x, posPlayer.y - thisPos.y);
						}

						BulletMove.x = sinf(m_fAngle) * 5.0f;	// X���̈ړ���
						BulletMove.y = cosf(m_fAngle) * 5.0f;	// Y���̈ړ���

						m_move = D3DXVECTOR3(BulletMove.x, BulletMove.y, 0);		// �e�̈ړ���

						thisPos += m_move;	// �ړ��ʂ��ʒu�ɉ��Z����

						for (int nCntHit = 0; nCntHit < 5; nCntHit++)
						{// �����蔻���c�q��ɒu��
							if (CScene3DBill::CollisionXY(D3DXVECTOR3((posPlayer.x - fLengthPlayer * 2.5f) + (nCntHit * fLengthPlayer), posPlayer.y, posPlayer.z), fLengthPlayer * 2.3f) && statePlayer == CPlayer::STATE_NORMAL)
							{// �����蔻��
								bDelete = true;								// �e������
								CLife *pLife = pPlayer->GetLife();			// ���C�t�̃|�C���^���擾
								pLife->AddSubtract(-10);					// �v���C���[�̃��C�t�����炷
								pPlayer->SetState(CPlayer::STATE_DAMAGE);	// �v���C���[���_���[�W��Ԃɂ���

								break;
							}
						}
					}
					else if (CPlayer::TYPE_SHIP == typePlayer)
					{// �v���C���[�̌`�Ԃ���͂̏ꍇ
						thisPos.y = 10.0f;
						if (m_nCntAngle < 5)
						{// �v���C���[�ւ̊p�x���v�Z
							m_fAngle = atan2f(posPlayer.x - thisPos.x, posPlayer.z - thisPos.z);
						}

						BulletMove.x = sinf(m_fAngle) * 5.0f;	// X���̈ړ���
						BulletMove.z = cosf(m_fAngle) * 5.0f;	// Z���̈ړ���

						m_move = D3DXVECTOR3(BulletMove.x, 0.0f, BulletMove.z);		// �e�̈ړ���

						thisPos += m_move;	// �ړ��ʂ��ʒu�ɉ��Z����

						for (int nCntHit = 0; nCntHit < 6; nCntHit++)
						{// �����蔻���c�q��ɒu��
							if (CScene3DBill::CollisionXZ(D3DXVECTOR3((posPlayer.x - fLengthPlayer * 3) + (nCntHit * fLengthPlayer), posPlayer.y, posPlayer.z), fLengthPlayer) && statePlayer == CPlayer::STATE_NORMAL)
							{// �����蔻��
								bDelete = true;								// �e������
								CLife *pLife = pPlayer->GetLife();			// ���C�t�̃|�C���^���擾
								pLife->AddSubtract(-10);					// �v���C���[�̃��C�t�����炷
								pPlayer->SetState(CPlayer::STATE_DAMAGE);	// �v���C���[���_���[�W��Ԃɂ���

								break;
							}
						}
					}
					else if(CPlayer::TYPE_TANK == typePlayer)
					{// �v���C���[�̌`�Ԃ���Ԃ̏ꍇ
						for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
						{// �G�̏��̒T��
							CScene3DModel *pScene3DModel;											// 3D�I�u�W�F�N�g�̃|�C���^�ϐ�
							pScene3DModel = (CScene3DModel*)GetScene(ENEMY_PRIORITY, nCntScene);	// �C���f�b�N�X���Ɏ擾

							if (pScene3DModel != NULL)
							{// null�`�F�b�N
								CScene::OBJTYPE objType;					// �I�u�W�F�N�g�̎�ޔ��ʗp�̕ϐ�
								objType = pScene3DModel->GetObjType();		// �I�u�W�F�N�g�̎�ނ��擾

								if (objType == CScene::OBJTYPE_ENEMY)
								{// �I�u�W�F�N�g�̎�ނ��G�̂Ƃ�
									CEnemy *pEnemy;															// �G�̏����i�[
									pEnemy = (CEnemy*)pScene3DModel->GetScene(ENEMY_PRIORITY, nCntScene);	// �G�̏����擾
									if (pEnemy != NULL)
									{
										CEnemy::TYPE type = pEnemy->GetType();		// �G�̎�ނ��擾

										if (CEnemy::TYPE_AIRCRAFT == type)
										{// �G���퓬�@�̏ꍇ
											m_move.y = -5.0f;
										}
										else
										{// �G����͂���Ԃ̏ꍇ
											thisPos.y = 10.0f;
											if (m_nCntAngle < 5)
											{// �v���C���[�ւ̊p�x���v�Z
												m_fAngle = atan2f(posPlayer.x - thisPos.x, posPlayer.z - thisPos.z);
											}

											BulletMove.x = sinf(m_fAngle) * 3.0f;	// X���̈ړ���
											BulletMove.z = cosf(m_fAngle) * 3.0f;	// Z���̈ړ���

											m_move = D3DXVECTOR3(BulletMove.x, 0.0f, BulletMove.z);		// �e�̈ړ���
										}
									}
									thisPos += m_move;	// �ړ��ʂ��ʒu�ɉ��Z����
								}
							}
						}

						for (int nCntHit = 0; nCntHit < 3; nCntHit++)
						{// �����蔻���c�q��ɒu��
							if (CScene3DBill::Collision(D3DXVECTOR3((posPlayer.x - fLengthPlayer) + (nCntHit * fLengthPlayer), posPlayer.y, posPlayer.z), fLengthPlayer) && statePlayer == CPlayer::STATE_NORMAL && !bDelete)
							{// �����蔻��
								bDelete = true;			// �e������

								CLife *pLife = pPlayer->GetLife();			// ���C�t�̃|�C���^���擾
								pLife->AddSubtract(-10);					// �v���C���[�̃��C�t�����炷

								pPlayer->SetState(CPlayer::STATE_DAMAGE);	// �v���C���[���_���[�W��Ԃɂ���

								CSound *pSound = CManager::GetSound();		// �T�E���h�̏����擾
								pSound->PlaySound(CSound::SOUND_LABEL_HIT1);
							}
						}
					}
				}
			}
		}
		// �ʒu�̍X�V
		CScene3DBill::SetPos(thisPos);

		if (thisPos.y <= 0)
		{// �n�ʂɂ���
			bDelete = true;
		}
	}

	if (bDelete)
	{// �e�̏I������
		Uninit();
	}
}

//=========================================
// �G�̒e�̕`�揈��
//=========================================
void CBulletEnemy::Draw(void)
{
	CScene3DBill::Draw();
}

