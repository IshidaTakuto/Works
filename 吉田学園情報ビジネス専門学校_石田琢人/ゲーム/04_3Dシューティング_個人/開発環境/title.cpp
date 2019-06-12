//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "title.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "polygon.h"
#include "meshCylinder.h"
#include "camera.h"
#include "enemy.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TIME	(1200)		// ランキング画面までの時間

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CPolygon *CTitle::m_apPolygon[TITLE_POLYGON] = {};	// ポリゴンクラスのポインタ変数
CField *CTitle::m_pField[MAX_FIELD] = {};

//=========================================
// コンストラクタ
//=========================================
CTitle::CTitle()
{
	m_nCntEnemyAppear = 0;
	m_nCntTitle = 0;
	m_nCntEnemyAppear = 0;
}

//=========================================
// デストラクタ
//=========================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	CPolygon::Load();	// ポリゴンのテクスチャを取得
	CCylinder::Load();
	CField::Load();
	CEnemy::Load();
	CShadow::Load();

	CCylinder::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), 8000, 7000, 8000, 20, 1, 0, 0);

	m_pField[0] = CField::Create(CField::TYPE_GROUND);
	m_pField[1] = CField::Create(CField::TYPE_GROUND);
	m_pField[2] = CField::Create(CField::TYPE_SEA);

	m_apPolygon[0] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0),300,150, CPolygon::TYPE_TITLE_LOGO);
	m_apPolygon[1] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 600, 0), 250, 50, CPolygon::TYPE_PRESS_ANY);

	// 透明にする
	m_apPolygon[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apPolygon[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	m_nCntEnemyAppear = 1;

	CCamera *pCamera = CManager::GetCamera();			// カメラの情報を取得
	pCamera->SetPosV(D3DXVECTOR3(700, 0.0f, 0.0f));		// カメラの位置の設定

	// 敵を生成
	CEnemy::Create(D3DXVECTOR3(1450.0f, 0.0f, -50.0f), CEnemy::TYPE_TANK);
	CEnemy::Create(D3DXVECTOR3(1500.0f, 0.0f, 50.0f), CEnemy::TYPE_TANK);
	CEnemy::Create(D3DXVECTOR3(1400.0f, 0.0f, 100.0f), CEnemy::TYPE_TANK);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	CPolygon::Unload();	// ポリゴンのテクスチャを破棄
	CCylinder::Unload();
	CField::Unload();
	CEnemy::Unload();
	CShadow::Unload();

	// フィールドクラスの破棄
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (NULL != m_pField[nCntField])
		{
			m_pField[nCntField] = NULL;
		}
	}

	// ポリゴンクラスの破棄
	for (int nCntPolygon = 0; nCntPolygon < TITLE_POLYGON; nCntPolygon++)
	{
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			m_apPolygon[nCntPolygon]->Uninit();
			delete m_apPolygon[nCntPolygon];
			m_apPolygon[nCntPolygon] = NULL;
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
void CTitle::Update(void)
{
	// タイトルの経過時間
	m_nCntTitle++;

	CCamera *pCamera = CManager::GetCamera();				// カメラの情報を取得
	D3DXVECTOR3 posCamera = pCamera->GetPosV();				// カメラの位置を取得
	D3DXVECTOR3 posRCamera = pCamera->GetPosR();			// カメラの注視点

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
		{
			// フィールドをランダム生成
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

	// 入力の情報を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	D3DXCOLOR col = m_apPolygon[0]->GetColor();			// タイトルロゴの色を取得

	// カメラの注視点が一定までいくとロゴを出す
	if (posRCamera.y >= posCamera.y)
	{
		if (col.a >= 1.0f)
		{// 透明度が一定以上
			col.a = 1.0f;
		}
		else if (col.a < 1.0f)
		{// 透明度を加算
			col.a += 0.02f;
		}
	}

	if (col.a >= 1.0f)
	{// 透明でない状態
		// 入力待ちテクスチャを点滅
		m_apPolygon[1]->Flashing(60);

		if (pKeyboard->GetAnyKey() || pJoypad->GetAnyTrigger(0))
		{// 画面遷移
			CFade::Create(CManager::MODE_TUTORIAL);
		}
		else if (m_nCntTitle >= TITLE_TIME)
		{// 一定時間経過
			CFade::Create(CManager::MODE_RANKING);
		}
	}
	if (pKeyboard->GetAnyKey() || pJoypad->GetAnyTrigger(0))
	{// ボタンを押すとすぐに表示される
		col.a = 1.0f;
	}

	// 色の設定
	m_apPolygon[0]->SetColor(col);

	// 敵を出現させる
	if (posCamera.x >= m_nCntEnemyAppear * 50)
	{// 敵の生成
		if (CEnemy::GetNumEnemy() < 40)
		{// 一定数を保つ
			m_nCntEnemyAppear++;
			CEnemy::Create(D3DXVECTOR3(rand() % 400 + (300.0f * m_nCntEnemyAppear), rand() % 400 + 100.0f, -1000.0f + (rand() % 2000)), CEnemy::TYPE_AIRCRAFT);

			for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
			{// フィールドを調べる
				if (m_pField[nCntField] != NULL)
				{
					if (m_pField[nCntField]->Collision(&D3DXVECTOR3(posCamera.x + 3000.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_SEA == m_pField[nCntField]->GetType())
					{// 戦艦
						CEnemy::Create(D3DXVECTOR3(rand() % 1000 + (2500.0f + posCamera.x), 0.0f, -1000.0f + (rand() % 2000)), CEnemy::TYPE_SHIP);
					}
					else if (m_pField[nCntField]->Collision(&D3DXVECTOR3(posCamera.x + 3000.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_GROUND == m_pField[nCntField]->GetType())
					{// 戦車
						CEnemy::Create(D3DXVECTOR3(rand() % 1000 + (2500.0f + posCamera.x), 0.0f, -2000.0f + (rand() % 4000)), CEnemy::TYPE_TANK);
					}
				}
			}
		}
	}

	CRenderer *pRenderer = CManager::GetRenderer();
	pRenderer->Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < TITLE_POLYGON; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon]->Draw();
	}
}
