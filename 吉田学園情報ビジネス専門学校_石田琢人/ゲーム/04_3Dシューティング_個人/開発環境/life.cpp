//=============================================================================
//
// 体力処理 [life.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "life.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BAR_WIDTH_LIFE	(300)		// ライフバーの幅
#define BAR_HEIGHT_LIFE	(40)		// ライフバーの高さ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// 生成処理
//=============================================================================
CLife* CLife::Create(void)
{
	CLife *pLife;

	pLife = new CLife;	// メモリを確保

	pLife->m_pPolygon[0] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 630.0f, 0.0f), BAR_WIDTH_LIFE, BAR_HEIGHT_LIFE);		// バーの長さ
	pLife->m_pPolygon[1] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 630.0f, 0.0f), BAR_WIDTH_LIFE, BAR_HEIGHT_LIFE);		// 現在のライフ
	pLife->m_pPolygon[2] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 630.0f, 0.0f), BAR_WIDTH_LIFE, BAR_HEIGHT_LIFE);		// 現在のライフと受けたダメージの差

	pLife->Init();

	return pLife;
}

//=========================================
// コンストラクタ
//=========================================
CLife::CLife(int nPriority) : CScene(nPriority)
{
	for (int nCntLife = 0; nCntLife < MAX_POLYGON_LIFE; nCntLife++)
	{// ゲージのポリゴン
		m_pPolygon[nCntLife] = NULL;
	}

	m_nLife = 0;			// ライフの値
	m_nCntSubtraction = 0;	// ダメージバーを減らし始めるまでのカウンタ
}

//=========================================
// デストラクタ
//=========================================
CLife::~CLife()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLife::Init(void)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_nLife = MAX_LIFE;

	m_pPolygon[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// 最大値を表すバー
	m_pPolygon[1]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));		// 後ろのバー
	m_pPolygon[2]->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));		// 前のバー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLife::Uninit(void)
{
	// テクスチャの開放
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_LIFE; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{
			m_pPolygon[nCntPolygon]->Uninit();
			delete m_pPolygon[nCntPolygon];
			m_pPolygon[nCntPolygon] = NULL;
		}
	}

	// 自分を破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLife::Update(void)
{
	float fLife = m_pPolygon[1]->GetWidth();		// ダメージバーの取得
	float fLifeGreen = m_pPolygon[2]->GetWidth();	// ダメージバーの取得

	if (fLife > fLifeGreen)
	{// ライフと同じ値になるまで後ろのゲージを減らす
		m_nCntSubtraction++;					// ダメージバーを減らし始めるためのカウント

		if (m_nCntSubtraction >= 25)
		{// 一定時間経過
			fLife -= BAR_WIDTH_LIFE * 0.01f;	// ダメージバーの長さを1％ずつ減らす

			m_pPolygon[1]->SetWidth(fLife);		// ダメージバーの長さを設定
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLife::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_LIFE; nCntPolygon++)
	{// ポリゴンの描画
		m_pPolygon[nCntPolygon]->Draw();
	}
}

//=============================================================================
// 体力の増減
//=============================================================================
const void CLife::AddSubtract(int nValue)
{
	m_nLife += nValue;		// ライフを増減させる
	float fLife;			// バーの長さの値

	if (m_nLife > 0)
	{// ライフが0より大きいとき
		fLife = (BAR_WIDTH_LIFE * 1.0f) / ((MAX_LIFE * 1.0f) / m_nLife);	// バーの元の長さを100%としてライフを増減させる
	}
	else
	{// ライフが0のとき
		fLife = 0;
	}

	if (m_nLife >= MAX_LIFE)
	{// 最大値を超えたら戻す
		m_nLife = MAX_LIFE;
		fLife = BAR_WIDTH_LIFE;
	}

	// バーの長さを更新
	m_pPolygon[2]->SetWidth(fLife);

	if (nValue > 0)
	{// 回復したとき後ろのバーも増やす
		m_pPolygon[1]->SetWidth(fLife);			// バーの長さを更新
	}
	m_nCntSubtraction = 0;						// ダメージバーを減らし始めるためのカウンタを初期化
}
