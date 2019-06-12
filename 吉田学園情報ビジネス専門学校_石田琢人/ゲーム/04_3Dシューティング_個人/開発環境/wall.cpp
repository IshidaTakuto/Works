//=============================================================================
//
// 壁処理 [wall.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "wall.h"
#include "scene3DModel.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WALL_MODELNAME00	"data/model/rock.x"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9	CWall::m_pTexture = NULL;	// テクスチャ情報へのポインタ
LPD3DXMESH			CWall::m_pMesh = NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER		CWall::m_pBuffMat = NULL;	// マテリアル情報へのポインタ
DWORD				CWall::m_nNumMat = NULL;	// マテリアル情報の数

//==================================
// 生成処理
//==================================
CWall *CWall::Create(int nPos)
{
	CWall *pWall;

	pWall = new CWall;	// メモリを確保

	if (pWall != NULL)
	{// メモリが確保できた
		pWall->m_nPos = nPos;	// 基準のX座標の設定
		pWall->Init();			// 初期化処理
	}

	return pWall;
}

//=========================================
// モデル情報の読み込み
//=========================================
HRESULT CWall::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(WALL_MODELNAME00,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// テクスチャの生成
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture);

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CWall::Unload(void)
{
	if (NULL != m_pTexture)
	{// テクスチャの破棄
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (NULL != m_pMesh)
	{// メッシュ情報の破棄
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (NULL != m_pBuffMat)
	{// マテリアル情報の破棄
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	if (NULL != m_nNumMat)
	{// マテリアル数を初期化する
		m_nNumMat = 0;
	}
}

//=========================================
// コンストラクタ
//=========================================
CWall::CWall(int nPriority) : CScene(nPriority)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL_ROCK; nCntWall++)
	{// モデルクラスへのポインタ変数
		m_pModel[nCntWall] = NULL;
	}

	m_nPos = 0;		// 基準のX座標
}

//=========================================
// デストラクタ
//=========================================
CWall::~CWall()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWall::Init()
{
	for (int nCntWall = 0; nCntWall < MAX_WALL_ROCK; nCntWall++)
	{
		m_pModel[nCntWall] = new CScene3DModel;		// メモリの確保

		if (m_pModel[nCntWall] != NULL)
		{// メモリが確保できた
			m_pModel[nCntWall]->BindInfo(m_pTexture, m_pMesh, m_pBuffMat, m_nNumMat);	// モデル情報の設定

			// 初期化処理
			m_pModel[nCntWall]->Init();

			//_位置の設定
			m_pModel[nCntWall]->SetPos(D3DXVECTOR3((float)m_nPos + (rand() % 200 + 1.0f) + (nCntWall * 100.0f), 0.0f, (nCntWall % 2) * -1000.0f + 500.0f));

			// 向きの設定
			m_pModel[nCntWall]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI - (rand() % 6) - (rand() % 10 * 0.01f), 0.0f));
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CWall::Uninit(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL_ROCK; nCntWall++)
	{// モデル分ループ
		if (m_pModel[nCntWall] != NULL)
		{// モデルの破棄
			m_pModel[nCntWall]->Uninit();
			m_pModel[nCntWall] = NULL;
		}
	}

	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CWall::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CWall::Draw(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL_ROCK; nCntWall++)
	{// モデル分ループ
		m_pModel[nCntWall]->Draw();
	}
}
