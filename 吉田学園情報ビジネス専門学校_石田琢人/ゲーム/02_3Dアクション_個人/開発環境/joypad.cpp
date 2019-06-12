//=============================================================================
//
// ジョイスティック処理 [joypad.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "joypad.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CONTROLLERS	(1)
#define MAX_KEY	(32)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8 g_pDInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDIDevice = NULL;
CONTROLLER g_Controller[MAX_CONTROLLERS];
int g_aKey[MAX_KEY];
XINPUT_STATE g_state;

//*****************************************************************************
// ジョイスティックの接続判定
//*****************************************************************************
void ConnectJoystick(void)
{
	//Player *pPlayer;
	//pPlayer = GetPlayer();

	for (int nCntControllers = 0; nCntControllers < MAX_CONTROLLERS; nCntControllers++)
	{
		ZeroMemory(&g_state, sizeof(XINPUT_STATE));

		// Simply get the g_state of the controller from XInput.
		g_Controller[nCntControllers].dwResult = XInputGetState(nCntControllers, &g_state);

		if (g_Controller[nCntControllers].dwResult == ERROR_SUCCESS)
		{// 接続成功
		}
		else if(g_Controller[nCntControllers].dwResult == ERROR_DEVICE_NOT_CONNECTED)
		{// 接続失敗
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

int GetPadTrigger(int nKey)
{
	int nTrigger;

	nTrigger = g_aKey[nKey];
	//if (g_aKey[nKey] < 1) { bTrigger = true; };
	g_aKey[nKey] = 2;

	return nTrigger;
}

void Trigger(void)
{
	for (int nCntPad = 0; nCntPad < MAX_KEY; nCntPad++)
	{
		g_aKey[nCntPad]--;
	}
}

XINPUT_STATE GetJoyPad(void)
{
	return g_state;
}