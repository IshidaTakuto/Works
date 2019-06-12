//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "game.h"
#include "fade.h"
#include "input.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "meshField.h"
#include "meshCylinder.h"
#include "snow.h"
#include "rain.h"
#include "particle.h"
#include "pause.h"
#include "pausemenu.h"
#include "spiral.h"
#include "magic.h"
#include "petals.h"
#include "timer.h"
#include "score.h"
#include "ranking.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMYNUMBER	(10)		// 敵の一種類の数
#define BLOCK_NUM		(15)		// ブロックの数
#define ITEM_NUM		(17)		// アイテムの数
#define GENERATOR_NUM	(5)

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;	// ゲームの状態
int g_nCounterGameState;				// 状態管理のカウンター
PAUSECHECK g_bPause;

MeshWallInfo g_aMeshWallInfo[12]
{
	{ D3DXVECTOR3(0.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), 0, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(0.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 0, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(2500.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), 1, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(2500.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 1, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(5000.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), 2, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(5000.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 2, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(7500.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), 3, WALL_WIDTH, WALL_HEIGHT },
	{ D3DXVECTOR3(7500.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 3, WALL_WIDTH, WALL_HEIGHT },

	{ D3DXVECTOR3(0.0f, 300.0f, 200.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), 4, WALL_WIDTH, WALL_HEIGHT * 2 },
	{ D3DXVECTOR3(2500.0f, 300.0f, 200.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), 4, WALL_WIDTH, WALL_HEIGHT * 2 },
	{ D3DXVECTOR3(5000.0f, 300.0f, 200.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), 4, WALL_WIDTH, WALL_HEIGHT * 2 },
	{ D3DXVECTOR3(7500.0f, 300.0f, 200.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), 4, WALL_WIDTH, WALL_HEIGHT * 2 }
};

ModelInfo g_aModelInfo[61]
{
	{ D3DXVECTOR3(-1250.0f,0.0f,-100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(-1250.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(-1250.0f,0.0f,100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(-900.0f,0.0f,-100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(-550.0f,0.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(-50.0f,0.0f,50.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(550.0f,0.0f,100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(1050.0f,0.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 0, MODELTYPE_OBJECT },

	{ D3DXVECTOR3(1550.0f,0.0f,-120.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(1850.0f,0.0f,70.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(2300.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(2850.0f,0.0f,-90.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(3300.0f,0.0f,150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(3600.0f,0.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 1, MODELTYPE_OBJECT },

	{ D3DXVECTOR3(4000.0f,0.0f,100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(4750.0f,0.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(5300.0f,0.0f,150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(5600.0f,0.0f,10.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(6100.0f,0.0f,-130.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2, MODELTYPE_OBJECT },

	{ D3DXVECTOR3(6800.0f,0.0f,30.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 3, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(7600.0f,0.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 3, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(8300.0f,0.0f,100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 3, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(8750.0f,0.0f,150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 3, MODELTYPE_OBJECT },
	{ D3DXVECTOR3(8750.0f,0.0f,-150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 3, MODELTYPE_OBJECT },

	{ D3DXVECTOR3(-500.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(-100.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(100.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(260.0f,60.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(530.0f,25.0f,50.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(950.0f,25.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(1250.0f,25.0f,120.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(1500.0f,60.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(1750.0f,25.0f,30.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(1980.0f,60.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(2250.0f,25.0f,150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(2500.0f,60.0f,50.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(2750.0f,25.0f,-130.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(2980.0f,25.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(3180.0f,25.0f,10.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(3360.0f,25.0f,90.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(3650.0f,60.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(3920.0f,25.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(4240.0f,25.0f,40.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(4590.0f,25.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(4750.0f,60.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(4960.0f,25.0f,-80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(5350.0f,25.0f,30.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(5507.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(5950.0f,60.0f,-100.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(6350.0f,25.0f,-150.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(6590.0f,25.0f,10.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(6750.0f,25.0f,-20.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(6900.0f,25.0f,80.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(7050.0f,25.0f,60.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(7350.0f,25.0f,-40.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(7500.0f,60.0f,-120.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(7850.0f,25.0f,70.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(8050.0f,60.0f,90.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(8200.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(8400.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM },
	{ D3DXVECTOR3(8600.0f,25.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 4, MODELTYPE_ITEM }

};

//=============================================================================
// ゲーム初期化処理
//=============================================================================
void InitGame(void)
{
	// メッシュシリンダーの初期化処理
	//InitMeshCylinder();

	// メッシュフィールドの初期化処理
	InitMeshField();

	// メッシュウォールの初期化処理
	for (int nCntWall = 0; nCntWall < sizeof g_aMeshWallInfo / sizeof(MeshWallInfo); nCntWall++)
	{
		InitMeshWall(g_aMeshWallInfo[nCntWall].pos, g_aMeshWallInfo[nCntWall].rot, g_aMeshWallInfo[nCntWall].nType, g_aMeshWallInfo[nCntWall].fWidth, g_aMeshWallInfo[nCntWall].fHeight);
	}

	// 弾初期化処理
	InitBullet();

	InitSpiral();
	for (int nCntSpiral = 0; nCntSpiral < SPIRAL_RING; nCntSpiral++)
	{
		SetSpiral(D3DXVECTOR3(8750.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SPIRALTYPE_RING);
	}

	// 影の初期化処理
	InitShadow();

	// パーティクルの初期化処理
	InitParticle();

	// プレイヤーの初期化処理
	InitPlayer();

	// モデルの初期化処理
	InitModel();
	for (int nCntModelInfo = 0; nCntModelInfo < sizeof g_aModelInfo / sizeof(ModelInfo); nCntModelInfo++)
	{
		SetModel(g_aModelInfo[nCntModelInfo].pos, g_aModelInfo[nCntModelInfo].rot, g_aModelInfo[nCntModelInfo].nType, g_aModelInfo[nCntModelInfo].type);
	}

	// 爆発初期化処理
	InitExplosion();

	// エフェクト初期化処理
	InitEffect();
	SetEffect(D3DXVECTOR3(0.0f, 60.0f, 400.0f), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), 100.0f, 0, EFFECTSTATE_MOON);
	SetEffect(D3DXVECTOR3(0.0f, 60.0f, 400.0f), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), 130.0f, 0, EFFECTSTATE_NONE);

	// 魔法初期化処理
	InitMagic();

	// 花びら初期化処理
	InitPetals();

	// 雪の初期化処理
	InitSnow();

	// 雨の初期化処理
	InitRain();

	// タイマー初期化処理
	InitTimer();

	// スコア初期化処理
	InitScore();

	// ポーズ初期化処理
	InitPause();

	// ポーズメニュー初期化処理
	InitPmenu();

	g_gameState = GAMESTATE_NORMAL;	// 通常状態に設定
	g_nCounterGameState = 0;

	g_bPause.bPause = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// メッシュシリンダーの終了処理
	//UninitMeshCylinder();

	// メッシュフィールドの終了処理
	UninitMeshField();

	// メッシュウォールの終了処理
	UninitMeshWall();

	// 弾終了処理
	UninitBullet();

	// プレイヤーの終了処理
	UninitPlayer();

	// モデルの終了処理
	UninitModel();

	// ビルボードの終了処理
	UninitBullet();

	// 影の終了処理
	UninitShadow();

	// 爆発終了処理
	UninitExplosion();

	// エフェクト終了処理
	UninitEffect();

	// 魔法の終了処理
	UninitMagic();

	// パーティクルの終了処理
	UninitParticle();

	UninitSpiral();

	// 花びら終了処理
	UninitPetals();

	// 雪の終了処理
	UninitSnow();

	// 雨の終了処理
	UninitRain();

	// タイマー終了処理
	UninitTimer();

	// スコア終了処理
	UninitScore();

	// ポーズ終了処理
	UninitPause();

	// ポーズメニュー終了処理
	UninitPmenu();
}

//=============================================================================
// ゲーム更新処理
//=============================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true)
	{// ポーズON/OFF
		g_bPause.bPause = g_bPause.bPause ? false : true;
	}

	if (g_bPause.bPause == false)
	{
		// メッシュシリンダーの更新処理
		//UpdateMeshCylinder();

		// メッシュフィールドの更新処理
		UpdateMeshField();

		// メッシュウォールの更新処理
		UpdateMeshWall();

		// 弾更新処理
		UpdateBullet();

		// プレイヤーの更新処理
		UpdatePlayer();

		UpdateModel();

		// ビルボードの更新処理
		UpdateBullet();

		// 影の更新処理
		UpdateShadow();

		// 爆発更新処理
		UpdateExplosion();

		// エフェクト更新処理
		UpdateEffect();

		// 魔法の更新処理
		UpdateMagic();

		// 螺旋の更新処理
		UpdateSpiral();

		// 花びら更新処理
		UpdatePetals(PETALSTYPE_SAKURA);

		// タイマー更新処理
		UpdateTimer();

		// スコア更新処理
		UpdateScore();

		// 雪の更新処理
		UpdateSnow();

		// 雨の更新処理
		UpdateRain();

		// パーティクルの更新処理
		UpdateParticle();
	}
	else
	{
		// ポーズ更新処理
		UpdatePause();

		// ポーズメニュー更新処理
		UpdatePmenu();
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

			int nScore = GetScore();

			SetRanking(nScore);

			// 画面(モード)の設定
			SetFade(MODE_GAMEOVER);
		}
		break;

	case GAMESTATE_CLEAR:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			int nScore = GetScore();

			SetRanking(nScore);

			g_gameState = GAMESTATE_NONE;

			// 画面(モード)の設定
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
	// メッシュフィールドの描画処理
	DrawMeshField();

	// メッシュウォールの描画処理
	DrawMeshWall();

	// メッシュシリンダーの描画処理
	//DrawMeshCylinder();

	// 影の描画処理
	DrawShadow();

	// 弾描画処理
	DrawBullet();

	// 爆発描画処理
	DrawExplosion();

	// エフェクト描画処理
	DrawEffect();

	// 螺旋の描画処理
	DrawSpiral();

	// 魔法の描画処理
	DrawMagic();

	// モデルの描画処理
	DrawModel();

	// プレイヤーの描画処理
	DrawPlayer();

	// ビルボードの描画処理
	DrawBullet();

	// パーティクルの描画処理
	DrawParticle();

	// 花びら描画処理
	DrawPetals();

	// 雪の更新処理
	DrawSnow();

	// 雨の更新処理
	DrawRain();

	// タイマー描画処理
	DrawTimer();

	// スコア描画処理
	DrawScore();

	if (g_bPause.bPause == true)
	{
		// ポーズ描画処理
		DrawPause();

		// ポーズメニュー描画処理
		DrawPmenu();
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
