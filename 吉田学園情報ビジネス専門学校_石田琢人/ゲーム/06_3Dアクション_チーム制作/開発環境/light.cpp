//*****************************************************************************
//
//     ライトの処理[light.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "light.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//    コンストラクタ
//=============================================================================
CLight::CLight()
{
	// ライトをクリアする
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの数だけ繰り返し
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CLight::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXVECTOR3 vecDir;  // カメラの法線設定用

		    // ライトの種類を設定
			m_Light[0].Type = D3DLIGHT_DIRECTIONAL;
			m_Light[1].Type = D3DLIGHT_DIRECTIONAL;
			m_Light[2].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			m_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			m_Light[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			m_Light[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			// ライトの方向の設定
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[0].Direction = vecDir;
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[1].Direction = vecDir;
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[2].Direction = vecDir;

			// ライトを設定する
			for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
			{// ライトの数だけ繰り返し
				pDevice->SetLight(nCntLight, &m_Light[nCntLight]);
			}

			// 設定したライトを有効にする
			for (int nCntLight = 0; nCntLight < 1; nCntLight++)
			{// ライトの数だけ繰り返し
				pDevice->LightEnable(nCntLight, TRUE);
			}
		}
	}
	else
	{// レンダリングクラスが取得できなかった
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CLight::Uninit(void)
{
	// ライトをクリアする
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの数だけ繰り返し
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CLight::Update(void)
{

}