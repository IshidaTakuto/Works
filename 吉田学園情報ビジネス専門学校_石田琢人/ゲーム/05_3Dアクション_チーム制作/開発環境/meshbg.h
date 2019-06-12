//=============================================================================
//
// メッシュフィールド処理 [meshbg.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _MESHBG_H_
#define _MESHBG_H_

#include "main.h"

//*****************************************************************************
// 壁の構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posMeshBg;							//位置
	D3DXVECTOR3 rotMeshBg;							//向き
	D3DXMATRIX mtxWorldMeshBg;						//ワールドマトリックス
	int nNumVertexMeshBg;							//頂点数
	int nNumIdxMeshBg;								//インデックス数
	int nNumPolygonMeshBg;							//ポリゴン数
	bool bUse;										//使用してるかどうか
}MeshBg;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshBg(void);
void UninitMeshBg(void);
void UpdateMeshBg(void);
void DrawMeshBg(void);

#endif
