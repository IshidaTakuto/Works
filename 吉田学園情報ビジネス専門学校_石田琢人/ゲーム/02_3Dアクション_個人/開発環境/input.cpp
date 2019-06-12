//=============================================================================
//
// 入力処理 [input.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// キーの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8 g_pInput = NULL;				//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//入力デバイス(キーボード)へのポインタ

BYTE g_aKeyState[NUM_KEY_MAX];				//キーボードの入力情報(プレス情報)
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];		//キーボードの入力情報(トリガー情報)
BYTE g_aKeyStateRelease[NUM_KEY_MAX];		//キーボードの入力情報(リリース情報)
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];		//キーボードの入力情報(リリース情報)

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	// 入力デバイス(キーボード)の開放
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();	//キーボードのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの開放
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//キーボードの入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{//キーボードの入力情報保存
			//トリガー(一回だけ)
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			//リリース(キーを離した時)
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];

			//リピート(キーを押し続けた時)
			g_aKeyStateRepeat[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			//プレス(押しっぱなし)
			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	//キーボードへのアクセス権を獲得
	}
}

//=============================================================================
// キーボードの入力情報(プレス情報)を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return(g_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードの入力情報(トリガー情報)を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードの入力情報(リピート情報)を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return(g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードの入力情報(リリース情報)を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return(g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}
