//=============================================================================
//
// 影処理 [shadow.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_SHADOW	"data/TEXTURE/shadow001.jpg"	// 読み込むテクスチャファイル名
#define SHADOW_WIDTH	(10.0f)		// x軸の大きさ
#define SHADOW_DEPTH	(10.0f)		// z軸の大きさ
#define MAX_SHADOW		(900)		// 影の最大数

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CShadow::m_pTexture[NUM_SHADOW_TEXTURE] = {};

//==================================
// 生成処理
//==================================
CShadow *CShadow::Create(D3DXVECTOR3 pos)
{
	CShadow *pShadow;

	pShadow = new CShadow;	// メモリを確保

	if (pShadow != NULL)
	{// メモリ確保成功
		pShadow->Init(pos);
		pShadow->BindTexture(m_pTexture[0]);
	}

	return pShadow;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CShadow::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SHADOW, &m_pTexture[0]);

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CShadow::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < NUM_SHADOW_TEXTURE; nCntTexture++)
	{
		if (NULL != m_pTexture[nCntTexture])
		{// テクスチャの破棄
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CShadow::CShadow(int nPriority) : CScene3D(nPriority)
{
}

//=========================================
// デストラクタ
//=========================================
CShadow::~CShadow()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos)
{
	CScene3D::Init();		// 初期化処理
	CScene3D::SetPos(D3DXVECTOR3(pos.x, 0.001f, pos.z));		// 位置の設定
	CScene3D::SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));	// 位置の設定
	CScene3D::SetLength(D3DXVECTOR3(30.0f, 0.0f, 30.0f));
	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
	CScene::SetObjType(CScene::OBJTYPE_SHADOW);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CShadow::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CShadow::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CScene3D::Draw();		// 描画

	// Zバッファを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 破棄
//=============================================================================
void CShadow::Delelte(void)
{
	Uninit();
}