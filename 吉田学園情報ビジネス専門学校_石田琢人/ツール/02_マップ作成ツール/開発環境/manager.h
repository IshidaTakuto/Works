//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "modelObject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GRAVITY				(0.5f)			// 重力

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CInput;
class CRenderer;
class CInputKeyboard;
class CCamera;
class CLight;
class CDebugProc;
class CSetPlayer;
class CMeshField;

//*****************************************************************************
// クラス定義
//*****************************************************************************
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
	static CSetPlayer *GetSetPlayer(void) { return m_pSetPlayer; };		// 設置用プレイヤーの取得
	static CMeshField *GetMeshField(void) { return m_pMeshField; };		// メッシュフィールドの取得

	static int GetNumStage(void) { return m_nNumStage; };
	static char *GetFieldFileName(int nField) { return m_pFieldFileName[nField]; };
	static char *GetSetFileName(int nStage) { return m_pSetFileName[nStage]; };

	HRESULT Load(void);													// システムスクリプトを読み込む

private:
	int SetNumType(void);

	static CRenderer		*m_pRenderer;	// レンダリングクラスのポインタ変数
	static CInputKeyboard	*m_pInput;		// インプットクラスのポインタ変数
	static CCamera			*m_pCamera;		// カメラクラスのポインタ変数
	static CLight			*m_pLight;		// ライトクラスのポインタ変数
	static CDebugProc		*m_pDebugProc;	// デバッグ文字クラスのポインタ変数

	static CSetPlayer *m_pSetPlayer;		// 設置用プレイヤークラスのポインタ変数
	static CMeshField *m_pMeshField;		// メッシュフィールドクラスのポインタ変数

	char	m_aTxtFileName[64];			// 読み込むテキストファイル名
	int		m_nNumEnemyType;			// 敵の種類
	CModelObjectManager::TYPE	m_type;	// 設置オブジェクトの種類

	static int				m_nNumStage;			// ステージ数
	static char				**m_pFieldFileName;		// 地面スクリプト名
	static char				**m_pSetFileName;		// 配置スクリプト名
};

#endif