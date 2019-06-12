//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING			(5)					// ランキングの個数
#define RANKING_MAX_TEXTURE	(MAX_RANKING + 1)	// テクスチャの数
#define RANKING_NUM_BG		(2)					// ランキングの背景の数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CNumber;
class CLogo;
class CLogoPressButton;
class CBg;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRanking : public CScene
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
	LPDIRECT3DTEXTURE9 m_apTexture[RANKING_MAX_TEXTURE];	// テクスチャへのポインタ変数

	CScene2D			*m_apPolygon[MAX_RANKING];			// 2Dポリゴンクラスのポインタ変数
	CNumber				**m_apNumber[MAX_RANKING];			// ナンバークラスのポインタ変数
	static int			m_nRanking[MAX_RANKING];			// ランキングスコア
	static int			m_nScore;							// 獲得したスコア
	CLogo				*m_pLogo;							// ロゴクラスのポインタ変数
	CLogoPressButton	*m_pPress;							// 入力待ちクラスのポインタ変数
	int					m_nCntFade;							// タイトルへフェードするまでの時間
	int					m_nCntHighScore;					// スコア点滅用カウンタ
	int					m_nDigits[MAX_RANKING];				// スコアの桁数
	CBg					*m_pBg[RANKING_NUM_BG];				// 背景のポインタ変数
	CBg					*m_pMountain;						// 背景の山
	int					m_nFlash;							// 点滅させる速度
};

#endif
