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
int g_nCntBlock;

BlockInfo g_aBlockInfo[BLOCK_NUM]
{
	{ D3DXVECTOR3(20.0f, 500.0f, 0.0f), BLOCK_SIZE * 2, BLOCK_SIZE * 1.6f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_NORMAL },
	{ D3DXVECTOR3(200.0f, 550.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_NORMAL },
	{ D3DXVECTOR3(350.0f, 500.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_NORMAL },
	{ D3DXVECTOR3(650.0f, 580.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_NORMAL },
	{ D3DXVECTOR3(1650.0f, 500.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_NORMAL },
	{ D3DXVECTOR3(2500, 480.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 300.0f, 0.0f), BLOCKTYPE_VERTICAL },
	{ D3DXVECTOR3(4500.0f, 480.0f, 0.0f), BLOCK_SIZE * 2, BLOCK_SIZE * 2, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_GOAL },

	{ D3DXVECTOR3(SCREEN_HALF - 100, 200.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 300.0f, 0.0f), BLOCKTYPE_VERTICAL },
	{ D3DXVECTOR3(3200 - (BLOCK_SIZE * 4), 200.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 380.0f, 0.0f), BLOCKTYPE_VERTICAL },
	{ D3DXVECTOR3(3500 - (BLOCK_SIZE * 4), 480.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 300.0f, 0.0f), BLOCKTYPE_VERTICAL },
	{ D3DXVECTOR3(3800 - (BLOCK_SIZE * 4), 200.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 380.0f, 0.0f), BLOCKTYPE_VERTICAL },
	{ D3DXVECTOR3(SCREEN_WIDTH - (4 * BLOCK_SIZE), 500.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 500.0f, 0.0f), BLOCKTYPE_FALL },
	{ D3DXVECTOR3(4000.0f, 250.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 250.0f, 0.0f), BLOCKTYPE_FALL },
	{ D3DXVECTOR3(2000, 250.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 250.0f, 0.0f), BLOCKTYPE_FALL },
	{ D3DXVECTOR3(1400.0f, 450.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 450.0f, 0.0f), BLOCKTYPE_FALL }
};

ItemInfo g_aItemInfo[ITEM_NUM]
{
	{ D3DXVECTOR3(150, 450.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(1500, 500.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(2500, 400.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(3600, 350.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(850, 450.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(2800, 500.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(4150, 450.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(3150, 550.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(1850, 500.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(3850, 450.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(2150, 400.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(3400, 450.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(1100.0f, 600.0f, 0.0f), ITEMTYPE_EGG },
	{ D3DXVECTOR3(400.0f, 500.0f, 0.0f), ITEMTYPE_DIRTY },
	{ D3DXVECTOR3(1600.0f, 600.0f, 0.0f), ITEMTYPE_DIRTY },
	{ D3DXVECTOR3(2400.0f, 300.0f, 0.0f), ITEMTYPE_DIRTY },
	{ D3DXVECTOR3(4100.0f, 600.0f, 0.0f), ITEMTYPE_DIRTY },
};

GeneratorInfo g_aGeneratorInfo[GENERATOR_NUM]
{
	{ D3DXVECTOR3(SCREEN_WIDTH + 60, 660.0f, 0.0f), BLOCKTYPE_BELT_LEFT, 200.0f, 100.0f, D3DXVECTOR3(0.0f,658.0f,0.0f), 120 },
	{ D3DXVECTOR3(-150.0f, 125.0f, 0.0f), BLOCKTYPE_HORIZONTAL, BLOCK_SIZE + 20, BLOCK_SIZE, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), 270 },
	{ D3DXVECTOR3(-150.0f, 250.0f, 0.0f), BLOCKTYPE_HORIZONTAL, BLOCK_SIZE + 20, BLOCK_SIZE, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), 450 },
	{ D3DXVECTOR3(-150.0f, 375.0f, 0.0f), BLOCKTYPE_HORIZONTAL, BLOCK_SIZE + 20, BLOCK_SIZE, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), 400 },
	{ D3DXVECTOR3(-150.0f, 500.0f, 0.0f), BLOCKTYPE_HORIZONTAL, BLOCK_SIZE + 20, BLOCK_SIZE, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), 350 },
};
//=============================================================================
// ゲーム初期化処理
//=============================================================================
void InitGame(void)
{
	g_nCntBlock = 0;

	// 背景初期化処理
	InitBg();

	// ブロック初期化処理
	InitBlock();
	for (int nCntBlock = 0; nCntBlock < sizeof g_aBlockInfo / sizeof(BlockInfo); nCntBlock++)
	{
		SetBlock(g_aBlockInfo[nCntBlock].pos, g_aBlockInfo[nCntBlock].fWidth, g_aBlockInfo[nCntBlock].fHeight, g_aBlockInfo[nCntBlock].des, g_aBlockInfo[nCntBlock].type);
	}

	// ジェネレータ初期化処理
	InitGenerator();
	for (int nCntGenerator = 0; nCntGenerator < sizeof g_aGeneratorInfo / sizeof(GeneratorInfo); nCntGenerator++)
	{
		SetGenerator(g_aGeneratorInfo[nCntGenerator].pos, 
					g_aGeneratorInfo[nCntGenerator].type, 
					g_aGeneratorInfo[nCntGenerator].fWidth, 
					g_aGeneratorInfo[nCntGenerator].fHeight, 
					g_aGeneratorInfo[nCntGenerator].des, 
					g_aGeneratorInfo[nCntGenerator].interval);
	}

	// アイテム初期化処理
	InitItem();
	for (int nCntItem = 0; nCntItem < sizeof g_aItemInfo / sizeof(ItemInfo); nCntItem++)
	{
		SetItem(g_aItemInfo[nCntItem].pos, g_aItemInfo[nCntItem].type);
	}

	// スコア初期化処理
	InitScore();

	// タイマー初期化処理
	InitTimer();

	// 残機初期化処理
	InitStock();

	// プレイヤー初期化処理
	InitPlayer();

	// デストロイヤー初期化処理
	InitDestroyer();

	// エフェクト初期化処理
	InitEffect();

	// 血初期化処理
	InitBlood();

	// ポーズ初期化処理
	InitPause();

	// ポーズ背景初期化処理
	InitPausebg();

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
	// 背景終了処理
	UninitBg();

	// ブロック終了処理
	UninitBlock();

	// ジェネレータ終了処理
	UninitGenerator();

	//スコア終了処理
	UninitScore();

	// アイテム終了処理
	UninitItem();

	// タイマー終了処理
	UninitTimer();

	// 残機終了処理
	UninitStock();

	// プレイヤー終了処理
	UninitPlayer();

	// デストロイヤー終了処理
	UninitDestroyer();

	// エフェクト終了処理
	UninitEffect();

	// 血終了処理
	UninitBlood();

	// ポーズ終了処理
	UninitPause();

	// ポーズ背景終了処理
	UninitPausebg();

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
		if (g_bPause.bPause == true)
		{
			PlaySound(SOUND_LABEL_SE_PAUSEON);
		}
		else
		{
			PlaySound(SOUND_LABEL_SE_PAUSEOFF);
		}
	}

	g_nCntBlock++;

	if (g_bPause.bPause == false)
	{
		// 背景更新処理
		UpdateBg();

		// ブロック更新処理
		UpdateBlock();
		if (g_gameState == GAMESTATE_NORMAL && g_nCntBlock % 150 == 0)
		{
			SetBlock(D3DXVECTOR3((rand() % SCREEN_WIDTH) * 1.0f, -100.0f, 0.0f), BLOCK_SIZE, BLOCK_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_ENEMY);
		}

		// ジェネレータ更新処理
		UpdateGenerator();

		//スコア更新処理
		UpdateScore();

		// アイテム更新処理
		UpdateItem();

		// タイマー更新処理
		UpdateTimer();

		// 残機更新処理
		UpdateStock();

		// プレイヤー更新処理
		UpdatePlayer();

		// デストロイヤー更新処理
		UpdateDestroyer();

		// エフェクト更新処理
		UpdateEffect();

		// 血更新処理
		UpdateBlood();
	}
	else
	{
		// ポーズ更新処理
		UpdatePause();

		// ポーズ背景更新処理
		UpdatePausebg();
		
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

			// 画面(モード)の設定
			SetFade(MODE_GAMEOVER);
		}
		break;

	case GAMESTATE_CLEAR:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;

			AddScore(30000);

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
	// 背景描画処理
	DrawBg();

	// ブロック描画処理
	DrawBlock();

	// ジェネレータ描画処理
	DrawGenerator();

	// スコア描画処理
	DrawScore();

	// アイテム描画処理
	DrawItem();

	// タイマー描画処理
	DrawTimer();

	// 残機描画処理
	DrawStock();

	// プレイヤー描画処理
	DrawPlayer();

	// デストロイヤー描画処理
	DrawDestroyer();

	// エフェクト描画処理
	DrawEffect();

	// 血描画処理
	DrawBlood();

	if (g_bPause.bPause == true)
	{
		// ポーズ描画処理
		DrawPause();

		// ポーズ背景描画処理
		DrawPausebg();

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