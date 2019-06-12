//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : Ishida Takuto
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

#include "setPlayer.h"
#include "meshField.h"
#include "scene3DIndex.h"
#include "GUIManager.h"

#include <stdio.h>

//=========================================
// マクロ定義
//=========================================
#define SYSTEM_FILE "data/TEXT/system.ini"

//=========================================
// 静的メンバ変数
//=========================================
CRenderer *CManager::m_pRenderer = NULL;		// レンダラークラスのポインタ変数
CInputKeyboard *CManager::m_pInput = NULL;		// インプットクラスのポインタ変数
CCamera *CManager::m_pCamera = NULL;
CLight * CManager::m_pLight = NULL;
CDebugProc *CManager::m_pDebugProc = NULL;
CSetPlayer *CManager::m_pSetPlayer = NULL;
CMeshField *CManager::m_pMeshField = NULL;

int	 CManager::m_nNumStage = 0;
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

//=============================================================================
// オブジェクト数チェック
//=============================================================================
int CManager::SetNumType(void)
{
	for (int nCntType = 0; nCntType < CModelObjectManager::TYPE_MAX; nCntType++)
	{
		if (0 < CModelObjectManager::GetNumType(m_type))
		{
			return nCntType;
		}
		m_type = (CModelObjectManager::TYPE)(1 + m_type);
	}

	return -1;
}

//=========================================
// コンストラクタ
//=========================================
CManager::CManager()
{
	m_type = CModelObjectManager::TYPE_MODEL;
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
	// システムファイルの読み込み
	Load();

	// レンダリングクラスの生成
	if (NULL == m_pRenderer)
	{// メモリが空
		m_pRenderer = new CRenderer;	// メモリ確保

		if (NULL != m_pRenderer)
		{// メモリが確保できた
			if (FAILED(m_pRenderer->Init(hInstance, hWnd, TRUE)))	//FALSEでフルスクリーン
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

	// インプットクラスの生成
	if (NULL == m_pInput)
	{// メモリが空
		m_pInput = new CInputKeyboard;	// キーボードの入力用

		if (NULL != m_pInput)
		{// メモリ確保
			if (FAILED(m_pInput->Init(hInstance, hWnd)))
			{// 初期化処理
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "インプットクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// 空でなかったとき
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

	// デバッグ文字クラスの生成
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

	m_type = (CModelObjectManager::TYPE)SetNumType();
	if (m_type >= 0)
	{// プレイヤーの生成
		m_pSetPlayer = CSetPlayer::Create(m_type);
		CGuiManager::SetObjectType(m_type);
	}

	// メッシュフィールド生成
	m_pMeshField = CMeshField::Create();

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CManager::Uninit(void)
{
	CModelObjectManager::Unload();

	if (m_pSetPlayer != NULL)
	{// プレイヤーの破棄
		m_pSetPlayer->Uninit();
		m_pSetPlayer = NULL;
	}

	CScene::ReleaseAll();	// 全てのオブジェクトの破棄

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

	// インプットクラスの破棄
	if (NULL != m_pInput)
	{
		// 終了処理
		m_pInput->Uninit();
		delete m_pInput;
		m_pInput = NULL;
	}
	else
	{
		MessageBox(0, "インプットクラスの破棄に失敗しました", "失敗", MB_OK);
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

	// デバッグ文字クラスの破棄
	if (NULL != m_pDebugProc)
	{
		// 終了処理
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
	else
	{
		MessageBox(0, "デバッグ文字クラスの破棄に失敗しました", "失敗", MB_OK);
	}
}

//=========================================
// 更新処理
//=========================================
void CManager::Update(void)
{
	CDebugProc::Print("-------------------------------");
	CDebugProc::Print("表示・非表示　　[F1]");
	CDebugProc::Print("ワイヤーフレーム[F8]");
	CDebugProc::Print("テクスチャ　　　[F7]");
	CDebugProc::Print("カメラ操作はNUMパッドの数字でできます");
	CDebugProc::Print("-------------------------------\n\n");

	if (m_pInput->GetTrigger(DIK_F1))
	{// デバッグ表示・非表示
		m_pDebugProc->DebugDisp();
	}

	m_pCamera->Update();	// カメラクラスの更新
	m_pInput->Update();		// インプットクラスの更新
	m_pRenderer->Update();	// レンダラークラスの更新
	m_pLight->Update();		// ライトクラスの更新
}

//=========================================
// 描画処理
//=========================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();	// 描画
}
