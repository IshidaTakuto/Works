//=============================================================================
//
// 雨処理 [rain.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _RAIN_H_
#define _RAIN_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	RAINTYPE_TITLE = 0,
	RAINTYPE_GAME,
	RAINTYPE_MAX
}RAINTYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	bool bUse;
	RAINTYPE type;
	D3DXMATRIX mtxWorld;
}RAIN;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRain(void);
void UninitRain(void);
void UpdateRain(void);
void DrawRain(void);

#endif