//=============================================================================
//
// 残機処理 [stock.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "stock.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_STOCK0		"data/TEXTURE/stock.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_STOCK1		"data/TEXTURE/frame_stock.png"	// 読み込むテクスチャファイル名
#define STOCK_SIZE	(25)	// 残機の大きさ
#define MAX_STOCK	(NUM_STOCK + 1)
#define MAX_TEX		(2)
#define FRAME_SIZE_X	(200)
#define FRAME_SIZE_Y	(150)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexStock(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureStock[MAX_TEX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStock = NULL;		// 頂点バッファへのポインタ
STOCK					g_Stock[NUM_STOCK];			// ストック

//=============================================================================
// 初期化処理
//=============================================================================
void InitStock(void)
{
	int nCntStock;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntStock = 0; nCntStock < NUM_STOCK; nCntStock++)
	{// 残機の初期化
		g_Stock[nCntStock].pos = D3DXVECTOR3(100 + (50.0f * nCntStock), 65, 0.0f);	// 残機の位置
		g_Stock[nCntStock].nCntStock = NUM_STOCK;
		g_Stock[nCntStock].nType = 0;
		g_Stock[nCntStock].bUse = true;
	}
	g_Stock[NUM_STOCK].pos = D3DXVECTOR3(150, 90, 0.0f);	// 残機の位置
	g_Stock[NUM_STOCK].nType = 1;
	g_Stock[NUM_STOCK].bUse = true;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_STOCK0, &g_pTextureStock[0]);	// テクスチャへのポインタ
	D3DXCreateTextureFromFile(pDevice, TEXTURE_STOCK1, &g_pTextureStock[1]);	// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexStock(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStock(void)
{
	for (int nCntStock = 0; nCntStock < MAX_TEX; nCntStock++)
	{
		// テクスチャの開放
		if (g_pTextureStock[nCntStock] != NULL)
		{
			g_pTextureStock[nCntStock]->Release();
			g_pTextureStock[nCntStock] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffStock != NULL)
	{
		g_pVtxBuffStock->Release();
		g_pVtxBuffStock = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStock(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{
		if (g_Stock[nCntStock].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureStock[g_Stock[nCntStock].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntStock, 2);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexStock(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntStock;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_STOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStock,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStock = 0; nCntStock < NUM_STOCK; nCntStock++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x - STOCK_SIZE, g_Stock[nCntStock].pos.y - STOCK_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x + STOCK_SIZE, g_Stock[nCntStock].pos.y - STOCK_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x - STOCK_SIZE, g_Stock[nCntStock].pos.y + STOCK_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x + STOCK_SIZE, g_Stock[nCntStock].pos.y + STOCK_SIZE, 0.0f);

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

		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x - FRAME_SIZE_X, g_Stock[nCntStock].pos.y - FRAME_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x + FRAME_SIZE_X, g_Stock[nCntStock].pos.y - FRAME_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x - FRAME_SIZE_X, g_Stock[nCntStock].pos.y + FRAME_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Stock[nCntStock].pos.x + FRAME_SIZE_X, g_Stock[nCntStock].pos.y + FRAME_SIZE_Y, 0.0f);

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
	g_pVtxBuffStock->Unlock();
}

//=============================================================================
// 残機の減算
//=============================================================================
void SubtractionStock(int nValue)
{
	if (g_Stock[nValue - 1].bUse == true)
	{
		g_Stock[nValue - 1].bUse = false;
	}
}

//=============================================================================
// 残機の取得
//=============================================================================
STOCK *GetStock(void)
{
	return &g_Stock[0];
}