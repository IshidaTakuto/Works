//*****************************************************************************
//
//     Xファイルモデルの処理[sceneX.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"


//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define XFILE_NAME  "data/MODEL/object/coin000.x"     // モデルのファイル名

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************
LPD3DXMESH     CSceneX::m_apMesh = NULL;                                              // メッシュ情報へのポインタ
LPD3DXBUFFER   CSceneX::m_apBuffMat = NULL;                                           // マテリアル情報へのポインタ
DWORD          CSceneX::m_anNumMat = NULL;                                            // マテリアル情報の数
D3DXVECTOR3    CSceneX::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);    // 頂点座標の最小値
D3DXVECTOR3    CSceneX::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f); // 頂点座標の最小値

//=============================================================================
//    コンストラクタ
//=============================================================================
CSceneX::CSceneX(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTexture = NULL;                        // テクスチャへのポインタ
	m_pMesh = NULL;                           // メッシュ情報へのポインタ
	m_pBuffMat = NULL;                        // マテリアル情報へのポインタ
	m_nNumMat = 0;                            // マテリアル情報の数
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 現在の座標
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 前回の座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 向き
	D3DXMatrixIdentity(&m_MtxWorld);          // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
//    Xファイルモデルの生成処理
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	CSceneX *pSceneX = NULL;   // Xファイルモデルクラス型のポインタ
	if (pSceneX == NULL)
	{// メモリが空になっている
		pSceneX = new CSceneX(nPriority);
		if (pSceneX != NULL)
		{// インスタンスを生成できた
			pSceneX->SetPos(pos);
			pSceneX->SetRot(rot);
			if (FAILED(pSceneX->Init()))
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

	return pSceneX;
}

//=============================================================================
//    Xファイルモデルの読み込み処理
//=============================================================================
HRESULT CSceneX::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	     // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
	        // xファイルの読み込み
			D3DXLoadMeshFromX(XFILE_NAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);

			int nNumVtx;     // 頂点数
			DWORD sizeFVF;   // 頂点フォーマットのサイズ
			BYTE *pVtxBuff;  // 頂点バッファへのポインタ

			// 頂点数を獲得
			nNumVtx = m_apMesh->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(m_apMesh->GetFVF());

			// 頂点バッファをロック
			m_apMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// すべての頂点を比較してモデルの最小値,最大値を抜き出す
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

				if (Vtx.x < m_aVtxMin.x)
				{// 取り出したX座標の最小値が今までのよりも小さい
					m_aVtxMin.x = Vtx.x;  // 最小値更新
				}
				if (Vtx.y < m_aVtxMin.y)
				{// 取り出したY座標の最小値が今までのよりも小さい
					m_aVtxMin.y = Vtx.y;  // 最小値更新
				}
				if (Vtx.z < m_aVtxMin.z)
				{// 取り出したZ座標の最小値が今までのよりも小さい
					m_aVtxMin.z = Vtx.z;  // 最小値更新
				}
				if (Vtx.x > m_aVtxMax.x)
				{// 取り出したX座標の最大値が今までのよりも大きい
					m_aVtxMax.x = Vtx.x;  // 最大値更新
				}
				if (Vtx.y > m_aVtxMax.y)
				{// 取り出したY座標の最大値が今までのよりも大きい
					m_aVtxMax.y = Vtx.y;  // 最大値更新
				}
				if (Vtx.z > m_aVtxMax.z)
				{// 取り出したZ座標の最大値が今までのよりも大きい
					m_aVtxMax.z = Vtx.z;  // 最大値更新
				}

				pVtxBuff += sizeFVF;  // サイズ分ポインタを進める
			}
			// 頂点バッファをアンロック
			m_apMesh->UnlockVertexBuffer();
		}
	}

	return S_OK;
}

//=============================================================================
//    Xファイルモデルの開放処理
//=============================================================================
void CSceneX::UnLoad(void)
{
	// メッシュの破棄
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// マテリアルの破棄
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    Xファイルモデルの初期化処理
//=============================================================================
HRESULT CSceneX::Init(void)
{

	return S_OK;
}

//=============================================================================
//    Xファイルモデルの終了処理
//=============================================================================
void CSceneX::Uninit(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{// メモリが確保されている
		for (DWORD nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
		{
			if (m_pTexture[nCntMat] != NULL)
			{
				m_pTexture[nCntMat]->Release();
				m_pTexture[nCntMat] = NULL;
			}
		}
		delete m_pTexture;
		m_pTexture = NULL;
	}

	Release();  // 自身のオブジェクトを破棄
}

//=============================================================================
//    Xファイルモデルの更新処理
//=============================================================================
void CSceneX::Update(void)
{

}

//=============================================================================
//    Xファイルモデルの描画処理
//=============================================================================
void CSceneX::Draw(void)
{
	D3DMATERIAL9 matDef;   // 現在のマテリアル保存用
	D3DXMATERIAL *pMat;    // マテリアルデータへのポインタ

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの計算処理
	CalcMtxWorld(pDevice);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// 設定されていたマテリアルの数だけ繰り返し
	    // マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (m_pTexture[nCntMat] != NULL)
		{// テクスチャ情報が引き出せている
		    // テクスチャの設定
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}
		else
		{// テクスチャ情報が引き出せていない
		    // テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//    Xファイルモデルのワールドマトリックス計算処理
//=============================================================================
void CSceneX::CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
	SetMtxWorld(m_MtxWorld);
}

//=============================================================================
//    Xファイルモデルのモデル情報設定処理
//=============================================================================
void CSceneX::BindModel(const LPDIRECT3DDEVICE9 pDevice, const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat)
{
	// モデル情報の割り当て
	if (pMesh != NULL)
	{// メッシュ情報が読み込めた
		m_pMesh = pMesh;       // メッシュ情報へのポインタ
	}
	if (pBuffMat != NULL)
	{// マテリアル情報へのポインタを読み込めた
		m_pBuffMat = pBuffMat; // マテリアル情報へのポインタ
	}
	if (nNumMat >= 0)
	{// マテリアルの数情報が読み込めた
		m_nNumMat = nNumMat;   // マテリアル情報の数
	}

	// マテリアル情報からテクスチャ情報を引き出す
	if (m_pBuffMat != NULL && m_nNumMat >= 0)
	{// マテリアル情報が読み込めている
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();  // マテリアル情報のポインタを取得
		D3DMATERIAL9* pMatBuff = new D3DMATERIAL9[m_nNumMat];                // マテリアル情報の数だけポインタを動的確保
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];                      // テクスチャ情報へのポインタをマテリアル情報の数だけ動的確保

		for (DWORD nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
		{// 頂点数の数だけ繰り返し
		 // マテリアル情報を読み込む
			pMatBuff[nCntMat] = pMat[nCntMat].MatD3D;

			// 環境光を初期化する
			pMatBuff[nCntMat].Ambient = pMatBuff[nCntMat].Diffuse;

			// テクスチャ情報を初期化
			m_pTexture[nCntMat] = NULL;

			// テクスチャの情報を読み込む
			if (pMat[nCntMat].pTextureFilename != NULL &&
				lstrlen(pMat[nCntMat].pTextureFilename) > 0)
			{// テクスチャのファイル名がある
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_pTexture[nCntMat]);
			}
		}

		delete[] pMatBuff;
	}
}

//=============================================================================
//    Xファイルモデルの頂点座標最小値設定処理
//=============================================================================
void CSceneX::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    Xファイルモデルの頂点座標最大値設定処理
//=============================================================================
void CSceneX::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    Xファイルモデルの現在の座標設定処理
//=============================================================================
void CSceneX::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    Xファイルモデルの前回の座標設定処理
//=============================================================================
void CSceneX::SetPosOld(const D3DXVECTOR3 posOld)
{
	m_PosOld = posOld;
}

//=============================================================================
//    Xファイルモデルの向き設定処理
//=============================================================================
void CSceneX::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    Xファイルモデルのワールドマトリックス設定処理
//=============================================================================
void CSceneX::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    Xファイルモデルの頂点座標最小値取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    Xファイルモデルの頂点座標最大値取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    Xファイルモデルの現在の座標取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    Xファイルモデルの前回の座標設定処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
//    Xファイルモデルの向き取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    Xファイルモデルのワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CSceneX::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    Xファイルモデルのメッシュ情報取得処理
//=============================================================================
LPD3DXMESH CSceneX::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    Xファイルモデルのマテリアル情報取得処理
//=============================================================================
LPD3DXBUFFER CSceneX::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    Xファイルモデルのマテリアルの数取得処理
//=============================================================================
DWORD CSceneX::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    Xファイルモデルのテクスチャ情報取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CSceneX::GetTexture(int nIdxTex)
{
	return m_pTexture[nIdxTex];
}