//=============================================================================
//
// GUI管理処理 [GUIManager.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _GUIMANAGER_H_
#define _GUIMANAGER_H_

#include "main.h"
#include "ImGui/imgui.h"
#include "modelObject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGuiManager
{
public:
	CGuiManager();
	~CGuiManager();
	HRESULT Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGuiManager *Create(void);

	static CModelObjectManager::TYPE GetObjectType(void) { return m_type; };		// 設置オブジェクトの種類の取得
	static void SetObjectType(CModelObjectManager::TYPE type) { m_type = type; };	// 設置オブジェクトの種類の設定

	static int GetType(void) { return m_nType; };						// モデルの種類の取得
	static float GetRange(void) { return m_fRange; };					// 選択範囲の取得
	static float GetHeight(void) { return m_fHeight; };					// 上げる高さの取得
	static float GetLength(int nArray) { return m_aLength[nArray]; };	// 長さの取得
	static void SetLength(float nLengthX, float nLengthZ);				// 長さの設定

	static int GetSplit(int nArray) { return m_aSplit[nArray]; };		// 分割数の取得
	static void SetSplit(int nSplitX, int nSplitZ);						// 分割数の設定

	void SetPresent(HRESULT result, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS d3dpp);

private:
	void LoadFont(void);									// 全ての日本語を使えるようにする
	void SpaceLine(bool bPartition = false, int nLine = 1);	// 改行

	static CModelObjectManager::TYPE m_type;	// 設置オブジェクトの種類
	static int	m_nType;						// モデルの種類
	static float m_fRange;						// 選択範囲
	static float m_fHeight;						// 上げる高さ
	static float m_aLength[2];					// 長さ
	static int m_aSplit[2];						// 分割数

	char **m_aModelFileName[CModelObjectManager::TYPE_MAX];

	bool m_showDemoWindow;		// デモウインドウの表示
	bool m_showFieldWindow;		// 地面ウインドウの表示
	ImVec4 m_clearColor;		// 色
	bool m_bRepeat;				// ボタンを押下フラグ

	ImWchar m_glyphRangesJapanese[8253];
};

#endif
