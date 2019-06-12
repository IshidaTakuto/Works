//=============================================================================
//
// リザルト処理 [result.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "field.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_POLYGON		(2)		// 使うポリゴンの数
#define RESULT_SCORE_DIGITS	(8)		// スコアの最大桁数
#define RESULT_KILL_DIGITS	(3)		// 倒した敵の最大桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;
class CNumber;
class CScene3DModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResult
{
public:
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetScore(int nScore) { m_nScore = nScore; };
	static void AddKillEnemy(void) { m_nNumKill++; };

private:
	CPolygon *m_apPolygon[RESULT_POLYGON];		// ポリゴンクラスのポインタ変数
	CNumber *m_apNumScore[RESULT_SCORE_DIGITS];	// ナンバークラスのポインタ変数
	CNumber *m_apNumKill[RESULT_KILL_DIGITS];	// ナンバークラスのポインタ変数

	CField	*m_pField[MAX_FIELD];	// フィールドクラスのポインタ変数
	CScene3DModel *m_model;			// モデルクラスのポインタ変数

	static int	m_nScore;			// スコア
	static int	m_nNumKill;			// 倒した敵の数
	int			m_nCntResult;		// リザルトの時間
	int			m_nScoreDigit;		// スコアの桁
	int			m_nKillDigit;		// キル数の桁
};

#endif