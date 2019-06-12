//=========================================================================================================//
//
// ゲーム処理 [game.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "game.h"
#include "fade.h"
#include "main.h"
#include "sound.h"
#include "input.h"

#include "camera.h"
#include "light.h"
#include "model.h"
#include "fade.h"
#include "object.h"
#include "joypad.h"

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
bool bPause;		//Pauseしてるかどうか
bool bMap;		//Pauseしてるかどうか
bool bStart;		//起動時のエンター
int g_nCountTime;		//時間カウント

//=========================================================================================================//
// * ゲームの初期化処理　RESETも
//=========================================================================================================//
void InitGame(int nFadeType)
{
	PLAYER *pPlayer = GetPlayer();
	Camera *pCamera = GetCamera();
	GAME *pGame;	//現在の状態をポインタで取得

	pGame = GetGameState();
	bPause = false;
	bMap = false;
	g_nCountTime = 0;
	
	if(nFadeType == 2)
	{ 
		pGame[0].now = GAMESTATE_START;
		pPlayer[0].state = JAYPAD_STATE_SELECT;
	}
	else
	{ 
		pGame[0].now = GAMESTATE_NORMAL;
		pPlayer[0].state = JAYPAD_STATE_GAME;
		StopSound();
		PlaySound(SOUND_LABEL_FBGM000);
	}
	pPlayer[0].SDstate = SDROP_STATE_NONE;
	pPlayer[0].nCntSDstate = 0;
	for (int nCount = 0; nCount < 3; nCount++) { pPlayer[0].nDSkill[nCount] = -1; }
	for(int nCount = 0;nCount < 3;nCount++){ pPlayer[0].nCntDWindow[nCount] = -1; }

	pPlayer[0].nScnSpeech = 0;
	pPlayer[0].nNumSpeech = 0;

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();
	
	InitModel();
	InitObject();
	InitShadow();
	InitPolygon();
	InitBGWindow();

	//InitAnimation();
	InitZWindow();
	bStart = false;
	pCamera[0].fDistance = 300;
	pCamera[0].nCntModel = 0;

	// マップ初期化
	SetLoad_Map(D3DXVECTOR3(300.0f, -300.0f, 2000.0f), MAP_TYPE_100_YEUNG);
	//SetLoad_Map(D3DXVECTOR3(0.0f, 300.0f, -2000.0f), MAP_TYPE_301_CASTLE);
	UpdateMap_Change(-1, -1);
	BreakEnemy();
	BreakBullet();

	Result *pResult = GetResult();
	MODEL_MOTION *pMotion = GetModel_Motion();
	CHAR_DATA data = GetGifted(&pResult[0].aStr[0]);
	MODEL_SETTING set;
	Set_P_NOR(&set);
	data.nSkill[0][0] = 24;
	data.nSkill[1][0] = -1;

	GetStatus(&data, false);

	SetModel(D3DXVECTOR3(0.0f, 100.0f, 2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_USE_PLAYER, &set, &data, 40.0f, 85.0f);
	//SetModel(D3DXVECTOR3(0.0f, 0.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_USE_PLAYER, &set, &data, 20.0f, 85.0f);
	ResetCamera();
	pPlayer[0].bGame = true;
	pPlayer[0].nTime[0] = 5000;
	pPlayer[0].nTime[1] = 0;
	//MP
	pPlayer[0].nCntMPGage[0] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.153f, SCREEN_HEIGHT * 0.95f, 0.0f), D3DXCOLOR{ 0.1f,0.1f,0.1f, 1.0f }, 400, 70, 1, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_MAXMPGAGE, WINDOWUSE_GAME, WINDOWTYPE_LEFT_DOWN, WINDOW_DRAW_1);
	pPlayer[0].nCntMPGage[1] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.153f, SCREEN_HEIGHT * 0.95f, 0.0f), D3DXCOLOR{ 0.0f,0.52f,1.0f, 1.0f }, 400, 70, 1, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_AUTOMPGAGE, WINDOWUSE_GAME, WINDOWTYPE_LEFT_DOWN, WINDOW_DRAW_1);
	pPlayer[0].nCntMPGage[2] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.153f, SCREEN_HEIGHT * 0.95f, 0.0f), D3DXCOLOR{ 0.0f,0.52f,1.0f, 1.0f }, 400, 70, 1, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_MPGAGE, WINDOWUSE_GAME, WINDOWTYPE_LEFT_DOWN, WINDOW_DRAW_1);
	//HP
	pPlayer[0].nCntHPGage[0] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.153f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 0.0f,0.0f,0.0f, 1.0f }, 430, 70, 1, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_MAXLIFEGAGE, WINDOWUSE_GAME, WINDOWTYPE_LEFT_DOWN, WINDOW_DRAW_1);
	pPlayer[0].nCntHPGage[1] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.153f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 0.51f,1.0f,0.0f, 1.0f }, 430, 70, 1, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_AUTOLIFEGAGE, WINDOWUSE_GAME, WINDOWTYPE_LEFT_DOWN, WINDOW_DRAW_1);
	pPlayer[0].nCntHPGage[2] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.153f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 0.51f,1.0f,0.0f, 1.0f }, 430, 70, 1, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_LIFEGAGE, WINDOWUSE_GAME, WINDOWTYPE_LEFT_DOWN, WINDOW_DRAW_1);
	pPlayer[0].nCntGageWindow = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.13f, SCREEN_HEIGHT * 0.877f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 50, 54, 11, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_LIFEGAGE, WINDOWUSE_GAME, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	//EXP
	pPlayer[0].nCntEXPGage[0] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.08f, SCREEN_HEIGHT * 0.85f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 100, 100, 3, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_LIFEGAGE, WINDOWUSE_GAME, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	pPlayer[0].nCntEXPGage[1] = SetExplosion(D3DXVECTOR3(SCREEN_WIDTH * 0.015f, SCREEN_HEIGHT * 0.97f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 170, 170, 5, 3, 4, 3, ANIMATION_LOOP, EXPLOSIONSTATE_GAGE, D3DX_PI, 1);
	//スキル枠
	pPlayer[0].nCntSkillWindow[0] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.85f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 200, 100, 19, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_GAME, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	pPlayer[0].nCntSkillWindow[1] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.85f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 200, 100, 20, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_GAME, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	//スタミナゲージ
	pPlayer[0].nCntStamina[0] = SetZWindow(D3DXVECTOR3(9999.9f, 9999.9f, 9999.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 50.0f, 1.0f, 0, 0, WINDOWSTATE_MAXSTAMINAGAGE, 0.0f);
	pPlayer[0].nCntStamina[1] = SetZWindow(D3DXVECTOR3(9999.9f, 9999.9f, 9999.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50.0f, 1.0f, 0, 0, WINDOWSTATE_STAMINAGAGE, 0.0f);

	//ステータス
	pPlayer[0].nCntIcon[0] = SetWindow(D3DXVECTOR3(150, SCREEN_HEIGHT * 0.96f, 0.0f), D3DXCOLOR{ 0.1f,0.1f,0.1f, 1.0f }, 15, 15, 26, 0, 8, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	pPlayer[0].nCntIcon[1] = SetWindow(D3DXVECTOR3(180, SCREEN_HEIGHT * 0.96f, 0.0f), D3DXCOLOR{ 0.1f,0.1f,0.1f, 1.0f }, 15, 15, 26, 1, 8, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	pPlayer[0].nCntIcon[2] = SetWindow(D3DXVECTOR3(210, SCREEN_HEIGHT * 0.96f, 0.0f), D3DXCOLOR{ 0.1f,0.1f,0.1f, 1.0f }, 15, 15, 26, 2, 8, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	pPlayer[0].nCntIcon[3] = SetWindow(D3DXVECTOR3(240, SCREEN_HEIGHT * 0.96f, 0.0f), D3DXCOLOR{ 0.1f,0.1f,0.1f, 1.0f }, 15, 15, 26, 3, 8, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	pPlayer[0].nCntIcon[4] = SetWindow(D3DXVECTOR3(270, SCREEN_HEIGHT * 0.96f, 0.0f), D3DXCOLOR{ 0.1f,0.1f,0.1f, 1.0f }, 15, 15, 26, 4, 8, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	pPlayer[0].nCntIcon[5] = SetWindow(D3DXVECTOR3(300, SCREEN_HEIGHT * 0.96f, 0.0f), D3DXCOLOR{ 0.1f,0.1f,0.1f, 1.0f }, 15, 15, 26, 5, 8, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	pPlayer[0].nCntIcon[6] = SetWindow(D3DXVECTOR3(330, SCREEN_HEIGHT * 0.96f, 0.0f), D3DXCOLOR{ 0.1f,0.1f,0.1f, 1.0f }, 15, 15, 26, 6, 8, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	pPlayer[0].nCntIcon[7] = SetWindow(D3DXVECTOR3(360, SCREEN_HEIGHT * 0.96f, 0.0f), D3DXCOLOR{ 0.1f,0.1f,0.1f, 1.0f }, 15, 15, 26, 7, 8, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_1);

	//文字
	SetFont(RECT{ 42 , 602, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "lv 1", 0, 0, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);
	SetFont(RECT{ 187 , 522, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "職業", 1, 2, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);
	SetFont(RECT{ 182 , 542, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "名前", 2, 1, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);
	SetFont(RECT{ 212 , 612, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "HP", 3, 2, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);
	SetFont(RECT{ 212 , 652, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "SP", 4, 2, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);

	//SkillMP
	char aStr[48];
	int nLength;

	wsprintf(&aStr[0], "%d SP\n", MotionSkill(pMotion[set.nNumMotion[0] * MAX_MODEL_MOTION + data.nSkill[0][0]].key[0].nAtkEffect));
	ReturnSkillName(&aStr[0], data.nSkill[0][0]);
	SetFont(RECT{ 970 , 550, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0f , 1.0f }, &aStr[0], 5, 1, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);
	wsprintf(&aStr[0], "");
	ReturnSkillName(&aStr[0], data.nSkill[1][0]);
	nLength = (int)strlen(&aStr[0]);
	wsprintf(&aStr[nLength], "\n%d SP", MotionSkill(pMotion[set.nNumMotion[0] * MAX_MODEL_MOTION + data.nSkill[1][0]].key[0].nAtkEffect));
	SetFont(RECT{ 885 , 590, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0f , 1.0f }, &aStr[0], 6, 1, DT_RIGHT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);

	SetFont(RECT{ 540 , 5, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 7, 0, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_TIME);
	SetWindow(D3DXVECTOR3(1000, 280, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 280, 280, 6, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_LEFT_DOWN, WINDOW_DRAW_MINIMAP);

	pPlayer[0].nCntModel_M = SetObject(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), OBJECT_TYPE_999_PARROW, OBJECT_USE_MAP, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);
	pPlayer[0].nCntTarget = SetPolygon(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f , 1.0f }, 500.0f, 300.0f, 1);
}
//=========================================================================================================//
// * ゲームの初期化処理　RESETされては困るもの
//=========================================================================================================//
void InitGame0(void)
{
	//bStart = true;
}
//=========================================================================================================//
// * ゲームの終了処理
//=========================================================================================================//
void UninitGame(void)
{
	GAME *pGame;	//現在の状態をポインタで取得
	pGame = GetGameState();
	pGame[0].now = GAMESTATE_NONE;

	UninitCamera();
	UninitLight();

	UninitShadow();
	UninitPolygon();
	UninitModel();
	UninitObject();
	UninitZWindow();
	UninitBGWindow();

	BreakWindow();
	BreakExplosion();
	BreakFont();
}
//=========================================================================================================//
// * ゲームの更新処理
//=========================================================================================================//
void UpdateGame(void)
{
	PLAYER *pPlayer = GetPlayer();
	MODEL_INFO *model = GetModel_Info();
	GAME *pGame;	//現在の状態をポインタで取得
	Camera *pCamera = GetCamera();
	static int nNumber = 0;
	char aStr[126];
	int		nLength;

	pGame = GetGameState();

		switch (pGame[0].now)
		{//ゲーム状態に応じた処理開始
		case GAMESTATE_NONE:
			break;

		case GAMESTATE_START:
			if (pPlayer[0].bButton[BUTTON_ENTER] == true)
			{ 
				pGame[0].now = GAMESTATE_NORMAL;
				pPlayer[0].state = JAYPAD_STATE_GAME;
				StopSound();
				PlaySound(SOUND_LABEL_FBGM000);
			}
			break;

		case GAMESTATE_NORMAL:
			if (bMap == true)
			{//マップ操作中
				float fRotX = 0.0f, fRotZ = 0.0f, fRot;
				XINPUT_STATE state;
				XInputGetState(0, &state);

				 //閉じる
				if (pPlayer[0].bButton[BUTTON_BACKSPACE] == true || pPlayer[0].bButton[BUTTON_ENTER] == true || pPlayer[0].bButton[BUTTON_P] == true)
				{
					bMap = false; pPlayer[0].state = JAYPAD_STATE_SELECT;
					BackWindow(1);
				}

				//カメラの操作
				if (pPlayer[0].bButton[BUTTON_W] == true) { fRotZ = 1.0f; }
				else if (pPlayer[0].bButton[BUTTON_S] == true) { fRotZ = -1.0f; }
				if (pPlayer[0].bButton[BUTTON_A] == true) { fRotX = -1.0f; }
				else if (pPlayer[0].bButton[BUTTON_D] == true) { fRotX = 1.0f; }
				//ジョイパッド補正
				if (pPlayer[0].bButton[BUTTON_WS] == true) { fRotZ = state.Gamepad.sThumbLY; }
				if (pPlayer[0].bButton[BUTTON_SS] == true) { fRotZ = state.Gamepad.sThumbLY; }
				if (pPlayer[0].bButton[BUTTON_AS] == true) { fRotX = state.Gamepad.sThumbLX; }
				if (pPlayer[0].bButton[BUTTON_DS] == true) { fRotX = state.Gamepad.sThumbLX; }

				//角度設定
				fRot = atan2f(fRotX, fRotZ);
				if (fRotX != 0.0f || fRotZ != 0.0f)
				{//カメラの平行移動
					pCamera[2].posV += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -30.0f;
					pCamera[2].posR += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -30.0f;
				}
			}
			else if (bPause == false)
			{
				// カメラの更新処理
				UpdateCamera();

				// ライトの更新処理
				UpdateLight();

				// ポリゴンの更新処理
				UpdateZWindow();
				UpdateModel();
				UpdateObject();
				UpdateShadow();
				UpdateFont();
				UpdateBullet();
				UpdateExplosion();
				UpdateBGWindow();
				UpdateAnimation();
				Attackjudge();
				UpdateWindow(WINDOW_UPDATE_GAME);

				wsprintf(&aStr[0], "L.v %d", (int)model[0].data.nLevel);
				ChangeFont(true, &aStr[0], 0, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				GetCareer(model[0].data.nCareer, &aStr[0]);
				ChangeFont(true, &aStr[0], 1, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				ChangeFont(true, &model[0].data.aName[0], 2, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				wsprintf(&aStr[0], "HP %d / %d", (int)model[0].data.nLifePoint, (int)model[0].data.status.fLifePoint);
				ChangeFont(true, &aStr[0], 3, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				wsprintf(&aStr[0], "SP %d / %d", (int)model[0].data.nSkillPoint, (int)model[0].data.status.fSkillPoint);
				ChangeFont(true, &aStr[0], 4, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				wsprintf(&aStr[0], "TIME %d.%d", pPlayer[0].nTime[0] / 10, pPlayer[0].nTime[0] % 10);
				ChangeFont(true, &aStr[0], 7, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				if (pPlayer[0].nTime[0] == 0)
				{//時間切れ
					GAME_PLAY *pGame_Play = GetGame_Play();
					SetGameState(GAMESTATE_END, MODE_RESULT, 60);
					StopSound();
					PlaySound(SOUND_LABEL_DEAD_END);
					pGame_Play[0].nResults = 16;
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 22, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_2);
				}
				if (pPlayer[0].bButton[BUTTON_P] == true && pPlayer[0].nTime[0] > 0)
				{//ポーズ
					MODEL_MOTION *pMotion = GetModel_Motion();

					PlaySound(SOUND_LABEL_LOCKON);
					pPlayer[0].state = JAYPAD_STATE_SELECT;
					nNumber = 0;
					bPause = true;
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4, 0, 1, 1, 0, WINDOW_UPDATE_SELECT, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_PAUSE);
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.13f, SCREEN_HEIGHT * 0.2f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 30, 5, 0, 1, 4, 0, WINDOW_UPDATE_SELECT, WINDOWSTATE_SELECTOFF, WINDOWUSE_SELECT, WINDOWTYPE_CENTER, WINDOW_DRAW_3);
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.13f, SCREEN_HEIGHT * 0.4f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 30, 5, 1, 1, 4, 1, WINDOW_UPDATE_SELECT, WINDOWSTATE_SELECTOFF, WINDOWUSE_SELECT, WINDOWTYPE_CENTER, WINDOW_DRAW_3);
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.13f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 30, 5, 2, 1, 4, 2, WINDOW_UPDATE_SELECT, WINDOWSTATE_SELECTOFF, WINDOWUSE_SELECT, WINDOWTYPE_CENTER, WINDOW_DRAW_3);
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.13f, SCREEN_HEIGHT * 0.8f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 30, 5, 3, 1, 4, 3, WINDOW_UPDATE_SELECT, WINDOWSTATE_SELECTOFF, WINDOWUSE_SELECT, WINDOWTYPE_CENTER, WINDOW_DRAW_3);

					//CAMERA1と2の設定
					pPlayer[0].nCntModel_P = SetModel(D3DXVECTOR3(0.0f, -999999.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_USE_Pause, &model[0].set, &model[0].data, 40.0f, 85.0f);
					ResetMotion(&model[pPlayer[0].nCntModel_P].set);
					pCamera[1].posR = model[pPlayer[0].nCntModel_P].set.pos + D3DXVECTOR3(0.0f, 30.0f, 0.0f);
					pCamera[1].posV = model[pPlayer[0].nCntModel_P].set.pos + D3DXVECTOR3(80.0f, 80.0f, -230.0f);
					pCamera[2].posR = model[0].set.pos;
					pCamera[2].posV = model[0].set.pos + D3DXVECTOR3(0.0f, 5000.0f, 100.0f);
					
					wsprintf(&aStr[0], "LV\nHP\nMP\nATK\nDEF\nINT\nRES\nSPD\nLUK\n戦闘力");
					SetFont(RECT{ 852 , 128, 1300, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 14, 0, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);
					ShowStatus0(model[0].data, &aStr[0]);
					SetFont(RECT{ 1002 , 128, 1300, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 15, 0, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);
					GetCareer(model[0].data.nCareer, &aStr[0]);
					SetFont(RECT{ 352 , 108, 1300, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 16, 2, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);
					SetFont(RECT{ 352 , 138, 1300, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &model[0].data.aName[0], 31, 1, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);
					wsprintf(&aStr[0], "HP %d / %d", model[0].data.nLifePoint, (int)model[0].data.status.fLifePoint);
					SetFont(RECT{ 352 , 420, 1300, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 17, 1, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);
					wsprintf(&aStr[0], "MP %d / %d", model[0].data.nSkillPoint, (int)model[0].data.status.fSkillPoint);
					SetFont(RECT{ 652 , 420, 1300, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 18, 1, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);

					wsprintf(&aStr[0], "%d SP\n", MotionSkill(pMotion[model[0].set.nNumMotion[0] * MAX_MODEL_MOTION + model[0].data.nSkill[0][0]].key[0].nAtkEffect));
					ReturnSkillName(&aStr[0], model[0].data.nSkill[0][0]);
					SetFont(RECT{ 352 , 470, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0f , 1.0f }, &aStr[0], 19, 1, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);
					ReturnSkillName_P(&aStr[0], &model[0].data.nSkill[0][0]);
					SetFont(RECT{ 352 , 550, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0f , 1.0f }, &aStr[0], 20, 2, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);

					wsprintf(&aStr[0], "%d SP\n", MotionSkill(pMotion[model[0].set.nNumMotion[0] * MAX_MODEL_MOTION + model[0].data.nSkill[1][0]].key[0].nAtkEffect));
					ReturnSkillName(&aStr[0], model[0].data.nSkill[1][0]);
					SetFont(RECT{ 652 , 470, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0f , 1.0f }, &aStr[0], 21, 1, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);
					ReturnSkillName_P(&aStr[0], &model[0].data.nSkill[1][0]);
					SetFont(RECT{ 652 , 550, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0f , 1.0f }, &aStr[0], 22, 2, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);

					wsprintf(&aStr[0], "\n");
					nLength = strlen(&aStr[0]);
					if (model[0].data.plus[1].fLifePoint > 0.0f) { wsprintf(&aStr[nLength], "+%d\n", (int)model[0].data.plus[1].fLifePoint); }
					else { wsprintf(&aStr[nLength], "\n"); }
					nLength = strlen(&aStr[0]);
					if (model[0].data.plus[1].fSkillPoint > 0.0f) { wsprintf(&aStr[nLength], "+%d\n", (int)model[0].data.plus[1].fSkillPoint); }
					else { wsprintf(&aStr[nLength], "\n"); }
					nLength = strlen(&aStr[0]);
					if (model[0].data.plus[1].fAttack > 0.0f) { wsprintf(&aStr[nLength], "+%d\n", (int)model[0].data.plus[1].fAttack); }
					else { wsprintf(&aStr[nLength], "\n"); }
					nLength = strlen(&aStr[0]);
					if (model[0].data.plus[1].fDefense > 0.0f) { wsprintf(&aStr[nLength], "+%d\n", (int)model[0].data.plus[1].fDefense); }
					else { wsprintf(&aStr[nLength], "\n"); }
					nLength = strlen(&aStr[0]);
					if (model[0].data.plus[1].fInt > 0.0f) { wsprintf(&aStr[nLength], "+%d\n", (int)model[0].data.plus[1].fInt); }
					else { wsprintf(&aStr[nLength], "\n"); }
					nLength = strlen(&aStr[0]);
					if (model[0].data.plus[1].fRes > 0.0f) { wsprintf(&aStr[nLength], "+%d\n", (int)model[0].data.plus[1].fRes); }
					else { wsprintf(&aStr[nLength], "\n"); }
					nLength = strlen(&aStr[0]);
					if (model[0].data.plus[1].fSpeed > 0.0f) { wsprintf(&aStr[nLength], "+%d\n", (int)model[0].data.plus[1].fSpeed); }
					else { wsprintf(&aStr[nLength], "\n"); }
					nLength = strlen(&aStr[0]);
					if (model[0].data.plus[1].fLucky > 0.0f) { wsprintf(&aStr[nLength], "+%d\n", (int)model[0].data.plus[1].fLucky); }
					else { wsprintf(&aStr[nLength], "\n"); }
					SetFont(RECT{ 1112 , 128, 1300, 720 }, D3DXCOLOR{ 0.98f, 1.0f, 0.05f , 1.0f }, &aStr[0], 23, 0, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_FRONT);


				}
			}
			else 
			{
				UpdateModel_P();

				if (pPlayer[0].bButton[BUTTON_W] == true)
				{//上移動
					PlaySound(SOUND_LABEL_BOTTON002);
					nNumber = (nNumber + 3) % 4;
				}
				if (pPlayer[0].bButton[BUTTON_S] == true)
				{//下移動
					PlaySound(SOUND_LABEL_BOTTON002);
					nNumber = (nNumber + 1) % 4;
				}

				SelectWindow(nNumber);
				if (pPlayer[0].bButton[BUTTON_ENTER] == true)
				{
					PlaySound(SOUND_LABEL_TitleSE001);
					if (nNumber == 0)
					{//戻る
						pPlayer[0].state = JAYPAD_STATE_GAME;
						BackWindow(5);
						bPause = false;
						model[pPlayer[0].nCntModel_P].bUse = false;
						for (int nCntFont = 14; nCntFont < 24; nCntFont++)
						{
							ChangeFont(false, &aStr[0], nCntFont, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
					}
					else if (nNumber == 1)
					{//初めから
						SetFade(MODE_GAME, 0, 0.0f, 0);
					}
					else if (nNumber == 2)
					{//Titleへ
						SetFade(MODE_TITLE, 0, 0.0f, 0);		//Titleへ
					}
					else if (nNumber == 3)
					{//マップへ
						bMap = true;
						pPlayer[0].state = JAYPAD_STATE_GAME;
						SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 340, 287, 6, 0, 1, 1, 0, WINDOW_UPDATE_SELECT, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_MAP);
					}
				}
				else if (pPlayer[0].bButton[BUTTON_P] == true)
				{
					PlaySound(SOUND_LABEL_LOCKOFF);
					pPlayer[0].state = JAYPAD_STATE_GAME;
					BackWindow(5);
					bPause = false;
					model[pPlayer[0].nCntModel_P].bUse = false;
					for (int nCntFont = 14; nCntFont < 24; nCntFont++)
					{
						ChangeFont(false, &aStr[0], nCntFont, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}
			}
			break;

		case GAMESTATE_END:	//終了までのカウントダウン
			pGame[0].nCntGameState++;
			if (pGame[0].nCntGameState > 200)
			{
				pGame[0].now = GAMESTATE_NONE;
				SetFade(pGame[0].CHANGE, 0, 0.0f, 0);
			}
			break;
		}
}
//=========================================================================================================//
// * ゲームの描画処理
//=========================================================================================================//
void DrawGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Camera *pCamera = GetCamera();
	FADE *pFade = GetFade();

	DrawBGWindow();

	// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(20, 20, 20, 255), 1.0f, 0);

	// カメラの設定
	SetCamera(0);

	// ポリゴンの描画処理
	//DrawMeshField();
	DrawObject();
	DrawModel();
	DrawShadow();
	//DrawMeshBack();
	//DrawMeshDome();

	DrawAnimation();
	DrawWindow(WINDOW_DRAW_1);
	DrawExplosion();
	DrawZWindow();
	DrawFont(WINDOW_DRAW_1);

	DrawWindow(WINDOW_DRAW_2);
	DrawFont(WINDOW_DRAW_0);

	if (pCamera[3].nCntState != 2)
	{
		DrawWindow(WINDOW_DRAW_MINIMAP);
		SetCamera(3);
		// Ｚバッファのクリア
		pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
		if (pCamera[3].nCntState == 0) { DrawObject_MAP(-1); }
		else { DrawObject_MAP(0); }
		DrawPolygon();
	}

	if (bPause == true)
	{//Pause中
		SetCamera(0);
		DrawWindow(WINDOW_DRAW_PAUSE);

		// Ｚバッファのクリア
		pDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

		// カメラの設定
		SetCamera(1);
		DrawModel_P();
		DrawFont(WINDOW_DRAW_FRONT);
		DrawWindow(WINDOW_DRAW_MAP);

		if (bMap == true)
		{//マップの表示
			SetCamera(2);
			// バックバッファ＆Ｚバッファのクリア
			pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

			// カメラの設定
			DrawModel_P();
			DrawObject_MAP(-1);
			DrawPolygon();
		}
		SetCamera(1);

		DrawWindow(WINDOW_DRAW_3);
	}
}
//=========================================================================================================//
// * ゲームの状態管理　取得
//=========================================================================================================//
GAME *GetGameState(void)
{
	static GAME game = GAME{ GAMESTATE_NORMAL , MODE_GAME , 0 };	//初回初期化

	return &game;
}
//=========================================================================================================//
// * ゲームの状態管理　set
//=========================================================================================================//
void SetGameState(GAMESTATE game, MODE mode, int nCntGameState)
{
	GAME *pGame;	//現在の状態をポインタで取得

	pGame = GetGameState();

	pGame[0].now = game;
	pGame[0].CHANGE = mode;
	pGame[0].nCntGameState = nCntGameState;
}
//=============================================================================
// キーボード更新処理
//=============================================================================
void ButtonKeybord(void)
{
	PLAYER *pPlayer = GetPlayer();
	if (pPlayer[0].state == JAYPAD_STATE_GAME)
	{
		pPlayer[0].bButton[BUTTON_W] = GetKeyboardPress(DIK_W);
		pPlayer[0].bButton[BUTTON_A] = GetKeyboardPress(DIK_A);
		pPlayer[0].bButton[BUTTON_S] = GetKeyboardPress(DIK_S);
		pPlayer[0].bButton[BUTTON_D] = GetKeyboardPress(DIK_D);
		pPlayer[0].bButton[BUTTON_ENTER] = GetKeyboardTrigger(DIK_RETURN);
		pPlayer[0].bButton[BUTTON_BACKSPACE] = GetKeyboardTrigger(DIK_BACKSPACE);

		pPlayer[0].bButton[BUTTON_UP] = GetKeyboardTrigger(DIK_UP);
		pPlayer[0].bButton[BUTTON_DOWN] = GetKeyboardTrigger(DIK_DOWN);
		pPlayer[0].bButton[BUTTON_LEFT] = GetKeyboardTrigger(DIK_LEFT);
		pPlayer[0].bButton[BUTTON_RIGHT] = GetKeyboardTrigger(DIK_RIGHT);
	}
	else
	{
		pPlayer[0].bButton[BUTTON_W] = GetKeyboardTPress(DIK_W);
		pPlayer[0].bButton[BUTTON_A] = GetKeyboardTPress(DIK_A);
		pPlayer[0].bButton[BUTTON_S] = GetKeyboardTPress(DIK_S);
		pPlayer[0].bButton[BUTTON_D] = GetKeyboardTPress(DIK_D);
		pPlayer[0].bButton[BUTTON_ENTER] = GetKeyboardTPress(DIK_RETURN);
		pPlayer[0].bButton[BUTTON_BACKSPACE] = GetKeyboardTPress(DIK_BACKSPACE);

		if (GetKeyboardTPress(DIK_UP) == true) { pPlayer[0].bButton[BUTTON_W] = true; }
		if (GetKeyboardTPress(DIK_LEFT) == true) { pPlayer[0].bButton[BUTTON_A] = true; }
		if (GetKeyboardTPress(DIK_DOWN) == true) { pPlayer[0].bButton[BUTTON_S] = true; }
		if (GetKeyboardTPress(DIK_RIGHT) == true) { pPlayer[0].bButton[BUTTON_D] = true; }
	}
	pPlayer[0].bButton[BUTTON_Q] = GetKeyboardPress(DIK_Q);
	pPlayer[0].bButton[BUTTON_U] = GetKeyboardPress(DIK_U);
	pPlayer[0].bButton[BUTTON_O] = GetKeyboardPress(DIK_O);
	pPlayer[0].bButton[BUTTON_Y] = GetKeyboardPress(DIK_Y);
	pPlayer[0].bButton[BUTTON_H] = GetKeyboardPress(DIK_H);
	pPlayer[0].bButton[BUTTON_E] = GetKeyboardTrigger(DIK_E);
	pPlayer[0].bButton[BUTTON_X] = GetKeyboardTrigger(DIK_X);
	pPlayer[0].bButton[BUTTON_C] = GetKeyboardTrigger(DIK_C);
	pPlayer[0].bButton[BUTTON_P] = GetKeyboardTrigger(DIK_P);
	pPlayer[0].bButton[BUTTON_J] = GetKeyboardTrigger(DIK_J);
	pPlayer[0].bButton[BUTTON_I] = GetKeyboardTrigger(DIK_I);
	pPlayer[0].bButton[BUTTON_K] = GetKeyboardTrigger(DIK_K);
	pPlayer[0].bButton[BUTTON_L] = GetKeyboardTrigger(DIK_L);
	pPlayer[0].bButton[BUTTON_Z] = GetKeyboardTrigger(DIK_Z);


	pPlayer[0].bButton[BUTTON_LSHIFT] = GetKeyboardPress(DIK_LSHIFT);
	pPlayer[0].bButton[BUTTON_LCTRL] = GetKeyboardPress(DIK_LCONTROL);
	pPlayer[0].bButton[BUTTON_SPACE] = GetKeyboardTrigger(DIK_SPACE);
}
//=============================================================================
// ジョイパッド更新処理
//=============================================================================
void ButtonJoypad(void)
{
	PLAYER *pPlayer = GetPlayer();

	if (pPlayer[0].bJoyUse == true)
	{
		XINPUT_STATE state;
		XInputGetState(0, &state);
		//XINPUT_GAMEPAD_A
		if (pPlayer[0].state == JAYPAD_STATE_GAME)
		{
			if (GetPadTrigger(BUTTON_BACKSPACE, XINPUT_GAMEPAD_A, &state) < 1)
			{
				pPlayer[0].bButton[BUTTON_SPACE] = true;
				pPlayer[0].bButton[BUTTON_ENTER] = true;
				pPlayer[0].bButton[BUTTON_JENTER] = true;
			}
			if (GetPadTrigger(BUTTON_L, XINPUT_GAMEPAD_B, &state) < 1)
			{
				pPlayer[0].bButton[BUTTON_L] = true;
				pPlayer[0].bButton[BUTTON_ENTER] = true;
				pPlayer[0].bButton[BUTTON_JENTER] = true;
			}
			if (state.Gamepad.sThumbRX > 0) { pPlayer[0].bButton[BUTTON_U] = true; pPlayer[0].bButton[BUTTON_US] = true; }
			if (state.Gamepad.sThumbRX < 0) { pPlayer[0].bButton[BUTTON_O] = true; pPlayer[0].bButton[BUTTON_OS] = true; }
			if (state.Gamepad.sThumbRY > 0) { pPlayer[0].bButton[BUTTON_H] = true; pPlayer[0].bButton[BUTTON_HS] = true; }
			if (state.Gamepad.sThumbRY < 0) { pPlayer[0].bButton[BUTTON_Y] = true; pPlayer[0].bButton[BUTTON_YS] = true; }

			if (state.Gamepad.sThumbLX > 0) { pPlayer[0].bButton[BUTTON_D] = true; pPlayer[0].bButton[BUTTON_DS] = true; }
			if (state.Gamepad.sThumbLX < 0) { pPlayer[0].bButton[BUTTON_A] = true; pPlayer[0].bButton[BUTTON_AS] = true; }
			if (state.Gamepad.sThumbLY > 0) { pPlayer[0].bButton[BUTTON_W] = true; pPlayer[0].bButton[BUTTON_WS] = true; }
			if (state.Gamepad.sThumbLY < 0) { pPlayer[0].bButton[BUTTON_S] = true; pPlayer[0].bButton[BUTTON_SS] = true; }

			if (GetPadTPress(BUTTON_D, XINPUT_GAMEPAD_DPAD_RIGHT, &state) < 1) { pPlayer[0].bButton[BUTTON_RIGHT] = true; }
			if (GetPadTPress(BUTTON_A, XINPUT_GAMEPAD_DPAD_LEFT, &state) < 1) { pPlayer[0].bButton[BUTTON_LEFT] = true; }
			if (GetPadTPress(BUTTON_W, XINPUT_GAMEPAD_DPAD_UP, &state) < 1) { pPlayer[0].bButton[BUTTON_UP] = true; }
			if (GetPadTPress(BUTTON_S, XINPUT_GAMEPAD_DPAD_DOWN, &state) < 1) { pPlayer[0].bButton[BUTTON_DOWN] = true; }
		}
		else
		{
			if (GetAnalogTPress(BUTTON_DS, 18000, state.Gamepad.sThumbLX, true, &state) < 1) { pPlayer[0].bButton[BUTTON_D] = true; pPlayer[0].bButton[BUTTON_DS] = true; }
			if (GetAnalogTPress(BUTTON_AS, 18000, state.Gamepad.sThumbLX, false, &state) < 1) { pPlayer[0].bButton[BUTTON_A] = true; pPlayer[0].bButton[BUTTON_AS] = true; }
			if (GetAnalogTPress(BUTTON_WS, 18000, state.Gamepad.sThumbLY, true, &state) < 1) { pPlayer[0].bButton[BUTTON_W] = true; pPlayer[0].bButton[BUTTON_WS] = true; }
			if (GetAnalogTPress(BUTTON_SS, 18000, state.Gamepad.sThumbLY, false, &state) < 1) { pPlayer[0].bButton[BUTTON_S] = true; pPlayer[0].bButton[BUTTON_SS] = true; }
			if (GetPadTPress(BUTTON_D, XINPUT_GAMEPAD_DPAD_RIGHT, &state) < 1) { pPlayer[0].bButton[BUTTON_D] = true; }
			if (GetPadTPress(BUTTON_A, XINPUT_GAMEPAD_DPAD_LEFT, &state) < 1) { pPlayer[0].bButton[BUTTON_A] = true; }
			if (GetPadTPress(BUTTON_W, XINPUT_GAMEPAD_DPAD_UP, &state) < 1) { pPlayer[0].bButton[BUTTON_W] = true; }
			if (GetPadTPress(BUTTON_S, XINPUT_GAMEPAD_DPAD_DOWN, &state) < 1) { pPlayer[0].bButton[BUTTON_S] = true; }

			if (GetPadTPress(BUTTON_L, XINPUT_GAMEPAD_B, &state) < 1)
			{
				pPlayer[0].bButton[BUTTON_ENTER] = true;
			}
			if (GetPadTPress(BUTTON_BACKSPACE, XINPUT_GAMEPAD_A, &state) < 1)
			{
				pPlayer[0].bButton[BUTTON_BACKSPACE] = true;
				pPlayer[0].bButton[BUTTON_P] = true;
				GetPadTrigger(BUTTON_SPACE, XINPUT_GAMEPAD_A, &state);
			}

		}
		if (GetPadTPress(BUTTON_P, XINPUT_GAMEPAD_START, &state) < 1) { pPlayer[0].bButton[BUTTON_P] = true; pPlayer[0].bButton[BUTTON_LCTRL] = true; }

		if (GetPadTrigger(BUTTON_J, XINPUT_GAMEPAD_X, &state) < 1) { pPlayer[0].bButton[BUTTON_J] = true; }
		if (GetPadTrigger(BUTTON_I, XINPUT_GAMEPAD_Y, &state) < 1) { pPlayer[0].bButton[BUTTON_I] = true; }

		if (GetAnalogTrigger(BUTTON_E, 50, state.Gamepad.bRightTrigger, &state) < 1) { pPlayer[0].bButton[BUTTON_E] = true; }
		if (state.Gamepad.bLeftTrigger > 50) { pPlayer[0].bButton[BUTTON_Q] = true; }
		if (GetPadTrigger(BUTTON_S, XINPUT_GAMEPAD_RIGHT_SHOULDER, &state) != 100) { pPlayer[0].bButton[BUTTON_LSHIFT] = true; }
		if (GetPadTrigger(BUTTON_K, XINPUT_GAMEPAD_LEFT_SHOULDER, &state) < 1) { pPlayer[0].bButton[BUTTON_K] = true; }
		if (GetPadTrigger(BUTTON_X, XINPUT_GAMEPAD_DPAD_LEFT, &state) < 1) { pPlayer[0].bButton[BUTTON_X] = true; }
		if (GetPadTrigger(BUTTON_C, XINPUT_GAMEPAD_DPAD_RIGHT, &state) < 1) { pPlayer[0].bButton[BUTTON_C] = true; }
	}
}

