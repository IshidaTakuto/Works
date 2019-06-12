//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "light.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LIGHT	(3)		// ���C�g�̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DLIGHT9 g_alight[MAX_LIGHT];	// ���C�g�̏��
int g_nCntLight;
bool g_bLight;
LIGHTTYPE g_LightType;

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXVECTOR3 vecDir;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g���N���A����
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ���C�g�̊g�U����ݒ�
	g_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_alight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
	g_alight[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// ���K������
	g_alight[0].Direction = vecDir;

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// ���K������
	g_alight[1].Direction = vecDir;

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// ���K������
	g_alight[2].Direction = vecDir;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	g_nCntLight = 0;
	g_bLight = false;
	g_LightType = LIGHTTYPE_NORMAL;
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void UpdateLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXVECTOR3 vecDir;

	switch (g_LightType)
	{
	case LIGHTTYPE_NORMAL:
		g_alight[0].Diffuse = D3DXCOLOR(2.0f, 1.8f, 1.5f, 1.0f);
		g_alight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

		// ���C�g�̕����̐ݒ�
		vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
		D3DXVec3Normalize(&vecDir, &vecDir);	// ���K������
		g_alight[0].Direction = vecDir;

		// ���C�g��ݒ肷��
		pDevice->SetLight(0, &g_alight[0]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(0, TRUE);

		// ���C�g��ݒ肷��
		pDevice->SetLight(1, &g_alight[1]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(1, TRUE);

		break;

	case LIGHTTYPE_TITLE:
		g_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_alight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

		// ���C�g�̕����̐ݒ�
		vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
		D3DXVec3Normalize(&vecDir, &vecDir);	// ���K������
		g_alight[0].Direction = vecDir;

		// ���C�g��ݒ肷��
		pDevice->SetLight(0, &g_alight[0]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(0, TRUE);

		// ���C�g��ݒ肷��
		pDevice->SetLight(1, &g_alight[1]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(1, TRUE);

		break;

	case LIGHTTYPE_RESULT:
		g_alight[0].Diffuse = D3DXCOLOR(2.0f, 2.0f, 2.0f, 1.0f);
		g_alight[1].Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// ���C�g�̕����̐ݒ�
		vecDir = D3DXVECTOR3(0.67f, -0.43f, 0.6f);
		D3DXVec3Normalize(&vecDir, &vecDir);	// ���K������
		g_alight[0].Direction = vecDir;

		// ���C�g��ݒ肷��
		pDevice->SetLight(0, &g_alight[0]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(0, TRUE);

		// ���C�g��ݒ肷��
		pDevice->SetLight(1, &g_alight[1]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(1, TRUE);

		break;
	}
}

//=============================================================================
// ����Ԃ̐��ۂ̎擾
//=============================================================================
bool GetLightRain(void)
{
	return g_bLight;
}

//=============================================================================
// ���C�g�̎�ނ̎擾
//=============================================================================
LIGHTTYPE *GetLightType(void)
{
	return &g_LightType;
}