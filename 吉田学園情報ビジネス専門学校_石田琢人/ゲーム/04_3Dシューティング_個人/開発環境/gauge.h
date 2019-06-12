//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_POLYGON_GAUGE	(2)		// ポリゴンの最大数
#define MAX_GAUGE	(100)			// ゲージの上限

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGauge : public CScene
{
public:
	CGauge(int nPriority = 6);
	~CGauge();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGauge* Create(void);

	static const void AddSubtract(int nValue);	// ライフを増減させる
	static int Get(void) { return m_nGauge; };	// ライフの取得
	static void Set(int nGauge) { m_nGauge = nGauge; };

private:
	static CPolygon *m_pPolygon[MAX_POLYGON_GAUGE];	// ポリゴンクラスのポインタ変数
	static int m_nGauge;						// 現在のゲージの値
};

#endif
