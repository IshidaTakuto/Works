//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "tutorial.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "field.h"
#include "model.h"
#include "polygon.h"
#include "effect.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "string.h"
#include "gauge.h"
#include "stock.h"
#include "camera.h"
#include "number.h"
#include "particle.h"
#include "meshCylinder.h"
#include "communicateWindow.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_STRING	(6)			// 文字列の数
#define TIME_STRING	(6 * 60)	// 表示する時間
#define TUTORIAL_ENEMY_APPEAR	(6 * 60)	// 敵の出現間隔
#define TUTORIAL_FIELD_APPEAR	(9 * 60)	// 敵の出現間隔

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CField *CTutorial::m_pField[MAX_FIELD] = {};

//=========================================
// コンストラクタ
//=========================================
CTutorial::CTutorial()
{
	m_nCntString = 0;
	m_nCntEnemyAppear = 0;
	m_nCntTutorialTime = 0;
	m_pCommunicateWindow = NULL;
	m_pString[0] = NULL;
	m_pString[1] = NULL;
}

//=========================================
// デストラクタ
//=========================================
CTutorial::~CTutorial()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{
	CBullet::Load();
	CEnemy::Load();
	CPlayer::Load();
	CEffect::Load();
	CField::Load();
	CModel::Load();
	CPolygon::Load();
	CString::Load();
	CNumber::Load();
	CParData::Load();
	CParticlePolygon::Load();
	CCylinder::Load();
	CShadow::Load();

	m_pString[0] = CString::Create();
	m_pString[1] = CString::Create();

	CCylinder::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), 8000, 7000, 8000, 20, 1, 0, 0);

	m_pField[0] = CField::Create(CField::TYPE_GROUND);
	m_pField[1] = CField::Create(CField::TYPE_SEA);
	m_pField[2] = CField::Create(CField::TYPE_GROUND);

	// オブジェクトクラスの生成
	CPlayer::Create(D3DXVECTOR3(0, 200, 0), CPlayer::TYPE_AIRCRAFT);

	CStock::Create();
	CGauge::Create();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// テクスチャの破棄
	CBullet::Unload();
	CPlayer::Unload();
	CEnemy::Unload();
	CField::Unload();
	CModel::Unload();
	CPolygon::Unload();
	CString::Unload();
	CNumber::Unload();
	CParData::Unload();
	CParticlePolygon::Unload();
	CCylinder::Unload();
	CShadow::Unload();

	if (NULL != m_pCommunicateWindow)
	{// 文字の表示枠の破棄
		m_pCommunicateWindow = NULL;
	}

	for (int nCntString = 0; nCntString < 2; nCntString++)
	{
		if (m_pString[nCntString] != NULL)
		{// 文字の破棄
			m_pString[nCntString]->Uninit();
			delete m_pString[nCntString];
			m_pString[nCntString] = NULL;
		}
	}

	// フィールドクラスの破棄
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
void CTutorial::Update(void)
{
	m_nCntTutorialTime++;

	CCamera *pCamera = CManager::GetCamera();				// カメラの情報を取得
	D3DXVECTOR3 posCamera = pCamera->GetPosV();				// カメラの位置を取得

	// フィールドの自動生成
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (NULL != m_pField[nCntField])
		{
			D3DXVECTOR3 pos = m_pField[nCntField]->GetPos();		// フィールドの位置を取得
			D3DXVECTOR3 length = m_pField[nCntField]->GetLength();	// フィールドの長さを取得

			if (pos.x + length.x < posCamera.x - 5000)
			{// フィールドの開放
				m_pField[nCntField]->Uninit();
				m_pField[nCntField] = NULL;
			}
		}
		else
		{// フィールドを生成する
			if (rand() % 2 == 0)
			{//	地面
				m_pField[nCntField] = CField::Create(CField::TYPE_GROUND);

				D3DXVECTOR3 posField = m_pField[nCntField]->GetPos();		// フィールドの位置の取得
				D3DXVECTOR3 length = m_pField[nCntField]->GetLength();		// フィールドの長さを取得
				CEnemy::Create(D3DXVECTOR3(posField.x, 0.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_TANK);
				CEnemy::Create(D3DXVECTOR3(posField.x + (length.x / 2), 0.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_TANK);
			}
			else
			{// 海
				m_pField[nCntField] = CField::Create(CField::TYPE_SEA);

				D3DXVECTOR3 posField = m_pField[nCntField]->GetPos();		// フィールドの位置の取得
				D3DXVECTOR3 length = m_pField[nCntField]->GetLength();		// フィールドの長さを取得
				CEnemy::Create(D3DXVECTOR3(posField.x, 0.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_TANK);
				CEnemy::Create(D3DXVECTOR3(posField.x + (length.x / 2), 0.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_TANK);
			}
		}
	}

	// 敵を出現させる
	if (posCamera.x >= m_nCntEnemyAppear * 800)
	{// 敵の生成
		m_nCntEnemyAppear++;
		CEnemy::Create(D3DXVECTOR3(800.0f * m_nCntEnemyAppear, 200.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_AIRCRAFT);

		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{// フィールドを調べる
			if (m_pField[nCntField] != NULL)
			{
				if (m_pField[nCntField]->Collision(&D3DXVECTOR3(m_nCntEnemyAppear * 800.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_SEA == m_pField[nCntField]->GetType())
				{// 戦艦
					CEnemy::Create(D3DXVECTOR3(800.0f * m_nCntEnemyAppear + 200.0f, 0.0f, -400.0f + (rand() % 800)), CEnemy::TYPE_SHIP);
				}
				else if (m_pField[nCntField]->Collision(&D3DXVECTOR3(m_nCntEnemyAppear * 800.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_GROUND == m_pField[nCntField]->GetType())
				{// 戦車
					CEnemy::Create(D3DXVECTOR3(800.0f * m_nCntEnemyAppear + 200.0f, 0.0f, -400.0f + (rand() % 800)), CEnemy::TYPE_TANK);
				}
			}
		}
	}


	// 通信ウィンドウを出す
	if (m_nCntTutorialTime == 60)
	{
		if (NULL == m_pCommunicateWindow)
		{
			m_pCommunicateWindow = CCommunicateWindow::Create();
		}
	}

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// ジョイパッドの取得

	if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_7) || pKeyboard->GetTrigger(DIK_P))
	{// 画面遷移
		CFade::Create(CManager::MODE_GAME);
	}

	// オブジェクトの更新
	CRenderer *pRenderer = CManager::GetRenderer();
	pRenderer->Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{
	if (NULL != m_pCommunicateWindow)
	{
		if (m_pCommunicateWindow->GetCompleted())
		{// 通信ウィンドウが準備できた
			bool bComp = false;
			CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
			CInputJoypad *pJoypad = CManager::GetInputJoypad();			// ジョイパッドの取得

			m_pString[0]->Set("[ＳＴＡＲＴ]でチュートリアルをスキップ\n[Ｂボタン]次へ", RECT{ 940,70,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1,1);

			if (m_nCntString == 0)
			{// 開始メッセージ
				bComp = m_pString[1]->Set("操作方法の説明をします。", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 1)
			{// 移動方法の文字表示
				bComp = m_pString[1]->Set("［左スティック］で移動ができます。", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 2)
			{// 攻撃方法の文字表示
				bComp = m_pString[1]->Set("［Ｘボタン］で攻撃ができます。", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 3)
			{// 地形の文字表示
				bComp = m_pString[1]->Set("地上で戦艦に変形または \n水上で戦車になるとダメージを受け続けます。", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 4)
			{// 変形方法の文字表示
				bComp = m_pString[1]->Set("十字キーで変形ができます。ですが、再度変形できるようになるまで数秒かかります。 \n←：戦闘機　↓：戦車　→：戦艦", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 5)
			{// 特殊攻撃方法の文字表示
				bComp = m_pString[1]->Set("［Ｙボタン］でそれぞれのフォームの強力な攻撃ができます。 \nただし、ゲージが溜まっている状態でないと使えません。", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5);
			}
			else if (m_nCntString == 6)
			{
				bComp = m_pString[1]->Set("［ＳＴＡＲＴ］でチュートリアルを終了します。 \n［Ａボタン］でもう一度最初から説明します。", RECT{ 130,30,0,0 }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 3);

				if (bComp)
				{// 文章が全部表示されている
					if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_0) || pKeyboard->GetTrigger(DIK_RETURN))
					{// 文章を最初に戻す
						m_nCntString = 0;
					}
				}
			}

			if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_1) || pKeyboard->GetTrigger(DIK_RETURN))
			{// 文章を送る
				if (m_nCntString < 6 && bComp)
				{
					m_nCntString++;
				}
			}
		}
	}
}
