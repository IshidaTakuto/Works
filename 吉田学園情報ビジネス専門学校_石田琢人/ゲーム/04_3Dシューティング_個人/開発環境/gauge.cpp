//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "gauge.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BAR_WIDTH	(200)
#define BAR_HEIGHT	(30)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CPolygon *CGauge::m_pPolygon[MAX_POLYGON_GAUGE] = {};
int CGauge::m_nGauge = 0;

//=============================================================================
// 生成処理
//=============================================================================
CGauge* CGauge::Create(void)
{
	CGauge *pGauge;

	pGauge = new CGauge;	// メモリを確保

	pGauge->m_pPolygon[0] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 670.0f, 0.0f), BAR_WIDTH, BAR_HEIGHT);	// バーの長さ
	pGauge->m_pPolygon[1] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 670.0f, 0.0f), 0, BAR_HEIGHT);			// 現在のバーの長さ

	pGauge->Init();

	return pGauge;
}

//=========================================
// コンストラクタ
//=========================================
CGauge::CGauge(int nPriority) : CScene(nPriority)
{
}

//=========================================
// デストラクタ
//=========================================
CGauge::~CGauge()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGauge::Init(void)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_pPolygon[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// バーの色
	m_pPolygon[1]->SetColor(D3DXCOLOR(0.0f, 0.1f, 1.0f, 1.0f));		// バーの色

	m_nGauge = MAX_GAUGE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGauge::Uninit(void)
{
	// テクスチャの開放
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_GAUGE; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{
			m_pPolygon[nCntPolygon]->Uninit();
			delete m_pPolygon[nCntPolygon];
			m_pPolygon[nCntPolygon] = NULL;
		}
	}

	// 使い終わったものに0を入れる
	if (0 != m_nGauge)
	{
		m_nGauge = 0;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGauge::Update(void)
{
	float fLife = m_pPolygon[1]->GetWidth();	// ダメージバーの取得
	float fGaugeView = fLife * (1 / (BAR_WIDTH * 0.01f));	// ％にする

	if (m_nGauge < fGaugeView)
	{// ゲージが現在の値と同じになるまで減らす
		fLife -= BAR_WIDTH * 0.01f;			// ゲージの長さを1％ずつ減らす
		m_pPolygon[1]->SetWidth(fLife);		// ダメージバーの長さを設定
	}
	else if (m_nGauge > fGaugeView)
	{// ゲージが現在の値と同じになるまで増やす
		fLife += BAR_WIDTH * 0.01f;			// ゲージの長さを1％ずつ増やす
		m_pPolygon[1]->SetWidth(fLife);		// ダメージバーの長さを設定
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGauge::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_GAUGE; nCntPolygon++)
	{// ポリゴンの描画
		m_pPolygon[nCntPolygon]->Draw();
	}
}

//=============================================================================
// ゲージの増減
//=============================================================================
const void CGauge::AddSubtract(int nValue)
{
  	m_nGauge += nValue;		// ライフを増減させる

	if (m_nGauge >= MAX_GAUGE)
	{// 最大値を超えたら戻す
		m_nGauge = MAX_GAUGE;
	}
	else if(m_nGauge <= 0)
	{// 0より低くならないようにする
		m_nGauge = 0;
	}
}
