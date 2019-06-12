//=============================================================================
//
// 操作制御処理 [operation.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "operation.h"
#include "string.h"
#include "input.h"
#include "player.h"
#include "particle.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define STRING_FILE	"data/string.txt"		// 文字列の情報を格納するファイル

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================
int g_nNumMode;		// モードの制御
char g_aStrData[2][MAX_STRING];			// モードによって中身が変わる文字列の表示
char g_aString[TYPE_MAX][MAX_STRING];	// ファイルからのデータを格納
char g_aButton[TYPE_MAX][MAX_STRING];	// 使うキーの表示

bool g_bMove[MAX_EMITTER];		// パーティクルを動かすかどうか
char g_aStr[2][MAX_STRING];		// 文字列を格納

int g_nSelectParticle;				// パーティクル番号の指定

//=============================================================================
// 初期化処理
//=============================================================================
void InitOperation(void)
{
	g_nNumMode = 1;

	// パーティクル初期化処理
	InitParticle();

	// グローバル変数の初期化
	g_nSelectParticle = 0;

	strcpy(&g_aString[0][0], "[モード選択]");
	strcpy(&g_aString[1][0], "[ファイル出力]");
	strcpy(&g_aString[2][0], "[数値変更]");
	strcpy(&g_aString[3][0], "[項目切り替え]");
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitOperation(void)
{
	UninitParticle();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateOperation(void)
{
	int nSelectEmitter = 0;	// エミッタ番号を記憶
	Player *pPlayer;

	pPlayer = GetPlayer();

	switch (g_nNumMode)
	{
	case MODE_PARTICLE:		// パーティクル
		sprintf(&g_aStrData[0][0], "[ MODE_PARTICLE ]");		// 現在のモードを表示
		sprintf(&g_aStrData[1][0], "[ data/TEXT/EFFECT/save_particle.txt ]");	// 保存するファイル名

		if (GetKeyboardTrigger(DIK_P) == true)
		{// パーティクルのエミッタを出す
			SetParticleEmitter(pPlayer->pos);
		}

		// エフェクト番号の指定
		if (GetKeyboardRepeat(DIK_2) == true)
		{// 加算する
			if (g_nSelectParticle < MAX_EMITTER)
			{// 最大を越さない
				g_nSelectParticle++;
			}
		}
		else if (GetKeyboardRepeat(DIK_1) == true)
		{// 減算する
			if (g_nSelectParticle > 0)
			{// 0より下げない
				g_nSelectParticle--;
			}
		}
		wsprintf(&g_aStr[1][0], "[ %d ]", g_nSelectParticle);

		if (GetKeyboardTrigger(DIK_DELETE) == true)
		{// 選択中のパーティクルを消す
			DestroyParticle(g_nSelectParticle);
		}

		if (GetKeyboardTrigger(DIK_M) == true)
		{// パーティクルを動かすかどうか
			g_bMove[g_nSelectParticle] = g_bMove[g_nSelectParticle] ? false : true;
		}
		wsprintf(&g_aStr[0][0], "[ %s ]", g_bMove[g_nSelectParticle] ? "する" : "しない");

		for (int nCount = 0; nCount < MAX_EMITTER; nCount++)
		{
			if (g_bMove[nCount] == true)
			{// 動いている状態のとき
				SetPosParticleEmitter(nCount, pPlayer->pos);	// 位置を更新
			}
		}

		// パーティクルの更新
		UpdateParticle();

		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawOperation(void)
{
	// 現在のモード
	SetString(&g_aStrData[0][0], { 300,10,0,0 }, 0, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	// 出力するファイル名
	SetString(&g_aStrData[1][0], { 300,30,0,0 }, 0, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	// 左上部分---------------------------------------------------------------------------------------
	strcpy(&g_aButton[TYPE_MODE][0], &MODE_CHANGE.aKeyNamy[0]);

	strcpy(&g_aButton[TYPE_SELECT][0], &SELECT_DOWN.aKeyNamy[0]);
	strcat(&g_aButton[TYPE_SELECT][0], " ");
	strcat(&g_aButton[TYPE_SELECT][0], &SELECT_UP.aKeyNamy[0]);

	strcpy(&g_aButton[TYPE_NUM][0], &NUM_DOWN.aKeyNamy[0]);
	strcat(&g_aButton[TYPE_NUM][0], " ");
	strcat(&g_aButton[TYPE_NUM][0], &NUM_UP.aKeyNamy[0]);
	strcat(&g_aButton[TYPE_NUM][0], " (一部LSHIFT対応)");

	strcpy(&g_aButton[TYPE_SAVE_FILE][0], &FILE_OUTPUT.aKeyNamy[0]);

	for (int nCntString = 0; nCntString < TYPE_MAX; nCntString++)
	{// 描画する
		SetString(&g_aString[nCntString][0], { 10, 10 + (nCntString * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString(&g_aButton[nCntString][0], { 200, 10 + (nCntString * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	SetString("[ 選択中の数値リセット ]", { 10,10 + (TYPE_MAX * 20),0,0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetString("[ ENTER ]", { 200,10 + (TYPE_MAX * 20),0,0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	SetString("[ カラーの変更]", { 10, 10 + ((TYPE_MAX + 1) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetString("[ (赤:r 緑:g 青:b 透明度:t) 押しながら]", { 200, 10 + ((TYPE_MAX + 1) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	switch (g_nNumMode)
	{
	case MODE_PARTICLE:		// パーティクル
		SetString("[ 移動量の変更]", { 10, 10 + ((TYPE_MAX + 2) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString("[ (Ｘ軸:x Ｙ軸:y Ｚ軸:z) 押しながら ]", { 200, 10 + ((TYPE_MAX + 2) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		SetString("[ パーティクルを生成 ]", { 10, 10 + ((TYPE_MAX + 3) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString("[ P ]", { 200, 10 + ((TYPE_MAX + 3) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		SetString("[ 移動の有無 ]", { 10, 10 + ((TYPE_MAX + 4) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString("[ M ]", { 200, 10 + ((TYPE_MAX + 4) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString(&g_aStr[0][0], { 300, 10 + ((TYPE_MAX + 4) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		SetString("[ 選択中のパーティクル ]", { 10, 10 + ((TYPE_MAX + 5) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString("[ 1 2 ]", { 200, 10 + ((TYPE_MAX + 5) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString(&g_aStr[1][0], { 300, 10 + ((TYPE_MAX + 5) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		DrawParticle();
		break;
	}

}