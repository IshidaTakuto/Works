//=============================================================================
//
// ライト処理 [light.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
D3DLIGHT9 CLight::m_alight[MAX_LIGHT] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CLight::~CLight()
{
}

//=============================================================================
// ライトの初期化処理
//=============================================================================
void CLight::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();	// デバイスの取得

	D3DXVECTOR3 vecDir;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&m_alight[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		m_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ライトの拡散光を設定
	m_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_alight[0].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_alight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
	m_alight[1].Ambient = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
	m_alight[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);
	m_alight[2].Ambient = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// 正規化する
	m_alight[0].Direction = vecDir;

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// 正規化する
	m_alight[1].Direction = vecDir;

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// 正規化する
	m_alight[2].Direction = vecDir;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_alight[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void CLight::Update(void)
{
}
