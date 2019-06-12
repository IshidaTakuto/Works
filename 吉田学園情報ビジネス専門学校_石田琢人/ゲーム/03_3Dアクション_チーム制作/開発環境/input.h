//=============================================================================
//
// 入力処理 [input.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardQuick(int nKey, bool bUse);
bool GetKeyboardRun(int nKey, bool bRun);
int *GetKeyTrigger(void);
bool GetKeyboardTPress(int nKey);
void GetKeyboardCount(void);
#endif