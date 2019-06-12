//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "timer.h"
#include "game.h"
#include "fade.h"
#include "load.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TIMER_00	"data/TEXTURE/number.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TIMER_01	"data/TEXTURE/icon_timer.png"	// 読み込むテクスチャファイル名
#define NUM_TIMER	(3)		// タイマーの最大桁数
#define TIMER_SIZE	(25)	// タイマーの文字の大きさ
#define MAX_TEX		(2)
#define MAX_TIMER	(NUM_TIMER + 2)
#define FRAME_SIZE_X	(120)	// フレームの大きさ
#define FRAME_SIZE_Y	(100)	// フレームの大きさ
#define TIMER_TIME		(105)	// 時間
#define TIMER_POSITION	(935.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTimer[MAX_TEX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		// 頂点バッファへのポインタ
TIMER g_aTimer[MAX_TIMER];		// タイマー
int g_nTimer;		// 制限時間
int g_nCntSecond;	// 秒のカウント

//=============================================================================
// 初期化処理
//=============================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTimer = 0; nCntTimer < NUM_TIMER; nCntTimer++)
	{
		g_aTimer[nCntTimer].pos = D3DXVECTOR3(TIMER_POSITION - (nCntTimer * TIMER_SIZE * 2.0f), 50.0f, 0.0f);
		g_aTimer[nCntTimer].nType = 0;
	}

	g_aTimer[NUM_TIMER].pos = D3DXVECTOR3(940 - (NUM_TIMER * TIMER_SIZE * 2), 50, 0.0f);
	g_aTimer[NUM_TIMER].nType = 1;

	g_aTimer[NUM_TIMER + 1].pos = D3DXVECTOR3(770 - TIMER_SIZE + (NUM_TIMER * TIMER_SIZE * 2.0f), 100, 0.0f);
	g_aTimer[NUM_TIMER + 1].nType = -1;

	g_nTimer = TIMER_TIME;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TIMER_00, &g_pTextureTimer[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TIMER_01, &g_pTextureTimer[1]);

	// 頂点情報の作成
	MakeVertexTimer(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{
	for (int nCntTimer = 0; nCntTimer < MAX_TEX; nCntTimer++)
	{
		// テクスチャの開放
		if (g_pTextureTimer[nCntTimer] != NULL)
		{
			g_pTextureTimer[nCntTimer]->Release();
			g_pTextureTimer[nCntTimer] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimer(void)
{
	bool bSwap = GetStageSwat();
	bool bFade = GetFadeNow();
	LOADFADE loadFade = GetLoadFade();

	// フェードとクリアメッセージのときは動けなくする
	if (false == bSwap && false == bFade && LOADFADE_NONE == loadFade)
	{
		g_nCntSecond++;

		if (g_nCntSecond % 60 == 0 && g_nTimer > 0)
		{// 時間を減らす
			g_nTimer--;	// カウントダウン
		}

		if (g_nTimer <= 0 && g_nCntSecond % 180 == 0)
		{// タイムオーバー
			SetGameState(GAMESTATE_END);
		}
	}

		int aNumber[NUM_TIMER];

		aNumber[0] = g_nTimer % 10 / 1;
		aNumber[1] = g_nTimer % 100 / 10;
		aNumber[2] = g_nTimer % 1000 / 100;

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// 桁数
	int nDigit = (int)log10f((float)g_nTimer) + 1;
	if (nDigit <= 0) { nDigit = 1; }

	for (int nCntTimer = 0; nCntTimer < nDigit; nCntTimer++)
	{
		//テクスチャ座標
		pVtx[(nCntTimer * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[(nCntTimer * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[(nCntTimer * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntTimer] * 0.1f, 1.0f);
		pVtx[(nCntTimer * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntTimer] * 0.1f, 1.0f);

		// 頂点カラー
		pVtx[(nCntTimer * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		// 桁数によって位置を変える
		if (g_nTimer < 10)
		{// 一桁
			g_aTimer[0].pos.x = TIMER_POSITION - ((NUM_TIMER - 1) * 50);

			// 二桁目を透明にする
			pVtx[(1 * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(1 * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(1 * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(1 * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		else if (g_nTimer < 100)
		{// 二桁
			g_aTimer[0].pos.x = TIMER_POSITION - ((NUM_TIMER - 2) * 50);
			g_aTimer[1].pos.x = TIMER_POSITION - ((NUM_TIMER - 1) * 50);

			// 三桁目を透明にする
			pVtx[(nDigit * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nDigit * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nDigit * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[(nDigit * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		else
		{// 三桁
			g_aTimer[0].pos.x = TIMER_POSITION - ((NUM_TIMER - 3) * 50);
			g_aTimer[1].pos.x = TIMER_POSITION - ((NUM_TIMER - 2) * 50);
		}

		//頂点座標の設定
		pVtx[(nCntTimer * 4) + 0].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - TIMER_SIZE, g_aTimer[nCntTimer].pos.y - TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 1].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + TIMER_SIZE, g_aTimer[nCntTimer].pos.y - TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 2].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - TIMER_SIZE, g_aTimer[nCntTimer].pos.y + TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 3].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + TIMER_SIZE, g_aTimer[nCntTimer].pos.y + TIMER_SIZE, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTimer = MAX_TIMER - 1; nCntTimer >= 0; nCntTimer--)
	{
		// テクスチャの設定
		if (g_aTimer[nCntTimer].nType >= 0)
		{// テクスチャ番号通りにつける
			pDevice->SetTexture(0, g_pTextureTimer[g_aTimer[nCntTimer].nType]);
		}
		else
		{// nTypeがマイナスの値はテクスチャなし
			pDevice->SetTexture(0, NULL);
		}
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTimer, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntTimer;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < NUM_TIMER; nCntTimer++)
	{
		// 頂点座標の設定
		pVtx[(nCntTimer * 4) + 0].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - TIMER_SIZE, g_aTimer[nCntTimer].pos.y - TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 1].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + TIMER_SIZE, g_aTimer[nCntTimer].pos.y - TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 2].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - TIMER_SIZE, g_aTimer[nCntTimer].pos.y + TIMER_SIZE, 0.0f);
		pVtx[(nCntTimer * 4) + 3].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + TIMER_SIZE, g_aTimer[nCntTimer].pos.y + TIMER_SIZE, 0.0f);

		// 1.0fで固定
		pVtx[(nCntTimer * 4) + 0].rhw = 1.0f;
		pVtx[(nCntTimer * 4) + 1].rhw = 1.0f;
		pVtx[(nCntTimer * 4) + 2].rhw = 1.0f;
		pVtx[(nCntTimer * 4) + 3].rhw = 1.0f;

		// 頂点カラー
		pVtx[(nCntTimer * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標
		pVtx[(nCntTimer * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntTimer * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntTimer * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntTimer * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//----------------------------------------------------------------
	/*							アイコン							*/
	//----------------------------------------------------------------
	// 頂点座標の設定
	pVtx[(NUM_TIMER * 4) + 0].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x - TIMER_SIZE, g_aTimer[NUM_TIMER].pos.y - TIMER_SIZE, 0.0f);
	pVtx[(NUM_TIMER * 4) + 1].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x + TIMER_SIZE, g_aTimer[NUM_TIMER].pos.y - TIMER_SIZE, 0.0f);
	pVtx[(NUM_TIMER * 4) + 2].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x - TIMER_SIZE, g_aTimer[NUM_TIMER].pos.y + TIMER_SIZE, 0.0f);
	pVtx[(NUM_TIMER * 4) + 3].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x + TIMER_SIZE, g_aTimer[NUM_TIMER].pos.y + TIMER_SIZE, 0.0f);

	// 1.0fで固定
	pVtx[(NUM_TIMER * 4) + 0].rhw = 1.0f;
	pVtx[(NUM_TIMER * 4) + 1].rhw = 1.0f;
	pVtx[(NUM_TIMER * 4) + 2].rhw = 1.0f;
	pVtx[(NUM_TIMER * 4) + 3].rhw = 1.0f;

	// 頂点カラー
	pVtx[(NUM_TIMER * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_TIMER * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_TIMER * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_TIMER * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[(NUM_TIMER * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[(NUM_TIMER * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[(NUM_TIMER * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[(NUM_TIMER * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//----------------------------------------------------------------
	/*							  黒い帯							*/
	//----------------------------------------------------------------
	// 頂点座標の設定
	pVtx[((NUM_TIMER + 1) * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 2].pos = D3DXVECTOR3(0.0f, FRAME_SIZE_Y, 0.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 3].pos = D3DXVECTOR3(SCREEN_WIDTH, FRAME_SIZE_Y, 0.0f);

	// 1.0fで固定
	pVtx[((NUM_TIMER + 1) * 4) + 0].rhw = 1.0f;
	pVtx[((NUM_TIMER + 1) * 4) + 1].rhw = 1.0f;
	pVtx[((NUM_TIMER + 1) * 4) + 2].rhw = 1.0f;
	pVtx[((NUM_TIMER + 1) * 4) + 3].rhw = 1.0f;

	// 頂点カラー
	pVtx[((NUM_TIMER + 1) * 4) + 0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[((NUM_TIMER + 1) * 4) + 1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[((NUM_TIMER + 1) * 4) + 2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[((NUM_TIMER + 1) * 4) + 3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

	// テクスチャ座標
	pVtx[((NUM_TIMER + 1) * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[((NUM_TIMER + 1) * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}

//=============================================================================
// タイムの取得
//=============================================================================
int *GetTimer(void)
{
	return &g_nTimer;
}

//=============================================================================
// 制限時間を戻す
//=============================================================================
void ResetTimer(void)
{
	g_nTimer = TIMER_TIME;
}