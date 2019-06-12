//*****************************************************************************
//
//     リザルトの処理[result.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScene2DSquare;
class CLogo;
class CLogoPress;
class CNumber;
class CMap;

//*****************************************************************************
//    リザルトクラスの定義
//*****************************************************************************
class CResult : public CScene
{
public:     // 誰でもアクセス可能
	typedef enum
	{
		LOGO_RESULT = 0,	// リザルトロゴ
		LOGO_CLEAR_STAGE,	// クリアステージ数
		LOGO_STAGE_BONUS,	// ステージボーナス
		LOGO_TOTAL,      	// 合計スコア
		LOGO_MAX
	}RESULT_LOGO;

	typedef enum
	{
		NUMBER_TOTAL = 0,	       // 合計スコア
		NUMBER_CLEAR_STAGE,	       // クリアステージ数
		NUMBER_CLEAR_STAGE_BONUS,  // クリアステージ数ボーナススコア
		NUMBER_STAGE_BONUS,	       // ステージボーナス
		NUMBER_MAX
	}RESULT_NUMBER;

	CResult(int nPriority = 0, OBJTYPE objType = OBJTYPE_RESULT);
	~CResult();

	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	const static void SetNumClearStage(int nNumClearStage);				// クリアステージ数の設定
	static int GetNumClearStage(void) { return m_nNumClearStage; };		// クリアステージ数の取得

	const static void SetStageBonus(int StageBonus);					// ステージボーナスの設定
	static int GetStageBonus(void) { return m_nStageBonus; };			// ステージボーナスの取得

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	static int m_nNumClearStage;				// クリアしたステージ数
	static int m_nStageBonus;					// クリアしたステージボーナス

	static LPDIRECT3DTEXTURE9 m_apTexture[LOGO_MAX];	// テクスチャのポインタ変数
	static LPDIRECT3DTEXTURE9 m_apTexture2;	// テクスチャのポインタ変数

	CScene2DSquare *m_pMulti;                   // 掛けるポリゴン
	CNumber **m_apNumber[NUMBER_MAX];			// ナンバークラスのポインタ変数
	CLogo *m_apLogo[LOGO_MAX];					// ロゴクラスのポインタ変数
	CLogoPress *m_pLogoPress;					// プレスロゴクラスのポインタ変数
	int m_nScoreTotal;							// スコアの総合
	int m_nScoreNumMap;                         // クリアしたマップ数で加算するスコア
	int m_nCounter;								// 遷移を管理するカウンター
	CMap *m_pMap;								// マップクラスのポインタ変数
	bool m_bChange;
};

#endif