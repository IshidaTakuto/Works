//=============================================================================
//
// スコア処理 [score.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE0		"data/TEXTURE/number.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_SCORE1		"data/TEXTURE/score_icon.png"	// 読み込むテクスチャファイル名
#define NUM_SCORE			(7)								// スコアの最大桁数
#define SCORE_SIZE			(25)							// スコアの大きさ
#define MAX_SIZE_SCORE		(40)							// 大きさの最大値
#define SIZE_LARGE_SCORE	(20)							// どれくらい大きくするか
#define SIZE_SMALL_SCORE	(0.1f)							// 差分を割る値
#define MAX_TEX				(2)
#define MAX_SCORE			(NUM_SCORE + 1)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureScore[MAX_TEX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// 頂点バッファへのポインタ
SCORE					g_aScore[MAX_SCORE];					// スコア
int g_nScore;
D3DXVECTOR3 g_aDiffSizeScore[NUM_SCORE][4];						// 元のサイズ

//=============================================================================
// 初期化処理
//=============================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		// スコアの初期化
		g_aScore[nCntScore].pos = D3DXVECTOR3(730 - (nCntScore * 50.0f), 50, 0.0f);
		g_aScore[nCntScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aScore[nCntScore].nSize = SCORE_SIZE;
		g_aScore[nCntScore].nType = 0;
	}

	g_aScore[NUM_SCORE].pos = D3DXVECTOR3(710 - (NUM_SCORE * 50.0f), 50, 0.0f);
	g_aScore[NUM_SCORE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aScore[NUM_SCORE].nSize = SCORE_SIZE;
	g_aScore[NUM_SCORE].nType = 1;

	g_nScore = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SCORE0, &g_pTextureScore[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SCORE1, &g_pTextureScore[1]);

	// 頂点情報の作成
	MakeVertexScore(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	for (int nCntScore = 0; nCntScore < MAX_TEX; nCntScore++)
	{
		// テクスチャの開放
		if (g_pTextureScore[nCntScore] != NULL)
		{
			g_pTextureScore[nCntScore]->Release();
			g_pTextureScore[nCntScore] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	D3DXVECTOR3 aDestSize[4];	// サイズの差分を求めるのに必要

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		if (pVtx[(nCntScore * 4) + 0].pos.x < g_aDiffSizeScore[nCntScore][0].x)
		{// サイズを戻す
			aDestSize[0].x = g_aDiffSizeScore[nCntScore][0].x - pVtx[(nCntScore * 4) + 0].pos.x;
			aDestSize[0].y = g_aDiffSizeScore[nCntScore][0].y - pVtx[(nCntScore * 4) + 0].pos.y;
			aDestSize[1].x = g_aDiffSizeScore[nCntScore][1].x - pVtx[(nCntScore * 4) + 1].pos.x;
			aDestSize[1].y = g_aDiffSizeScore[nCntScore][1].y - pVtx[(nCntScore * 4) + 1].pos.y;
			aDestSize[2].x = g_aDiffSizeScore[nCntScore][2].x - pVtx[(nCntScore * 4) + 2].pos.x;
			aDestSize[2].y = g_aDiffSizeScore[nCntScore][2].y - pVtx[(nCntScore * 4) + 2].pos.y;
			aDestSize[3].x = g_aDiffSizeScore[nCntScore][3].x - pVtx[(nCntScore * 4) + 3].pos.x;
			aDestSize[3].y = g_aDiffSizeScore[nCntScore][3].y - pVtx[(nCntScore * 4) + 3].pos.y;

			//頂点座標の設定
			pVtx[(nCntScore * 4) + 0].pos.x += aDestSize[0].x * SIZE_SMALL_SCORE;
			pVtx[(nCntScore * 4) + 0].pos.y += aDestSize[0].y * SIZE_SMALL_SCORE;
			pVtx[(nCntScore * 4) + 1].pos.x += aDestSize[1].x * SIZE_SMALL_SCORE;
			pVtx[(nCntScore * 4) + 1].pos.y += aDestSize[1].y * SIZE_SMALL_SCORE;
			pVtx[(nCntScore * 4) + 2].pos.x += aDestSize[2].x * SIZE_SMALL_SCORE;
			pVtx[(nCntScore * 4) + 2].pos.y += aDestSize[2].y * SIZE_SMALL_SCORE;
			pVtx[(nCntScore * 4) + 3].pos.x += aDestSize[3].x * SIZE_SMALL_SCORE;
			pVtx[(nCntScore * 4) + 3].pos.y += aDestSize[3].y * SIZE_SMALL_SCORE;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore[g_aScore[nCntScore].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntScore;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x - SCORE_SIZE, g_aScore[nCntScore].pos.y - SCORE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x + SCORE_SIZE, g_aScore[nCntScore].pos.y - SCORE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x - SCORE_SIZE, g_aScore[nCntScore].pos.y + SCORE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x + SCORE_SIZE, g_aScore[nCntScore].pos.y + SCORE_SIZE, 0.0f);

		// サイズの記憶
		g_aDiffSizeScore[nCntScore][0] = pVtx[0].pos;
		g_aDiffSizeScore[nCntScore][1] = pVtx[1].pos;
		g_aDiffSizeScore[nCntScore][2] = pVtx[2].pos;
		g_aDiffSizeScore[nCntScore][3] = pVtx[3].pos;

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aScore[NUM_SCORE].pos.x - g_aScore[NUM_SCORE].nSize, g_aScore[NUM_SCORE].pos.y - g_aScore[NUM_SCORE].nSize, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aScore[NUM_SCORE].pos.x + g_aScore[NUM_SCORE].nSize, g_aScore[NUM_SCORE].pos.y - g_aScore[NUM_SCORE].nSize, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aScore[NUM_SCORE].pos.x - g_aScore[NUM_SCORE].nSize, g_aScore[NUM_SCORE].pos.y + g_aScore[NUM_SCORE].nSize, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aScore[NUM_SCORE].pos.x + g_aScore[NUM_SCORE].nSize, g_aScore[NUM_SCORE].pos.y + g_aScore[NUM_SCORE].nSize, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// スコアの加算
//=============================================================================
void AddScore(int nValue)
{
	if (nValue != 0)
	{// 加算されるスコアが0以外
		int nCntScore;
		int aNumber[NUM_SCORE];

		g_nScore += nValue;

		// 頂点情報を設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

		aNumber[0] = g_nScore % 10;
		aNumber[1] = g_nScore % 100 / 10;
		aNumber[2] = g_nScore % 1000 / 100;
		aNumber[3] = g_nScore % 10000 / 1000;
		aNumber[4] = g_nScore % 100000 / 10000;
		aNumber[5] = g_nScore % 1000000 / 100000;
		aNumber[6] = g_nScore % 10000000 / 1000000;

		for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
		{
			//テクスチャ座標
			pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
			pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 0.0f);
			pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 1.0f);

			// 数字を大きくする
			if ((g_aScore[nCntScore].pos.x - SCORE_SIZE) - (MAX_SIZE_SCORE * 1.1f) < pVtx[(nCntScore * 4) + 0].pos.x)
			{// 大きさの限度
				//頂点座標の設定
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE_SCORE * 1.1f;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE_SCORE * 1.1f;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE_SCORE * 1.1f;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE_SCORE * 1.1f;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE_SCORE * 1.1f;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE_SCORE * 1.1f;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE_SCORE * 1.1f;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE_SCORE * 1.1f;
			}
		}

		// 頂点バッファをアンロックする
		g_pVtxBuffScore->Unlock();
	}
}

//=============================================================================
// スコアの取得
//=============================================================================
int GetScore(void)
{
	return g_nScore;
}