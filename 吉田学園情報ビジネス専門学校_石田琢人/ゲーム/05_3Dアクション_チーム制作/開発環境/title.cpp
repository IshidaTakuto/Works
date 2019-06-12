//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : shotaro kanamaru
//
//=============================================================================
#include "main.h"		// メイン
#include "title.h"		// タイトル
#include "input.h"		// 入力
#include "sound.h"		// 音
#include "fade.h"		// フェード
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "wall.h"
#include "object.h"
#include "objectnot.h"
#include "player.h"
#include "shadow.h"
#include "meshbg.h"
#include "rain.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_TITLE		(10)				// 最大数
#define TITLE_DOTSIZE	(4.0f)				// ドットサイズ
// 爪痕
#define TITLESCARS_TEX		"data/TEXTURE/title_scar.png"		// テクスチャ
#define TITLESCARS_START	(30)								// 処理開始の時間
#define TITLESCARS_ANIM		(6)									// アニメーション数
#define TITLESCARS_SWITCH	(4)									// アニメーション速度
#define TITLESCARS_SIZE		D3DXVECTOR3(124.0f,145.0f,0.0f)		// 大きさ
#define TITLESCARS_POS		D3DXVECTOR3(440.0f, 320.0f, 0.0f)	// 初期位置
// タイトルアイコン
#define TITLEICON_TEX		"data/TEXTURE/title_icon.png"		// テクスチャ
#define TITLEICON_START		(43)								// 処理開始の時間
#define TITLEICON_SIZE		D3DXVECTOR3(98.0f,79.0f,0.0f)		// 大きさ
#define TITLEICON_POS		D3DXVECTOR3(440.0f, 320.0f, 0.0f)	// 初期位置
// タイトル文字
#define TITLECHAR_TEX		"data/TEXTURE/title_char.png"		// テクスチャ
#define TITLECHAR_START		(78)								// 処理開始の時間
#define TITLECHAR_SIZE		D3DXVECTOR3(144.0f,51.0f,0.0f)		// 大きさ
#define TITLECHAR_POS		D3DXVECTOR3(440.0f, 500.0f, 0.0f)	// 初期位置
#define TITLECHAR_POSY		(320.0f)							// 指定y位置
// キー入力促し
#define TITLEPRESS_TEX		"data/TEXTURE/pressany.png"			// テクスチャ
#define TITLEPRESS_SIZE		D3DXVECTOR3(200.0f,40.0f,0.0f)		// 大きさ
#define TITLEPRESS_POS		D3DXVECTOR3(440.0f, 550.0f, 0.0f)	// 初期位置
#define TITLE_COLASPEED		(0.08f)								// 透明度の変化速度

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
void TitleMakeVtx(void);						// 頂点情報の作成
void TitleScars(int nIndex);					// 爪痕処理
void TitleIcon(int nIndex);						// タイトルアイコンの処理
void TitleChar(int nIndex);						// タイトル文字の処理
void TitlePress(int nIndex);					// キー入力促しの処理
void SkipEvent(int nIndex);						// 演出スキップの処理
void TitleColAFixed(int nIndex);				// 透明度の幅の固定

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Title			g_aTitle[MAX_TITLE];			// タイトル情報
TitleOther		g_titleOther;					// その他設定情報

//=============================================================================
//タイトルの初期化処理
//=============================================================================
void InitTitle(void)
{
	// タイトルの設定情報の初期化
	g_titleOther.nNum = 0;								// 総数
	g_titleOther.nCntAll = 0;							// 共通カウンタ
	g_titleOther.nPatternAnim = 0;						// アニメーションパターン
	g_titleOther.fAnimWidth = 1.0f / TITLESCARS_ANIM;	// アニメーションの幅
	g_titleOther.nCntChar = 0;							// タイトル文字カウンタ
	g_titleOther.nCntFlash = 0;							// フラッシュカウンタ
	g_titleOther.bFlash = false;						// フラッシュの使用
	g_titleOther.bPress = false;						// キー入力促しの使用
	g_titleOther.bEvent = true;							// 演出

	// 項目ごとの情報設定
	TitleInfo aTitletype[] =
	{// [位置]:[大きさ]:[種類]
		/*爪痕*/			TITLESCARS_POS,	TITLESCARS_SIZE,TITLE_SCARS,
		/*タイトルアイコン*/TITLEICON_POS,	TITLEICON_SIZE,	TITLE_ICON,
		/*タイトル文字*/	TITLECHAR_POS,	TITLECHAR_SIZE,	TITLE_CHAR,
		/*キー入力促し*/	TITLEPRESS_POS,	TITLEPRESS_SIZE,TITLE_PRESS,
	};

	// 設定した数分総数を加算
	g_titleOther.nNum += sizeof aTitletype / sizeof(TitleInfo);

	for (int nCntTitle = 0; nCntTitle < g_titleOther.nNum; nCntTitle++)
	{
		g_aTitle[nCntTitle].pos = aTitletype[nCntTitle].pos;		// 位置
		g_aTitle[nCntTitle].posInit = g_aTitle[nCntTitle].pos;		// 初期位置
		g_aTitle[nCntTitle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aTitle[nCntTitle].size = aTitletype[nCntTitle].size * TITLE_DOTSIZE / 2;	// 大きさ
		g_aTitle[nCntTitle].type = aTitletype[nCntTitle].type;		// タイプ
		g_aTitle[nCntTitle].col = D3DXCOLOR(0.4f, 0.7f, 0.5f, 0.0f);// 色
	}

	g_aTitle[TITLE_SCARS].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);// 色

	//カメラの初期化処理
	InitCamera();

	InitLight();

	InitMeshField();

	InitMeshBg();

	InitShadow();

	InitPlayer();

	InitWall();

	InitObject();

	InitObjectNot();

	InitRain();

	//壁の配置
	//上
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 870.0f), D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f));
	//右
	SetWall(D3DXVECTOR3(870.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f));
	//下
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -870.0f), D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f));
	//左
	SetWall(D3DXVECTOR3(-870.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f));

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

	TitleMakeVtx();		// 頂点情報の作成
}

//=============================================================================
//タイトルの終了処理
//=============================================================================
void UninitTitle(void)
{
	// 頂点バッファの破棄
	if (g_titleOther.pVtxBuff != NULL)
	{
		g_titleOther.pVtxBuff->Release();
		g_titleOther.pVtxBuff = NULL;
	}
	// テクスチャの破棄
	for (int nCntType = 0; nCntType < TITLE_MAX; nCntType++)
	{
		if (g_titleOther.pTexture[nCntType] != NULL)
		{
			g_titleOther.pTexture[nCntType]->Release();
			g_titleOther.pTexture[nCntType] = NULL;
		}
	}

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	UninitWall();

	UninitShadow();

	UninitPlayer();

	UninitObject();

	UninitObjectNot();

	UninitMeshField();

	UninitRain();

	UninitMeshBg();
}

//=============================================================================
//タイトルの更新処理
//=============================================================================
void UpdateTitle(void)
{
	VERTEX_2D*pVtx;  //頂点情報へのポインタ

	if (g_titleOther.pVtxBuff != NULL)
	{
		g_titleOther.nCntAll++;		// 共通カウンタの加算

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_titleOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ここでSetFadeなどお願いします
		FADE fade = *GetFade();
		if (fade == FADE_NONE)
		{
			if (GetAnyTrigger(1, 0) == true || GetKeyboardTrigger(DIK_RETURN))
			{// 決定キー押下
				PlaySound(SOUND_LABEL_SELECT);
				SetFadeMode(MODE_TUTORIAL);
				g_titleOther.bFlash = true;		// 文字のフラッシュ
				g_titleOther.bPress = false;	// キー入力促しをoffに
			}
		}

		SkipEvent(0);

		for (int nCntTitle = 0; nCntTitle < g_titleOther.nNum; nCntTitle++, pVtx += 4)
		{
			TitleScars(nCntTitle);		// 爪痕の処理
			TitleIcon(nCntTitle);		// タイトルアイコンの処理
			TitleChar(nCntTitle);		// タイトル文字の処理
			TitlePress(nCntTitle);		// キー入力促しの処理
			TitleColAFixed(nCntTitle);	// 透明度の幅の固定

			// 慣性の加算
			g_aTitle[nCntTitle].move += (D3DXVECTOR3(0.0f, 0.0f, 0.0f) - g_aTitle[nCntTitle].move) * 0.1f;

			// 移動量の加算
			g_aTitle[nCntTitle].pos += g_aTitle[nCntTitle].move;

			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(-g_aTitle[nCntTitle].size.x, -g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCntTitle].size.x, -g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aTitle[nCntTitle].size.x, g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCntTitle].size.x, g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;

			// 頂点カラー
			pVtx[0].col = g_aTitle[nCntTitle].col;
			pVtx[1].col = g_aTitle[nCntTitle].col;
			pVtx[2].col = g_aTitle[nCntTitle].col;
			pVtx[3].col = g_aTitle[nCntTitle].col;
		}

		//頂点バッファをアンロック
		g_titleOther.pVtxBuff->Unlock();
	}

	//ライトの更新処理
	UpdateLight();

	//カメラの更新処理
	UpdateCamera();

	UpdateShadow();

	UpdatePlayer();

	UpdateMeshField();

	UpdateMeshBg();

	UpdateWall();

	UpdateObject();

	UpdateObjectNot();

	UpdateRain();
}

//=============================================================================
//タイトルの描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	//カメラの描画処理
	SetCamera(CAMERATYPE_TITLE);

	DrawObject();

	DrawObjectNot();

	DrawShadow();

	DrawPlayer();

	DrawMeshField();

	DrawWall();

	DrawMeshBg();

	DrawRain();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_titleOther.pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < g_titleOther.nNum; nCntTitle++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_titleOther.pTexture[g_aTitle[nCntTitle].type]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTitle, 2);
	}
}

//=============================================================================
// 爪痕処理
//=============================================================================
void TitleScars(int nIndex)
{
	if (g_aTitle[nIndex].type == TITLE_SCARS)
	{// 爪痕の時
		VERTEX_2D*pVtx;  //頂点情報へのポインタ

		if (g_titleOther.nCntAll > TITLESCARS_START)
		{// 指定の時間になったら
			if (g_titleOther.nPatternAnim != TITLESCARS_ANIM - 1)
			{// アニメーションが最後まで到達していないとき
				if ((g_titleOther.nCntAll % TITLESCARS_SWITCH) == 0)
				{// 指定の間隔になったら
					// パターン番号を更新
					g_titleOther.nPatternAnim = (g_titleOther.nPatternAnim + 1) % TITLESCARS_ANIM;
				}
			}
		}

		g_aTitle[nIndex].col.a = 1.0f;	// 不透明に

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_titleOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(g_titleOther.fAnimWidth * g_titleOther.nPatternAnim, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_titleOther.fAnimWidth * g_titleOther.nPatternAnim + g_titleOther.fAnimWidth, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_titleOther.fAnimWidth * g_titleOther.nPatternAnim, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_titleOther.fAnimWidth * g_titleOther.nPatternAnim + g_titleOther.fAnimWidth, 1.0f);

		//頂点バッファをアンロック
		g_titleOther.pVtxBuff->Unlock();
	}
}

//=============================================================================
// タイトルアイコンの処理
//=============================================================================
void TitleIcon(int nIndex)
{
	if (g_aTitle[nIndex].type == TITLE_ICON)
	{// タイトル文字の時
		if (g_titleOther.nCntAll > TITLEICON_START)
		{// 指定の時間になったら
			g_aTitle[nIndex].col.a += 0.01f;		// 不透明度の加算
		}

		if (!g_titleOther.bEvent)
		{// イベントスキップ時
			g_aTitle[nIndex].col.a = 1.0f;				// 不透明に
		}
	}
}

//=============================================================================
// タイトル文字の処理
//=============================================================================
void TitleChar(int nIndex)
{
	if (g_aTitle[nIndex].type == TITLE_CHAR)
	{// タイトル文字の時
			if (g_titleOther.nCntAll > TITLECHAR_START)
			{// 指定の時間になったら
				g_aTitle[nIndex].col.a += 0.01f;		// 不透明度の加算

				if (g_aTitle[nIndex].pos.y < TITLECHAR_POSY)
				{// 指定位置に来たら
					g_titleOther.nCntChar++;			// 文字カウンタを加算
					g_aTitle[nIndex].move.y = 0.0f;

					if (g_titleOther.nCntChar == 40 ||
						g_titleOther.nCntChar == 45)
					{// 指定のタイミングになったら
						g_titleOther.bFlash = true;		// フラッシュさせる
					}

					if (g_titleOther.nCntChar == 53)
					{
						// キー入力促しを出現
						g_titleOther.bPress = true;
						g_titleOther.bEvent = false;
					}
				}
				else
				{
					g_aTitle[nIndex].move.y -= 0.5f;	// 指定位置まで上昇
				}

				// フラッシュ処理
				if (g_titleOther.bFlash)
				{// フラッシュ使用時
					g_titleOther.nCntFlash++;			// フラッシュカウンタを加算

					if (g_titleOther.nCntFlash > 2)
					{// 指定時間フラッシュしたら
					 // 色をもとに戻す
						g_aTitle[nIndex].col = D3DXCOLOR(0.7f, 0.5f, 0.5f, 1.0f);
						// フラッシュカウンタをリセット
						g_titleOther.nCntFlash = 0;
						// フラッシュの終了
						g_titleOther.bFlash = false;
					}
					else
					{
						// フラッシュ色にする
						g_aTitle[nIndex].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
				}
			}

			if (!g_titleOther.bEvent)
			{
				{// イベントスキップ時
					g_aTitle[nIndex].pos.y = TITLECHAR_POSY;					// 指定位置に
					g_aTitle[nIndex].move.y = 0.0f;								// 移動量を0に
					g_aTitle[nIndex].col = D3DXCOLOR(0.7f, 0.5f, 0.5f, 1.0f);	// 色を設定
				}
			}
	}
}

//=============================================================================
// キー入力促しの処理
//=============================================================================
void TitlePress(int nIndex)
{
	static float fColAChange = 0.0f;				// 透明度の変更
	static bool bFrash = true;						// 点滅の有無

	if (g_aTitle[nIndex].type == TITLE_PRESS)
	{// PRESSANYKEYの処理
		// 点滅処理
		if (g_titleOther.bPress)
		{// 使用時
			if (g_aTitle[nIndex].col.a >= 1.0f)
			{// 透明にしていく
				fColAChange = -0.008f;
			}
			else if (g_aTitle[nIndex].col.a <= 0.2f)
			{// 不透明にしていく
				fColAChange = 0.01f;
			}
		}
		else
		{// 点滅off時
			fColAChange = -TITLE_COLASPEED;		// 透明に
		}

		g_aTitle[nIndex].col.a += fColAChange;	// 透明度の変化を加算
	}
}

//=============================================================================
// 演出スキップ処理
//=============================================================================
void SkipEvent(int nIndex)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetAnyTrigger(1, 0))
	{
		if (g_titleOther.bEvent)
		{
			g_titleOther.bEvent = false;					// 演出終了
			g_titleOther.bPress = true;						// キー入力促しを出現
			g_titleOther.nPatternAnim = TITLESCARS_ANIM - 1;// 爪痕アニメーションを終了
		}
	}
}

//=============================================================================
// 透明度の幅の固定
//=============================================================================
void TitleColAFixed(int nIndex)
{
	// 透明度のふり幅を固定
	if (g_aTitle[nIndex].col.a < 0.0f)
	{// 透明度が0未満になったら
	 // 0で止めておく
		g_aTitle[nIndex].col.a = 0.0f;
	}

	if (g_aTitle[nIndex].col.a > 1.0f)
	{// 透明度が1.0を超過したら
	 // 1.0で止めておく
		g_aTitle[nIndex].col.a = 1.0f;
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void TitleMakeVtx(void)
{
	VERTEX_2D*pVtx;								// 頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	if (g_titleOther.pVtxBuff == NULL)
	{
		// テクスチャの読み込み
		// 爪痕
		D3DXCreateTextureFromFile(pDevice,
			TITLESCARS_TEX,
			&g_titleOther.pTexture[TITLE_SCARS]);
		// タイトルアイコン
		D3DXCreateTextureFromFile(pDevice,
			TITLEICON_TEX,
			&g_titleOther.pTexture[TITLE_ICON]);
		// タイトル文字
		D3DXCreateTextureFromFile(pDevice,
			TITLECHAR_TEX,
			&g_titleOther.pTexture[TITLE_CHAR]);
		// キー入力促し
		D3DXCreateTextureFromFile(pDevice,
			TITLEPRESS_TEX,
			&g_titleOther.pTexture[TITLE_PRESS]);

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * g_titleOther.nNum,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_titleOther.pVtxBuff,
			NULL);

		//頂点バッファをロックして頂点データへのポインタを取得
		g_titleOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報の設定
		for (int nCntTitle = 0; nCntTitle < g_titleOther.nNum; nCntTitle++, pVtx += 4)
		{
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(-g_aTitle[nCntTitle].size.x, -g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCntTitle].size.x, -g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aTitle[nCntTitle].size.x, g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCntTitle].size.x, g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;

			// 頂点カラー
			pVtx[0].col = g_aTitle[nCntTitle].col;
			pVtx[1].col = g_aTitle[nCntTitle].col;
			pVtx[2].col = g_aTitle[nCntTitle].col;
			pVtx[3].col = g_aTitle[nCntTitle].col;

			// rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}

		//頂点バッファをアンロック
		g_titleOther.pVtxBuff->Unlock();
	}
}