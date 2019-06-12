//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// カウントシーン処理 [countScene.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "game.h"
#include "countScene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COUNTSCENE_WIDTH		(600.0f)				// 幅
#define COUNTSCENE_HEIGHT		(200.0f)				// 高さ
#define START_COUNTSCENE_X		(SCREEN_WIDTH / 2)		// 開始位置[X]
#define START_COUNTSCENE_Y		(SCREEN_HEIGHT / 2)		// 開始位置[Y]
#define COUNTSCENE_FADESPEED	(0.1f)					// フェード速度
#define COUNTSCENE_WAITTIME		(120)					// 待機時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexcountScene(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffcountScene = NULL;		// 頂点バッファへのポインタ
COUNTSCENE				g_CountScene;						// カウントシーン情報

LPDIRECT3DTEXTURE9		g_pTexturecountScene[COUNTSCENETYPE_MAX] = {};		// テクスチャへのポインタ

//--- テクスチャ ---//
CountSceneTexInfo g_aCountScenetexInfo[COUNTSCENETYPE_MAX] =
{
	{ "data/TEXTURE/スタート.png" },		// スタート
	{ "data/TEXTURE/goal.png" },		// ゴール
};

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitcountScene(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期化
	g_CountScene.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 非表示
	g_CountScene.type = COUNTSCENETYPE_START;
	g_CountScene.countSceneState = COUNTSCENESTATE_NONE;
	g_CountScene.countSceneStateCounter = 0;

	// テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < COUNTSCENETYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aCountScenetexInfo[nCntTex].pFilename, &g_pTexturecountScene[nCntTex]);
	}

	// 頂点情報の作成
	MakeVertexcountScene(pDevice);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitcountScene(void)
{
	// テクスチャの開放
	for (int nCntTex = 0; nCntTex < COUNTSCENETYPE_MAX; nCntTex++)
	{
		if (g_pTexturecountScene[nCntTex] != NULL)
		{
			g_pTexturecountScene[nCntTex]->Release();
			g_pTexturecountScene[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffcountScene != NULL)
	{
		g_pVtxBuffcountScene->Release();
		g_pVtxBuffcountScene = NULL;
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdatecountScene(void)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ

	if (g_CountScene.countSceneState != COUNTSCENESTATE_NONE)
	{// 何もしていない
		if (g_CountScene.countSceneState == COUNTSCENESTATE_IN)
		{// カウントシーンイン状態
			if (g_CountScene.color.a < 1.0f)
			{
				g_CountScene.color.a += COUNTSCENE_FADESPEED;
			}

			if(g_CountScene.color.a >= 1.0f)
			{
				g_CountScene.color.a = 1.0f;
				g_CountScene.countSceneState = COUNTSCENESTATE_WAIT;	// 待機状態
			}
		}
		else if (g_CountScene.countSceneState == COUNTSCENESTATE_WAIT)
		{// カウントシーン待機状態
			if (g_CountScene.countSceneStateCounter < COUNTSCENE_WAITTIME)
			{
				g_CountScene.countSceneStateCounter++;
			}

			if (g_CountScene.countSceneStateCounter == COUNTSCENE_WAITTIME)
			{
				g_CountScene.countSceneState = COUNTSCENESTATE_OUT;		// アウト状態
			}
		}
		else if (g_CountScene.countSceneState == COUNTSCENESTATE_OUT)
		{// カウントシーンアウト状態
			if (g_CountScene.color.a > 0.0f)
			{
				g_CountScene.color.a -= COUNTSCENE_FADESPEED;
			}

			if (g_CountScene.color.a >= 0.0f)
			{
				g_CountScene.color.a = 0.0f;
				g_CountScene.countSceneState = COUNTSCENESTATE_NONE;	// 何もしていない
			}
		}
	}

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffcountScene->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntcountScene = 0; nCntcountScene < MAX_COUNTSCENE; nCntcountScene++, pVtx += 4)
	{
		// 頂点座標の更新
		pVtx[0].col = g_CountScene.color;
		pVtx[1].col = g_CountScene.color;
		pVtx[2].col = g_CountScene.color;
		pVtx[3].col = g_CountScene.color;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffcountScene->Unlock();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawcountScene(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffcountScene, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturecountScene[g_CountScene.type]);

	// ポリゴンの描画
	for (int nCntcountScene = 0; nCntcountScene < MAX_COUNTSCENE; nCntcountScene++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
								nCntcountScene * 4,		// 描画を開始する頂点インデックス
								2);						// 描画するプリミティブ数
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 頂点情報の作成
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void MakeVertexcountScene(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COUNTSCENE,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffcountScene,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffcountScene->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	for (int nCntcountScene = 0; nCntcountScene < MAX_COUNTSCENE; nCntcountScene++, pVtx += 4)
	{
		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(START_COUNTSCENE_X + (-COUNTSCENE_WIDTH / 2), START_COUNTSCENE_Y + (-COUNTSCENE_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(START_COUNTSCENE_X + COUNTSCENE_WIDTH / 2, START_COUNTSCENE_Y + (-COUNTSCENE_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(START_COUNTSCENE_X + (-COUNTSCENE_WIDTH / 2), START_COUNTSCENE_Y + COUNTSCENE_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(START_COUNTSCENE_X + COUNTSCENE_WIDTH / 2, START_COUNTSCENE_Y + COUNTSCENE_HEIGHT / 2, 0.0f);
		// rhw
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			pVtx[nCntVtx].rhw = 1.0f;
		}
		// 頂点カラー
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffcountScene->Unlock();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// カウントシーンの設定
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SetcountScene(COUNTSCENETYPE type)
{
	// 情報の代入
	g_CountScene.type = type;								// 種類
	g_CountScene.countSceneState = COUNTSCENESTATE_IN;		// カウントシーンイン
	g_CountScene.countSceneStateCounter = 0;				// 状態カウンター
}