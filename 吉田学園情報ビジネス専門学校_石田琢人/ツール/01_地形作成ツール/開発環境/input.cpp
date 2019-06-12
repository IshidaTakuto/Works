//=============================================================================
//
// 入力処理 [input.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DI_JOY_I_INPUT	(1000)  // スティックの方向入力受付を行う範囲
#define REPEAT_TIME		(30)	// リピート起動までのフレーム
#define REPEAT_FRAME     (20)   // リピート情報を起動し始めるフレーム数の合計
#define REPEAT_TRIGGER   (6)    // リピート情報を起動するフレームの間隔

//=========================================
// 静的メンバ変数
//=========================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
int CInputJoypad::m_ConnectPad = 0;
LPDIRECTINPUTDEVICE8 CInputJoypad::m_pDevice[MAX_JOYPAD] = {};

//=========================================
// コンストラクタ
//=========================================
CInput::CInput()
{
}

//=========================================
// デストラクタ
//=========================================
CInput::~CInput()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInput::Uninit(void)
{
	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	キーボード
//
//=========================================
// コンストラクタ
//=========================================
CInputKeyboard::CInputKeyboard()
{
	m_pDevice = NULL;

	for (int nCount = 0; nCount < MAX_KEY; nCount++)
	{
		m_aState[nCount] = 0;
		m_aStateTrigger[nCount] = 0;
		m_aStateRelease[nCount] = 0;
		m_aStateRepeat[nCount] = 0;
		m_nCntKey[nCount] = 0;
	}
}

//=========================================
// デストラクタ
//=========================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// 入力デバイス(キーボード)の開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	//キーボードのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	CInput::Uninit();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[MAX_KEY];	//キーボードの入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{//キーボードの入力情報保存
			//トリガー(一回だけ)
			m_aStateTrigger[nCntKey] = (m_aState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			//リリース(キーを離した時)
			m_aStateRelease[nCntKey] = (m_aState[nCntKey] ^ aKeyState[nCntKey]) & m_aState[nCntKey];

			// リピート
			m_aStateRepeat[nCntKey] = aKeyState[nCntKey];

			//プレス(押しっぱなし)		一番下にする
			m_aState[nCntKey] = aKeyState[nCntKey];

			if ((m_aStateRepeat[nCntKey] & 0x80) == false)
			{// キーを押してないときリピート用のカウンタを0にする
				m_nCntKey[nCntKey] = 0;
			}
		}
	}
	else
	{
		m_pDevice->Acquire();	//キーボードへのアクセス権を獲得
	}
}

//=============================================================================
// キーボードの入力情報(リピート情報)を取得
//=============================================================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	bool bPush = false;

	if (GetPress(nKey) == true)
	{// キーが押されている
		m_nCntKey[nKey]++;	// 押されている時間のカウント
	}

	if ((m_nCntKey[nKey] > 30 && m_nCntKey[nKey] % 3 == 0) || m_nCntKey[nKey] == 1)
	{// 30フレーム以上押している
		bPush = true;
	}
	else if (m_nCntKey[nKey] <= 30)
	{// 30フレーム以下
		bPush = false;
	}

	return bPush;
}

//=============================================================================
// キーが押されているか確認
//=============================================================================
bool CInputKeyboard::GetAnyKey(void)
{
	bool bKey = false;

	for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
	{//キーボードの入力情報保存
		//トリガー(一回だけ)
		bKey = (m_aStateTrigger[nCntKey] & 0x80) ? true : false;

		if (bKey == true)
		{// 押されているキーがあればブレイクする
			break;
		}
	}

	return bKey;
}

#if 0
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	マウス
//
//=========================================
// コンストラクタ
//=========================================
CInputMouse::CInputMouse()
{
	m_pDevice = NULL;
}

//=========================================
// デストラクタ
//=========================================
CInputMouse::~CInputMouse()
{
}

//=============================================================================
// マウスの初期化
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT ret = S_FALSE;
	if (m_pInput == NULL)
	{
		return false;
	}

	// マウス用にデバイスオブジェクトを作成
	ret = m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL);
	if (FAILED(ret)) {
		// デバイスの作成に失敗
		return false;
	}

	// データフォーマットを設定
	ret = m_pDevice->SetDataFormat(&c_dfDIMouse);	// マウス用のデータ・フォーマットを設定
	if (FAILED(ret)) {
		// データフォーマットに失敗
		return false;
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	ret = m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret)) {
		// モードの設定に失敗
		return false;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	ret = m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) {
		// デバイスの設定に失敗
		return false;
	}

	// 入力制御開始
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// マウスの終了処理
//=============================================================================
void CInputMouse::Uninit(void)
{
	// 入力デバイス(マウス)の開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	// マウスのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	CInput::Uninit();
}

//=============================================================================
// マウスの更新処理
//=============================================================================
void CInputMouse::Update(void)
{
	// マウスの状態を取得
	HRESULT	hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
	if (hr == DIERR_INPUTLOST)
	{
		m_pDevice->Acquire();
		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	ジョイパッド
//
//=========================================
// コンストラクタ
//=========================================
CInputJoypad::CInputJoypad()
{
	for (int nCntPad = 0; nCntPad < MAX_JOYPAD; nCntPad++)
	{
		for (int nCount = 0; nCount < DIJS_BUTTON_MAX; nCount++)
		{
			m_aState[nCntPad][nCount] = 0;
			m_aStateTrigger[nCntPad][nCount] = 0;
			m_aStateRelease[nCntPad][nCount] = 0;
			m_aStateRepeat[nCntPad][nCount] = 0;
			m_aCounter[nCntPad][nCount] = 0;
		}
	}
}

//=========================================
// デストラクタ
//=========================================
CInputJoypad::~CInputJoypad()
{
}

//=============================================================================
// ジョイパッドの初期化
//=============================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyDeviceProc, NULL, DIEDFL_ATTACHEDONLY)))
	{// 複数のデバイスを列挙する
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// ジョイパッドの終了処理
//=============================================================================
void CInputJoypad::Uninit(void)
{
	// 入力デバイス(ジョイパッド)の開放
	for (int nCntPad = 0; nCntPad < MAX_JOYPAD; nCntPad++)
	{
		if (m_pDevice[nCntPad] != NULL)
		{
			m_pDevice[nCntPad]->Unacquire();   // ジョイパッドへのアクセス権を放棄
			m_pDevice[nCntPad]->Release();
			m_pDevice[nCntPad] = NULL;
		}
	}

	CInput::Uninit();
}

//=============================================================================
// ジョイパッドの更新処理
//=============================================================================
void CInputJoypad::Update(void)
{
	for (int nCntPad = 0; nCntPad < m_ConnectPad; nCntPad++)
	{// パッドの数だけ繰り返し
		DIJOYSTATE dijs[MAX_JOYPAD];    // ジョイパッドの入力情報を格納する
		if (m_pDevice != NULL)
		{// 生成できた
		 // 入力デバイス(ジョイパッド)からデータを取得
			m_pDevice[nCntPad]->Poll();
			if (SUCCEEDED(m_pDevice[nCntPad]->GetDeviceState(sizeof(DIJOYSTATE), &dijs[nCntPad])))
			{
				m_YAxisL[nCntPad] = 0.0f;
				m_XAxisL[nCntPad] = 0.0f;

				m_XAxisL[nCntPad] = (float)dijs[nCntPad].lX;
				m_YAxisL[nCntPad] = (float)dijs[nCntPad].lY;

				m_YAxisR[nCntPad] = 0.0f;
				m_XAxisR[nCntPad] = 0.0f;

				m_XAxisR[nCntPad] = (float)dijs[nCntPad].lRx;
				m_YAxisR[nCntPad] = (float)dijs[nCntPad].lRy;

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
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 45)
				{// 十字キーの右上が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_18] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_20] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 90)
				{// 十字キーの下が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_20] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 135)
				{// 十字キーの右下が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_19] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_20] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 180)
				{// 十字キーの右が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_19] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 225)
				{// 十字キーの左下が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_19] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_21] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 270)
				{// 十字キーの左が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_21] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 315)
				{// 十字キーの左上が押された
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_18] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_21] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}


				for (int nCntJoyButton = 0; nCntJoyButton < DIJS_BUTTON_MAX; nCntJoyButton++)
				{// ジョイパッドのボタンの数だけ繰り返し
					m_aStateTrigger[nCntPad][nCntJoyButton] = (m_aState[nCntPad][nCntJoyButton] ^ dijs[nCntPad].rgbButtons[nCntJoyButton]) & dijs[nCntPad].rgbButtons[nCntJoyButton];  // ジョイパッドの入力情報(トリガー情報)保存
					m_aStateRelease[nCntPad][nCntJoyButton] = (m_aState[nCntPad][nCntJoyButton] ^ dijs[nCntPad].rgbButtons[nCntJoyButton]) & m_aState[nCntPad][nCntJoyButton];      // ジョイパッドの入力情報(リリース情報)保存
					if (dijs[nCntPad].rgbButtons[nCntJoyButton] == 0x80)
					{// ジョイパッドが入力されている
						m_aCounter[nCntPad][nCntJoyButton]++;    // カウンターを進める
						if (m_aCounter[nCntPad][nCntJoyButton] == 1)
						{// 最初の1フレームだけ起動
							m_aStateRepeat[nCntPad][nCntJoyButton] = 0x80;
						}
						else if (m_aCounter[nCntPad][nCntJoyButton] == 2)
						{
							m_aStateRepeat[nCntPad][nCntJoyButton] = 0;      // ジョイパッドの入力情報(リピート情報)を停止
						}
						else if (m_aCounter[nCntPad][nCntJoyButton] >= REPEAT_FRAME)
						{// 20フレーム分キーが押されている
							if (m_aCounter[nCntPad][nCntJoyButton] % REPEAT_TRIGGER == 0)
							{// 3フレームごと
								m_aStateRepeat[nCntPad][nCntJoyButton] = 0x80;   // ジョイパッドの入力情報(リピート情報)を起動
							}
							else
							{// それ以外のフレーム
								m_aStateRepeat[nCntPad][nCntJoyButton] = 0;     // ジョイパッドの入力情報(リピート情報)を一時停止
							}
						}
					}
					else
					{// ジョイパッドが入力されていない
						m_aCounter[nCntPad][nCntJoyButton] = 0;          // カウンターを戻す
						m_aStateRepeat[nCntPad][nCntJoyButton] = 0;      // ジョイパッドの入力情報(リピート情報)を停止
					}
					m_aState[nCntPad][nCntJoyButton] = dijs[nCntPad].rgbButtons[nCntJoyButton];   // ジョイパッドの入力情報(プレス情報)保存
				}
			}
			else
			{
				m_pDevice[nCntPad]->Acquire(); // ジョイパッドへのアクセス権を獲得
			}
		}
	}
}

//=============================================================================
// ジョイパッドの入力判定を取得
//=============================================================================
bool CInputJoypad::GetAnyTrigger(int nIdxPad)
{
	for (int nCount = 0; nCount < DIJS_BUTTON_MAX; nCount++)
	{// 全部のボタンを調べる
		if (m_aStateTrigger[nIdxPad][nCount] == 0x80)
		{// 入力された
			return true;
		}
	}

	// 入力されたボタンがなかった
	return false;
}

//=============================================================================
// ジョイパッドの十字キーのラジアンを取得
//=============================================================================
float CInputJoypad::GetRadian(int nIdxPad)
{
	return m_Radian[nIdxPad] / 100;
}

//=============================================================================
// ジョイパッドの左アナログスティックの軸を取得
//=============================================================================
float CInputJoypad::GetLeftStick(int nIdxPad)
{
	// 軸の角度を求める
	float Ziku = atan2f(m_XAxisL[nIdxPad], m_YAxisL[nIdxPad]);

	return Ziku;
}

//=============================================================================
// ジョイパッドの右アナログスティックの軸を取得
//=============================================================================
float CInputJoypad::GetRightStick(int nIdxPad)
{
	// 軸の角度を求める
	float Ziku = atan2f(m_XAxisR[nIdxPad], m_YAxisR[nIdxPad]);

	return Ziku;
}

//=============================================================================
// 列挙されたジョイパッドの分デバイスを生成する
//=============================================================================
BOOL PASCAL CInputJoypad::EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	// 入力デバイス(ジョイパッド)の生成
	if (FAILED(m_pInput->CreateDevice(lpddi->guidInstance, &m_pDevice[m_ConnectPad], NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice[m_ConnectPad]->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	//if (FAILED(m_pDevice[m_ConnectPad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	//{
	//	return E_FAIL;
	//}

	if (m_pDevice[m_ConnectPad] != NULL)
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
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Z;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 右アナログスティック
		diprg.diph.dwObj = DIJOFS_RX;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RY;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 十字キー(上を0度とし時計回りに角度 * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ジョイパッドへのアクセス権を獲得(入力制御開始)
		m_pDevice[m_ConnectPad]->Acquire();

		m_ConnectPad++;    // 接続完了
	}

	// 複数列挙する
	return DIENUM_CONTINUE;
}
#endif