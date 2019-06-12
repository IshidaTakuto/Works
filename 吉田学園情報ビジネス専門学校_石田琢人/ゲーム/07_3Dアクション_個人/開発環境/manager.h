//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : Takuto Ishida
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
class CInputKeyboard;
class CInputJoyStick;
class CXInput;
class CCamera;
class CLight;
class CDebugProc;
class CSound;

class CTitle;
class CTutorial;
class CGame;
class CResult;
class CRanking;

//=====================================
// クラス定義
//=====================================
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

	HRESULT Load(void);
	void Unload(void);

	const static void SetMode(MODE mode);
	static MODE GetMode(void) { return m_mode; };

	static CRenderer *GetRenderer(void) { return m_pRenderer; };					// レンダラーの取得
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; };		// インプット(キーボード)の取得
	static CInputJoyStick *GetJoyStick(void) { return m_pJoyStick; };				// インプット(DirectInput)の取得
	static CXInput *GetXInput(void) { return m_pXInput; };							// インプット(XInput)の取得
	static CSound *GetSound(void) { return m_pSound; };								// サウンドの取得
	static CCamera *GetCamera(void) { return m_pCamera; };							// カメラの取得
	static CLight *GetLight(void) { return m_pLight; };								// ライトの取得
	static CDebugProc *GetDebugProc(void) { return m_pDebugProc; };					// デバッグ文字の取得

	static CGame *GetGame(void) { return m_pGame; };

	static int GetNumStage(void) { return m_nNumStage; };
	static char *GetFieldFileName(int nField) { return m_pFieldFileName[nField]; };
	static char *GetSetFileName(int nStage) { return m_pSetFileName[nStage]; };

private:
	static CRenderer		*m_pRenderer;			// レンダリングクラスのポインタ変数
	static CInputKeyboard	*m_pInputKeyboard;		// インプットクラスのポインタ変数
	static CInputJoyStick	*m_pJoyStick;			// DirectInputクラス型のポインタ変数
	static CXInput			*m_pXInput;				// XInputクラス型のポインタ変数
	static CCamera			*m_pCamera;				// カメラクラスのポインタ変数
	static CLight			*m_pLight;				// ライトクラスのポインタ変数
	static CDebugProc		*m_pDebugProc;			// デバッグ文字クラスのポインタ変数
	static CSound			*m_pSound;				// サウンドクラスのポインタ変数

	static CTitle			*m_pTitle;				// タイトルクラスのポインタ変数
	static CTutorial		*m_pTutorial;			// チュートリアルクラスのポインタ変数
	static CGame			*m_pGame;				// ゲームクラスのポインタ変数
	static CResult			*m_pResult;				// リザルトクラスのポインタ変数
	static CRanking			*m_pRanking;			// ランキングクラスのポインタ変数
	static MODE				m_mode;					// 現在のフレームワーク

	static int				m_nNumStage;			// ステージ数
	static char				**m_pFieldFileName;		// 地面スクリプト名
	static char				**m_pSetFileName;		// 配置スクリプト名
	bool					m_bDebugProc;			// デバッグ文字を表示させるかどうか
};

#endif