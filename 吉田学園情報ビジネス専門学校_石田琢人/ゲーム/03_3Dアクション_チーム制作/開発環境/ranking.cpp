//=========================================================================================================//
//
// タイトルの処理 [Title.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <stdio.h>				//インクルドファイル セーブロードのため
#include "game.h"
#include "main.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//


//=========================================================================================================//
// * ゲームの初期化処理
//=========================================================================================================//
void InitRanking(void)
{
	int *pTime = GetTime();
	pTime[0] = 0;

	SetExplosion(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 5, 1, 2, ANIMATION_LOOP, EXPLOSIONSTATE_FADE, D3DX_PI * 0.0f, 1);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 450, 300, 15, 0, 1, 1, 0, WINDOW_UPDATE_SELECT, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.07f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 180, 80, 14, 0, 1, 1, 0, WINDOW_UPDATE_SELECT, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_1);

	//ロード
	FILE *pFile;
	CHAR_DATA *pChar_Data = GetChar_Data();

	InitChar_Data(1);

	//初期化
	int nCountData[2];	//カウント用
	CHAR_DATA nData;

	for (nCountData[0] = 1; nCountData[0] < 16; nCountData[0]++)
	{//並び替え初期化
		for (nCountData[1] = 0; nCountData[1] < nCountData[0]; nCountData[1]++)
		{
			if (pChar_Data[nCountData[0]].nStrength > pChar_Data[nCountData[1]].nStrength)
			{//大きい順に前へ　(大小比較)
				nData = pChar_Data[nCountData[1]];	//保管(小)
				pChar_Data[nCountData[1]] = pChar_Data[nCountData[0]];	//入れ替え(大)
				pChar_Data[nCountData[0]] = nData;	//入れ替え(小)
			}
		}
	}

	pFile = fopen("data/Ranking.bin", "rb");

	if (pFile != NULL)
	{//ロード　成功
	 //fread(&pChar_Data[0], sizeof(CHAR_DATA), 5, pFile);

		fclose(pFile);
	}
	else
	{//失敗
		MessageBox(0, "", "ロードに失敗しました", MB_OK);
	}

	GAME_PLAY *pGamePlay = GetGame_Play();
	int nStrong = -1;

	if (pGamePlay[0].bUse == true)
	{//入れ替えがあるなら
		int nCountData;	//カウント用
		CHAR_DATA Data;
		MODEL_INFO *pmodel = GetModel_Info();

		pChar_Data[5] = pmodel[0].data;
		nStrong = pChar_Data[5].nStrength;
		for (nCountData = 0; nCountData < 5; nCountData++)
		{
			if (pChar_Data[nCountData].nStrength < pChar_Data[5].nStrength)
			{//大きい順に前へ　(大小比較)
				Data = pChar_Data[5];	//保管(小)
				pChar_Data[5] = pChar_Data[nCountData];	//入れ替え(大)
				pChar_Data[nCountData] = Data;	//入れ替え(小)
			}
		}
	}

	pFile = fopen("data/Ranking.bin", "wb");

	if (pFile != NULL)
	{//Save　成功
		fwrite(&pChar_Data[0], sizeof(CHAR_DATA), 5, pFile);

		fclose(pFile);
	}
	else
	{//失敗
		MessageBox(0, "", "セーブに失敗しました", MB_OK);
	}

	char aStr[48];
	int nLength;

	for (int nCount = 0; nCount < 5; nCount++)
	{
		strcpy(&aStr[0], &pChar_Data[nCount].aName[0]);
		nLength = (int)strlen(&aStr[0]);//文字数確認
		wsprintf(&aStr[nLength], "  %d", (int)pChar_Data[nCount].nStrength);
		if (nStrong == pChar_Data[nCount].nStrength)
		{
			SetFont(RECT{ 350 , nCount * 125 + 80, 1200, 720 }, D3DXCOLOR{ 1.0f, 0.95f, 0.08f , 1.0f }, &aStr[0], nCount * 2 + 1, 0, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);
		}
		else
		{
			SetFont(RECT{ 350 , nCount * 125 + 80, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0f , 1.0f }, &aStr[0], nCount * 2 + 1, 3, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);
		}
	}
}
//=========================================================================================================//
// * ゲームの終了処理
//=========================================================================================================//
void UninitRanking(void)
{
	BreakFont();
	BreakExplosion();
	BreakWindow();
}
//=========================================================================================================//
// * ゲームの更新処理
//=========================================================================================================//
void UpdateRanking(void)
{
	PLAYER *pPlayer = GetPlayer();
	int *pTime = GetTime();
	pTime[0]++;

	UpdateExplosion();
	UpdateFont();
	if (pPlayer[0].bButton[BUTTON_ENTER] == true || pTime[0] == 216)
	{
		SetFade(MODE_TITLE, 0, 0.0f, 0);
	}
}
//=========================================================================================================//
// * ゲームの描画処理
//=========================================================================================================//
void DrawRanking(void)
{
	DrawExplosion();
	DrawWindow(WINDOW_DRAW_1);
	DrawFont(WINDOW_DRAW_1);
	DrawWindow(WINDOW_DRAW_2);
	DrawFont(WINDOW_DRAW_0);
}
