//=============================================================================
//
// 変形可能成否処理 [formTime.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "formTime.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BAR_WIDTH_FORMTIME	(200)
#define BAR_HEIGHT_FORMTIME	(20)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// 生成処理
//=============================================================================
CFormTime* CFormTime::Create(void)
{
	// メモリを確保
	CFormTime *pFormTime = new CFormTime;;

	// バー全体の長さ
	pFormTime->m_pPolygon[0] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 700.0f, 0.0f), BAR_WIDTH_FORMTIME, BAR_HEIGHT_FORMTIME);

	// 値を示すバーの長さ
	pFormTime->m_pPolygon[1] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 700.0f, 0.0f), 0, BAR_HEIGHT_FORMTIME);

	// 初期化処理
	pFormTime->Init();

	return pFormTime;
}

//=========================================
// コンストラクタ
//=========================================
CFormTime::CFormTime(int nPriority) : CScene(nPriority)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_GAUGE_FORMTIME; nCntPolygon++)
	{// ゲージのポリゴン
		m_pPolygon[nCntPolygon] = NULL;
	}

	m_nFormTime = 0;		// 変形可能ゲージの値
	m_CntFormTime = 0;		// ゲージが溜まり始めるまでのカウンタ
	m_bSuccess = false;		// 変形できるかどうかのフラグ
}

//=========================================
// デストラクタ
//=========================================
CFormTime::~CFormTime()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFormTime::Init(void)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_pPolygon[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// バー全体の色
	m_pPolygon[1]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f));		// 値を示すバーの色
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFormTime::Uninit(void)
{
	// テクスチャの開放
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_GAUGE_FORMTIME; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{// 使われていたら
			m_pPolygon[nCntPolygon]->Uninit();
			delete m_pPolygon[nCntPolygon];
			m_pPolygon[nCntPolygon] = NULL;
		}
	}

	// 自分の破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFormTime::Update(void)
{
	// 変形可能ゲージが溜まり始めるまでのカウント
	m_CntFormTime++;

	if (m_CntFormTime > 60)
	{
		if (m_nFormTime < MAX_GAUGE_FORMTIME)
		{// 一定の値まで増やす
			m_nFormTime++;
		}
	}

	// ゲージが現在の値と同じになるまで増減
	float fLife = (BAR_WIDTH_FORMTIME * 1.0f) / ((MAX_GAUGE_FORMTIME * 1.0f) / m_nFormTime);
	m_pPolygon[1]->SetWidth(fLife);				// バーの長さを設定

	if (fLife >= BAR_WIDTH_FORMTIME)
	{// ゲージが最大値に達したとき
		m_bSuccess = true;
		m_pPolygon[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));		// バーの色
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFormTime::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_GAUGE_FORMTIME; nCntPolygon++)
	{// ポリゴンの描画
		m_pPolygon[nCntPolygon]->Draw();
	}
}

//=============================================================================
// 変形した後の数値リセット
//=============================================================================
void CFormTime::FormTimeReset(void)
{
	m_bSuccess = false;
	m_CntFormTime = 0;
	m_nFormTime = 0;
	m_pPolygon[1]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f));		// バーの色
}