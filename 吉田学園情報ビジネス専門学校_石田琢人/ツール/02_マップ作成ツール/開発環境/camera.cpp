//=============================================================================
//
// �J�������� [camera.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "setPlayer.h"

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
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
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
	m_posV = D3DXVECTOR3(0.0f, 300.0f, -300.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_fLength = m_posV.z;
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
	CInputKeyboard *pKeyboard = CManager::GetInput();
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (CManager::GetSetPlayer() != NULL)
	{
		posPlayer = CManager::GetSetPlayer()->GetPos();
	}

	//===================================
	// �����_���v���C���[�ɒǏ]�悤�ɂ���
	//===================================
	m_posR.x = posPlayer.x;
	m_posR.y = posPlayer.y;
	m_posR.z = posPlayer.z;
	m_posV.x = posPlayer.x + sinf(m_fAngle) * m_fLength;
	m_posV.y = posPlayer.y + 300.0f;
	m_posV.z = posPlayer.z + cosf(m_fAngle) * m_fLength;

	if (pKeyboard->GetPress(DIK_NUMPAD8) == true)
	{// ����������������
		if (m_fLength < -20.0f)
		{
			m_fLength += 5.0f;
		}
	}
	else if (pKeyboard->GetPress(DIK_NUMPAD2) == true)
	{// ������傫������
		//if (m_fLength > -1000.0f)
		{
			m_fLength -= 5.0f;
		}
	}
	if (pKeyboard->GetPress(DIK_NUMPAD4) == true)
	{// ����������������
		m_fAngle += 0.02f;
	}
	else if (pKeyboard->GetPress(DIK_NUMPAD6) == true)
	{// ������傫������
		m_fAngle -= 0.02f;
	}

	if (pKeyboard->GetPress(DIK_NUMPAD1) == true)
	{// ����������������
		m_posV.y -= 5.0f;
	}
	else if (pKeyboard->GetPress(DIK_NUMPAD3) == true)
	{// ������傫������
		m_posV.y += 5.0f;
	}

	if (pKeyboard->GetPress(DIK_NUMPAD0) == true)
	{// �J�������Z�b�g
		Init();
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
