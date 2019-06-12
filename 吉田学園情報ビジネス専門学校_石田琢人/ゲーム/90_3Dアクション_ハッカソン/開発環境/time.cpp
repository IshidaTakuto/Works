//=============================================================================
//
// タイム処理 [time.cpp]
// Author : 佐藤亮太
//
//=============================================================================
#include "time.h"		// タイムヘッダー
#include "game.h"		// ゲーム情報のヘッダー
#include "sound.h"		// サウンド情報のヘッダー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TIME			"data/TEXTURE/number001.png"	// 読み込むテクスチャファイル名
#define TEXTURE_FRAME			"data/TEXTURE/frame_timer.png"	// 読み込むテクスチャファイル名
#define TEXTURE_COLON			"data/TEXTURE/colon.png"		// 読み込むテクスチャファイル名
#define MAX_TEXTURE_RESULT		(3)								// テクスチャの種類
#define MAX_FRAME_TIME			(1)								// タイムの枠の数
#define MAX_COLON				(1)								// コロンの数
#define MAX_TIME				(6)								// タイムの最大桁数

#define MILLI_SECOND_POS_X		(1200)							// ミリ秒のXの位置
#define MILLI_SECOND_POS_Y		(40)							// ミリ秒のYの位置
#define MILLI_SECOND_WIDTH		(15)							// ミリ秒のポリゴンの幅
#define MILLI_SECOND_HEIGHT		(25)							// ミリ秒のポリゴンの高さ
#define TIME_WIDTH				(30)							// タイムのポリゴンの幅
#define TIME_HEIGHT				(50)							// タイムのポリゴンの高さ
#define SECOND_POS_X			(1150)							// 秒のXの位置
#define SECOND_POS_Y			(20)							// 秒のYの位置
#define MINUTE_POS_X			(1060)							// 分のXの位置
#define MINUTE_POS_Y			(20)							// 分のYの位置

#define TIME_INTERVAL			(-35)							// タイムの配置の間隔
#define MILLI_SECOND_INTERVAL	(-20)							// ミリ秒の配置の間隔

#define FRAME_POS_X				(1000)							// フレームのXの位置
#define FRAME_POS_Y				(0)								// フレームのYの位置
#define FRAME_WIDTH				(260)							// フレームのポリゴンの幅
#define FRAME_HEIGHT			(75)							// フレームのポリゴンの高さ
#define COLON_POS_X				(1085)							// コロンのXの位置
#define COLON_POS_Y				(20)							// コロンのYの位置
#define COLON_WIDTH				(30)							// コロンのポリゴンの幅
#define COLON_HEIGHT			(50)							// コロンのポリゴンの高さ

#define INIT_TIME				(0)							// タイムの初期値
#define TIME_SCORE				(100)							// タイムの特典


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTime[MAX_TEXTURE_RESULT];		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;					// 頂点バッファへのポインタ
int						g_nTime;								// タイム
char g_acTimeTextureInfo[MAX_TEXTURE_RESULT][MAX_CHAR] =
{// テクスチャのアドレス
	"data/TEXTURE/number001.png",		// 数字のテクスチャ
	"data/TEXTURE/frame_timer.png",		// フレームのテクスチャ
	"data/TEXTURE/colon.png",			// コロンのテクスチャ
};


//=============================================================================
// 初期化処理
//=============================================================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期化
	g_nTime = INIT_TIME;								// タイムの初期化

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE_RESULT; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice,							// デバイスへのポインタ
								  g_acTimeTextureInfo[nCntTexture],	// ファイルの名前
								  &g_pTextureTime[nCntTexture]);	// テクスチャへのポインタ
	}

	// 頂点情報の作成
	MakeVertexTime(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTime(void)
{
	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE_RESULT; nCntTexture++)
	{// テクスチャの総数分ループ
		if (g_pTextureTime[nCntTexture] != NULL)
		{// テクスチャの変数に何か入ってるとき
			g_pTextureTime[nCntTexture]->Release();
			g_pTextureTime[nCntTexture] = NULL;			//中身を捨てる
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;			//中身を捨てる
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTime(void)
{
	VERTEX_2D *pVtx;					// 頂点情報のポインタ変数
	GAMESTATE GameState;				// ゲームの状態を受け取る変数
	GameState = GetGameState();			// ゲームの状態を取得

	if (GameState == GAMESTATE_NORMAL)
	{// ゲームが通常状態の時のみタイムを減らす
		int nTimeTextur;						// テクスチャの位置を計算するための変数

		g_nTime++;								// タイムを増やす

		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntTime = 0; nCntTime < MAX_TIME_SET_TYPE; nCntTime++)
		{// セットの種類分ループ
			nTimeTextur = 10;												// テクスチャの桁の計算するための値を初期化
			for (int nCntTimeSet = 0; nCntTimeSet < TIME_SET; nCntTimeSet++)
			{// ワンセット数分ループ
				if (nCntTime == TimeType_MilliSecond)
				{// ミリ秒の時
					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2((g_nTime % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2((g_nTime % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2((g_nTime % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_nTime % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 1.0f);

				}
				else if (nCntTime == TimeType_Second)
				{// 秒の時
					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2((g_nTime / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2((g_nTime / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2((g_nTime / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_nTime / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 1.0f);
				}
				else if (nCntTime == TimeType_Minute)
				{// 分の時
					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2((g_nTime / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2((g_nTime / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2((g_nTime / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_nTime / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 1.0f);
				}

				pVtx += 4;													// ポインタを進める
				nTimeTextur = nTimeTextur * 10;								// 10倍して次の桁に合わせる
			}
		}

		// 頂点バッファをアンロックする
		g_pVtxBuffTime->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntTime;														// カウンター変数

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// タイム8桁分のポリゴン表示
	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTime[0]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
							   nCntTime * 4,												//開始する頂点のインデックス
							   2);															//描写するプリミティブ数
	}
	// フレームの表示
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime[1]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
						   MAX_TIME * 4,													//開始する頂点のインデックス
						   2);

	// コロンの表示
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime[2]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		(MAX_TIME + 1) * 4,																	//開始する頂点のインデックス
		2);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;
	int nTimePos;			// タイムの位置
	int nTimeTextur;		// テクスチャの位置を計算するための変数

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_TIME + MAX_FRAME_TIME + MAX_COLON),	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,																				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//--------------------------------------
	// タイムの頂点情報の設定
	//--------------------------------------
	for (int nCntTime = 0; nCntTime < MAX_TIME_SET_TYPE; nCntTime++)
	{// セットの種類分ループ
		nTimePos = 0;																// タイムの間隔を初期化
		nTimeTextur = 10;															// テクスチャの桁の計算するための値を初期化

		for (int nCntTimeSet = 0; nCntTimeSet < TIME_SET; nCntTimeSet++)
		{// ワンセット数分ループ

			if (nCntTime == TimeType_MilliSecond)
			{// ミリ秒の時
				nTimePos = nCntTimeSet * MILLI_SECOND_INTERVAL;						// 数字の間隔をあける
				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(MILLI_SECOND_POS_X + (nTimePos + 0.0f),					   MILLI_SECOND_POS_Y,				 0.0f);
				pVtx[1].pos = D3DXVECTOR3(MILLI_SECOND_POS_X + (nTimePos + 0.0f + MILLI_SECOND_WIDTH), MILLI_SECOND_POS_Y,				 0.0f);
				pVtx[2].pos = D3DXVECTOR3(MILLI_SECOND_POS_X + (nTimePos + 0.0f),					   MILLI_SECOND_POS_Y + MILLI_SECOND_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(MILLI_SECOND_POS_X + (nTimePos + 0.0f + MILLI_SECOND_WIDTH), MILLI_SECOND_POS_Y + MILLI_SECOND_HEIGHT, 0.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((g_nTime % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nTime % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nTime % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nTime % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 1.0f);
			}
			else if (nCntTime == TimeType_Second)
			{// 秒の時
				nTimePos = nCntTimeSet * TIME_INTERVAL;								// 数字の間隔をあける

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(SECOND_POS_X + (nTimePos + 0.0f),				 SECOND_POS_Y,				 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SECOND_POS_X + (nTimePos + 0.0f + TIME_WIDTH), SECOND_POS_Y,				 0.0f);
				pVtx[2].pos = D3DXVECTOR3(SECOND_POS_X + (nTimePos + 0.0f),				 SECOND_POS_Y + TIME_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SECOND_POS_X + (nTimePos + 0.0f + TIME_WIDTH), SECOND_POS_Y + TIME_HEIGHT, 0.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((g_nTime / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nTime / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nTime / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nTime / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 1.0f);
			}
			if (nCntTime == TimeType_Minute)
			{// 分の時
				nTimePos = nCntTimeSet * TIME_INTERVAL;								// 数字の間隔をあける

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(MINUTE_POS_X + (nTimePos + 0.0f),				 MINUTE_POS_Y,				 0.0f);
				pVtx[1].pos = D3DXVECTOR3(MINUTE_POS_X + (nTimePos + 0.0f + TIME_WIDTH), MINUTE_POS_Y,				 0.0f);
				pVtx[2].pos = D3DXVECTOR3(MINUTE_POS_X + (nTimePos + 0.0f),				 MINUTE_POS_Y + TIME_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(MINUTE_POS_X + (nTimePos + 0.0f + TIME_WIDTH), MINUTE_POS_Y + TIME_HEIGHT, 0.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((g_nTime / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nTime / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nTime / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nTime / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 1.0f);
			}


			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 4;													// ポインタを進める
			nTimeTextur = nTimeTextur * 10;								// 10倍して次の桁に合わせる
		}
	}

	//--------------------------------------
	// フレームの頂点情報の設定
	//--------------------------------------
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(FRAME_POS_X,				 FRAME_POS_Y,				 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FRAME_POS_X + FRAME_WIDTH, FRAME_POS_Y,				 0.0f);
	pVtx[2].pos = D3DXVECTOR3(FRAME_POS_X,				 FRAME_POS_Y + FRAME_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FRAME_POS_X + FRAME_WIDTH, FRAME_POS_Y + FRAME_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;													// ポインタを進める

	//--------------------------------------
	// コロンの頂点情報の設定
	//--------------------------------------
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(COLON_POS_X,				 COLON_POS_Y,				 0.0f);
	pVtx[1].pos = D3DXVECTOR3(COLON_POS_X + COLON_WIDTH, COLON_POS_Y,				 0.0f);
	pVtx[2].pos = D3DXVECTOR3(COLON_POS_X,				 COLON_POS_Y + COLON_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(COLON_POS_X + COLON_WIDTH, COLON_POS_Y + COLON_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//=============================================================================
// タイムを取得
//=============================================================================
int GetTime(void)
{
	return g_nTime;			// タイムの情報を返す
}

//=============================================================================
// タイムを初期化
//=============================================================================
void ResetTime(void)
{
	g_nTime = 0;			// タイムを0にする
}