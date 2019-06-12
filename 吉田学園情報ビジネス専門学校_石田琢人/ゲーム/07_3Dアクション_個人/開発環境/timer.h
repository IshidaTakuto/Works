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
#define TIMER_PRIORITY	(6)		// 処理の優先順位

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTimer : public CScene
{
public:
	CTimer(int nPriority = TIMER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_STAGE);
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer* Create(D3DXVECTOR3 pos, int nTime);

	int GetTime(void) { return m_nTimer; };

private:
	CScene2D		*m_pFrame;			// 枠
	D3DXVECTOR3		m_pos;				// 位置
	CNumber			**m_pNumber;		// ナンバー変数へのポインタ
	int				m_nTimer;			// 現在の時間
	int				m_nCntSecond;		// 秒の計算に必要
	int				m_nDigits;			// 桁数
};

#endif
