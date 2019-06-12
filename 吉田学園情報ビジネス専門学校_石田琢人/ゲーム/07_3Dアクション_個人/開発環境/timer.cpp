//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "timer.h"
#include "number.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TIMER_TEXTURE	"data/TEXTURE/time.png"
#define TIMER_WIDTH		(60)	// タイマーの文字の幅
#define TIMER_HEIGHT	(60)	// タイマーの文字の高さ

#define TIMER_POS		(D3DXVECTOR3((m_pos.x - (nCntDigits * TIMER_WIDTH)) + (TIMER_WIDTH * (m_nDigits / 2.0f)) - TIMER_WIDTH / 2, m_pos.y, 0.0f))

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// 生成処理
//=============================================================================
CTimer* CTimer::Create(D3DXVECTOR3 pos, int nTime)
{
	CTimer *pTimer;

	pTimer = new CTimer;	// メモリを確保

	if (pTimer)
	{// メモリ確保成功
		pTimer->m_pos = pos;		// 位置の設定
		pTimer->m_nTimer = nTime;	// 時間の設定
		pTimer->Init();				// 初期化処理
	}

	return pTimer;
}

//=========================================
// コンストラクタ
//=========================================
CTimer::CTimer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nDigits = 0;
	m_nTimer = 0;
	m_nCntSecond = 0;
	m_pNumber = NULL;
	m_pFrame = NULL;
}

//=========================================
// デストラクタ
//=========================================
CTimer::~CTimer()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTimer::Init(void)
{
	m_nDigits = (int)log10f((float)m_nTimer) + 1;	// 桁数を求める
	if (m_nDigits <= 0) { m_nDigits = 1; }			// 桁数が0以下のとき

	m_pNumber = new CNumber*[m_nDigits];			// メモリを確保

	// 枠の生成
	m_pFrame = CScene2D::Create();

	if (m_pNumber)
	{// メモリ確保成功
		for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
		{// 桁数分ループ
			m_pNumber[nCntDigits] = CNumber::Create(TIMER_POS, TIMER_WIDTH, TIMER_HEIGHT);

			// 数字の設定
			int aNumber = m_nTimer % (int)powf(10.0f, (float)(nCntDigits + 1)) / (int)powf(10.0f, (float)nCntDigits);
			m_pNumber[nCntDigits]->SetNumber(aNumber);

			// 色の設定
			m_pNumber[nCntDigits]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < m_nDigits; nCntScore++)
	{// 桁数分ループ
		if (NULL != m_pNumber[nCntScore])
		{// 使われている場合破棄する
			m_pNumber[nCntScore]->Uninit();
			m_pNumber[nCntScore] = NULL;
		}
	}

	if (NULL != m_pNumber)
	{// 使われている場合破棄する
		delete m_pNumber;
		m_pNumber = NULL;
	}

	// 自分の破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTimer::Update(void)
{
	// フレームのカウント
	m_nCntSecond++;

	if (m_nCntSecond % 60 == 0)
	{// 時間を減らす
		m_nTimer++;		// カウント

		int nDigits = (int)log10f((float)m_nTimer) + 1;	// 桁数を求める
		if (nDigits <= 0) { nDigits = 1; }			// 桁数が0以下のとき

		if (nDigits != m_nDigits)
		{// 桁数が変わった
			for (int nCntScore = 0; nCntScore < m_nDigits; nCntScore++)
			{// 桁数分ループ
				if (NULL != m_pNumber[nCntScore])
				{// 使われている場合破棄する
					m_pNumber[nCntScore]->Uninit();
					m_pNumber[nCntScore] = NULL;
				}
			}

			if (NULL != m_pNumber)
			{// 使われている場合破棄する
				delete m_pNumber;
				m_pNumber = NULL;
			}

			// 桁数の設定
			m_nDigits = nDigits;

			// メモリ確保
			m_pNumber = new CNumber*[m_nDigits];

			if (m_pNumber)
			{// メモリ確保できた
				for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
				{// 桁数分ループ
					m_pNumber[nCntDigits] = CNumber::Create(TIMER_POS, TIMER_WIDTH, TIMER_HEIGHT);
					m_pNumber[nCntDigits]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				}
			}
		}

		// 数字の設定
		int aNumber;
		for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
		{// 桁の小さい方から設定
			aNumber = m_nTimer % (int)powf(10.0f, (float)(nCntDigits + 1)) / (int)powf(10.0f, (float)nCntDigits);
			m_pNumber[nCntDigits]->SetNumber(aNumber);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTimer::Draw(void)
{
}
