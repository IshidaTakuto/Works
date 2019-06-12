//=============================================================================
//
// UI処理 [UI.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "UI.h"
#include "Gamepad.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_TEXTURENAME0	"data/TEXTURE/LB.jpg"	// タイトルのテクスチャ名
#define UI_TEXTURENAME1	"data/TEXTURE/RB.jpg"	// タイトルのテクスチャ名
#define UI_TEXTURENAME2	"data/TEXTURE/LT.jpg"	// タイトルのテクスチャ名
#define UI_TEXTURENAME3	"data/TEXTURE/RT.jpg"	// タイトルのテクスチャ名
#define UI_SIZE_X	(50.0f)	// タイトルの大きさ(X)
#define UI_SIZE_Y	(50.0f)	// タイトルの大きさ(Y)
#define MAX_UI		(4)			// タイトルのテクスチャ数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureUI[MAX_UI] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;	// 頂点バッファへのポインタ
UI g_aUI[MAX_UI];
int g_nCntUI;

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	g_aUI[0].pos = D3DXVECTOR3(UI_SIZE_X + 10.0f, UI_SIZE_Y + 10.0f, 0.0f);
	g_aUI[0].nType = 0;
	g_aUI[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_aUI[1].pos = D3DXVECTOR3(UI_SIZE_X + 120.0f, UI_SIZE_Y + 10.0f, 0.0f);
	g_aUI[1].nType = 1;
	g_aUI[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_aUI[2].pos = D3DXVECTOR3(UI_SIZE_X + 10.0f, UI_SIZE_Y + 120.0f, 0.0f);
	g_aUI[2].nType = 2;
	g_aUI[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_aUI[3].pos = D3DXVECTOR3(UI_SIZE_X + 120.0f, UI_SIZE_Y + 120.0f, 0.0f);
	g_aUI[3].nType = 3;
	g_aUI[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_nCntUI = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, UI_TEXTURENAME0, &g_pTextureUI[0]);
	D3DXCreateTextureFromFile(pDevice, UI_TEXTURENAME1, &g_pTextureUI[1]);
	D3DXCreateTextureFromFile(pDevice, UI_TEXTURENAME2, &g_pTextureUI[2]);
	D3DXCreateTextureFromFile(pDevice, UI_TEXTURENAME3, &g_pTextureUI[3]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL);

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - UI_SIZE_X, g_aUI[nCntUI].pos.y - UI_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + UI_SIZE_X, g_aUI[nCntUI].pos.y - UI_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - UI_SIZE_X, g_aUI[nCntUI].pos.y + UI_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + UI_SIZE_X, g_aUI[nCntUI].pos.y + UI_SIZE_Y, 0.0f);

		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffUI->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitUI(void)
{
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		// テクスチャの破棄
		if (g_pTextureUI[nCntUI] != NULL)
		{
			g_pTextureUI[nCntUI]->Release();
			g_pTextureUI[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateUI(void)
{
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		g_aUI[nCntUI].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	}

	if (GetGamapadKeyPress(ELECOM_LB))
	{
		g_aUI[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (GetGamapadKeyPress(ELECOM_RB))
	{
		g_aUI[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (GetGamapadKeyPress(ELECOM_LT))
	{
		g_aUI[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (GetGamapadKeyPress(ELECOM_RT))
	{
		g_aUI[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - UI_SIZE_X, g_aUI[nCntUI].pos.y - UI_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + UI_SIZE_X, g_aUI[nCntUI].pos.y - UI_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - UI_SIZE_X, g_aUI[nCntUI].pos.y + UI_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + UI_SIZE_X, g_aUI[nCntUI].pos.y + UI_SIZE_Y, 0.0f);

		// 頂点カラー
		pVtx[0].col = g_aUI[nCntUI].col;
		pVtx[1].col = g_aUI[nCntUI].col;
		pVtx[2].col = g_aUI[nCntUI].col;
		pVtx[3].col = g_aUI[nCntUI].col;

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffUI->Unlock();
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureUI[g_aUI[nCntUI].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntUI, 2);
	}
}
