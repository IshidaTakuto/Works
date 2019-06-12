//*****************************************************************************
//
//     ビルボードの処理[sceneBillboard.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sceneBillboard.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CSceneBillboard::CSceneBillboard(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTexture = NULL;                         // テクスチャへのポインタ
	m_pVtxBuff = NULL;                         // 頂点バッファへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ポリゴンの座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // ポリゴンの色
	m_fWidth = 0.0f;                           // ポリゴンの幅
	m_fHeight = 0.0f;                          // ポリゴンの高さ
	m_fAngle = 0.0f;                           // ポリゴンの角度
	m_fLength = 0.0f;                          // ポリゴンの長さ
	D3DXMatrixIdentity(&m_MtxWorld);           // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSceneBillboard::~CSceneBillboard()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSceneBillboard *CSceneBillboard::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority)
{
	CSceneBillboard *pSceneBillboard = NULL;      // ビルボードクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pSceneBillboard == NULL)
		{// メモリが空になっている
			pSceneBillboard = new CSceneBillboard(nPriority);
			if (pSceneBillboard != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pSceneBillboard->SetPos(pos);         // ポリゴンの座標
				pSceneBillboard->SetCol(col);         // ポリゴンの色
				pSceneBillboard->SetWidth(fWidth);    // ポリゴンの幅
				pSceneBillboard->SetHeight(fHeight);  // ポリゴンの高さ

				if (FAILED(pSceneBillboard->Init()))
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
	{// これ以上シーンを作成できない
		return NULL;
	}

	return pSceneBillboard;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSceneBillboard::Init(void)
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
//    終了処理
//=============================================================================
void CSceneBillboard::Uninit(void)
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
//    更新処理
//=============================================================================
void CSceneBillboard::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CSceneBillboard::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CSceneBillboard::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxView, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// カメラの向きを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ワールドマトリックスに向きを設定(カメラの逆行列を入れることでカメラの正面に向きを補正)
	m_MtxWorld._11 = mtxView._11;
	m_MtxWorld._12 = mtxView._21;
	m_MtxWorld._13 = mtxView._31;
	m_MtxWorld._21 = mtxView._12;
	m_MtxWorld._22 = mtxView._22;
	m_MtxWorld._23 = mtxView._32;
	m_MtxWorld._31 = mtxView._13;
	m_MtxWorld._32 = mtxView._23;
	m_MtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CSceneBillboard::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// オフセットの長さを求める
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	// オフセットの角度を求める
	m_fAngle = atan2f(m_fWidth, m_fHeight);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

	// 法線を代入
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[1].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[2].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[3].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

	// 頂点カラー
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CSceneBillboard::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CSceneBillboard::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    ポリゴンの座標設定処理
//=============================================================================
void CSceneBillboard::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ポリゴンの色設定処理
//=============================================================================
void CSceneBillboard::SetCol(const D3DXCOLOR col)
{
	// 色を設定
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラー
		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの幅設定処理
//=============================================================================
void CSceneBillboard::SetWidth(const float fWidth)
{
	// 幅を設定
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// オフセットの長さを求める
		m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

		// オフセットの角度を求める
		m_fAngle = atan2f(m_fWidth, m_fHeight);

		// 頂点座標
		pVtx[0].pos.x = -sinf(m_fAngle) * m_fLength;
		pVtx[1].pos.x = sinf(m_fAngle) * m_fLength;
		pVtx[2].pos.x = -sinf(m_fAngle) * m_fLength;
		pVtx[3].pos.x = sinf(m_fAngle) * m_fLength;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの高さ設定処理
//=============================================================================
void CSceneBillboard::SetHeight(const float fHeight)
{
	// 高さを設定
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// オフセットの長さを求める
		m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

		// オフセットの角度を求める
		m_fAngle = atan2f(m_fWidth, m_fHeight);

		// 頂点座標
		pVtx[0].pos.y = cosf(m_fAngle) * m_fLength;
		pVtx[1].pos.y = cosf(m_fAngle) * m_fLength;
		pVtx[2].pos.y = -cosf(m_fAngle) * m_fLength;
		pVtx[3].pos.y = -cosf(m_fAngle) * m_fLength;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの角度設定処理
//=============================================================================
void CSceneBillboard::SetAngle(const float fAngle)
{
	// 角度を設定
	m_fAngle = fAngle;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの長さ設定処理
//=============================================================================
void CSceneBillboard::SetLength(const float fLength)
{
	// 長さを設定
	m_fLength = fLength;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CSceneBillboard::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CSceneBillboard::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CSceneBillboard::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ポリゴンの座標取得処理
//=============================================================================
D3DXVECTOR3 CSceneBillboard::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ポリゴンの色取得処理
//=============================================================================
D3DXCOLOR CSceneBillboard::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    ポリゴンの幅取得処理
//=============================================================================
float CSceneBillboard::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ポリゴンの高さ取得処理
//=============================================================================
float CSceneBillboard::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ポリゴンの角度取得処理
//=============================================================================
float CSceneBillboard::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
//    ポリゴンの長さ取得処理
//=============================================================================
float CSceneBillboard::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CSceneBillboard::GetMtxWorld(void)
{
	return m_MtxWorld;
}