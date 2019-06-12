//*****************************************************************************
//
//     シーン(3D)メッシュの処理[scene3DMesh.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene3DMesh.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MESH_TEXTURE_NAME  "data/TEXTURE/tank.png"     // テクスチャのファイル名(戦車)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CScene3DMesh::m_apTexture = NULL;  // メッシュのテクスチャ画像

//=============================================================================
//    コンストラクタ
//=============================================================================
CScene3DMesh::CScene3DMesh(int nPriority, OBJTYPE objType) : CScene3D(nPriority, objType)
{
	// 各種値のクリア
	m_pIdxBuff = NULL;     // インデックスバッファ
	m_nNumIndex = 0;       // インデックス数
	m_nNumVertex = 0;      // 頂点数
	m_nNumPolygon = 0;     // ポリゴン数
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScene3DMesh::~CScene3DMesh()
{

}

//=============================================================================
//    メッシュの生成処理
//=============================================================================
CScene3DMesh *CScene3DMesh::Create(int nPriority)
{
	CScene3DMesh *pScene3DMesh = NULL;     // シーン3Dメッシュクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pScene3DMesh == NULL)
		{// メモリが空になっている
			pScene3DMesh = new CScene3DMesh(nPriority);
			if (pScene3DMesh != NULL)
			{// インスタンスを生成できた
				if (FAILED(pScene3DMesh->Init()))
				{// 初期化に失敗した
					return NULL;
				}
				else
				{// 初期化に成功した
				    // インスタンスのアドレスを返す
					return pScene3DMesh;
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
//    メッシュのテクスチャ読み込み処理
//=============================================================================
HRESULT CScene3DMesh::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, MESH_TEXTURE_NAME, &m_apTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    メッシュのテクスチャ解放処理
//=============================================================================
void CScene3DMesh::UnLoad(void)
{
	// テクスチャの破棄
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    メッシュの初期化処理
//=============================================================================
HRESULT CScene3DMesh::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// 頂点バッファの作成
			MakeVertex(pDevice);

			// インデックスバッファの作成
			MakeIndex(pDevice);
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
//    メッシュの終了処理
//=============================================================================
void CScene3DMesh::Uninit(void)
{
	// インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 共通の終了処理
	CScene3D::Uninit();
}

//=============================================================================
//    メッシュの更新処理
//=============================================================================
void CScene3DMesh::Update(void)
{

}

//=============================================================================
//    メッシュの描画処理
//=============================================================================
void CScene3DMesh::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファの取得
			LPDIRECT3DTEXTURE9 pTexture = GetTexture();       // テクスチャへのポインタを取得

			// ワールドマトリックスの設定
			SetMtxWorld(pDevice);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデバイスのデータストリームにバインド
			pDevice->SetIndices(m_pIdxBuff);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			if (pTexture != NULL)
			{
				pDevice->SetTexture(0, pTexture);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
		}
	}
}

//=============================================================================
//    メッシュのワールドマトリックス計算処理
//=============================================================================
void CScene3DMesh::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	// 各種値の取得
	D3DXMATRIX mtxWorld = GetMtxWorld();  // ワールドマトリックス
	D3DXVECTOR3 rot = GetRot();           // 向き
	D3DXVECTOR3 pos = GetPos();           // 座標

	D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CScene3D::SetMtxWorld(mtxWorld);
}

//=============================================================================
//    メッシュの頂点バッファ生成処理
//=============================================================================
void CScene3DMesh::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファの取得

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);
}

//=============================================================================
//    メッシュのインデックスバッファ生成処理
//=============================================================================
void CScene3DMesh::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);
}

//=============================================================================
//    メッシュのインデックスバッファ設定処理
//=============================================================================
void CScene3DMesh::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    メッシュの頂点数設定処理
//=============================================================================
void CScene3DMesh::SetNumVertex(const int nNumVertex)
{
	m_nNumVertex = nNumVertex;
}

//=============================================================================
//    メッシュのインデックス数設定処理
//=============================================================================
void CScene3DMesh::SetNumIndex(const int nNumIndex)
{
	m_nNumIndex = nNumIndex;
}

//=============================================================================
//    メッシュのポリゴン数設定処理
//=============================================================================
void CScene3DMesh::SetNumPolygon(const int nNumPolygon)
{
	m_nNumPolygon = nNumPolygon;
}

//=============================================================================
//    メッシュのインデックスバッファ取得処理
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CScene3DMesh::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    メッシュの頂点数取得処理
//=============================================================================
int CScene3DMesh::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    メッシュのインデックス数取得処理
//=============================================================================
int CScene3DMesh::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    メッシュのポリゴン数取得処理
//=============================================================================
int CScene3DMesh::GetNumPolygon(void)
{
	return m_nNumPolygon;
}
