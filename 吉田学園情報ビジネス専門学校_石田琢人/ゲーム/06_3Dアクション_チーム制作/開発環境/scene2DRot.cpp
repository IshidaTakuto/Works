//*****************************************************************************
//
//     シーン(2D)回転の処理[scene2DRot.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene2DRot.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//   コンストラクタ
//=============================================================================
CScene2DRot::CScene2DRot(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// 各種値のクリア
	m_fRotation = 0.0f;  // 向き
	m_fScale = 0.0f;     // 現在の大きさ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScene2DRot::~CScene2DRot()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CScene2DRot *CScene2DRot::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fRotation, int nPriority)
{
	CScene2DRot *pScene2DRot = NULL;      // シーン2D回転クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();  // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pScene2DRot == NULL)
		{// メモリが空になっている
			pScene2DRot = new CScene2DRot(nPriority);
			if (pScene2DRot != NULL)
			{// インスタンスを生成できた
				if (FAILED(pScene2DRot->Init(pos, col, fScale, fRotation)))
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

	return pScene2DRot;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CScene2DRot::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fRotation)
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
			SetPos(pos);              // 座標
			SetCol(col);              // 色
			m_fRotation = fRotation;  // 回転量
			m_fScale = fScale;        // 現在の大きさ

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
void CScene2DRot::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CScene2DRot::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CScene2DRot::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}

//=============================================================================
//    ポリゴンの頂点バッファ生成処理
//=============================================================================
void CScene2DRot::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
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
	pVtx[0].pos = D3DXVECTOR3(GetPos().x + sinf(-D3DX_PI * 0.75f + m_fRotation) * m_fScale, GetPos().y + cosf(-D3DX_PI * 0.75f + m_fRotation) * m_fScale, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetPos().x + sinf(D3DX_PI * 0.75f + m_fRotation) * m_fScale, GetPos().y + cosf(D3DX_PI * 0.75f + m_fRotation) * m_fScale, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(GetPos().x + sinf(-D3DX_PI * 0.25f + m_fRotation) * m_fScale, GetPos().y + cosf(-D3DX_PI * 0.25f + m_fRotation) * m_fScale, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetPos().x + sinf(D3DX_PI * 0.25f + m_fRotation) * m_fScale, GetPos().y + cosf(D3DX_PI * 0.25f + m_fRotation) * m_fScale, 0.0f);

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
void CScene2DRot::SetVtxBuffPos(D3DXVECTOR3 pos)
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
		pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.75f + m_fRotation) * m_fScale, pos.y + cosf(-D3DX_PI * 0.75f + m_fRotation) * m_fScale, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.75f + m_fRotation) * m_fScale, pos.y + cosf(D3DX_PI * 0.75f + m_fRotation) * m_fScale, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.25f + m_fRotation) * m_fScale, pos.y + cosf(-D3DX_PI * 0.25f + m_fRotation) * m_fScale, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.25f + m_fRotation) * m_fScale, pos.y + cosf(D3DX_PI * 0.25f + m_fRotation) * m_fScale, 0.0f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの頂点カラー処理
//=============================================================================
void CScene2DRot::SetVtxBuffCol(D3DXCOLOR col)
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

		// 頂点座標
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの向き設定処理
//=============================================================================
void CScene2DRot::SetRotation(const float fRotation)
{
	m_fRotation = fRotation;
}

//=============================================================================
//    ポリゴンの現在の大きさ設定処理
//=============================================================================
void CScene2DRot::SetScale(const float fScale)
{
	m_fScale = fScale;
}

//=============================================================================
//    ポリゴンの向き取得処理
//=============================================================================
float CScene2DRot::GetRotation(void)
{
	return m_fRotation;
}

//=============================================================================
//    ポリゴンの現在の大きさ取得処理
//=============================================================================
float CScene2DRot::GetScale(void)
{
	return m_fScale;
}
