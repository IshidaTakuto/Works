//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "input.h"
#include "ranking.h"
#include "meshbg.h"
#include "score.h"
#include "killLog.h"
#include "meshField.h"
#include "shadow.h"
#include "player.h"
#include "enemy.h"
#include "object.h"
#include "object2.h"
#include "object3.h"
#include "objectnot.h"
#include "pause.h"
#include "petals.h"
#include "Blood.h"
#include "wall.h"
#include "particle.h"
#include "rain.h"
#include "particle2D.h"
#include "effect.h"
#include "clear.h"
#include "life.h"
#include "timer.h"
#include "combo.h"
#include "remnantsEnemy.h"
#include "load.h"
#include "gauge.h"
#include "dragon.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_NUM_APPEAR	(10)	// 出現させる最大数

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;		//ゲームの状態
int g_nCounterGameState;					//状態管理のカウンター
PAUSECHECK g_bPause;						//ポーズ
STAGENUM g_StageState;						//ステージ状態
int	g_nCntSetStage;							//ステージセットカウンタ
bool g_bStageSwat;							// ステージが変わる始めと終わり
EnemyInfo g_aEnemyInfo[30];					// 敵の情報

//=============================================================================
// ゲーム初期化処理
//=============================================================================
void InitGame(void)
{
	g_nCounterGameState = 0;
	//開始ステージ
	g_StageState = STAGENUM_1;			// ステージ状態

	//カメラの初期化
	InitCamera();

	// キルログ初期化処理
	InitKillLog();

	//背景の初期化処理
	InitMeshBg();

	// フィールド初期化処理
	InitMeshField();

	// パーティクル初期化処理
	InitParticle();

	// スコア初期化処理
	InitScore();

	// 影初期化処理
	InitShadow();

	//プレイヤー初期化処理
	InitPlayer();
	InitDragon();

	//敵の初期化処理
	InitEnemy();

	// 血の初期化処理
	InitBlood();

	//オブジェクトの初期化処理
	InitObject();
	InitObject2();
	InitObject3();
	InitObjectNot();

	// 花びら初期化処理
	InitPetals();

	// 壁初期化処理
	InitWall();

	// ポーズ初期化処理
	InitPause();

	if (g_StageState == STAGENUM_1)
	{// 雨初期化処理
		InitRain();
	}

	// 2Dパーティクル初期化処理
	InitParticle2D();

	// エフェクト初期化処理
	InitEffect();

	// クリアメッセージ初期化処理
	InitClear();

	// ライフ初期化処理
	InitLife();

	// タイマー初期化処理
	InitTimer();

	// コンボ初期化処理
	InitCombo(D3DXVECTOR3(200, 200, 0));

	// 敵残数初期化処理
	InitRemnants();

	// 変身ゲージ初期化処理
	InitGauge();

	// マップ移動初期化処理
	InitLoad();

	//壁の配置
	//上
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 870.0f), D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f));
	//右
	SetWall(D3DXVECTOR3(870.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f));
	//下
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -870.0f), D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f));
	//左
	SetWall(D3DXVECTOR3(-870.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f));

	//通常状態に設定
	g_gameState = GAMESTATE_NORMAL;

	//カウンタ初期化
	g_nCntSetStage = 0;

	g_bPause.bPause = false;
	g_bStageSwat = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	//カメラの終了処理
	UninitCamera();

	// キルログ終了処理
	UninitKillLog();

	//背景の終了処理
	UninitMeshBg();

	// フィールド終了処理
	UninitMeshField();

	// スコア終了処理
	UninitScore();

	// 影終了処理
	UninitShadow();

	// プレイヤー終了処理
	UninitPlayer();
	UninitDragon();

	// 敵の終了処理
	UninitEnemy();

	// パーティクル終了処理
	UninitParticle();

	// 2Dパーティクル終了処理
	UninitParticle2D();

	//オブジェクトの終了処理
	UninitObject();
	UninitObject2();
	UninitObject3();
	UninitObjectNot();

	// 花びら終了処理
	UninitPetals();

	// 壁終了処理
	UninitWall();

	// モデル終了処理
	UninitScore();

	// 血の終了処理
	UninitBlood();

	// ポーズ終了処理
	UninitPause();

	if (g_StageState == STAGENUM_1)
	{// 雨終了処理
		UninitRain();
	}

	// エフェクト終了処理
	UninitEffect();

	// クリアメッセージ終了処理
	UninitClear();

	// ライフ終了処理
	UninitLife();

	// タイマー終了処理
	UninitTimer();

	// コンボ終了処理
	UninitCombo();

	// 敵残数終了処理
	UninitRemnants();

	// 変身ゲージ終了処理
	UninitGauge();

	// マップ移動終了処理
	UninitLoad();
}

//=============================================================================
// ゲーム更新処理
//=============================================================================
void UpdateGame(void)
{
	g_bStageSwat;
	bool bFade = GetFadeNow();
	LOADFADE loadFade = GetLoadFade();

	if (false == g_bStageSwat && false == bFade && LOADFADE_NONE == loadFade)
	{// ステージ切り替え中などはポーズ無効
		if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadTrigger(DIJS_BUTTON_7, 0) == true)
		{// ポーズON/OFF
			PlaySound(SOUND_LABEL_PAUSE);
			g_bPause.bPause = g_bPause.bPause ? false : true;
		}
	}

	if (g_bPause.bPause == false)
	{
		//カメラの更新処理
		UpdateCamera();

		// キルログ更新処理
		UpdateKillLog();

		// フィールド更新処理
		UpdateMeshBg();

		// フィールド更新処理
		UpdateMeshField();

		// スコア更新処理
		UpdateScore();

		// 影更新処理
		UpdateShadow();

		// ステージ設定
		SetStage();

		// プレイヤー更新処理
		UpdatePlayer();
		UpdateDragon();

		// 敵の更新処理
		UpdateEnemy();

		// パーティクル更新処理
		UpdateParticle();

		// 2Dパーティクル更新処理
		UpdateParticle2D();

		// 血の更新処理
		UpdateBlood();

		//オブジェクトの更新処理
		UpdateObject();
		UpdateObject2();
		UpdateObject3();
		UpdateObjectNot();

		// 花びら更新処理
		UpdatePetals();

		// 壁終了処理
		UpdateWall();

		if (g_StageState == STAGENUM_1)
		{// 雨更新処理
			UpdateRain();
		}

		// エフェクト更新処理
		UpdateEffect();

		// クリアメッセージ更新処理
		UpdateClear();

		// コンボ更新処理
		UpdateCombo();

		// 敵残数更新処理
		UpdateRemnants();

		// タイマー更新処理
		UpdateTimer();

		// ライフ更新処理
		UpdateLife();

		// 変身ゲージ更新処理
		UpdateGauge();

		// マップ移動更新処理
		UpdateLoad();

		// 敵の生成
		for (int nCntEnemyInfo = 0; nCntEnemyInfo < sizeof g_aEnemyInfo / sizeof(EnemyInfo); nCntEnemyInfo++)
		{
			int nNumEnemy = GetNumEnemy();	// 使われている敵の数の取得
			if (ENEMY_NUM_APPEAR > nNumEnemy)
			{// 一定の数まで敵を出現させる
				if (!g_aEnemyInfo[nCntEnemyInfo].bUse)
				{// 使われていない情報を使用
					SetEnemy(g_aEnemyInfo[nCntEnemyInfo].pos, g_aEnemyInfo[nCntEnemyInfo].nType);
					g_aEnemyInfo[nCntEnemyInfo].bUse = true;
				}
			}
			else
			{// 一定数になった
				break;
			}
		}
	}
	else
	{
		// ポーズ更新処理
		UpdatePause();
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
			SetFadeMode(MODE_GAMEOVER);
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
			SetFadeMode(MODE_RESULT);
		}
		break;
	}
}

//=============================================================================
// ゲーム描画処理
//=============================================================================
void DrawGame(void)
{
	//カメラの描画処理
	SetCamera(CAMERATYPE_GAME);

	// フィールド描画処理
	DrawMeshField();

	// フィールド描画処理
	//DrawMeshBg();

	// 血の描画処理
	DrawBlood();

	// 影描画処理
	DrawShadow();

	//プレイヤー描画処理
	PlayerType type = GetPlayerType();
	if (type == TYPE_HUMAN)
	{
		DrawPlayer();
	}
	else
	{
		DrawDragon();
	}

	// 敵の描画処理
	DrawEnemy();

	//オブジェクトの描画処理
	DrawObject();
	DrawObject2();
	DrawObject3();
	DrawObjectNot();

	// 壁描画処理
	DrawWall();

	// 花びら描画処理
	DrawPetals();

	// パーティクル描画処理
	DrawParticle();

	// 2Dパーティクル描画処理
	DrawParticle2D();

	// エフェクト描画処理
	DrawEffect();

	if (g_StageState == STAGENUM_1)
	{// 雨描画処理
		DrawRain();
	}

	// タイマー描画処理
	DrawTimer();

	// スコア描画処理
	DrawScore();

	// キルログ描画処理
	DrawKillLog();

	// クリアメッセージ描画処理
	DrawClear();

	// ライフ描画処理
	DrawLife();

	// コンボ描画処理
	DrawCombo();

	// 敵残数描画処理
	DrawRemnants();

	// 変身ゲージ描画処理
	DrawGauge();

	// マップ移動描画処理
	DrawLoad();

	if (g_bPause.bPause == true)
	{
		// ポーズ描画処理
		DrawPause();
	}
}

//=============================================================================
// ゲームの状態の設定
//=============================================================================
void SetGameState(GAMESTATE state)
{
	if (g_StageState != state)
	{
		DeletePlayer();
	}
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
// マップの取得
//=============================================================================
STAGENUM GetNumState(void)
{
	return g_StageState;
}

//=============================================================================
// ポーズ状態の取得
//=============================================================================
PAUSECHECK *GetPauseMenu(void)
{
	return &g_bPause;
}

//=========================================================================================================================
// ゲーム設定処理
//=========================================================================================================================
void SetStageState(STAGENUM stage)
{

	if (g_StageState != stage)
	{
		DeletePlayer();		//プレイヤーの削除
		DeleteEnemy();		//敵の削除
		DeleteObject();		//木の削除
		DeleteObject2();	//岩の削除
		DeleteObject3();	//ビルの削除
		DeleteNotObject();	//木の草の削除
		DeleteBlood();		//血の削除
		DeleteCamera();


		// エフェクトの削除
		DeletePetals();
		DeleteParticle();
		DeleteEffect();

		// 時間のリセット
		ResetTimer();

		// 敵残数のリセット
		SetRemnants(0);
	}

	// 状態を記憶
	g_StageState = stage;
	g_nCntSetStage = 0;
}

//=========================================================================================================================
// マップの設定
//=========================================================================================================================
void SetStage(void)
{
	if (g_nCntSetStage == 0)
	{
		//=====================================
		//	          ステージ１
		//=====================================
		if (g_StageState == STAGENUM_1)
		{//設置物
			// 前のステージのBGMを止める
			StopSound(SOUND_LABEL_STAGE2);
			StopSound(SOUND_LABEL_STAGE3);

			// ステージ1のBGM
			PlaySound(SOUND_LABEL_STAGE1);
			PlaySound(SOUND_LABEL_RAIN);

		 //================================================================================
		 // 当たり判定のあるオブジェクト
		 //================================================================================
			for (int nCount = 0; nCount < 5; nCount++)
			{
				//右壁
				SetObject(D3DXVECTOR3(800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObject(D3DXVECTOR3(800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//左壁
				SetObject(D3DXVECTOR3(-800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObject(D3DXVECTOR3(-800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//上壁
				SetObject(D3DXVECTOR3(nCount * 200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObject(D3DXVECTOR3(nCount * -200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//下壁
				SetObject(D3DXVECTOR3(nCount * -200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObject(D3DXVECTOR3(nCount * 200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}//40

			 //右上角
			SetObject(D3DXVECTOR3(640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(660.0f, 0.0f, 540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//左下角
			SetObject(D3DXVECTOR3(-640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-660.0f, 0.0f, -540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//右下角
			SetObject(D3DXVECTOR3(640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(660.0f, 0.0f, -540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//左上角
			SetObject(D3DXVECTOR3(-640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-660.0f, 0.0f, 540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//マップに散りばめるモデル
			SetObject(D3DXVECTOR3(-120.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(-320.0f, 0.0f, -240.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(340.0f, 0.0f, 40.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject(D3DXVECTOR3(160.0f, 0.0f, -320.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//現在個数 : 72

			//================================================================================
			//当たり判定のないオブジェクト
			//================================================================================
			for (int nCount = 0; nCount < 5; nCount++)
			{
				//右壁
				SetObjectNot(D3DXVECTOR3(800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObjectNot(D3DXVECTOR3(800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//左壁
				SetObjectNot(D3DXVECTOR3(-800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObjectNot(D3DXVECTOR3(-800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//上壁
				SetObjectNot(D3DXVECTOR3(nCount * 200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObjectNot(D3DXVECTOR3(nCount * -200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				//下壁
				SetObjectNot(D3DXVECTOR3(nCount * -200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetObjectNot(D3DXVECTOR3(nCount * 200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}//40

			 //右上角
			SetObjectNot(D3DXVECTOR3(640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(660.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//左下角
			SetObjectNot(D3DXVECTOR3(-640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-660.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//右下角
			SetObjectNot(D3DXVECTOR3(640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(660.0f, 0.0f, -540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//左上角
			SetObjectNot(D3DXVECTOR3(-640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-660.0f, 0.0f, 540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//マップに散りばめるモデル
			SetObjectNot(D3DXVECTOR3(-120.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(-320.0f, 0.0f, -240.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(340.0f, 0.0f, 40.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObjectNot(D3DXVECTOR3(160.0f, 0.0f, -320.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//現在個数 : 72

			//右上角
			//SetObject(D3DXVECTOR3(640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//左下角
			//SetObject(D3DXVECTOR3(-640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//右下角
			//SetObject(D3DXVECTOR3(640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//左上角
			//SetObject(D3DXVECTOR3(-640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//==================================
			//             敵の配置
			//==================================
			g_aEnemyInfo[0]  = { D3DXVECTOR3(600.0f, 50.0f, 600.0f), 0		};
			g_aEnemyInfo[1]  = { D3DXVECTOR3(-600.0f, 50.0f, 600.0f), 0		};
			g_aEnemyInfo[2]  = { D3DXVECTOR3(600.0f, 50.0f, -600.0f), 0		};
			g_aEnemyInfo[3]  = { D3DXVECTOR3(-600.0f, 50.0f, -600.0f), 0	};
			g_aEnemyInfo[4]  = { D3DXVECTOR3(300.0f, 50.0f, -100.0f), 0		};
			g_aEnemyInfo[5]  = { D3DXVECTOR3(200.0f, 50.0f, 300.0f), 0		};
			g_aEnemyInfo[6]  = { D3DXVECTOR3(500.0f, 50.0f, -400.0f), 0		};
			g_aEnemyInfo[7]  = { D3DXVECTOR3(-150.0f, 50.0f, -200.0f), 0	};
			g_aEnemyInfo[8]  = { D3DXVECTOR3(-300.0f, 50.0f, 200.0f), 0		};
			g_aEnemyInfo[9]  = { D3DXVECTOR3(50.0f, 50.0f, -100.0f), 0		};
			g_aEnemyInfo[10] = { D3DXVECTOR3(-200.0f, 50.0f, 500.0f), 0		};
			g_aEnemyInfo[11] = { D3DXVECTOR3(-300.0f, 50.0f, 450.0f), 0		};
			g_aEnemyInfo[12] = { D3DXVECTOR3(0.0f, 50.0f, -300.0f), 0		};
			g_aEnemyInfo[13] = { D3DXVECTOR3(0.0f, 50.0f, 500.0f), 0		};
			g_aEnemyInfo[14] = { D3DXVECTOR3(500.0f, 50.0f, 0.0f), 0		};
			g_aEnemyInfo[15] = { D3DXVECTOR3(-200.0f, 50.0f, 50.0f), 0		};
			g_aEnemyInfo[16] = { D3DXVECTOR3(-500.0f, 50.0f, -100.0f), 0	};
			g_aEnemyInfo[17] = { D3DXVECTOR3(200.0f, 50.0f, 90.0f), 0		};
			g_aEnemyInfo[18] = { D3DXVECTOR3(20.0f, 50.0f, -400.0f), 0		};
			g_aEnemyInfo[19] = { D3DXVECTOR3(120.0f, 50.0f, -320.0f), 0		};
			g_aEnemyInfo[20] = { D3DXVECTOR3(-320.0f, 50.0f, -120.0f), 0	};
			g_aEnemyInfo[21] = { D3DXVECTOR3(-400.0f, 50.0f, 0.0f), 0		};
			g_aEnemyInfo[22] = { D3DXVECTOR3(-400.0f, 50.0f, 200.0f), 0		};
			g_aEnemyInfo[23] = { D3DXVECTOR3(300.0f, 50.0f, 600.0f), 0		};
			g_aEnemyInfo[24] = { D3DXVECTOR3(-300.0f, 50.0f, 600.0f), 0		};
			g_aEnemyInfo[25] = { D3DXVECTOR3(-600.0f, 50.0f, 300.0f), 0		};
			g_aEnemyInfo[26] = { D3DXVECTOR3(-50.0f, 50.0f, 400.0f), 0		};
			g_aEnemyInfo[27] = { D3DXVECTOR3(150.0f, 50.0f, -200.0f), 0		};
			g_aEnemyInfo[28] = { D3DXVECTOR3(250.0f, 50.0f, 0.0f), 0		};
			g_aEnemyInfo[29] = { D3DXVECTOR3(0.0f, 50.0f, 120.0f), 0		};

			for (int nCntEnemyInfo = 0; nCntEnemyInfo < 30; nCntEnemyInfo++)
			{// 使用していない状態にする
				g_aEnemyInfo[nCntEnemyInfo].bUse = false;
			}

			SetNumEnemy(30);	// 敵の総数の設定
			SetRemnants(30);	// 敵の残数の設定
		}
		//=====================================
		//	          ステージ２
		//=====================================
		else if (g_StageState == STAGENUM_2)
		{//設置物
		 //木のオブジェクトを消すと判定がおかしくなるバグ発生
			// 前のステージのBGMを止める
			StopSound(SOUND_LABEL_STAGE1);
			StopSound(SOUND_LABEL_RAIN);

			// ステージ2のBGM
			PlaySound(SOUND_LABEL_STAGE2);

		 //================================================================================
		 // 当たり判定のあるオブジェクト
		 //================================================================================
			for (int nCount = 0; nCount < 9; nCount++)
			{
				//右壁
				SetObject2(D3DXVECTOR3(800.0f, 0.0f, nCount * 100.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				SetObject2(D3DXVECTOR3(800.0f, 0.0f, nCount * -100.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				//左壁
				SetObject2(D3DXVECTOR3(-800.0f, 0.0f, nCount * 100.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				SetObject2(D3DXVECTOR3(-800.0f, 0.0f, nCount * -100.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				//上壁
				SetObject2(D3DXVECTOR3(nCount * 100.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				SetObject2(D3DXVECTOR3(nCount * -100.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				//下壁
				SetObject2(D3DXVECTOR3(nCount * -100.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
				SetObject2(D3DXVECTOR3(nCount * 100.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			}//72

			 //マップに散りばめるモデル
			SetObject2(D3DXVECTOR3(300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(-300.0f, 0.0f, 150.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(600.0f, 0.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(0.0f, 0.0f, -320.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(-560.0f, 0.0f, -560.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(-430.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(330.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(540.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(0.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(-580.0f, 0.0f, 320.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));
			SetObject2(D3DXVECTOR3(680.0f, 0.0f, 680.0f), D3DXVECTOR3(0.0f, 0.0f + rand() % 180, 0.0f));

			//現在個数 : 83

			//==================================
			//             敵の配置
			//==================================
			g_aEnemyInfo[0]  = { D3DXVECTOR3(600.0f, 50.0f, 600.0f), 0 };
			g_aEnemyInfo[1] = { D3DXVECTOR3(600.0f, 50.0f, -600.0f), 0 };
			g_aEnemyInfo[2] = { D3DXVECTOR3(-600.0f, 50.0f, 600.0f), 0 };
			g_aEnemyInfo[3] = { D3DXVECTOR3(-480.0f, 50.0f, -580.0f), 0 };
			g_aEnemyInfo[4] = { D3DXVECTOR3(600.0f, 50.0f, 0.0f), 0 };
			g_aEnemyInfo[5] = { D3DXVECTOR3(-600.0f, 50.0f, 0.0f), 0 };
			g_aEnemyInfo[6] = { D3DXVECTOR3(0.0f, 50.0f, 600.0f), 0 };
			g_aEnemyInfo[7] = { D3DXVECTOR3(0.0f, 50.0f, -600.0f), 0 };
			g_aEnemyInfo[8] = { D3DXVECTOR3(-320.0f, 50.0f, 600.0f), 0 };
			g_aEnemyInfo[9] = { D3DXVECTOR3(-120.0f, 50.0f, -600.0f), 0 };
			g_aEnemyInfo[10] = { D3DXVECTOR3(600.0f, 50.0f, 300.0f), 0 };
			g_aEnemyInfo[11] = { D3DXVECTOR3(200.0f, 50.0f, -400.0f), 0 };
			g_aEnemyInfo[12] = { D3DXVECTOR3(-150.0f, 50.0f, 200.0f), 0 };
			g_aEnemyInfo[13] = { D3DXVECTOR3(-50.0f, 50.0f, -200.0f), 0 };
			g_aEnemyInfo[14] = { D3DXVECTOR3(400.0f, 50.0f, 280.0f), 0 };
			g_aEnemyInfo[15] = { D3DXVECTOR3(220.0f, 50.0f, 0.0f), 0 };
			g_aEnemyInfo[16] = { D3DXVECTOR3(220.0f, 50.0f, 220.0f), 0 };
			g_aEnemyInfo[17] = { D3DXVECTOR3(10, 50.0f, 100.0f), 0 };
			g_aEnemyInfo[18] = { D3DXVECTOR3(-100.0f, 50.0f, -100.0f), 0 };
			g_aEnemyInfo[19] = { D3DXVECTOR3(-510.0f, 50.0f, -300), 0 };
			g_aEnemyInfo[20] = { D3DXVECTOR3(30.0f, 50.0f, 50.0f), 0 };
			g_aEnemyInfo[21] = { D3DXVECTOR3(50.0f, 50.0f, -50.0f), 0 };
			g_aEnemyInfo[22] = { D3DXVECTOR3(-50.0f, 50.0f, 30.0f), 0 };
			g_aEnemyInfo[23] = { D3DXVECTOR3(-50.0f, 50.0f, -50.0f), 0 };
			g_aEnemyInfo[24] = { D3DXVECTOR3(150.0f, 50.0f, 20.0f), 0 };
			g_aEnemyInfo[25] = { D3DXVECTOR3(700.0f, 50.0f, 130.0f), 0 };
			g_aEnemyInfo[26] = { D3DXVECTOR3(0.0f, 50.0f, 220.0f), 0 };
			g_aEnemyInfo[27] = { D3DXVECTOR3(10, 50.0f, 100.0f), 0 };
			g_aEnemyInfo[28] = { D3DXVECTOR3(-100.0f, 50.0f, -190.0f), 0 };
			g_aEnemyInfo[29] = { D3DXVECTOR3(-380.0f, 50.0f, -460), 0 };

			for (int nCntEnemyInfo = 0; nCntEnemyInfo < 30; nCntEnemyInfo++)
			{// 使用していない状態にする
				g_aEnemyInfo[nCntEnemyInfo].bUse = false;
			}

			SetNumEnemy(30);	// 敵の総数の設定
			SetRemnants(30);	// 敵の残数の設定
		}
		//=====================================
		//	          ステージ3
		//=====================================
		else if (g_StageState == STAGENUM_3)
		{//設置物
			// 前のステージのBGMを止める
			StopSound(SOUND_LABEL_STAGE2);

			// ステージ3のBGM
			PlaySound(SOUND_LABEL_STAGE3);

		 //横上右
			SetObject3(D3DXVECTOR3(750.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(550.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//横上左
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-550.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-750.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//横下右
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(550.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(750.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//横下左
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-550.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-750.0f, 0.0f, -340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//縦下左
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, -550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, -750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//縦上右
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, 550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(-350.0f, 0.0f, 750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//縦上右
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, -550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, -750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//縦上右
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, 550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetObject3(D3DXVECTOR3(350.0f, 0.0f, 750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			//==================================
			//             敵の配置
			//==================================
			g_aEnemyInfo[0]  = { D3DXVECTOR3(620.0f, 50.0f, -100.0f), 0 };
			g_aEnemyInfo[1] = { D3DXVECTOR3(-620.0f, 50.0f, 230.0f), 0 };
			g_aEnemyInfo[2] = { D3DXVECTOR3(0.0f, 50.0f, 620.0f), 0 };
			g_aEnemyInfo[3] = { D3DXVECTOR3(0.0f, 50.0f, -620.0f), 0 };
			g_aEnemyInfo[4] = { D3DXVECTOR3(580.0f, 50.0f, 60.0f), 0 };
			g_aEnemyInfo[5] = { D3DXVECTOR3(-580.0f, 50.0f, -240.0f), 0 };
			g_aEnemyInfo[6] = { D3DXVECTOR3(120.0f, 50.0f, 580.0f), 0 };
			g_aEnemyInfo[7] = { D3DXVECTOR3(180.0f, 50.0f, -580.0f), 0 };
			g_aEnemyInfo[8] = { D3DXVECTOR3(-160.0f, 50.0f, 200.0f), 0 };
			g_aEnemyInfo[9] = { D3DXVECTOR3(100.0f, 50.0f, -360.0f), 0 };
			g_aEnemyInfo[10] = { D3DXVECTOR3(480.0f, 50.0f, 220.0f), 0 };
			g_aEnemyInfo[11] = { D3DXVECTOR3(-480.0f, 50.0f, 120.0f), 0 };
			g_aEnemyInfo[12] = { D3DXVECTOR3(-140.0f, 50.0f, 480.0f), 0 };
			g_aEnemyInfo[13] = { D3DXVECTOR3(180.0f, 50.0f, -480.0f), 0 };
			g_aEnemyInfo[14] = { D3DXVECTOR3(520.0f, 50.0f, 180.0f), 0 };
			g_aEnemyInfo[15] = { D3DXVECTOR3(-520.0f, 50.0f, 230.0f), 0 };
			g_aEnemyInfo[16] = { D3DXVECTOR3(-90.0f, 50.0f, 520.0f), 0 };
			g_aEnemyInfo[17] = { D3DXVECTOR3(-190.0f, 50.0f, -520.0f), 0 };
			g_aEnemyInfo[18] = { D3DXVECTOR3(350.0f, 50.0f, -70.0f), 0 };
			g_aEnemyInfo[19] = { D3DXVECTOR3(-350.0f, 50.0f, -120.0f), 0 };
			g_aEnemyInfo[20] = { D3DXVECTOR3(60.0f, 50.0f, 350.0f), 0 };
			g_aEnemyInfo[21] = { D3DXVECTOR3(60.0f, 50.0f, -350.0f), 0 };
			g_aEnemyInfo[22] = { D3DXVECTOR3(250.0f, 50.0f, -120.0f), 0 };
			g_aEnemyInfo[23] = { D3DXVECTOR3(-220.0f, 50.0f, 190.0f), 0 };
			g_aEnemyInfo[24] = { D3DXVECTOR3(120.0f, 50.0f, 250.0f), 0 };
			g_aEnemyInfo[25] = { D3DXVECTOR3(210.0f, 50.0f, -250.0f), 0 };
			g_aEnemyInfo[26] = { D3DXVECTOR3(-210.0f, 50.0f, 480.0f), 0 };
			g_aEnemyInfo[27] = { D3DXVECTOR3(-100.0f, 50.0f, -210.0f), 0 };

			for (int nCntEnemyInfo = 0; nCntEnemyInfo < 28; nCntEnemyInfo++)
			{// 使用していない状態にする
				g_aEnemyInfo[nCntEnemyInfo].bUse = false;
			}

			SetNumEnemy(28);	// 敵の総数の設定
			SetRemnants(28);	// 敵の残数の設定
		}

		g_nCntSetStage = 1;
	}
}

void StageStateSwat(bool bSwat)
{
	g_bStageSwat = bSwat;
}

bool GetStageSwat(void)
{
	return g_bStageSwat;
}