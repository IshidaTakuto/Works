//=============================================================================
//
// �J�������� [camera.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE_ANGLE	(0.01f)
#define TITLE_MOVE	(0.001f)
#define MOVE_CAMERA (160.0f)
#define MOVE_START	(90)	// �����܂ł̎���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Camera g_Camera;		// �J�����̏��
int nCntAngle;			// �J�����������܂ł̃J�E���^

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	g_Camera.posV = D3DXVECTOR3(0.0f, 150.0f, -200.0f);
	g_Camera.posR = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.fLength = -300.0f;
	g_Camera.type = CAMERATYPE_NONE;
	g_Camera.fAngle = 0.0f;
	nCntAngle = 0;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	Player *pPlayer;
	pPlayer = GetPlayer();

	bool bOn = false;

#if 1
	switch (g_Camera.type)
	{
	case CAMERATYPE_TITLE:

		pPlayer->pos = D3DXVECTOR3(60.0f, 0.0f, 0.0f);
		pPlayer->rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.8f, 0.0f);

		g_Camera.posV = D3DXVECTOR3(120.0f, 50.0f, -50.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 70.0f, 0.0f);
		g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		break;

	case CAMERATYPE_GAME:

		//if (bOn == false)
	{
		g_Camera.posV = D3DXVECTOR3(0.0f, 150.0f, -200.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
		g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//bOn = true;
	}

	if (GetJoyPadPress(DIJS_BUTTON_17, 0) == true)
	{// �J�����̈ړ�
		g_Camera.fAngle -= 0.02f;
	}
	else if (GetJoyPadPress(DIJS_BUTTON_16, 0) == true)
	{
		g_Camera.fAngle += 0.02f;
	}

	//===================================
	//�����_���v���C���[�ɒǏ]�悤�ɂ���
	//===================================
	g_Camera.posR.x = pPlayer->pos.x;
	g_Camera.posR.z = pPlayer->pos.z;
	//���_���v���C���[�ɒǏ]�悤�ɂ���
	g_Camera.posV.x = pPlayer->pos.x - sinf(g_Camera.fAngle)* MOVE_CAMERA;
	g_Camera.posV.z = pPlayer->pos.z - cosf(g_Camera.fAngle)* MOVE_CAMERA;
	break;

	case CAMERATYPE_RANKING:

		break;
	}
#endif
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(CAMERATYPE cameratype)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	g_Camera.type = cameratype;

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
								D3DXToRadian(45.0f),							// ��p
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// �A�X�y�N�g��
								10.0f,											// �J�����͈̔́i��O�j
								3000.0f);										// �J�����͈̔́i���s�j

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&g_Camera.mtxView,
		&g_Camera.posV,
		&g_Camera.posR,
		&g_Camera.vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}
void DeleteCamera(void)
{
	g_Camera.posV = D3DXVECTOR3(0.0f, 150.0f, -200.0f);
	g_Camera.posR = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.fLength = -300.0f;
	g_Camera.fAngle = 0.0f;
	g_Camera.type = CAMERATYPE_NONE;
	nCntAngle = 0;
}

Camera *GetCamera(void)
{
	return &g_Camera;
}