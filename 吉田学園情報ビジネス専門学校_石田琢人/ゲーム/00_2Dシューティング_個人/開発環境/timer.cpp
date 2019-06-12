//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "timer.h"
#include "enemy.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_Timer		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define MAX_TIMER	(2)		//タイマーの最大桁数
#define TIMER_SIZE	(25)	//タイマーの文字の大きさ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTimer = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		// 頂点バッファへのポインタ
int						g_nTimer;					// タイマー
D3DXVECTOR3 g_posTimer;								//タイマーの位置

//=============================================================================
// 初期化処理
//=============================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posTimer = D3DXVECTOR3(SCREEN_WIDTH / 2 + 25, 40, 0.0f);

	// タイマーの初期化
	g_nTimer = 6000;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_Timer,		// ファイルの名前
		&g_pTextureTimer);	// テクスチャへのポインタ

							// 頂点情報の作成
	MakeVertexTimer(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{
	// テクスチャの開放
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}

	//頂点バッファの破棄
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
	int aNumber[MAX_TIMER];

	// 頂点情報を設定
	VERTEX_2D *pVtx;
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	aNumber[0] = g_nTimer % 1000 / 100;
	aNumber[1] = g_nTimer % 10000 / 1000;

	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aNumber[nCntTimer] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aNumber[nCntTimer] * 0.1f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	g_nTimer--;

	if (g_nTimer <= 0)
	{
		SetGameState(GAMESTATE_END);
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

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTimer);

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
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

	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posTimer.x - TIMER_SIZE, g_posTimer.y - TIMER_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTimer.x + TIMER_SIZE, g_posTimer.y - TIMER_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTimer.x - TIMER_SIZE, g_posTimer.y + TIMER_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTimer.x + TIMER_SIZE, g_posTimer.y + TIMER_SIZE, 0.0f);

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
		g_posTimer.x -= 50;
	}

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