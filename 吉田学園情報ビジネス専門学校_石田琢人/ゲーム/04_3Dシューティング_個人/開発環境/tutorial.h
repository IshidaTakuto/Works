//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "field.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CString;
class CCommunicateWindow;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTutorial
{
public:
	CTutorial();
	~CTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CField *GetField(int nIndex) { return m_pField[nIndex]; };	// フィールドの取得

private:
	static CField	*m_pField[MAX_FIELD];	// フィールドクラスのポインタ変数

	CString *m_pString[2];		// 文字表示クラスのポインタ変数
	CCommunicateWindow *m_pCommunicateWindow;
	int	m_nCntString;			// 文字の表示時間のカウンタ
	int m_nCntEnemyAppear;		// 敵の出現用カウンタ
	int m_nCntTutorialTime;		// チュートリアルの時間計測
};

#endif