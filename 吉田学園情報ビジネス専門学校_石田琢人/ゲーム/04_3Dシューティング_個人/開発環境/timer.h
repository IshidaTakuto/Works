//=============================================================================
//
// タイマー処理 [timer.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TIMER_DIGITS	(2)		// 桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTimer : public CScene
{
public:
	CTimer(int nPriority = 6);
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer* Create(D3DXVECTOR3 pos);

	static int GetTime(void) { return m_nTimer; };

private:
	static CNumber *m_pNumber[TIMER_DIGITS];	// ナンバー変数へのポインタ
	static int		m_nTimer;					// 制限時間
	int				m_nCntSecond;				// 秒の計算に必要
};

#endif
