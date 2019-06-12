//=========================================================================================================//
//
// ゲーム処理 [game.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <stdio.h>				//インクルドファイル セーブロードのため
#include "game.h"
#include "fade.h"
#include "main.h"
#include "sound.h"

#include "camera.h"
#include "light.h"
#include "model.h"
#include "fade.h"
#include "object.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// * マクロ宣言 *
//*****************************************************************************
#define S_RANK		(2.25f)
#define AA_RANK		(2.0f)
#define A_RANK		(1.85f)
#define B_RANK		(1.45f)
#define C_RANK		(1.15f)
#define D_RANK		(0.8f)


//=========================================================================================================//
// * ゲームの初期化処理　RESETも
//=========================================================================================================//
void InitStart(void)
{
	int *pTime = GetTime();
	pTime[0] = 0;
	Camera *pCamera = GetCamera();
	GAME *pGame;	//現在の状態をポインタで取得
	GAME_PLAY *pGame_Play = GetGame_Play();
	Result *pResult = GetResult();
	strcpy(&pResult[0].aStr[0], "");
	pResult[0].nHightFont = 0;
	pResult[0].nWideFont = 0;
	pGame = GetGameState();
	pGame[0].now = GAMESTATE_START;
	pGame_Play[0].bUse = true;

	// カメラの初期化処理
	InitCamera();
	InitModel();
	InitObject();
	InitShadow();

	// マップ初期化
	SetLoad_Map(D3DXVECTOR3(0.0f, 0.0f, 0.0f), MAP_TYPE_000_CHAR_MAKE);
	UpdateMap_Change(-1, -1);

	// ライトの初期化処理
	InitLight();

	InitShadow();
	InitZWindow();
	pCamera[0].fDistance = 200;
	pCamera[0].addpos.x = 20;
	pCamera[0].addpos.y = 5;
	pCamera[0].nCntModel = 0;
	pCamera[0].state = CAMERA_STATE_GAME;

	// 名前入力画面の作成
	SetFont(RECT{ 470 , 120, 1200, 200 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "", 0, 0, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
	SetWindow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, SCREEN_HEIGHT / 16, 12, 0, 1, 1, 2, WINDOW_UPDATE_SELECT, WINDOWSTATE_SELECTON, WINDOWUSE_FONT, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	int *nSetFont = GetSetFont();
	nSetFont[0] = 1;
	InitChar_Data(1);

	MODEL_SETTING set;
	Set_P_NOR(&set);

	PLAYER *pPlayer = GetPlayer();
	pPlayer[0].state = JAYPAD_STATE_SELECT;
	pPlayer[0].bGame = false;
	pPlayer[0].SDstate = SDROP_STATE_NONE;
	pPlayer[0].nCntSDstate = 0;
	for (int nCount = 0; nCount < 3; nCount++) { pPlayer[0].nDSkill[nCount] = -1; }
	for (int nCount = 0; nCount < 3; nCount++) { pPlayer[0].nCntDWindow[nCount] = -1; }

	pPlayer[0].nScnSpeech = -1;
	pPlayer[0].nNumSpeech = 0;

	CHAR_DATA data;
	data = GetGifted("すぎも");
	GetStatus(&data, false);
	data.nLifePoint = (int)data.status.fLifePoint;
	data.nSkillPoint = (int)data.status.fSkillPoint;
	data.nSkill[0][0] = 24;
	data.nSkill[1][0] = 27;

	BreakModel(0);
	SetModel(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), MODEL_USE_PLAYER, &set, &data, 15.0f, 85.0f);
	MODEL_INFO *model = GetModel_Info();
	model[0].set.state = MODEL_STATE_MAP;
	//UpdateCamera();
	//ResetCamera();
}

//=========================================================================================================//
// * ゲームの終了処理
//=========================================================================================================//
void UninitStart(void)
{
	GAME *pGame;	//現在の状態をポインタで取得
	pGame = GetGameState();
	pGame[0].now = GAMESTATE_NONE;

	BreakWindow();
	BreakFont();
	UninitCamera();
	UninitLight();

	UninitShadow();
	UninitModel();
	UninitObject();
	UninitZWindow();
}
//=========================================================================================================//
// * ゲームの更新処理
//=========================================================================================================//
void UpdateStart(void)
{
	int *pTime = GetTime();
	GAME *pGame;	//現在の状態をポインタで取得
	Result *pResult = GetResult();
	MODEL_INFO *model = GetModel_Info();
	PLAYER *pPlayer = GetPlayer();

	int *nSetFont = GetSetFont();
	bool bSound = true;

	static int nNumber;
	int nLength;
	//PLAYER *pPlayer = GetPlayer();

	pGame = GetGameState();
	//if (bStart == false)
	//{
	

	switch (pGame[0].now)
	{//ゲーム状態に応じた処理開始
	case GAMESTATE_NONE:
		break;

	case GAMESTATE_START:
		if (pPlayer[0].bButton[BUTTON_ENTER] == true)
		{ 
			pGame[0].now = GAMESTATE_NORMAL;
			StopSound();
			PlaySound(SOUND_LABEL_CHAR_MAKE000);
		}
		break;

	case GAMESTATE_NORMAL:
		//名前入力処理
		model[0].set.state = MODEL_STATE_MAP;

		// カメラの更新処理
		UpdateCamera();

		// ライトの更新処理
		UpdateLight();
		UpdateFont();
		UpdateModel();
		UpdateExplosion();
		UpdateAnimation();
		if (pTime[0] > 10) { pTime[0]++; }
		else
		{
			if (pPlayer[0].bButton[BUTTON_W] == true)
			{//一番上じゃなければ 選択を上へ
				PlaySound(SOUND_LABEL_BOTTON002);
				if (pResult[0].nHightFont == 5)
				{//終わり, 平等に出るとき　それぞれ真上に合わせる
					if (pResult[0].nWideFont == 0) { pResult[0].nWideFont = 3; }
					else if (pResult[0].nWideFont == 1) { pResult[0].nWideFont = 5; }
					else if (pResult[0].nWideFont == 2) { pResult[0].nWideFont = 7; }
					else { pResult[0].nWideFont = 9; }
				}
				pResult[0].nHightFont--;
				if (pResult[0].nHightFont == -1)
				{
					if (pResult[0].nWideFont > 8) { pResult[0].nWideFont = 2; }	//終わりに合わせる
					else if (pResult[0].nWideFont > 5) { pResult[0].nWideFont = 1; }	//カナに合わせる
					else { pResult[0].nWideFont = 0; }	//ひらに合わせる
					pResult[0].nHightFont = 5;
				}
			}
			if (pPlayer[0].bButton[BUTTON_S] == true)
			{//一番下じゃなければ 選択を下へ
				PlaySound(SOUND_LABEL_BOTTON002);
				if (pResult[0].nHightFont == 4)
				{//終わり, 平等に入るとき
					if (pResult[0].nWideFont > 8) { pResult[0].nWideFont = 3; }	//終わりに合わせる
					else if (pResult[0].nWideFont > 5) { pResult[0].nWideFont = 2; }	//カナに合わせる
					else if (pResult[0].nWideFont > 3) { pResult[0].nWideFont = 1; }	//カナに合わせる
					else { pResult[0].nWideFont = 0; }	//ひらに合わせる
				}
				pResult[0].nHightFont++;
				if (pResult[0].nHightFont == 6)
				{//終わり, 平等に出るとき　それぞれ真上に合わせる
					if (pResult[0].nWideFont == 0) { pResult[0].nWideFont = 3; }
					else if (pResult[0].nWideFont == 1) { pResult[0].nWideFont = 5; }
					else if (pResult[0].nWideFont == 2) { pResult[0].nWideFont = 7; }
					else { pResult[0].nWideFont = 9; }
					pResult[0].nHightFont = 0;
				}
			}
			if (pPlayer[0].bButton[BUTTON_A] == true)
			{//一番左じゃなければ 選択を左へ
				PlaySound(SOUND_LABEL_BOTTON002);
				pResult[0].nWideFont--;
				if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == -1) { pResult[0].nWideFont = 3; }
				else if (pResult[0].nWideFont == -1) { pResult[0].nWideFont = 10; }
			}
			if (pPlayer[0].bButton[BUTTON_D] == true)
			{//一番右じゃなければ 選択を右へ
				PlaySound(SOUND_LABEL_BOTTON002);
				pResult[0].nWideFont++;
				if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 4) { pResult[0].nWideFont = 0; }
				else if (pResult[0].nWideFont == 11) { pResult[0].nWideFont = 0; }
			}
			if (pPlayer[0].bButton[BUTTON_LCTRL] == true)
			{//終わりに合わせる
				pResult[0].nWideFont = 3;
				pResult[0].nHightFont = 5;
			}
			//枠の更新
			FontWindowMove(pResult[0].nWideFont, pResult[0].nHightFont);

			if (pPlayer[0].bButton[BUTTON_ENTER] == true)
			{//選択に応じた処理
				if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 3)
				{//終了
					SetExplosion(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH, SCREEN_HEIGHT / 2, 2, 8, 3, 5, ANIMATION_NORMAL, EXPLOSIONSTATE_EFFECT, D3DX_PI * 0.0f, 1);
					pTime[0] = 11;
					PlaySound(SOUND_LABEL_TitleSE001);
					bSound = false;
				}
				else if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 2)
				{
					nSetFont[0] = 2;
					//PlaySound(SOUND_LABEL_BOTTON001);
				}	//カタカナに
				else if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 1)
				{
					nSetFont[0] = 1;
					//PlaySound(SOUND_LABEL_BOTTON001);
				}	//ひらがなに
				else if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 0)
				{
					nSetFont[0] = 3;
					//PlaySound(SOUND_LABEL_BOTTON001);
				}	//英語に
				else
				{
					nLength = (int)strlen(&pResult[0].aStr[0]);		//文字数確認
					if (nLength < 23) { InputName(nSetFont[0]); }	//文字数が余っているなら
					else
					{//文字数が最大
						if (nSetFont[0] != 3)
						{//平とカナは濁点と〇あり
							if ((pResult[0].nHightFont == 1 && pResult[0].nWideFont == 3) || pResult[0].nHightFont == 3 && pResult[0].nWideFont == 3) { InputName(nSetFont[0]); }
						}	//変換なら
						if (pResult[0].nHightFont == 4 && pResult[0].nWideFont == 0) { InputName(nSetFont[0]); }	//小文字変換変換なら
						else { PlaySound(SOUND_LABEL_BOTTON001); bSound = false; }//エラー音
					}
				}
				if(bSound == true){ PlaySound(SOUND_LABEL_BOTTON000); }
			}
			if (pPlayer[0].bButton[BUTTON_BACKSPACE] == true)
			{//ひとつ文字を消す
				//PlaySound(SOUND_LABEL_BOTTON003);
				nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認
				if (nLength > 1)
				{ //文字が入っていれば
					strcpy(&pResult[0].aStr[nLength - 2], "\0");
					ChangeFont(true, &pResult[0].aStr[0], 0, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認
					if (nLength == 0)
					{
						ChangeFont(false, &pResult[0].aStr[0], 0, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						SetFont(RECT{ 470 , 120, 1200, 200 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "", 0, 0, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
					}

					PlaySound(SOUND_LABEL_BOTTON001);//削除音
				}
				else { PlaySound(SOUND_LABEL_BOTTON002); }//削除失敗音
			}
			//終了時NORMAL1へ ステ表示も
		}

		if (pTime[0] == 30)
		{
			SetFadeWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 0, WINDOWSTATE_FADEOUT, WINDOWUSE_NORMAL);

			nSetFont[0] = 0;
			//int	nValue;
			char	aStr[252];
			char	aCareer[48];
			nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認
			if (nLength == 0) { strcat(&pResult[0].aStr[0], "ベルセレージュ"); }

			model[0].set.state = MODEL_STATE_NONE;
			strcpy(&model[0].data.aName[0], &pResult[0].aStr[0]);//名前の代入
			model[0].data = GetGifted(&model[0].data.aName[0]);
			GetStatus(&model[0].data, false);
			model[0].data.nLifePoint = (int)model[0].data.status.fLifePoint;
			model[0].data.nSkillPoint = (int)model[0].data.status.fSkillPoint;
			model[0].data.nSkill[0][0] = 24;
			model[0].data.nSkill[1][0] = 27;

			BackWindow(1);
			SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 200, 360, 13, 0, 1, 1, 0, WINDOW_UPDATE_SELECT, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_0);
			ChangeFont(false, "", 0, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			SetFont(RECT{ 50 , 20, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pResult[0].aStr[0], 13, 0, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 22, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &pResult[0].aStr[0], 0, 0, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			wsprintf(&aStr[0], "戦闘力 %d", model[0].data.nStrength);
			SetFont(RECT{ 50 , 90, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 14, 0, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 92, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 1, 0, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			wsprintf(&aStr[0], "種族 : ");
			nLength = (int)strlen(&aStr[0]);//文字数確認
			GetCareer(model[0].data.nCareer, &aCareer[0]);
			wsprintf(&aStr[nLength], &aCareer[0]);
			SetFont(RECT{ 50 , 140, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 15, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 142, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 2, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			wsprintf(&aStr[0], "HP %d", (int)model[0].data.status.fLifePoint);
			SetFont(RECT{ 50 , 185, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 16, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 187, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 3, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			wsprintf(&aStr[0], "MP %d", (int)model[0].data.status.fSkillPoint);
			SetFont(RECT{ 50 , 225, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 17, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 227, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 4, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			wsprintf(&aStr[0], "ATK %d", (int)model[0].data.status.fAttack);
			SetFont(RECT{ 50 , 265, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 18, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 267, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 5, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			wsprintf(&aStr[0], "DEF %d", (int)model[0].data.status.fDefense);
			SetFont(RECT{ 50 , 305, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 19, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 307, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 6, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			wsprintf(&aStr[0], "INT %d", (int)model[0].data.status.fInt);
			SetFont(RECT{ 50 , 345, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 20, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 347, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 7, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			wsprintf(&aStr[0], "RES %d", (int)model[0].data.status.fRes);
			SetFont(RECT{ 50 , 385, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 21, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 387, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 8, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			wsprintf(&aStr[0], "SPD %d", (int)model[0].data.status.fSpeed);
			SetFont(RECT{ 50 , 425, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 22, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 427, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 9, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			wsprintf(&aStr[0], "LUK %d", (int)model[0].data.status.fLucky);
			SetFont(RECT{ 50 , 465, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 23, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 467, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 10, 1, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);

			wsprintf(&aStr[0], "");
			nLength = (int)strlen(&aStr[0]);//文字数確認
											//ステータス別評価 HP
			if (model[0].data.status.fLifePoint - 10000.0f > (INIT_HP * 2 * 11) * S_RANK) { wsprintf(&aStr[nLength], "S\n"); }
			else if (model[0].data.status.fLifePoint - 10000.0f >  (INIT_HP * 2 * 11) * AA_RANK) { wsprintf(&aStr[nLength], "A+\n"); }
			else if (model[0].data.status.fLifePoint - 10000.0f >  (INIT_HP * 2 * 11) * A_RANK) { wsprintf(&aStr[nLength], "A\n"); }
			else if (model[0].data.status.fLifePoint - 10000.0f >  (INIT_HP * 2 * 11) * B_RANK) { wsprintf(&aStr[nLength], "B\n"); }
			else if (model[0].data.status.fLifePoint - 10000.0f >  (INIT_HP * 2 * 11) * C_RANK) { wsprintf(&aStr[nLength], "C\n"); }
			else if (model[0].data.status.fLifePoint - 10000.0f >  (INIT_HP * 2 * 11) * D_RANK) { wsprintf(&aStr[nLength], "D\n"); }
			else { wsprintf(&aStr[nLength], "E\n"); }
			//ステータス別評価 SP
			nLength = (int)strlen(&aStr[0]);//文字数確認
			if (model[0].data.status.fSkillPoint - 2000.0f > (INIT_SP * 2 * 11) * S_RANK) { wsprintf(&aStr[nLength], "S\n"); }
			else if (model[0].data.status.fSkillPoint - 2000.0f  >  (INIT_SP * 2 * 11) * AA_RANK) { wsprintf(&aStr[nLength], "A+\n"); }
			else if (model[0].data.status.fSkillPoint - 2000.0f >  (INIT_SP * 2 * 11) * A_RANK) { wsprintf(&aStr[nLength], "A\n"); }
			else if (model[0].data.status.fSkillPoint - 2000.0f >  (INIT_SP * 2 * 11) * B_RANK) { wsprintf(&aStr[nLength], "B\n"); }
			else if (model[0].data.status.fSkillPoint - 2000.0f >  (INIT_SP * 2 * 11) * C_RANK) { wsprintf(&aStr[nLength], "C\n"); }
			else if (model[0].data.status.fSkillPoint - 2000.0f >  (INIT_SP * 2 * 11) * D_RANK) { wsprintf(&aStr[nLength], "D\n"); }
			else { wsprintf(&aStr[nLength], "E\n"); }
			//ステータス別評価 攻撃
			nLength = (int)strlen(&aStr[0]);//文字数確認
			if (model[0].data.status.fAttack > (INIT_ATK * 2 * 11) * S_RANK) { wsprintf(&aStr[nLength], "S\n"); }
			else if (model[0].data.status.fAttack >  (INIT_ATK * 2 * 11) * AA_RANK) { wsprintf(&aStr[nLength], "A+\n"); }
			else if (model[0].data.status.fAttack >  (INIT_ATK * 2 * 11) * A_RANK) { wsprintf(&aStr[nLength], "A\n"); }
			else if (model[0].data.status.fAttack >  (INIT_ATK * 2 * 11) * B_RANK) { wsprintf(&aStr[nLength], "B\n"); }
			else if (model[0].data.status.fAttack >  (INIT_ATK * 2 * 11) * C_RANK) { wsprintf(&aStr[nLength], "C\n"); }
			else if (model[0].data.status.fAttack >  (INIT_ATK * 2 * 11) * D_RANK) { wsprintf(&aStr[nLength], "D\n"); }
			else { wsprintf(&aStr[nLength], "E\n"); }
			//ステータス別評価 防御
			nLength = (int)strlen(&aStr[0]);//文字数確認
			if (model[0].data.status.fDefense > (INIT_DFF * 2 * 11) * S_RANK) { wsprintf(&aStr[nLength], "S\n"); }
			else if (model[0].data.status.fDefense >  (INIT_DFF * 2 * 11) * AA_RANK) { wsprintf(&aStr[nLength], "A+\n"); }
			else if (model[0].data.status.fDefense >  (INIT_DFF * 2 * 11) * A_RANK) { wsprintf(&aStr[nLength], "A\n"); }
			else if (model[0].data.status.fDefense >  (INIT_DFF * 2 * 11) * B_RANK) { wsprintf(&aStr[nLength], "B\n"); }
			else if (model[0].data.status.fDefense >  (INIT_DFF * 2 * 11) * C_RANK) { wsprintf(&aStr[nLength], "C\n"); }
			else if (model[0].data.status.fDefense >  (INIT_DFF * 2 * 11) * D_RANK) { wsprintf(&aStr[nLength], "D\n"); }
			else { wsprintf(&aStr[nLength], "E\n"); }
			//ステータス別評価 速さ
			nLength = (int)strlen(&aStr[0]);//文字数確認
			if (model[0].data.status.fSpeed > (INIT_SPD * 2 * 11) * S_RANK) { wsprintf(&aStr[nLength], "S\n"); }
			else if (model[0].data.status.fSpeed >  (INIT_SPD * 2 * 11) * AA_RANK) { wsprintf(&aStr[nLength], "A+\n"); }
			else if (model[0].data.status.fSpeed >  (INIT_SPD * 2 * 11) * A_RANK) { wsprintf(&aStr[nLength], "A\n"); }
			else if (model[0].data.status.fSpeed >  (INIT_SPD * 2 * 11) * B_RANK) { wsprintf(&aStr[nLength], "B\n"); }
			else if (model[0].data.status.fSpeed >  (INIT_SPD * 2 * 11) * C_RANK) { wsprintf(&aStr[nLength], "C\n"); }
			else if (model[0].data.status.fSpeed >  (INIT_SPD * 2 * 11) * D_RANK) { wsprintf(&aStr[nLength], "D\n"); }
			else { wsprintf(&aStr[nLength], "E\n"); }
			//ステータス別評価 運
			nLength = (int)strlen(&aStr[0]);//文字数確認
			if (model[0].data.status.fLucky - 1000.0f > (INIT_LUK * 2 * 11) * S_RANK) { wsprintf(&aStr[nLength], "S\n"); }
			else if (model[0].data.status.fLucky - 1000.0f  >  (INIT_LUK * 2 * 11) * AA_RANK) { wsprintf(&aStr[nLength], "A+\n"); }
			else if (model[0].data.status.fLucky - 1000.0f  >  (INIT_LUK * 2 * 11) * A_RANK) { wsprintf(&aStr[nLength], "A\n"); }
			else if (model[0].data.status.fLucky - 1000.0f  >  (INIT_LUK * 2 * 11) * B_RANK) { wsprintf(&aStr[nLength], "B\n"); }
			else if (model[0].data.status.fLucky - 1000.0f  >  (INIT_LUK * 2 * 11) * C_RANK) { wsprintf(&aStr[nLength], "C\n"); }
			else if (model[0].data.status.fLucky - 1000.0f  >  (INIT_LUK * 2 * 11) * D_RANK) { wsprintf(&aStr[nLength], "D\n"); }
			else { wsprintf(&aStr[nLength], "E\n"); }

			//総合評価
			wsprintf(&aStr[0], "評価 : ");
			nLength = (int)strlen(&aStr[0]);//文字数確認
			if (model[0].data.nStrength > 110000) { wsprintf(&aStr[nLength], "＊   神   ＊"); }
			else if (model[0].data.nStrength > 1000000) { wsprintf(&aStr[nLength], "＊  天才  ＊"); }
			else if (model[0].data.nStrength > 90000) { wsprintf(&aStr[nLength], "＊ 超優秀 ＊"); }
			else if (model[0].data.nStrength > 80000) { wsprintf(&aStr[nLength], "＊  優秀  ＊"); }
			else if (model[0].data.nStrength > 70000) { wsprintf(&aStr[nLength], "＊  平凡  ＊"); }
			else { wsprintf(&aStr[nLength], "落ちこぼれ..."); }


			SetFont(RECT{ 50 , 625, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 27, 0, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);
			SetFont(RECT{ 52 , 627, 1200, 720 }, D3DXCOLOR{ 0.0f, 0.0f, 0.0 , 1.0f }, &aStr[0], 26, 0, DT_LEFT, -1, false, FONTSTATE_NONE, WINDOW_DRAW_1);

			pTime[0] = 0;
			pGame[0].now = GAMESTATE_NORMAL1;
			pPlayer[0].state = JAYPAD_STATE_GAME;

		}
		break;
	case GAMESTATE_NORMAL1:
		//キャラステータス確認 継続orやり直し
		pTime[0]++;

			// カメラの更新処理
			UpdateCamera();

			// ライトの更新処理
			UpdateLight();

			// ポリゴンの更新処理
			UpdateModel();
			UpdateFont();
			UpdateExplosion();
			UpdateZWindow();
			UpdateAnimation();
			if (pTime[0] == 60)
			{
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 150, 50, 0, 0, 1, 1, 0, WINDOW_UPDATE_SELECT, WINDOWSTATE_FLASH, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_2);
			}
			if (pTime[0] > 120 && pPlayer[0].bButton[BUTTON_ENTER] == true)
			{
				if (pPlayer[0].bButton[BUTTON_JENTER] == true) { SetFade(MODE_GAME, 2, 0.0f, 2); }
				else if (pPlayer[0].bButton[BUTTON_ENTER] == true) { SetFade(MODE_GAME, 2, 0.0f, 1); }
			}
		break;

	case GAMESTATE_END:	//終了までのカウントダウン
		pGame[0].nCntGameState++;
		if (pGame[0].nCntGameState > 200)
		{
			pGame[0].now = GAMESTATE_NONE;
			SetFade(pGame[0].CHANGE, 0, 0.0f, 1);
		}
		break;
	}
	//}
}
//=========================================================================================================//
// * ゲームの描画処理
//=========================================================================================================//
void DrawStart(void)
{
	// カメラの設定
	SetCamera(0);

	// ポリゴンの描画処理
	DrawModel();
	DrawObject();
	DrawShadow();
	DrawZWindow();
	DrawWindow(WINDOW_DRAW_0);
	DrawFont(WINDOW_DRAW_1);
	DrawWindow(WINDOW_DRAW_1);
	DrawExplosion();
	DrawAnimation();

	DrawWindow(WINDOW_DRAW_2);
	DrawFont(WINDOW_DRAW_0);
}
//=========================================================================================================//
// * セーブロードのため
//=========================================================================================================//
Result *GetResult(void)
{
	static Result result;

	return &result;
}
//=========================================================================================================//
// * 名前入力の選択に応じた処理
//=========================================================================================================//
void InputName(int nMode)
{
	Result *pResult = GetResult();

	//PlaySound(SOUND_LABEL_BOTTON001);
	if (nMode == 1)
	{//ひら
		if (pResult[0].nWideFont == 0)
		{//ん行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ん"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "！"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "？"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { InputName3(); }
		}
		else if (pResult[0].nWideFont == 1)
		{//わ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "わ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ー"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "‐"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "～"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "を"); }
		}
		else if (pResult[0].nWideFont == 2)
		{//ら行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ら"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "り"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "る"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "れ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ろ"); }
		}
		else if (pResult[0].nWideFont == 3)
		{//や行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "や"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ゆ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "よ"); }
			if (pResult[0].nHightFont == 1) { InputName1(); }
			if (pResult[0].nHightFont == 3) { InputName2(); }
		}
		else if (pResult[0].nWideFont == 4)
		{//ま行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ま"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "み"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "む"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "め"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "も"); }
		}
		else if (pResult[0].nWideFont == 5)
		{//は行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "は"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ひ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ふ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "へ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ほ"); }
		}
		else if (pResult[0].nWideFont == 6)
		{//な行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "な"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "に"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ぬ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "ね"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "の"); }
		}
		else if (pResult[0].nWideFont == 7)
		{//た行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "た"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ち"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "つ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "て"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "と"); }
		}
		else if (pResult[0].nWideFont == 8)
		{//さ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "さ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "し"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "す"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "せ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "そ"); }
		}
		else if (pResult[0].nWideFont == 9)
		{//か行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "か"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "き"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "く"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "け"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "こ"); }
		}
		else if (pResult[0].nWideFont == 10)
		{//あ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "あ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "い"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "う"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "え"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "お"); }
		}
	}
	else if (nMode == 2)//---------------------------------------------------------------------//
	{//カナ
		if (pResult[0].nWideFont == 0)
		{//ン行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ン"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "！"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "？"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { InputName3(); }
		}
		else if (pResult[0].nWideFont == 1)
		{//ワ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ワ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ー"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "‐"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "～"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ヲ"); }
		}
		else if (pResult[0].nWideFont == 2)
		{//ラ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ラ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "リ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ル"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "レ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ロ"); }
		}
		else if (pResult[0].nWideFont == 3)
		{//ヤ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ヤ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ユ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ヨ"); }
			if (pResult[0].nHightFont == 1) { InputName1(); }
			if (pResult[0].nHightFont == 3) { InputName2(); }
		}
		else if (pResult[0].nWideFont == 4)
		{//マ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "マ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ミ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ム"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "メ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "モ"); }
		}
		else if (pResult[0].nWideFont == 5)
		{//ハ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ハ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ヒ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "フ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "ヘ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ホ"); }
		}
		else if (pResult[0].nWideFont == 6)
		{//ナ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ナ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ニ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ヌ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "ネ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ノ"); }
		}
		else if (pResult[0].nWideFont == 7)
		{//タ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "タ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "チ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ツ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "テ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ト"); }
		}
		else if (pResult[0].nWideFont == 8)
		{//サ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "サ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "シ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ス"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "セ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ソ"); }
		}
		else if (pResult[0].nWideFont == 9)
		{//カ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "カ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "キ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ク"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "ケ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "コ"); }
		}
		else if (pResult[0].nWideFont == 10)
		{//ア行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ア"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "イ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ウ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "エ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "オ"); }
		}
	}
	else if (nMode == 3)//---------------------------------------------------------------------//
	{//英語
		if (pResult[0].nWideFont == 0)
		{//0行目
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "０"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ａ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｌ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "Ｗ"); }
			if (pResult[0].nHightFont == 4) { InputName3(); }
		}
		else if (pResult[0].nWideFont == 1)
		{//1行目
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "１"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ｂ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｍ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "Ｘ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "　"); }
		}
		else if (pResult[0].nWideFont == 2)
		{//2行目
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "２"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ｃ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｎ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "Ｙ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "　"); }
		}
		else if (pResult[0].nWideFont == 3)
		{//3行目
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "３"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ｄ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｏ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "Ｚ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "　"); }
		}
		else if (pResult[0].nWideFont == 4)
		{//4行目
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "４"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ｅ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｐ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "　"); }
		}
		else if (pResult[0].nWideFont == 5)
		{//5行目
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "５"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ｆ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｑ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "　"); }
		}
		else if (pResult[0].nWideFont == 6)
		{//6行目
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "６"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ｇ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｒ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "　"); }
		}
		else if (pResult[0].nWideFont == 7)
		{//7行目
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "７"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ｈ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｓ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "　"); }
		}
		else if (pResult[0].nWideFont == 8)
		{//8行目
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "８"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ｉ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｔ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "　"); }
		}
		else if (pResult[0].nWideFont == 9)
		{//9行目
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "９"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ｊ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｕ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "　"); }
		}
		else if (pResult[0].nWideFont == 10)
		{//10行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "．"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "Ｋ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "Ｖ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "　"); }
		}
	}

	//文字をセットする
	ChangeFont(true, &pResult[0].aStr[0], 0, 0, -1, 0, FONTSTATE_NONE,D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
//=========================================================================================================//
// * 名前入力の選択に応じた処理　〃変換
//=========================================================================================================//
void InputName1(void)
{
	Result *pResult = GetResult();
	int nLength;
	nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認

	if (nLength > 1)
	{//〃に変換
		if (strcmp(&pResult[0].aStr[nLength - 2], "う") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヴ"); }
		//か行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "か") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "が"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "き") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぎ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "く") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぐ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "け") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "げ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "こ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ご"); }
		//さ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "さ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ざ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "し") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "じ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "す") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ず"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "せ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぜ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "そ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぞ"); }
		//た行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "た") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "だ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ち") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぢ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "つ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "づ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "て") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "で"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "と") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ど"); }
		//は行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "は") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ば"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ひ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "び"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ふ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぶ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "へ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "べ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ほ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぼ"); }

		//カ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ウ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヴ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "カ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ガ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "キ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ギ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ク") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "グ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ケ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゲ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "コ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゴ"); }
		//サ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "サ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ザ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "シ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ジ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ス") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ズ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "セ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゼ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ソ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゾ"); }
		//タ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "タ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ダ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "チ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヂ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ツ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヅ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "テ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "デ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ト") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ド"); }
		//ハ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ハ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "バ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヒ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ビ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "フ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ブ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヘ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ベ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ホ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ボ"); }
		//ぱ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ぱ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ば"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ぴ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "び"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ぷ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぶ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ぺ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "べ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ぽ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぼ"); }
		//パ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "パ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "バ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ピ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ビ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "プ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ブ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ペ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ベ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ポ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ボ"); }
	}
	else {}//失敗音
}
//=========================================================================================================//
// * 名前入力の選択に応じた処理　゜変換
//=========================================================================================================//
void InputName2(void)
{
	Result *pResult = GetResult();
	int nLength;
	nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認

	if (nLength > 1)
	{//〇に変換
	 //は行
		if (strcmp(&pResult[0].aStr[nLength - 2], "は") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぱ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ひ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぴ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ふ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぷ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "へ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぺ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ほ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぽ"); }
		//ハ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ハ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "パ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヒ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ピ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "フ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "プ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヘ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぺ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ホ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ポ"); }
		//ば
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ば") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぱ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "び") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぴ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ぶ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぷ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "べ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぺ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ぼ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぽ"); }
		//バ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "バ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "パ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ビ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ピ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ブ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "プ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ベ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぺ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ボ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ポ"); }
	}
	else {}//失敗音
}
//=========================================================================================================//
// * 名前入力の選択に応じた処理　小文字変換
//=========================================================================================================//
void InputName3(void)
{
	Result *pResult = GetResult();
	int nLength;
	nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認

	if (nLength > 1)
	{//小文字に変換
		if (strcmp(&pResult[0].aStr[nLength - 2], "あ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぁ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "い") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぃ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "う") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぅ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "え") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぇ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "お") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぉ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "つ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "っ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "や") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゃ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ゆ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゅ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "よ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ょ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "か") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヵ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "け") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヶ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "わ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゎ"); }

		//カタカナ
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ア") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ァ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "イ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ィ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ウ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゥ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "エ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ェ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "オ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ォ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ツ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ッ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヤ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ャ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ユ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ュ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヨ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ョ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "カ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヵ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ケ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヶ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ワ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヮ"); }
		
		//英語
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ａ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ａ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｂ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｂ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｃ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｃ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｄ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｄ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｅ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｅ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｆ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｆ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｇ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｇ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｈ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｈ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｉ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｉ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｊ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｊ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｋ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｋ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｌ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｌ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｍ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｍ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｎ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｎ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｏ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｏ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｐ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｐ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｑ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｑ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｒ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｒ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｓ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｓ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｔ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｔ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｕ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｕ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｖ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｖ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｗ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｗ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｘ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｘ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｙ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｙ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "Ｚ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ｚ"); }
		//
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ａ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ａ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｂ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｂ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｃ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｃ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｄ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｄ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｅ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｅ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｆ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｆ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｇ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｇ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｈ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｈ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｉ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｉ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｊ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｊ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｋ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｋ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｌ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｌ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｍ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｍ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｎ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｎ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｏ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｏ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｐ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｐ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｑ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｑ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｒ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｒ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｓ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｓ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｔ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｔ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｕ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｕ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｖ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｖ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｗ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｗ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｘ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｘ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｙ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｙ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ｚ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "Ｚ"); }
	}
	else {}//失敗音
}