//=============================================================================
//
// モデル処理 [object2.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _OBJECT2_H_
#define _OBJECT2_H_

#include "main.h"

//*****************************************************************************
// モデルの構造体
//*****************************************************************************
typedef struct
{
	LPD3DXMESH		pMeshObject = NULL;			//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatObject = NULL;		//マテリアルの情報へのポインタ
	DWORD			nNumMatObject = 0;			//マテリアルの情報数
	D3DXMATRIX		mtxWorldObject;				//ワールドマトリックス
	D3DXVECTOR3		pos;						//位置
	D3DXVECTOR3		rot;						//向き
	bool			bUse;						//種類
	D3DXVECTOR3		VtxMinObject, VtxMaxObject;	//モデルの最小値・最大値
	LPDIRECT3DTEXTURE9* pMeshTextures = NULL;
}Object2;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitObject2(void);
void UninitObject2(void);
void UpdateObject2(void);
void DrawObject2(void);
Object2 *GetObject2(void);
void DeleteObject2(void);

bool CollisionObject2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
void SetObject2(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif
