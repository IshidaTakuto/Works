//=============================================================================
//
// ポーズ処理 [Pausebg.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "pausebg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSEBG_TEXTURENAME	"data/TEXTURE/pause100.png"	//ポーズのテクスチャ名
#define TEX_UPDATE	(g_Pausebg.nCountAnim * 0.005f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePausebg = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPausebg = NULL;	//頂点バッファへのポインタ
PAUSEBG g_Pausebg;

//=============================================================================
// 初期化処理
//=============================================================================
void InitPausebg(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//背景の情報の初期化
	g_Pausebg.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);	//背景の位置
	g_Pausebg.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	g_Pausebg.nCountAnim = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PAUSEBG_TEXTURENAME, &g_pTexturePausebg);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPausebg,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPausebg->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_Pausebg.pos.x - 200, g_Pausebg.pos.y - 200, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Pausebg.pos.x + 200, g_Pausebg.pos.y - 200, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Pausebg.pos.x - 200, g_Pausebg.pos.y + 200, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Pausebg.pos.x + 200, g_Pausebg.pos.y + 200, 0.0f);

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

	//頂点バッファをアンロック
	g_pVtxBuffPausebg->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPausebg(void)
{
	//テクスチャの破棄
	if (g_pTexturePausebg != NULL)
	{
		g_pTexturePausebg->Release();
		g_pTexturePausebg = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPausebg != NULL)
	{
		g_pVtxBuffPausebg->Release();
		g_pVtxBuffPausebg = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePausebg(void)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPausebg->Lock(0, 0, (void**)&pVtx, 0);

	g_Pausebg.nCountAnim++;

	// テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2(0.0f - TEX_UPDATE, 0.0f - TEX_UPDATE);
	pVtx[1].tex = D3DXVECTOR2(1.0f - TEX_UPDATE, 0.0f - TEX_UPDATE);
	pVtx[2].tex = D3DXVECTOR2(0.0f - TEX_UPDATE, 1.0f - TEX_UPDATE);
	pVtx[3].tex = D3DXVECTOR2(1.0f - TEX_UPDATE, 1.0f - TEX_UPDATE);

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);

	if (pVtx[0].tex.x > 1.0f)
	{
		// 頂点の座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	}

	if (pVtx[1].tex.x > 1.0f)
	{
		// 頂点の座標
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffPausebg->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPausebg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPausebg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePausebg);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
