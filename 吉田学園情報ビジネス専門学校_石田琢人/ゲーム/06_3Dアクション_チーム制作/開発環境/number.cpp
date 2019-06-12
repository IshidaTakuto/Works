//*****************************************************************************
//
//     数字ポリゴンの処理[number.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "number.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define NUMBER_TEXTURE_NAME "data/TEXTURE/COMMON/number.png"   // テクスチャのファイル名(数字)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;    // テクスチャへのポインタ

//=============================================================================
//    コンストラクタ
//=============================================================================
CNumber::CNumber(int nPriority, OBJTYPE objType) : CScene2DSquare(nPriority, objType)
{
	// 各種値のクリア
	m_nNumber = 0;    // 表示する数字
}

//=============================================================================
//    デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber, int nPriority)
{
	CNumber *pNumber = NULL;   // 数字ポリゴンクラス型のポインタ
	if (pNumber == NULL)
	{// メモリが確保できる状態にある
		pNumber = new CNumber(nPriority); // 数字ポリゴンクラスのインスタンス生成
		if (pNumber != NULL)
		{// メモリが確保できた
		    // 初期化処理
			if (FAILED(pNumber->Init(pos, col, fWidth, fHeight, nNumber)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// メモリが確保できなかった
			return NULL;
		}
	}
	else
	{// メモリが確保できる状態にない
		return NULL;
	}

	return pNumber;
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CNumber::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, NUMBER_TEXTURE_NAME, &m_pTexture);
		}
	}

	if (m_pTexture == NULL)
	{// テクスチャが読み込めなかった
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    テクスチャの解放処理
//=============================================================================
void CNumber::UnLoad(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber)
{
	// 各種値の設定
	m_nNumber = nNumber;  // 表示する数字

	// 共通の初期化処理
	if (FAILED(CScene2DSquare::Init(pos, col, fWidth, fHeight)))
	{
		return E_FAIL;
	}

	// テクスチャの設定
	BindTexture(m_pTexture);

	SetNumber(nNumber);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CNumber::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}

//=============================================================================
//    数字の設定処理
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	// 数字の設定
	m_nNumber = nNumber;

	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 頂点情報を設定
			VERTEX_2D *pVtx;
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
			// 頂点バッファをロックし,頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 1.0f);

			// 頂点バッファをアンロックする
			pVtxBuff->Unlock();
		}
	}
}