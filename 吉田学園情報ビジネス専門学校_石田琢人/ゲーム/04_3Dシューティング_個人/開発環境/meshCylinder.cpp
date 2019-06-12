//=============================================================================
//
// メッシュシリンダー処理 [meshCylinder.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "meshCylinder.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MESHCYLINDER		"data/TEXTURE/sky.jpg"	// 読み込むテクスチャファイル名
#define MESH_HORIZONTAL	(20)			// 水平方向の分割数
#define MESH_VERTICAL	(1)				// 垂直方向の分割数
#define MAX_VERTEX		((MESH_HORIZONTAL + 1) * (MESH_VERTICAL + 1))							// 頂点数
#define MAX_INDEX		(MESH_HORIZONTAL * MESH_VERTICAL * 2 + (4 * (MESH_VERTICAL - 1)) + 2)	// インデックスの数
#define MAX_POLYGON		((MESH_HORIZONTAL * MESH_VERTICAL * 2) + (4 * (MESH_VERTICAL - 1)))		// ポリゴン数

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CCylinder::m_pTexture[CYLINDER_TEXTURE] = {};

//=============================================================================
// 生成処理
//=============================================================================
CCylinder* CCylinder::Create(D3DXVECTOR3 pos, int nWidth, int nHeight, int nDepth, int nMeshX, int nMeshY, int nMeshZ, int nTexture)
{
	CCylinder *pCylinder;

	pCylinder = new CCylinder;	// メモリを確保
	pCylinder->Init(pos, nWidth, nHeight, nDepth, nMeshX, nMeshY, nMeshZ);
	pCylinder->BindTexture(m_pTexture[nTexture]);

	return pCylinder;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CCylinder::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHCYLINDER, &m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// 読み込んだテクスチャの解放
//=============================================================================
void CCylinder::Unload(void)
{
	//テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < CYLINDER_TEXTURE; nCntTexture++)
	{
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CCylinder::CCylinder(int nPriority) : CScene3DIndex(nPriority)
{
	m_nTexture = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCylinder::~CCylinder()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCylinder::Init(D3DXVECTOR3 pos, int nWidth, int nHeight, int nDepth, int nMeshX, int nMeshY, int nMeshZ)
{
	CScene3DIndex::Init();
	CScene::SetObjType(CScene::OBJTYPE_CYLINDER);
	CScene3DIndex::MakeVertex(nMeshX, nMeshY, nMeshZ, nWidth, nHeight, nDepth, TYPE_CYLINDER);
	CScene3DIndex::SetPos(pos);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCylinder::Uninit(void)
{
	CScene3DIndex::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCylinder::Update(void)
{
	// 回転させる
	D3DXVECTOR3 rot = CScene3DIndex::GetRot();
	rot.y -= 0.0001f;
	CScene3DIndex::SetRot(rot);
}

//=============================================================================
// 描画処理
//=============================================================================
void CCylinder::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	CCamera *pCamera = CManager::GetCamera();

	D3DXMATRIX projection = pCamera->GetProjection();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&projection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&projection,
		D3DXToRadian(45.0f),							// 画角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// アスペクト比
		10.0f,											// カメラの範囲（手前）
		10000.0f);										// カメラの範囲（奥行）

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &projection);

	CScene3DIndex::Draw();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&projection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&projection,
		D3DXToRadian(45.0f),							// 画角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// アスペクト比
		10.0f,											// カメラの範囲（手前）
		8000.0f);										// カメラの範囲（奥行）

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &projection);
}
