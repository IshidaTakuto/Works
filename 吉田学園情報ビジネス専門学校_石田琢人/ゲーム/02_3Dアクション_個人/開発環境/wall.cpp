//=============================================================================
//
// 壁処理 [wall.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "wall.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WALL		"data/TEXTURE/bg000.jpg"	// 読み込むテクスチャファイル名
#define MAX_WALL	(4)
#define WALL_WIDTH	(50.0f)		// 壁の幅
#define WALL_HEIGHT	(50.0f)		// 壁の高さ


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	// 頂点バッファへのポインタ
Wall g_aWall[MAX_WALL];							// 壁の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitWall(void)
{
	int nCntWall;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_WALL, &g_pTextureWall);

	// 頂点情報の作成
	MakeVertexWall(pDevice);

	// 壁の情報の初期化
	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの初期化
		g_aWall[nCntWall].fDepth = 0.0f;
		g_aWall[nCntWall].fWidth = 0.0f;
		g_aWall[nCntWall].bUse = false;
	}

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWall(void)
{
	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWall(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				// 計算用マトリックス
	int nCntWall;

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
		D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
		D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWall);

		if (g_aWall[nCntWall].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntWall, 2);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntWall;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(-WALL_WIDTH, WALL_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(WALL_WIDTH, WALL_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-WALL_WIDTH, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(WALL_WIDTH, 0.0f, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffWall->Unlock();
}

//=============================================================================
// 壁の設定処理
//=============================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntShadow;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_WALL; nCntShadow++)
	{
		if (g_aWall[nCntShadow].bUse == false)
		{
			g_aWall[nCntShadow].pos = pos;
			g_aWall[nCntShadow].rot = rot;

			// 頂点の座標
			pVtx[0].pos = D3DXVECTOR3(-WALL_WIDTH, WALL_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(WALL_WIDTH, WALL_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-WALL_WIDTH, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(WALL_WIDTH, 0.0f, 0.0f);

			g_aWall[nCntShadow].bUse = true;
			break;
		}
		pVtx += 4;	// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffWall->Unlock();
}
