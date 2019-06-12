//=============================================================================
//
// 敵残数表示処理 [remnantsEnemy.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "remnantsEnemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_REMNANTS_00		"data/TEXTURE/number.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_REMNANTS_01		"data/TEXTURE/title_icon.png"	// 読み込むテクスチャファイル名
#define NUM_REMNANTS			(3)								// スコアの最大桁数
#define SIZE_REMNANTS			(25)							// スコアの大きさ
#define MAX_TEX_REMNANTS		(2)								// テクスチャの数
#define MAX_SIZE_REMNANTS		(40)							// 大きさの最大値
#define SIZE_LARGE_REMNANTS		(20)							// どれくらい大きくするか
#define SIZE_SMALL_REMNANTS		(0.1f)							// 差分を割る値
#define MAX_REMNANTS_POLYGON	(NUM_REMNANTS + 1)				// ポリゴン数
#define REMNANTS_POSITION		(1180)							// X座標の位置

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexRemnants(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRemnants[MAX_TEX_REMNANTS] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRemnants = NULL;				// 頂点バッファへのポインタ
Remnants				g_aRemnants[MAX_REMNANTS_POLYGON];		// 敵の残数を桁ごとに格納
int g_nNumRemnants;												// 敵の残数
int g_aNumLogRemnants[NUM_REMNANTS];							// 桁ごとの敵の残数を保存
D3DXVECTOR3 g_aDiffSizeRemnants[NUM_REMNANTS][4];				// 元のサイズ

//=============================================================================
// 初期化処理
//=============================================================================
void InitRemnants(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntScore = 0; nCntScore < NUM_REMNANTS; nCntScore++)
	{
		// スコアの初期化
		g_aRemnants[nCntScore].pos = D3DXVECTOR3(REMNANTS_POSITION - (nCntScore * 50.0f), 50, 0.0f);
		g_aRemnants[nCntScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aRemnants[nCntScore].nType = 0;

		g_aNumLogRemnants[nCntScore] = 0;
	}

	g_aRemnants[NUM_REMNANTS].pos = D3DXVECTOR3(REMNANTS_POSITION - (NUM_REMNANTS * 50.0f), 50, 0.0f);
	g_aRemnants[NUM_REMNANTS].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aRemnants[NUM_REMNANTS].nType = 1;

	g_nNumRemnants = 30;		// 敵の残数の初期化

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_REMNANTS_00, &g_pTextureRemnants[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_REMNANTS_01, &g_pTextureRemnants[1]);

	// 頂点情報の作成
	MakeVertexRemnants(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRemnants(void)
{
	for (int nCntScore = 0; nCntScore < MAX_TEX_REMNANTS; nCntScore++)
	{
		// テクスチャの開放
		if (g_pTextureRemnants[nCntScore] != NULL)
		{
			g_pTextureRemnants[nCntScore]->Release();
			g_pTextureRemnants[nCntScore] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRemnants != NULL)
	{
		g_pVtxBuffRemnants->Release();
		g_pVtxBuffRemnants = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRemnants(void)
{
	D3DXVECTOR3 aDestSize[4];	// サイズの差分を求めるのに必要
	D3DXCOLOR col[4];

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRemnants->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nNumRemnants < 10)
	{
		g_aRemnants[0].pos.x = REMNANTS_POSITION - ((NUM_REMNANTS - 1) * 50);
	}
	else if (g_nNumRemnants < 100)
	{
		g_aRemnants[0].pos.x = REMNANTS_POSITION - ((NUM_REMNANTS - 2) * 50);
		g_aRemnants[1].pos.x = REMNANTS_POSITION - ((NUM_REMNANTS - 1) * 50);
	}
	else
	{
		g_aRemnants[0].pos.x = REMNANTS_POSITION - ((NUM_REMNANTS - 3) * 50);
		g_aRemnants[1].pos.x = REMNANTS_POSITION - ((NUM_REMNANTS - 2) * 50);
	}

	for (int nCntVtx = 0; nCntVtx < (int)log10f((float)g_nNumRemnants) + 1; nCntVtx++)
	{// 現在の桁数分回す
		//頂点座標の設定
		pVtx[(nCntVtx * 4) + 0].pos = D3DXVECTOR3(g_aRemnants[nCntVtx].pos.x - SIZE_REMNANTS, g_aRemnants[nCntVtx].pos.y - SIZE_REMNANTS, 0.0f);
		pVtx[(nCntVtx * 4) + 1].pos = D3DXVECTOR3(g_aRemnants[nCntVtx].pos.x + SIZE_REMNANTS, g_aRemnants[nCntVtx].pos.y - SIZE_REMNANTS, 0.0f);
		pVtx[(nCntVtx * 4) + 2].pos = D3DXVECTOR3(g_aRemnants[nCntVtx].pos.x - SIZE_REMNANTS, g_aRemnants[nCntVtx].pos.y + SIZE_REMNANTS, 0.0f);
		pVtx[(nCntVtx * 4) + 3].pos = D3DXVECTOR3(g_aRemnants[nCntVtx].pos.x + SIZE_REMNANTS, g_aRemnants[nCntVtx].pos.y + SIZE_REMNANTS, 0.0f);

		col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (pVtx[(nCntVtx * 4) + 0].pos.x < g_aDiffSizeRemnants[nCntVtx][0].x)
		{// サイズを戻す
			aDestSize[0].x = g_aDiffSizeRemnants[nCntVtx][0].x - pVtx[(nCntVtx * 4) + 0].pos.x;
			aDestSize[0].y = g_aDiffSizeRemnants[nCntVtx][0].y - pVtx[(nCntVtx * 4) + 0].pos.y;
			aDestSize[1].x = g_aDiffSizeRemnants[nCntVtx][1].x - pVtx[(nCntVtx * 4) + 1].pos.x;
			aDestSize[1].y = g_aDiffSizeRemnants[nCntVtx][1].y - pVtx[(nCntVtx * 4) + 1].pos.y;
			aDestSize[2].x = g_aDiffSizeRemnants[nCntVtx][2].x - pVtx[(nCntVtx * 4) + 2].pos.x;
			aDestSize[2].y = g_aDiffSizeRemnants[nCntVtx][2].y - pVtx[(nCntVtx * 4) + 2].pos.y;
			aDestSize[3].x = g_aDiffSizeRemnants[nCntVtx][3].x - pVtx[(nCntVtx * 4) + 3].pos.x;
			aDestSize[3].y = g_aDiffSizeRemnants[nCntVtx][3].y - pVtx[(nCntVtx * 4) + 3].pos.y;

			//頂点座標の設定
			pVtx[(nCntVtx * 4) + 0].pos.x += aDestSize[0].x * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 0].pos.y += aDestSize[0].y * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 1].pos.x += aDestSize[1].x * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 1].pos.y += aDestSize[1].y * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 2].pos.x += aDestSize[2].x * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 2].pos.y += aDestSize[2].y * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 3].pos.x += aDestSize[3].x * SIZE_SMALL_REMNANTS;
			pVtx[(nCntVtx * 4) + 3].pos.y += aDestSize[3].y * SIZE_SMALL_REMNANTS;

			col[0] -= pVtx[(nCntVtx * 4) + 0].col;
			col[1] -= pVtx[(nCntVtx * 4) + 1].col;
			col[2] -= pVtx[(nCntVtx * 4) + 2].col;
			col[3] -= pVtx[(nCntVtx * 4) + 3].col;

			//頂点カラー
			pVtx[(nCntVtx * 4) + 0].col += col[0] * 0.02f;
			pVtx[(nCntVtx * 4) + 1].col += col[1] * 0.02f;
			pVtx[(nCntVtx * 4) + 2].col += col[2] * 0.02f;
			pVtx[(nCntVtx * 4) + 3].col += col[3] * 0.02f;
		}

		int aNumber[3];

		aNumber[0] = g_nNumRemnants % 10;				// １桁目
		aNumber[1] = g_nNumRemnants % 100 / 10;			// ２桁目
		aNumber[2] = g_nNumRemnants % 1000 / 100;		// ３桁目

		for (int nCntScore = 0; nCntScore < NUM_REMNANTS; nCntScore++)
		{
			//テクスチャ座標
			pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
			pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 0.0f);
			pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 1.0f);

			g_aNumLogRemnants[nCntScore] = aNumber[nCntScore];	// 新しいデータを保存
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRemnants->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRemnants(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRemnants, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < MAX_REMNANTS_POLYGON; nCntScore++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRemnants[g_aRemnants[nCntScore].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

//=============================================================================
// 敵残数の設定
//=============================================================================
void SetRemnants(int nRemnant)
{
	int nCntScore;
	int aNumber[NUM_REMNANTS];	// 桁ごとの敵の残数

	g_nNumRemnants = nRemnant;	// 敵の残数を更新

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRemnants->Lock(0, 0, (void**)&pVtx, 0);

	aNumber[0] = g_nNumRemnants % 10;				// １桁目
	aNumber[1] = g_nNumRemnants % 100 / 10;			// ２桁目
	aNumber[2] = g_nNumRemnants % 1000 / 100;		// ３桁目

	for (nCntScore = 0; nCntScore < NUM_REMNANTS; nCntScore++)
	{
		//テクスチャ座標
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 1.0f);

		if (g_aNumLogRemnants[nCntScore] != aNumber[nCntScore])
		{
			if ((g_aRemnants[nCntScore].pos.x - SIZE_REMNANTS) - MAX_SIZE_REMNANTS < pVtx[(nCntScore * 4) + 0].pos.x)
			{// 大きさの限度
				//頂点座標の設定
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE_REMNANTS;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE_REMNANTS;
			}
		}

		g_aNumLogRemnants[nCntScore] = aNumber[nCntScore];	// 新しいデータを保存
	}

	if (g_nNumRemnants < 10)
	{// ９以下だと2,3桁目を透明に
	 //頂点カラー
		pVtx[4].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[5].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[6].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[7].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pVtx[8].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[9].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[10].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[11].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
	}
	else if (g_nNumRemnants < 100)
	{// ９９以下だと3桁目を透明に
		pVtx[8].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[9].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[10].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[11].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRemnants->Unlock();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexRemnants(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntScore;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_REMNANTS_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRemnants,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRemnants->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_REMNANTS; nCntScore++)
	{
		//頂点座標の設定
		pVtx[(nCntScore * 4) + 0].pos = D3DXVECTOR3(g_aRemnants[nCntScore].pos.x - SIZE_REMNANTS, g_aRemnants[nCntScore].pos.y - SIZE_REMNANTS, 0.0f);
		pVtx[(nCntScore * 4) + 1].pos = D3DXVECTOR3(g_aRemnants[nCntScore].pos.x + SIZE_REMNANTS, g_aRemnants[nCntScore].pos.y - SIZE_REMNANTS, 0.0f);
		pVtx[(nCntScore * 4) + 2].pos = D3DXVECTOR3(g_aRemnants[nCntScore].pos.x - SIZE_REMNANTS, g_aRemnants[nCntScore].pos.y + SIZE_REMNANTS, 0.0f);
		pVtx[(nCntScore * 4) + 3].pos = D3DXVECTOR3(g_aRemnants[nCntScore].pos.x + SIZE_REMNANTS, g_aRemnants[nCntScore].pos.y + SIZE_REMNANTS, 0.0f);

		// サイズの記憶
		g_aDiffSizeRemnants[nCntScore][0] = pVtx[(nCntScore * 4) + 0].pos;
		g_aDiffSizeRemnants[nCntScore][1] = pVtx[(nCntScore * 4) + 1].pos;
		g_aDiffSizeRemnants[nCntScore][2] = pVtx[(nCntScore * 4) + 2].pos;
		g_aDiffSizeRemnants[nCntScore][3] = pVtx[(nCntScore * 4) + 3].pos;

		//1.0fで固定
		pVtx[(nCntScore * 4) + 0].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 1].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 2].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 3].rhw = 1.0f;

		//頂点カラー
		pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//頂点座標の設定
	pVtx[(NUM_REMNANTS * 4) + 0].pos = D3DXVECTOR3(g_aRemnants[NUM_REMNANTS].pos.x - SIZE_REMNANTS, g_aRemnants[NUM_REMNANTS].pos.y - SIZE_REMNANTS, 0.0f);
	pVtx[(NUM_REMNANTS * 4) + 1].pos = D3DXVECTOR3(g_aRemnants[NUM_REMNANTS].pos.x + SIZE_REMNANTS, g_aRemnants[NUM_REMNANTS].pos.y - SIZE_REMNANTS, 0.0f);
	pVtx[(NUM_REMNANTS * 4) + 2].pos = D3DXVECTOR3(g_aRemnants[NUM_REMNANTS].pos.x - SIZE_REMNANTS, g_aRemnants[NUM_REMNANTS].pos.y + SIZE_REMNANTS, 0.0f);
	pVtx[(NUM_REMNANTS * 4) + 3].pos = D3DXVECTOR3(g_aRemnants[NUM_REMNANTS].pos.x + SIZE_REMNANTS, g_aRemnants[NUM_REMNANTS].pos.y + SIZE_REMNANTS, 0.0f);

	//1.0fで固定
	pVtx[(NUM_REMNANTS * 4) + 0].rhw = 1.0f;
	pVtx[(NUM_REMNANTS * 4) + 1].rhw = 1.0f;
	pVtx[(NUM_REMNANTS * 4) + 2].rhw = 1.0f;
	pVtx[(NUM_REMNANTS * 4) + 3].rhw = 1.0f;

	//頂点カラー
	pVtx[(NUM_REMNANTS * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_REMNANTS * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_REMNANTS * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_REMNANTS * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[(NUM_REMNANTS * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[(NUM_REMNANTS * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[(NUM_REMNANTS * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[(NUM_REMNANTS * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffRemnants->Unlock();
}
