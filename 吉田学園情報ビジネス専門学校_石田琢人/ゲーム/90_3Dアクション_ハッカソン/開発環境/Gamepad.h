//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// コントローラー処理 [Gamepad.h]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	GAMEPAD_NUM_KEY_MAX			(128)		// キーの最大数(128固定)
#define GAMEPAD_STICK_SENSITIVITY	(1000)		// スティックの感度領域
#define DEADRATE					(0.2f)		// 無効感度率
#define GAMEPAD_DEADZONE			(GAMEPAD_STICK_SENSITIVITY * DEADRATE)			// 無効感度領域

//*****************************************************************************
// クラス定義
//*****************************************************************************
	//--- ELECOM [JC-U3613] ---//
typedef enum
{
	ELECOM_X = 0,
	ELECOM_Y,
	ELECOM_A,
	ELECOM_B,
	ELECOM_LB,
	ELECOM_RB,
	ELECOM_LT,
	ELECOM_RT,
	ELECOM_LS,
	ELECOM_RS,
	ELECOM_BACK,
	ELECOM_START,
	ELECOM_MAX
}ELECOM_CTRL;

//--- 方向 ---//
typedef enum
{
	DIRECTION_UP = 0,	// 上
	DIRECTION_DOWN,		// 下
	DIRECTION_RIGHT,	// 右
	DIRECTION_LEFT,		// 左
	DIRECTION_MAX
}DIRECTION;

typedef struct
{// ゲームパッドの情報
	DIJOYSTATE2 GamepadState;							// コントローラー情報 [全ての入力情報]
	BYTE aKeyState[GAMEPAD_NUM_KEY_MAX];				// ボタン入力情報(プレス情報)
	BYTE aKeyStateTrigger[GAMEPAD_NUM_KEY_MAX];		// ボタン入力情報(トリガー情報)
	BYTE aKeyStateRelease[GAMEPAD_NUM_KEY_MAX];		// ボタン入力情報(リリース情報)
	BYTE aCrossKeyState[DIRECTION_MAX];				// 十字キー入力情報(プレス情報)
	BYTE aCrossKeyStateTrigger[DIRECTION_MAX];		// 十字キー入力情報(トリガー情報)
	BYTE aCrossKeyStateRelease[DIRECTION_MAX];		// 十字キー入力情報(リリース)
	BYTE aStickLeftState[DIRECTION_MAX];				// 左スティック入力情報(プレス情報)
	BYTE aStickLeftStateTrigger[DIRECTION_MAX];		// 左スティック入力情報(トリガー情報)
	BYTE aStickLeftStateRelease[DIRECTION_MAX];		// 左スティック入力情報(リリース)
	BYTE aStickRightState[DIRECTION_MAX];				// 右スティック入力情報(プレス情報)
	BYTE aStickRightStateTrigger[DIRECTION_MAX];		// 右スティック入力情報(トリガー情報)
	BYTE aStickRightStateRelease[DIRECTION_MAX];		// 右スティック入力情報(リリース)

}Gamepad;

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
HRESULT InitGamapad(HINSTANCE hInstance, HWND hWnd);
void UninitGamapad(void);
void UpdateGamapad(void);
/* 取得関数 */
DIJOYSTATE2 *GetGamepadState(void);		// コントローラー情報を取得
										/* 入力関数 */
bool GetGamapadKeyPress(int nKey);							// ボタン [プレス]
bool GetGamapadKeyTrigger(int nKey);						// ボタン [トリガー]
bool GetGamapadKeyRelease(int nKey);						// ボタン [リリース]
bool GetCrossKeyPress(DIRECTION direction);			// 十字キー [プレス]
bool GetCrossKeyTrigger(DIRECTION direction);		// 十字キー [トリガー]
bool GetCrossKeyRelease(DIRECTION direction);		// 十字キー [リリース]
bool GetStickLeftPress(DIRECTION direction);		// 左スティック [プレス]
bool GetStickLeftTrigger(DIRECTION direction);		// 左スティック [トリガー]
bool GetStickLeftRelease(DIRECTION direction);		// 左スティック [リリース]
bool GetStickRightPress(DIRECTION direction);		// 右スティック [プレス]
bool GetStickRightTrigger(DIRECTION direction);		// 右スティック [トリガー]
bool GetStickRightRelease(DIRECTION direction);		// 右スティック [リリース]

#endif