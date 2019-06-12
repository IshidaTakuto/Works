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
#define TEXTURE_MESHCYLINDER		"data/TEXTURE/circle.png"	// 読み込むテクスチャファイル名
#define MESH_HORIZONTAL	(20)			// 水平方向の分割数
#define MESH_VERTICAL	(1)				// 垂直方向の分割数
#define CYLINDER_HEIGHT	(200.0f)		// 高さ

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
CCylinder* CCylinder::Create(D3DXVECTOR3 pos)
{
	CCylinder *pCylinder;

	pCylinder = new CCylinder;	// メモリを確保
	if (pCylinder != NULL)
	{// メモリ確保できた
		pCylinder->Init(pos);
		pCylinder->BindTexture(m_pTexture[0]);
	}

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
	m_fRange = 0.0f;
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
HRESULT CCylinder::Init(D3DXVECTOR3 pos)
{
	CScene3DIndex::Init();
	CScene::SetObjType(CScene::OBJTYPE_CYLINDER);
	CScene3DIndex::MakeVertex();
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
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshYZ = 0; nCntMeshYZ <= MESH_VERTICAL; nCntMeshYZ++)
	{
		for (int nCntMeshX = 0; nCntMeshX <= MESH_HORIZONTAL; nCntMeshX++)
		{

			float fAngle = 1.0f / (MESH_HORIZONTAL / 2);	// 角度

			pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * m_fRange;		// ずれる
			pVtx[0].pos.y = CYLINDER_HEIGHT - (nCntMeshYZ * CYLINDER_HEIGHT) * 1.0f;		// Y軸を下へずらす
			pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * m_fRange;		// ずれる
		}
	}

	// 頂点バッファをアンロック
	pVtxBuff->Unlock();
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
