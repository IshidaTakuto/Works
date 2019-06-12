//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "timer.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TIMER0		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TIMER1		"data/TEXTURE/frame_timer.png"	// 読み込むテクスチャファイル名
#define NUM_TIMER	(3)		// タイマーの最大桁数
#define TIMER_SIZE	(25)	// タイマーの文字の大きさ
#define MAX_TEX		(2)
#define MAX_TIMER	(NUM_TIMER + 1)
#define FRAME_SIZE_X	(200)	// フレームの大きさ
#define FRAME_SIZE_Y	(150)	// フレームの大きさ

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
		g_aTimer[nCntTimer].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f + (nCntTimer * 50.0f), 80, 0.0f);
		g_aTimer[nCntTimer].nType = 0;
	}

	g_aTimer[NUM_TIMER].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 100, 0.0f);
	g_aTimer[NUM_TIMER].nType = 1;

	g_nTimer = 60;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,TEXTURE_TIMER0,&g_pTextureTimer[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TIMER1, &g_pTextureTimer[1]);

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
	int nCntTimer;
	int aNumber[NUM_TIMER];

	// 頂点情報を設定
	VERTEX_2D *pVtx;
		
	g_nCntSecond++;

	if (g_nCntSecond % 60 == 0 && g_nTimer > 0)
	{// 時間を減らす
		g_nTimer--;	// カウントダウン
	}

	if (g_nTimer <= 0 && g_nCntSecond % 60 == 0)
	{// タイムオーバー
		SetGameState(GAMESTATE_END);
	}

	aNumber[0] = g_nTimer % 1000 / 100;;
	aNumber[1] = g_nTimer % 100 / 10;
	aNumber[2] = g_nTimer % 10;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < NUM_TIMER; nCntTimer++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aNumber[nCntTimer] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aNumber[nCntTimer] * 0.1f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを4つ進める
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


	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTimer[g_aTimer[nCntTimer].nType]);

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
		pVtx[0].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - TIMER_SIZE, g_aTimer[nCntTimer].pos.y - TIMER_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + TIMER_SIZE, g_aTimer[nCntTimer].pos.y - TIMER_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - TIMER_SIZE, g_aTimer[nCntTimer].pos.y + TIMER_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + TIMER_SIZE, g_aTimer[nCntTimer].pos.y + TIMER_SIZE, 0.0f);

		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x - FRAME_SIZE_X, g_aTimer[NUM_TIMER].pos.y - FRAME_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x + FRAME_SIZE_X, g_aTimer[NUM_TIMER].pos.y - FRAME_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x - FRAME_SIZE_X, g_aTimer[NUM_TIMER].pos.y + FRAME_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTimer[NUM_TIMER].pos.x + FRAME_SIZE_X, g_aTimer[NUM_TIMER].pos.y + FRAME_SIZE_Y, 0.0f);

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

	// 頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}

//=============================================================================
// 残機の取得
//=============================================================================
int *GetTimer(void)
{
	return &g_nTimer;
}