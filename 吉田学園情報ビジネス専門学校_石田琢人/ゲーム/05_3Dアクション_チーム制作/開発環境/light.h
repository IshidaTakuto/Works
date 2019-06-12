//=============================================================================
//
// ライト処理 [light.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	LIGHTTYPE_NORMAL = 0,
	LIGHTTYPE_TITLE,
	LIGHTTYPE_RESULT,
	LIGHTTYPE_MAX
}LIGHTTYPE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);

bool GetLightRain(void);
LIGHTTYPE *GetLightType(void);
#endif
