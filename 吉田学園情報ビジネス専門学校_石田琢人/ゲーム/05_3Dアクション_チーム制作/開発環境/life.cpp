//=============================================================================
//
// 体力処理 [life.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "life.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BAR_WIDTH_LIFE		(500)
#define BAR_HEIGHT_LIFE		(60)
#define NUM_TEXTURE_LIFE	(1)
#define NUM_POLYGON_LIFE	(4)
#define SUBTRACT_TIME		(25)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureLife[NUM_TEXTURE_LIFE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;						// 頂点バッファへのポインタ
Life g_Life[NUM_POLYGON_LIFE];		// ライフの構造体
int g_nCntSubtraction;				// バーを減らし始めるカウンタ

//=============================================================================
// 初期化処理
//=============================================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 枠
	g_Life[0].pos = D3DXVECTOR3(20.0f, 630.0f, 0.0f);
	g_Life[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	g_Life[0].nLife = MAX_LIFE;				// ライフを最大にする
	g_Life[0].fLength = BAR_WIDTH_LIFE;		// バーの長さを設定
	g_Life[0].nType = -1;

	// バー
	for (int nCntLife = 1; nCntLife < NUM_POLYGON_LIFE; nCntLife++)
	{
		g_Life[nCntLife].pos = D3DXVECTOR3(20.0f, 630.0f, 0.0f);
		g_Life[nCntLife].nLife = MAX_LIFE;				// ライフを最大にする
		g_Life[nCntLife].fLength = BAR_WIDTH_LIFE;		// バーの長さを設定
		g_Life[nCntLife].nType = -1;
	}

	g_Life[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_Life[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	g_Life[3].col = D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f);

	// バーを減らし始めるカウンタ
	g_nCntSubtraction = 0;

	// 頂点情報の作成
	MakeVertexLife(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife(void)
{
	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < NUM_TEXTURE_LIFE; nCntTexture++)
	{
		if (g_pTextureLife[nCntTexture] != NULL)
		{
			g_pTextureLife[nCntTexture]->Release();
			g_pTextureLife[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife(void)
{
	/*	g_Life[3].fLength == 現在のライフ					（最前面に来るバー）			*/
	/*	g_Life[2].fLength == ダメージを受けて減った分のバー	（最前面から一つ後ろに来るバー）*/

	if (g_Life[2].fLength > g_Life[3].fLength)
	{// ライフと同じ値になるまで後ろのゲージを減らす
		g_nCntSubtraction++;								// ダメージバーを減らし始めるためのカウント

		if (g_nCntSubtraction >= SUBTRACT_TIME)
		{// 一定時間経過
			g_Life[2].fLength -= BAR_WIDTH_LIFE * 0.01f;	// ダメージバーの長さを1％ずつ減らす

			VERTEX_2D *pVtx;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[(2 * 4) + 1].pos = D3DXVECTOR3(g_Life[2].pos.x + g_Life[2].fLength, g_Life[2].pos.y, 0.0f);
			pVtx[(2 * 4) + 3].pos = D3DXVECTOR3(g_Life[2].pos.x + g_Life[2].fLength, g_Life[2].pos.y + BAR_HEIGHT_LIFE, 0.0f);

			// 頂点バッファをアンロックする
			g_pVtxBuffLife->Unlock();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLife = 0; nCntLife < NUM_POLYGON_LIFE; nCntLife++)
	{
		// テクスチャの設定
		if (g_Life[nCntLife].nType >= 0)
		{// テクスチャの指定
			pDevice->SetTexture(0, g_pTextureLife[g_Life[nCntLife].nType]);
		}
		else
		{// 0より低いとテクスチャを張らない
			pDevice->SetTexture(0, NULL);

		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntLife, 2);
	}
}

//=============================================================================
// 体力の増減
//=============================================================================
void AddSubtractLife(int nValue)
{
	g_Life[3].nLife += nValue;		// ライフを増減させる
	float fLife;

	if (g_Life[3].nLife > 0)
	{// ライフが0より大きいとき
		fLife = (BAR_WIDTH_LIFE * 1.0f) / ((MAX_LIFE * 1.0f) / g_Life[3].nLife);	// バーの元の長さを100%としてライフを増減させる
	}
	else
	{// ライフが0以下のとき
		fLife = 0;
	}

	if (g_Life[3].nLife >= MAX_LIFE)
	{// 最大値を超えたら戻す
		g_Life[3].nLife = MAX_LIFE;
		fLife = BAR_WIDTH_LIFE;
	}

	g_Life[3].fLength = fLife;	// 現在のライフバーの長さ

	g_nCntSubtraction = 0;		// ダメージバーの長さを減らし始めるためのカウンタを初期化

	// 頂点情報の設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	if (nValue > 0)
	{// 回復したとき後ろのバーも増やす
		g_Life[2].fLength = fLife;

		//頂点座標の設定
		pVtx[(2 * 4) + 1].pos = D3DXVECTOR3(g_Life[2].pos.x + g_Life[2].fLength, g_Life[2].pos.y, 0.0f);
		pVtx[(2 * 4) + 3].pos = D3DXVECTOR3(g_Life[2].pos.x + g_Life[2].fLength, g_Life[2].pos.y + BAR_HEIGHT_LIFE, 0.0f);
	}

	// 現在のライフ
	pVtx[(3 * 4) + 1].pos = D3DXVECTOR3(g_Life[3].pos.x + g_Life[3].fLength, g_Life[3].pos.y, 0.0f);
	pVtx[(3 * 4) + 3].pos = D3DXVECTOR3(g_Life[3].pos.x + g_Life[3].fLength, g_Life[3].pos.y + BAR_HEIGHT_LIFE, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// 現在のライフの取得
//=============================================================================
int GetLife(void)
{
	return g_Life[3].nLife;
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntLife;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_POLYGON_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntLife = 0; nCntLife < NUM_POLYGON_LIFE; nCntLife++)
	{
		//頂点座標の設定
		pVtx[(nCntLife * 4) + 0].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x,					g_Life[nCntLife].pos.y, 0.0f);
		pVtx[(nCntLife * 4) + 1].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x + BAR_WIDTH_LIFE,	g_Life[nCntLife].pos.y, 0.0f);
		pVtx[(nCntLife * 4) + 2].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x,					g_Life[nCntLife].pos.y + BAR_HEIGHT_LIFE, 0.0f);
		pVtx[(nCntLife * 4) + 3].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x + BAR_WIDTH_LIFE,	g_Life[nCntLife].pos.y + BAR_HEIGHT_LIFE, 0.0f);

		//1.0fで固定
		pVtx[(nCntLife * 4) + 0].rhw = 1.0f;
		pVtx[(nCntLife * 4) + 1].rhw = 1.0f;
		pVtx[(nCntLife * 4) + 2].rhw = 1.0f;
		pVtx[(nCntLife * 4) + 3].rhw = 1.0f;

		//頂点カラー
		pVtx[(nCntLife * 4) + 0].col = g_Life[nCntLife].col;
		pVtx[(nCntLife * 4) + 1].col = g_Life[nCntLife].col;
		pVtx[(nCntLife * 4) + 2].col = g_Life[nCntLife].col;
		pVtx[(nCntLife * 4) + 3].col = g_Life[nCntLife].col;

		//テクスチャ座標
		pVtx[(nCntLife * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntLife * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntLife * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntLife * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}
