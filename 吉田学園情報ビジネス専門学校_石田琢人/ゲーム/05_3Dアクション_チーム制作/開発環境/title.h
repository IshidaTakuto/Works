//=============================================================================
//
// タイトル処理 [title.h]
// Author : shotaro kanamaru
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// 列挙体の定義
//*****************************************************************************
typedef enum
{// タイトルの種類
	TITLE_SCARS = 0,		// 爪痕
	TITLE_ICON,				// タイトルアイコン
	TITLE_CHAR,				// タイトル文字
	TITLE_PRESS,			// キー入力の促し
	TITLE_MAX,				// その他
}TITLETYPE;

//*****************************************************************************
//構造体の定義
//***************************************************************************** 
typedef struct
{// タイトル情報
	D3DXVECTOR3 pos,posInit;// 現在位置、初期位置
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 size;		// 大きさ
	D3DXCOLOR	col;		// 色
	TITLETYPE	type;		// 種類
}Title;

typedef struct
{// 配置情報
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 size;		// 大きさ
	TITLETYPE	type;		// 種類
}TitleInfo;

typedef struct
{// その他設定情報
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;			// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		pTexture[TITLE_MAX] = {};	// テクスチャへのポインタ
	int						nNum;						// 総数
	int						nCntAll;					// 共通カウンタ
	int						nPatternAnim;				// アニメーションパターン
	float					fAnimWidth;					// アニメーションの幅
	int						nCntChar;					// タイトルロゴのカウンタ
	int						nCntFlash;					// フラッシュのカウンタ
	bool					bFlash;						// フラッシュの有無
	bool					bPress;						// キー入力促しの使用
	bool					bEvent;						// 演出の有無
}TitleOther;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif
