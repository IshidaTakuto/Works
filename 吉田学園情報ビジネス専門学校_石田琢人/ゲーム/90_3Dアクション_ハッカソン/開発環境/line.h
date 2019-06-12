//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// ライン処理 [line.h]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef _LINE_H_
#define _LINE_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
// ラインの情報
typedef struct
{
	int nIdxLine;				// 線の使用番号
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posStart;		// 開始位置
	D3DXVECTOR3 posEnd;			// 終了位置
	D3DXCOLOR col;				// カラー
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	bool bUse;					// 使用しているかどうか
}Line;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitLine(void);
void UninitLine(void);
void UpdateLine(void);
void DrawLine(void);

void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd, D3DXCOLOR col,int nIdxLine);
int SetColisionLine(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
int SetColisionLine2(D3DXVECTOR3 pos, float fRadius, float fHeight);
void SetPositionLine(int nIdxLine,D3DXVECTOR3 pos);
void DeleteLine(int nIdxLine);
#endif
