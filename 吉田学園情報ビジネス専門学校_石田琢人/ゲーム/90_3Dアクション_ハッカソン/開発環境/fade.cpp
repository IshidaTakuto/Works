//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// フェード処理 [fade.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_FADE		""					// フェードのテクスチャ名
#define FADE_POS_X			(0)					// フェードの左上X座標
#define FADE_POS_Y			(0)					// フェードの左上Y座標			
#define FADE_WIDTH			(SCREEN_WIDTH)		// フェードの幅
#define FADE_HEIGHT			(SCREEN_HEIGHT)		// フェードの高さ
#define FADE_SPEED			(0.01f)				// フェードスピード	
#define TITLE_FADE_SLOW		(3)					// フェードスピードを遅くする

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);		// 頂点情報の設定

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// 頂点バッファへのポインタ
FADE					g_fade;					// フェード状態
MODE					g_modeNext;				// 次のモード
D3DXCOLOR				g_colorFade;			// フェード色

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitFade(MODE modeNext)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 値の初期化
	g_fade = FADE_IN;			// フェードイン状態
	g_modeNext = modeNext;		
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// 暗転

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FADE, &g_pTextureFade);

	// 頂点情報の設定
	MakeVertexFade(pDevice);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitFade(void)
{
	// テクスチャの開放
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdateFade(void)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{//　フェードイン
			g_colorFade.a -= FADE_SPEED;		// 画面を透明にしていく

			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;		// 何もしていない状態にする
			}
		}
		else if (g_fade == FADE_OUT)
		{// フェードアウト
			// 現在のモードを取得
			MODE Mode = GetMode();

			g_colorFade.a += FADE_SPEED;		// 画面を暗転していく

			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;		// フェードイン状態にする

				// モード設定
				SetMode(g_modeNext);
			}
		}

		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラー
		for (int nCol = 0; nCol < 4; nCol++)
		{
			pVtx[nCol].col = g_colorFade;
		}

		// 頂点バッファをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawFade(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 頂点情報の作成
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		// 確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,			// 頂点フォーマット
								D3DPOOL_MANAGED,
								&g_pVtxBuffFade,
								NULL);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	// 頂点情報
	pVtx[0].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_POS_X + FADE_WIDTH, FADE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y + FADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_POS_X + FADE_WIDTH, FADE_POS_Y + FADE_HEIGHT, 0.0f);
	// rhw
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
	}
	// 頂点カラー
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;
	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// フェードの設定
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;										// フェードアウト状態
	g_modeNext = modeNext;									// 次のモード
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// 黒い画面(透明)
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// フェードの取得
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
FADE GetFade(void)
{
	return g_fade;
}

