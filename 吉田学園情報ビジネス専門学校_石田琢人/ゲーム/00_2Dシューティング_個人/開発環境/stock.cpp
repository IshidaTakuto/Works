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
#define	TEXTURE_STOCK		"data/TEXTURE/player000.png"	// 読み込むテクスチャファイル名
#define MAX_STOCK	(3)		//体力の最大桁数
#define STOCK_SIZE	(25)	//体力の大きさ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexStock(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureStock = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStock = NULL;		// 頂点バッファへのポインタ
STOCK					g_Stock[MAX_STOCK];			// ストック

//=============================================================================
// 初期化処理
//=============================================================================
void InitStock(void)
{
	int nCntStock;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{// 体力の初期化
		g_Stock[nCntStock].pos = D3DXVECTOR3(40, 80, 0.0f);	// 体力の位置
		g_Stock[nCntStock].nCntStock = MAX_STOCK;
		g_Stock[nCntStock].bUse = true;
	}


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_STOCK,		// ファイルの名前
		&g_pTextureStock);	// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexStock(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStock(void)
{
	// テクスチャの開放
	if (g_pTextureStock != NULL)
	{
		g_pTextureStock->Release();
		g_pTextureStock = NULL;
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

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureStock);

	for (int nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{
		if (g_Stock[nCntStock].bUse == true)
		{
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
	int nCntStock2;

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

	for (nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
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
		for (nCntStock2 = 0; nCntStock2 < MAX_STOCK; nCntStock2++)
		{
			g_Stock[nCntStock2].pos.x += 50;
		}
	}

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