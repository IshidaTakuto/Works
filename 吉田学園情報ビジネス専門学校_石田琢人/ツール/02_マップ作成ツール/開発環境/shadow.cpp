//=============================================================================
//
// 影処理 [shadow.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "meshField.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//==================================
// 生成処理
//==================================
CShadow *CShadow::Create(void)
{
	CShadow *pShadow;

	pShadow = new CShadow;	// メモリを確保

	if (pShadow != NULL)
	{// メモリ確保成功
		pShadow->Init();
	}

	return pShadow;
}

//=========================================
// コンストラクタ
//=========================================
CShadow::CShadow(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
}

//=========================================
// デストラクタ
//=========================================
CShadow::~CShadow()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShadow::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	// オブジェクトの破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CShadow::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CShadow::Draw(void)
{
}

//=============================================================================
// モデルに影をつける
//=============================================================================
void CShadow::Set(D3DXMATRIX mtxWorld, LPD3DXMESH mesh, D3DXVECTOR3 posObj)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DXPLANE planeField;		// 地面の平面
	D3DXVECTOR4 vecLight;		// ライトのベクトル
	D3DXVECTOR3 pos, normal;	// 位置と法線

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// ライトのベクトルの設定
	D3DXVECTOR3 vecDir = CLight::GetDirection();
	vecLight = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);

	// 地面オブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);
	float fHeight = 0.0f;

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき
			CMeshField *pMeshField = (CMeshField*)pScene;

			fHeight = pMeshField->GetHeight(posObj);	// 地面の高さの取得
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	// 影の位置
	pos = D3DXVECTOR3(0.0f, fHeight + 1.0f, 0.0f);

	// 影の法線
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 点と法線から平面を作成する
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	// 平面に射影するマトリックスを作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	// ワールドマトリックスと掛け合わせる
	D3DXMatrixMultiply(&mtxShadow, &mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// テクスチャをNULLにする
	pDevice->SetTexture(0, NULL);

	// 影の描画
	mesh->DrawSubset(0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}