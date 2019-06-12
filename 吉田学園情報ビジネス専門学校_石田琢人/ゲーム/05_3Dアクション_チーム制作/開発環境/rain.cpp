//=============================================================================
//
// 雨処理 [rain.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "rain.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RAIN		"data/TEXTURE/rain01.png"	// 読み込むテクスチャファイル名
#define MAX_RAIN		(2)		// 雨の最大数
#define RAIN_SPEED		(2.0f)	// 雨の速さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRain = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRain = NULL;	// 頂点バッファへのポインタ
int g_nCntRain;

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitRain(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	g_nCntRain = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RAIN, &g_pTextureRain);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_RAIN,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRain,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRain->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//1.0fで固定
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
		pVtx[1].tex = D3DXVECTOR2(1.0f + (nCntRain * 1.5f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f + (nCntRain * 1.5f));
		pVtx[3].tex = D3DXVECTOR2(1.0f + (nCntRain * 1.5f), 1.0f + (nCntRain * 1.5f));

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffRain->Unlock();
}

//=============================================================================
// ポリゴン終了処理
//=============================================================================
void UninitRain(void)
{
	// テクスチャの破棄
	if (g_pTextureRain != NULL)
	{
		g_pTextureRain->Release();
		g_pTextureRain = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRain != NULL)
	{
		g_pVtxBuffRain->Release();
		g_pVtxBuffRain = NULL;
	}
}

//=============================================================================
// ポリゴン更新処理
//=============================================================================
void UpdateRain(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRain->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		// テクスチャ座標
		pVtx[0].tex -= D3DXVECTOR2(0.0f, 0.015f);
		pVtx[1].tex -= D3DXVECTOR2(0.0f, 0.015f);
		pVtx[2].tex -= D3DXVECTOR2(0.0f, 0.015f);
		pVtx[3].tex -= D3DXVECTOR2(0.0f, 0.015f);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffRain->Unlock();
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawRain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRain, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRain);

	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRain, 2);
	}

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}
