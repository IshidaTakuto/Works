//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// デバッグログ表示処理 [Debug_Log.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "Debug_Log.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPD3DXFONT		g_pFont = NULL;				// フォントへのポインタ
PLAYERINDEX		g_nPlayer = PLAYERINDEX_1;	// 表示するプレイヤーの情報

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitDebug_Log(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// デバッグ表示用フォントを設定
	// pDevice         : デバイスへのポインタ
	// Height          : 文字の高さ
	// Width           : 文字の幅
	// Weight          : フォントの太さ
	// MipLevels       : ミップマップレベル数
	// Italic          : イタリックフォント
	// CharSet         : フォントの文字セット
	// OutputPrecision : 実際のフォントと目的のフォントのサイズおよび特性の一致方法を指定
	// Quality         : 実際のフォントと目的のフォントとの一致方法を指定
	// PitchAndFamily  : ピッチとファミリインデックス
	// pFacename       : フォントの名前を保持する文字列
	// ppFont          : フォントへのポインタ
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitDebug_Log(void)
{
	if (g_pFont != NULL)
	{// デバッグ表示用フォントの開放
		g_pFont->Release();
		g_pFont = NULL;
	}
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdateDebug_Log(void)
{

}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawDebug_Log(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[1024 * 3];		// 文字列を格納する
	int nStr = 0;			// 文字列の最終位置を保存する
	// FPSを取得
	int nFPS = GetFPS();
	// カメラ情報を取得
	Camera *pCamera = GetCamera();
	// プレイヤー情報を取得
	Player *pPlayer = GetPlayer();

	//--- FPS 表示 ---//
	// 文字列を代入
	wsprintf(&aStr[0], "FPS:%d\n", nFPS);

	//--- フィールド情報 ---//
	// 文字列の位置最終を進める
	nStr = strlen(aStr);
	// 文字列を代入
	wsprintf(&aStr[nStr], "\n//--- カメラ ---//\n");
	// 文字列の位置最終を進める
	nStr = strlen(aStr);
	// 文字列を代入
	wsprintf(&aStr[nStr], "視点 　: (%d ,%d, %d)\n",(int)pCamera->posV.x, (int)pCamera->posV.y, (int)pCamera->posV.z);
	// 文字列の位置最終を進める
	nStr = strlen(aStr);
	// 文字列を代入
	wsprintf(&aStr[nStr], "注視点 : (%d ,%d, %d)\n", (int)pCamera->posR.x, (int)pCamera->posR.y, (int)pCamera->posR.z);
	// 文字列の位置最終を進める
	nStr = strlen(aStr);
	// 文字列を代入
	wsprintf(&aStr[nStr], "カメラの向き : %d\n", (int)(pCamera->rot.y * 100));

	//--- プレイヤー情報 ---//
	// 文字列の位置最終を進める
	nStr = strlen(aStr);
	// 文字列を代入
	wsprintf(&aStr[nStr], "プレイヤーの位置 : (%d ,%d ,%d)\n", (int)pPlayer->pos.x, (int)pPlayer->pos.y, (int)pPlayer->pos.z);
	// 文字列の位置最終を進める
	nStr = strlen(aStr);
	// 文字列を代入
	wsprintf(&aStr[nStr], "プレイヤーの向き : (%d ,%d ,%d)\n", (int)(pPlayer->rot.x * 100), (int)(pPlayer->rot.y * 100), (int)(pPlayer->rot.z * 100));
	// 文字列の位置最終を進める
	nStr = strlen(aStr);
	// 文字列を代入
	wsprintf(&aStr[nStr], "現在のフレーム数 : [ %d : %d ]\n", pPlayer->nNowFrame, pPlayer->nFrameAll);
	// 文字列の位置最終を進める
	nStr = strlen(aStr);
	// 文字列を代入
	wsprintf(&aStr[nStr], "左漕ぎフラグ : %d\n", pPlayer->bLeft);
	// 文字列の位置最終を進める
	nStr = strlen(aStr);
	// 文字列を代入
	wsprintf(&aStr[nStr], "右漕ぎフラグ : %d\n", pPlayer->bRight);

	// テキスト描画
	// hDC      : デバイスコンテキストのハンドル
	// lpString : 描画する文字列へのポインタ
	// nCount   : 文字列の文字数
	// lpRect   : 長方形領域
	// uFormat  : テキストの整形方法を指定
	//g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
}
