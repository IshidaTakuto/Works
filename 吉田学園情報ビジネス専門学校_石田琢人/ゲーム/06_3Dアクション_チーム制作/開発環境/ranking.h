//*****************************************************************************
//
//     ランキングの処理[ranking.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CNumber;
class CLogo;
class CScene2DSquare;
class CMap;

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MAX_RANKING             (5)     // ランキングのスコア数
#define MAX_RANKING_TEXTURE		(2)		// ランキングのテクスチャ数

//*****************************************************************************
//    ランキングクラスの定義
//*****************************************************************************
class CRanking : public CScene
{
public:     // 誰でもアクセス可能
	//------------------------
	//  ランキングスコアの状態
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // 何もない状態
		STATE_UPDATE,    // 更新された状態
		STATE_MAX
	}STATE;

	CRanking(int nPriority = 3, OBJTYPE objType = OBJTYPE_RANKING);
	~CRanking();

	static CRanking *Create(void);

	static void RankingScoreInit(void);
	static void SetRankingScore(int nScore);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	static int   m_nRankingScore[MAX_RANKING];   // ランキングスコア
	static STATE m_State[MAX_RANKING];           // スコアの状態
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_RANKING_TEXTURE];		// ランキング順位用のテクスチャへのポインタ
	CScene2DSquare *m_apSceneSquare[MAX_RANKING];					// ランキング順位用ポリゴン
	CNumber     **m_apNumber[MAX_RANKING];							// ランキングスコアの数字ポリゴン
	CLogo       *m_pLogo;											// ランキングロゴクラス型のポインタ
	int         m_nCounter;											// 画面遷移を管理するカウンター
	static int	m_nScore;											// 最新の獲得したスコア
	bool		m_bFlash;											// 点滅用の判定
	int			m_aScoreDigits[MAX_RANKING];						// スコアの桁数
	CMap		*m_pMap;											// マップクラスのポインタ変数
};

#endif