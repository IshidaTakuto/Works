//*****************************************************************************
//
//     タイマーの処理[timer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "timer.h"
#include "number.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define TIMER_POS_INI         (D3DXVECTOR3(1200.0f,60.0f,0.0f))   // タイマーの座標(初期値)
#define TIMER_COL_INI         (D3DXCOLOR(0.0f,1.0f,1.0f,1.0f))    // タイマーの色(初期値)
#define TIMER_WIDTH_INI       (60.0f)                             // タイマーの座標(初期値)
#define TIMER_HEIGHT_INI      (60.0f)                             // タイマーの高さ(初期値)
#define TIMER_INI             (180 * 60)                          // タイマーの初期値
#define TIMER_WARNING         (40 * 60)                           // タイマーが赤くなるタイミング
#define TIMER_NUMBER_PRIORITY (6)                                 // タイマーの処理優先順位

//=============================================================================
//    コンストラクタ
//=============================================================================
CTimer::CTimer(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER_MAX; nCntTimer++)
	{// スコアの桁数分繰り返し
		m_apNumber[nCntTimer] = NULL;
	}
	m_nTimer = 0;  // タイマー
}

//=============================================================================
//    デストラクタ
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTimer::Init(void)
{
	// 各種値の初期化
	D3DXVECTOR3 NumberPos = TIMER_POS_INI;   // 座標
	D3DXCOLOR NumberCol = TIMER_COL_INI;     // 色
	float fNumberWidth = TIMER_WIDTH_INI;    // 幅
	float fNumberHeight = TIMER_HEIGHT_INI;  // 高さ
	int nNumber = 0;                         // 表示する数字の計算用
	m_nTimer = TIMER_INI;                    // タイマー

	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER_MAX; nCntTimer++)
	{// スコアの桁数分繰り返し
		if (m_apNumber[nCntTimer] == NULL)
		{// メモリが確保できる状態にある
			// 表示する数字の計算
			nNumber = (m_nTimer / 60) % ((int)powf(10.0f, (float)nCntTimer) * 10) / (int)powf(10.0f, (float)nCntTimer);

			// 数字ポリゴンのインスタンス生成処理
			m_apNumber[nCntTimer] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, nNumber, TIMER_NUMBER_PRIORITY);

			// 横に進める
			NumberPos.x -= TIMER_WIDTH_INI + (TIMER_WIDTH_INI * 0.5f);
		}
	}
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER_MAX; nCntTimer++)
	{// タイマーの桁数分繰り返し
		if (m_apNumber[nCntTimer] != NULL)
		{// メモリが確保されている
		    // 数字ポリゴンの終了処理
			m_apNumber[nCntTimer]->Uninit();
			m_apNumber[nCntTimer] = NULL;
		}
	}

	// タイマーの消去処理
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CTimer::Update(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲームモード中だったら
		CGame *pGame = CManager::GetGame();  // ゲームのインスタンスを取得
		if(pGame != NULL)
		{// ゲームのインスタンスが取得できた
			if (pGame->GetState() == CGame::STATE_CHALLENGE)
			{// マップ攻略中ならば
				if (m_nTimer > 0)
				{// タイマーがまだある
					// タイマーを減らす
					m_nTimer--;
					if (m_nTimer % 60 == 0)
					{// アップデートが60回まわった
						int nSetNumber = 0;   // 表示する数字の計算用
						for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER_MAX; nCntTimer++)
						{// タイマーの桁数分繰り返し
							if (m_nTimer == TIMER_WARNING)
							{// タイマーがなくなりそうならば
								m_apNumber[nCntTimer]->SetVtxBuffCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
								CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WARNING);
							}

							// 表示する数字の計算
							nSetNumber = (m_nTimer / 60) % ((int)powf(10.0f, (float)nCntTimer) * 10) / (int)powf(10.0f, (float)nCntTimer);

							// 数字ポリゴンの数字設定処理
							m_apNumber[nCntTimer]->SetNumber(nSetNumber);
						}
					}
				}
				else
				{// タイマーがなくなった
					if (CManager::GetMode() == CManager::MODE_GAME)
					{// ゲームモード中だったら
						if (pGame->GetState() != CGame::STATE_END)
						{// ゲーム終了状態でない
							pGame->SetState(CGame::STATE_END);
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_FINISH);
						}
					}
				}
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CTimer::Draw(void)
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTimer *CTimer::Create(void)
{
	CTimer *pTimer = NULL;                 // タイマークラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pTimer == NULL)
		{// メモリが空になっている
			pTimer = new CTimer;
			if (pTimer != NULL)
			{// メモリを確保できた
				if (FAILED(pTimer->Init()))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// メモリを確保できなかった
				return NULL;
			}
		}
		else
		{// メモリが空ではない
			return NULL;
		}
	}
	else
	{// これ以上生成できない
		return NULL;
	}

	return pTimer;
}