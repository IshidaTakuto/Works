//=============================================================================
//
// モデル処理 [object.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _OBJECTNOT_H_
#define _OBJECTNOT_H_

#include "main.h"

//*****************************************************************************
// モデルの構造体
//*****************************************************************************
typedef struct
{
	LPD3DXMESH		pMeshObjectNot = NULL;			//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatObjectNot = NULL;		//マテリアルの情報へのポインタ
	DWORD			nNumMatObjectNot = 0;			//マテリアルの情報数
	D3DXMATRIX		mtxWorldObjectNot;				//ワールドマトリックス
	D3DXVECTOR3		pos;						//位置
	D3DXVECTOR3		rot;						//向き
	bool			bUse;						//種類
	D3DXVECTOR3		VtxMinObjectNot, VtxMaxObjectNot;	//モデルの最小値・最大値
	LPDIRECT3DTEXTURE9* pMeshTextures = NULL;
}ObjectNot;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitObjectNot(void);
void UninitObjectNot(void);
void UpdateObjectNot(void);
void DrawObjectNot(void);
void DeleteNotObject(void);
ObjectNot *GetObjectNot(void);
void SetObjectNot(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif
#pragma once
