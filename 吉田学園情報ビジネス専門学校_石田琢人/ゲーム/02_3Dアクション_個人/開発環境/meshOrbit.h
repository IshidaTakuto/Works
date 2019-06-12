//=============================================================================
//
// メッシュオービット処理 [meshOrbit.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	int nType;				// テクスチャ
	D3DXCOLOR col;			// カラー
	D3DXVECTOR3 start;		// 開始座標
	D3DXVECTOR3 end;		// 終わり座標
	int nTime;				// 残る時間
	bool bUse;
}MeshOrbit;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshOrbit(void);
void UninitMeshOrbit(void);
void UpdateMeshOrbit(void);
void DrawMeshOrbit(void);

void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col, int nTime, int nType);

#endif
