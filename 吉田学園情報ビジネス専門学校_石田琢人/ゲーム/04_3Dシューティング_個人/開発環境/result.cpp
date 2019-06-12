//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "result.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "polygon.h"
#include "number.h"
#include "ranking.h"
#include "meshCylinder.h"
#include "scene3DModel.h"
#include "particle.h"
#include "field.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_TIME			(420)			// タイトル画面までの時間
#define RESULT_NUMBER_SIZE	(40.0f)			// 数字の大きさ

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CResult::m_nScore = 0;		// スコア
int CResult::m_nNumKill = 0;	// 倒した敵の数

//=========================================
// コンストラクタ
//=========================================
CResult::CResult()
{
	// ナンバーを初期化
	for (int nCntNumber = 0; nCntNumber < RESULT_SCORE_DIGITS; nCntNumber++)
	{// スコア
		m_apNumScore[nCntNumber] = NULL;
	}
	for (int nCntNumber = 0; nCntNumber < RESULT_KILL_DIGITS; nCntNumber++)
	{// 倒した敵の数
		m_apNumKill[nCntNumber] = NULL;
	}

	// ポリゴンを初期化
	for (int nCntPolygon = 0; nCntPolygon < RESULT_POLYGON; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon] = NULL;
	}

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{// フィールドを初期化
		m_pField[nCntField] = NULL;
	}

	m_model = NULL;
	m_nCntResult = 0;
	m_nScoreDigit = 0;
	m_nKillDigit = 0;
}

//=========================================
// デストラクタ
//=========================================
CResult::~CResult()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	// テクスチャを取得
	CPolygon::Load();
	CNumber::Load();
	CParData::Load();
	CParticlePolygon::Load();
	CField::Load();
	CCylinder::Load();

	// ロゴ
	m_apPolygon[0] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 150, 0), 400, 100);
	m_apPolygon[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 入力待ち
	m_apPolygon[1] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 100, 0), 250, 50, CPolygon::TYPE_PRESS_ANY);

	m_nScoreDigit = (int)log10f((float)m_nScore) + 1;		// スコアの桁数
	if (m_nScoreDigit == 0) { m_nScoreDigit = 1; }			// 0だったとき
	for (int nCntDigit = 0; nCntDigit < m_nScoreDigit; nCntDigit++)
	{// スコア
		m_apNumScore[nCntDigit] = CNumber::Create(D3DXVECTOR3((700 + (RESULT_NUMBER_SIZE * m_nScoreDigit) - RESULT_NUMBER_SIZE) - (nCntDigit * RESULT_NUMBER_SIZE * 2), 400, 0.0f), RESULT_NUMBER_SIZE);
	}

	m_nKillDigit = (int)log10f((float)m_nNumKill) + 1;		// 倒した敵の数の桁数
	if (m_nKillDigit == 0) { m_nKillDigit = 1; }			// 0だったとき
	for (int nCntDigit = 0; nCntDigit < m_nKillDigit; nCntDigit++)
	{// 倒した敵の数
		m_apNumKill[nCntDigit] = CNumber::Create(D3DXVECTOR3((700 + (RESULT_NUMBER_SIZE * m_nKillDigit) - RESULT_NUMBER_SIZE) - (nCntDigit * RESULT_NUMBER_SIZE * 2), 500, 0.0f), RESULT_NUMBER_SIZE);
	}

	// シリンダーの生成
	CCylinder::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), 8000, 7000, 8000, 20, 1, 0, 0);

	// フィールドの生成
	CField::Create(CField::TYPE_GROUND);

	// モデルを出す
	m_model = new CScene3DModel;

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	LPDIRECT3DTEXTURE9	pTexture;		// 共有テクスチャへのポインタ
	LPD3DXMESH			pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER		pBuffMat;		// マテリアル情報へのポインタ
	DWORD				nNumMat;		// マテリアル情報の数

										// Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/aircraft.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pBuffMat,
		NULL,
		&nNumMat,
		&pMesh);

	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// テクスチャの生成
	pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &pTexture);

	// モデル情報を渡す
	m_model->BindInfo(pTexture, pMesh, pBuffMat, nNumMat);

	// 現在のモードを取得
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_RESULT)
	{// ゲームクリア
		m_apPolygon[0]->TextureChange(CPolygon::TYPE_RESULT_LOGO);

		// モデルの位置と角度
		m_model->SetPos(D3DXVECTOR3(0.0f, 500.0f, 0.0f));
		m_model->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		CCamera *pCamera = CManager::GetCamera();			// カメラの情報を取得
		pCamera->SetPosV(D3DXVECTOR3(0.0f, 500.0f, -100.0f));		// カメラの位置の設定
	}
	else if (mode == CManager::MODE_GAMEOVER)
	{// ゲームオーバー
		m_apPolygon[0]->TextureChange(CPolygon::TYPE_GAMEOVER_LOGO);

		// モデルの位置と角度
		m_model->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_model->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.25f, D3DX_PI * 0.25f));

		// フィールドの生成
		m_pField[0]->Create(CField::TYPE_GROUND);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	CPolygon::Unload();	// ポリゴンのテクスチャを破棄
	CNumber::Unload();	// ナンバーのテクスチャを破棄
	CParData::Unload();
	CParticlePolygon::Unload();
	CField::Unload();
	CCylinder::Unload();

	// 静的変数を初期値にする
	m_nNumKill = 0;
	m_nScore = 0;

	// ポリゴンクラスの破棄
	for (int nCntPolygon = 0; nCntPolygon < RESULT_POLYGON; nCntPolygon++)
	{
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			m_apPolygon[nCntPolygon]->Uninit();
			delete m_apPolygon[nCntPolygon];
			m_apPolygon[nCntPolygon] = NULL;
		}
	}

	// ナンバーの開放
	for (int nCntNumber = 0; nCntNumber < RESULT_SCORE_DIGITS; nCntNumber++)
	{// スコア
		if (m_apNumScore[nCntNumber] != NULL)
		{// ナンバーの終了処理
			m_apNumScore[nCntNumber]->Uninit();
			delete m_apNumScore[nCntNumber];
			m_apNumScore[nCntNumber] = NULL;
		}
	}
	for (int nCntNumber = 0; nCntNumber < RESULT_KILL_DIGITS; nCntNumber++)
	{// 倒した敵の数
		if (m_apNumKill[nCntNumber] != NULL)
		{// ナンバーの終了処理
			m_apNumKill[nCntNumber]->Uninit();
			delete m_apNumKill[nCntNumber];
			m_apNumKill[nCntNumber] = NULL;
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
void CResult::Update(void)
{
	// リザルトの経過時間をカウント
	m_nCntResult++;

	// 点滅
	m_apPolygon[1]->Flashing(60);

	int nNumber;

	for (int nCntDigits = 0; nCntDigits < m_nKillDigit; nCntDigits++)
	{// 倒した敵の数
		nNumber = m_nNumKill % (int)powf(10.0f, (nCntDigits + 1) * 1.0f) / (int)powf(10.0f, nCntDigits * 1.0f);

		m_apNumKill[nCntDigits]->SetNumber(nNumber);
	}
	for (int nCntDigits = 0; nCntDigits < m_nScoreDigit; nCntDigits++)
	{// スコア
		nNumber = m_nScore % (int)powf(10.0f, (nCntDigits + 1) * 1.0f) / (int)powf(10.0f, nCntDigits * 1.0f);

		m_apNumScore[nCntDigits]->SetNumber(nNumber);
	}

	// 現在のモードを取得
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_RESULT)
	{// ゲームクリア
		if (m_model != NULL)
		{
			D3DXVECTOR3 pos = m_model->GetPos();					// 位置の取得
			pos.x += 2;												// モデルを動かす
			m_model->SetPos(pos);									// 位置の設定
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, pos.y, pos.z), 2);	// 移動のエフェクト
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
	}
	else if (mode == CManager::MODE_GAMEOVER)
	{// ゲームオーバー
		if (m_nCntResult % 3 == 0)
		{// 一定時間経過ごとにモデルの位置から黒煙を出す
			CParticle::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
		}
	}

	// 入力の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	CRenderer *pRenderer = CManager::GetRenderer();
	pRenderer->Update();

	if (pKeyboard->GetAnyKey() || pJoypad->GetAnyTrigger(0))
	{// 画面遷移
		CFade::Create(CManager::MODE_RANKING);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
	// ポリゴンの描画
	for (int nCntPolygon = 0; nCntPolygon < RESULT_POLYGON; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon]->Draw();
	}

	// ナンバーの描画
	//for (int nCntNumber = 0; nCntNumber < m_nScoreDigit; nCntNumber++)
	//{// スコア
	//	m_apNumScore[nCntNumber]->Draw();
	//}
	//for (int nCntNumber = 0; nCntNumber < m_nKillDigit; nCntNumber++)
	//{// 倒した敵の数
	//	m_apNumKill[nCntNumber]->Draw();
	//}
}
