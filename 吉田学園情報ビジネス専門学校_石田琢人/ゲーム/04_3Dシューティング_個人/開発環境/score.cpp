//=============================================================================
//
// スコア処理 [score.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "score.h"
#include "number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_SIZE	(40)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CNumber *CScore::m_pNumber[SCORE_DIGITS] = {};
int CScore::m_nScore = 0;

//=============================================================================
// 生成処理
//=============================================================================
CScore* CScore::Create(D3DXVECTOR3 pos)
{
	CScore *pScore;

	pScore = new CScore;	// メモリを確保
	for (int nCntScore = 0; nCntScore < SCORE_DIGITS; nCntScore++)
	{
		pScore->m_pNumber[nCntScore] = CNumber::Create(D3DXVECTOR3((pos.x - (nCntScore * SCORE_SIZE)) + (SCORE_SIZE * (SCORE_DIGITS / 2.0f)) - SCORE_SIZE, pos.y, 0.0f), SCORE_SIZE);
	}

	pScore->Init();

	return pScore;
}

//=========================================
// コンストラクタ
//=========================================
CScore::CScore(int nPriority) : CScene(nPriority)
{
}

//=========================================
// デストラクタ
//=========================================
CScore::~CScore()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(void)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_UI);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DIGITS; nCntScore++)
	{
		if (NULL != m_pNumber[nCntScore])
		{
			m_pNumber[nCntScore]->Uninit();
			delete m_pNumber[nCntScore];
			m_pNumber[nCntScore] = NULL;
		}
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DIGITS; nCntScore++)
	{
		m_pNumber[nCntScore]->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DIGITS; nCntScore++)
	{// ナンバーポリゴンの描画
		m_pNumber[nCntScore]->Draw();
	}
}

//=============================================================================
// スコアの加算
//=============================================================================
const void CScore::AddScore(int nValue)
{
	int aNumber;	// 計算結果を格納

	m_nScore += nValue;			// スコアの加算

	for (int nCntDigits = 0; nCntDigits < SCORE_DIGITS; nCntDigits++)
	{// 桁別に格納
		aNumber = m_nScore % (int)powf(10.0f, (nCntDigits + 1) * 1.0f) / (int)powf(10.0f, nCntDigits * 1.0f);
		m_pNumber[nCntDigits]->SetNumber(aNumber);	// スコアの更新
	}
}
