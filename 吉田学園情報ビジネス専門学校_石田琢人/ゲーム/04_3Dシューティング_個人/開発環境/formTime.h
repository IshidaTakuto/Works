//=============================================================================
//
// 変形可能成否処理 [formTime.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _FORMTIME_H_
#define _FORMTIME_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_POLYGON_GAUGE_FORMTIME	(2)		// ポリゴンの最大数
#define MAX_GAUGE_FORMTIME	(150)			// ゲージの上限

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFormTime : public CScene
{
public:
	CFormTime(int nPriority = 6);
	~CFormTime();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CFormTime* Create(void);

	bool GetFormSuccess(void) { return m_bSuccess; };	// 変形可能フラグの取得
	void FormTimeReset(void);							// 値のリセット

private:
	CPolygon *m_pPolygon[MAX_POLYGON_GAUGE_FORMTIME];	// ポリゴンクラスのポインタ変数
	int m_nFormTime;									// 変形可能ゲージの値
	bool m_bSuccess;									// 変形できるかどうかのフラグ
	int m_CntFormTime;									// ゲージが溜まり始めるまでのカウンタ
};

#endif
