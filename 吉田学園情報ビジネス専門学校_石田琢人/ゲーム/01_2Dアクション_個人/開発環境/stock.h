//=============================================================================
//
// 残機処理 [stock.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _STOCK_H_
#define _STOCK_H_

#include "main.h"

#define NUM_STOCK	(3)		// 残機の最大桁数

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	int nCntStock;		// 体力
	int nType;			// 種類
	bool bUse;			// 使用しているかどうか
}STOCK;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitStock(void);
void UninitStock(void);
void UpdateStock(void);
void DrawStock(void);

void SubtractionStock(int nValue);
STOCK *GetStock(void);

#endif
