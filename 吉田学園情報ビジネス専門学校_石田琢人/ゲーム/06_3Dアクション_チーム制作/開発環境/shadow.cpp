//*****************************************************************************
//
//     影の処理[shadow.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"

#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define SHADOW_TEXTURE_NAME  "data/TEXTURE/COMMON/shadow000.jpg"     // テクスチャのファイル名(影)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CShadow::m_apTexture = NULL;  // 影のテクスチャ画像

//=============================================================================
//    コンストラクタ
//=============================================================================
CShadow::CShadow(int nPriority, OBJTYPE objType) : CScene3D(nPriority, objType)
{
	// 各種値のクリア
	m_fRadius = 0.0f;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
//    影の生成処理
//=============================================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nPriority)
{
	CShadow *pShadow = NULL;               // 影クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pShadow == NULL)
		{// メモリが空になっている
			pShadow = new CShadow(nPriority);
			if (pShadow != NULL)
			{// インスタンスを生成できた
				if (FAILED(pShadow->Init(pos,rot,col,fRadius)))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// インスタンスを生成できなかった
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pShadow;
}

//=============================================================================
//    影のテクスチャ読み込み処理
//=============================================================================
HRESULT CShadow::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTURE_NAME, &m_apTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    影のテクスチャ解放処理
//=============================================================================
void CShadow::UnLoad(void)
{
	// テクスチャの破棄
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius)
{
	// 各種値の代入
	BindTexture(m_apTexture);   // テクスチャへのポインタ
	SetPos(pos);                // 代入
	SetRot(rot);                // 向き
	SetCol(col);                // 色
	m_fRadius = fRadius;        // 半径

	// 共通の初期化処理
	CScene3D::Init();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	// 共通の終了処理
	CScene3D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CShadow::Update(void)
{
	// 前回の座標更新
	m_PosOld = GetPos();
}

//=============================================================================
//    描画処理
//=============================================================================
void CShadow::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 減算合成の設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// 共通の描画処理
			CScene3D::Draw();

			// 合成の設定を戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    影の頂点バッファ生成処理
//=============================================================================
void CShadow::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * m_fRadius, 0.0f, cosf(-D3DX_PI * 0.75f) * -m_fRadius);
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * m_fRadius, 0.0f, cosf(D3DX_PI * 0.75f) * -m_fRadius);
	pVtx[2].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * m_fRadius, 0.0f, cosf(-D3DX_PI * 0.25f) * -m_fRadius);
	pVtx[3].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * m_fRadius, 0.0f, cosf(D3DX_PI * 0.25f) * -m_fRadius);

	// 法線
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファの設定
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    影の半径設定処理
//=============================================================================
void CShadow::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    影の半径取得処理
//=============================================================================
float CShadow::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    影の前回の座標取得処理
//=============================================================================
D3DXVECTOR3 CShadow::GetPosOld(void)
{
	return m_PosOld;
}