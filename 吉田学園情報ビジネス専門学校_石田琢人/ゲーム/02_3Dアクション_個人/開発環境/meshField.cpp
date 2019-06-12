//=============================================================================
//
// メッシュフィールド処理 [meshField.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "meshField.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MESHFIELD00		"data/TEXTURE/sakura_field.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD01		"data/TEXTURE/weed_field.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD02		"data/TEXTURE/momiji_field.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD03		"data/TEXTURE/snow_field.jpg"	// 読み込むテクスチャファイル名

#define MESH_X			(6)			// x軸方向のマス数
#define MESH_Z			(3)			// z軸方向のマス数
#define MESH_WIDTH		(2500.0f)		// 幅
#define MESH_DEPTH		(400.0f)		// 奥行
#define MAX_VERTEX		((MESH_X + 1) * (MESH_Z + 1))						// 頂点数
#define MAX_INDEX		(MESH_X * MESH_Z * 2 + (4 * (MESH_Z - 1)) + 2)		// インデックスの数
#define MAX_POLYGON		((MESH_X * MESH_Z * 2) + (4 * (MESH_Z - 1)))		// ポリゴン数

#define MAX_FIELD		(4)
#define MAX_TEXTURE		(4)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;	// インデックスへのポインタ
MeshField g_MeshField[MAX_FIELD];

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD00, &g_pTextureMeshField[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD01, &g_pTextureMeshField[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD02, &g_pTextureMeshField[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD03, &g_pTextureMeshField[3]);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_MeshField[nCntField].pos = D3DXVECTOR3(MESH_WIDTH * nCntField, 0.0f, 0.0f);	// 位置の初期化
		g_MeshField[nCntField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの初期化
		g_MeshField[nCntField].nType = nCntField;
	}

	// 頂点情報の作成
	MakeVertexMeshField(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureMeshField[nCntTex] != NULL)
		{
			g_pTextureMeshField[nCntTex]->Release();
			g_pTextureMeshField[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	// インデックスバッファの開放
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				// 計算用マトリックス

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_MeshField[nCntField].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField[nCntField].rot.y, g_MeshField[nCntField].rot.x, g_MeshField[nCntField].rot.z);
		D3DXMatrixMultiply(&g_MeshField[nCntField].mtxWorld, &g_MeshField[nCntField].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_MeshField[nCntField].pos.x, g_MeshField[nCntField].pos.y, g_MeshField[nCntField].pos.z);
		D3DXMatrixMultiply(&g_MeshField[nCntField].mtxWorld, &g_MeshField[nCntField].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_MeshField[nCntField].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffMeshField);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshField[g_MeshField[nCntField].nType]);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_VERTEX, 0, MAX_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMeshZ;
	int nCntMeshX;
	int nCntIdx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshZ = 0; nCntMeshZ <= MESH_Z; nCntMeshZ++)
	{
		for (nCntMeshX = 0; nCntMeshX <= MESH_X; nCntMeshX++)
		{
			// 頂点の座標
			pVtx[0].pos = D3DXVECTOR3((MESH_WIDTH / 2) - ((MESH_WIDTH * nCntMeshX) / MESH_X), 0.0f, (-MESH_DEPTH / 2) + ((MESH_DEPTH * nCntMeshZ) / MESH_Z));
			
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshZ * 1.0f);

			pVtx += 1;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMeshField->Unlock();

	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshZ = 0, nCntIdx = 0; nCntMeshZ < MESH_Z; nCntMeshZ++)
	{
		for (nCntMeshX = 0; nCntMeshX <= MESH_X; nCntMeshX++, nCntIdx++)
		{
			pIdx[0] = (MESH_X + 1) + nCntIdx;
			pIdx[1] = 0 + nCntIdx;

			pIdx += 2;

			if (nCntMeshZ < MESH_Z - 1 && nCntMeshX == MESH_X)
			{// 折り返し時
				pIdx[0] = 0 + nCntIdx;
				pIdx[1] = (MESH_X + 1) + nCntIdx + 1;

				pIdx += 2;
			}
		}
	}

	// 頂点バッファをアンロック
	g_pIdxBuffMeshField->Unlock();
}

//=============================================================================
// 当たり判定の処理
//=============================================================================
bool CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (pPos->y < g_MeshField[nCntField].pos.y && pPosOld->y >= g_MeshField[nCntField].pos.y)
		{// 上から当たった時
			bLand = true;
			pPos->y = g_MeshField[nCntField].pos.y;
			pMove->y = 0.0f;
		}
	}

	return bLand;
}