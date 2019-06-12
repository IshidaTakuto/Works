//=============================================================================
//
// クリアメッセージ処理 [clear.h]
// Author : shotaro kanamaru
//
//=============================================================================
#ifndef _CLEAR_H_
#define _CLEAR_H_

#include "main.h"

//*****************************************************************************
// 列挙体の定義
//*****************************************************************************
typedef enum
{// メッセージタイプ
	TYPE_NORMAL = 0,	// ノーマル
	TYPE_COMPLETE,		// コンプリート
	TYPE_EMPTY,			// その他
	TYPE_MAX,
}CLEARTYPE;

typedef enum
{// 状態
	STATE_NONE = 0,		// 待機
	STATE_RESET,		// リセット
	STATE_START,		// 起動
	STATE_MAX,
}CLEARSTATE;

typedef enum
{//	メッセージの部位
	MESSAGE_BAND = 0,	// 帯
	MESSAGE_UP,			// 上部
	MESSAGE_DOWN,		// 下部
	MESSAGE_ONE,		// 1文字目
	MESSAGE_TWO,		// 2文字目
	MESSAGE_THREE,		// 3文字目
	MESSAGE_FOUR,		// 4文字目
	MESSAGE_MAX,		// 最大数
}CLEARMESSAGE;

//*****************************************************************************
//構造体の定義
//*****************************************************************************
typedef struct
{// メッセージ情報
	D3DXVECTOR3 pos;		// 現在位置
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 size;		// 大きさ
	D3DXCOLOR	col;		// 色
	CLEARTYPE	type;		// メッセージタイプ
	bool		bUse;		// 使用
}Clear;

typedef struct
{// その他設定情報
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;			// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		pTexture[MESSAGE_MAX] = {};	// テクスチャへのポインタ
	CLEARTYPE				type;				// メッセージタイプ
	CLEARSTATE				state;				// 状態
	int						nCntAll;			// 共通カウンタ
	int						nCntDraw;			// 表示カウンタ
	bool					bSwitch;			// 移動の切替
}ClearOther;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitClear(void);
void UninitClear(void);
void UpdateClear(void);
void DrawClear(void);
void SetClearMessage(const CLEARTYPE type);

#endif
