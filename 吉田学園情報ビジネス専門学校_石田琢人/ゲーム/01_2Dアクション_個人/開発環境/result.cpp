//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "result.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"
#include "block.h"
#include "effect.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_TEXTURENAME0	"DATA/TEXTURE/result_logo000.png"	// テクスチャ名
#define RESULT_TEXTURENAME1	"DATA/TEXTURE/result001.png"	// テクスチャ名
#define RESULT_TEXTURENAME2	"DATA/TEXTURE/result002.png"	// テクスチャ名
#define RESULT_TIME	(600)	// リザルト表示時間
#define MAX_RESULT	(3)
#define LOGO_SIZE_X	(200)
#define LOGO_SIZE_Y	(50)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// 頂点バッファへのポインタ
RESULT g_aResult[MAX_RESULT];
int g_nCntResult;

//=============================================================================
// 初期化処理
//=============================================================================
void InitResult(void)
{
	int nCntResult;

	// 背景初期化処理
	InitBg();

	// ブロック初期化処理
	InitBlock();

	// エフェクト初期化処理
	InitEffect();

	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 初期化
	g_aResult[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aResult[0].nType = 0;

	g_aResult[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 - 50, 0.0f);
	g_aResult[1].nType = 1;

	g_aResult[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 70, SCREEN_HEIGHT / 2 + 50, 0.0f);
	g_aResult[2].nType = 2;

	g_nCntResult = 0;

	// 朝日
	SetEffect(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),D3DXCOLOR(1.0f,0.7f,0.0f,0.4f),SCREEN_HEIGHT,0,EFFECTSTATE_RESULT);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME0, &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME1, &g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME2, &g_pTextureResult[2]);

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

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

	for (nCntResult = 1; nCntResult < MAX_RESULT; nCntResult++)
	{
		pVtx += 4;	//頂点データのポインタを4つ進める

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x - LOGO_SIZE_X, g_aResult[nCntResult].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x + LOGO_SIZE_X, g_aResult[nCntResult].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x - LOGO_SIZE_X, g_aResult[nCntResult].pos.y + LOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x + LOGO_SIZE_X, g_aResult[nCntResult].pos.y + LOGO_SIZE_Y, 0.0f);

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.5f, 0.3f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.3f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.3f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.3f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	// 背景終了処理
	UninitBg();

	// ブロック終了処理
	UninitBlock();

	// エフェクト終了処理
	UninitEffect();

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
	// 背景更新処理
	UpdateBg();

	// ブロック更新処理
	UpdateBlock();

	// エフェクト更新処理
	UpdateEffect();

	FADE pFade;

	g_nCntResult++;

	//フェードの取得
	pFade = *GetFade();
	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);

			//モード選択
			SetFade(MODE_RANKING);
		}
		else if(g_nCntResult >= RESULT_TIME)
		{
			//モード選択
			SetFade(MODE_RANKING);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	// 背景描画処理
	DrawBg();

	// ブロック描画処理
	DrawBlock();

	// エフェクト描画処理
	DrawEffect();

	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[g_aResult[nCntResult].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntResult, 2);
	}
}
