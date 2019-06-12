//*****************************************************************************
//
//     モデル管轄の処理[modelManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "modelManager.h"

//=============================================================================
//    コンストラクタ
//=============================================================================
CModelManager::CModelManager()
{
	// 各種値のクリア
	m_nNumModel = 0;    // 読み込んだモデルの数
	m_pMesh = NULL;     // モデルのメッシュ情報
	m_pBuffMat = NULL;  // モデルのマテリアル情報
	m_pNumMat = NULL;   // モデルのマテリアルの数情報
	m_pVtxMax = NULL;   // 頂点座標の最大値へのポインタ
	m_pVtxMin = NULL;   // 頂点座標の最小値へのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CModelManager::~CModelManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CModelManager *CModelManager::Create(int nNumModel)
{
	CModelManager *pModelManager = NULL;  // モデル管轄クラス型のポインタ
	if (pModelManager == NULL)
	{// メモリが確保できる状態にある
		pModelManager = new CModelManager;
		if (pModelManager != NULL)
		{// メモリが確保できた
			pModelManager->SetNumModel(nNumModel);  // モデルの数を設定
		    // 初期化処理を行う
			if (FAILED(pModelManager->Init()))
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

	// インスタンスを返す
	return pModelManager;
}

//=============================================================================
//    モデルの読み込み処理
//=============================================================================
HRESULT CModelManager::Init(void)
{
	if (m_nNumModel >= 1)
	{// 読み込むモデルの数が1個以上ある
		// 読み込むモデルの数だけメモリを確保する
		// メッシュ情報
		if (m_pMesh == NULL)
		{// メモリが確保できる状態にある
			m_pMesh = new LPD3DXMESH[m_nNumModel];
		}

		// マテリアル情報
		if (m_pBuffMat == NULL)
		{// メモリが確保できる状態にある
			m_pBuffMat = new LPD3DXBUFFER[m_nNumModel];
		}

		// マテリアルの数情報
		if (m_pNumMat == NULL)
		{// メモリが確保できる状態にある
			m_pNumMat = new DWORD[m_nNumModel];
		}

		// 頂点座標の最大値情報
		if (m_pVtxMax == NULL)
		{// メモリが確保できる状態にある
			m_pVtxMax = new D3DXVECTOR3[m_nNumModel];
		}

		// 頂点座標の最小値情報
		if (m_pVtxMin == NULL)
		{// メモリが確保できる状態にある
			m_pVtxMin = new D3DXVECTOR3[m_nNumModel];
		}
	}

	return S_OK;
}

//=============================================================================
//    モデルの開放処理
//=============================================================================
void CModelManager::Uninit(void)
{
	// メッシュ情報の開放
	if (m_pMesh != NULL)
	{// メモリが確保されている
		ReleaseMesh();
	}

	// マテリアル情報の開放
	if (m_pBuffMat != NULL)
	{// メモリが確保されている
		ReleaseBuffMat();
	}

	// マテリアルの数情報の開放
	if (m_pNumMat != NULL)
	{// メモリが確保されている
		ReleaseNumMat();
	}

	// 頂点の最大値情報の開放
	if (m_pVtxMax != NULL)
	{// メモリが確保されている
		ReleaseVtxMax();
	}

	// 頂点の最小値情報の開放
	if (m_pVtxMin != NULL)
	{// メモリが確保されている
		ReleaseVtxMin();
	}
}

//=============================================================================
//    読み込むモデルの数を設定する
//=============================================================================
void CModelManager::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//=============================================================================
//    モデルのメッシュ情報を設定する
//=============================================================================
void CModelManager::SetMesh(LPD3DXMESH pMesh, int nIdx)
{
	m_pMesh[nIdx] = pMesh;
}

//=============================================================================
//    モデルのマテリアル情報を設定する
//=============================================================================
void CModelManager::SetBuffMat(LPD3DXBUFFER pBuffMat, int nIdx)
{
	m_pBuffMat[nIdx] = pBuffMat;
}

//=============================================================================
//    モデルのマテリアルの数情報を設定する
//=============================================================================
void CModelManager::SetNumMat(DWORD nNumMat, int nIdx)
{
	m_pNumMat[nIdx] = nNumMat;
}

//=============================================================================
//    モデルの頂点の最大値情報を設定する
//=============================================================================
void CModelManager::SetVtxMax(D3DXVECTOR3 VtxMax, int nIdx)
{
	m_pVtxMax[nIdx] = VtxMax;
}

//=============================================================================
//    モデルの頂点の最小値情報を設定する
//=============================================================================
void CModelManager::SetVtxMin(D3DXVECTOR3 VtxMin, int nIdx)
{
	m_pVtxMin[nIdx] = VtxMin;
}

//=============================================================================
//    モデルのメッシュ情報を取得する
//=============================================================================
LPD3DXMESH CModelManager::GetMesh(int nIdx)
{
	LPD3DXMESH pMesh = NULL;  // メッシュ情報

	if (m_pMesh != NULL)
	{// メモリが確保されてる
		pMesh = m_pMesh[nIdx];   // 保存されたメッシュ情報を代入
	}

	return pMesh;  // メッシュ情報を返す
}

//=============================================================================
//    モデルのマテリアル情報を取得する
//=============================================================================
LPD3DXBUFFER CModelManager::GetBuffMat(int nIdx)
{
	LPD3DXBUFFER pBuffMat = NULL;  // マテリアル情報

	if (m_pBuffMat != NULL)
	{// メモリが確保されてる
		pBuffMat = m_pBuffMat[nIdx];   // 保存されたマテリアル情報を代入
	}

	return pBuffMat;  // マテリアル情報を返す
}

//=============================================================================
//    モデルのマテリアルの数情報を取得する
//=============================================================================
DWORD CModelManager::GetNumMat(int nIdx)
{
	DWORD nNumMat = 0;  // マテリアルの数情報

	if (m_pNumMat != NULL)
	{// メモリが確保されてる
		nNumMat = m_pNumMat[nIdx];   // 保存されたマテリアルの数情報を代入
	}

	return nNumMat;  // マテリアルの数情報を返す
}

//=============================================================================
//    モデルの頂点の最大値情報を取得する
//=============================================================================
D3DXVECTOR3 CModelManager::GetVtxMax(int nIdx)
{
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 頂点の最大値情報

	if (m_pVtxMax != NULL)
	{// メモリが確保されてる
		VtxMax = m_pVtxMax[nIdx];   // 保存された頂点の最大値情報を代入
	}

	return VtxMax;  // 頂点の最大値情報を返す
}

//=============================================================================
//    モデルの頂点の最小値情報を取得する
//=============================================================================
D3DXVECTOR3 CModelManager::GetVtxMin(int nIdx)
{
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 頂点の最小値情報

	if (m_pVtxMin != NULL)
	{// メモリが確保されてる
		VtxMin = m_pVtxMin[nIdx];   // 保存された頂点の最小値情報を代入
	}

	return VtxMin;  // 頂点の最小値情報を返す
}

//=============================================================================
//    モデルのメッシュ情報を開放する
//=============================================================================
void CModelManager::ReleaseMesh(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{// 読み込んだモデルの数だけ繰り返し
		if (m_pMesh[nCntModel] != NULL)
		{// メモリが確保されている
			m_pMesh[nCntModel]->Release();
			m_pMesh[nCntModel] = NULL;
		}
	}
	delete[] m_pMesh;
	m_pMesh = NULL;
}

//=============================================================================
//    モデルのマテリアル情報を開放する
//=============================================================================
void CModelManager::ReleaseBuffMat(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{// 読み込んだモデルの数だけ繰り返し
		if (m_pBuffMat[nCntModel] != NULL)
		{// メモリが確保されている
			m_pBuffMat[nCntModel]->Release();
			m_pBuffMat[nCntModel] = NULL;
		}
	}
	delete[] m_pBuffMat;
	m_pBuffMat = NULL;
}

//=============================================================================
//    モデルのマテリアルの数情報を開放する
//=============================================================================
void CModelManager::ReleaseNumMat(void)
{
	delete[] m_pNumMat;
	m_pNumMat = NULL;
}

//=============================================================================
//    モデルの頂点の最大値情報を開放する
//=============================================================================
void CModelManager::ReleaseVtxMax(void)
{
	delete[] m_pVtxMax;
	m_pVtxMax = NULL;
}

//=============================================================================
//    モデルの頂点の最小値情報情報を開放する
//=============================================================================
void CModelManager::ReleaseVtxMin(void)
{
	delete[] m_pVtxMin;
	m_pVtxMin = NULL;
}