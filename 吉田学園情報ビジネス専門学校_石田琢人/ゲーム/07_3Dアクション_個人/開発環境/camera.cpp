//=============================================================================
//
// �J�������� [camera.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "debugProc.h"
#include "player.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_MOVE_HEIGHT	(100.0f)		// �v���C���[�Ƃ̍��ō����𓮂���
#define CAMERA_HEIGHT_SPEED	(0.1f)			// �����𓮂������x
#define CAMERA_ZOOM_LENGTH	(-500.0f)		// �Y�[�������Ƃ��̋���
#define CAMERA_LENGTH		(-600.0f)		// �J�����̋���
#define CAMERA_ZOOM			(5.0f)			// �Y�[�����鑬�x

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
	m_fLookHeight = 0.0f;

#ifdef _DEBUG
	m_bProj = false;
#endif
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 80.0f, CAMERA_LENGTH);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = m_posV.z;
	m_fLookHeight = m_posV.y;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
	// �v���C���[��T��
	CScene *pScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CPlayer *pPlayer = NULL;

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_PLAYER)
		{// �v���C���[�������Ƃ�
			pPlayer = (CPlayer*)pScene;

			// �v���C���[�̈ʒu�ƌ���
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (pPlayer != NULL)
			{// �v���C���[�̏����擾
				pos = pPlayer->GetPos();
				rot = pPlayer->GetRot();
			}

			m_rot.y = D3DX_PI * 1.0f;

			if (m_bZoom)
			{// �Y�[�����
				m_fLookHeight -= (m_posR.y - pos.y) * CAMERA_HEIGHT_SPEED;		// �����̒���

				if (m_fLength <= CAMERA_ZOOM_LENGTH)
				{// �J�����̋������k�߂�
					m_fLength += CAMERA_ZOOM;
				}

				// �ړI�̒����_
				m_posRDest.x = pos.x + sinf(rot.y);
				m_posRDest.y = m_fLookHeight;

				// �ړI�̎��_
				m_posVDest.x = pos.x + sinf(m_rot.y) * m_fLength;
				m_posVDest.y = m_fLookHeight;

				// �����_
				m_posR.x += (m_posRDest.x - m_posR.x) * 1.0f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 1.0f;

				// ���_
				m_posV.x += (m_posVDest.x - m_posV.x) * 1.0f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 1.0f;
				m_posV.z = m_fLength;
			}
			else
			{// �Y�[���łȂ�
				m_fLookHeight = pos.y;

				if (m_fLength >= CAMERA_LENGTH)
				{// �J�����̋����𗣂�
					m_fLength -= CAMERA_ZOOM;
				}

				// �ړI�̒����_
				m_posRDest.x = pos.x + sinf(rot.y);
				m_posRDest.y = m_fLookHeight;

				// �ړI�̎��_
				m_posVDest.x = pos.x + sinf(m_rot.y) * m_fLength;
				m_posVDest.y = m_fLookHeight;

				// �����_
				m_posR.x += (m_posRDest.x - m_posR.x) * 1.0f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 1.0f;

				// ���_
				m_posV.x += (m_posVDest.x - m_posV.x) * 1.0f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 1.0f;
				m_posV.z = m_fLength;
			}
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetRepeat(DIK_UP))
	{
		m_fLength += 5.0f;
	}
	else if (pKeyboard->GetRepeat(DIK_DOWN))
	{
		m_fLength -= 5.0f;
	}
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCamera::Draw(void)
{
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
