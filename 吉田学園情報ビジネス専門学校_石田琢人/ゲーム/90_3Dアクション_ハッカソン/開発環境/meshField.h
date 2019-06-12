//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// メッシュフィールド(インデックスバッファ使用)の処理 [meshField.h]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MESHFIELD		(16)			// メッシュフィールドの最大数
#define MESH_WIDTH			(50.0f)			// 1分割のメッシュの幅
#define MESH_DEPTH			(50.0f)			// 1分割のメッシュの奥行

//*****************************************************************************
// メッシュフィールドの構造体
//*****************************************************************************
// メッシュフィールドの種類
typedef enum
{
	 MESHFIELDTYPE_000 = 0,
	MESHFIELDTYPE_MAX
}MESHFIELDTYPE;

// テクスチャの情報
typedef struct
{
	char *pFilename;		// テクスチャ名
}meshFieldTexture;

// メッシュフィールドの情報データ
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	int nNumSplitX;			// X方向の分割数
	int nNumSplitZ;			// Z方向の分割数
	float fWidth;			// 幅
	float fDepth;			// 奥行
}meshFieldInfo;

// メッシュフィールドの情報
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	int nNumSplitX;						// X方向の分割数
	int nNumSplitZ;						// Z方向の分割数
	float fWidth;						// 幅
	float fDepth;						// 奥行
	int nNumVertex;						// 頂点数
	int nNumIndex;						// インデックス数
	int nNumPolygon;					// ポリゴン数
}meshField;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitmeshField(void);
void UninitmeshField(void);
void UpdatemeshField(void);
void DrawmeshField(void);
#endif
