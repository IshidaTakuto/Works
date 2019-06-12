//=============================================================================
//
// �J�������� [camera.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE_ANGLE	(0.01f)
#define MOVE_CAMERA (1.0f)
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
	g_Camera.posV = D3DXVECTOR3(0.0f, 180.0f, -50.0f);
	g_Camera.posR = D3DXVECTOR3(0.0f, 150.0f, 0.0f);
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.fLength = -300.0f;
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

	//if (pPlayer->state == PLAYERSTATE_NORMAL)
	//{
	//	nCntAngle++;

	//	if (nCntAngle > MOVE_START)
	//	{
	//		g_Camera.rotDest.y = D3DX_PI + pPlayer->rot.y - g_Camera.rot.y;
	//		if (g_Camera.rotDest.y > D3DX_PI)
	//		{
	//			g_Camera.rotDest.y -= D3DX_PI * 2;
	//		}
	//		else if (g_Camera.rotDest.y < -D3DX_PI)
	//		{
	//			g_Camera.rotDest.y += D3DX_PI * 2;
	//		}

	//		g_Camera.rot.y += g_Camera.rotDest.y * MOVE_ANGLE;
	//	}
	//}
	//else
	//{
	//	nCntAngle = 0;
	//}

		// �v���C���[�̌��ɉ��
		//g_Camera.rotDest.y = D3DX_PI + pPlayer->rot.y - g_Camera.rot.y;
		//if (g_Camera.rotDest.y > D3DX_PI)
		//{
		//	g_Camera.rotDest.y -= D3DX_PI * 2;
		//}
		//else if (g_Camera.rotDest.y < -D3DX_PI)
		//{
		//	g_Camera.rotDest.y += D3DX_PI * 2;
		//}

	g_Camera.rot.y += g_Camera.rotDest.y * MOVE_ANGLE;

	g_Camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * 1.0f;
	g_Camera.posRDest.y = pPlayer->pos.y;
	g_Camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * 1.0f;

	g_Camera.posVDest.x = pPlayer->pos.x + sinf(g_Camera.rot.y) * (g_Camera.fLength);
	g_Camera.posVDest.y = pPlayer->pos.y + 45.0f;
	g_Camera.posVDest.z = pPlayer->pos.z + cosf(g_Camera.rot.y) * (g_Camera.fLength);

	g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * 0.1f;
	g_Camera.posR.y += (g_Camera.posRDest.y - g_Camera.posR.y) * 0.1f;
	g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * 0.1f;

	g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * 0.15f;
	g_Camera.posV.y += (g_Camera.posVDest.y - g_Camera.posV.y) * 0.15f;
	g_Camera.posV.z += (g_Camera.posVDest.z - g_Camera.posV.z) * 0.15f;

	if (GetKeyboardPress(DIK_NUMPAD8) == true)
	{
		g_Camera.fLength += 5.0f;
	}
	else if (GetKeyboardPress(DIK_NUMPAD2) == true)
	{
		g_Camera.fLength -= 5.0f;
	}

	//// �J�����̈ړ�
	//if (GetKeyboardPress(DIK_NUMPAD8) == true)
	//{// �O
	//	if (GetKeyboardPress(DIK_NUMPAD6) == true)
	//	{// �E�O
	//		g_Camera.posV.x -= sinf((-D3DX_PI * 0.75f) + g_Camera.rot.y) * 1.5f;
	//		g_Camera.posV.z -= cosf((-D3DX_PI * 0.75f) + g_Camera.rot.y) * 1.5f;

	//		g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
	//		g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
	//	}
	//	else if (GetKeyboardPress(DIK_NUMPAD4) == true)
	//	{// ���O
	//		g_Camera.posV.x -= sinf((D3DX_PI * 0.75f) + g_Camera.rot.y) * 1.5f;
	//		g_Camera.posV.z -= cosf((D3DX_PI * 0.75f) + g_Camera.rot.y) * 1.5f;

	//		g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
	//		g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
	//	}
	//	else
	//	{// �O
	//		g_Camera.posV.x += sinf((D3DX_PI * 0.0f) + g_Camera.rot.y) * 1.5f;
	//		g_Camera.posV.z += cosf((D3DX_PI * 0.0f) + g_Camera.rot.y) * 1.5f;

	//		g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
	//		g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_NUMPAD2) == true)
	//{// ���
	//	if (GetKeyboardPress(DIK_NUMPAD6) == true)
	//	{// �E���
	//		g_Camera.posV.x -= sinf((-D3DX_PI) * 0.25f + g_Camera.rot.y) * 1.5f;
	//		g_Camera.posV.z -= cosf((-D3DX_PI) * 0.25f + g_Camera.rot.y) * 1.5f;

	//		g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
	//		g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
	//	}
	//	else if (GetKeyboardPress(DIK_NUMPAD4) == true)
	//	{// �����
	//		g_Camera.posV.x -= sinf((D3DX_PI) * 0.25f + g_Camera.rot.y) * 1.5f;
	//		g_Camera.posV.z -= cosf((D3DX_PI) * 0.25f + g_Camera.rot.y) * 1.5f;

	//		g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
	//		g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
	//	}
	//	else
	//	{// ���
	//		g_Camera.posV.x -= sinf((D3DX_PI * 0.0f) + g_Camera.rot.y) * 1.5f;
	//		g_Camera.posV.z -= cosf((D3DX_PI * 0.0f) + g_Camera.rot.y) * 1.5f;

	//		g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
	//		g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_NUMPAD6) == true)
	//{// �E
	//	g_Camera.posV.x -= sinf((D3DX_PI * -0.5f) + g_Camera.rot.y) * 1.5f;
	//	g_Camera.posV.z -= cosf((D3DX_PI * -0.5f) + g_Camera.rot.y) * 1.5f;

	//	g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
	//	g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
	//}
	//else if (GetKeyboardPress(DIK_NUMPAD4) == true)
	//{// ��
	//	g_Camera.posV.x -= sinf((D3DX_PI * 0.5f) + g_Camera.rot.y) * 1.5f;
	//	g_Camera.posV.z -= cosf((D3DX_PI * 0.5f) + g_Camera.rot.y) * 1.5f;

	//	g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
	//	g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
	//}

	////// �����_�̈ړ�
	////if (GetKeyboardPress(DIK_U) == true)
	////{// ������
	////	g_Camera.rot.y -= MOVE_ANGLE;
	////	if (g_Camera.rot.y < -D3DX_PI)
	////	{
	////		g_Camera.rot.y = D3DX_PI;
	////	}
	////	g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
	////	g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
	////}
	////else if (GetKeyboardPress(DIK_J) == true)
	////{// �E����
	////	g_Camera.rot.y += MOVE_ANGLE;
	////	if (g_Camera.rot.y > D3DX_PI)
	////	{
	////		g_Camera.rot.y = -D3DX_PI;
	////	}
	////	g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
	////	g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
	////}

	// // ���_�̈ړ�
	//if (GetKeyboardPress(DIK_NUMPAD1) == true)
	//{// ������
	//	g_Camera.rot.y += MOVE_ANGLE;
	//	if (g_Camera.rot.y > D3DX_PI)
	//	{
	//		g_Camera.rot.y = -D3DX_PI;
	//	}
	//	g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	//	g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
	//}
	//else if (GetKeyboardPress(DIK_NUMPAD3) == true)
	//{// �E����
	//	g_Camera.rot.y -= MOVE_ANGLE;
	//	if (g_Camera.rot.y < -D3DX_PI)
	//	{
	//		g_Camera.rot.y = D3DX_PI;
	//	}
	//	g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	//	g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
	//}

	//// �����̍X�V
	//if (GetKeyboardPress(DIK_NUMPAD9) == true)
	//{// �グ��
	//	g_Camera.posV.y += 1.0f;
	//}
	//if (GetKeyboardPress(DIK_NUMPAD7) == true)
	//{// ������
	//	g_Camera.posV.y -= 1.0f;
	//}

	//if (GetKeyboardPress(DIK_ADD) == true)
	//{
	//	g_Camera.posR.y += 1.0f;
	//}
	//else if (GetKeyboardPress(DIK_SUBTRACT) == true)
	//{
	//	g_Camera.posR.y -= 1.0f;
	//}

	//// �����l�ɖ߂�
	//if (GetKeyboardPress(DIK_NUMPAD0) == true)
	//{
	//	InitCamera();
	//}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
								D3DXToRadian(45.0f),							// ��p
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// �A�X�y�N�g��
								10.0f,											// �J�����͈̔́i��O�j
								100000.0f);										// �J�����͈̔́i���s�j

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

//=============================================================================
// �J�����̎擾
//=============================================================================
Camera *GetCamera(void)
{
	return &g_Camera;
}