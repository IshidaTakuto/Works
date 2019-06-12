//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "ranking.h"
#include "fade.h"
#include "input.h"
#include "polygon.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "meshCylinder.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKING_TIME			(420)			// タイトル画面までの時間
#define RANKINGNUM_SIZE			(40.0f)			// 数字の大きさ

//==================================
// 静的メンバ変数宣言
//==================================
int CRanking::m_nRanking[MAX_RANKING] = { 15000, 13000, 10000, 5000, 1000 };
int CRanking::m_nScore = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CRanking::CRanking()
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ナンバーを初期化
		for (int nCntNumber = 0; nCntNumber < RANKING_DIGITS; nCntNumber++)
		{
			m_apNumber[nCntRanking][nCntNumber] = NULL;
			m_aDestPos[nCntRanking][nCntNumber] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		m_bArrive[nCntRanking] = false;
	}

	for (int nCntPolygon = 0; nCntPolygon < NUM_POLYGON; nCntPolygon++)
	{// ポリゴンを初期化
		m_apPolygon[nCntPolygon] = NULL;
	}

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{// フィールドを初期化
		m_pField[nCntField] = NULL;
	}

	m_nCntFade = 0;
	m_nCntHighScore = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	// テクスチャの読み込み
	CNumber::Load();
	CPolygon::Load();
	CField::Load();
	CCylinder::Load();

	// ランキングのロゴ
	m_apPolygon[0] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 60, 0.0f), 200, 160, CPolygon::TYPE::TYPE_RANKING_LOGO);

	// ランキングのスコア(数字)
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数
		for (int nCntDigit = 0; nCntDigit < RANKING_DIGITS; nCntDigit++)
		{// ランキングスコアの桁数
			m_apNumber[nCntRanking][nCntDigit] = CNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 2.5f + (RANKINGNUM_SIZE * RANKING_DIGITS) - RANKINGNUM_SIZE) - (nCntDigit * (RANKINGNUM_SIZE * 2)), (125.0f + (RANKINGNUM_SIZE * 2)) + (nCntRanking * RANKINGNUM_SIZE * (3.3f - (nCntRanking * 0.1f))), 0.0f), RANKINGNUM_SIZE - (nCntRanking * 4));

			// 目的の位置
			m_aDestPos[nCntRanking][nCntDigit] = D3DXVECTOR3((SCREEN_WIDTH / 2 + (RANKINGNUM_SIZE * RANKING_DIGITS) - RANKINGNUM_SIZE) - (nCntDigit * (RANKINGNUM_SIZE * 2)), (125.0f + (RANKINGNUM_SIZE * 2)) + (nCntRanking * RANKINGNUM_SIZE * (3.3f - (nCntRanking * 0.1f))), 0.0f);
		}
	}

	CCamera *pCamera = CManager::GetCamera();			// カメラの情報を取得
	pCamera->SetPosV(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// カメラの位置の設定

	// シリンダーの生成
	CCylinder::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), 8000, 7000, 8000, 20, 1, 0, 0);

	// フィールドの生成
	m_pField[0] = CField::Create(CField::TYPE_SEA);
	m_pField[1] = CField::Create(CField::TYPE_SEA);
	m_pField[2] = CField::Create(CField::TYPE_GROUND);

	m_bArrive[MAX_RANKING - 1] = true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	// テクスチャの破棄
	CNumber::Unload();
	CPolygon::Unload();
	CField::Unload();
	CCylinder::Unload();

	// ポリゴンの開放
	for (int nCntPolygon = 0; nCntPolygon < NUM_POLYGON; nCntPolygon++)
	{
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			m_apPolygon[nCntPolygon]->Uninit();	// ポリゴンの終了処理
			delete m_apPolygon[nCntPolygon];
			m_apPolygon[nCntPolygon] = NULL;
		}
	}

	// ナンバーの開放
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntNumber = 0; nCntNumber < RANKING_DIGITS; nCntNumber++)
		{
			if (m_apNumber[nCntRanking][nCntNumber] != NULL)
			{
				m_apNumber[nCntRanking][nCntNumber]->Uninit();	// ナンバーの終了処理
				delete m_apNumber[nCntRanking][nCntNumber];
				m_apNumber[nCntRanking][nCntNumber] = NULL;
			}
		}
	}

	// フィールドクラスの開放
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (NULL != m_pField[nCntField])
		{
			m_pField[nCntField] = NULL;
		}
	}

	// フェード以外のオブジェクトを破棄
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			CScene *pScene = CScene::GetScene(nCntPriority, nCnt);

			if (pScene != NULL)
			{
				CScene::OBJTYPE objType = pScene->GetObjType();		// オブジェクトタイプの取得
				if (objType != CScene::OBJTYPE_FADE)
				{// フェード以外のオブジェクトを破棄
					pScene->Uninit();
				}
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	// 点滅用カウントアップ
	m_nCntHighScore++;
	m_nCntHighScore = m_nCntHighScore % 4;

	// 自分の取得したスコアを点滅させる
	if (m_nScore > -1)
	{// スコアが入っている
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			if (m_nRanking[nCntRanking] == m_nScore)
			{// 自分のスコアを点滅させる
				for (int nCntNumber = 0; nCntNumber < RANKING_DIGITS; nCntNumber++)
				{// 桁数分まわす
					if (m_nCntHighScore < 2)
					{
						m_apNumber[nCntRanking][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					}
					else if (m_nCntHighScore >= 2)
					{
						m_apNumber[nCntRanking][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
					}
				}
				break;	// 同じスコアがあった場合の保険
			}
		}
	}

	D3DXVECTOR3 deffPos;	// 差分
	D3DXVECTOR3 pos;		// 現在
	int nCntNumber;

	// ランキングスコアを目的の位置まで移動
	for (int nCntRanking = MAX_RANKING - 1; nCntRanking >= 0; nCntRanking--)
	{
		if (m_bArrive[nCntRanking] == true)
		{// 一つ前のスコアが目的の位置についたら
			for (nCntNumber = 0; nCntNumber < RANKING_DIGITS; nCntNumber++)
			{// 桁数分まわす
				pos = m_apNumber[nCntRanking][nCntNumber]->GetPos();

				if (pos.x >= m_aDestPos[nCntRanking][nCntNumber].x)
				{// 一定の位置まで動かす
					deffPos = m_aDestPos[nCntRanking][nCntNumber] - pos;	// 差分の計算

					pos.x += deffPos.x * 0.4f;
					pos.y += deffPos.y * 0.4f;
					pos.z += deffPos.z * 0.4f;

					m_apNumber[nCntRanking][nCntNumber]->SetPos(pos);
				}

				if (pos.x - 0.1f <= m_aDestPos[nCntRanking][nCntNumber].x)
				{// スコアが目的の位置についた
					if (nCntRanking > 0)
					{// 次のスコアを動かせるようにする
						m_bArrive[nCntRanking - 1] = true;
					}
				}
			}
		}
	}

	// ランキングスコアの設定
	int aNumber;

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数分
		for (int nCntDigits = 0; nCntDigits < RANKING_DIGITS; nCntDigits++)
		{// 桁の小さい方から設定
			aNumber = m_nRanking[nCntRanking] % (int)powf(10.0f, (nCntDigits + 1) * 1.0f) / (int)powf(10.0f, nCntDigits * 1.0f);
			m_apNumber[nCntRanking][nCntDigits]->SetNumber(aNumber);
		}
	}

	CCamera *pCamera = CManager::GetCamera();	// カメラの情報を取得
	D3DXVECTOR3 posCamera = pCamera->GetPosV();	// カメラの位置を取得

	// フィールドの自動生成
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (NULL != m_pField[nCntField])
		{
			D3DXVECTOR3 pos = m_pField[nCntField]->GetPos();		// フィールドの位置を取得
			D3DXVECTOR3 length = m_pField[nCntField]->GetLength();	// フィールドの長さを取得

			if (pos.x + length.x < posCamera.x - 500)
			{// フィールドの開放
				if (NULL != m_pField[nCntField])
				{
					m_pField[nCntField]->Uninit();
					m_pField[nCntField] = NULL;
				}
			}
		}
		else
		{// フィールドの生成
			if (rand() % 2 == 0)
			{// 地面
				m_pField[nCntField] = CField::Create(CField::TYPE_GROUND);
			}
			else
			{// 海
				m_pField[nCntField] = CField::Create(CField::TYPE_SEA);
			}
		}
	}

	// レンダラーの更新を呼ぶ
	CRenderer *pRenderer = CManager::GetRenderer();
	pRenderer->Update();

	if (pKeyboard->GetAnyKey() || pJoypad->GetAnyTrigger(0))
	{
		//モード選択
		CFade::Create(CManager::MODE_TITLE);
	}

	m_nCntFade++;
	if (m_nCntFade >= RANKING_TIME)
	{
		//モード選択
		CFade::Create(CManager::MODE_TITLE);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < NUM_POLYGON; nCntPolygon++)
	{// ポリゴンの描画
		m_apPolygon[nCntPolygon]->Draw();
	}

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ナンバーの描画
		for (int nCntNumber = 0; nCntNumber < RANKING_DIGITS; nCntNumber++)
		{
			m_apNumber[nCntRanking][nCntNumber]->Draw();
		}
	}
}

//=============================================================================
// スコアのセット
//=============================================================================
void CRanking::Set(int nScore)
{
	int nSwap;							// 入れ替え用
	int aEmptyData[MAX_RANKING + 1];	// 入れ替え用

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{// 今までの記録を空のデータに入れる
		aEmptyData[nCntNum] = m_nRanking[nCntNum];
	}

	aEmptyData[MAX_RANKING] = nScore;	// 最後の配列に最新のスコアを入れる

	// 入れ替え
	for (int nCntNum = 1; nCntNum < MAX_RANKING + 1; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aEmptyData[nCntNum1 - 1] <= aEmptyData[nCntNum1])
			{
				nSwap = aEmptyData[nCntNum1];
				aEmptyData[nCntNum1] = aEmptyData[nCntNum1 - 1];
				aEmptyData[nCntNum1 - 1] = nSwap;
			}
		}
	}

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{// 入れ替えが終わったデータをランキングに反映する
		m_nRanking[nCntNum] = aEmptyData[nCntNum];

	}

	m_nScore = nScore;	// 最新のスコアを更新する
}
