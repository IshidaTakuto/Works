//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "bullet.h"
#include "renderer.h"
#include "life.h"
#include "stock.h"
#include "effect.h"
#include "game.h"
#include "camera.h"
#include "field.h"
#include "gauge.h"
#include "scene2D.h"
#include "tutorial.h"
#include "particle.h"
#include "scene3DIndex.h"
#include "meshCylinder.h"
#include "formTime.h"
#include "sound.h"

//==================================
// �}�N����`
//==================================
#define PLAYER_MODEL_AIRCRAFT		"data/MODEL/aircraft.x"
#define PLAYER_MODEL_SHIP			"data/MODEL/ship.x"
#define PLAYER_MODEL_TANK			"data/MODEL/tank.x"
#define MOVE_SPEED					(5.0f)	// �ړ��̑��x
#define TRANSFORM_TIME				(120)	// �ό`�ł���܂ł̎���
#define NUM_SHIP_SPBULLET			(20)	// ��͂̓���e�̐�
#define BULLET_INTERVAL_PLAYER		(20)	// �e�̔��ˊԊu

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9	CPlayer::m_pTexture[TYPE_MAX] = {};	// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH			CPlayer::m_pMesh[TYPE_MAX] = {};	// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		CPlayer::m_pBuffMat[TYPE_MAX] = {};	// �}�e���A�����ւ̃|�C���^
DWORD				CPlayer::m_nNumMat[TYPE_MAX] = {};	// �}�e���A�����̐�

//==================================
// ��������
//==================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, TYPE type)
{
	CPlayer *pPlayer = NULL;

	switch (type)
	{
	case TYPE_AIRCRAFT:
		pPlayer = new CPlayerAircraft;	// ���������m��
		break;

	case TYPE_SHIP:
		pPlayer = new CPlayerShip;	// ���������m��
		break;

	case TYPE_TANK:
		pPlayer = new CPlayerTank;	// ���������m��
		break;
	}

	// ���f�����̐ݒ�
	pPlayer->BindInfo(m_pTexture[type], m_pMesh[type], m_pBuffMat[type], m_nNumMat[type]);

	// ����������
	pPlayer->Init(pos);

	// ���݂̌`�Ԃ̐ݒ�
	pPlayer->m_type = type;

	// �v���C���[���̐ݒ�
	CGame::SetPlayer(pPlayer);

	return pPlayer;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CPlayer::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(PLAYER_MODEL_AIRCRAFT,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[0],
		NULL,
		&m_nNumMat[0],
		&m_pMesh[0]);

	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// �e�N�X�`���̐���
	pMat = (D3DXMATERIAL*)m_pBuffMat[0]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[0]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(PLAYER_MODEL_SHIP,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[1],
		NULL,
		&m_nNumMat[1],
		&m_pMesh[1]);

	// �e�N�X�`���̐���
	pMat = (D3DXMATERIAL*)m_pBuffMat[1]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[1]);


	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(PLAYER_MODEL_TANK,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[2],
		NULL,
		&m_nNumMat[2],
		&m_pMesh[2]);

	// �e�N�X�`���̐���
	pMat = (D3DXMATERIAL*)m_pBuffMat[2]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[2]);

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CPlayer::Unload(void)
{
	for (int nCntInfo = 0; nCntInfo < TYPE_MAX; nCntInfo++)
	{// �`�ԕ����[�v
		if (NULL != m_pTexture)
		{// �e�N�X�`���̔j��
			m_pTexture[nCntInfo]->Release();
			m_pTexture[nCntInfo] = NULL;
		}

		if (NULL != m_pMesh[nCntInfo])
		{// ���b�V�����̔j��
			m_pMesh[nCntInfo]->Release();
			m_pMesh[nCntInfo] = NULL;
		}

		if (NULL != m_pBuffMat[nCntInfo])
		{// �}�e���A�����̔j��
			m_pBuffMat[nCntInfo]->Release();
			m_pBuffMat[nCntInfo] = NULL;
		}

		if (NULL != m_nNumMat[nCntInfo])
		{// �}�e���A����������������
			m_nNumMat[nCntInfo] = 0;
		}
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CPlayer::CPlayer(int nPriority) : CScene3DModel(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_state = STATE_APPEAR;					// ���
	m_nCntState = 0;						// ��Ԃ��ς��܂ł̃J�E���^
	m_type = TYPE_AIRCRAFT;					// �`��
	m_bDisp = true;							// �`�悷�邩�ǂ���
	m_nCntBullet = 0;						// �e�̔��ˊԊu
	m_pLife = NULL;							// ���C�t�o�[
	m_pFormTime = NULL;						// �ό`�o�[
}

//=========================================
// �f�X�g���N�^
//=========================================
CPlayer::~CPlayer()
{
}

//=========================================
// ����������
//=========================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);

	// ����������
	CScene3DModel::Init();

	// �ʒu��ݒ�
	CScene3DModel::SetPos(pos);
	CScene3DModel::SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

	if (NULL == m_pLife)
	{// ���C�t�Q�[�W�̐���
		m_pLife = CLife::Create();
	}

	if (NULL == m_pFormTime)
	{// �ό`�Q�[�W�̐���
		m_pFormTime = CFormTime::Create();
	}

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CPlayer::Uninit(void)
{
	if (NULL != m_pFormTime)
	{// �ό`�Q�[�W�̔j��
		m_pFormTime->Uninit();
	}

	CScene3DModel::Uninit();
	CGame::NullPlayer();
}

//=========================================
// �X�V����
//=========================================
void CPlayer::Update(void)
{
	int nLife = m_pLife->GetLife();		// ���C�t�̎擾
	int nStock = CStock::GetStock();	// �X�g�b�N�̎擾
	bool bDelete = false;
	CManager::MODE mode = CManager::GetMode();

	// ���
	switch (m_state)
	{
	case STATE_APPEAR:		// �o�����
		m_nCntState++;		// �X�e�[�g���Ԃ��J�E���g����

		if (m_nCntState % 120 == 0)
		{// �ʏ펞�ɖ߂�
			m_state = STATE_NORMAL;
			CScene3DModel::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�����Ƃɖ߂�
			m_nCntState = 0;
		}
		break;

	case STATE_NORMAL:		// �ʏ���
		break;

	case STATE_DAMAGE:		// �_���[�W���
		m_nCntState++;		// �X�e�[�g���Ԃ��J�E���g����
		CScene3DModel::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// �Ԃ�����

		if (m_nCntState % 40 == 0 && nLife > 0)
		{// �ʏ펞�ɖ߂�
			m_state = STATE_NORMAL;
			CScene3DModel::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�����Ƃɖ߂�
			m_nCntState = 0;
		}
		else if (nLife <= 0)
		{// ���C�t���Ȃ��Ȃ����Ƃ�
			m_state = STATE_DEATH;	// ���S��Ԃɂ���
		}
		break;

	case STATE_DEATH:		// ���S���
		if (nStock <= 0)
		{// �X�g�b�N��0�ȉ��̂Ƃ�
			if (mode == CManager::MODE_TUTORIAL)
			{// �`���[�g���A���̏ꍇ�X�g�b�N�𑝂₷
				CStock::AddSubtract(1);
			}
			else
			{
				bDelete = true;		// �t���O�𗧂Ă�
				CGame::SetGameState(CGame::STATE_END);
			}
		}
		else
		{// �X�g�b�N���܂�����Ƃ�
			m_bDisp = false;	// �`�悵�Ȃ���Ԃɂ���

			m_nCntState++;		// �����܂ł̃J�E���^

			if (m_nCntState % 50 == 0)
			{// ��莞�Ԍo��
				m_bDisp = true;	// �`�悷���Ԃɂ���
				CScene3DModel::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));	// �������ɂ���
				m_state = STATE_APPEAR;			// �o����Ԃɂ���
				CStock::AddSubtract(-1);		// �X�g�b�N������炷
				m_pLife->AddSubtract(MAX_LIFE);	// ���C�t���ő�l�ɂ���
				m_nCntState = 0;				// �X�e�[�g�J�E���^��������
			}
		}

		break;
	}

	D3DXVECTOR3 pos = CScene3DModel::GetPos();					// �ʒu�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// �W���C�p�b�h�̎擾
	CSound *pSound = CManager::GetSound();						// �T�E���h�̏����擾

	// �e������
	if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_2) || pKeyboard->GetPress(DIK_SPACE))
	{// �ʏ�e
		if (m_nCntBullet == 0)
		{// ���Ԋu�Ŕ���
			CBullet::Create(pos, CBullet::TYPE_PLAYERNORMAL);
			pSound->PlaySound(CSound::SOUND_LABEL_BULLET000);
		}

		// ���ˊԊu�̃J�E���g
		m_nCntBullet = (1 + m_nCntBullet) % BULLET_INTERVAL_PLAYER;
	}

	// �J�����Ɠ������x�Ői��
	pos.x -= sinf(D3DX_PI * -0.5f) * 1.1f;

	if (pos.z < -400.0f)
	{// ���͈̔͂𒴂���ƈʒu��߂�
		pos.z = -400.0f;
	}
	else if (pos.z > 400.0f)
	{// ���͈̔͂𒴂���ƈʒu��߂�
		pos.z = 400.0f;
	}

	// �ʒu�̍X�V
	CScene3DModel::SetPos(pos);

	// �ό`�ł��邩�ǂ���
	bool bTransform = m_pFormTime->GetFormSuccess();

	if (bTransform == true)
	{// �ό`�ł���
		bool bTransForm = false;	// �ό`���邩�ǂ����̃t���O

		if (bTransForm == false)
		{// �ό`���łȂ��Ƃ�
			// �ό`
			if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_21) || pKeyboard->GetTrigger(DIK_LEFT) == true)
			{// �퓬�@
				m_type = TYPE_AIRCRAFT;
				bTransForm = true;
			}
			else if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_19) || pKeyboard->GetTrigger(DIK_DOWN) == true)
			{// ���
				m_type = TYPE_TANK;
				bTransForm = true;
			}
			else if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_20) || pKeyboard->GetTrigger(DIK_RIGHT) == true)
			{// ���
				m_type = TYPE_SHIP;
				bTransForm = true;
			}
		}

		if (bTransForm == true)
		{// �ό`
			m_pFormTime->FormTimeReset();	// �ό`�Q�[�W�̒l�����Z�b�g

			D3DXVECTOR3 pos = CScene3DModel::GetPos();
			TYPE type = m_type;

			// ���̃��f��������
			Uninit();
			bDelete = false;

			if (TYPE_AIRCRAFT == type)
			{// �퓬�@�ɂȂ�ꍇ
				pos.y = 180;
				pos.z = 0;
			}

			// �w�肵����ނ𐶐�
			Create(pos, type);
			CParEmitter::Create(pos, 9);	// �p�[�e�B�N�����o��
		}

		if (bDelete == true)
		{// ����
			Uninit();	// �I������
		}
	}
}

//=========================================
// �`�揈��
//=========================================
void CPlayer::Draw(void)
{
	if (m_bDisp == true)
	{
		CScene3DModel::Draw();
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �퓬�@
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=========================================
// �퓬�@�̃R���X�g���N�^
//=========================================
CPlayerAircraft::CPlayerAircraft(int nPriority) : CPlayer(nPriority)
{
}

//=========================================
// �퓬�@�̃f�X�g���N�^
//=========================================
CPlayerAircraft::~CPlayerAircraft()
{
}

//=========================================
// �퓬�@�̏���������
//=========================================
HRESULT CPlayerAircraft::Init(D3DXVECTOR3 pos)
{
	CPlayer::Init(pos);

	return S_OK;
}

//=========================================
// �퓬�@�̏I������
//=========================================
void CPlayerAircraft::Uninit(void)
{
	CPlayer::Uninit();
}

//=========================================
// �퓬�@�̍X�V����
//=========================================
void CPlayerAircraft::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// �W���C�p�b�h�̎擾

	D3DXVECTOR3 pos = CScene3DModel::GetPos();	// �ʒu�̎擾
	D3DXVECTOR3 posOld = pos;

	if (m_state != STATE_DEATH)
	{// ���S��Ԃ̂Ƃ��ȊO
		// �L�[���͂ɂ��ړ�---------------------------------------------------------------------------------------------
		if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_13) || pKeyboard->GetPress(DIK_A))
		{// �������ֈړ�
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// ����Ɉړ�
				m_move.x += sinf(-D3DX_PI * 0.25) * MOVE_SPEED;
				m_move.y += cosf(-D3DX_PI * 0.25) * MOVE_SPEED;
			}
			else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))
			{// �����Ɉړ�
				m_move.x += sinf(-D3DX_PI * 0.75f) * MOVE_SPEED;
				m_move.y += cosf(-D3DX_PI * 0.75f) * MOVE_SPEED;
			}
			else
			{// ���Ɉړ�
				m_move.x -= MOVE_SPEED;	//�|���S�������Ɉړ�
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_12) || pKeyboard->GetPress(DIK_D))	//-------------------------- �� ------------------------------
		{// �������ֈړ�
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// �E��Ɉړ�
				m_move.x += sinf(D3DX_PI * 0.25) * MOVE_SPEED;
				m_move.y += cosf(D3DX_PI * 0.25) * MOVE_SPEED;
			}
			else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))
			{// �E���Ɉړ�
				m_move.x += sinf(D3DX_PI * 0.75f) * MOVE_SPEED;
				m_move.y += cosf(D3DX_PI * 0.75f) * MOVE_SPEED;
			}
			else
			{//�E�Ɉړ�
				m_move.x += MOVE_SPEED;	//�|���S�����E�Ɉړ�
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))	//-------------------------- �� ------------------------------
		{// ��Ɉړ�
			m_move.y += MOVE_SPEED;
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))	//-------------------------- �� ------------------------------
		{// ���Ɉړ�
			m_move.y -= MOVE_SPEED;
		}

		// �ړ��̊���
		m_move.x = m_move.x * 0.5f;
		m_move.y = m_move.y * 0.5f;

		if (m_move.x > 0.1f || m_move.x < -0.1f ||
			m_move.y > 0.1f || m_move.y < -0.1f)
		{// �ړ��ʂ����ȏ�̂Ƃ��G�t�F�N�g������
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, pos.y, pos.z), 2);
		}
		else
		{// �ړ��ʂ��Ȃ���
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, pos.y, pos.z), 7);
		}

		// �ړ�������
		pos += m_move;

		// �J�����͈̔͊O�ɏo�Ȃ��悤�ɂ���
		CCamera *pCamera = CManager::GetCamera();
		D3DXVECTOR3 posCamera = pCamera->GetPosV();

		if (pos.x < posCamera.x - 310 || pos.x > posCamera.x + 310)
		{// ���E�̉�ʒ[�ɂ���Ƃ�
			pos.x = posOld.x;	// �ʒu��߂�
			m_move.x = 0;		// �ړ��ʂ�0�ɂ���
		}
		if (pos.y > 380 || pos.y < 90)
		{// �㉺�̉�ʒ[�ɂ���Ƃ�
			pos.y = posOld.y;	// �ʒu��߂�
			m_move.y = 0;		// �ړ��ʂ�0�ɂ���
		}
	}

	if ((pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_3) || pKeyboard->GetTrigger(DIK_B)) && MAX_GAUGE <= CGauge::Get())
	{// ����e
		CBullet::Create(pos, CBullet::TYPE_PLAYERSPECIAL);
		CGauge::AddSubtract(-MAX_GAUGE);
	}

	// �ʒu�̍X�V
	CScene3DModel::SetPos(pos);

	// �v���C���[�̍X�V����
	CPlayer::Update();
}

//=========================================
// �퓬�@�̕`�揈��
//=========================================
void CPlayerAircraft::Draw(void)
{
	CPlayer::Draw();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=========================================
// ��͂̃R���X�g���N�^
//=========================================
CPlayerShip::CPlayerShip(int nPriority) : CPlayer(nPriority)
{
	m_nCntSpBullet = 0;
	m_bSpShoot = false;
}

//=========================================
// ��͂̃f�X�g���N�^
//=========================================
CPlayerShip::~CPlayerShip()
{
}

//=========================================
// ��͂̏���������
//=========================================
HRESULT CPlayerShip::Init(D3DXVECTOR3 pos)
{
	CPlayer::Init(pos);

	return S_OK;
}

//=========================================
// ��͂̏I������
//=========================================
void CPlayerShip::Uninit(void)
{


	CPlayer::Uninit();
}

//=========================================
// ��͂̍X�V����
//=========================================
void CPlayerShip::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// �W���C�p�b�h�̎擾

	D3DXVECTOR3 pos = CScene3DModel::GetPos();	// �ʒu�̎擾
	D3DXVECTOR3 posOld = pos;

	if (m_state != STATE_DEATH)
	{// ���S��Ԃ̂Ƃ��ȊO
		// �L�[���͂ɂ��ړ�---------------------------------------------------------------------------------------------
		if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_13) || pKeyboard->GetPress(DIK_A))
		{// �������ֈړ�
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// ����Ɉړ�
				m_move.x += sinf(D3DX_PI * 0.25) * MOVE_SPEED;
				m_move.z += cosf(D3DX_PI * 0.25) * MOVE_SPEED;
			}
			else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))
			{// �����Ɉړ�
				m_move.x += sinf(-D3DX_PI * 0.25) * MOVE_SPEED;
				m_move.z += cosf(-D3DX_PI * 0.25) * MOVE_SPEED;
			}
			else
			{//���Ɉړ�
				m_move.z += MOVE_SPEED;	//�|���S�������Ɉړ�
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_12) || pKeyboard->GetPress(DIK_D))	//-------------------------- �� ------------------------------
		{// �������ֈړ�
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// �E��Ɉړ�
				m_move.x += sinf(D3DX_PI * 0.75f) * MOVE_SPEED;
				m_move.z += cosf(D3DX_PI * 0.75f) * MOVE_SPEED;
			}
			else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))
			{// �E���Ɉړ�
				m_move.x += sinf(-D3DX_PI * 0.75f) * MOVE_SPEED;
				m_move.z += cosf(-D3DX_PI * 0.75f) * MOVE_SPEED;
			}
			else
			{//�E�Ɉړ�
				m_move.z -= MOVE_SPEED;	//�|���S�����E�Ɉړ�
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))	//-------------------------- �� ------------------------------
		{// ��Ɉړ�
			m_move.x += MOVE_SPEED;
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))	//-------------------------- �� ------------------------------
		{// ���Ɉړ�
			m_move.x -= MOVE_SPEED;
		}

		// �ړ��̊���
		m_move.x = m_move.x * 0.5f;
		m_move.z = m_move.z * 0.5f;
		m_move.y = -1;

		if (m_move.x > 0.1f || m_move.x < -0.1f ||
			m_move.z > 0.1f || m_move.z < -0.1f)
		{// �ړ��ʂ����ȏ�̂Ƃ��G�t�F�N�g������
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, pos.y, pos.z), 3);
		}

		// �g����o��
		CEffect::Create(D3DXVECTOR3(pos.x + 10, pos.y, pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 20, CEffect::TYPE_RIPPLES);

		// �ړ�������
		pos += m_move;

		if (pos.z > 450 || pos.z < -450)
		{// ���̈ړ��͈�
			pos.z = posOld.z;
			m_move.z = 0;
		}

		// �n�ʂɂ߂荞�܂Ȃ��悤�ɂ���
		float vtxY = CScene3DModel::GetMinVtxY();	// ��ԉ��̒��_���W

		CField *pField[MAX_FIELD];

		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// ���[�h���Q�[���̂Ƃ�
				pField[nCntField] = CGame::GetField(nCntField);		// �t�B�[���h�̏����擾
			}
			else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
			{
				pField[nCntField] = CTutorial::GetField(nCntField);		// �t�B�[���h�̏����擾
			}

			if (NULL != pField[nCntField])
			{// ��񂪂�����̂̂�
				CField::TYPE type = pField[nCntField]->GetType();	// �t�B�[���h�̎�ނ̎擾

				if (pField[nCntField]->Collision(&pos, vtxY) == true && type == CField::TYPE_GROUND && m_state == STATE_NORMAL)
				{// ����ɂ���
					m_pLife->AddSubtract(-1);	// ���C�t�����炷
					m_state = STATE_DAMAGE;
				}
			}
		}

		// �J�����͈̔͊O�ɏo�Ȃ��悤�ɂ���
		CCamera *pCamera = CManager::GetCamera();
		D3DXVECTOR3 posCamera = pCamera->GetPosV();

		if (pos.x < posCamera.x - 150)
		{// ���̉�ʒ[�ɂ���Ƃ�
			pos.x = posCamera.x - 150;	// �ʒu��߂�
			m_move.x = 0;		// �ړ��ʂ�0�ɂ���
		}
		else if (pos.x > posCamera.x + 250)
		{// ��̉�ʒ[�ɂ���Ƃ�
			pos.x = posCamera.x + 250;	// �ʒu��߂�
			m_move.x = 0;		// �ړ��ʂ�0�ɂ���
		}

		// �ʒu�̍X�V
		CScene3DModel::SetPos(pos);

		int nGauge = CGauge::Get();	// �Q�[�W�ʂ��擾

		// �e������
		if ((pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_3) || pKeyboard->GetTrigger(DIK_B)) && MAX_GAUGE <= nGauge && !m_bSpShoot)
		{// ����e
			m_bSpShoot = true;
			CGauge::AddSubtract(-MAX_GAUGE);
		}

		if (m_nCntSpBullet >= ((NUM_SHIP_SPBULLET + 1) * 5))
		{// �����I������Ƃ�
			m_bSpShoot = false;
			m_nCntSpBullet = 0;
		}

		if (m_bSpShoot == true)
		{// �����Ă����
			if (0 == m_nCntSpBullet % 5)
			{// ��莞�Ԍo��
				CBullet::Create(pos, CBullet::TYPE_PLAYERSPECIAL);
			}

			m_nCntSpBullet++;
		}
	}

	CPlayer::Update();	// �v���C���[�̍X�V����
}

//=========================================
// ��͂̕`�揈��
//=========================================
void CPlayerShip::Draw(void)
{
	CPlayer::Draw();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=========================================
// ��Ԃ̃R���X�g���N�^
//=========================================
CPlayerTank::CPlayerTank(int nPriority) : CPlayer(nPriority)
{
}

//=========================================
// ��Ԃ̃f�X�g���N�^
//=========================================
CPlayerTank::~CPlayerTank()
{
}

//=========================================
// ��Ԃ̏���������
//=========================================
HRESULT CPlayerTank::Init(D3DXVECTOR3 pos)
{
	CPlayer::Init(pos);

	return S_OK;
}

//=========================================
// ��Ԃ̏I������
//=========================================
void CPlayerTank::Uninit(void)
{
	CPlayer::Uninit();
}

//=========================================
// ��Ԃ̍X�V����
//=========================================
void CPlayerTank::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// �W���C�p�b�h�̎擾

	D3DXVECTOR3 pos = CScene3DModel::GetPos();	// �ʒu�̎擾
	D3DXVECTOR3 posOld = pos;

	if (m_state != STATE_DEATH)
	{// ���S��Ԃ̂Ƃ��ȊO
		// �L�[���͂ɂ��ړ�---------------------------------------------------------------------------------------------
		if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_13) || pKeyboard->GetPress(DIK_A))
		{// �������ֈړ�
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// ����Ɉړ�
				m_move.x += sinf(D3DX_PI * 0.25) * (MOVE_SPEED * 0.5f);
				m_move.z += cosf(D3DX_PI * 0.25) * MOVE_SPEED;
			}
			else
			{//���Ɉړ�
				m_move.z += MOVE_SPEED;	//�|���S�������Ɉړ�
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_12) || pKeyboard->GetPress(DIK_D))	//-------------------------- �� ------------------------------
		{// �������ֈړ�
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// �E��Ɉړ�
				m_move.x += sinf(D3DX_PI * 0.75f) * (MOVE_SPEED * 0.5f);
				m_move.z += cosf(D3DX_PI * 0.75f) * MOVE_SPEED;
			}
			else
			{//�E�Ɉړ�
				m_move.z -= MOVE_SPEED;	//�|���S�����E�Ɉړ�
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))	//-------------------------- �� ------------------------------
		{// �O�Ɉړ�
			m_move.x += MOVE_SPEED * 0.5f;
		}

		if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))
		{// ���x�𗎂Ƃ�
			pos.x -= sinf(D3DX_PI * -0.5f) * 0.5f;
		}
		else
		{
			pos.x -= sinf(D3DX_PI * -0.5f) * 1.1f;
		}

		// �ړ��̊���
		m_move.x = m_move.x * 0.5f;
		m_move.z = m_move.z * 0.5f;
		m_move.y -= 1;

		if (m_move.x > 0.1f || m_move.x < -0.1f ||
			m_move.z > 0.1f || m_move.z < -0.1f)
		{// �ړ��ʂ����ȏ�̂Ƃ��G�t�F�N�g������
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z), 4);
		}
		else
		{// �ړ��ʂ��Ȃ���
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z), 8);
		}

		// �ړ�������
		pos += m_move;

		if (pos.z > 450 || pos.z < -450)
		{// ���̈ړ��͈�
			pos.z = posOld.z;
			m_move.z = 0;
		}

		// �n�ʂɂ߂荞�܂Ȃ��悤�ɂ���
		float vtxY = CScene3DModel::GetMinVtxY();	// ��ԉ��̒��_���W
		CField *pField[MAX_FIELD];

		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// ���[�h���Q�[���̂Ƃ�
				pField[nCntField] = CGame::GetField(nCntField);		// �t�B�[���h�̏����擾
			}
			else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
			{
				pField[nCntField] = CTutorial::GetField(nCntField);		// �t�B�[���h�̏����擾
			}

			if (NULL != pField[nCntField])
			{// ��񂪂�����̂̂�
				CField::TYPE type = pField[nCntField]->GetType();	// �t�B�[���h�̎�ނ̎擾

				if (pField[nCntField]->Collision(&pos, -vtxY) == true && type == CField::TYPE_SEA && m_state == STATE_NORMAL)
				{// �C��ɂ���
					m_pLife->AddSubtract(-1);	// ���C�t�����炷
					m_state = STATE_DAMAGE;
				}
			}
		}

		int nGauge = CGauge::Get();	// �Q�[�W�ʂ��擾

		// �e������
		if ((pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_3) || pKeyboard->GetTrigger(DIK_B)) && MAX_GAUGE <= nGauge)
		{// ����e
			CBullet::Create(D3DXVECTOR3(pos.x,pos.y + 20.f,pos.z), CBullet::TYPE_PLAYERSPECIAL);
			CGauge::Set(0);
		}
	}

	// �ʒu�̍X�V
	CScene3DModel::SetPos(pos);

	CPlayer::Update();	// �v���C���[�̍X�V����
}

//=========================================
// ��Ԃ̕`�揈��
//=========================================
void CPlayerTank::Draw(void)
{
	CPlayer::Draw();
}
