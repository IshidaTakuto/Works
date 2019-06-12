//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMYNUMBER	(10)		//敵の一種類の数

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;	//ゲームの状態
int g_nCounterGameState;				//状態管理のカウンター
PAUSECHECK g_bPause;					//

//=============================================================================
// ゲーム初期化処理
//=============================================================================
void InitGame(void)
{
	int nCntEnemy;

	//背景初期化処理
	InitBg();

	//スコア初期化処理
	InitScore();

	//タイマー初期化処理
	InitTimer();

	//体力初期化処理
	InitLife();

	//残機初期化処理
	InitStock();

	//ビーム初期化処理
	InitBeam();

	//弾初期化処理
	InitBullet();

	//爆発初期化処理
	InitExplosion();

	//パーティクル初期化処理
	InitParticle();

	//エフェクト初期化処理
	InitEffect();

	//プレイヤー初期化処理
	InitPlayer();

	//敵初期化処理
	InitEnemy();
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMYNUMBER; nCntEnemy++)
	{
		SetEnemy(D3DXVECTOR3(nCntEnemy * 100 + 200.0f, 100.0f, 0.0f), 0);
		SetEnemy(D3DXVECTOR3(nCntEnemy * 100 + 200.0f, 150.0f, 0.0f), 1);
		SetEnemy(D3DXVECTOR3(nCntEnemy * 100 + 200.0f, 200.0f, 0.0f), 2);
		SetEnemy(D3DXVECTOR3(nCntEnemy * 100 + 200.0f, 250.0f, 0.0f), 3);
	}

	//ポーズ初期化処理
	InitPause();

	//ポーズ背景初期化処理
	InitPausebg();

	//ポーズメニュー初期化処理
	InitMenu();

	g_gameState = GAMESTATE_NORMAL;	//通常状態に設定
	g_nCounterGameState = 0;

	g_bPause.bPause = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	//背景終了処理
	UninitBg();

	//スコア終了処理
	UninitScore();

	//タイマー終了処理
	UninitTimer();

	//体力終了処理
	UninitLife();

	//残機終了処理
	UninitStock();

	//ビーム終了処理
	UninitBeam();

	//弾終了処理
	UninitBullet();

	//爆発終了処理
	UninitExplosion();

	//パーティクル終了処理
	UninitParticle();

	//エフェクト終了処理
	UninitEffect();

	//プレイヤー終了処理
	UninitPlayer();

	//敵終了処理
	UninitEnemy();

	//ポーズ終了処理
	UninitPause();

	//ポーズ背景終了処理
	UninitPausebg();

	//ポーズメニュー終了処理
	UninitMenu();
}

//=============================================================================
// ゲーム更新処理
//=============================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true)
	{//ポーズON/OFF
		g_bPause.bPause = g_bPause.bPause ? false : true;
	}

	if (g_bPause.bPause == false)
	{
		//背景更新処理
		UpdateBg();

		//スコア更新処理
		UpdateScore();

		//タイマー更新処理
		UpdateTimer();

		//体力更新処理
		UpdateLife();

		//残機更新処理
		UpdateStock();

		//ビーム更新処理
		UpdateBeam();

		//爆発更新処理
		UpdateExplosion();

		//パーティクル更新処理
		UpdateParticle();

		//弾更新処理
		UpdateBullet();

		//エフェクト更新処理
		UpdateEffect();

		//プレイヤー更新処理
		UpdatePlayer();

		//敵更新処理
		UpdateEnemy();
	}
	else
	{
		//ポーズ更新処理
		UpdatePause();

		//ポーズ背景更新処理
		UpdatePausebg();
		
		//ポーズメニュー更新処理
		UpdateMenu();
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		break;

	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;

			int *pScore;
			pScore = GetScore();

			int *pTimer;
			pTimer = GetTimer();

			// 残りタイムをスコアに加算
			*pScore += *pTimer * 12;

			SetRankScore(*pScore);

			//画面(モード)の設定
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//=============================================================================
// ゲーム描画処理
//=============================================================================
void DrawGame(void)
{
	//背景描画処理
	DrawBg();

	//スコア描画処理
	DrawScore();

	//タイマー描画処理
	DrawTimer();

	//体力描画処理
	DrawLife();

	//残機描画処理
	DrawStock();

	//ビーム描画処理
	DrawBeam();

	//弾描画処理
	DrawBullet();

	//爆発描画処理
	DrawExplosion();

	//パーティクル描画処理
	DrawParticle();

	//エフェクト描画処理
	DrawEffect();

	//プレイヤー描画処理
	DrawPlayer();

	//敵描画処理
	DrawEnemy();

	if (g_bPause.bPause == true)
	{
		//ポーズ描画処理
		DrawPause();

		//ポーズ背景描画処理
		DrawPausebg();

		//ポーズメニュー描画処理
		DrawMenu();
	}
}

//=============================================================================
// ゲームの状態の設定
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//=============================================================================
// ゲームの状態の取得
//=============================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//=============================================================================
// ポーズ状態の取得
//=============================================================================
PAUSECHECK *GetPauseMenu(void)
{
	return &g_bPause;
}