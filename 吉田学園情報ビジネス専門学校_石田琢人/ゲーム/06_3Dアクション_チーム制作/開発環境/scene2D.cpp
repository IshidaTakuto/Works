//*****************************************************************************
//
//     シーン(2D)の処理[scene2D.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define POLYGON2D_TEXTURE_NAME_0  "data/TEXTURE/cutin002.png"    // テクスチャのファイル名(戦車)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CScene2D::m_apTexture[TEXTURE_TYPE_MAX] = {};  // 2Dポリゴンのテクスチャ画像

//=============================================================================
//    コンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTexture = NULL;                         // テクスチャへのポインタ
	m_pVtxBuff = NULL;                         // 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // 色
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
//    ポリゴンの生成処理(テクスチャの種類あり)
//=============================================================================
CScene2D *CScene2D::Create(TEXTURE_TYPE type, int nPriority)
{
	CScene2D *pScene2D = NULL;             // シーン2Dクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pScene2D == NULL)
		{// メモリが空になっている
			pScene2D = new CScene2D(nPriority);
			if (pScene2D != NULL)
			{// インスタンスを生成できた
				pScene2D->BindTexture(m_apTexture[type]);
				pScene2D->SetPos(D3DXVECTOR3(200.0f + (200.0f * type), SCREEN_HEIGHT / 3, 0.0f));
				pScene2D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				if (FAILED(pScene2D->Init()))
				{// 初期化に失敗した
					return NULL;
				}
				else
				{// 初期化に成功した
				 // インスタンスのアドレスを返す
					return pScene2D;
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
}

//=============================================================================
//    ポリゴンのテクスチャ読み込み処理
//=============================================================================
HRESULT CScene2D::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			char aTextureName[TEXTURE_TYPE_MAX][256] =
			{// テクスチャのファイルパス名
				POLYGON2D_TEXTURE_NAME_0,
			};
			for (int nCntTex = 0; nCntTex < TEXTURE_TYPE_MAX; nCntTex++)
			{// 読み込むテクスチャの数だけ繰り返し
				D3DXCreateTextureFromFile(pDevice, aTextureName[nCntTex], &m_apTexture[nCntTex]);
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    ポリゴンのテクスチャ解放処理
//=============================================================================
void CScene2D::UnLoad(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TEXTURE_TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    ポリゴンの初期化処理(アニメーションなし)
//=============================================================================
HRESULT CScene2D::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// 頂点バッファの生成
			MakeVertex(pDevice);
		}
		else
		{// デバイスを取得できなかった
			return E_FAIL;
		}
	}
	else
	{// レンダリングクラスが生成されていない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    ポリゴンの終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    ポリゴンの更新処理
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
//    ポリゴンの描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
//    ポリゴンの頂点バッファ生成処理
//=============================================================================
void CScene2D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 150.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, 100.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 150.0f, 100.0f, 0.0f);

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ポリゴンのテクスチャ設定処理
//=============================================================================
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ポリゴンの位置設定処理
//=============================================================================
void CScene2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
//    ポリゴンの色設定処理
//=============================================================================
void CScene2D::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    ポリゴンの頂点情報設定処理
//=============================================================================
void CScene2D::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    ポリゴンの頂点情報取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    ポリゴンの位置取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}

//=============================================================================
//    ポリゴンの色取得処理
//=============================================================================
D3DXCOLOR CScene2D::GetCol(void)
{
	return m_Col;
}