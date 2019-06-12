//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "game.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "meshDome.h"
#include "meshField.h"
#include "field.h"
#include "player.h"
#include "effect.h"
#include "gauge.h"
#include "particle.h"
#include "Gamepad.h"

#include "sound.h"
#include "UI.h"

#if _DEBUG
#include "line.h"
#endif

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ゲーム本編初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitTutorial(void)
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
	// フィールド
	InitField();
	// プレイヤー
	InitPlayer();
	// エフェクト
	InitEffect();
	// パーティクル
	InitParticle();
	// UI
	InitUI();

	//--- UI ---//
	// スタミナゲージ
	InitGauge();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ゲーム本編終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitTutorial(void)
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
	// フィールド
	UninitField();
	// プレイヤー
	UninitPlayer();
	// エフェクト
	UninitEffect();
	// パーティクル
	UninitParticle();

	//--- UI ---//
	// スタミナゲージ
	UninitGauge();
	// UI
	UninitUI();

}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ゲーム本編更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdateTutorial(void)
{
	// フェードモードの取得
	FADE pFade = GetFade();

	if (pFade == FADE_NONE)
	{// フェードが何もしていない場合
		if (GetKeyboardTrigger(DIK_RETURN) || GetGamapadKeyTrigger(ELECOM_START))
		{// 画面遷移
			SetFade(MODE_GAME);
		}
	}

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
	// フィールド
	UpdateField();
	// プレイヤー
	UpdatePlayer();
	// エフェクト
	UpdateEffect();
	// パーティクル
	UpdateParticle();

	//--- UI ---//
	// スタミナゲージ
	UpdateGauge();
	// UI
	UpdateUI();

}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ゲーム本編描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawTutorial(void)
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
	// プレイヤー
	DrawPlayer();
	// フィールド
	DrawField();
	// エフェクト
	DrawEffect();
	// パーティクル
	DrawParticle();

	//--- UI ---//
	// スタミナゲージ
	DrawGauge();
	// UI
	DrawUI();

}
