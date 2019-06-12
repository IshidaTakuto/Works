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
#include "camera.h"
#include "rain.h"
#include "effect.h"
#include "petals.h"
#include "snow.h"
#include "joypad.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TEXTURENAME0	"DATA/TEXTURE/title_logo.png"	// タイトルのテクスチャ名
#define TITLE_TEXTURENAME1	"DATA/TEXTURE/press_enter.png"	// タイトルのテクスチャ名
#define TITLE_SIZE_X	(380.0f)	// タイトルの大きさ(X)
#define TITLE_SIZE_Y	(125.0f)	// タイトルの大きさ(Y)
#define ENTER_WIDTH		(150.0f)	// 「PRESS ENTER」の幅
#define ENTER_HEIGHT	(50.0f)		// 「PRESS ENTER」の高さ
#define MAX_TITLE		(2)			// タイトルのテクスチャ数
#define TITLE_TIME		(400)		// ランキング画面までの時間

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;	// 頂点バッファへのポインタ
TITLE g_aTitle[MAX_TITLE];
int g_nTransparent;
int g_nCntTitle;
int g_nTitleSeason = 3;

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitTitle(void)
{
	// カメラの初期化処理
	InitCamera();

	// 雨の初期化処理
	InitRain();

	InitSnow();

	InitPetals();

	// エフェクト初期化処理
	InitEffect();
	SetEffect(D3DXVECTOR3(200.0f, 0.0f, 500.0f), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), 100.0f, 0, EFFECTSTATE_MOON);
	SetEffect(D3DXVECTOR3(200.0f, 0.0f, 500.0f), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), 120.0f, 0, EFFECTSTATE_NONE);

	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();


	g_aTitle[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f, 0.0f);
	g_aTitle[0].nType = 0;
	g_aTitle[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f);
	g_aTitle[1].nType = 1;
	g_aTitle[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	g_nTransparent = 0;
	g_nCntTitle = 0;

	g_nTitleSeason = (g_nTitleSeason + 1) % 4;

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点の座標
	pVtx[4].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_WIDTH, g_aTitle[1].pos.y - ENTER_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_WIDTH, g_aTitle[1].pos.y - ENTER_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_WIDTH, g_aTitle[1].pos.y + ENTER_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_WIDTH, g_aTitle[1].pos.y + ENTER_HEIGHT, 0.0f);

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
	// カメラの終了処理
	UninitCamera();

	// 雨の終了処理
	UninitRain();

	UninitSnow();

	UninitPetals();

	// エフェクトの終了処理
	UninitEffect();

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
	XINPUT_STATE state;
	state = GetJoyPad();

	// カメラの更新処理
	UpdateCamera();

	switch (g_nTitleSeason)
	{
	case 0:
		UpdatePetals(PETALSTYPE_SAKURA);
		break;

	case 1:
		// 雨の更新処理
		UpdateRain();
		break;

	case 2:
		UpdatePetals(PETALSTYPE_MOMIJI);
		break;

	case 3:
		UpdateSnow();
		break;
	}

	// エフェクトの更新処理
	UpdateEffect();

	//if (g_aTitle[0].pos.y < SCREEN_HEIGHT / 2 - 100)
	//{// 上から
	//	g_aTitle[0].pos.y++;

	//	if (GetKeyboardTrigger(DIK_RETURN) == true)
	//	{// スキップ
	//		g_aTitle[0].pos.y = SCREEN_HEIGHT / 2 - 100;
	//	}
	//}
	if (g_aTitle[0].col.a < 1.0f)
	{
		g_aTitle[0].col.a += 0.005f;

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// スキップ
			g_aTitle[0].col.a = 1.0f;
		}
	}
	else
	{
		g_nCntTitle++;	// ランキング画面までのカウンタ

		FADE pFade;

		// フェードの取得
		pFade = *GetFade();
		if (pFade == FADE_NONE)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{// メニュー画面へ
				PlaySound(SOUND_LABEL_DECIDE);
				// モード選択
				SetFade(MODE_MENU);
			}
			else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				PlaySound(SOUND_LABEL_DECIDE);
				// モード選択
				SetFade(MODE_MENU);
			}
			else if (g_nCntTitle >= TITLE_TIME)
			{// 時間経過でランキング画面へ
				//モード選択
				SetFade(MODE_RANKING);
			}
		}

		if (g_aTitle[1].col.a <= 1.0f && g_nTransparent == 0)
		{//	透明度が1.0fの時
			g_aTitle[1].col.a -= 0.02f;
			if (g_aTitle[1].col.a <= 0.0f)
			{// 0.0f以下になったとき
				g_aTitle[1].col.a = 0.0f;
				g_nTransparent = 1;
			}
		}
		else if (g_aTitle[1].col.a >= 0.0f && g_nTransparent == 1)
		{//	透明度が0.0fの時
			g_aTitle[1].col.a += 0.02f;
			if (g_aTitle[1].col.a >= 1.0f)
			{// 1.0f以上になったとき
				g_aTitle[1].col.a = 1.0f;
				g_nTransparent = 0;
			}
		}
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
	pVtx[4].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_WIDTH, g_aTitle[1].pos.y - ENTER_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_WIDTH, g_aTitle[1].pos.y - ENTER_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aTitle[1].pos.x - ENTER_WIDTH, g_aTitle[1].pos.y + ENTER_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aTitle[1].pos.x + ENTER_WIDTH, g_aTitle[1].pos.y + ENTER_HEIGHT, 0.0f);

	// 頂点カラー
	pVtx[0].col = g_aTitle[0].col;
	pVtx[1].col = g_aTitle[0].col;
	pVtx[2].col = g_aTitle[0].col;
	pVtx[3].col = g_aTitle[0].col;

	pVtx[4].col = g_aTitle[1].col;
	pVtx[5].col = g_aTitle[1].col;
	pVtx[6].col = g_aTitle[1].col;
	pVtx[7].col = g_aTitle[1].col;

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawTitle(void)
{
	// エフェクトの描画処理
	DrawEffect();

	switch (g_nTitleSeason)
	{
	case 0:
		DrawPetals();
		break;

	case 1:
		// 雨の更新処理
		DrawRain();
		break;

	case 2:
		DrawPetals();
		break;

	case 3:
		DrawSnow();
		break;
	}

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
}
