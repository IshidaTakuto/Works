//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"
#include "block.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TEXTURENAME0	"DATA/TEXTURE/title_logo.png"	// タイトルのテクスチャ名
#define TITLE_TEXTURENAME1	"DATA/TEXTURE/press_enter.png"	// タイトルのテクスチャ名
#define TITLE_SIZE_X	(450.0f)	// タイトルの大きさ(X)
#define TITLE_SIZE_Y	(225.0f)	// タイトルの大きさ(Y)
#define ENTER_SIZE_X	(150.0f)	// 「PRESS ENTER」の大きさ(X)
#define ENTER_SIZE_Y	(50.0f)		// 「PRESS ENTER」の大きさ(Y)
#define MAX_TITLE		(2)			// タイトルのテクスチャ数
#define TITLE_TIME		(900)		// ランキング画面までの時間

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;	// 頂点バッファへのポインタ
TITLE g_aTitle[MAX_TITLE];
D3DXCOLOR g_colorTitle;	// 「PRESS ENTER」の点滅
int g_nTransparent;
int g_nCntTitle;

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitTitle(void)
{
	// 背景初期化処理
	InitBg();

	// ブロック初期化処理
	InitBlock();

	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();


	g_aTitle[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, -55, 0.0f);
	g_aTitle[0].nType = 0;

	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f);
	g_aTitle[1].nType = 1;

	g_colorTitle = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	g_nTransparent = 0;
	g_nCntTitle = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME0, &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME1, &g_pTextureTitle[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aTitle[0].pos.x - TITLE_SIZE_X, g_aTitle[0].pos.y - TITLE_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTitle[0].pos.x + TITLE_SIZE_X, g_aTitle[0].pos.y - TITLE_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTitle[0].pos.x - TITLE_SIZE_X, g_aTitle[0].pos.y + TITLE_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTitle[0].pos.x + TITLE_SIZE_X, g_aTitle[0].pos.y + TITLE_SIZE_Y, 0.0f);

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
	pVtx[4].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_SIZE_X, g_aTitle[1].pos.y - ENTER_SIZE_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_SIZE_X, g_aTitle[1].pos.y - ENTER_SIZE_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_SIZE_X, g_aTitle[1].pos.y + ENTER_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_SIZE_X, g_aTitle[1].pos.y + ENTER_SIZE_Y, 0.0f);

	// 1.0fで固定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラー
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	// テクスチャ座標
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitTitle(void)
{
	// 背景終了処理
	UninitBg();

	// ブロック終了処理
	UninitBlock();

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{
		// テクスチャの破棄
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateTitle(void)
{
	// 背景更新処理
	UpdateBg();

	// ブロック更新処理
	UpdateBlock();

	if (g_aTitle[0].pos.y < SCREEN_HEIGHT / 2 - 100)
	{// 上から
		g_aTitle[0].pos.y++;

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// スキップ
			g_aTitle[0].pos.y = SCREEN_HEIGHT / 2 - 100;
		}
	}
	else
	{// 一定地点についたとき
		g_nCntTitle++;	// ランキング画面までのカウンタ

		FADE pFade;

		// フェードの取得
		pFade = *GetFade();
		if (pFade == FADE_NONE)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{// メニュー画面へ
				PlaySound(SOUND_LABEL_SE_DECIDE);
				// モード選択
				SetFade(MODE_MENU);
			}
			else if (g_nCntTitle >= TITLE_TIME)
			{// 時間経過でランキング画面へ
				//モード選択
				SetFade(MODE_RANKING);
			}
		}

		if (g_colorTitle.a <= 1.0f && g_nTransparent == 0)
		{//	透明度が1.0fの時
			g_colorTitle.a -= 0.02f;
			if (g_colorTitle.a <= 0.0f)
			{// 0.0f以下になったとき
				g_colorTitle.a = 0.0f;
				g_nTransparent = 1;
			}
		}
		else if (g_colorTitle.a >= 0.0f && g_nTransparent == 1)
		{//	透明度が0.0fの時
			g_colorTitle.a += 0.02f;
			if (g_colorTitle.a >= 1.0f)
			{// 1.0f以上になったとき
				g_colorTitle.a = 1.0f;
				g_nTransparent = 0;
			}
		}

		if (g_nCntTitle % 450 == 0)
		{
			SetBlock(D3DXVECTOR3(-100, SCREEN_HEIGHT - 80.0f, 0.0f), 100.0f, 100.0f, D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 70.0f, 0.0f), BLOCKTYPE_SHARK);
		}
		else if (g_nCntTitle % 700 == 0)
		{
			SetBlock(D3DXVECTOR3(100 + SCREEN_WIDTH, SCREEN_HEIGHT - 80.0f, 0.0f), 100.0f, 100.0f, D3DXVECTOR3(0, SCREEN_HEIGHT - 70.0f, 0.0f), BLOCKTYPE_SHARK);
		}
		//else if (g_nCntTitle % 100 == 0)
		//{
		//	SetParticle(D3DXVECTOR3(rand() % SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT, 0.0f),D3DXCOLOR(0.0f,0.0f,1.0f,1.0f), rand() % 20 + (rand() % 10 * 0.2f), 80, PARTICLETYPE_NORMAL);
		//}
	}

	if (rand() % 740 == 0)
	{// ランダムに鳥を出す
		SetBlock(D3DXVECTOR3(-50, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}
	else if (rand() % 640 == 0)
	{// ランダムに鳥を出す
		SetBlock(D3DXVECTOR3(50 + SCREEN_WIDTH, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aTitle[0].pos.x - TITLE_SIZE_X, g_aTitle[0].pos.y - TITLE_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTitle[0].pos.x + TITLE_SIZE_X, g_aTitle[0].pos.y - TITLE_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTitle[0].pos.x - TITLE_SIZE_X, g_aTitle[0].pos.y + TITLE_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTitle[0].pos.x + TITLE_SIZE_X, g_aTitle[0].pos.y + TITLE_SIZE_Y, 0.0f);

	// 頂点の座標
	pVtx[4].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_SIZE_X, g_aTitle[1].pos.y - ENTER_SIZE_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_SIZE_X, g_aTitle[1].pos.y - ENTER_SIZE_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_SIZE_X, g_aTitle[1].pos.y + ENTER_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_SIZE_X, g_aTitle[1].pos.y + ENTER_SIZE_Y, 0.0f);

	// 頂点カラー
	pVtx[4].col = g_colorTitle;
	pVtx[5].col = g_colorTitle;
	pVtx[6].col = g_colorTitle;
	pVtx[7].col = g_colorTitle;

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawTitle(void)
{
	// 背景描画処理
	DrawBg();

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[g_aTitle[nCntTitle].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTitle, 2);
	}

	// ブロック描画処理
	DrawBlock();
}
