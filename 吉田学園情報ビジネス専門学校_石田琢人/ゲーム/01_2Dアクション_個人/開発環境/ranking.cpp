//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "ranking.h"
#include "score.h"
#include "bg.h"
#include "block.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RANKING0	"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RANKING1	"data/TEXTURE/ranking_logo.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RANKING2	"data/TEXTURE/ranking_logo001.png"	// 読み込むテクスチャファイル名
#define NUM_RANKING	(14)	// ランキングの数
#define MAX_RANKING	(NUM_RANKING + 2)
#define RANKING_SIZE	(50)
#define MAX_TEXTURE		(3)
#define RANKING_TIME	(600)
#define LOGO_SIZE_X		(250)
#define LOGO_SIZE_Y		(50)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRanking[MAX_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;				// 頂点バッファへのポインタ
RANKING					g_aRankScore[MAX_RANKING];				// ランキング
int g_aNumber[NUM_RANKING];
int g_nCntRanking;
int g_nRankScore = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void InitRanking(void)
{
	// 背景初期化処理
	InitBg();

	// ブロック初期化処理
	InitBlock();

	int nCntRanking;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntRanking = 0; nCntRanking < NUM_RANKING / 2; nCntRanking++)
	{
		// ランキングの初期化
		g_aRankScore[nCntRanking].nScore = 0;
		g_aRankScore[nCntRanking].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		g_aRankScore[nCntRanking].pos = D3DXVECTOR3((SCREEN_WIDTH / 4 + SCREEN_WIDTH / 2) - (nCntRanking * RANKING_SIZE * 2.0f), SCREEN_HEIGHT / 2 - 50, 0.0f);
		g_aRankScore[nCntRanking].nType = 0;
	}
	for (; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		g_aRankScore[nCntRanking].nScore = 0;
		g_aRankScore[nCntRanking].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		g_aRankScore[nCntRanking].pos = D3DXVECTOR3((SCREEN_WIDTH / 4 + SCREEN_WIDTH / 2) - ((nCntRanking % (NUM_RANKING / 2)) * RANKING_SIZE * 2.0f), SCREEN_HEIGHT / 2 + 250, 0.0f);
		g_aRankScore[nCntRanking].nType = 0;
	}

	g_aRankScore[NUM_RANKING].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5, 0.0f);
	g_aRankScore[NUM_RANKING].nType = 1;

	g_aRankScore[NUM_RANKING + 1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 75, 0.0f);
	g_aRankScore[NUM_RANKING + 1].nType = 2;

	g_nCntRanking = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING0, &g_pTextureRanking[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING1, &g_pTextureRanking[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING2, &g_pTextureRanking[2]);

	// 頂点情報の作成
	MakeVertexRanking(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRanking(void)
{
	// 背景終了処理
	UninitBg();

	// ブロック終了処理
	UninitBlock();

	int nCntRanking;

	for (nCntRanking = 0; nCntRanking < MAX_TEXTURE; nCntRanking++)
	{
		// テクスチャの開放
		if (g_pTextureRanking[nCntRanking] != NULL)
		{
			g_pTextureRanking[nCntRanking]->Release();
			g_pTextureRanking[nCntRanking] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRanking(void)
{
	// 背景更新処理
	UpdateBg();

	// ブロック更新処理
	UpdateBlock();

	FADE pFade;

	g_nCntRanking++;

	// フェードの取得
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			// モード選択
			SetFade(MODE_TITLE);
		}
		else if (g_nCntRanking >= RANKING_TIME)
		{
			StopSound(SOUND_LABEL_TITLE000);
			//モード選択
			SetFade(MODE_TITLE);
		}
	}

	if (rand() % 160 == 0)
	{// ランダムに鳥を出す
		SetBlock(D3DXVECTOR3(-50, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}
	else if (rand() % 120 == 0)
	{// ランダムに鳥を出す
		SetBlock(D3DXVECTOR3(50 + SCREEN_WIDTH, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}

	int nScore;
	nScore = GetScore();

	if (g_nRankScore < nScore)
	{// ハイスコア更新
		g_nRankScore = nScore;
	}

	int nCntRanking;

	g_aNumber[0] = g_nRankScore % 10;
	g_aNumber[1] = g_nRankScore % 100 / 10;
	g_aNumber[2] = g_nRankScore % 1000 / 100;
	g_aNumber[3] = g_nRankScore % 10000 / 1000;
	g_aNumber[4] = g_nRankScore % 100000 / 10000;
	g_aNumber[5] = g_nRankScore % 1000000 / 100000;
	g_aNumber[6] = g_nRankScore % 10000000 / 1000000;

	g_aNumber[7] = nScore % 10;
	g_aNumber[8] = nScore % 100 / 10;
	g_aNumber[9] = nScore % 1000 / 100;
	g_aNumber[10] = nScore % 10000 / 1000;
	g_aNumber[11] = nScore % 100000 / 10000;
	g_aNumber[12] = nScore % 1000000 / 100000;
	g_aNumber[13] = nScore % 10000000 / 1000000;

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntRanking] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + g_aNumber[nCntRanking] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntRanking] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + g_aNumber[nCntRanking] * 0.1f, 1.0f);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - RANKING_SIZE, g_aRankScore[nCntRanking].pos.y - RANKING_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + RANKING_SIZE, g_aRankScore[nCntRanking].pos.y - RANKING_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - RANKING_SIZE, g_aRankScore[nCntRanking].pos.y + RANKING_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + RANKING_SIZE, g_aRankScore[nCntRanking].pos.y + RANKING_SIZE, 0.0f);

		// 頂点カラー
		pVtx[0].col = g_aRankScore[nCntRanking].col;
		pVtx[1].col = g_aRankScore[nCntRanking].col;
		pVtx[2].col = g_aRankScore[nCntRanking].col;
		pVtx[3].col = g_aRankScore[nCntRanking].col;

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRanking(void)
{
	// 背景描画処理
	DrawBg();

	// ブロック描画処理
	DrawBlock();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRanking[g_aRankScore[nCntRanking].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntRanking;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - RANKING_SIZE, g_aRankScore[nCntRanking].pos.y - RANKING_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + RANKING_SIZE, g_aRankScore[nCntRanking].pos.y - RANKING_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - RANKING_SIZE, g_aRankScore[nCntRanking].pos.y + RANKING_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + RANKING_SIZE, g_aRankScore[nCntRanking].pos.y + RANKING_SIZE, 0.0f);

		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}

	for (; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - LOGO_SIZE_X, g_aRankScore[nCntRanking].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + LOGO_SIZE_X, g_aRankScore[nCntRanking].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x - LOGO_SIZE_X, g_aRankScore[nCntRanking].pos.y + LOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRankScore[nCntRanking].pos.x + LOGO_SIZE_X, g_aRankScore[nCntRanking].pos.y + LOGO_SIZE_Y, 0.0f);

		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}
