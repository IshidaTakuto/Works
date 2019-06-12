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
#define BG_TEXTURENAME0	"data/TEXTURE/bg100.png"	//背景のテクスチャ名1
#define BG_TEXTURENAME1	"data/TEXTURE/bg101.png"	//背景のテクスチャ名2
#define BG_TEXTURENAME2	"data/TEXTURE/bg102.png"	//背景のテクスチャ名3
#define BG_POS_X	(0)				//背景の左上X座標
#define BG_POS_Y	(0)				//背景の左上Y座標
#define BG_WIDTH	(SCREEN_WIDTH)	//背景の幅
#define BG_HEIGHT	(SCREEN_HEIGHT)	//背景の高さ
#define TEX_Y_UPDATE	(g_Bg.nCountAnim * 0.005f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBg = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;	//頂点バッファへのポインタ
BG g_Bg;

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//背景の情報の初期化
	g_Bg.pos = D3DXVECTOR3(BG_WIDTH / 2, BG_HEIGHT / 2, 0.0f);	//背景の位置
	g_Bg.nCountAnim = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME0, &g_pTextureBg);
	//D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME1, &g_apTextureBg[1]);
	//D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME2, &g_apTextureBg[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, 
								D3DUSAGE_WRITEONLY, 
								FVF_VERTEX_2D, 
								D3DPOOL_MANAGED, 
								&g_pVtxBuffBg, 
								NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, BG_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// ポリゴン終了処理
//=============================================================================
void UninitBg(void)
{
	//テクスチャの破棄
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}

	//頂点バッファの破棄
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
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	g_Bg.nCountAnim++;

	//テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f - TEX_Y_UPDATE);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f - TEX_Y_UPDATE);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f - TEX_Y_UPDATE);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f - TEX_Y_UPDATE);

	if (pVtx[0].tex.y > 1.0f)
	{
		//頂点の座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	if (pVtx[2].tex.y > 1.0f)
	{
		//頂点の座標
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);	

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBg);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	
}
