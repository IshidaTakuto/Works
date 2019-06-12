//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

class CInput;
class CRenderer;
class CInputKeyboard;
class CCamera;
class CLight;
class CDebugProc;
class CPlayer;
class CMeshField;
class CCylinder;

//=====================================
// クラス定義
//=====================================
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer *GetRenderer(void) { return m_pRenderer; };		// レンダラーの取得
	static CInputKeyboard *GetInput(void) { return m_pInput; };			// インプットの取得
	static CCamera *GetCamera(void) { return m_pCamera; };				// カメラの取得
	static CDebugProc *GetDebugProc(void) { return m_pDebugProc; };		// デバッグ文字の取得
	static CPlayer *GetPlayer(void) { return m_pPlayer; };				// プレイヤーの取得
	static CMeshField *GetMeshField(void) { return m_pMeshField; };		// メッシュフィールドの取得
	static CCylinder *GetCylinder(void) { return m_pCylinder; };		// メッシュシリンダーの取得

private:
	static CRenderer		*m_pRenderer;	// レンダリングクラスのポインタ変数
	static CInputKeyboard	*m_pInput;		// インプットクラスのポインタ変数
	static CCamera			*m_pCamera;		// カメラクラスのポインタ変数
	static CLight			*m_pLight;		// ライトクラスのポインタ変数
	static CDebugProc		*m_pDebugProc;	// デバッグ文字クラスのポインタ変数

	static CPlayer *m_pPlayer;				// プレイヤークラスのポインタ変数
	static CMeshField *m_pMeshField;		// メッシュフィールドクラスのポインタ変数
	static CCylinder *m_pCylinder;			// メッシュシリンダークラスのポインタ変数
};

#endif