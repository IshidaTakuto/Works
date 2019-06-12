//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debugProc.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "pause.h"
#include "particle.h"
#include "meshField.h"
#include "sound.h"

#include <stdio.h>

//=========================================
// マクロ定義
//=========================================
#define SYSTEM_FILE	"data/TEXT/system.ini"		// 地面の情報テキスト

//=========================================
// 静的メンバ変数
//=========================================
CRenderer			*CManager::m_pRenderer = NULL;			// レンダラークラスのポインタ変数
CInputKeyboard		*CManager::m_pInputKeyboard = NULL;		// インプットクラスのポインタ変数
CInputJoyStick		*CManager::m_pJoyStick = NULL;			// ジョイスティッククラス型のポインタ
CXInput				*CManager::m_pXInput = NULL;			// XInputクラス型のポインタ
CCamera				*CManager::m_pCamera = NULL;			// カメラクラスのポインタ変数
CLight				*CManager::m_pLight = NULL;				// ライトクラスのポインタ変数
CDebugProc			*CManager::m_pDebugProc = NULL;			// デバッグ文字クラスのポインタ変数
CSound				*CManager::m_pSound = NULL;				// サウンドクラスのポインタ変数
CTitle				*CManager::m_pTitle = NULL;				// タイトルクラスのポインタ変数
CTutorial			*CManager::m_pTutorial = NULL;			// チュートリアルクラスのポインタ変数
CGame				*CManager::m_pGame = NULL;				// ゲームクラスのポインタ変数
CResult				*CManager::m_pResult = NULL;			// リザルトクラスのポインタ変数
CRanking			*CManager::m_pRanking = NULL;			// ランキングクラスのポインタ変数
CManager::MODE		CManager::m_mode = CManager::MODE_TITLE;

int  CManager::m_nNumStage = 0;
char **CManager::m_pFieldFileName = NULL;
char **CManager::m_pSetFileName = NULL;

//=========================================
// システムスクリプトの読み込み
//=========================================
HRESULT CManager::Load(void)
{
	FILE *pFile;	// ファイルのポインタ変数
	char aStr[128];	// 取得する文字列
	int nCntField = 0;
	int nCntSet = 0;

	// ファイルを開く
	pFile = fopen(SYSTEM_FILE, "r");

	if (NULL != pFile)
	{// ファイルがあった
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", aStr);

			if (strcmp(aStr, "NUM_STAGE") == 0)
			{// 読み込むスクリプトファイル数
				fscanf(pFile, " = %d", &m_nNumStage);

				// スクリプト分のメモリ確保
				m_pFieldFileName = new char*[m_nNumStage];
				m_pSetFileName = new char*[m_nNumStage];
			}
			if (strcmp(aStr, "FIELD_FILENAME") == 0)
			{// 読み込むスクリプトファイル
				m_pFieldFileName[nCntField] = new char[64];
				fscanf(pFile, " = %s", &m_pFieldFileName[nCntField][0]);
				nCntField++;
			}
			if (strcmp(aStr, "SET_FILENAME") == 0)
			{// 読み込むスクリプトファイル
				m_pSetFileName[nCntSet] = new char[64];
				fscanf(pFile, " = %s", &m_pSetFileName[nCntSet][0]);
				nCntSet++;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！", "system.ini", MB_OK);
	}

	return S_OK;
}

//=========================================
// 読み込んだデータの解放
//=========================================
void CManager::Unload(void)
{
	for (int nCntStage = 0; nCntStage < m_nNumStage; nCntStage++)
	{// メモリの開放
		if (NULL != m_pFieldFileName[nCntStage])
		{// 文字列の破棄
			delete m_pFieldFileName[nCntStage];
			m_pFieldFileName[nCntStage] = NULL;
		}

		if (NULL != m_pSetFileName[nCntStage])
		{// 文字列の破棄
			delete m_pSetFileName[nCntStage];
			m_pSetFileName[nCntStage] = NULL;
		}
	}

	if (NULL != m_pFieldFileName)
	{// メモリの開放
		delete m_pFieldFileName;
		m_pFieldFileName = NULL;
	}

	if (NULL != m_pSetFileName)
	{// メモリの開放
		delete m_pSetFileName;
		m_pSetFileName = NULL;
	}
}

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
	// システムファイルを読み込む
	Load();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// レンダリングクラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pRenderer)
	{// メモリが空
		m_pRenderer = new CRenderer;	// メモリ確保

		if (NULL != m_pRenderer)
		{// メモリが確保できた
			if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))	//FALSEでフルスクリーン
			{// 初期化処理
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "レンダリングクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// 空でなかったとき
		MessageBox(0, "レンダリングクラスに何か入っています", "失敗", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// インプット(キーボード)クラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pInputKeyboard)
	{// メモリが空
		m_pInputKeyboard = new CInputKeyboard;	// キーボードの入力用

		if (NULL != m_pInputKeyboard)
		{// メモリ確保
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{// 初期化処理
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "インプット(キーボード)クラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// 空でなかったとき
		MessageBox(0, "インプット(キーボード)クラスに何か入っています", "失敗", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// インプット(DirectInput)クラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pJoyStick)
	{// メモリが空
		m_pJoyStick = new CInputJoyStick;	// キーボードの入力用

		if (NULL != m_pJoyStick)
		{// メモリ確保
			if (FAILED(m_pJoyStick->Init(hInstance, hWnd)))
			{// 初期化処理
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "インプット(DirectInput)クラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// 空でなかったとき
		MessageBox(0, "インプット(DirectInput)クラスに何か入っています", "失敗", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// インプット(XInput)クラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pXInput)
	{// メモリが空
		m_pXInput = new CXInput;	// キーボードの入力用

		if (NULL != m_pXInput)
		{// メモリ確保
			if (FAILED(m_pXInput->Init()))
			{// 初期化処理
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "インプット(XInput)クラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// 空でなかったとき
		MessageBox(0, "インプット(XInput)クラスに何か入っています", "失敗", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// カメラクラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ライトクラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

#ifdef _DEBUG
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// デバッグ文字クラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pDebugProc)
	{// ヌルチェック
		m_pDebugProc = new CDebugProc;	// メモリ確保

		if (NULL != m_pDebugProc)
		{// メモリを確保できたかどうか
			m_pDebugProc->Init();
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "デバッグ文字クラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// NULLでなかったとき
		MessageBox(0, "デバッグ文字クラスに何か入っています", "失敗", MB_OK);
	}
#endif

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// サウンドクラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

	// 現在のモードを設定
	SetMode(m_mode);

	// ファイルからデータを読み取る
	CParData::Load();

	// デバッグ文字の表示
	m_bDebugProc = true;

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CManager::Uninit(void)
{
	// データの破棄
	Unload();
	CMeshField::Unload();
	CParData::Unload();

	CScene::ReleaseAll();	// 全てのオブジェクトの破棄

	if (NULL != m_pRenderer)
	{// レンダリングクラスの破棄
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (NULL != m_pInputKeyboard)
	{// インプット(キーボード)クラスの破棄
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (NULL != m_pJoyStick)
	{// インプット(DirectInput)クラスの破棄
		m_pJoyStick->Uninit();
		delete m_pJoyStick;
		m_pJoyStick = NULL;
	}

	if (NULL != m_pXInput)
	{// インプット(XInput)クラスの破棄
		m_pXInput->Uninit();
		delete m_pXInput;
		m_pXInput = NULL;
	}

	if (NULL != m_pCamera)
	{// カメラクラスの破棄
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (NULL != m_pLight)
	{// ライトクラスの破棄
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

#ifdef _DEBUG
	if (NULL != m_pDebugProc)
	{// デバッグ文字クラスの破棄
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
#endif

	if (NULL != m_pSound)
	{// サウンドクラスの破棄
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
}

//=========================================
// 更新処理
//=========================================
void CManager::Update(void)
{
	m_pInputKeyboard->Update();		// インプットクラスの更新
	m_pJoyStick->Update();
	m_pXInput->Update();

	if (m_pGame != NULL)
	{// ゲーム中
		if (!m_pGame->GetPauseBool())
		{// ポーズ中でないとき
			m_pCamera->Update();			// カメラクラスの更新
			m_pRenderer->Update();			// レンダラークラスの更新
			m_pLight->Update();				// ライトクラスの更新
		}
		else
		{// ポーズ中
			CPause *pPause = m_pGame->GetPause();
			if (pPause != NULL)
			{
				pPause->Update();
			}

			m_pGame->Update();
		}
	}
	else
	{// ゲーム以外
		m_pCamera->Update();			// カメラクラスの更新
		m_pRenderer->Update();			// レンダラークラスの更新
		m_pLight->Update();				// ライトクラスの更新
	}

#ifdef _DEBUG
	if (m_pInputKeyboard->GetTrigger(DIK_F1))
	{
		m_bDebugProc = m_bDebugProc ? false : true;
		m_pDebugProc->DebugDisp(m_bDebugProc);
	}
#endif
}

//=========================================
// 描画処理
//=========================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();	// 描画
}

//=============================================================================
// モードの終了・初期化処理
//=============================================================================
const void CManager::SetMode(MODE mode)
{
	// フェード以外のオブジェクトを破棄
	CScene::ModeUninit();

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// タイトル
			m_pTitle = NULL;
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// チュートリアル
			m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{// ゲーム
			m_pSound->StopSound(CSound::SOUND_LABEL_GAME);
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{// クリア
			m_pResult = NULL;
		}
		break;

	case MODE_GAMEOVER:
		if (m_pResult != NULL)
		{// ゲームオーバー
			m_pResult = NULL;
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{// ランキング
			m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			m_pSound->StopSound(CSound::SOUND_LABEL_CLEAR);
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
		{// タイトル
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
			//m_pSound->PlaySound(CSound::SOUND_LABEL_OVER);
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
}