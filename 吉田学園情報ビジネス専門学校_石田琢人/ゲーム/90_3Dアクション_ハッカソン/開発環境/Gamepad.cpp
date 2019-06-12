//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// コントローラー処理 [Gamepad.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "Gamepad.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECTINPUT8		 g_pGamepadInput = NULL;		// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDeviceGamepad = NULL;		// デバイスのポインタ (コールバック関数の関係で [static] 化)
DIDEVCAPS			 g_GamepadCaps;					// ゲームパッドの機能情報
Gamepad				 g_GamePad;						// ゲームパッドの情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGamapad(HINSTANCE hInstance, HWND hWnd)
{
	// ローカル変数
	HRESULT hr;		// 実行結果確認用

	// 値をクリア
	for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
	{
		g_GamePad.aKeyState[nCntKey] = 0;				// ボタン入力情報(プレス情報)
		g_GamePad.aKeyStateTrigger[nCntKey] = 0;		// ボタン入力情報(トリガー情報)
		g_GamePad.aKeyStateRelease[nCntKey] = 0;		// ボタン入力情報(リリース情報)
	}
	for (int nCntDirection = 0; nCntDirection < DIRECTION_MAX; nCntDirection++)
	{
		g_GamePad.aCrossKeyState[nCntDirection] = 0;			// 十字キー入力情報(プレス情報)
		g_GamePad.aCrossKeyStateTrigger[nCntDirection] = 0;		// 十字キー入力情報(トリガー情報)
		g_GamePad.aCrossKeyStateRelease[nCntDirection] = 0;		// 十字キー入植情報(リリース情報)
		g_GamePad.aStickLeftState[nCntDirection] = 0;			// 左スティック入力情報(プレス情報)
		g_GamePad.aStickLeftStateTrigger[nCntDirection] = 0;	// 左スティック入力情報(トリガー情報)
		g_GamePad.aStickLeftStateRelease[nCntDirection] = 0;	// 左スティック入力情報(リリース)
		g_GamePad.aStickRightState[nCntDirection] = 0;			// 右スティック入力情報(プレス情報)
		g_GamePad.aStickRightStateTrigger[nCntDirection] = 0;	// 右スティック入力情報(トリガー情報)
		g_GamePad.aStickRightStateRelease[nCntDirection] = 0;	// 右スティック入力情報(リリース)
	}


	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pGamepadInput, NULL)))
	{
		MessageBox(NULL, "DirectInput8オブジェクトの作成に失敗", "Direct Input Error", MB_OK);
		return E_FAIL;
	}

	// 入力デバイス(コントローラー)の生成
	if (FAILED(hr = g_pGamepadInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))		// 第4引数 (ATTACHEDONLY = 現在接続されているジョイスティックのみ検出する)
	{
		return FALSE;
	}

	if (hr == S_OK && g_pDeviceGamepad != NULL)
	{// コントローラーが生成できた場合
		// デバイスにデータフォーマットを設定
		if (FAILED(hr = g_pDeviceGamepad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return FALSE;
		}

		// 協調モードを設定
		if (FAILED(hr = g_pDeviceGamepad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			return FALSE;
		}

		// ゲームパッドの機能の情報を収集
		g_GamepadCaps.dwSize = sizeof(DIDEVCAPS);

		if (FAILED(hr = g_pDeviceGamepad->GetCapabilities(&g_GamepadCaps)))
		{
			g_pDeviceGamepad->Release();
			return FALSE;
		}

		// 各軸のモード設定
		if (FAILED(hr = g_pDeviceGamepad->EnumObjects(EnumAxesCallback, (void*)g_pDeviceGamepad, DIDFT_AXIS)))
		{
			return FALSE;
		}

		// ゲームパッドへのアクセス権を獲得(入力制御開始)
		hr = g_pDeviceGamepad->Poll();

		if (FAILED(hr))
		{
			hr = g_pDeviceGamepad->Acquire();

			while (hr == DIERR_INPUTLOST)
			{
				hr = g_pDeviceGamepad->Acquire();
			}
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGamapad(void)
{
	// DirectInputの開放処理
	if (g_pGamepadInput != NULL)
	{
		g_pGamepadInput->Release();
		g_pGamepadInput = NULL;
	}

	// ゲームパッドのポインタを解放
	if (g_pDeviceGamepad != NULL)
	{
		g_pDeviceGamepad->Release();
		g_pDeviceGamepad = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGamapad(void)
{
	if (g_pDeviceGamepad != NULL)
	{// コントローラーが繋がっていないとき処理しない

		DIJOYSTATE2 GamepadState;							// コントローラーの情報
		BYTE aCrossKeyState[DIRECTION_MAX] = { 0,0,0,0 };	// 十字キーの情報
		BYTE aLeftStickState[DIRECTION_MAX] = { 0,0,0,0 };	// 左スティックの情報
		BYTE aRightStickState[DIRECTION_MAX] = { 0,0,0,0 };	// 右スティックの情報
		HRESULT hr;

		hr = g_pDeviceGamepad->Poll();

		// 入力デバイスからデータを取得
		if (SUCCEEDED(hr = g_pDeviceGamepad->GetDeviceState(sizeof(DIJOYSTATE2), &GamepadState)))
		{
			//--- 十字キー ---//
			if (GamepadState.rgdwPOV[0] == 0)
			{// 上
				aCrossKeyState[DIRECTION_UP] = 128;
			}
			if (GamepadState.rgdwPOV[0] == 9000)
			{// 下
				aCrossKeyState[DIRECTION_DOWN] = 128;
			}
			if (GamepadState.rgdwPOV[0] == 18000)
			{// 右
				aCrossKeyState[DIRECTION_RIGHT] = 128;
			}
			if (GamepadState.rgdwPOV[0] == 27000)
			{// 左
				aCrossKeyState[DIRECTION_LEFT] = 128;
			}

			//--- 左スティック ---//
			if (GamepadState.lY < -GAMEPAD_DEADZONE)
			{// 上
				aLeftStickState[DIRECTION_UP] = 128;
			}
			if (GamepadState.lY > GAMEPAD_DEADZONE)
			{// 下
				aLeftStickState[DIRECTION_DOWN] = 128;
			}
			if (GamepadState.lX > GAMEPAD_DEADZONE)
			{// 右
				aLeftStickState[DIRECTION_RIGHT] = 128;
			}
			if (GamepadState.lX < -GAMEPAD_DEADZONE)
			{// 左
				aLeftStickState[DIRECTION_LEFT] = 128;
			}

			//--- 右スティック ---//
			if (GamepadState.lRz < -GAMEPAD_DEADZONE)
			{// 上
				aRightStickState[DIRECTION_UP] = 128;
			}
			if (GamepadState.lRz > GAMEPAD_DEADZONE)
			{// 下
				aRightStickState[DIRECTION_DOWN] = 128;
			}
			if (GamepadState.lZ > GAMEPAD_DEADZONE)
			{// 右
				aRightStickState[DIRECTION_RIGHT] = 128;
			}
			if (GamepadState.lZ < -GAMEPAD_DEADZONE)
			{// 左
				aRightStickState[DIRECTION_LEFT] = 128;
			}

			// スティック & 十字キー
			for (int nCntDirection = 0; nCntDirection < DIRECTION_MAX; nCntDirection++)
			{
				// 十字キー入力情報 [トリガー] = (現在 ^ 過去) & 現在
				g_GamePad.aCrossKeyStateTrigger[nCntDirection] = (aCrossKeyState[nCntDirection] ^ g_GamePad.aCrossKeyState[nCntDirection]) & aCrossKeyState[nCntDirection];
				// 十字キー入力情報 [リリース = (現在 ^ 過去) & 過去
				g_GamePad.aCrossKeyStateRelease[nCntDirection] = (aCrossKeyState[nCntDirection] ^ g_GamePad.aCrossKeyState[nCntDirection]) & g_GamePad.aCrossKeyState[nCntDirection];
				// 十字キー入力情報 [プレス] ※ 前回の入力情報を基に判断するため、最後に更新すること！
				g_GamePad.aCrossKeyState[nCntDirection] = aCrossKeyState[nCntDirection];
				// 左スティック入力情報 [トリガー] = (現在 ^ 過去) & 現在
				g_GamePad.aStickLeftStateTrigger[nCntDirection] = (aLeftStickState[nCntDirection] ^ g_GamePad.aStickLeftState[nCntDirection]) & aLeftStickState[nCntDirection];
				// 左スティック入力情報 [リリース = (現在 ^ 過去) & 過去
				g_GamePad.aStickLeftStateRelease[nCntDirection] = (aLeftStickState[nCntDirection] ^ g_GamePad.aStickLeftState[nCntDirection]) & g_GamePad.aStickLeftState[nCntDirection];
				// 左スティック入力情報 [プレス] ※ 前回の入力情報を基に判断するため、最後に更新すること！
				g_GamePad.aStickLeftState[nCntDirection] = aLeftStickState[nCntDirection];
				// 右スティック入力情報 [トリガー] = (現在 ^ 過去) & 現在
				g_GamePad.aStickRightStateTrigger[nCntDirection] = (aRightStickState[nCntDirection] ^ g_GamePad.aStickRightState[nCntDirection]) & aRightStickState[nCntDirection];
				// 右スティック入力情報 [リリース = (現在 ^ 過去) & 過去
				g_GamePad.aStickRightStateRelease[nCntDirection] = (aRightStickState[nCntDirection] ^ g_GamePad.aStickRightState[nCntDirection]) & g_GamePad.aStickRightState[nCntDirection];
				// 右スティック入力情報 [プレス] ※ 前回の入力情報を基に判断するため、最後に更新すること！
				g_GamePad.aStickRightState[nCntDirection] = aRightStickState[nCntDirection];
			}

			// ボタン
			for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
			{
				// ボタン入力情報(トリガー情報) = (現在 ^ 過去) & 現在
				g_GamePad.aKeyStateTrigger[nCntKey] = (GamepadState.rgbButtons[nCntKey] ^ g_GamePad.aKeyState[nCntKey]) & GamepadState.rgbButtons[nCntKey];
				// ボタン入力情報(リリース情報) = (現在 ^ 過去) & 過去
				g_GamePad.aKeyStateRelease[nCntKey] = (GamepadState.rgbButtons[nCntKey] ^ g_GamePad.aKeyState[nCntKey]) & g_GamePad.aKeyState[nCntKey];
				// ボタン入力情報(プレス情報) ※ 前回の入力情報を基に判断するため、最後に更新すること！
				g_GamePad.aKeyState[nCntKey] = GamepadState.rgbButtons[nCntKey];

			}
		}
		else
		{
			g_pDeviceGamepad->Acquire();		// ゲームパッドへのアクセス権を獲得
		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ボタン [プレス]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetGamapadKeyPress(int nKey)
{
	return(g_GamePad.aKeyState[nKey] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ボタン [トリガー]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetGamapadKeyTrigger(int nKey)
{
	return(g_GamePad.aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ボタン [リリース]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetGamapadKeyRelease(int nKey)
{
	return(g_GamePad.aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 左スティック [プレス]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetStickLeftPress(DIRECTION direction)
{
	return(g_GamePad.aStickLeftState[(int)direction] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 左スティック [トリガー]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetStickLeftTrigger(DIRECTION direction)
{
	return(g_GamePad.aStickLeftStateTrigger[(int)direction] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 左スティック [リリース]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetStickLeftRelease(DIRECTION direction)
{
	return(g_GamePad.aStickLeftStateRelease[(int)direction] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 右スティック [プレス]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetStickRightPress(DIRECTION direction)
{
	return(g_GamePad.aStickRightState[(int)direction] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 右スティック [トリガー]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetStickRightTrigger(DIRECTION direction)
{
	return(g_GamePad.aStickRightStateTrigger[(int)direction] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 右スティック [リリース]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetStickRightRelease(DIRECTION direction)
{
	return(g_GamePad.aStickRightStateRelease[(int)direction] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 十字キー [プレス]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetCrossKeyPress(DIRECTION direction)
{
	return(g_GamePad.aCrossKeyState[(int)direction] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 十字キー [トリガー]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetCrossKeyTrigger(DIRECTION direction)
{
	return(g_GamePad.aCrossKeyStateTrigger[(int)direction] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 十字キー [リリース]
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool GetCrossKeyRelease(DIRECTION direction)
{
	return(g_GamePad.aCrossKeyStateRelease[(int)direction] & 0x80) ? true : false;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// コントローラー情報を取得
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
DIJOYSTATE2 *GetGamepadState(void)
{
	return(&g_GamePad.GamepadState);
}

//*****************************************************************************
// joystick用コールバック関数
//*****************************************************************************
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext)
{
	HRESULT hr;

	// 列挙されたジョイスティックへのインターフェイスを取得
	hr = g_pGamepadInput->CreateDevice(pdidinstance->guidInstance, &g_pDeviceGamepad, NULL);

	// デバイスを検出出来なかった場合
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

//*****************************************************************************
// joystickの機能コールバック関数
//*****************************************************************************
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -(GAMEPAD_STICK_SENSITIVITY);		// 十字キーの最小値 [上] [左]	マイナス
	diprg.lMax = GAMEPAD_STICK_SENSITIVITY;			// 十字キーの最大値 [下] [右]	プラス

	// デバイスに設定
	hr = g_pDeviceGamepad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}