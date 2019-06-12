//=============================================================================
//
// メイン処理 [main.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"
#include "xaudio2.h"				//サウンド処理で必要
#include "Xinput.h"
//*****************************************************************************
// ライブラリファイル
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "Xinput.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)	// ウインドウの幅
#define SCREEN_HEIGHT	(720)	// ウインドウの高さ

// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 3Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 頂点カラー  )
#define	FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//*****************************************************************************
#define		VERTICAL_BLOCK		(40)		//縦のブロック数
#define		CROSS_BLOCK			(40)		//横のブロック数
#define		VERTICAL_LENGTH		(80.0f)		//縦の長さ
#define		CROSS_LENGTH		(80.0f)		//横の長さ
#define		VERTEX_NUMBER		((VERTICAL_BLOCK + 1) * (CROSS_BLOCK + 1))		//頂点数
#define		INDEX_NUMBER		(VERTICAL_BLOCK * (CROSS_BLOCK + 1) * 2 + (VERTICAL_BLOCK - 1) * 2 + 1)		//頂点数

#define		GAP_CROSS			(0)
#define		GAP_CENTERX			(0)
#define		GAP_CENTERZ			(0)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

// 3Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;

typedef enum
{//アニメーションの使用状態
	ANIMATION_NORMAL = 0,
	ANIMATION_FADE,
	ANIMATION_LOOP,
	ANIMATION_UP,
	ANIMATION_MAX,

}ANIMATION;

typedef struct
{
	int nCross;
	int nVertical;

}WallInfo;

typedef enum
{//モードの状態

	MODE_TITLE = 0,		//  タイトル
	MODE_RANKING,		//  ランキング
	MODE_SELCT,			//　セレクト
	MODE_GAME,			//  ゲームメイン
	MODE_START,			//  キャラメイク
	MODE_STORY,			//　ストーリー
	MODE_CREDIT,		//　クレジット
	MODE_ENDSTORY,		//	エンドストーリー
	MODE_ENDROLL,		//　エンドロール
	MODE_GAMEOVER,		//　ゲームオーバー
	MODE_RESULT,		//  リザルト
	MODE_MAX

}MODE;

typedef enum
{//windowの表示状態
	WINDOWSTATE_NORMAL = 0,			//何も更新しない
	WINDOWSTATE_Delete0,			//すぐ消す
	WINDOWSTATE_VIBRATION,			//振動
	WINDOWSTATE_LEVELUP,			//レベルアップ
	WINDOWSTATE_STEALTH,			//だんだん不透明に->通常になる
	WINDOWSTATE_FADEIN,				//
	WINDOWSTATE_FADEOUT,			//
	WINDOWSTATE_FADEOUT_B,			//
	WINDOWSTATE_FADE_Z,				//ズームしながら消える
	WINDOWSTATE_FADE_ALL,			//出てきて勝手に消える
	WINDOWSTATE_FLASH,				//点滅
	WINDOWSTATE_FLASH2,				//超点滅
	WINDOWSTATE_SCROLL0,			//縦スクロール
	WINDOWSTATE_SCROLL1,			//斜スクロール 左下へ
	WINDOWSTATE_SCROLL2,			//左SCROLL
	WINDOWSTATE_SELECTON,			//選択されている状態
	WINDOWSTATE_SELECTOFF,			//選択されていない状態
	WINDOWSTATE_WALL,				//壁
	WINDOWSTATE_BG,					//スクロールによる座標変更
	WINDOWSTATE_FIELD,				//地面
	WINDOWSTATE_STAMINAGAGE,			//右から左へ減るゲージ
	WINDOWSTATE_MAXSTAMINAGAGE,			//右から左へ減るゲージ
	WINDOWSTATE_LIFEGAGE,			//右から左へ減るゲージ
	WINDOWSTATE_AUTOLIFEGAGE,			//右から左へ減るゲージ
	WINDOWSTATE_MAXLIFEGAGE,		//ゲージ枠
	WINDOWSTATE_MPGAGE,			//右から左へ減るゲージ
	WINDOWSTATE_AUTOMPGAGE,		//自動で増減するゲージ
	WINDOWSTATE_MAXMPGAGE,		//ゲージ枠
	WINDOWSTATE_EXPGAGE,			//GAGE
	WINDOWSTATE_MAXEXPGAGE,			//後から増減していくGAGE
	WINDOWSTATE_PLAYER,				//自機のパターンにより変更
	WINDOWSTATE_FLOW,				//流れる
	WINDOWSTATE_FLOW0,				//斜め右下から流れる
	WINDOWSTATE_FLOW1,				//斜め左下から流れる
	WINDOWSTATE_SHOT,				//10倍a0.0fから1倍a1.0fまで
	WINDOWSTATE_MAX,

}WINDOWSTATE; typedef enum
{//windowの表示状態
	WINDOWTYPE_CENTER = 0,			//中心
	WINDOWTYPE_LEFT_DOWN,			//左下
	WINDOWTYPE_MAX,

}WINDOWTYPE;
typedef enum
{//windowの表示状態
	WINDOW_UPDATE_GAME = 0,			//ゲーム内のみ更新
	WINDOW_UPDATE_SELECT,			//いつでも更新
	WINDOW_UPDATE_MAX,

}WINDOW_UPDATE;
typedef enum
{//windowの表示状態
	WINDOW_DRAW_0 = 0,		//表示順番
	WINDOW_DRAW_1,
	WINDOW_DRAW_2,
	WINDOW_DRAW_3,
	WINDOW_DRAW_PAUSE,
	WINDOW_DRAW_MAP,
	WINDOW_DRAW_MINIMAP,
	WINDOW_DRAW_MINIMAP0,
	WINDOW_DRAW_FRONT,
	WINDOW_DRAW_TIME,
	WINDOW_DRAW_BACK,
	WINDOW_DRAW_MAX,

}WINDOW_DRAW;

typedef enum
{//windowの使用状態
	WINDOWUSE_NORMAL = 0,
	WINDOWUSE_GAME,	//残機
	WINDOWUSE_SELECT,			//選択
	WINDOWUSE_GUARD,			//GUARD
	WINDOWUSE_LOAD,				//読み込み画面で
	WINDOWUSE_FONT,				//文字入力時 判定
	WINDOWUSE_RANKING,			//RANKINGの時に使用　難易度のロゴ
	WINDOWUSE_RANKNUMBER,		//RANKINGの時に使用　王冠
	WINDOWUSE_PLAYER,			//プレイヤーの場合
	WINDOWUSE_ENEMY,			//BOSSエネミーの場合
	WINDOWUSE_MAX,

}WINDOWUSE;

typedef enum
{//文字の状態
	FONTSTATE_NONE = 0,
	FONTSTATE_NORMAL,
	FONTSTATE_DELETE,
	FONTSTATE_RANKDOWN,
	FONTSTATE_RANKSET,		//ランキング時のセット
	FONTSTATE_RANKDELETE,	//ランキング時のデリート
	FONTSTATE_FLASH,		//点滅
	FONTSTATE_FLASH0,		//点滅黒
	FONTSTATE_FLOW,		//流れる

}FONTSTATE;
typedef struct
{
	FONTSTATE	state;		//文字の状態
	D3DXCOLOR	col;		//カラー
	RECT		rect;		//文字の表示範囲
	int			nType;		//フォントのタイプ
	int			nFormat;	//表示場所
	int			nCntState;	//状態管理用
	int			nMaxFont;	//表示する最大数
	int			nCntTime;	//表示のためのカウント
	char		aStr[84];	//文字の量
	bool		bDisplay;	//表示非表示
	bool		bUse;		//使用or未使用
	WINDOW_DRAW draw;
	bool		bshadow;	//影の有無

}Font;

typedef enum
{//アニメーションの使用状態
	EXPLOSIONSTATE_PHIT,		//プレイヤーにあたり判定のある爆発
	EXPLOSIONSTATE_PHITMINUS,	//プレイヤーにあたり判定のある爆発 判定の小さい
	EXPLOSIONSTATE_EHIT,		//敵にあたり判定のある爆発
	EXPLOSIONSTATE_EFFECT,		//あたり判定無し
	EXPLOSIONSTATE_DELETE,		//消えていく
	EXPLOSIONSTATE_FADE,
	EXPLOSIONSTATE_GAGE,		//EXPゲージ
	EXPLOSIONSTATE_LOAD,		//LOAD専用
	EXPLOSIONSTATE_DFADE,		//出てきて消える

}EXPLOSIONSTATE;

typedef struct
{//リザルトで使う
	int nCntScore;		//forで回さないためローカルだと初期化に困る
	int nCntResult;		//時間カウント
	int nRankin;		//ハイスコア時の順位保存 名前入力時使用
	int nWideFont;		//50音表の横選択部分
	int nHightFont;		//50音表の縦選択部分
	char aStr[25];		//名前入力のため

}Result;

//*********************************************************************************************************//
// 構造体定義
//*********************************************************************************************************//
typedef enum
{//スコアの状態
	DIGITALSTATE_NONE = 0,
	DIGITALSTATE_WAVE,
	DIGITALSTATE_DAMAGE,		//斜め右下にスライド
	DIGITALSTATE_HEEL,			//斜め左上にスライド
	DIGITALSTATE_WAVE0,

}DIGITALSTATE;
typedef struct
{//スコアの構造体
	D3DXVECTOR3 pos;			//場所
	D3DXVECTOR3 move;			//加速値
	D3DXCOLOR	col;			//カラー
	int			nDigital;		// 数字
	int			nLengthX;
	int			nLengthY;
	float		fHeight;
	DIGITALSTATE	state;		//状態
	int		nCntstate;			//状態管理用
	bool		bUse;			//使ってるか使ってないか

}DIGITAL;

typedef struct
{
	int			nResults;		//ゲーム終了状態
	int			nCntPoint;
	bool		bUse;			//使用してるかどうか

}GAME_PLAY;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode, int nFade);
int *GetTime(void);
MODE *GetMode(void);
HWND *GethWnd(void);
GAME_PLAY *GetGame_Play(void);
int *GetStoryTXT(void);
int *GetENDStoryTXT(void);

//*********************************************************************************************************//
// プロトタイプ宣言
//*********************************************************************************************************//
void InitFont(void);
void UninitFont(void);
void UpdateFont(void);
void DrawFont(WINDOW_DRAW draw);
void SetFont(RECT rect, D3DXCOLOR col, char *aStr, int nNumber, int nType, int nFormat, int nMaxFont, bool bShadow, FONTSTATE state, WINDOW_DRAW draw);
void ChangeFont(bool bUse, char *aStr, int nNumber, int nStrType, int nMaxFont, int nCntTime, FONTSTATE state, D3DCOLOR col);
void BreakFont(void);
int *GetSetFont(void);
void SetFontDamage(RECT rect, D3DCOLOR col, char *aStr, int nWide, int nHeight, FONTSTATE state);
void FontScroll(D3DXVECTOR3 move);

//*****************************************************************************
// プロトタイプ宣言　爆発 てかAnimation全部
//*****************************************************************************
void InitAnimation(void);
void UninitAnimation(void);
void UpdateAnimation(void);
void DrawAnimation(void);
void SetAnimation(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nPtnWide, int nPtnHeight, int nPtnSpeed, int nType, int nView, ANIMATION anim);
void BreakAnimation(void);

//*****************************************************************************
// プロトタイプ宣言　ビルボード
//*****************************************************************************
void InitZWindow(void);
void UninitZWindow(void);
void UpdateZWindow(void);
void DrawZWindow(void);
int SetZWindow(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float nLengthX, float nLengthY, int nType, int nView, WINDOWSTATE state, float fCntState);
void BreakZWindow(void);
void ChangeZWindow(int nCount, float fMax, float fMin, D3DXVECTOR3 pos, bool bUse);

//*********************************************************************************************************//
// プロトタイプ宣言　背景
//*********************************************************************************************************//
void InitWindow(void);
void UninitWindow(void);
void UpdateWindow(WINDOW_UPDATE update);
void DrawWindow(WINDOW_DRAW draw);
int SetWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, int nPtnUV, int nPtnWide, int nPtnHeight
	, int nCntUse, WINDOW_UPDATE nUpdateType, WINDOWSTATE state, WINDOWUSE use, WINDOWTYPE type, WINDOW_DRAW draw);
void BreakWindow(void);
void ChangeWindow(int nCntWindow, D3DXCOLOR col, WINDOWSTATE state);
void CheckGage(float fGage, float fMaxGage, int nCntWindow);
void SelectWindow(int nNumber);
void BackWindow(int nNumber);
void FontWindowMove(int nWide, int nHeight);
void ColPlayerWindow(float fCola);

//*********************************************************************************************************//
// プロトタイプ宣言　フェードのポリゴン表示処理
//*********************************************************************************************************//
void InitFadeWindow(void);
void UninitFadeWindow(void);
void UpdateFadeWindow(void);
void DrawFadeWindow(void);
void SetFadeWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, int nCntUse, WINDOWSTATE state, WINDOWUSE use);
void LoadBreakWindow(void);
void BreakFadeWindow(void);

//*********************************************************************************************************//
// プロトタイプ宣言　背景
//*********************************************************************************************************//
void InitBGWindow(void);
void UninitBGWindow(void);
void UpdateBGWindow(void);
void DrawBGWindow(void);
void SetBGWindow(D3DXVECTOR3 pos, D3DXCOLOR col, float fSpeed, float fPos, int nLengthX, int nLengthY, int nType, WINDOWSTATE state);
void BreakBGWindow(void);
void BGScroll(D3DXVECTOR3 move);

//*********************************************************************************************************//
// プロトタイプ宣言 リザルト
//*********************************************************************************************************//
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
int nCntPoint(int nCntFont, int nShadowFont, int *nPoint, int nMaxPoint, bool bCnt);

//*********************************************************************************************************//
// プロトタイプ宣言 ランキング
//*********************************************************************************************************//
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

//*********************************************************************************************************//
// プロトタイプ宣言 キャラメイク
//*********************************************************************************************************//
void InitStart(void);
void UninitStart(void);
void UpdateStart(void);
void DrawStart(void);
Result *GetResult(void);
void InputName(int nMode);
void InputName1(void);
void InputName2(void);
void InputName3(void);

//*****************************************************************************
// プロトタイプ宣言　Animation全部
//*****************************************************************************
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
int SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 Interval, D3DXCOLOR col, int nLengthX, int nLengthY, int nPtnWide, int nPtnHeight, int nPtnSpeed, int nType, ANIMATION anim, EXPLOSIONSTATE state, float steep, int nView);
void ExplosionScroll(D3DXVECTOR3 move);
void BreakExplosion(void);
void CheckEXP(float fEXP, float fMaxEXP, int nCntExplosion);
void ChangeExplosion(int nCntExplosion, EXPLOSIONSTATE state);

//*****************************************************************************
// プロトタイプ宣言  digital
//*****************************************************************************
DIGITAL *GetDigital(void);
void BreakDigital(void);
void DigitalScroll(D3DXVECTOR3 move);
void SetDigital(D3DXVECTOR3 pos, D3DXCOLOR col, DIGITALSTATE state, int nLengthX, int nLengthY, int nDigital);
void DrawDigital(void);
void UpdateDigital(void);
void UninitDigital(void);
void InitDigital(void);
#endif