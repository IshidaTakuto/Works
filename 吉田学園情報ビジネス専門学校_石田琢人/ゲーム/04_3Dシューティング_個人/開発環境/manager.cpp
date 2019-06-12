//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "score.h"

//=========================================
// マクロ定義
//=========================================

//=========================================
// 静的メンバ変数
//=========================================
CRenderer *CManager::m_pRenderer = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight * CManager::m_pLight = NULL;
CSound *CManager::m_pSound = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;
CTitle *CManager::m_pTitle = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CRanking *CManager::m_pRanking = NULL;

//=========================================
// コンストラクタ
//=========================================
CManager::CManager()
{
}

//=========================================
// デストラクタ
//=========================================
CManager::~CManager()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// レンダリングクラスの生成
	if (NULL == m_pRenderer)
	{
		m_pRenderer = new CRenderer;

		if (NULL != m_pRenderer)
		{
			// 初期化処理
			if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))	//FALSEでフルスクリーン
			{
				return -1;
			}
		}
		else
		{
			MessageBox(0, "レンダリングクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "レンダリングクラスに何か入っています", "失敗", MB_OK);
	}

	// インプットクラスの生成
	if (NULL == m_pInputKeyboard)
	{// キーボード
		m_pInputKeyboard = new CInputKeyboard;	// キーボードの入力用

		if (NULL != m_pInputKeyboard)
		{// メモリ確保
			// 初期化処理
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "インプットクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// NULLでなかったとき
		MessageBox(0, "インプットクラスに何か入っています", "失敗", MB_OK);
	}
	if (NULL == m_pInputJoypad)
	{// パッド
		m_pInputJoypad = new CInputJoypad;	// キーボードの入力用

		if (NULL != m_pInputJoypad)
		{// メモリ確保
		 // 初期化処理
			if (FAILED(m_pInputJoypad->Init(hInstance, hWnd)))
			{
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "インプットクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// NULLでなかったとき
		MessageBox(0, "インプットクラスに何か入っています", "失敗", MB_OK);
	}


	// カメラクラスの生成
	if (NULL == m_pCamera)
	{// ヌルチェック
		m_pCamera = new CCamera;	// メモリ確保

		if (NULL != m_pCamera)
		{// メモリを確保できたかどうか
			m_pCamera->Init();
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "カメラクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// NULLでなかったとき
		MessageBox(0, "カメラクラスに何か入っています", "失敗", MB_OK);
	}

	// ライトクラスの生成
	if (NULL == m_pLight)
	{// ヌルチェック
		m_pLight = new CLight;	// メモリ確保

		if (NULL != m_pLight)
		{// メモリを確保できたかどうか
			m_pLight->Init();
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "ライトクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// NULLでなかったとき
		MessageBox(0, "ライトクラスに何か入っています", "失敗", MB_OK);
	}

	// サウンドクラスの生成
	if (NULL == m_pSound)
	{// ヌルチェック
		m_pSound = new CSound;	// メモリ確保

		if (NULL != m_pSound)
		{// メモリを確保できたかどうか
			m_pSound->Init(hWnd);

		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "サウンドクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// NULLでなかったとき
		MessageBox(0, "サウンドクラスに何か入っています", "失敗", MB_OK);
	}

	// モードの設定
	SetMode(m_mode);

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CManager::Uninit(void)
{
	CScene::ReleaseAll();

	// レンダリングクラスの破棄
	if (NULL != m_pRenderer)
	{
		// 終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	else
	{
		MessageBox(0, "レンダリングクラスの破棄に失敗しました", "失敗", MB_OK);
	}

	// カメラクラスの破棄
	if (NULL != m_pCamera)
	{
		// 終了処理
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	else
	{
		MessageBox(0, "カメラクラスの破棄に失敗しました", "失敗", MB_OK);
	}

	// ライトクラスの破棄
	if (NULL != m_pLight)
	{
		// 終了処理
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	else
	{
		MessageBox(0, "ライトクラスの破棄に失敗しました", "失敗", MB_OK);
	}

	// サウンドクラスの破棄
	if (NULL != m_pSound)
	{
		// 終了処理
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
	else
	{
		MessageBox(0, "ライトクラスの破棄に失敗しました", "失敗", MB_OK);
	}

	// インプットクラスの破棄
	if (NULL != m_pInputKeyboard)
	{// キーボード
		// 終了処理
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	else
	{
		MessageBox(0, "インプットクラスの破棄に失敗しました", "失敗", MB_OK);
	}
	if (NULL != m_pInputJoypad)
	{// パッド
		// 終了処理
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}
	else
	{
		MessageBox(0, "インプットクラスの破棄に失敗しました", "失敗", MB_OK);
	}

	// タイトルクラスの破棄
	if (NULL != m_pTitle)
	{
		// 終了処理
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = NULL;
	}

	// チュートリアルクラスの破棄
	if (NULL != m_pTutorial)
	{
		// 終了処理
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = NULL;
	}

	// ゲームクラスの破棄
	if (NULL != m_pGame)
	{
		// 終了処理
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = NULL;
	}

	// リザルトクラスの破棄
	if (NULL != m_pResult)
	{
		// 終了処理
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = NULL;
	}

	// ランキングクラスの破棄
	if (NULL != m_pRanking)
	{
		// 終了処理
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = NULL;
	}

}

//=========================================
// 更新処理
//=========================================
void CManager::Update(void)
{
	m_pInputKeyboard->Update();
	m_pInputJoypad->Update();
	m_pLight->Update();

	// ポーズ中はカメラを更新しない
	bool bPause = CGame::GetPause();
	if (bPause == false)
	{
		m_pCamera->Update();
	}

	switch (m_mode)
	{
	case MODE_TITLE:	// タイトル
		if (m_pTitle != NULL)
		{// タイトルの更新処理
			m_pTitle->Update();
		}

		break;

	case MODE_TUTORIAL:	// チュートリアル
		if (m_pTutorial != NULL)
		{// チュートリアルの更新処理
			m_pTutorial->Update();
		}

		break;

	case MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
		{// ゲームの更新処理
			m_pGame->Update();
		}

		break;

	case MODE_RESULT:	// リザルト
		if (m_pResult != NULL)
		{// ゲームの更新処理
			m_pResult->Update();
		}

		break;

	case MODE_GAMEOVER:	// リザルト
		if (m_pResult != NULL)
		{// ゲームの更新処理
			m_pResult->Update();
		}

		break;

	case MODE_RANKING:	// ランキング
		if (m_pRanking != NULL)
		{// ゲームの更新処理
			m_pRanking->Update();
		}

		break;
	}
}

//=========================================
// 描画処理
//=========================================
void CManager::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	pRenderer->Draw();
}

//=============================================================================
// モードの終了・初期化処理
//=============================================================================
const void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// タイトル
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// チュートリアル
			m_pSound->StopSound(CSound::SOUND_LABEL_TUTORIAL);
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{// ゲーム
			m_pRanking->Set(CScore::GetScore());
			m_pSound->StopSound(CSound::SOUND_LABEL_GAME);
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{// クリア
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;

	case MODE_GAMEOVER:
		if (m_pResult != NULL)
		{// ゲームオーバー
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{// ランキング
			m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			m_pSound->StopSound(CSound::SOUND_LABEL_CLEAR);
			m_pSound->StopSound(CSound::SOUND_LABEL_OVER);
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
		}
		break;
	}

	m_mode = mode;

	//新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{// タイトル
			m_pSound->PlaySound(CSound::SOUND_LABEL_TITLE);
			m_pCamera->Init();
			m_pTitle = new CTitle;
			m_pTitle->Init();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// チュートリアル
			m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			m_pSound->PlaySound(CSound::SOUND_LABEL_TUTORIAL);
			m_pCamera->Init();
			m_pTutorial = new CTutorial;
			m_pTutorial->Init();
		}
		break;

	case MODE_GAME:
		if (m_pGame == NULL)
		{// ゲーム
			m_pSound->PlaySound(CSound::SOUND_LABEL_GAME);
			m_pCamera->Init();
			m_pGame = new CGame;
			m_pGame->Init();
		}
		break;

	case MODE_RESULT:
		if (m_pResult == NULL)
		{// クリア
			m_pSound->PlaySound(CSound::SOUND_LABEL_CLEAR);
			m_pResult = new CResult;
			m_pResult->Init();
		}
		break;

	case MODE_GAMEOVER:
		if (m_pResult == NULL)
		{// ゲームオーバー
			m_pSound->PlaySound(CSound::SOUND_LABEL_OVER);
			m_pResult = new CResult;
			m_pResult->Init();
		}
		break;

	case MODE_RANKING:
		if (m_pRanking == NULL)
		{// ランキング
			m_pRanking = new CRanking;
			m_pRanking->Init();
		}
		break;
	}

	//m_mode = mode;
}