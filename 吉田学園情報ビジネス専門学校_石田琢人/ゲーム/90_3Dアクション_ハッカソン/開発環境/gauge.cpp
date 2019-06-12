//=============================================================================
//
// 変身ゲージ処理 [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "gauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BAR_WIDTH_GAUGE		(500)		// バーの幅
#define BAR_HEIGHT_GAUGE	(30)		// バーの高さ
#define NUM_TEXTURE_GAUGE	(1)			// バーのテクスチャ数
#define NUM_POLYGON_GAUGE	(2)			// ポリゴン数
#define GAUGE_SPEED			(2)			// ゲージの増減の速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexGauge(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureGauge[NUM_TEXTURE_GAUGE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;						// 頂点バッファへのポインタ
Gauge g_Gauge[NUM_POLYGON_GAUGE];		// ゲージの構造体
int g_nGauge;				// 現在のゲージの値
bool g_bGaugeColor;

//=============================================================================
// 初期化処理
//=============================================================================
void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// [0]バーの元の長さ　[1]現在の値を示す
	for (int nCntGauge = 0; nCntGauge < NUM_POLYGON_GAUGE; nCntGauge++)
	{
		g_Gauge[nCntGauge].pos = D3DXVECTOR3(20.0f, 650.0f, 0.0f);
		g_Gauge[nCntGauge].nType = -1;
	}

	// バーの長さを設定
	g_Gauge[0].fLength = BAR_WIDTH_GAUGE;
	g_Gauge[1].fLength = BAR_WIDTH_GAUGE;

	g_Gauge[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_Gauge[1].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);

	// ゲージの値
	g_nGauge = MAX_GAUGE;
	g_bGaugeColor = false;

	// 頂点情報の作成
	MakeVertexGauge(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGauge(void)
{
	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < NUM_TEXTURE_GAUGE; nCntTexture++)
	{
		if (g_pTextureGauge[nCntTexture] != NULL)
		{
			g_pTextureGauge[nCntTexture]->Release();
			g_pTextureGauge[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGauge(void)
{
	float fDestLength = (BAR_WIDTH_GAUGE * 1.0f) / ((MAX_GAUGE * 1.0f) / g_nGauge);		// バーの長さを現在の値から求める

	if (g_Gauge[1].fLength + 0.01f < fDestLength)
	{// バーの長さ　＜　目的の長さ
		g_Gauge[1].fLength += GAUGE_SPEED;
	}
	else if (g_Gauge[1].fLength - 0.01f > fDestLength)
	{// バーの長さ　＞　目的の長さ
		g_Gauge[1].fLength -= GAUGE_SPEED;
	}

	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Gauge[1].fLength >= BAR_WIDTH_GAUGE)
	{// ゲージが最大値で点滅
		if (!g_bGaugeColor)
		{// 色を加算していく
			g_Gauge[1].col += D3DXCOLOR(0.02f, 0.02f, 0.02f, 0.0f);

			if (g_Gauge[1].col.b >= 1.0f)
			{// 一定値まで加算した
				g_Gauge[1].col = D3DXCOLOR(2.0f, 1.7f, 1.0f, 1.0f);
				g_bGaugeColor = true;
			}
		}
		else
		{// 色を減算していく
			g_Gauge[1].col -= D3DXCOLOR(0.02f, 0.02f, 0.02f, 0.0f);

			if (g_Gauge[1].col.b <= 0.0f)
			{// 一定値まで減算した
				g_Gauge[1].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
				g_bGaugeColor = false;
			}
		}

		//頂点カラー
		pVtx[4 + 0].col = g_Gauge[1].col;
		pVtx[4 + 1].col = g_Gauge[1].col;
		pVtx[4 + 2].col = g_Gauge[1].col;
		pVtx[4 + 3].col = g_Gauge[1].col;
	}
	else
	{
		g_Gauge[1].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);

		//頂点カラー
		pVtx[4 + 0].col = g_Gauge[1].col;
		pVtx[4 + 1].col = g_Gauge[1].col;
		pVtx[4 + 2].col = g_Gauge[1].col;
		pVtx[4 + 3].col = g_Gauge[1].col;
	}

	//頂点座標の設定
	pVtx[4 + 1].pos = D3DXVECTOR3(g_Gauge[1].pos.x + g_Gauge[1].fLength, g_Gauge[1].pos.y, 0.0f);
	pVtx[4 + 3].pos = D3DXVECTOR3(g_Gauge[1].pos.x + g_Gauge[1].fLength, g_Gauge[1].pos.y + BAR_HEIGHT_GAUGE, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGauge(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < NUM_POLYGON_GAUGE; nCntGauge++)
	{
		// テクスチャの設定
		if (g_Gauge[nCntGauge].nType >= 0)
		{// テクスチャの指定
			pDevice->SetTexture(0, g_pTextureGauge[g_Gauge[nCntGauge].nType]);
		}
		else
		{// 0より低いとテクスチャを張らない
			pDevice->SetTexture(0, NULL);

		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGauge, 2);
	}
}

//=============================================================================
// ゲージの増減
//=============================================================================
void AddSubtractGauge(int nValue)
{
	g_nGauge += nValue;		// ゲージを増減させる

	if (g_nGauge >= MAX_GAUGE)
	{// 最大値を超えたら戻す
		g_nGauge = MAX_GAUGE;
	}
	else if (g_nGauge <= 0)
	{// 最小値を超えたら戻す
		g_nGauge = 0;
	}
}

//=============================================================================
// 現在のゲージの取得
//=============================================================================
int GetGauge(void)
{
	return g_nGauge;
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexGauge(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntGauge;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_POLYGON_GAUGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGauge = 0; nCntGauge < NUM_POLYGON_GAUGE; nCntGauge++)
	{
		//頂点座標の設定
		pVtx[(nCntGauge * 4) + 0].pos = D3DXVECTOR3(g_Gauge[nCntGauge].pos.x, g_Gauge[nCntGauge].pos.y, 0.0f);
		pVtx[(nCntGauge * 4) + 1].pos = D3DXVECTOR3(g_Gauge[nCntGauge].pos.x + BAR_WIDTH_GAUGE, g_Gauge[nCntGauge].pos.y, 0.0f);
		pVtx[(nCntGauge * 4) + 2].pos = D3DXVECTOR3(g_Gauge[nCntGauge].pos.x, g_Gauge[nCntGauge].pos.y + BAR_HEIGHT_GAUGE, 0.0f);
		pVtx[(nCntGauge * 4) + 3].pos = D3DXVECTOR3(g_Gauge[nCntGauge].pos.x + BAR_WIDTH_GAUGE, g_Gauge[nCntGauge].pos.y + BAR_HEIGHT_GAUGE, 0.0f);

		//1.0fで固定
		pVtx[(nCntGauge * 4) + 0].rhw = 1.0f;
		pVtx[(nCntGauge * 4) + 1].rhw = 1.0f;
		pVtx[(nCntGauge * 4) + 2].rhw = 1.0f;
		pVtx[(nCntGauge * 4) + 3].rhw = 1.0f;

		//頂点カラー
		pVtx[(nCntGauge * 4) + 0].col = g_Gauge[nCntGauge].col;
		pVtx[(nCntGauge * 4) + 1].col = g_Gauge[nCntGauge].col;
		pVtx[(nCntGauge * 4) + 2].col = g_Gauge[nCntGauge].col;
		pVtx[(nCntGauge * 4) + 3].col = g_Gauge[nCntGauge].col;

		//テクスチャ座標
		pVtx[(nCntGauge * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntGauge * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntGauge * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntGauge * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}
