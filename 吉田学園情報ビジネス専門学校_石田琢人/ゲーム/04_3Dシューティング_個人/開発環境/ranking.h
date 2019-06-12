//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "field.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKING_DIGITS		(8)				// 桁数
#define MAX_RANKING			(5)				// ランキングの個数
#define NUM_POLYGON			(1)				// ポリゴンの個数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;
class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRanking
{
public:
	CRanking();
	~CRanking();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void Set(int nScore);

private:
	CField		*m_pField[MAX_FIELD];			// フィールドクラスのポインタ変数
	CPolygon	*m_apPolygon[NUM_POLYGON];		// ポリゴンクラスのポインタ変数
	CNumber		*m_apNumber[MAX_RANKING][RANKING_DIGITS];		// ナンバークラスのポインタ変数
	static int	m_nRanking[MAX_RANKING];		// ランキングスコア
	static int	m_nScore;						// 獲得したスコア
	D3DXVECTOR3	m_aDestPos[MAX_RANKING][RANKING_DIGITS];		// スコアの表示位置
	int			m_nCntFade;						// タイトルへフェードするまでの時間
	int			m_nCntHighScore;				// スコア点滅用カウンタ
	bool		m_bArrive[MAX_RANKING];			// 目的の位置についたかどうか
};

#endif
