//=========================================================================================================//
//
// 背景処理 [bg.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "camera.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define MAX_FONT			(48)
#define MAX_FONT_TYPE		(8)
#define SETFONT_WIDE		(SCREEN_WIDTH * 0.35)
#define SETFONT_HEIGHT		(SCREEN_HEIGHT * 0.4)
#define Font_TextureName	"data/TEXTURE/50on.jpg"		//50音表

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9 g_pTextureFont = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFont = NULL;	//頂点BUFFERへのポインタ
LPD3DXFONT			g_pFont[MAX_FONT_TYPE] = {};			// フォントへのポインタ
Font				g_Font[MAX_FONT];
int					g_nSetFont = 0;
//=========================================================================================================//
// 初期化処理
//=========================================================================================================//
void InitFont(void)
{
	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		//g_Font[nCntFont].rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
		g_Font[nCntFont].bUse = false;
		g_Font[nCntFont].bDisplay = false;
		g_Font[nCntFont].draw = WINDOW_DRAW_2;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice, Font_TextureName, &g_pTextureFont);	//タイトル

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffFont,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFont->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f - SETFONT_WIDE, SCREEN_HEIGHT * 0.5f - SETFONT_HEIGHT, 0.0f);		//X,Y,0.0f
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f + SETFONT_WIDE, SCREEN_HEIGHT * 0.5f - SETFONT_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f - SETFONT_WIDE, SCREEN_HEIGHT * 0.5f + SETFONT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f + SETFONT_WIDE, SCREEN_HEIGHT * 0.5f + SETFONT_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラー設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// A = 透明度
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f);

	//頂点BUFFERをアンロックする
	g_pVtxBuffFont->Unlock();

	D3DXCreateFont(pDevice, 48, 18, 0, 0, FALSE, MONO_FONT,
		OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Font", &g_pFont[0]);
	D3DXCreateFont(pDevice, 40, 12, 0, 0, FALSE, MONO_FONT,
		OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Font", &g_pFont[1]);
	D3DXCreateFont(pDevice, 25, 9, 0, 0, FALSE, MONO_FONT,
		OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Font", &g_pFont[2]);
	D3DXCreateFont(pDevice, 58, 22, 0, 0, FALSE, MONO_FONT,
		OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Font", &g_pFont[3]);
	D3DXCreateFont(pDevice, 100, 34, 0, 0, FALSE, MONO_FONT,
		OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Font", &g_pFont[4]);
	D3DXCreateFont(pDevice, 68, 32, 0, 0, FALSE, MONO_FONT,
		OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Font", &g_pFont[5]);
	D3DXCreateFont(pDevice, 110, 44, 0, 0, FALSE, MONO_FONT,
		OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Font", &g_pFont[6]);
	D3DXCreateFont(pDevice, 34, 11, 0, 0, FALSE, MONO_FONT,
		OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Font", &g_pFont[7]);
}
//=========================================================================================================//
// 終了処理
//=========================================================================================================//
void UninitFont(void)
{
	BreakFont();
	for (int nCntFont = 0; nCntFont < MAX_FONT_TYPE; nCntFont++)
	{
		if (g_pFont[nCntFont] != NULL)
		{// デバッグ表示用フォントの開放
			g_pFont[nCntFont]->Release();
			g_pFont[nCntFont] = NULL;
		}
	}

	if (g_pTextureFont != NULL)
	{
		g_pTextureFont->Release();
		g_pTextureFont = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffFont != NULL)
	{
		g_pVtxBuffFont->Release();
		g_pVtxBuffFont = NULL;
	}
}
//=========================================================================================================//
// 更新処理
//=========================================================================================================//
void UpdateFont(void)
{
	int nCntFont;
	Camera *pCamera = GetCamera();

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (g_Font[nCntFont].bUse == true)
		{//文字が表示状態なら
			switch (g_Font[nCntFont].state)
			{
			case FONTSTATE_NONE:
				if (pCamera[0].state == CAMERA_STATE_MOVIE) { g_Font[nCntFont].bDisplay = false; }
				else { g_Font[nCntFont].bDisplay = true; }
				break;
			case FONTSTATE_DELETE:
				g_Font[nCntFont].rect.bottom -= 1;
				g_Font[nCntFont].rect.top -= 1;
				g_Font[nCntFont].col.a -= 0.01f;
				if (g_Font[nCntFont].col.a < 0.0f) { g_Font[nCntFont].bUse = false; }
				break;
			case FONTSTATE_FLASH:
				g_Font[nCntFont].nCntState++;
				if (g_Font[nCntFont].nCntState % 2 == 0) { g_Font[nCntFont].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);}
				else{ g_Font[nCntFont].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
				break;
			case FONTSTATE_FLASH0:
				g_Font[nCntFont].nCntState++;
				if (g_Font[nCntFont].nCntState % 2 == 0) { g_Font[nCntFont].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f); }
				else { g_Font[nCntFont].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); }
				break;
			case FONTSTATE_FLOW:
				if (g_Font[nCntFont].nCntState == 0)
				{//左から右に流れながら表示
					g_Font[nCntFont].col.a += 0.05f;
					g_Font[nCntFont].rect.left += 20;
					g_Font[nCntFont].rect.right += 20;
					if (g_Font[nCntFont].col.a >= 1.0f)
					{
						g_Font[nCntFont].col.a = 1.0f;
						g_Font[nCntFont].nCntState = 1;
					}
				}
				else
				{//時間経過で
					g_Font[nCntFont].nCntState++;
					if (g_Font[nCntFont].nCntState > 120)
					{//さらに時間経過で
						g_Font[nCntFont].col.a -= 0.029f;
						g_Font[nCntFont].rect.left += 8;
						g_Font[nCntFont].rect.right += 8;
						if (g_Font[nCntFont].col.a <= 0.0f)
						{//右に流れながら消える
							g_Font[nCntFont].col.a = 0.0f;
							g_Font[nCntFont].bUse = false;
						}
					}
				}
			}
		}
	}
}

//=========================================================================================================//
// 描画処理
//=========================================================================================================//
void DrawFont(WINDOW_DRAW draw)
{
	int nCntFont;
	D3DCOLOR col;
	RECT rect;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char aStr[84];
	char aStr0[84];		//\n確認用
	int nLength;
	
	if (g_nSetFont != 0 && draw == WINDOW_DRAW_1)
	{//50音表表示
		VERTEX_2D *pVtx;		//頂点情報へのポインタ
		//頂点BUFFERをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFont->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, (g_nSetFont - 1.0f) * (1.0f / 3.0f));
		pVtx[1].tex = D3DXVECTOR2(1.0f, (g_nSetFont - 1.0f) * (1.0f / 3.0f));
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_nSetFont * (1.0f / 3.0f));
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_nSetFont * (1.0f / 3.0f));

		//頂点BUFFERをアンロックする
		g_pVtxBuffFont->Unlock();

		LPDIRECT3DDEVICE9 pDevice;
		//デバイスを取得する
		pDevice = GetDevice();


		//頂点BUFFERをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffFont, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);	// 頂点フォーマットの設定

										//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureFont);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	// テキスト描画
	// hDC      : デバイスコンテキストのハンドル
	// lpString : 描画する文字列へのポインタ
	// nCount   : 文字列の文字数
	// lpRect   : 長方形領域
	// uFormat  : テキストの整形方法を指定

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (g_Font[nCntFont].bUse == true && g_Font[nCntFont].bDisplay == true && g_Font[nCntFont].draw == draw)
		{// 文字列を代入 表示
			strcpy(&aStr[0], &g_Font[nCntFont].aStr[0]);
			if (g_Font[nCntFont].nMaxFont > -1)
			{//文字数が決められている
				nLength = (int)strlen(&aStr[0]);
				if (nLength > g_Font[nCntFont].nMaxFont)
				{//文字列のほうが長い
					strcpy(&aStr[g_Font[nCntFont].nMaxFont], "\0");
					g_Font[nCntFont].nCntTime--;
					if (g_Font[nCntFont].nCntTime == 0)
					{//カウントが進んだ
						strcpy(&aStr0[0], &g_Font[nCntFont].aStr[0]);
						strcpy(&aStr0[g_Font[nCntFont].nMaxFont + 1], "\0");
						if(strcmp(&aStr0[g_Font[nCntFont].nMaxFont], "\n") == 0){ g_Font[nCntFont].nMaxFont++; }
						g_Font[nCntFont].nCntTime = SPEECH_FLOW;
						g_Font[nCntFont].nMaxFont += 2;
						if (nLength <= g_Font[nCntFont].nMaxFont){ g_Font[nCntFont].nMaxFont = -1; }
					}
				}
			}

			if (g_Font[nCntFont].bshadow == true)
			{
				col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				rect = g_Font[nCntFont].rect;
				rect.left += 2; rect.top += 2; rect.right += 2; rect.bottom += 2;
				g_pFont[g_Font[nCntFont].nType]->DrawText(NULL, &aStr[0], -1, &rect, g_Font[nCntFont].nFormat, col);
			}
			col = g_Font[nCntFont].col;
			g_pFont[g_Font[nCntFont].nType]->DrawText(NULL, &aStr[0], -1, &g_Font[nCntFont].rect, g_Font[nCntFont].nFormat, col);
		}
	}
	//DT_LEFT;
}
//=========================================================================================================//
// 文字代入処理  Set!!
//=========================================================================================================//
void SetFont(RECT rect, D3DXCOLOR col, char *aStr, int nNumber, int nType, int nFormat, int nMaxFont, bool bShadow, FONTSTATE state, WINDOW_DRAW draw)
{//文字セット
	g_Font[nNumber].bUse = true;
	g_Font[nNumber].bDisplay = true;
	g_Font[nNumber].draw = draw;
	g_Font[nNumber].state = state;
	g_Font[nNumber].nType = nType;
	g_Font[nNumber].nFormat = nFormat;
	g_Font[nNumber].nMaxFont = nMaxFont;
	g_Font[nNumber].nCntTime = 0;
	g_Font[nNumber].rect = rect;
	strcpy(&g_Font[nNumber].aStr[0], &aStr[0]);
	g_Font[nNumber].col = col;
	g_Font[nNumber].nCntState = 0;
	g_Font[nNumber].bshadow = bShadow;
}
//=========================================================================================================//
// 文字代入処理  Set!!
//=========================================================================================================//
void SetFontDamage(RECT rect, D3DCOLOR col, char *aStr, int nWide, int nHeight, FONTSTATE state)
{//文字セット
	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (g_Font[nCntFont].bUse == false)
		{
			g_Font[nCntFont].bUse = true;
			g_Font[nCntFont].bDisplay = true;
			g_Font[nCntFont].state = state;
			g_Font[nCntFont].rect = rect;
			strcpy(&g_Font[nCntFont].aStr[0], &aStr[0]);
			g_Font[nCntFont].col = col;
			g_Font[nCntFont].nCntState = 0;
			break;
		}
	}

}
//=========================================================================================================//
// 文字代入処理  Change!!
//=========================================================================================================//
void ChangeFont(bool bUse, char *aStr, int nNumber, int nStrType, int nMaxFont, int nCntTime, FONTSTATE state, D3DCOLOR col)
{//セットされたものを変更
	if (g_Font[nNumber].bUse == true)
	{
		g_Font[nNumber].bUse = bUse;
		if (bUse == true)
		{
			g_Font[nNumber].state = state;
			g_Font[nNumber].nMaxFont = nMaxFont;
			g_Font[nNumber].nCntTime = nCntTime;
			g_Font[nNumber].col = col;

			switch (nStrType)
			{
			case 0:
				if (aStr[0] != NULL) { strcpy(&g_Font[nNumber].aStr[0], &aStr[0]); }//NULLじゃなければ更新
				break;
			case 1:
				g_Font[nNumber].nMaxFont = (int)strlen(&g_Font[nNumber].aStr[0]);
				if (aStr[0] != NULL) { strcat(&g_Font[nNumber].aStr[0], &aStr[0]); }//NULLじゃなければ更新
				break;
			}
		}
	}
}
//=========================================================================================================//
// 文字を全て破壊
//=========================================================================================================//
void BreakFont(void)
{//セットされた文字をすべて破壊
	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		g_Font[nCntFont].bUse = false;
	}

}
//=========================================================================================================//
// セットされている50音の取得
//=========================================================================================================//
int *GetSetFont(void)
{
	return &g_nSetFont;
}
//=========================================================================================================//
// * 全Animationスクロール処理
//=========================================================================================================//
void FontScroll(D3DXVECTOR3 move)
{
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_FONT; nCntItem++)
	{
		if (g_Font[nCntItem].bUse == true && g_Font[nCntItem].state == FONTSTATE_DELETE)
		{//スクロール
			//g_Font[nCntItem].rect.top += (int)move.y;
			//g_Font[nCntItem].rect.bottom += (int)move.y;
			//g_Font[nCntItem].rect.left += (int)move.x;
			//g_Font[nCntItem].rect.right += (int)move.x;
		}
	}
}