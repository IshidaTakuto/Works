//=============================================================================
//
// 雪処理 [snow.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _SNOW_H_
#define _SNOW_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	SNOWTYPE_TITLE = 0,
	SNOWTYPE_GAME,
	SNOWTYPE_MAX
}SNOWTYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXCOLOR col;
	float fRadius;
	bool bUse;
	SNOWTYPE type;
	D3DXMATRIX mtxWorld;
}SNOW;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSnow(void);
void UninitSnow(void);
void UpdateSnow(void);
void DrawSnow(void);

#endif