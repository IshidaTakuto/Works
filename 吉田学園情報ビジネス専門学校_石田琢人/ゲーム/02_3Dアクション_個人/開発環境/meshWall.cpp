//=============================================================================
//
// メッシュウォール処理 [meshWall.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "meshWall.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MESHWALL00		"data/TEXTURE/sakura_wall.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHWALL01		"data/TEXTURE/summer_wall.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHWALL02		"data/TEXTURE/fall_wall.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHWALL03		"data/TEXTURE/winter_wall.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHWALL04		"data/TEXTURE/sky.jpg"			// 読み込むテクスチャファイル名

#define MAX_TEXTURE		(5)

#define SPLITX			(5)
#define SPLITY			(1)

#define MAX_VERTEX		((SPLITX + 1) * (SPLITY + 1))			// 頂点数
#define MAX_INDEX		(SPLITX * SPLITY * 2 + (4 * (SPLITY - 1)) + 2)	// インデックスの数
#define MAX_POLYGON		(SPLITX * SPLITY * 2) + (4 * (SPLITY - 1))		// ポリゴン数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice, int nIndex);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshWall[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshWall = NULL;	// インデックスへのポインタ
MeshWall g_aMeshWall[MAX_WALL];

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHWALL00, &g_pTextureMeshWall[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHWALL01, &g_pTextureMeshWall[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHWALL02, &g_pTextureMeshWall[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHWALL03, &g_pTextureMeshWall[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHWALL04, &g_pTextureMeshWall[4]);

	int nCntWall;

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{// モデルの情報を初期化
		if (g_aMeshWall[nCntWall].bUse == false)
		{
			g_aMeshWall[nCntWall].pos = pos;
			g_aMeshWall[nCntWall].rot = rot;
			g_aMeshWall[nCntWall].fWidth = fWidth;
			g_aMeshWall[nCntWall].fHeight = fHeight;
			g_aMeshWall[nCntWall].nType = nType;
			g_aMeshWall[nCntWall].fTexPos = 1.0f;
			g_aMeshWall[nCntWall].bUse = true;
			break;
		}
	}

	// 頂点情報の作成
	MakeVertexMeshWall(pDevice, nCntWall);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshWall(void)
{
	for (int nCntWall = 0; nCntWall < MAX_TEXTURE; nCntWall++)
	{
		//テクスチャの破棄
		if (g_pTextureMeshWall[nCntWall] != NULL)
		{
			g_pTextureMeshWall[nCntWall]->Release();
			g_pTextureMeshWall[nCntWall] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	// インデックスバッファの開放
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshWall(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				// 計算用マトリックス
	int nCntMeshWall;

	for (nCntMeshWall = 0; nCntMeshWall < MAX_WALL; nCntMeshWall++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMeshWall[nCntMeshWall].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntMeshWall].rot.y, g_aMeshWall[nCntMeshWall].rot.x, g_aMeshWall[nCntMeshWall].rot.z);
		D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntMeshWall].pos.x, g_aMeshWall[nCntMeshWall].pos.y, g_aMeshWall[nCntMeshWall].pos.z);
		D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntMeshWall].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffMeshWall);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshWall[g_aMeshWall[nCntMeshWall].nType]);

		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_VERTEX, 0, MAX_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice, int nIndex)
{
	int nCntMeshY;
	int nCntMeshX;
	int nCntIdx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshWall,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshY = 0; nCntMeshY <= SPLITY; nCntMeshY++)
	{
		for (nCntMeshX = 0; nCntMeshX <= SPLITX; nCntMeshX++)
		{
			// 頂点の座標
			pVtx[0].pos = D3DXVECTOR3((-g_aMeshWall[nIndex].fWidth / 2) + ((g_aMeshWall[nIndex].fWidth * nCntMeshX) / SPLITX), g_aMeshWall[nIndex].fHeight - ((g_aMeshWall[nIndex].fHeight * nCntMeshY) / SPLITY), 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshY * 1.0f);

			pVtx += 1;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMeshWall->Unlock();

	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshY = 0, nCntIdx = 0; nCntMeshY < SPLITY; nCntMeshY++)
	{
		for (nCntMeshX = 0; nCntMeshX <= SPLITX; nCntMeshX++, nCntIdx++)
		{
			pIdx[0] = (SPLITX + 1) + nCntIdx;
			pIdx[1] = 0 + nCntIdx;

			pIdx += 2;

			if (nCntMeshY < SPLITY - 1 && nCntMeshX == SPLITX)
			{// 折り返し時
				pIdx[0] = 0 + nCntIdx;
				pIdx[1] = (SPLITX + 1) + nCntIdx + 1;

				pIdx += 2;
			}
		}
	}

	// インデックスバッファをアンロック
	g_pIdxBuffMeshWall->Unlock();
}
