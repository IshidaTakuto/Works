//=============================================================================
//
// 入力処理 [input.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_KEY	(256)
#define MAX_JOYPAD	(1)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
class CRenderer;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;		// DirectInputオブジェクトへのポインタ
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// キーボード
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetAnyKey(void);

	bool GetPress(int nKey) { return(m_aState[nKey] & 0x80) ? true : false; };
	bool GetTrigger(int nKey) { return(m_aStateTrigger[nKey] & 0x80) ? true : false; };
	bool GetRelease(int nIdxPad, int nKey) { return(m_aStateRelease[nKey] & 0x80) ? true : false; };
	bool GetRepeat(int nKey);

private:
	LPDIRECTINPUTDEVICE8 m_pDevice;		// 入力デバイスへのポインタ

	BYTE m_aState[MAX_KEY];				// キーボードの入力情報(プレス情報)
	BYTE m_aStateTrigger[MAX_KEY];		// キーボードの入力情報(トリガー情報)
	BYTE m_aStateRelease[MAX_KEY];		// キーボードの入力情報(リリース情報)
	BYTE m_aStateRepeat[MAX_KEY];		// キーボードの入力情報(リピート情報)

	int m_nCntKey[MAX_KEY];				// リピート用のカウンタ
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マウス
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);


private:
	LPDIRECTINPUTDEVICE8 m_pDevice;		// 入力デバイスへのポインタ
	DIMOUSESTATE m_mouseState;			// マウス状態
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ジョイパッド
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputJoypad : public CInput
{
public:
	typedef enum
	{
		DIJS_BUTTON_0 = 0,  // Aボタン
		DIJS_BUTTON_1,      // Bボタン
		DIJS_BUTTON_2,      // Xボタン
		DIJS_BUTTON_3,      // Yボタン
		DIJS_BUTTON_4,      // LBボタン
		DIJS_BUTTON_5,      // RBボタン
		DIJS_BUTTON_6,      // BACKボタン
		DIJS_BUTTON_7,      // STARTボタン
		DIJS_BUTTON_8,      // LTボタン
		DIJS_BUTTON_9,      // RTボタン
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

	CInputJoypad();
	~CInputJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nIdxPad, int nKey) { return(m_aState[nIdxPad][nKey] & 0x80) ? true : false; };
	bool GetTrigger(int nIdxPad, int nKey) { return(m_aStateTrigger[nIdxPad][nKey] & 0x80) ? true : false; };
	bool GetRelease(int nIdxPad, int nKey) { return(m_aStateRelease[nIdxPad][nKey] & 0x80) ? true : false; };
	bool GetRepeat(int nIdxPad, int nKey) { return(m_aStateRepeat[nIdxPad][nKey] & 0x80) ? true : false; };
	bool GetAnyTrigger(int nIdxPad);

	float GetRadian(int nIdxPad);
	float GetLeftStick(int nIdxPad);
	float GetRightStick(int nIdxPad);

private:
	static BOOL PASCAL EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	static LPDIRECTINPUTDEVICE8 m_pDevice[MAX_JOYPAD];		// 入力デバイス(ジョイパッド)へのポインタ

	BYTE m_aState[MAX_JOYPAD][DIJS_BUTTON_MAX];			// ジョイパッドの入力情報(プレス情報)
	BYTE m_aStateTrigger[MAX_JOYPAD][DIJS_BUTTON_MAX];	// ジョイパッドの入力情報(トリガー情報)
	BYTE m_aStateRelease[MAX_JOYPAD][DIJS_BUTTON_MAX];	// ジョイパッドの入力情報(リリース情報)
	BYTE m_aStateRepeat[MAX_JOYPAD][DIJS_BUTTON_MAX];	// ジョイパッドの入力情報(リピート情報)
	int m_aCounter[MAX_JOYPAD][DIJS_BUTTON_MAX];		// リピート用のカウンタ
	static int   m_ConnectPad;							// 接続されたパッドの数
	float m_Radian[MAX_JOYPAD];							// 十字キーのラジアンを格納する
	float m_XAxisL[MAX_JOYPAD];						    // 左アナログスティックのX軸を格納する
	float m_YAxisL[MAX_JOYPAD];							// 左アナログスティックのY軸を格納する
	float m_XAxisR[MAX_JOYPAD];						    // 右アナログスティックのX軸を格納する
	float m_YAxisR[MAX_JOYPAD];							// 右アナログスティックのY軸を格納する
};

#endif