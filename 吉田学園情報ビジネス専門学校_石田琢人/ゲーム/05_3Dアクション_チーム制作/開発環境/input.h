//=============================================================================
//
// 入力処理 [input.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define   MAX_JOYPAD  (3)       // 接続できるパッドの数

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
typedef enum
{
	DIJS_BUTTON_0 = 0,  // Xボタン
	DIJS_BUTTON_1,      // Aボタン
	DIJS_BUTTON_2,      // Bボタン
	DIJS_BUTTON_3,      // Yボタン
	DIJS_BUTTON_4,      // LBボタン
	DIJS_BUTTON_5,      // RBボタン
	DIJS_BUTTON_6,      // LTボタン
	DIJS_BUTTON_7,      // RTボタン
	DIJS_BUTTON_8,      // BACKボタン
	DIJS_BUTTON_9,      // STARTボタン
	DIJS_BUTTON_10,     // 左アナログスティック上方向入力
	DIJS_BUTTON_11,     // 左アナログスティック下方向入力
	DIJS_BUTTON_12,     // 左アナログスティック右方向入力
	DIJS_BUTTON_13,     // 左アナログスティック左方向入力
	DIJS_BUTTON_14,     // 右アナログスティック上方向入力
	DIJS_BUTTON_15,     // 右アナログスティック下方向入力
	DIJS_BUTTON_16,     // 右アナログスティック右方向入力
	DIJS_BUTTON_17,     // 右アナログスティック左方向入力
	DIJS_BUTTON_18,     // 十字キー上方向入力
	DIJS_BUTTON_19,     // 十字キー下方向入力
	DIJS_BUTTON_20,     // 十字キー右方向入力
	DIJS_BUTTON_21,     // 十字キー左方向入力
						// 以下割り当てなし
						DIJS_BUTTON_22,
						DIJS_BUTTON_23,
						DIJS_BUTTON_24,
						DIJS_BUTTON_25,
						DIJS_BUTTON_26,
						DIJS_BUTTON_27,
						DIJS_BUTTON_28,
						DIJS_BUTTON_29,
						DIJS_BUTTON_30,
						DIJS_BUTTON_31,
						DIJS_BUTTON_MAX
}PADBUTTON;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetJoyPadPress(int nButton, int nIdxPad);
bool GetJoyPadTrigger(int nButton, int nIdxPad);
bool GetJoyPadRelease(int nButton, int nIdxPad);
bool GetJoyPadRepeat(int nButton, int nIdxPad);
bool GetAnyTrigger(int nNumber, int nIdxPad);
BOOL PASCAL EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

float GetJotPadRadian(int nIdxPad);
float GetJotPadLeftStick(int nIdxPad);
float GetJotPadRightStick(int nIdxPad);

#endif