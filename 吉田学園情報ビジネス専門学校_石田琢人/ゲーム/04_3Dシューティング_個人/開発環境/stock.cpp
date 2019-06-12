//=============================================================================
//
// 残機処理 [stock.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "stock.h"
#include "polygon.h"
#include "number.h"
#include "scene3DModel.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STOCK_SIZE		(40)	// 残機の大きさ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CPolygon *CStock::m_pPolygon[MAX_STOCK] = {};
CNumber *CStock::m_pNumber				= NULL;
int		  CStock::m_nStock				= 0;

//=============================================================================
// 生成処理
//=============================================================================
CStock* CStock::Create(void)
{
	CStock *pStock;

	pStock = new CStock;	// メモリを確保

	// 残機のポリゴン
	pStock->m_pPolygon[0] = CPolygon::Create(0, D3DXVECTOR3(530.0f - (STOCK_SIZE * 2), 670.0f, 0.0f), STOCK_SIZE, STOCK_SIZE, CPolygon::TYPE_STOCK00);

	// 残機数
	pStock->m_pNumber = CNumber::Create(D3DXVECTOR3(530.0f, 670.0f, 0.0f), STOCK_SIZE);

	// 初期化処理
	pStock->Init();

	return pStock;
}

//=========================================
// コンストラクタ
//=========================================
CStock::CStock(int nPriority) : CScene(nPriority)
{
}

//=========================================
// デストラクタ
//=========================================
CStock::~CStock()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CStock::Init(void)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_nStock = NUM_STOCK;
	m_pNumber->SetNumber(NUM_STOCK);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CStock::Uninit(void)
{
	// ポリゴンクラスの終了処理
	for (int nCntPolygon = 0; nCntPolygon < MAX_STOCK; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{
			m_pPolygon[nCntPolygon]->Uninit();
			delete m_pPolygon[nCntPolygon];
			m_pPolygon[nCntPolygon] = NULL;
		}
	}

	// ナンバークラスの終了処理
	if (m_pNumber != NULL)
	{
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = NULL;
	}


	// ライフのポインタ変数を開放
	if (NULL != m_nStock)
	{
		m_nStock = NULL;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CStock::Update(void)
{
	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		CScene3DModel *pScene3DModel;		// オブジェクトのポインタ変数
		pScene3DModel = (CScene3DModel*)GetScene(PLAYER_PRIORITY, nCntScene);		// インデックス順に取得

		if (pScene3DModel != NULL)
		{// 使われているかのチェック
			CScene::OBJTYPE objType = pScene3DModel->GetObjType();;					// オブジェクトの種類を取得

			if (objType == CScene::OBJTYPE_PLAYER)
			{// オブジェクトの種類がプレイヤーのとき
				CPlayer *pPlayer;	// プレイヤーの情報を格納
				pPlayer = (CPlayer*)pScene3DModel->GetScene(PLAYER_PRIORITY, nCntScene);	// プレイヤーの情報を取得
				CPlayer::TYPE typePlayer = pPlayer->GetType();								// プレイヤーの形態を取得

				if (typePlayer == CPlayer::TYPE_AIRCRAFT)
				{// 戦闘機
					m_pPolygon[0]->TextureChange(CPolygon::TYPE_STOCK00);
				}
				else if (typePlayer == CPlayer::TYPE_SHIP)
				{// 戦艦
					m_pPolygon[0]->TextureChange(CPolygon::TYPE_STOCK01);
				}
				else if (typePlayer == CPlayer::TYPE_TANK)
				{// 戦車
					m_pPolygon[0]->TextureChange(CPolygon::TYPE_STOCK02);
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CStock::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_STOCK; nCntPolygon++)
	{// ポリゴンの描画
		m_pPolygon[nCntPolygon]->Draw();
	}

	m_pNumber->Draw();	// 数字の描画処理
}

//=============================================================================
// 残機の増減
//=============================================================================
const void CStock::AddSubtract(int nValue)
{
	if (m_nStock <= NUM_STOCK)
	{// ストックが最大数を超えないようにする
		m_nStock += nValue;		// ストックの増減
	}

	m_pNumber->SetNumber(m_nStock);		// 残機数の更新
}
