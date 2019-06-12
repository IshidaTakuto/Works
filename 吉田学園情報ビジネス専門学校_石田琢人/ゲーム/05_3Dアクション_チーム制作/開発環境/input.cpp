//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 有馬　武志
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX	     (256)	 // キーの最大数
#define DI_JOY_I_INPUT   (1000)  // スティックの方向入力受付を行う範囲
#define REPEAT_FRAME     (20)    // リピート情報を起動し始めるフレーム数の合計
#define REPEAT_TRIGGER   (6)     // リピート情報を起動するフレームの間隔

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8 g_pInput = NULL;                       // DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;           // 入力デバイス(キーボード)へのポインタ
LPDIRECTINPUTDEVICE8 g_pDevJoypad[MAX_JOYPAD] = {};   // 入力デバイス(ジョイパッド)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];                        // キーボードの入力情報(プレス情報)
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];                 // キーボードの入力情報(トリガー情報)
BYTE g_aKeyStateRelease[NUM_KEY_MAX];                 // キーボードの入力情報(リリース情報)
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];                  // キーボードの入力情報(リピート情報)
int g_nCntKey[NUM_KEY_MAX];							  // リピート用のカウンタ

BYTE g_aJoyState[MAX_JOYPAD][DIJS_BUTTON_MAX];        // ジョイパッドの入力情報(プレス情報)
BYTE g_aJoyStateTrigger[MAX_JOYPAD][DIJS_BUTTON_MAX]; // ジョイパッドの入力情報(トリガー情報)
BYTE g_aJoyStateRelease[MAX_JOYPAD][DIJS_BUTTON_MAX]; // ジョイパッドの入力情報(リリース情報)
BYTE g_aJoyStateRepeat[MAX_JOYPAD][DIJS_BUTTON_MAX];  // ジョイパッドの入力情報(リピート情報)

int  g_aKeyboardCounter[NUM_KEY_MAX];                 // キーボードが入力されてる間を数えるカウンター
int  g_aJoyCounter[MAX_JOYPAD][DIJS_BUTTON_MAX];      // ジョイパッドが入力されてる間を数えるカウンター

int   g_ConnectPad;                          // 接続されたパッドの数
float g_Radian[MAX_JOYPAD];                  // 十字キーのラジアンを格納する
float g_Xziku[MAX_JOYPAD];                   // 左アナログスティックのX軸を格納する
float g_Yziku[MAX_JOYPAD];                   // 左アナログスティックのY軸を格納する
float g_XzikuR[MAX_JOYPAD];                  // 右アナログスティックのX軸を格納する
float g_YzikuR[MAX_JOYPAD];                  // 右アナログスティックのY軸を格納する

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//======================================
	// キーボード初期化処理
	//======================================
	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	//======================================
	// ジョイパッド初期化処理
	//======================================
	if (FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyDeviceProc, NULL, DIEDFL_ATTACHEDONLY)))
	{// 複数のデバイスを列挙する
		return E_FAIL;
	}

	for (int nCntInput = 0; nCntInput < NUM_KEY_MAX; nCntInput++)
	{// キーボードのボタンの数だけ繰り返し
		g_aKeyboardCounter[nCntInput] = 0;   // カウンターを初期化

		g_nCntKey[nCntInput] = 0;				// リピート用のカウンタを初期化


	}
	for (int nCntPad = 0; nCntPad < g_ConnectPad; nCntPad++)
	{// 接続されたパッドの数だけ繰り返し
		for (int nCntInput = 0; nCntInput < DIJS_BUTTON_MAX; nCntInput++)
		{// ジョイパッドのボタンの数だけ繰り返し
			g_aJoyCounter[nCntPad][nCntInput] = 0;        // カウンターを初期化
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitInput(void)
{
	// 入力デバイス(キーボード)の開放
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire(); // キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力デバイス(ジョイパッド)の開放
	for (int nCntPad = 0; nCntPad < 2; nCntPad++)
	{
		if (g_pDevJoypad[nCntPad] != NULL)
		{
			g_pDevJoypad[nCntPad]->Unacquire();   // ジョイパッドへのアクセス権を放棄
			g_pDevJoypad[nCntPad]->Release();
			g_pDevJoypad[nCntPad] = NULL;
		}
	}
	// DirectInputオブジェクトの開放
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateInput(void)
{
	//======================================
	// キーボード更新処理
	//======================================
	BYTE aKeyState[NUM_KEY_MAX]; // キーボードの入力情報

								 // 入力デバイス(キーボード)からデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];   // キーボードの入力情報(トリガー情報)保存
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey]; // キーボードの入力情報(リリース情報)保存
			//if (aKeyState[nCntKey] == 0x80)
			//{// キーボードが入力されている
			//	g_aKeyboardCounter[nCntKey]++;    // カウンターを進める
			//	if (g_aKeyboardCounter[nCntKey] >= REPEAT_FRAME)
			//	{// 20フレーム分キーが押されている
			//		if (g_aKeyboardCounter[nCntKey] % REPEAT_TRIGGER == 0)
			//		{// 3フレームごと
			//			g_aKeyStateRepeat[nCntKey] = 0x80;   // キーボードの入力情報(リピート情報)を起動
			//		}
			//		else
			//		{// それ以外のフレーム
			//			g_aKeyStateRepeat[nCntKey] = 0;      // キーボードの入力情報(リピート情報)を一時停止
			//		}
			//	}
			//}
			//else
			//{// キーボードが入力されていない
			//	g_aKeyboardCounter[nCntKey] = 0;        // カウンターを戻す
			//	g_aKeyStateRepeat[nCntKey] = 0;         // キーボードの入力情報(リピート情報)を停止
			//}

			// リピート
			g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];

			if ((g_aKeyStateRepeat[nCntKey] & 0x80) == false)
			{// キーを押してないときリピート用のカウンタを0にする
				g_nCntKey[nCntKey] = 0;
			}

			g_aKeyState[nCntKey] = aKeyState[nCntKey];  // キーボードの入力情報(プレス情報)保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire(); // キーボードへのアクセス権を獲得
	}

	//======================================
	// ジョイパッド更新処理
	//======================================
	for (int nCntPad = 0; nCntPad < g_ConnectPad; nCntPad++)
	{// パッドの数だけ繰り返し
		DIJOYSTATE dijs[MAX_JOYPAD];    // ジョイパッドの入力情報を格納する
		if (g_pDevJoypad != NULL)
		{// 生成できた
		 // 入力デバイス(ジョイパッド)からデータを取得
			g_pDevJoypad[nCntPad]->Poll();
			if (SUCCEEDED(g_pDevJoypad[nCntPad]->GetDeviceState(sizeof(DIJOYSTATE), &dijs[nCntPad])))
			{
				g_Yziku[nCntPad] = 0.0f;
				g_Xziku[nCntPad] = 0.0f;

				g_Xziku[nCntPad] = (float)dijs[nCntPad].lX;
				g_Yziku[nCntPad] = (float)dijs[nCntPad].lY;

				g_YzikuR[nCntPad] = 0.0f;
				g_XzikuR[nCntPad] = 0.0f;

				g_XzikuR[nCntPad] = (float)dijs[nCntPad].lRx;
				g_YzikuR[nCntPad] = (float)dijs[nCntPad].lRy;

				//-----------------------------
				// 左アナログスティック
				//-----------------------------
				if (dijs[nCntPad].lY <= -100)
				{// 左アナログスティックが上に倒された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_10] = 0x80;
				}
				if (dijs[nCntPad].lY >= 100)
				{// 左アナログスティックが下に倒された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_11] = 0x80;
				}
				if (dijs[nCntPad].lX <= -100)
				{// 左アナログスティックが左に倒された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_13] = 0x80;
				}
				if (dijs[nCntPad].lX >= 100)
				{// 左アナログスティックが右に倒された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_12] = 0x80;
				}

				//-----------------------------
				// 右アナログスティック
				//-----------------------------
				if (dijs[nCntPad].lRy <= -100)
				{// 右アナログスティックが上に倒された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_14] = 0x80;
				}
				if (dijs[nCntPad].lRy >= 100)
				{// 右アナログスティックが下に倒された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_15] = 0x80;
				}
				if (dijs[nCntPad].lRx <= -100)
				{// 右アナログスティックが左に倒された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_17] = 0x80;
				}
				if (dijs[nCntPad].lRx >= 100)
				{// 右アナログスティックが右に倒された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_16] = 0x80;
				}

				//-----------------------------
				// 十字キー
				//-----------------------------
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 0)
				{// 十字キーの上が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_18] = 0x80;
					g_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 45)
				{// 十字キーの右上が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_18] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_20] = 0x80;
					g_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 90)
				{// 十字キーの下が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_20] = 0x80;
					g_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 135)
				{// 十字キーの右下が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_19] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_20] = 0x80;
					g_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 180)
				{// 十字キーの右が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_19] = 0x80;
					g_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 225)
				{// 十字キーの左下が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_19] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_21] = 0x80;
					g_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 270)
				{// 十字キーの左が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_21] = 0x80;
					g_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 315)
				{// 十字キーの左上が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_18] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_21] = 0x80;
					g_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}


				for (int nCntJoyButton = 0; nCntJoyButton < DIJS_BUTTON_MAX; nCntJoyButton++)
				{// ジョイパッドのボタンの数だけ繰り返し
					g_aJoyStateTrigger[nCntPad][nCntJoyButton] = (g_aJoyState[nCntPad][nCntJoyButton] ^ dijs[nCntPad].rgbButtons[nCntJoyButton]) & dijs[nCntPad].rgbButtons[nCntJoyButton];  // ジョイパッドの入力情報(トリガー情報)保存
					g_aJoyStateRelease[nCntPad][nCntJoyButton] = (g_aJoyState[nCntPad][nCntJoyButton] ^ dijs[nCntPad].rgbButtons[nCntJoyButton]) & g_aJoyState[nCntPad][nCntJoyButton];      // ジョイパッドの入力情報(リリース情報)保存
					if (dijs[nCntPad].rgbButtons[nCntJoyButton] == 0x80)
					{// ジョイパッドが入力されている
						g_aJoyCounter[nCntPad][nCntJoyButton]++;    // カウンターを進める
						if (g_aJoyCounter[nCntPad][nCntJoyButton] == 1)
						{// 最初の1フレームだけ起動
							g_aJoyStateRepeat[nCntPad][nCntJoyButton] = 0x80;
						}
						else if (g_aJoyCounter[nCntPad][nCntJoyButton] == 2)
						{
							g_aJoyStateRepeat[nCntPad][nCntJoyButton] = 0;      // ジョイパッドの入力情報(リピート情報)を停止
						}
						else if (g_aJoyCounter[nCntPad][nCntJoyButton] >= REPEAT_FRAME)
						{// 20フレーム分キーが押されている
							if (g_aJoyCounter[nCntPad][nCntJoyButton] % REPEAT_TRIGGER == 0)
							{// 3フレームごと
								g_aJoyStateRepeat[nCntPad][nCntJoyButton] = 0x80;   // ジョイパッドの入力情報(リピート情報)を起動
							}
							else
							{// それ以外のフレーム
								g_aJoyStateRepeat[nCntPad][nCntJoyButton] = 0;     // ジョイパッドの入力情報(リピート情報)を一時停止
							}
						}
					}
					else
					{// ジョイパッドが入力されていない
						g_aJoyCounter[nCntPad][nCntJoyButton] = 0;          // カウンターを戻す
						g_aJoyStateRepeat[nCntPad][nCntJoyButton] = 0;      // ジョイパッドの入力情報(リピート情報)を停止
					}
					g_aJoyState[nCntPad][nCntJoyButton] = dijs[nCntPad].rgbButtons[nCntJoyButton];   // ジョイパッドの入力情報(プレス情報)保存
				}
			}
			else
			{
				g_pDevJoypad[nCntPad]->Acquire(); // ジョイパッドへのアクセス権を獲得
			}
		}
	}
}

//=============================================================================
// キーボードの入力情報(プレス情報)を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}
//=============================================================================
// キーボードの入力情報(トリガー情報)を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
//=============================================================================
// キーボードの入力情報(リリース情報)を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}
//=============================================================================
// キーボードの入力情報(リピート情報)を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	bool bPush = false;

	if (GetKeyboardPress(nKey) == true)
	{
		g_nCntKey[nKey]++;
	}

	if ((g_nCntKey[nKey] > 30 && g_nCntKey[nKey] % 3 == 0) || g_nCntKey[nKey] == 1)
	{//30F以上押している
		bPush = true;
	}
	else if (g_nCntKey[nKey] <= 30)
	{
		bPush = false;
	}

	return bPush;
}
//
//
//
bool GetAnyTrigger(int nNumber,int nIdxPad)
{
	if (nNumber == 1)
	{
		for (int nCount = 0; nCount < DIJS_BUTTON_MAX; nCount++)
		{
			if (g_aJoyStateTrigger[nIdxPad][nCount] == 0x80)
			{
				return true;
			}
		}
	}
	return false;
}
//=============================================================================
// ジョイパッドの入力情報(プレス情報)を取得
//=============================================================================
bool GetJoyPadPress(int nButton, int nIdxPad)
{
	return (g_aJoyState[nIdxPad][nButton] & 0x80) ? true : false;
}
//=============================================================================
// ジョイパッドの入力情報(トリガー情報)を取得
//=============================================================================
bool GetJoyPadTrigger(int nButton, int nIdxPad)
{
	return (g_aJoyStateTrigger[nIdxPad][nButton] & 0x80) ? true : false;
}
//=============================================================================
// ジョイパッドの入力情報(リリース情報)を取得
//=============================================================================
bool GetJoyPadRelease(int nButton, int nIdxPad)
{
	return (g_aJoyStateRelease[nIdxPad][nButton] & 0x80) ? true : false;
}
//=============================================================================
// ジョイパッドの入力情報(リピート情報)を取得
//=============================================================================
bool GetJoyPadRepeat(int nButton, int nIdxPad)
{
	return (g_aJoyStateRepeat[nIdxPad][nButton] & 0x80) ? true : false;

	//bool bPush = false;

	//if (GetJoyPadPress(nButton, nIdxPad) == true)
	//{
	//	g_nCntKey[nButton]++;
	//}

	//if ((g_nCntKey[nButton] > 30 && g_nCntKey[nButton] % 3 == 0) || g_nCntKey[nButton] == 1)
	//{//30F以上押している
	//	bPush = true;
	//}
	//else if (g_nCntKey[nButton] <= 30)
	//{
	//	bPush = false;
	//}

	//return bPush;
}
//=============================================================================
// ジョイパッドの十字キーのラジアンを取得
//=============================================================================
float GetJotPadRadian(int nIdxPad)
{
	return g_Radian[nIdxPad] / 100;
}
//=============================================================================
// ジョイパッドの左アナログスティックの軸を取得
//=============================================================================
float GetJotPadLeftStick(int nIdxPad)
{
	// 軸の角度を求める
	float Ziku = atan2f(g_Xziku[nIdxPad], g_Yziku[nIdxPad]);

	return Ziku;
}
//=============================================================================
// ジョイパッドの右アナログスティックの軸を取得
//=============================================================================
float GetJotPadRightStick(int nIdxPad)
{
	// 軸の角度を求める
	float Ziku = atan2f(g_XzikuR[nIdxPad], g_YzikuR[nIdxPad]);

	return Ziku;
}

//=============================================================================
// 列挙されたジョイパッドの分デバイスを生成する
//=============================================================================
BOOL PASCAL EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	// 入力デバイス(ジョイパッド)の生成
	if (FAILED(g_pInput->CreateDevice(lpddi->guidInstance, &g_pDevJoypad[g_ConnectPad], NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevJoypad[g_ConnectPad]->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	//// 協調モードを設定
	//if (FAILED(g_pDevJoypad[g_ConnectPad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	//{
	//	return E_FAIL;
	//}

	if (g_pDevJoypad[g_ConnectPad] != NULL)
	{// 生成できた
		DIPROPRANGE diprg;

		// 軸の値の範囲を設定
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -DI_JOY_I_INPUT;
		diprg.lMax = DI_JOY_I_INPUT;

		// 軸の設定
		// 左アナログスティック
		diprg.diph.dwObj = DIJOFS_X;
		g_pDevJoypad[g_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		g_pDevJoypad[g_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Z;
		g_pDevJoypad[g_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 右アナログスティック
		diprg.diph.dwObj = DIJOFS_RX;
		g_pDevJoypad[g_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RY;
		g_pDevJoypad[g_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RZ;
		g_pDevJoypad[g_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 十字キー(上を0度とし時計回りに角度 * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		g_pDevJoypad[g_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ジョイパッドへのアクセス権を獲得(入力制御開始)
		g_pDevJoypad[g_ConnectPad]->Acquire();

		g_ConnectPad++;    // 接続完了
	}

	// 最初の1つのみで終わる
	//return DIENUM_STOP;

	// 複数列挙する
	return DIENUM_CONTINUE;
}