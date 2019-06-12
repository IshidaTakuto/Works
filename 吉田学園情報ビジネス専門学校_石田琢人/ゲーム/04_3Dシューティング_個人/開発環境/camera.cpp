//=============================================================================
//
// �J�������� [camera.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "player.h"
#include "game.h"
#include "scene3DIndex.h"
#include "meshCylinder.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE_ANGLE	(0.01f)
#define TITLE_MOVE	(0.001f)
#define MOVE_CAMERA (1.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// �J�����̏���������
//=============================================================================
void CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 245.0f, -500.0f);
	m_posR = D3DXVECTOR3(0.0f, 200.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 300.0f;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CCamera::Update(void)
{
	// �V�����_�[���J�����̈ʒu�ɍ��킹�ē�����
	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		CScene3DIndex *pSceneIdx;					// �I�u�W�F�N�g�̃|�C���^�ϐ�
		pSceneIdx = (CScene3DIndex*)CScene::GetScene(CYLINDER_PRIORITY, nCntScene);		// �C���f�b�N�X���Ɏ擾

		if (pSceneIdx != NULL)
		{// �g���Ă��邩�̃`�F�b�N
			CScene::OBJTYPE objType;					// �I�u�W�F�N�g�̎�ޔ��ʗp�̕ϐ�
			objType = pSceneIdx->GetObjType();			// �I�u�W�F�N�g�̎�ނ��擾

			if (objType == CScene::OBJTYPE_CYLINDER)
			{// �I�u�W�F�N�g�̎�ނ��v���C���[�̂Ƃ�
				CCylinder *pCylinder = (CCylinder*)CScene::GetScene(CYLINDER_PRIORITY, nCntScene);

				// �V�����_�[�̈ʒu�X�V
				pCylinder->SetPos(D3DXVECTOR3(m_posV.x, -1000.0f, 0.0f));
				break;
			}
		}
	}

	CManager::MODE mode = CManager::GetMode();

	switch (mode)
	{
	case CManager::MODE_TITLE:
		// �J�����̌�����ς���
		m_rot.y = D3DX_PI * -0.5f;

		// X���͏�ɓ�����
		m_posV.x += 1.0f;
		m_posR.x = m_posV.x + 100;
		m_posV.y = 100.0f;
		m_posV.z = 0.0f;

		if (m_posV.x > 1000)
		{// ���܂Ői�񂾂�
			if (m_posR.y < m_posV.y)
			{// ���܂ŃJ�����̒����_���グ��
				m_posR.y += 5;
			}
			else if (m_posR.y >= m_posV.y)
			{// �l�������Ă��܂����Ƃ�
				m_posR.y = m_posV.y;
			}
		}
		else
		{
			m_posR.y = 0.0f;
		}
		break;

	case CManager::MODE_RESULT:
		// �J�����̌�����ς���
		m_rot.y = 0.0f;

		m_posV.x += 2.0f;
		m_posR = D3DXVECTOR3(m_posV.x, m_posV.y, 0.0f);

		break;

	case CManager::MODE_GAMEOVER:
		// �J�����̌�����ς���
		m_rot.y = 0.0f;

		m_posR = D3DXVECTOR3(0.0f, 0.0f, 10.0f);
		m_posV = D3DXVECTOR3(0.0f, 50.0f, -100.0f);

		break;

	case CManager::MODE_RANKING:
		// �J�����̌�����ς���
		m_rot.y = D3DX_PI * -0.5f;

		// X���͏�ɓ�����
		m_posV.x += 1.0f;
		m_posV.y = 100.0f;
		m_posV.z = 0.0f;
		m_posR.x = m_posV.x + 100;
		m_posR.y = m_posV.y;
		m_posR.z = 0.0f;

		break;
	}

	CPlayer *pPlayer = CGame::GetPlayer();	// �v���C���[�̏����擾

	if (NULL != pPlayer)
	{// �v���C���[�̏��̎擾
		D3DXVECTOR3 pos = pPlayer->GetPos();
		D3DXVECTOR3 rot = pPlayer->GetRot();
		CPlayer::TYPE type = pPlayer->GetType();

		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

		switch (type)
		{
		case CPlayer::TYPE_AIRCRAFT:
			if (m_posV.z > -500)
			{// Z���W�����߂��ʒu�ɍs���܂œ�����
				m_posV.z -= 5;
			}
			if (m_posV.z < -490)
			{// Z���W�����߂��ʒu�ɍs���܂œ�����
				m_posV.z += 5;
			}

			if (m_posV.y < 200)
			{// Y���W�����߂��ʒu�ɍs���܂œ�����
				m_posV.y += 5;
			}
			if (m_posV.y > 245)
			{// Y���W�����߂��ʒu�ɍs���܂œ�����
				m_posV.y -= 5;
			}

			// �J�����̌�����ς���
			m_rot.y = 0.0f;

			// X���͏�ɓ�����
			m_posV.x -= sinf((D3DX_PI * -0.5f) + m_rot.y) * 1.1f;
			m_posR.x = m_posV.x - sinf(m_rot.y);

			m_posR.y = 200.0f;
			m_posR.z = 0;

			m_fLength = 300.0f;
			break;

		case CPlayer::TYPE_SHIP:
			if (m_posV.y <= 745)
			{// Y���W�����߂��ʒu�ɍs���܂œ�����
				m_posV.y += 5;
			}

			// �J�����̌�����ς���
			m_rot.y = D3DX_PI * -0.5f;

			m_posR.y = 0;
			m_posV.z = pos.z;
			m_posR.z = pos.z;

			// X���͏�ɓ�����
			m_posV.x += 1.0f;
			m_posR.x = m_posV.x - sinf(m_rot.y);
			break;

		case CPlayer::TYPE_TANK:
			// �J�����̌�����ς���
			m_rot.y = D3DX_PI * -0.5f;

			m_posRDest.x = pos.x + sinf(rot.y) * 1.0f;
			m_posRDest.y = pos.y + 30;
			m_posRDest.z = pos.z + cosf(rot.y) * 1.0f;

			m_posVDest.x = pos.x + sinf(m_rot.y) * (m_fLength * 0.7f);
			m_posVDest.y = pos.y + 60.0f;
			m_posVDest.z = pos.z + cosf(m_rot.y) * (m_fLength * 0.7f);

			m_posR.x += (m_posRDest.x - m_posR.x) * 0.1f;
			m_posR.y += (m_posRDest.y - m_posR.y) * 0.1f;
			m_posR.z += (m_posRDest.z - m_posR.z) * 0.1f;

			m_posV.x += (m_posVDest.x - m_posV.x) * 0.15f;
			m_posV.y += (m_posVDest.y - m_posV.y) * 0.15f;
			m_posV.z += (m_posVDest.z - m_posV.z) * 0.15f;

			break;
		}
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::Set(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),							// ��p
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// �A�X�y�N�g��
								10.0f,											// �J�����͈̔́i��O�j
								8000.0f);										// �J�����͈̔́i���s�j

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}
