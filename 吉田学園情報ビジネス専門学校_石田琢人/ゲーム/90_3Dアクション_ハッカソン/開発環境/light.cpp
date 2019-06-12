//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// ライト処理 [light.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIGHT		(3)		// ライトの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DLIGHT9 g_alight[MAX_LIGHT];		// ライトの情報

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ライトの初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// ライトの方向ベクトルを設定
	D3DXVECTOR3 vecDir[MAX_LIGHT] =
	{
		{D3DXVECTOR3(0.22f, -0.87f, 0.44f)},
		{D3DXVECTOR3(-0.18f, 0.88f, -0.44f)},
		{D3DXVECTOR3(0.9f, -0.1f, 0.44f)}
	};// 設定用方向ベクトル

	// ライトの拡散光を設定
	D3DXCOLOR Diffuse[MAX_LIGHT] =
	{
		//{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)},
		//{D3DXCOLOR(0.65f, 0.65f, 0.65f, 0.65f)},
		//{D3DXCOLOR(0.15f, 0.15f, 0.15f, 0.15f)}
		{ D3DXCOLOR(1.5f, 1.5f, 1.5f, 1.0f) },
		{ D3DXCOLOR(1.65f, 1.65f, 1.65f, 1.0f) },
		{ D3DXCOLOR(1.15f, 1.15f, 1.15f, 1.0f) }
	};// 設定用拡散光

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		g_alight[nCntLight].Diffuse = Diffuse[nCntLight];

		// ライトの方向の設定
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化
		g_alight[nCntLight].Direction = vecDir[nCntLight];

		// ライトを設定する
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ライトの終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitLight(void)
{
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ライトの更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdateLight(void)
{
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ライトの取得
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
D3DLIGHT9 GetLight(void)
{
	return g_alight[0];
}

