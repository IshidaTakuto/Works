//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "result.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_TEXTURENAME0	"DATA/TEXTURE/result_logo.png"	// テクスチャ名
#define RESULT_TEXTURENAME1	"DATA/TEXTURE/pressany.png"		// テクスチャ名
#define RESULT_TEXTURENAME2	"DATA/TEXTURE/result_clear.png"	// テクスチャ名
#define RESULT_TEXTURENAME3	"DATA/TEXTURE/result_over.png"	// テクスチャ名
#define RESULT_TEXTURENAME4	"DATA/TEXTURE/result_clear_bg.png"	// テクスチャ名
#define RESULT_TEXTURENAME5	"DATA/TEXTURE/result_over_bg.png"	// テクスチャ名
#define RESULT_TIME	(600)	// リザルト表示時間
#define MAX_RESULT	(4)
#define MAX_RESULT_TEXTURE	(6)
#define LOGO_SIZE_X	(200)
#define LOGO_SIZE_Y	(50)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// 頂点バッファへのポインタ
RESULT g_aResult[MAX_RESULT];
int g_nCntResult;
D3DXCOLOR g_colorResult;

//=============================================================================
// 初期化処理
//=============================================================================
void InitResult(void)
{
	int nCntResult;

	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// ロゴ
	g_aResult[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f);
	g_aResult[0].nType = 0;
	g_aResult[0].fWidth = LOGO_SIZE_X;
	g_aResult[0].fHeight = LOGO_SIZE_Y;

	// 入力待ち
	g_aResult[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f);
	g_aResult[1].nType = 1;
	g_aResult[1].fWidth = 200 * 1.5f;
	g_aResult[1].fHeight = 40 * 1.5f;

	// クリアかゲームオーバーの文字ロゴ
	g_aResult[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aResult[2].fWidth = 450;
	g_aResult[2].fHeight = 100;

	// 背景
	g_aResult[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aResult[3].fWidth = SCREEN_WIDTH / 2;
	g_aResult[3].fHeight = SCREEN_HEIGHT / 2;


	g_colorResult = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCntResult = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME0, &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME1, &g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME2, &g_pTextureResult[2]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME3, &g_pTextureResult[3]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME4, &g_pTextureResult[4]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME5, &g_pTextureResult[5]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x - g_aResult[nCntResult].fWidth, g_aResult[nCntResult].pos.y - g_aResult[nCntResult].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x + g_aResult[nCntResult].fWidth, g_aResult[nCntResult].pos.y - g_aResult[nCntResult].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x - g_aResult[nCntResult].fWidth, g_aResult[nCntResult].pos.y + g_aResult[nCntResult].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x + g_aResult[nCntResult].fWidth, g_aResult[nCntResult].pos.y + g_aResult[nCntResult].fHeight, 0.0f);

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntResult == MAX_RESULT - 1)
		{
			//頂点カラー
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		//テクスチャの破棄
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	FADE pFade;

	g_nCntResult++;

	//フェードの取得
	pFade = *GetFade();
	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetAnyTrigger(1,0) == true)
		{// 決定キー押下
			PlaySound(SOUND_LABEL_SELECT);

			SetFadeMode(MODE_RANKING);
		}
		if (g_nCntResult >= RESULT_TIME)
		{// 一定時間経過
			SetFadeMode(MODE_RANKING);
		}
	}

	if (g_colorResult.a <= 1.0f && g_aResult[1].nTransparent == 0)
	{//	透明度が1.0fの時
		g_colorResult.a -= 0.02f;
		if (g_colorResult.a <= 0.0f)
		{// 0.0f以下になったとき
			g_colorResult.a = 0.0f;
			g_aResult[1].nTransparent = 1;
		}
	}
	else if (g_colorResult.a >= 0.0f && g_aResult[1].nTransparent == 1)
	{//	透明度が0.0fの時
		g_colorResult.a += 0.02f;
		if (g_colorResult.a >= 1.0f)
		{// 1.0f以上になったとき
			g_colorResult.a = 1.0f;
			g_aResult[1].nTransparent = 0;
		}
	}

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[4].col = g_colorResult;
	pVtx[5].col = g_colorResult;
	pVtx[6].col = g_colorResult;
	pVtx[7].col = g_colorResult;

	// 頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();

	MODE mode = GetMode();
	// モードによってテクスチャを変える
	if (mode == MODE_RESULT)
	{
		g_aResult[2].nType = 2;
		g_aResult[3].nType = 4;
	}
	else if (mode == MODE_GAMEOVER)
	{
		g_aResult[2].nType = 3;
		g_aResult[3].nType = 5;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntResult = MAX_RESULT - 1; nCntResult >= 0; nCntResult--)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[g_aResult[nCntResult].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntResult, 2);
	}
}
