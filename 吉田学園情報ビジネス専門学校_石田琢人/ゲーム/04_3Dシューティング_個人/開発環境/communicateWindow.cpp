//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "communicateWindow.h"
#include "polygon.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COMMUNICATEWINDOW_TEXTURENAME	"data/TEXTURE/communicate_window.jpg"
#define COMMUNICATEWINDOW_WIDTH			(600)
#define COMMUNICATEWINDOW_HEIGHT		(60)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CPolygon *CCommunicateWindow::m_apPolygon[MAX_POLYGON_COMMUNICATE] = {};

//=============================================================================
// 生成処理
//=============================================================================
CCommunicateWindow* CCommunicateWindow::Create(void)
{
	CCommunicateWindow *pCommunicateWindow;

	pCommunicateWindow = new CCommunicateWindow;	// メモリを確保

	pCommunicateWindow->m_apPolygon[0] = CPolygon::Create(0, D3DXVECTOR3(COMMUNICATEWINDOW_WIDTH + 10.0f, COMMUNICATEWINDOW_HEIGHT + 10.0f, 0.0f), 0, 0, CPolygon::TYPE_COMMUNICATEWINDOW);	// 通信ウィンドウ
	pCommunicateWindow->m_apPolygon[1] = CPolygon::Create(0, D3DXVECTOR3(COMMUNICATEWINDOW_WIDTH + 10.0f, COMMUNICATEWINDOW_HEIGHT + 10.0f, 0.0f), 0, 0);	// 黒いポリゴン

	pCommunicateWindow->Init();		// 初期化処理

	return pCommunicateWindow;
}

//=========================================
// コンストラクタ
//=========================================
CCommunicateWindow::CCommunicateWindow()
{
	m_nCntTransparent = 0;
	m_bCompleted = false;
}

//=========================================
// デストラクタ
//=========================================
CCommunicateWindow::~CCommunicateWindow()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCommunicateWindow::Init(void)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_apPolygon[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// 通信ウィンドウ
	m_apPolygon[1]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// 黒いポリゴン
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCommunicateWindow::Uninit(void)
{
	// ポリゴンクラスの開放
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_COMMUNICATE; nCntPolygon++)
	{
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			m_apPolygon[nCntPolygon]->Uninit();
			delete m_apPolygon[nCntPolygon];
			m_apPolygon[nCntPolygon] = NULL;
		}
	}

	Release();	// 開放
}

//=============================================================================
// 更新処理
//=============================================================================
void CCommunicateWindow::Update(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_COMMUNICATE; nCntPolygon++)
	{
		float fWidth = m_apPolygon[nCntPolygon]->GetWidth();		// 幅を取得
		float fHeight = m_apPolygon[nCntPolygon]->GetHeight();		// 高さを取得

		if (fWidth < COMMUNICATEWINDOW_WIDTH)
		{// 幅の値を大きくする
			fWidth += (COMMUNICATEWINDOW_WIDTH * 0.01f) * 4;			// 幅の値を大きくする
		}
		if (fHeight < COMMUNICATEWINDOW_HEIGHT)
		{// 高さの値を大きくする
			fHeight += (COMMUNICATEWINDOW_HEIGHT * 0.01f) * 4;			// 高さの値を大きくする
		}

		// 大きさを更新する
		m_apPolygon[nCntPolygon]->SetWidth(fWidth);
		m_apPolygon[nCntPolygon]->SetHeight(fHeight);

		if (fWidth >= COMMUNICATEWINDOW_WIDTH && fHeight >= COMMUNICATEWINDOW_HEIGHT)
		{// 大きさが規定値に達したとき
			D3DXCOLOR col = m_apPolygon[1]->GetColor();		// 色の取得
			col.a -= 0.02f;		// アルファ値を減らす

			if (col.a > 0)
			{// 透明度が0より大きいとき
				m_apPolygon[1]->SetColor(col);	// 前面のポリゴンを透明化させる
			}
			else
			{
				m_bCompleted = true;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CCommunicateWindow::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_COMMUNICATE; nCntPolygon++)
	{// ポリゴンの描画
		m_apPolygon[nCntPolygon]->Draw();
	}
}
