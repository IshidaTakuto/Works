//=========================================================================================================//
//
// window処理 [window.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"
#include "input.h"

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define WINDOW_VERTEX_NUM			(4)					// 頂点の数
#define WINDOW_PRIMITIVE_NUM		(2)					// プリミティブの数
#define WINDOW_MAX_TEXTURE			(3)
#define WINDOW_TextureName0		"data/TEXTURE/press_enter.png"		//タイトル エンターキー
#define WINDOW_TextureName1		"data/TEXTURE/nowloading.png"		//NOW LOADING
#define WINDOW_TextureName2		""									//マテリアル
#define	MAX_WINDOW			(24)		// windowの最大数
#define HALFEXPLOSION		(30)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;
	int nLengthX;
	int nLengthY;
	int nType;
	float fSteep;			//角度
	WINDOWSTATE state;
	WINDOWUSE use;
	int nCounterState[2];	//エネミーの状態管理用
	bool bUse;
}Window;

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureFadeWindow[WINDOW_MAX_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFadeWindow = NULL;	// 頂点バッファへのポインタ
Window g_aFadeWindow[MAX_WINDOW];

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitFadeWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntWindow;

	// 弾の情報の初期化
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		//g_aWindow[nCntWindow].pos = { 0.0f, 0.0f, 0.0f };
		//g_aWindow[nCntWindow].nLengthX = 0;
		//g_aWindow[nCntWindow].nLengthY = 0;
		//g_aWindow[nCntWindow].nType = 0;
		//g_aWindow[nCntWindow].col = { 1.0f, 1.0f, 1.0f, };
		g_aFadeWindow[nCntWindow].bUse = false;
	}

	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName0, &g_pTextureFadeWindow[0]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName1, &g_pTextureFadeWindow[1]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName2, &g_pTextureFadeWindow[2]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * WINDOW_VERTEX_NUM * MAX_WINDOW,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffFadeWindow,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFadeWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		//頂点座標の設定
		pVtx[nCntWindow * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntWindow * 4].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 1].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 2].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 3].rhw = 1.0f;

		//カラー設定
		pVtx[nCntWindow * 4].col = D3DCOLOR_RGBA(0, 0, 0, 255);	// A = 透明度
		pVtx[nCntWindow * 4 + 1].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[nCntWindow * 4 + 2].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[nCntWindow * 4 + 3].col = D3DCOLOR_RGBA(0, 0, 0, 255);

		//テクスチャ座標設定
		pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffFadeWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitFadeWindow(void)
{//テクスチャの破棄
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < WINDOW_MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureFadeWindow[nCntTexture] != NULL)
		{
			g_pTextureFadeWindow[nCntTexture]->Release();
			g_pTextureFadeWindow[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffFadeWindow != NULL)
	{
		g_pVtxBuffFadeWindow->Release();
		g_pVtxBuffFadeWindow = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateFadeWindow(void)
{
	int nCntWindow;
	static float fStealth = 0.02f;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//pPlayer = GetPlayer();

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFadeWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aFadeWindow[nCntWindow].bUse == true && g_aFadeWindow[nCntWindow].state != WINDOWSTATE_NORMAL)
		{
			switch (g_aFadeWindow[nCntWindow].state)
			{
			case WINDOWSTATE_FADEIN:	//Stealthの早送りなし
				g_aFadeWindow[nCntWindow].col.a += 0.05f;
				if (g_aFadeWindow[nCntWindow].col.a >= 1.0f)
				{
					g_aFadeWindow[nCntWindow].col.a = 1.0f;
					g_aFadeWindow[nCntWindow].state = WINDOWSTATE_NORMAL;
				}
				break;
			case WINDOWSTATE_FADEOUT://設定された透明度から暗くなり0で消える
				g_aFadeWindow[nCntWindow].col.a -= 0.05f;
				if (g_aFadeWindow[nCntWindow].col.a <= 0.0f)
				{
					g_aFadeWindow[nCntWindow].col.a = 0.0f;
					g_aFadeWindow[nCntWindow].bUse = false;
				}
				break;

			case WINDOWSTATE_FLASH://点滅で表示
				g_aFadeWindow[nCntWindow].nCounterState[0]++;
				if (g_aFadeWindow[nCntWindow].nCounterState[1] % 2 == 0 && g_aFadeWindow[nCntWindow].nCounterState[0] % 45 == 0)
				{
					g_aFadeWindow[nCntWindow].col.a = 1.0f;
					g_aFadeWindow[nCntWindow].nCounterState[1]++;
				}
				else if (g_aFadeWindow[nCntWindow].nCounterState[0] % 45 == 0)
				{
					g_aFadeWindow[nCntWindow].col.a = 0.0f;
					g_aFadeWindow[nCntWindow].nCounterState[1]++;
				}
				break;
			}

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);

			pVtx[nCntWindow * 4].col = g_aFadeWindow[nCntWindow].col;	// A = 透明度
			pVtx[nCntWindow * 4 + 1].col = g_aFadeWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_aFadeWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_aFadeWindow[nCntWindow].col;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffFadeWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void DrawFadeWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWindow;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffFadeWindow, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// ポリゴンの描画
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aFadeWindow[nCntWindow].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureFadeWindow[g_aFadeWindow[nCntWindow].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, WINDOW_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// 弾の設定処理
//=============================================================================
void SetFadeWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, int nCntUse, WINDOWSTATE state, WINDOWUSE use)
{
	int nCntWindow;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFadeWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 1; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (use == WINDOWUSE_GUARD)
		{
			nCntWindow = 0;
			if (g_aFadeWindow[nCntWindow].bUse == true)
			{
				break;
			}
		}
		if (g_aFadeWindow[nCntWindow].bUse == false)
		{
			g_aFadeWindow[nCntWindow].pos = pos;
			g_aFadeWindow[nCntWindow].nLengthX = nLengthX;
			g_aFadeWindow[nCntWindow].nLengthY = nLengthY;
			g_aFadeWindow[nCntWindow].nType = nType;
			g_aFadeWindow[nCntWindow].state = state;
			g_aFadeWindow[nCntWindow].use = use;
			g_aFadeWindow[nCntWindow].nCounterState[0] = nCntUse;
			g_aFadeWindow[nCntWindow].nCounterState[1] = 1;
			g_aFadeWindow[nCntWindow].bUse = true;

			g_aFadeWindow[nCntWindow].fSteep = D3DX_PI;
			/*pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y - g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y - g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y + g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y + g_aWindow[nCntWindow].nLengthY, 0.0f);*/

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);

			g_aFadeWindow[nCntWindow].col = col;
			pVtx[nCntWindow * 4].col = col;	// A = 透明度
			pVtx[nCntWindow * 4 + 1].col = col;
			pVtx[nCntWindow * 4 + 2].col = col;
			pVtx[nCntWindow * 4 + 3].col = col;
			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffFadeWindow->Unlock();
}
//=============================================================================
// LOADポリゴン破壊処理
//=============================================================================
void LoadBreakWindow(void)
{
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aFadeWindow[nCntWindow].use == WINDOWUSE_LOAD && g_aFadeWindow[nCntWindow].bUse == true)
		{
			g_aFadeWindow[nCntWindow].bUse = false;
			break;
		}
	}
}
//=============================================================================
// 破壊処理
//=============================================================================
void BreakFadeWindow(void)
{
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
			g_aFadeWindow[nCntWindow].bUse = false;
	}
}