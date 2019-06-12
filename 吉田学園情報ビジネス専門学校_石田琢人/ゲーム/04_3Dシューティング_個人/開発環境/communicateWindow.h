//=============================================================================
//
// 通信ウィンドウ処理 [communicateWindow.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _COMMUNICATEWINDOW_H_
#define _COMMUNICATEWINDOW_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_POLYGON_COMMUNICATE	(2)		// ポリゴンの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCommunicateWindow : public CScene
{
public:
	CCommunicateWindow();
	~CCommunicateWindow();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CCommunicateWindow* Create(void);

	bool GetCompleted(void) { return m_bCompleted; };

private:
	static CPolygon *m_apPolygon[MAX_POLYGON_COMMUNICATE];	// ポリゴンクラスのポインタ変数
	int m_nCntTransparent;									// 透明化するまでの値
	bool m_bCompleted;										// 通信ウィンドウの準備ができたらtrue
};

#endif
