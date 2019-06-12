//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "ranking.h"
#include "score.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RANKING		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RANKING1	"data/TEXTURE/high_score.png"	// 読み込むテクスチャファイル名
#define MAX_RANKING	(8)	//ランキングの最大桁数
#define RANKING_SIZE	(50)
#define MAX_TEXTURE		(2)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexHighscore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRanking[MAX_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;				// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighscore = NULL;				// 頂点バッファへのポインタ
int						g_nRankScore;							// ランキング
int g_aNumber[8];
D3DXVECTOR3 g_posRanking;

//=============================================================================
// 初期化処理
//=============================================================================
void InitRanking(void)
{
	//背景初期化処理
	InitBg();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posRanking = D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 0.0f);

	// ランキングの初期化
	g_nRankScore = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING, &g_pTextureRanking[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING1, &g_pTextureRanking[1]);

	// 頂点情報の作成
	MakeVertexRanking(pDevice);

	// 頂点情報の作成
	MakeVertexHighscore(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRanking(void)
{
	//背景終了処理
	UninitBg();

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

	//頂点バッファの破棄
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHighscore != NULL)
	{
		g_pVtxBuffHighscore->Release();
		g_pVtxBuffHighscore = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRanking(void)
{
	//背景更新処理
	UpdateBg();

	FADE pFade;

	//フェードの取得
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			//モード選択
			SetFade(MODE_TITLE);
		}
	}
	int nCntRanking;

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntRanking] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + g_aNumber[nCntRanking] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntRanking] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + g_aNumber[nCntRanking] * 0.1f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRanking(void)
{
	//背景描画処理
	DrawBg();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking[0]);

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);
	}

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHighscore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking[1]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
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

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posRanking.x - RANKING_SIZE, g_posRanking.y - RANKING_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posRanking.x + RANKING_SIZE, g_posRanking.y - RANKING_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posRanking.x - RANKING_SIZE, g_posRanking.y + RANKING_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posRanking.x + RANKING_SIZE, g_posRanking.y + RANKING_SIZE, 0.0f);

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
		g_posRanking.x += 80;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexHighscore(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighscore,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHighscore->Lock(0, 0, (void**)&pVtx, 0);

	////頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffHighscore->Unlock();
}

//=============================================================================
// ランキングの設定
//=============================================================================
void SetRankScore(int nScore)
{
	if (nScore > g_nRankScore)
	{// ハイスコア更新
		g_nRankScore = nScore;

		g_aNumber[0] = g_nRankScore / 10000000;
		g_aNumber[1] = g_nRankScore % 10000000 / 1000000;
		g_aNumber[2] = g_nRankScore % 1000000 / 100000;
		g_aNumber[3] = g_nRankScore % 100000 / 10000;
		g_aNumber[4] = g_nRankScore % 10000 / 1000;
		g_aNumber[5] = g_nRankScore % 1000 / 100;
		g_aNumber[6] = g_nRankScore % 100 / 10;
		g_aNumber[7] = g_nRankScore % 10;

	}
}
