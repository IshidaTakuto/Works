//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_TEXTURENAME0	"data/TEXTURE/bg000.jpg"	// 背景のテクスチャ名
#define BG_TEXTURENAME1	"data/TEXTURE/river000.png"	// 背景のテクスチャ名
#define BG_POS_X	(0)				// 背景の左上X座標
#define BG_POS_Y	(0)				// 背景の左上Y座標
#define BG_WIDTH	(SCREEN_WIDTH)	// 背景の幅
#define BG_HEIGHT	(SCREEN_HEIGHT)	// 背景の高さ
#define TEX_UPDATE	(g_Bg.nCountAnim * 0.0001f)
#define MAX_BG		(2)		// 背景の数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBg[MAX_BG] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;	// 頂点バッファへのポインタ
BG g_aBg[MAX_BG];

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景の情報の初期化
	g_aBg[0].pos = D3DXVECTOR3(BG_WIDTH / 2, BG_HEIGHT / 2, 0.0f);	// 背景の位置
	g_aBg[1].pos = D3DXVECTOR3(BG_WIDTH / 2, BG_HEIGHT / 4 - BG_HEIGHT, 0.0f);	// 背景の位置

	g_aBg[0].nCountAnim = 0;
	g_aBg[1].nCountAnim = 0;

	g_aBg[0].nType = 0;
	g_aBg[1].nType = 1;


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME0, &g_pTextureBg[0]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME1, &g_pTextureBg[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG, 
								D3DUSAGE_WRITEONLY, 
								FVF_VERTEX_2D, 
								D3DPOOL_MANAGED, 
								&g_pVtxBuffBg, 
								NULL);

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, BG_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);

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

	// 頂点の座標
	pVtx[4].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT / 2, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT / 2, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);

	// 1.0fで固定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラー
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// ポリゴン終了処理
//=============================================================================
void UninitBg(void)
{
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		// テクスチャの破棄
		if (g_pTextureBg[nCntBg] != NULL)
		{
			g_pTextureBg[nCntBg]->Release();
			g_pTextureBg[nCntBg] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//=============================================================================
// ポリゴン更新処理
//=============================================================================
void UpdateBg(void)
{
	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	g_aBg[0].nCountAnim++;
	g_aBg[1].nCountAnim++;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2(0.0002f * g_aBg[0].nCountAnim + 0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0002f * g_aBg[0].nCountAnim + 1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0002f * g_aBg[0].nCountAnim + 0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0002f * g_aBg[0].nCountAnim + 1.0f, 1.0f);

	// テクスチャ座標を更新
	pVtx[4].tex = D3DXVECTOR2(0.0005f * g_aBg[1].nCountAnim - 0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.0005f * g_aBg[1].nCountAnim - 1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0005f * g_aBg[1].nCountAnim - 0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.0005f * g_aBg[1].nCountAnim - 1.0f, 1.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);	

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBg[g_aBg[nCntBg].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBg, 2);
	}
}
