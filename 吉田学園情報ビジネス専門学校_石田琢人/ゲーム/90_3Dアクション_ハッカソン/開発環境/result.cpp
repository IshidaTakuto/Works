//*************************************************************
//
// ランキングの処理[result.cpp]
//  Authot : 佐藤亮太
//
//*************************************************************
#include "main.h"				// メイン情報のヘッダー
#include "game.h"				// ゲーム情報のヘッダー
#include "result.h"				// ランキング情報のヘッダー
#include "camera.h"				// カメラ情報のヘッダー
#include "light.h"				// ライト情報のヘッダー
#include "meshField.h"			// メッシュフィールド情報のヘッダー
#include "field.h"				// 水面情報のヘッダー
#include "player.h"				// プレイヤー情報のヘッダー
#include "object.h"				// オブジェクト情報のヘッダー
#include "time.h"				// タイム情報のヘッダー
#include "fade.h"				// フェード情報のヘッダー
#include "input.h"				// キーボード入力情報のヘッダー
#include "Gamepad.h"			// コントローラー入力情報のヘッダー
#include "sound.h"				// サウンド情報のヘッダー
#if _DEBUG
#include "line.h"
#endif


//=============================================================
//マクロ定義
//=============================================================
// ミリ秒の設定
#define RESULT_MILLI_SECOND_POS_X		(750)										// ミリ秒の左X座標
#define RESULT_MILLI_SECOND_POS_Y		(155)										// ミリ秒の上Y座標
#define RESULT_MILLI_SECOND_WIDTH		(25)										// ミリ秒の幅
#define RESULT_MILLI_SECOND_HEIGHT		(45)										// ミリ秒の高さ
#define RESULT_MILLI_SECOND_INTERVAL	(-30)										// ミリ秒の間隔

// 秒の設定
#define RESULT_SECOND_POS_X				(660)										// 秒の左X座標
#define RESULT_SECOND_POS_Y				(135)										// 秒の上Y座標

// 分の設定
#define RESULT_MINUTE_POS_X				(540)										// 分の左X座標
#define RESULT_MINUTE_POS_Y				(135)										// 分の上Y座標

// 分・秒の大きさと間隔
#define RESULT_TIME_WIDTH				(50)										// 分・秒のポリゴンの幅
#define RESULT_TIME_HEIGHT				(70)										// 分・秒のポリゴンの高さ
#define RESULT_TIME_INTERVAL			(-50)										// 分・秒の間隔

// コロンの設定
#define RESULT_COLON_POS_X				(580)										// コロンのXの位置
#define RESULT_COLON_POS_Y				(140)										// コロンのYの位置
#define RESULT_COLON_WIDTH				(40)										// コロンのポリゴンの幅
#define RESULT_COLON_HEIGHT				(60)										// コロンのポリゴンの高さ

#define RESULT_INTERVAL					(110.0f)									// ランキングの間隔

#define MAX_RESULT_TIME					(6)											// タイムの桁数
#define MAX_RESULT_COLON				(5)											// コロンの数
#define MAX_RESULT_NUMBER				(5)											// 順位の数
#define MAX_RESULT						(MAX_RESULT_TIME * MAX_RESULT_NUMBER)		// ランキングタイムの頂点数(桁 * 順位数)
#define RESULT_TAITLE_WIDTH				(480)										// タイトルの幅
#define RESULT_TAITLE_HEIGHT			(100)										// タイトルの高さ
#define RESULT_TAITLE_POS_X				(SCREEN_WIDTH/2-RESULT_TAITLE_WIDTH/2)		// タイトルの左X座標
#define RESULT_TAITLE_POS_Y				(20)										// タイトルの上Y座標

#define RESULT_RANK_WIDTH				(200)										// 順位の幅
#define RESULT_RANK_HEIGHT				(100)										// 順位の高さ
#define RESULT_RANK_POS_X				(250)										// 順位の左X座標
#define RESULT_RANK_POS_Y				(125)										// 順位の上Y座標
#define MAX_TEXTURE						(4)											// テクスチャの数
#define FRASHING_TIMING					(2)											// 点滅の切り替えの速度
#define RESULT_END_TIME					(300)										// ランキングが終わるまでのフレーム数

//=============================================================
//プロトタイプ宣言宣言
//=============================================================
void MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);									// 頂点設定

//=============================================================
//グローバル変数宣言
//=============================================================
LPDIRECT3DTEXTURE9		g_apTextureResult[MAX_TEXTURE] ;							// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;									// 頂点バッファへのポインタ
int						g_aRankTime[MAX_RESULT_NUMBER] = { 60 * 60,65 * 60,70 * 60,75 * 60,80 * 60 };	// ランキングのスコア
bool					g_bResultIn;												// ランキングに入ったかどうか
bool					g_bResultEnd;												// ランキングが終わるかどうか
short int				g_nResultCounter;											// ランキングが終わるまでのカウンター
char g_acResultTextureInfo[MAX_TEXTURE][MAX_CHAR] =
{
	"data/TEXTURE/number001.png",				// 数字
	"data/TEXTURE/ranking_item_logo.png",		// ロゴ
	"data/TEXTURE/ranking_rank.png",			// 順位
	"data/TEXTURE/colon.png",					// コロン
};

static RESULTSTATE g_state = RESULTSTATE_GAMECLEAR;



//===========================================================
//初期化処理
//===========================================================
void InitResult(void)
{
	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

#if _DEBUG
	// ライン
	InitLine();
#endif

	// メッシュフィールドの初期化処理
	InitmeshField();

	// プレイヤーの初期化処理
	InitPlayer();

	// オブジェクトの初期化処理
	InitObject();

	// 水面
	InitField();

	LPDIRECT3DDEVICE9 pDevice;														//デバイスへのポインタ

	//デバイスの読み込み
	pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{// テクスチャの総数分ループ
		D3DXCreateTextureFromFile(pDevice,								//3Dデバイスのアドレス
			g_acResultTextureInfo[nCntTexture],							//読み込むテクスチャのファイル名
			&g_apTextureResult[nCntTexture]);							//テクスチャのポインタ
	}

	// 変数の初期化
	g_bResultIn = false;															// ランキングに入ったどうか（初期はfalse）
	g_nResultCounter = 0;															// カウンターの初期化
	g_bResultEnd = false;															// ランキングが終了するかどうか


	// 頂点情報の設定
	MakeVertexResult(pDevice);

	// BGM再生
	PlaySound(SOUND_LABEL_BGM003);
}

//===========================================================
//終了処理
//===========================================================
void UninitResult(void)
{
	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

#if _DEBUG
	// ライン
	UninitLine();
#endif

	// メッシュフィールドの終了処理
	UninitmeshField();

	// オブジェクトの終了処理
	UninitObject();

	// 水面
	UninitField();

	int nCntTexture;						// カウント変数

	for (nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		//テクスチャの破棄
		if (g_apTextureResult[nCntTexture] != NULL)
		{//テクスチャの変数に何か入ってるとき
			g_apTextureResult[nCntTexture] -> Release();
			g_apTextureResult[nCntTexture] = NULL;			//中身を捨てる
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;			//中身を捨てる
	}

	// BGM & SE停止
	StopSound();
}

//===========================================================
//更新処理
//===========================================================
void UpdateResult(void)
{
	VERTEX_2D *pVtx;									// ポインタ変数
	int nTime = GetTime();								// 今回のスコアを入れる変数
	static bool bFlashing = false;						// 点滅の切り替え(true = 白,false = 赤)
	static int nCntFlashing = 0;						// 点滅のカウント変数
	int nNextVtx = (MAX_RESULT_TIME + 1) * 4;			// 次の頂点を入れる変数(タイムの桁数+コロン)*頂点数

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// ハイスコアの位置を特定
	for (int nCntResult = 0; nCntResult < MAX_RESULT_NUMBER; nCntResult++)
	{// 1位から比較している
		if (nTime >= g_aRankTime[nCntResult])
		{// 今回のスコアがランキングより上だったら
			g_bResultIn = true;												// ランキングに入ったらtrueにする
			break;																// ハイスコアの場所を見つけたらループを抜けさせる
		}
		else
		{// ランキング外だったら
			pVtx += nNextVtx;													// ポインタを次のタイムに進める
		}
	}

	// ランキングに入った時の点滅処理
	if (g_bResultIn == true)
	{// ランキングに入ったら
		// 8桁分色を変える
		for (int nCntResultScore = 0; nCntResultScore < MAX_RESULT_TIME + 1; nCntResultScore++, pVtx += 4)
		{
			if (bFlashing == true)
			{// 白
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{// 赤
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
			}
		}

		// 点滅切り替え
		if (nCntFlashing % FRASHING_TIMING == 0)
		{
			bFlashing = bFlashing ? false : true;				// 切り替え
		}
	}

	nCntFlashing++;												// カウントを進める

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	// リザルト画面の終了処理
	if (g_bResultEnd == false)
	{// リザルト画面が変わり始めてなければ
		if (GetKeyboardTrigger(DIK_RETURN)		== true ||
			GetKeyboardTrigger(DIK_SPACE)		== true ||
			GetGamapadKeyTrigger(ELECOM_A)		== true ||
			GetGamapadKeyTrigger(ELECOM_START)	== true)
		{// エンター押したら終了
			g_bResultEnd = true;								// trueにして連打対策
			SetFade(MODE_TITLE);								// フェードの設定
		}
		if (g_nResultCounter > RESULT_END_TIME)
		{// ５秒経ったら終了
			g_bResultEnd = true;								// trueにして連打対策
			SetFade(MODE_TITLE);								// フェードの設定
		}
	}

	g_nResultCounter++;										// 毎フレーム加算する

	// カメラの更新処理
	UpdateCamera();

#if _DEBUG
	// ライン
	UpdateLine();
#endif

	// 流れるオブジェクトの更新処理
	//UpdateObject();

	// メッシュフィールドの更新処理
	UpdatemeshField();

	// 水面
	UpdateField();

	// プレイヤーの更新処理
	UpdatePlayer();
}

//===========================================================
//描画処理
//===========================================================
void DrawResult(void)
{
	// カメラの設定処理
	SetCamera();

#if _DEBUG
	DrawLine();
#endif

	// メッシュフィールドの描画処理
	DrawmeshField();

	// プレイヤーの描画処理
	DrawPlayer();

	// オブジェクトの描画処理
	DrawObject();

	// 水面
	DrawField();

	int nCntTexture;																// カウント変数
	int nCntVtx = 0;																// 描画する最初の頂点を入れる変数

	//デバイスの変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();															//main.cppから受け取ったg_aD3DDeviceのアドレスを代入

	// Zバッファの設定
	//pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);								// "FALSE"で描画の順番を無視して、常に手前に描画する
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);								// "FALSE"で描画する順番を遠くから描画するようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);							// 描画する順番を決める

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// スコアの描画
	for (int nCntResult = 0; nCntResult < MAX_RESULT_NUMBER; nCntResult++)
	{// ランキング5個分作成

		for (int nCntTime = 0; nCntTime < MAX_TIME_SET_TYPE; nCntTime++)
		{// セットの種類分ループ
			for (int nCntTimeSet = 0; nCntTimeSet < TIME_SET; nCntTimeSet++)
			{// ワンセット数分ループ
			 //テクスチャの設定
				pDevice->SetTexture(0, g_apTextureResult[0]);						//「0」を変えるとテクスチャが消える。

				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
					nCntVtx,														//開始する頂点のインデックス
					2);																//描写するプリミティブ数

				nCntVtx += 4;														// 描画する頂点を進める
			}
		}

		//--------------------------------------------
		// コロンの描画
		//--------------------------------------------

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResult[3]);								//「0」を変えるとテクスチャが消える。

																					//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntVtx,																//開始する頂点のインデックス
			2);																		//描写するプリミティブ数

		nCntVtx += 4;																// 描画する頂点を進める

	}
	// タイトルの描画
	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureResult[1]);									//「0」を変えるとテクスチャが消える。

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntVtx,																	//開始する頂点のインデックス
		2);																			//描写するプリミティブ数

	nCntVtx += 4;																	// 描画する頂点を進める


	// ランクの描画
	for (nCntTexture = 0; nCntTexture < MAX_RESULT_NUMBER; nCntTexture++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResult[2]);								//「0」を変えるとテクスチャが消える。

																					//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntVtx,																//開始する頂点のインデックス
			2);																		//描写するプリミティブ数

		nCntVtx += 4;																// 描画する頂点を進める

	}



	// Zバッファの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//===========================================================
//ランキングの頂点設定
//===========================================================
void MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	VERTEX_2D *pVtx;													//頂点情報へのポインタ
	int nCntResult;													// ランキングのカウント変数
	float fResultInterval ;											// ランキングの間隔
	int nScoreTextur = 10;												// 数字のテクスチャを決める計算の変数
	int nCntRank;														// 順位のカウント変数
	int nTimePos;														// タイムの位置
	int nTimeTextur;													// タイムのテクスチャの座標を計算するための変数
	float nRankTexture = 0.0f;											// 順位のテクスチャの計算の変数


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_RESULT + 1 + MAX_RESULT_NUMBER + MAX_RESULT_COLON),	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,																	//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResult = 0; nCntResult < MAX_RESULT_NUMBER; nCntResult++)
	{// ランキング5個分作成
		fResultInterval = nCntResult * RESULT_INTERVAL;								// ランキングの間隔

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
					nTimePos = nCntTimeSet * RESULT_MILLI_SECOND_INTERVAL;				// 間隔をあける

					// 頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3(RESULT_MILLI_SECOND_POS_X + (nTimePos + 0.0f),							   fResultInterval + RESULT_MILLI_SECOND_POS_Y, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(RESULT_MILLI_SECOND_POS_X + (nTimePos + 0.0f + RESULT_MILLI_SECOND_WIDTH), fResultInterval + RESULT_MILLI_SECOND_POS_Y, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(RESULT_MILLI_SECOND_POS_X + (nTimePos + 0.0f),							   fResultInterval + RESULT_MILLI_SECOND_POS_Y + RESULT_MILLI_SECOND_HEIGHT, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(RESULT_MILLI_SECOND_POS_X + (nTimePos + 0.0f + RESULT_MILLI_SECOND_WIDTH), fResultInterval + RESULT_MILLI_SECOND_POS_Y + RESULT_MILLI_SECOND_HEIGHT, 0.0f);

					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2((g_aRankTime[nCntResult] % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aRankTime[nCntResult] % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aRankTime[nCntResult] % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aRankTime[nCntResult] % 60 * 100 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 1.0f);
				}
				else if (nCntTime == TimeType_Second)
				{// 秒の時
					nTimePos = nCntTimeSet * RESULT_TIME_INTERVAL;						// 間隔をあける

					// 頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3(RESULT_SECOND_POS_X + (nTimePos + 0.0f),						 fResultInterval + RESULT_SECOND_POS_Y, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(RESULT_SECOND_POS_X + (nTimePos + 0.0f + RESULT_TIME_WIDTH), fResultInterval + RESULT_SECOND_POS_Y, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(RESULT_SECOND_POS_X + (nTimePos + 0.0f),						 fResultInterval + RESULT_SECOND_POS_Y + RESULT_TIME_HEIGHT, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(RESULT_SECOND_POS_X + (nTimePos + 0.0f + RESULT_TIME_WIDTH), fResultInterval + RESULT_SECOND_POS_Y + RESULT_TIME_HEIGHT, 0.0f);

					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2((g_aRankTime[nCntResult] / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aRankTime[nCntResult] / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aRankTime[nCntResult] / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aRankTime[nCntResult] / 60 % 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 1.0f);
				}
				if (nCntTime == TimeType_Minute)
				{// 分の時
					nTimePos = nCntTimeSet * RESULT_TIME_INTERVAL;						// 間隔をあける

					// 頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3(RESULT_MINUTE_POS_X + (nTimePos + 0.0f),						 fResultInterval + RESULT_MINUTE_POS_Y, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(RESULT_MINUTE_POS_X + (nTimePos + 0.0f + RESULT_TIME_WIDTH), fResultInterval + RESULT_MINUTE_POS_Y, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(RESULT_MINUTE_POS_X + (nTimePos + 0.0f),						 fResultInterval + RESULT_MINUTE_POS_Y + RESULT_TIME_HEIGHT, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(RESULT_MINUTE_POS_X + (nTimePos + 0.0f + RESULT_TIME_WIDTH), fResultInterval + RESULT_MINUTE_POS_Y + RESULT_TIME_HEIGHT, 0.0f);

					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2((g_aRankTime[nCntResult] / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aRankTime[nCntResult] / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aRankTime[nCntResult] / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aRankTime[nCntResult] / 60 / 60) % nTimeTextur / (nTimeTextur / 10) / 10.0f + 0.1f, 1.0f);
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

				pVtx += 4;																// ポインタを進める
				nTimeTextur = nTimeTextur * 10;											// 10倍して次の桁に合わせる
			}
		}

		//--------------------------------------
		// コロンの頂点情報の設定
		//--------------------------------------

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(RESULT_COLON_POS_X,						 fResultInterval + RESULT_COLON_POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(RESULT_COLON_POS_X + RESULT_COLON_WIDTH, fResultInterval + RESULT_COLON_POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RESULT_COLON_POS_X,						 fResultInterval + RESULT_COLON_POS_Y + RESULT_COLON_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RESULT_COLON_POS_X + RESULT_COLON_WIDTH, fResultInterval + RESULT_COLON_POS_Y + RESULT_COLON_HEIGHT, 0.0f);

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

		pVtx += 4;																		// ポインタを進める

		nScoreTextur = 10;																// 計算する桁を元に戻す
	}

	// タイトルの頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(RESULT_TAITLE_POS_X,							RESULT_TAITLE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RESULT_TAITLE_POS_X+ RESULT_TAITLE_WIDTH,	RESULT_TAITLE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RESULT_TAITLE_POS_X,							RESULT_TAITLE_POS_Y+ RESULT_TAITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RESULT_TAITLE_POS_X+ RESULT_TAITLE_WIDTH,	RESULT_TAITLE_POS_Y+ RESULT_TAITLE_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	////テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;																		// ポインタを進める

	//--------------------------------------
	// 順位の頂点情報の設定
	//--------------------------------------
	for (nCntRank = 0; nCntRank < MAX_RESULT_NUMBER; nCntRank++)
	{
		fResultInterval = nCntRank * RESULT_INTERVAL;								// ランキングの間隔

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(RESULT_RANK_POS_X					  + 0.0f, (RESULT_RANK_POS_Y + fResultInterval					   + 0.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(RESULT_RANK_POS_X + RESULT_RANK_WIDTH + 0.0f, (RESULT_RANK_POS_Y + fResultInterval					   + 0.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RESULT_RANK_POS_X					  + 0.0f, (RESULT_RANK_POS_Y + fResultInterval + RESULT_RANK_HEIGHT + 0.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RESULT_RANK_POS_X + RESULT_RANK_WIDTH + 0.0f, (RESULT_RANK_POS_Y + fResultInterval + RESULT_RANK_HEIGHT + 0.0f), 0.0f);


		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);

		////テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, nRankTexture + 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, nRankTexture + 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, nRankTexture + 0.2f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, nRankTexture + 0.2f);

		nRankTexture += 0.2f;														// 次のテクスチャの位置に進める
		pVtx += 4;																	// ポインタを進める

	}
	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}


//===========================================================
//ランキングの設定
//===========================================================
void SetRankTime(int nScore)
{
	int nAhead0;													// 入れ替え用変数0
	int nAhead1;													// 入れ替え用変数1
	int nCntRank = 0;												// 入れ替えカウント変数(0=ハイスコア,1<=それ以外の順位)

	for (int nCntResult = 0; nCntResult < MAX_RESULT_NUMBER; nCntResult++)
	{// 5位分ループ
		if (nScore < g_aRankTime[nCntResult])
		{// 今回のスコアがランキングよりも上だったら
			if (nCntRank == 0)
			{// 5位だったら
				nAhead0 = g_aRankTime[nCntResult];					// すでに入ってるスコアを退避
				g_aRankTime[nCntResult] = nScore;					// 新しいハイスコアを入れる
				nCntRank++;											// カウントを加算してハイスコア以外の順位の入れ替えをさせる
			}
			else
			{// 4位以上だったら入れ替え
				nAhead1 = g_aRankTime[nCntResult];					// すでに入ってるスコアを退避
				g_aRankTime[nCntResult] = nAhead0;					// 上の順位だったスコアを入れる
				nAhead0 = nAhead1;									// ここの順位だったスコアを退避しておく
			}
		}
	}
}

void SetResultState(RESULTSTATE state)
{
	g_state = state;
}

RESULTSTATE GetResultState(void)
{
	return g_state;
}