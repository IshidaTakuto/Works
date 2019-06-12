//=============================================================================
//
// スコア処理 [score.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_DIGITS	(8)		// 桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore(int nPriority = 6);
	~CScore();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore* Create(D3DXVECTOR3 pos);

	static const void AddScore(int nValue);

	static int GetScore(void) { return m_nScore; };

private:
	static CNumber *m_pNumber[SCORE_DIGITS];
	static int m_nScore;		// 現在のスコア
};

#endif
