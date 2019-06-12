//=============================================================================
//
// キルログ処理 [killLog.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "killLog.h"
#include "particle2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_KILLLOG_00	"data/TEXTURE/number.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_KILLLOG_01	"data/TEXTURE/game_kill.png"	// 読み込むテクスチャファイル名
#define NUM_KILLLOG			(3)								// スコアの最大桁数
#define SIZE_KILLLOG		(25)							// スコアの大きさ
#define MAX_TEX				(2)								// テクスチャの数
#define MAX_SIZE			(40)							// 大きさの最大値
#define SIZE_LARGE			(20)							// どれくらい大きくするか
#define SIZE_SMALL			(0.1f)							// 差分を割る値
#define MAX_KILLLOG_POLYGON	(NUM_KILLLOG + 1)
#define KILLLOG_POSITION	(275)
#define KILLING_VOID_SIZE	(50.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexKillLog(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureKillLog[MAX_TEX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffKillLog = NULL;				// 頂点バッファへのポインタ
KILLLOG					g_aKillLog[MAX_KILLLOG_POLYGON];		// キル数を桁ごとに格納
int g_nNumKill;													// キル数
int g_aNumLog[NUM_KILLLOG];										// 桁ごとのキル数を保存
D3DXVECTOR3 g_aDiffSize[NUM_KILLLOG][4];						// 元のサイズ

//=============================================================================
// 初期化処理
//=============================================================================
void InitKillLog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntScore = 0; nCntScore < NUM_KILLLOG; nCntScore++)
	{
		// スコアの初期化
		g_aKillLog[nCntScore].pos = D3DXVECTOR3(KILLLOG_POSITION - (nCntScore * KILLING_VOID_SIZE), 50, 0.0f);
		g_aKillLog[nCntScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aKillLog[nCntScore].nType = 0;

		g_aNumLog[nCntScore] = 0;
	}

	// スコアの初期化
	g_aKillLog[NUM_KILLLOG].pos = D3DXVECTOR3(KILLLOG_POSITION - (NUM_KILLLOG * KILLING_VOID_SIZE) - 20.0f, 50, 0.0f);
	g_aKillLog[NUM_KILLLOG].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aKillLog[NUM_KILLLOG].nType = 1;

	g_nNumKill = 0;		// キル数の初期化

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_KILLLOG_00, &g_pTextureKillLog[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_KILLLOG_01, &g_pTextureKillLog[1]);

	// 頂点情報の作成
	MakeVertexKillLog(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitKillLog(void)
{
	for (int nCntScore = 0; nCntScore < MAX_TEX; nCntScore++)
	{
		// テクスチャの開放
		if (g_pTextureKillLog[nCntScore] != NULL)
		{
			g_pTextureKillLog[nCntScore]->Release();
			g_pTextureKillLog[nCntScore] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffKillLog != NULL)
	{
		g_pVtxBuffKillLog->Release();
		g_pVtxBuffKillLog = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateKillLog(void)
{
	D3DXVECTOR3 aDestSize[4];	// サイズの差分を求めるのに必要
	D3DXCOLOR col[4];

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffKillLog->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nNumKill < 10)
	{
		g_aKillLog[0].pos.x = KILLLOG_POSITION - ((NUM_KILLLOG - 1) * KILLING_VOID_SIZE);
	}
	else if (g_nNumKill < 100)
	{
		g_aKillLog[0].pos.x = KILLLOG_POSITION - ((NUM_KILLLOG - 2) * KILLING_VOID_SIZE);
		g_aKillLog[1].pos.x = KILLLOG_POSITION - ((NUM_KILLLOG - 1) * KILLING_VOID_SIZE);
	}
	else
	{
		g_aKillLog[0].pos.x = KILLLOG_POSITION - ((NUM_KILLLOG - 3) * KILLING_VOID_SIZE);
		g_aKillLog[1].pos.x = KILLLOG_POSITION - ((NUM_KILLLOG - 2) * KILLING_VOID_SIZE);
	}

	for (int nCntVtx = 0; nCntVtx < NUM_KILLLOG; nCntVtx++)
	{
		//頂点座標の設定
		pVtx[(nCntVtx * 4) + 0].pos = D3DXVECTOR3(g_aKillLog[nCntVtx].pos.x - SIZE_KILLLOG, g_aKillLog[nCntVtx].pos.y - SIZE_KILLLOG, 0.0f);
		pVtx[(nCntVtx * 4) + 1].pos = D3DXVECTOR3(g_aKillLog[nCntVtx].pos.x + SIZE_KILLLOG, g_aKillLog[nCntVtx].pos.y - SIZE_KILLLOG, 0.0f);
		pVtx[(nCntVtx * 4) + 2].pos = D3DXVECTOR3(g_aKillLog[nCntVtx].pos.x - SIZE_KILLLOG, g_aKillLog[nCntVtx].pos.y + SIZE_KILLLOG, 0.0f);
		pVtx[(nCntVtx * 4) + 3].pos = D3DXVECTOR3(g_aKillLog[nCntVtx].pos.x + SIZE_KILLLOG, g_aKillLog[nCntVtx].pos.y + SIZE_KILLLOG, 0.0f);

		col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (pVtx[(nCntVtx * 4) + 0].pos.x < g_aDiffSize[nCntVtx][0].x)
		{// サイズを戻す
			aDestSize[0].x = g_aDiffSize[nCntVtx][0].x - pVtx[(nCntVtx * 4) + 0].pos.x;
			aDestSize[0].y = g_aDiffSize[nCntVtx][0].y - pVtx[(nCntVtx * 4) + 0].pos.y;
			aDestSize[1].x = g_aDiffSize[nCntVtx][1].x - pVtx[(nCntVtx * 4) + 1].pos.x;
			aDestSize[1].y = g_aDiffSize[nCntVtx][1].y - pVtx[(nCntVtx * 4) + 1].pos.y;
			aDestSize[2].x = g_aDiffSize[nCntVtx][2].x - pVtx[(nCntVtx * 4) + 2].pos.x;
			aDestSize[2].y = g_aDiffSize[nCntVtx][2].y - pVtx[(nCntVtx * 4) + 2].pos.y;
			aDestSize[3].x = g_aDiffSize[nCntVtx][3].x - pVtx[(nCntVtx * 4) + 3].pos.x;
			aDestSize[3].y = g_aDiffSize[nCntVtx][3].y - pVtx[(nCntVtx * 4) + 3].pos.y;

			//頂点座標の設定
			pVtx[(nCntVtx * 4) + 0].pos.x += aDestSize[0].x * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 0].pos.y += aDestSize[0].y * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 1].pos.x += aDestSize[1].x * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 1].pos.y += aDestSize[1].y * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 2].pos.x += aDestSize[2].x * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 2].pos.y += aDestSize[2].y * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 3].pos.x += aDestSize[3].x * SIZE_SMALL;
			pVtx[(nCntVtx * 4) + 3].pos.y += aDestSize[3].y * SIZE_SMALL;

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
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffKillLog->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawKillLog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffKillLog, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < MAX_KILLLOG_POLYGON; nCntScore++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureKillLog[g_aKillLog[nCntScore].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

//=============================================================================
// スコアの加算
//=============================================================================
void AddKill(void)
{
	/***********************************************************
	*
	*	敵を倒したときに呼んでください
	*
	***********************************************************/

	int nCntScore;
	int aNumber[NUM_KILLLOG];	// 桁ごとのキル数

	g_nNumKill++;	// キル数を増やす

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffKillLog->Lock(0, 0, (void**)&pVtx, 0);

	aNumber[0] = g_nNumKill % 10;				// １桁目
	aNumber[1] = g_nNumKill % 100 / 10;			// ２桁目
	aNumber[2] = g_nNumKill % 1000 / 100;		// ３桁目

	for (nCntScore = 0; nCntScore < NUM_KILLLOG; nCntScore++)
	{
		//テクスチャ座標
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 1.0f);

		if (g_nNumKill < 100)
		{// 100より小さいと黄色
			//頂点カラー
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{// 100以上でカラフル
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(rand() % 11 * 0.1f, rand() % 11 * 0.1f, rand() % 11 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(rand() % 11 * 0.1f, rand() % 11 * 0.1f, rand() % 11 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(rand() % 11 * 0.1f, rand() % 11 * 0.1f, rand() % 11 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(rand() % 11 * 0.1f, rand() % 11 * 0.1f, rand() % 11 * 0.1f, 1.0f);

			if ((g_aKillLog[nCntScore].pos.x - SIZE_KILLLOG) - (MAX_SIZE * 1.1f) < pVtx[(nCntScore * 4) + 0].pos.x)
			{// 大きさの限度
			 //頂点座標の設定
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE * 1.1f;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE * 1.1f;
			}
		}

		if (g_aNumLog[nCntScore] != aNumber[nCntScore])
		{
			if ((g_aKillLog[nCntScore].pos.x - SIZE_KILLLOG) - MAX_SIZE < pVtx[(nCntScore * 4) + 0].pos.x)
			{// 大きさの限度
				//頂点座標の設定
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE;
			}
		}

		g_aNumLog[nCntScore] = aNumber[nCntScore];	// 新しいデータを保存
	}

	if (g_nNumKill < 10)
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
	else if (g_nNumKill < 100)
	{// ９９以下だと3桁目を透明に
		pVtx[8].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[9].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[10].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[11].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffKillLog->Unlock();

	// パーティクルをつける
	if (g_nNumKill < 10)
	{// ９以下
		SetParticle2D(g_aKillLog[0].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
	}
	else if (g_nNumKill < 100)
	{// ９９以下
		SetParticle2D(g_aKillLog[0].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
		SetParticle2D(g_aKillLog[1].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
	}
	else
	{
		SetParticle2D(g_aKillLog[0].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
		SetParticle2D(g_aKillLog[1].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
		SetParticle2D(g_aKillLog[2].pos, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), 30, 10, PARTICLETYPE_NORMAL);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexKillLog(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntScore;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_KILLLOG_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffKillLog,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffKillLog->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_KILLLOG; nCntScore++)
	{
		//頂点座標の設定
		pVtx[(nCntScore * 4) + 0].pos = D3DXVECTOR3(g_aKillLog[nCntScore].pos.x - SIZE_KILLLOG, g_aKillLog[nCntScore].pos.y - SIZE_KILLLOG, 0.0f);
		pVtx[(nCntScore * 4) + 1].pos = D3DXVECTOR3(g_aKillLog[nCntScore].pos.x + SIZE_KILLLOG, g_aKillLog[nCntScore].pos.y - SIZE_KILLLOG, 0.0f);
		pVtx[(nCntScore * 4) + 2].pos = D3DXVECTOR3(g_aKillLog[nCntScore].pos.x - SIZE_KILLLOG, g_aKillLog[nCntScore].pos.y + SIZE_KILLLOG, 0.0f);
		pVtx[(nCntScore * 4) + 3].pos = D3DXVECTOR3(g_aKillLog[nCntScore].pos.x + SIZE_KILLLOG, g_aKillLog[nCntScore].pos.y + SIZE_KILLLOG, 0.0f);

		// サイズの記憶
		g_aDiffSize[nCntScore][0] = pVtx[(nCntScore * 4) + 0].pos;
		g_aDiffSize[nCntScore][1] = pVtx[(nCntScore * 4) + 1].pos;
		g_aDiffSize[nCntScore][2] = pVtx[(nCntScore * 4) + 2].pos;
		g_aDiffSize[nCntScore][3] = pVtx[(nCntScore * 4) + 3].pos;

		//1.0fで固定
		pVtx[(nCntScore * 4) + 0].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 1].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 2].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 3].rhw = 1.0f;

		if (nCntScore == 0)
		{// １桁目は最初から出す
			//頂点カラー
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{// ２桁目以降は隠しておく
			//頂点カラー
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}

		//テクスチャ座標
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//頂点座標の設定
	pVtx[(NUM_KILLLOG * 4) + 0].pos = D3DXVECTOR3(g_aKillLog[NUM_KILLLOG].pos.x - SIZE_KILLLOG, g_aKillLog[NUM_KILLLOG].pos.y - SIZE_KILLLOG, 0.0f);
	pVtx[(NUM_KILLLOG * 4) + 1].pos = D3DXVECTOR3(g_aKillLog[NUM_KILLLOG].pos.x + SIZE_KILLLOG, g_aKillLog[NUM_KILLLOG].pos.y - SIZE_KILLLOG, 0.0f);
	pVtx[(NUM_KILLLOG * 4) + 2].pos = D3DXVECTOR3(g_aKillLog[NUM_KILLLOG].pos.x - SIZE_KILLLOG, g_aKillLog[NUM_KILLLOG].pos.y + SIZE_KILLLOG, 0.0f);
	pVtx[(NUM_KILLLOG * 4) + 3].pos = D3DXVECTOR3(g_aKillLog[NUM_KILLLOG].pos.x + SIZE_KILLLOG, g_aKillLog[NUM_KILLLOG].pos.y + SIZE_KILLLOG, 0.0f);

	//1.0fで固定
	pVtx[(NUM_KILLLOG * 4) + 0].rhw = 1.0f;
	pVtx[(NUM_KILLLOG * 4) + 1].rhw = 1.0f;
	pVtx[(NUM_KILLLOG * 4) + 2].rhw = 1.0f;
	pVtx[(NUM_KILLLOG * 4) + 3].rhw = 1.0f;

	//頂点カラー
	pVtx[(NUM_KILLLOG * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_KILLLOG * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_KILLLOG * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_KILLLOG * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[(NUM_KILLLOG * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[(NUM_KILLLOG * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[(NUM_KILLLOG * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[(NUM_KILLLOG * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffKillLog->Unlock();
}
