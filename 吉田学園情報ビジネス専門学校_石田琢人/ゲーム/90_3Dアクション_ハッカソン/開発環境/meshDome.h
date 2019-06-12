//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// メッシュドーム処理 [meshDome.h]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef _MESHDOME_H_
#define _MESHDOME_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MESHDOME	(2)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// メッシュドームの種類
typedef enum
{
	MESHDOMETYPE_000 = 0,
	MESHDOMETYPE_001,
	MESHDOMETYPE_MAX,
}MESHDOMETYPE;

// テクスチャの情報
typedef struct
{
	char *pFilename;	// ファイル名
} meshDomeTexture;

// 描画向き
typedef enum
{
	DRAWSIDE_BOTH = 0,	// 両側
	DRAWSIDE_IN,		// 内側
	DRAWSIDE_OUT		// 外側
}DRAWSIDE;

// メッシュドームの情報データ
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	float			fRadius;		// 半径
	int				nNumSplitV;		// 縦方向のブロック数
	int				nNumSplitH;		// 横方向のブロック数
	MESHDOMETYPE	TexType;		// テクスチャの種類
	D3DXVECTOR2		TexMoveSpeed;	// テクスチャ移動速度
	DRAWSIDE		DrawSide;		// 描画する面
}meshDomeInfo;

// メッシュドームの情報
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	pIdxBuff;		// インデックスバッファのポインタ
	D3DXVECTOR3				pos;			// 位置
	D3DXVECTOR3				rot;			// 向き
	D3DXMATRIX				mtxWorld;		// ワールドマトリックス
	float					fRadius;		// 半径
	int						nNumSplitV;		// 縦方向のブロック数
	int						nNumSplitH;		// 横方向のブロック数
	int						nNumVertex;		// 頂点数
	int						nNumIndex;		// インデックス数
	int						nNumPolygon;	// ポリゴン数
	MESHDOMETYPE			TexType;		// テクスチャの種類
	D3DXVECTOR2				TexMoveSpeed;	// テクスチャ移動速度
	DRAWSIDE				DrawSide;		// 描画する面
}meshDome;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitmeshDome(void);
void UninitmeshDome(void);
void UpdatemeshDome(void);
void DrawmeshDome(void);

#endif
