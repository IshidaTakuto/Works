//=============================================================================
//
// タイマー処理 [timer.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

void AddScore(int nValue);
int *GetTimer(void);

#endif
