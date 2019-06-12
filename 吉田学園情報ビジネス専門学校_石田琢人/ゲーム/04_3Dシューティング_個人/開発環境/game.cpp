//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "camera.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "field.h"
#include "model.h"
#include "number.h"
#include "score.h"
#include "timer.h"
#include "polygon.h"
#include "effect.h"
#include "stock.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "pause.h"
#include "gauge.h"
#include "scene.h"
#include "camera.h"
#include "particle.h"
#include "meshCylinder.h"
#include "communicateWindow.h"
#include "shadow.h"
#include "sound.h"
#include "string.h"

#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMYINFO_FILENAME	"data/TEXT/enemy_info.txt"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CPlayer *CGame::m_pPlayer = NULL;
CField *CGame::m_pField[MAX_FIELD] = {};
bool CGame::m_bPause = false;
CGame::State CGame::m_state = CGame::STATE_NONE;
int CGame::m_stateCounter = 0;
CEnemyInfo CEnemyInfo::m_aEnemyInfo[MAX_ENEMY] = {};	// 敵の情報

//=========================================
// コンストラクタ
//=========================================
CGame::CGame()
{
	m_apPolygon = NULL;
	m_pause = NULL;
	m_nCntEnemyAppear = 0;
	m_pString = NULL;
}

//=========================================
// デストラクタ
//=========================================
CGame::~CGame()
{
}

//=============================================================================
// ゲーム初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	CBullet::Load();
	CPlayer::Load();
	CEnemy::Load();
	CEffect::Load();
	CField::Load();
	CModel::Load();
	CNumber::Load();
	CPolygon::Load();
	CParData::Load();
	CParticlePolygon::Load();
	CEnemyInfo::LoadEnemy();
	CCylinder::Load();
	CShadow::Load();
	CString::Load();

	// オブジェクトクラスの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0, 200, 0),CPlayer::TYPE_AIRCRAFT);

	CCylinder::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), 8000, 7000, 8000, 20, 1, 0, 0);

	m_pField[0] = CField::Create(CField::TYPE_GROUND);
	m_pField[1] = CField::Create(CField::TYPE_SEA);
	m_pField[2] = CField::Create(CField::TYPE_GROUND);

	m_apPolygon = CPolygon::Create(1, D3DXVECTOR3(0.0f, 620.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT - 620);
	m_apPolygon->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f));

	m_pString = CString::Create();

	CScore::Create(D3DXVECTOR3(800, 670, 0));
	CTimer::Create(D3DXVECTOR3(1200, 670, 0));
	CStock::Create();
	CGauge::Create();

	m_bPause = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// テクスチャの破棄
	CBullet::Unload();
	CPlayer::Unload();
	CEnemy::Unload();
	CField::Unload();
	CModel::Unload();
	CNumber::Unload();
	CPolygon::Unload();
	CParData::Unload();
	CParticlePolygon::Unload();
	CCylinder::Unload();
	CShadow::Unload();
	CString::Unload();

	// プレイヤークラスの破棄
	if (NULL != m_pPlayer)
	{
		// 終了処理
		m_pPlayer = NULL;
	}

	// ポーズクラスの破棄
	if (NULL != m_pause)
	{
		// 終了処理
		m_pause->Uninit();
		delete m_pause;
		m_pause = NULL;
	}

	// ポーズクラスの破棄
	if (NULL != m_apPolygon)
	{
		// 終了処理
		m_apPolygon->Uninit();
		delete m_apPolygon;
		m_apPolygon = NULL;
	}

	if (m_pString != NULL)
	{// 文字の破棄
		m_pString->Uninit();
		delete m_pString;
		m_pString = NULL;
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
// ゲーム更新処理
//=============================================================================
void CGame::Update(void)
{
	// フィールドの自動生成
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (NULL != m_pField[nCntField])
		{
			CCamera *pCamera = CManager::GetCamera();				// カメラの情報を取得
			D3DXVECTOR3 posCamera = pCamera->GetPosV();				// カメラの位置を取得

			D3DXVECTOR3 pos = m_pField[nCntField]->GetPos();		// フィールドの位置を取得
			D3DXVECTOR3 length = m_pField[nCntField]->GetLength();	// フィールドの長さを取得

			if (pos.x + length.x < posCamera.x - 5000)
			{// フィールドの生成
				m_pField[nCntField]->Uninit();
				m_pField[nCntField] = NULL;
			}
		}
		else
		{
			m_pField[nCntField] = CField::Create(CField::TYPE_GROUND);
		}
	}

	// 敵の自動生成
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		CEnemyInfo *pEnemyInfo = CEnemyInfo::GetInfo(nCntEnemy);
		int nNowTime = CTimer::GetTime();				// 現在の時間

		if (pEnemyInfo != NULL)
		{// 空でないかどうかの確認
			bool bUse = pEnemyInfo->GetUse();

			if (!bUse)
			{// まだ使用されていないデータ
				int nAppearTime = pEnemyInfo->GetAppearTime();	// 敵の出現する時間

				if (nNowTime == nAppearTime)
				{// 敵を生成
					pEnemyInfo->SetUse(true);
					CEnemy::Create(pEnemyInfo->GetPos(), pEnemyInfo->GeType());
				}
			}
		}
	}

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// ジョイパッドの取得
	CSound *pSound = CManager::GetSound();

	CFade::FADE fade = CFade::GetFade();

	if (fade == CFade::FADE_NONE)
	{// フェード中はポーズにできなくする
		if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_7) || pKeyboard->GetTrigger(DIK_P))
		{// ポーズON/OFF
			m_bPause = m_bPause ? false : true;

			if (true == m_bPause)
			{// ポーズになったとき
				if (m_pause == NULL)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_PAUSE);
					m_pause = CPause::Create();	// ポーズクラスを生成
				}
			}
		}
	}

	if (m_bPause == false)
	{// ポーズでないとき
		if (m_pause != NULL)
		{// ポーズでなくなったら破棄
			pSound->PlaySound(CSound::SOUND_LABEL_CANCEL);
			m_pause->Uninit();
			delete m_pause;
			m_pause = NULL;
		}

		CCamera *pCamera = CManager::GetCamera();				// カメラの情報を取得
		D3DXVECTOR3 posCamera = pCamera->GetPosV();				// カメラの位置を取得

		// 敵を出現させる
		if (posCamera.x >= m_nCntEnemyAppear * 600)
		{// 敵の生成
			m_nCntEnemyAppear++;
			CEnemy::Create(D3DXVECTOR3(600.0f * m_nCntEnemyAppear, 200.0f, (400 - (rand() % 800)) * 1.0f), CEnemy::TYPE_AIRCRAFT);

			for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
			{// フィールドを調べる
				if (m_pField[nCntField] != NULL)
				{
					if (m_pField[nCntField]->Collision(&D3DXVECTOR3(m_nCntEnemyAppear * 600.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_SEA == m_pField[nCntField]->GetType())
					{// 戦艦
						CEnemy::Create(D3DXVECTOR3(800.0f * m_nCntEnemyAppear + 200.0f, 0.0f, -400.0f + (rand() % 800)), CEnemy::TYPE_SHIP);
					}
					else if (m_pField[nCntField]->Collision(&D3DXVECTOR3(m_nCntEnemyAppear * 600.0f, 0.0f, 0.0f), 0.0f) && CField::TYPE_GROUND == m_pField[nCntField]->GetType())
					{// 戦車
						CEnemy::Create(D3DXVECTOR3(800.0f * m_nCntEnemyAppear + 200.0f, 0.0f, -400.0f + (rand() % 800)), CEnemy::TYPE_TANK);
					}
				}
			}
		}

		// レンダラークラスの更新
		CRenderer *pRenderer = CManager::GetRenderer();
		pRenderer->Update();
	}
	else
	{// ポーズ中
		m_pause->Update();
	}

	switch (m_state)
	{
	case STATE_NORMAL:
		break;

	case STATE_END:
		m_stateCounter++;
		if (m_stateCounter >= 60)
		{
			m_state = STATE_NONE;

			// 画面(モード)の設定
			CFade::Create(CManager::MODE_GAMEOVER);
		}
		break;

	case STATE_CLEAR:
		m_stateCounter++;
		if (m_stateCounter >= 60)
		{
			m_state = STATE_NONE;

			// 画面(モード)の設定
			CFade::Create(CManager::MODE_RESULT);
		}
		break;
	}
}

//=============================================================================
// ゲーム描画処理
//=============================================================================
void CGame::Draw(void)
{
	if (m_apPolygon != NULL)
	{// ポリゴンの描画
		m_apPolygon->Draw();
	}

	if (m_pString != NULL)
	{// 文字の描画
		m_pString->Set("[Ｘ]：攻撃\n[Ｙ]：特殊弾\n[←]：戦闘機\n[↓]：戦車\n[→]：戦艦", RECT{ 10,10,0,0 }, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), -1);
	}

	if (NULL != m_pause)
	{// ポーズ画面の描画
		m_pause->Draw();
	}
}

//=============================================================================
// ゲームの状態の設定
//=============================================================================
const void CGame::SetGameState(State state)
{
	m_state = state;
	m_stateCounter = 0;
}

//=========================================
// プレイヤーにヌルを入れる
//=========================================
void CGame::NullPlayer(void)
{
	if (NULL != m_pPlayer)
	{
		m_pPlayer = NULL;
	}
}

//=========================================
// プレイヤーの設定
//=========================================
const void CGame::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

//=========================================
// コンストラクタ
//=========================================
CEnemyInfo::CEnemyInfo()
{
	m_nAppearTime = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = CEnemy::TYPE_AIRCRAFT;
	m_bUse = false;
}

//=========================================
// デストラクタ
//=========================================
CEnemyInfo::~CEnemyInfo()
{
}

//=========================================
// 敵の情報を読み込む
//=========================================
HRESULT CEnemyInfo::LoadEnemy(void)
{
	FILE *pFile;
	int nNumEnemy = 0;

	pFile = fopen(ENEMYINFO_FILENAME, "r");

	if (NULL != pFile)
	{
		while (1)
		{
			if (nNumEnemy >= MAX_ENEMY)
			{// 敵の最大数
				break;
			}
			else
			{
				char aStr[256];				// 文字列を格納
				fgets(aStr, 256, pFile);	// 文字列の取得

				if (strcmp(aStr, "SETENEMY\n") == 0)
				{// 敵の情報を読み取る
					int nType;	// 敵の種類

					fscanf(pFile, "%f %f %f", &m_aEnemyInfo[nNumEnemy].m_pos.x, &m_aEnemyInfo[nNumEnemy].m_pos.y, &m_aEnemyInfo[nNumEnemy].m_pos.z);
					fscanf(pFile, "%d", &m_aEnemyInfo[nNumEnemy].m_nAppearTime);
					fscanf(pFile, "%d", &nType);

					if (nType == 0)
					{// 戦闘機型
						m_aEnemyInfo[nNumEnemy].m_type = CEnemy::TYPE_AIRCRAFT;
					}
					else if (nType == 1)
					{// 戦艦型
						m_aEnemyInfo[nNumEnemy].m_type = CEnemy::TYPE_SHIP;
					}
					else if (nType == 2)
					{// 戦車型
						m_aEnemyInfo[nNumEnemy].m_type = CEnemy::TYPE_TANK;
					}

					m_aEnemyInfo[nNumEnemy].m_bUse = false;
					nNumEnemy++;
				}
				else if (strcmp(aStr, "END") == 0)
				{
					break;
				}
			}
		}

		fclose(pFile);		// ファイルを閉じる
	}
	else
	{// 失敗
		MessageBox(0, "ファイルが開けませんでした", "particle.txt", MB_OK);
	}

	return S_OK;
}

