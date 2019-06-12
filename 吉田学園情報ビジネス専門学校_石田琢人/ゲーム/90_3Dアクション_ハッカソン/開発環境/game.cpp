//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// ゲーム本編処理 [game.h]
// Author : Toshiharu Ueno
// Editor : Eisuke Sakagawa
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "game.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "meshDome.h"
#include "meshField.h"
#include "stage_object.h"
#include "object.h"
#include "field.h"
#include "player.h"
#include "petals.h"
#include "effect.h"
#include "gauge.h"
#include "particle.h"
#include "time.h"
#include "result.h"
#include "countScene.h"

#include "sound.h"

#if _DEBUG
#include "line.h"
#endif

//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;		// ゲームの状態
int g_nCounterGameState;					// 状態管理カウンター
bool g_bPause;								// ポーズ画面の状態

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ゲーム本編初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitGame(void)
{
	// カメラ
	InitCamera();
	// ライト
	InitLight();
#if _DEBUG
	// ライン
	InitLine();
#endif
	// メッシュドーム
	InitmeshDome();
	// メッシュフィールド
	InitmeshField();
	// ステージオブジェクト
	InitStage_obj();
	// 水面
	InitField();
	// 流れるオブジェクト
	InitObject();
	// プレイヤー
	InitPlayer();
	// エフェクト
	InitEffect();
	// パーティクル
	InitParticle();
	// 葉っぱ
	InitPetals();

	//--- UI ---//
	// スタミナゲージ
	InitGauge();
	// タイム
	InitTime();
	// カウントシーン
	InitcountScene();

	// 変数
	g_gameState = GAMESTATE_START;		// 開始状態に設定
	g_nCounterGameState = 0;			// カウンターの初期化
	g_bPause = false;					// ポーズ画面の解除
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ゲーム本編終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitGame(void)
{

	// カメラ
	UninitCamera();
	// ライト
	UninitLight();
#if _DEBUG
	// ライン
	UninitLine();
#endif
	// メッシュドーム
	UninitmeshDome();
	// メッシュフィールド
	UninitmeshField();
	// ステージオブジェクト
	UninitStage_obj();
	// 水面
	UninitField();
	// 流れるオブジェクト
	UninitObject();
	// プレイヤー
	UninitPlayer();
	// エフェクト
	UninitEffect();
	// パーティクル
	UninitParticle();
	// 葉っぱ
	UninitPetals();

	//--- UI ---//
	// スタミナゲージ
	UninitGauge();
	// タイム
	UninitTime();
	// カウントシーン
	UninitcountScene();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ゲーム本編更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdateGame(void)
{
	// フェードモードの取得
	FADE pFade = GetFade();

	if (pFade == FADE_NONE)
	{// フェードが何もしていない場合
		if (GetKeyboardTrigger(DIK_P))
		{// ポーズ画面切替
			g_bPause = g_bPause ? false : true;
			// ポーズの項目初期化
			//InitPauseItem();
			// サウンド
			PlaySound(SOUND_LABEL_SE_DECISION01);
		}
	}

	if (g_bPause == false)
	{// ポーズ外
		// カメラ
		UpdateCamera();
		// ライト
		UpdateLight();
#if _DEBUG
		// ライン
		UpdateLine();
#endif
		// メッシュドーム
		UpdatemeshDome();
		// メッシュフィールド
		UpdatemeshField();
		// ステージオブジェクト
		UpdateStage_obj();
		// 水面
		UpdateField();
		// 流れるオブジェクト
		UpdateObject();
		// プレイヤー
		UpdatePlayer();
		// エフェクト
		UpdateEffect();
		// パーティクル
		UpdateParticle();
		// 葉っぱ
		UpdatePetals();

		//--- UI ---//
		// スタミナゲージ
		UpdateGauge();
		// タイム
		UpdateTime();
		// カウントシーン
		UpdatecountScene();

		//*************************
		// ゲーム内処理
		//*************************
		switch (g_gameState)
		{
		case GAMESTATE_START:		// 開始状態

			if (pFade == FADE_NONE)
			{// フェードが何もしていない場合

				// カウンター増加
				g_nCounterGameState++;

				if (g_nCounterGameState >= 150)
				{
					// カウントシーン
					SetcountScene(COUNTSCENETYPE_START);		// スタート
					//// サウンド
					//PlaySound(SOUND_LABEL_SE_GONG_START);		// ゴング [開始]
					// 通常状態に設定
					g_gameState = GAMESTATE_NORMAL;
				}
			}
			break;

		case GAMESTATE_NORMAL:		// 通常状態
			break;

		case GAMESTATE_END:			// 終了状態

				RESULTSTATE state = GetResultState();

				if (g_nCounterGameState == 0)
			{

				if (state == RESULTSTATE_GAMECLEAR)
				{
					// カウントシーン
					SetcountScene(COUNTSCENETYPE_GOAL);		// ゴール
				}
			}

			// カウンター増加
			g_nCounterGameState++;

			if (pFade == FADE_NONE)
			{// フェードか何もしていない
				if (g_nCounterGameState >= 60)
				{// 一秒後
					g_gameState = GAMESTATE_NONE;


					if (state == RESULTSTATE_GAMECLEAR)
					{
						//--- タイムの保存 ---//
						SetRankTime(GetTime());
					}

					// フェード移行
					SetFade(MODE_RESULT);		// リザルト画面
				}
			}
			break;
		}

		// 流れるオブジェクトの位置設定
		SetPosOldObject();
	}
	else
	{// ポーズ時
		// ポーズ
		//UpdatePause();
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ゲーム本編描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawGame(void)
{
	// カメラ
	SetCamera();		// メインカメラ
	// ライン
#if _DEBUG
	DrawLine();
#endif
	// メッシュドーム
	DrawmeshDome();
	// メッシュフィールド
	DrawmeshField();
	// ステージオブジェクト
	DrawStage_obj();
	// 水面
	DrawField();
	// 流れるオブジェクト
	DrawObject();
	// プレイヤー
	DrawPlayer();
	// エフェクト
	DrawEffect();
	// パーティクル
	DrawParticle();
	// 葉っぱ
	DrawPetals();

	//--- UI ---//
	// スタミナゲージ
	DrawGauge();
	// タイム
	DrawTime();
	// カウントシーン
	DrawcountScene();

	if (g_bPause == true)
	{// ポーズ時
		// ポーズ
		//DrawPause();
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ゲームの状態設定処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SetGameState(GAMESTATE state)
{
	g_gameState = state;		// 次のゲームの状態
	g_nCounterGameState = 0;	// ゲーム状態のカウンター
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ゲームの状態取得処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
GAMESTATE GetGameState(void)
{
	return g_gameState;		// ゲームの状態を返す
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ポーズ解除処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void PauseRelease(void)
{
	g_bPause = false;		// ポーズ画面の解除
}