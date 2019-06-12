//=============================================================================
//
// メッシュシリンダー処理 [meshCylinder.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "meshCylinder.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MESHCYLINDER		"data/TEXTURE/river000.jpg"	// 読み込むテクスチャファイル名
#define MESH_HORIZONTAL	(20)			// 水平方向の分割数
#define MESH_VERTICAL	(1)				// 垂直方向の分割数
#define MESH_HEIGHT		(50.0f)			// 高さ
#define MESH_RADIUS		(100.0f)			// 半径の長さ
#define MAX_VERTEX		((MESH_HORIZONTAL + 1) * (MESH_VERTICAL + 1))							// 頂点数
#define MAX_INDEX		(MESH_HORIZONTAL * MESH_VERTICAL * 2 + (4 * (MESH_VERTICAL - 1)) + 2)	// インデックスの数
#define MAX_POLYGON		((MESH_HORIZONTAL * MESH_VERTICAL * 2) + (4 * (MESH_VERTICAL - 1)))		// ポリゴン数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;	// インデックスへのポインタ
MeshCylinder g_MeshCylinder;

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHCYLINDER, &g_pTextureMeshCylinder);

	g_MeshCylinder.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置の初期化
	g_MeshCylinder.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きの初期化
	g_MeshCylinder.fAngle = 1.0f / (MESH_HORIZONTAL / 2);	// 角度
	g_MeshCylinder.fRadius = MESH_RADIUS;					// 半径

	// 頂点情報の作成
	MakeVertexMeshCylinder(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshCylinder(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	// インデックスバッファの開放
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshCylinder(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_MeshCylinder.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshCylinder.rot.y, g_MeshCylinder.rot.x, g_MeshCylinder.rot.z);
	D3DXMatrixMultiply(&g_MeshCylinder.mtxWorld, &g_MeshCylinder.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshCylinder.pos.x, g_MeshCylinder.pos.y, g_MeshCylinder.pos.z);
	D3DXMatrixMultiply(&g_MeshCylinder.mtxWorld, &g_MeshCylinder.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshCylinder.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_VERTEX, 0, MAX_POLYGON);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMeshV;
	int nCntMeshH;
	int nCntIdx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の設定

	for (nCntMeshV = 0; nCntMeshV <= MESH_VERTICAL; nCntMeshV++)
	{
		for (nCntMeshH = 0; nCntMeshH <= MESH_HORIZONTAL; nCntMeshH++)
		{

			// 頂点の座標
			pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (g_MeshCylinder.fAngle * nCntMeshH))) * g_MeshCylinder.fRadius;		// ずれる
			pVtx[0].pos.y = MESH_HEIGHT - (nCntMeshV * MESH_HEIGHT);													// Y軸を下へずらす
			pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (g_MeshCylinder.fAngle * nCntMeshH))) * g_MeshCylinder.fRadius;		// ずれる

			//if (g_MeshCylinder.fAngle == D3DX_PI)
			//{
			//	g_MeshCylinder.fAngle = g_MeshCylinder.fAngle * -1.0f;
			//}

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);	// 正規化する

			// 頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(nCntMeshH * 1.0f, nCntMeshV * 1.0f);

			pVtx += 1;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMeshCylinder->Unlock();

	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshH = 0, nCntIdx = 0; nCntMeshH <= MESH_HORIZONTAL; nCntMeshH++, nCntIdx++)
		{
			pIdx[0] = (MESH_HORIZONTAL + 1) + nCntIdx;
			pIdx[1] = 0 + nCntIdx;

			pIdx += 2;
	}

	// 頂点バッファをアンロック
	g_pIdxBuffMeshCylinder->Unlock();
}
