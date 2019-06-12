//*****************************************************************************
//
//     シーン(2D)スクエアの処理[scene2DSquare.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene2DSquare.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//    コンストラクタ
//=============================================================================
CScene2DSquare::CScene2DSquare(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// 各種値のクリア
	m_fWidth = 0.0f;   // 幅
	m_fHeight = 0.0f;  // 高さ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScene2DSquare::~CScene2DSquare()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CScene2DSquare *CScene2DSquare::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority)
{
	CScene2DSquare *pScene2DSquare = NULL;   // シーン2Dスクエアクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();     // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pScene2DSquare == NULL)
		{// メモリが空になっている
			pScene2DSquare = new CScene2DSquare(nPriority);
			if (pScene2DSquare != NULL)
			{// インスタンスを生成できた
				if (FAILED(pScene2DSquare->Init(pos, col, fWidth, fHeight)))
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

	return pScene2DSquare;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CScene2DSquare::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 各種値の設定
			SetPos(pos);          // 座標
			SetCol(col);          // 色
			m_fWidth = fWidth;    // 幅
			m_fHeight = fHeight;  // 高さ

			// 頂点バッファの生成
			CScene2D::Init();
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
//    終了処理
//=============================================================================
void CScene2DSquare::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CScene2DSquare::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CScene2DSquare::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}

//=============================================================================
//    ポリゴンの頂点バッファ生成処理
//=============================================================================
void CScene2DSquare::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(GetPos().x - m_fWidth, GetPos().y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetPos().x + m_fWidth, GetPos().y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(GetPos().x - m_fWidth, GetPos().y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetPos().x + m_fWidth, GetPos().y + m_fHeight, 0.0f);

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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
//    ポリゴンの頂点設定処理
//=============================================================================
void CScene2DSquare::SetVtxBuffPos(const D3DXVECTOR3 pos)
{
	// 座標を渡す
	SetPos(pos);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene2D::GetVtxBuff(); // 頂点情報取得
	if (pVtxBuff != NULL)
	{// 頂点情報を取得できた
	    // 頂点情報設定
		VERTEX_2D *pVtx;
		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(pos.x - m_fWidth, pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + m_fWidth, pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - m_fWidth, pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + m_fWidth, pos.y + m_fHeight, 0.0f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの頂点カラー設定処理
//=============================================================================
void CScene2DSquare::SetVtxBuffCol(const D3DXCOLOR col)
{
	// 色を渡す
	SetCol(col);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene2D::GetVtxBuff(); // 頂点情報取得
	if (pVtxBuff != NULL)
	{// 頂点情報を取得できた
	    // 頂点情報設定
		VERTEX_2D *pVtx;
		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラー
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの幅設定処理
//=============================================================================
void CScene2DSquare::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    ポリゴンの高さ設定処理
//=============================================================================
void CScene2DSquare::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    ポリゴンの幅取得処理
//=============================================================================
float CScene2DSquare::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ポリゴンの高さ取得処理
//=============================================================================
float CScene2DSquare::GetHeight(void)
{
	return m_fHeight;
}
