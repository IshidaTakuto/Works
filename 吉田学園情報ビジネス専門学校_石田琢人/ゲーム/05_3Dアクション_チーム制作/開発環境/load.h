//=============================================================================
//
// エリア移動処理 [load.h]
// Author : shotaro kanamaru
//
//=============================================================================
#ifndef _LOAD_H_
#define _LOAD_H_

#include "main.h"

//*****************************************************************************
// 列挙体の定義
//*****************************************************************************
typedef enum
{// 種類
	LOADTYPE_BACK = 0,		// 塗りつぶし
	LOADTYPE_BG,			// 背景
	LOADTYPE_LINE,			// ステージの線
	LOADTYPE_PLAYER,		// プレイヤーアイコン
	LOADTYPE_FADE,			// フェード
	LOADTYPE_MAX,			// 最大数
}LOADTYPE;

typedef enum
{// 状態
	LOADSTATE_NONE = 0,		// 待機
	LOADSTATE_START,		// 起動
	LOADSTATE_OUT,			// 終了
	LOADSTATE_MAX,
}LOADSTATE;

typedef enum
{// ロード用フェード
	LOADFADE_NONE = 0,
	LOADFADE_IN,		//フェードイン
	LOADFADE_OUT,		//フェードアウト
	LOADFADE_MAX		// 最大数
}LOADFADE;

//*****************************************************************************
//構造体の定義
//*****************************************************************************
typedef struct
{// エリア移動情報
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 size;			// 大きさ
	D3DXCOLOR	col;			// 色
	LOADTYPE	type;			// 種類
	bool		bUse;			// 使用
	int			nNumPattern;	// 現在のアニメーション番号
}Load;

typedef struct
{// 設定情報
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 size;			// 大きさ
	int			nAnimPattern;	// アニメーション数
}LoadInfo;

typedef struct
{// その他設定情報
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;				// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		pTexture[LOADTYPE_MAX] = {};	// テクスチャへのポインタ
	LOADSTATE				state;							// 状態
	int						nNextStage;						// 次のステージ
	int						nCntAll;						// 共通カウンタ
	bool					bUse;							// 全体の描画
}LoadOther;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitLoad(void);
void UninitLoad(void);
void UpdateLoad(void);
void DrawLoad(void);
void SetLoad(void);

LOADFADE GetLoadFade(void);

#endif
