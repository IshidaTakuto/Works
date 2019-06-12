//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODELNAME_00		"data/MODEL/item.x"		// 読み込むxファイル名

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		CModel::m_pTexture[MAX_MODEL] = {};		// テクスチャへのポインタ
LPD3DXMESH				CModel::m_pMesh[MAX_MODEL] = {};		// メッシュ情報へのポインタ
LPD3DXBUFFER			CModel::m_pBuffMat[MAX_MODEL] = {};		// マテリアル情報へのポインタ
DWORD					CModel::m_nNumMat[MAX_MODEL] = {};		// マテリアル情報の数

//=========================================
// 生成処理
//=========================================
CModel* CModel::Create(D3DXVECTOR3 pos, TYPE type)
{
	CModel *pModel;

	pModel = new CModel;	// メモリを確保
	pModel->BindInfo(m_pTexture[type], m_pMesh[type], m_pBuffMat[type], m_nNumMat[type]);	// モデルの情報をCScene3DModelに渡す
	pModel->Init(pos, type);
	return pModel;
}

//=========================================
// モデルの読み込み
//=========================================
HRESULT CModel::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	for (int nCntModel = 0; nCntModel < TYPE_MAX; nCntModel++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(MODELNAME_00,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMat[nCntModel],
			NULL,
			&m_nNumMat[nCntModel],
			&m_pMesh[nCntModel]);

		pMat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
		D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[nCntModel]);
	}

	return S_OK;
}

//=========================================
// 読み込んだモデルの解放
//=========================================
void CModel::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		// テクスチャの破棄
		if (NULL != m_pTexture[nCntModel])
		{
			m_pTexture[nCntModel]->Release();
			m_pTexture[nCntModel] = NULL;
		}

		if (NULL != m_pMesh[nCntModel])
		{
			m_pMesh[nCntModel]->Release();
			m_pMesh[nCntModel] = NULL;
		}

		if (NULL != m_pBuffMat[nCntModel])
		{
			m_pBuffMat[nCntModel]->Release();
			m_pBuffMat[nCntModel] = NULL;
		}

		if (NULL != m_nNumMat[nCntModel])
		{
			m_nNumMat[nCntModel] = NULL;
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CModel::CModel()
{
}

//=========================================
// デストラクタ
//=========================================
CModel::~CModel()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, TYPE type)
{
	CScene3DModel::Init();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	CScene3DModel::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{
	CScene3DModel::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	CScene3DModel::Draw();
}
