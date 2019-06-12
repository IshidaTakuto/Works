//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// ステージの配置物の処理 [stage_object.h]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef _STAGE_OBJ_H_
#define _STAGE_OBJ_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STAGE_OBJINFO	(16)		// ファイルから読み込むモデルの最大数
#define MAX_STAGE_OBJ		(128)		// 表示する最大数

//*****************************************************************************
// 構造体
//*****************************************************************************
// ファイル読み込み情報 [最大数,種類]
typedef struct
{
	int nTypeModel;						// 読み込むモデルの最大数
	int nMaxModel;						// 表示するモデルの最大数
}Stage_objInfo_Number;

// ファイル読み込み情報 [共通部分]
typedef struct
{
	char pXFileName[MAX_CHAR];			// 読み込むファイル名
	LPD3DXMESH pMesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;				// マテリアル情報へのポインタ
	DWORD NumMat;						// マテリアルの数
	LPDIRECT3DTEXTURE9 *pTexture;		// テクスチャへのポインタ
	D3DXVECTOR3 vtxMin;					// 頂点座標の最小値
	D3DXVECTOR3 vtxMax;					// 頂点座標の最大値
}Stage_objInfo;

// オブジェクトの情報
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	float fHeight;						// 高さ
	float fRadius;						// 半径
	int nType;							// 種類
#if _DEBUG
	int IdxLine;						// 線の使用番号 [インデックス]
#endif
}Stage_obj;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitStage_obj(void);
void UninitStage_obj(void);
void UpdateStage_obj(void);
void DrawStage_obj(void);

bool CollisionStage_obj(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius, float fHeight);
/* ファイル読み込み */
void LoadStage_obj(void);
void UnloadStage_obj(void);
Stage_obj *GetStage_obj(void);
Stage_objInfo *GetStage_objInfo(void);
int GetNumStage_obj(void);
#endif
