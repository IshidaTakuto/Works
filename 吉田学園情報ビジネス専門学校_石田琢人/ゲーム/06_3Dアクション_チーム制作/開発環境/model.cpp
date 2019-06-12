//*****************************************************************************
//
//     モデルの処理[model.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define XFILE_NAME  "data/MODEL/coin000.x"     // モデルのファイル名

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CModel::CModel()
{
	// 各種値のクリア
	m_pTexture = NULL;                          // テクスチャへのポインタ
	m_pMesh = NULL;                             // メッシュ情報へのポインタ
	m_pBuffMat = NULL;                          // マテリアル情報へのポインタ
	m_nNumMat = 0;                              // マテリアル情報の数
	m_VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 頂点の最大値
	m_VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 頂点の最小値
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 現在の座標
	m_InitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 初期化時の座標
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 前回の座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 向き
	D3DXMatrixIdentity(&m_MtxWorld);            // ワールドマトリックス
	m_pParent = NULL;                           // 親モデルへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
//    モデルの生成処理
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax)
{
	CModel *pModel= NULL;   // モデルクラス型のポインタ
	if (pModel == NULL)
	{// メモリが空になっている
		pModel = new CModel;
		if (pModel != NULL)
		{// インスタンスを生成できた
			if (FAILED(pModel->Init(pos, rot, pMesh, pBuffMat, nNumMat, VtxMin, VtxMin)))
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

	return pModel;   // インスタンスのアドレスを返す
}

//=============================================================================
//    モデルの初期化処理
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax)
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
			m_InitPos = pos;   // 初期化時の座標
			m_Pos = pos;       // 現在の座標
			m_Rot = rot;       // 現在の向き
			m_VtxMin = VtxMin; // 頂点座標の最小値
			m_VtxMax = VtxMax; // 頂点座標の最大値

		    // モデルの割り当て
			BindModel(pDevice, pMesh, pBuffMat, nNumMat);
		}
	}

	return S_OK;
}

//=============================================================================
//    モデルの終了処理
//=============================================================================
void CModel::Uninit(void)
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
		delete[] m_pTexture;
		m_pTexture = NULL;
	}
}

//=============================================================================
//    モデルの更新処理
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
//    モデルの描画処理
//=============================================================================
void CModel::Draw(void)
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
//    モデルのワールドマトリックス計算処理
//=============================================================================
void CModel::CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxParent; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x + m_AddPos.x, m_Pos.y + m_AddPos.y, m_Pos.z + m_AddPos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// 親モデルのワールドマトリックスを掛け合わせる
	if (m_pParent != NULL)
	{// 親モデルが存在する
		mtxParent = m_pParent->GetMtxWorld();  // 親モデルからワールドマトリックスを取得する
	}
	else
	{// 親モデルが存在しない
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);  // デバイスからワールドマトリックスを取得する
	}
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    モデルのモデル情報設定処理
//=============================================================================
void CModel::BindModel(const LPDIRECT3DDEVICE9 pDevice, const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat)
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
	if (nNumMat != -1)
	{// マテリアルの数情報が読み込めた
		m_nNumMat = nNumMat;   // マテリアル情報の数
	}

	// マテリアル情報からテクスチャ情報を引き出す
	if (m_pBuffMat != NULL)
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
//    モデルの頂点座標最小値設定処理
//=============================================================================
void CModel::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    モデルの頂点座標最大値設定処理
//=============================================================================
void CModel::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    モデルの現在の座標設定処理
//=============================================================================
void CModel::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    モデルのオフセットに加える座標設定処理
//=============================================================================
void CModel::SetAddPos(const D3DXVECTOR3 AddPos)
{
	m_AddPos = AddPos;
}

//=============================================================================
//    モデルの前回の座標設定処理
//=============================================================================
void CModel::SetPosOld(const D3DXVECTOR3 posOld)
{
	m_PosOld = posOld;
}

//=============================================================================
//    モデルの向き設定処理
//=============================================================================
void CModel::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    モデルのワールドマトリックス設定処理
//=============================================================================
void CModel::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    モデルの親モデル設定処理
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}

//=============================================================================
//    モデルの頂点座標最小値取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    モデルの頂点座標最大値取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    モデルの現在の座標取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    モデルのオフセットに加える座標取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetAddPos(void)
{
	return m_AddPos;
}

//=============================================================================
//    モデルの初期化時の座標取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetInitPos(void)
{
	return m_InitPos;
}

//=============================================================================
//    モデルの前回の座標設定処理
//=============================================================================
D3DXVECTOR3 CModel::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
//    モデルの向き取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    モデルのワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    モデルのメッシュ情報取得処理
//=============================================================================
LPD3DXMESH CModel::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    モデルのマテリアル情報取得処理
//=============================================================================
LPD3DXBUFFER CModel::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    モデルのマテリアルの数取得処理
//=============================================================================
DWORD CModel::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    モデルのテクスチャ情報取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CModel::GetTexture(int nIdxTex)
{
	return m_pTexture[nIdxTex];
}