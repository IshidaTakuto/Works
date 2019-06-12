//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : 
//
//=============================================================================
#include "fade.h"
#include "main.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FADE_TEX		(3)
#define Fade_TextureName	"data/TEXTURE/チュートリアル001.jpg"				//FADEのTEXTURE名
#define Fade_TextureName1	"data/TEXTURE/チュートリアル002.jpg"				//FADEのTEXTURE名
#define Fade_TextureName2	"data/TEXTURE/チュートリアル003.jpg"				//FADEのTEXTURE名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureFade[MAX_FADE_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// 頂点バッファへのポインタ
FADE					g_fade;					// フェード状態
MODE					g_modeNext;				// 次のモード
D3DXCOLOR				g_colorFade;			// フェード色
int						g_nTypeFade;			//更新内容
int						g_nTex;			//画像

//=============================================================================
// 初期化処理
//=============================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_fade = FADE_IN;	//初期真っ黒画面
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR{ 0.0f,0.0f,0.0f,1.0f };
	g_nTypeFade = 0;
	g_nTex = 0;

	D3DXCreateTextureFromFile(pDevice, Fade_TextureName, &g_pTextureFade[0]);	//チュートリアル作成編
	D3DXCreateTextureFromFile(pDevice, Fade_TextureName1, &g_pTextureFade[1]);	//チュートリアルキーボード戦闘編
	D3DXCreateTextureFromFile(pDevice, Fade_TextureName2, &g_pTextureFade[2]);	//チュートリアルジョイパッド戦闘編

	// 値の初期化
	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//X,Y,0.0f
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラー設定
	pVtx[0].col = g_colorFade;	// A = 透明度
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点BUFFERをアンロックする
	g_pVtxBuffFade->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFade(void)
{
	for (int nCount = 0; nCount < MAX_FADE_TEX; nCount++)
	{
		if (g_pTextureFade[nCount] != NULL)
		{
			g_pTextureFade[nCount]->Release();
			g_pTextureFade[nCount] = NULL;
		}
	}
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade(void)
{
	PLAYER *pPlayer = GetPlayer();

	if (g_fade != FADE_NONE)
	{//フェード状態
		VERTEX_2D *pVtx;		//頂点情報へのポインタ
		//頂点BUFFERをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		if (g_fade == FADE_IN)
		{//明るくな～る
			g_colorFade.a -= 0.03f;
			if (g_colorFade.a <= 0.0f)
			{//完全に透明になったら
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if(g_fade == FADE_OUT)
		{//暗くな～る
			g_colorFade.a += 0.03f;
			if (g_colorFade.a >= 1.0f)
			{//完全に不透明になったら
				g_colorFade.a = 1.0f;
				SetMode(g_modeNext, g_nTypeFade);

				if (g_nTypeFade == 0 || g_nTypeFade == 1)
				{//そのまま晴れ
					g_fade = FADE_IN;
				}
				else
				{
					g_fade = FADE_ENTER;
					LoadBreakWindow();
					SetFadeWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.95f, 0.0f), D3DXCOLOR{ 1.0f,0.99f,0.04f, 1.0f }, 150, 30, 0, 0, WINDOWSTATE_FLASH, WINDOWUSE_LOAD);
				}
			}
		}
		else if (g_fade == FADE_ENTER && pPlayer[0].bButton[BUTTON_ENTER] == true)
		{
			g_fade = FADE_IN;
			LoadBreakWindow();
		}
		//カラー設定
		pVtx[0].col = g_colorFade;	// A = 透明度
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		//頂点BUFFERをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade[g_nTex]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点バッファの情報を設定

}

//=============================================================================
// フェードの設定
//=============================================================================
void SetFade(MODE modeNext, int nTypeFade, float fCola, int nTex)
{
	if (g_fade == FADE_NONE)
	{
		g_nTex = nTex;
		g_nTypeFade = nTypeFade;
		g_fade = FADE_OUT;		//暗くな～る
		//切り替えの予約
		if (nTypeFade == 1)
		{
			MODE *pmode = GetMode();
			g_modeNext = pmode[0];
		}
		else{ g_modeNext = modeNext; }
		if (nTypeFade == 0 || nTypeFade == 1) { g_colorFade = D3DXCOLOR{ 0.0f,0.0f,0.0f,0.0f }; }
		else
		{
		g_colorFade = D3DXCOLOR{ 1.0f,1.0f,1.0f,0.0f };
		SetFadeWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.95f, 0.0f), D3DXCOLOR{ 0.0f,0.0f,0.0f, 0.0f }, 150, 30, 1, 0, WINDOWSTATE_FADEIN, WINDOWUSE_LOAD);
		}
	}
}

//=============================================================================
// フェードの取得
//=============================================================================
FADE *GetFade(void)
{
	return &g_fade;
}

