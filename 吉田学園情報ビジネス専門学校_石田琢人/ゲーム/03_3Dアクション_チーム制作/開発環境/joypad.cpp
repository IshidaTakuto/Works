//=============================================================================
//
// ジョイスティック処理 [joypad.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#include "joypad.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CONTROLLERS	(1)
#define JOY_MAX_KEY	(BUTTON_MAX)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8 g_pDInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDIDevice = NULL;
CONTROLLER g_Controller[MAX_CONTROLLERS];
int g_aKey[JOY_MAX_KEY];
int g_aPressKey[JOY_MAX_KEY];
XINPUT_STATE g_state;

//*****************************************************************************
// ジョイスティックの接続判定
//*****************************************************************************
void InitJoystick(void)
{
	PLAYER *pPlayer = GetPlayer();

	for (int nCntPad = 0; nCntPad < JOY_MAX_KEY; nCntPad++)
	{
		g_aKey[nCntPad] = 0;
		g_aPressKey[nCntPad] = 0;
	}
}
//*****************************************************************************
// ジョイスティックの接続判定
//*****************************************************************************
void ConnectJoystick(void)
{
	PLAYER *pPlayer = GetPlayer();


	for (int nCntControllers = 0; nCntControllers < MAX_CONTROLLERS; nCntControllers++)
	{
		ZeroMemory(&g_state, sizeof(XINPUT_STATE));

		// Simply get the g_state of the controller from XInput.
		g_Controller[nCntControllers].dwResult = XInputGetState(nCntControllers, &g_state);

		if (g_Controller[nCntControllers].dwResult == ERROR_SUCCESS)
		{// 接続成功
			pPlayer[0].bJoyUse = true;
		}
		else if(g_Controller[nCntControllers].dwResult == ERROR_DEVICE_NOT_CONNECTED)
		{// 接続失敗
			pPlayer[0].bJoyUse = false;
		}
	}

		// デッドゾーンの設定 
		if ((g_state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			g_state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(g_state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				g_state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			g_state.Gamepad.sThumbLX = 0;
			g_state.Gamepad.sThumbLY = 0;
		}

		if ((g_state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			g_state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(g_state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				g_state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			g_state.Gamepad.sThumbRX = 0;
			g_state.Gamepad.sThumbRY = 0;
		}
}
//=============================================================================
// ジョイパッドアナログスティック等のトリガー処理
//=============================================================================
int GetPadTPress(int nKey, int nPad, XINPUT_STATE *state)
{
	int nTrigger = 100;
	if (state[0].Gamepad.wButtons & nPad)
	{
		nTrigger = g_aKey[nKey];
		g_aKey[nKey] = 2;
		g_aPressKey[nKey]++;
		if (g_aPressKey[nKey] > 30 && g_aPressKey[nKey] % 2 == 0) { nTrigger = 0; }
	}
	else { g_aPressKey[nKey] = 0; }

	return nTrigger;
}
//=============================================================================
// ジョイパッドトリガー処理
//=============================================================================
int GetPadTrigger(int nKey, int nPad, XINPUT_STATE *state)
{
	int nTrigger = 100;
	if (state[0].Gamepad.wButtons & nPad)
	{
		nTrigger = g_aKey[nKey];
		g_aKey[nKey] = 2;
	}

	return nTrigger;
}

//=============================================================================
// ジョイパッドアナログスティック等のトリガー処理
//=============================================================================
int GetAnalogTPress(int nKey, int nPress, float fTrigger, bool bWhich, XINPUT_STATE *state)
{
	int nTrigger = 100;
	if (bWhich == false) { fTrigger *= -1; }
	if (fTrigger > nPress)
	{
		nTrigger = g_aKey[nKey];
		g_aKey[nKey] = 2;
		g_aPressKey[nKey]++;
		if (g_aPressKey[nKey] > 30 && g_aPressKey[nKey] % 3 == 0){ nTrigger = 0; }
	}
	else{ g_aPressKey[nKey] = 0; }

	return nTrigger;
}

//=============================================================================
// ジョイパッドアナログスティック等のトリガー処理
//=============================================================================
int GetAnalogTrigger(int nKey, int nPress, float fTrigger, XINPUT_STATE *state)
{
	int nTrigger = 100;
	if (fTrigger > nPress)
	{
		nTrigger = g_aKey[nKey];
		g_aKey[nKey] = 2;
	}

	return nTrigger;
}

//=============================================================================
// トリガーのための更新処理
//=============================================================================
void Trigger(void)
{
	for (int nCntPad = 0; nCntPad < JOY_MAX_KEY; nCntPad++)
	{
		g_aKey[nCntPad]--;
	}
}
//=============================================================================
// ジョイパッドステート排出処理
//=============================================================================
XINPUT_STATE GetJoyPad(void)
{
	return g_state;
}