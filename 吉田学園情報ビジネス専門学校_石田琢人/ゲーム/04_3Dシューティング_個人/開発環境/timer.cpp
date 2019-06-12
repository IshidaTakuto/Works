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
#define	TEXTURE_TIMER0		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TIMER1		"data/TEXTURE/frame_timer.png"	// 読み込むテクスチャファイル名
#define NUM_TIMER	(2)		// タイマーの最大桁数
#define TIMER_SIZE	(40)	// タイマーの文字の大きさ
#define MAX_TEX		(2)		// テクスチャの数
#define MAX_TIMER	(NUM_TIMER + 1)
#define FRAME_SIZE_X	(120)	// フレームの大きさ
#define FRAME_SIZE_Y	(100)	// フレームの大きさ
#define TIMER_TIME		(60)	// ゲーム時間

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CNumber *CTimer::m_pNumber[TIMER_DIGITS] = {};
int CTimer::m_nTimer = 0;

//=============================================================================
// 生成処理
//=============================================================================
CTimer* CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer *pTimer;

	pTimer = new CTimer;	// メモリを確保
	for (int nCntScore = 0; nCntScore < TIMER_DIGITS; nCntScore++)
	{
		pTimer->m_pNumber[nCntScore] = CNumber::Create(D3DXVECTOR3((pos.x - (nCntScore * 40)) + (40 * (TIMER_DIGITS / 2.0f)) - 40 / 2, pos.y, 0.0f), TIMER_SIZE);
	}

	pTimer->Init();

	return pTimer;
}

//=========================================
// コンストラクタ
//=========================================
CTimer::CTimer(int nPriority) : CScene(nPriority)
{
	m_nCntSecond = 0;
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
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_nTimer = TIMER_TIME;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < TIMER_DIGITS; nCntScore++)
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
void CTimer::Update(void)
{
	int aNumber[NUM_TIMER];		// 計算用

	m_nCntSecond++;

	if (m_nCntSecond % 60 == 0 && m_nTimer > 0)
	{// 時間を減らす
		m_nTimer--;	// カウントダウン
	}

	if (m_nTimer <= 0 && m_nCntSecond % 180 == 0)
	{// タイムオーバー
		CGame::SetGameState(CGame::STATE_CLEAR);
	}

	aNumber[0] = m_nTimer % 10 / 1;
	aNumber[1] = m_nTimer % 100 / 10;

	// タイマーの更新
	for (int nCntScore = 0; nCntScore < TIMER_DIGITS; nCntScore++)
	{
		m_pNumber[nCntScore]->SetNumber(aNumber[nCntScore]);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCntScore = 0; nCntScore < TIMER_DIGITS; nCntScore++)
	{// ナンバーポリゴンの描画
		m_pNumber[nCntScore]->Draw();
	}
}
