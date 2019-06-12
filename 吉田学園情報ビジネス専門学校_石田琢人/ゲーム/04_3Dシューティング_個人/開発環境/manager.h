//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CInput;
class CRenderer;
class CSound;
class CInputKeyboard;
class CInputJoypad;
class CCamera;
class CLight;
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CRanking;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CManager
{
public:
	typedef enum
	{// フレームワーク
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_GAMEOVER,
		MODE_RANKING,
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer *GetRenderer(void) { return m_pRenderer; };				// レンダラーの取得
	static CCamera *GetCamera(void) { return m_pCamera; };						// カメラの取得
	static CLight *GetLight(void) { return m_pLight; };							// ライトの取得
	static CSound *GetSound(void) { return m_pSound; };							// サウンドの取得
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; };	// キーボードのインプットの取得
	static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; };		// パッドのインプットの取得
	static CTitle *GetTitle(void) { return m_pTitle; };							// タイトルの取得
	static CTutorial *GetTutorial(void) { return m_pTutorial; };				// チュートリアルの取得
	static CGame *GetGame(void) { return m_pGame; };							// ゲームの取得
	static CResult *GetResult(void) { return m_pResult; };						// リザルトの取得
	static CRanking *GetRanking(void) { return m_pRanking; };					// ランキングの取得

	static MODE GetMode(void) { return m_mode; };
	static const void SetMode(MODE mode);

private:
	static CRenderer		*m_pRenderer;				// レンダリングクラスのポインタ変数
	static CCamera			*m_pCamera;					// カメラクラスのポインタ変数
	static CLight			*m_pLight;					// ライトクラスのポインタ変数
	static CSound			*m_pSound;					// サウンドクラスのポインタ変数
	static CInputKeyboard	*m_pInputKeyboard;			// キーボードのインプットクラスのポインタ変数
	static CInputJoypad		*m_pInputJoypad;			// パッドのインプットクラスのポインタ変数
	static CTitle			*m_pTitle;					// タイトルクラスのポインタ変数
	static CTutorial		*m_pTutorial;				// チュートリアルクラスのポインタ変数
	static CGame			*m_pGame;					// ゲームクラスのポインタ変数
	static CResult			*m_pResult;					// リザルトクラスのポインタ変数
	static CRanking			*m_pRanking;				// ランキングクラスのポインタ変数
	static MODE				m_mode;						// 現在のフレームワーク
};

#endif