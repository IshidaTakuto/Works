//=============================================================================
//
// メッシュドーム処理 [meshDome.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "meshDome.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MESHDOME		"data/TEXTURE/river000.jpg"	// 読み込むテクスチャファイル名
#define MESH_HORIZONTAL	(8)				// 水平方向の分割数
#define MESH_VERTICAL	(4)				// 垂直方向の分割数
#define MESH_HEIGHT		(50.0f)			// 高さ
#define MESH_RADIUS		(50.0f)			// 半径の長さ
#define MAX_VERTEX		((MESH_HORIZONTAL + 1) * (MESH_VERTICAL + 1) + (MESH_HORIZONTAL + 1))			// 頂点数
#define MAX_INDEX		(MESH_HORIZONTAL * MESH_VERTICAL * 2 + (4 * (MESH_VERTICAL - 1)) + 2 + (MESH_HORIZONTAL + 2))	// インデックスの数
#define MAX_POLYGON		((MESH_HORIZONTAL * MESH_VERTICAL * 2) + (4 * (MESH_VERTICAL - 1)) + MESH_HORIZONTAL)		// ポリゴン数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshDome(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshDome = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshDome = NULL;	// インデックスへのポインタ
MeshDome g_MeshDome;
int u = MAX_POLYGON;
//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHDOME, &g_pTextureMeshDome);

	g_MeshDome.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置の初期化
	g_MeshDome.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きの初期化
	g_MeshDome.fAngle = 1.0f / (MESH_HORIZONTAL / 2);	// 角度
	g_MeshDome.fRadius = MESH_RADIUS;					// 半径

															// 頂点情報の作成
	MakeVertexMeshDome(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshDome(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMeshDome != NULL)
	{
		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}

	// インデックスバッファの開放
	if (g_pIdxBuffMeshDome != NULL)
	{
		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshDome(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				// 計算用マトリックス

												// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_MeshDome.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshDome.rot.y, g_MeshDome.rot.x, g_MeshDome.rot.z);
	D3DXMatrixMultiply(&g_MeshDome.mtxWorld, &g_MeshDome.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshDome.pos.x, g_MeshDome.pos.y, g_MeshDome.pos.z);
	D3DXMatrixMultiply(&g_MeshDome.mtxWorld, &g_MeshDome.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshDome.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshDome, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshDome);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESH_HORIZONTAL + 1, 0, MESH_HORIZONTAL);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_VERTEX - (MESH_HORIZONTAL + 1), 0, MAX_POLYGON - MESH_HORIZONTAL);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshDome(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMeshV;
	int nCntMeshH;
	int nCntIdx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshDome,
		NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshDome,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos.x = 0.0f;
	pVtx[0].pos.y = MESH_HEIGHT * 2;	// Y軸を下へずらす
	pVtx[0].pos.z = 0.0f;

	// 法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);	// 正規化する

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	// 頂点の設定
	for (nCntMeshV = 0; nCntMeshV < MESH_VERTICAL; nCntMeshV++)
	{
		for (nCntMeshH = 0; nCntMeshH < MESH_HORIZONTAL; nCntMeshH++)
		{
			// 頂点の座標
			//pVtx[1].pos.x = sinf(D3DX_PI * (0.0f + (g_MeshDome.fAngle * nCntMeshH))) * sinf(nCntMeshH * g_MeshDome.fRadius) * g_MeshDome.fRadius;		// 45度ずつずれる
			//pVtx[1].pos.y = MESH_HEIGHT - (nCntMeshV * MESH_HEIGHT);									// Y軸を下へずらす
			//pVtx[1].pos.z = cosf(D3DX_PI * (0.0f + (g_MeshDome.fAngle * nCntMeshH))) * sinf(nCntMeshH * g_MeshDome.fRadius) * g_MeshDome.fRadius;		// 45度ずつずれる

			pVtx[1].pos.x = sinf(D3DX_PI * (0.0f + (g_MeshDome.fAngle * nCntMeshH))) * g_MeshDome.fRadius;		// 45度ずつずれる
			pVtx[1].pos.y = MESH_HEIGHT - (nCntMeshV * MESH_HEIGHT);									// Y軸を下へずらす
			pVtx[1].pos.z = cosf(D3DX_PI * (0.0f + (g_MeshDome.fAngle * nCntMeshH))) * g_MeshDome.fRadius;		// 45度ずつずれる


			// 法線の設定
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);	// 正規化する

			// 頂点カラー
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[1].tex = D3DXVECTOR2(0.0f + (nCntMeshH * 1.0f), 0.0f + (nCntMeshV * 1.0f));

			pVtx += 1;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMeshDome->Unlock();

	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (nCntMeshV = 0, nCntIdx = 0; nCntMeshV < MESH_VERTICAL; nCntMeshV++)
	{// 垂直方向
		for (nCntMeshH = 0; nCntMeshH <= MESH_HORIZONTAL; nCntMeshH++, nCntIdx++)
		{// 水平方向
			if (nCntMeshV == 0)
			{// FAN部分の設定
				pIdx[0] = 1 + nCntIdx;
				pIdx[1] = 0;

				pIdx += 2;

				if (nCntMeshH % 2 == 0)
				{// Fanの折り返し
					pIdx[0] = 0;
					pIdx[1] = 1 + nCntIdx + 1;

					pIdx += 2;
				}
			}
			else
			{// STRIP部分の設定
				pIdx[0] = 1 + MESH_HORIZONTAL + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				pIdx += 2;

				if (nCntMeshV < MESH_VERTICAL - 1 && nCntMeshH == MESH_HORIZONTAL)
				{// 折り返し時
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = 1 + MESH_HORIZONTAL + nCntIdx + 1;

					pIdx += 2;
				}
			}
		}
	}
	// 頂点バッファをアンロック
	g_pIdxBuffMeshDome->Unlock();
}
