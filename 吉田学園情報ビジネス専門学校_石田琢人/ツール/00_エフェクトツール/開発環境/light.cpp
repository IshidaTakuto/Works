//=============================================================================
//
// ライト処理 [light.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIGHT	(3)		// ライトの数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DLIGHT9 g_alight[MAX_LIGHT];	// ライトの情報

//=============================================================================
// ライトの初期化処理
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ライトの拡散光を設定
	g_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_alight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
	g_alight[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// 正規化する
	g_alight[0].Direction = vecDir;

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// 正規化する
	g_alight[1].Direction = vecDir;

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// 正規化する
	g_alight[2].Direction = vecDir;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトを設定する
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void UpdateLight(void)
{
}

