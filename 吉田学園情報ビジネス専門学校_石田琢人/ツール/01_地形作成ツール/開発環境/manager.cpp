//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debugProc.h"

#include "player.h"
#include "meshField.h"
#include "meshCylinder.h"
#include "scene3DIndex.h"

//=========================================
// マクロ定義
//=========================================

//=========================================
// 静的メンバ変数
//=========================================
CRenderer *CManager::m_pRenderer = NULL;		// レンダラークラスのポインタ変数
CInputKeyboard *CManager::m_pInput = NULL;		// インプットクラスのポインタ変数
CCamera *CManager::m_pCamera = NULL;
CLight * CManager::m_pLight = NULL;
CDebugProc *CManager::m_pDebugProc = NULL;
CPlayer *CManager::m_pPlayer = NULL;
CMeshField *CManager::m_pMeshField = NULL;
CCylinder *CManager::m_pCylinder = NULL;

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

	CScene3DIndex::Load();
	CMeshField::Load();	// メッシュフィールドテクスチャの読み込み
	CPlayer::Load();	// プレイヤーの読み込み

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create();

	// メッシュフィールド生成
	m_pMeshField = CMeshField::Create();

	// メッシュシリンダー生成
	m_pCylinder = CCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CManager::Uninit(void)
{
	CMeshField::Unload();	// フィールドテクスチャの破棄
	CPlayer::Unload();	// プレイヤーモデルの破棄

	if (m_pPlayer != NULL)
	{// プレイヤーの破棄
		m_pPlayer->Uninit();
		m_pPlayer = NULL;
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
	CDebugProc::Print("ファイル出力　　[F9]");
	CDebugProc::Print("ファイル読み込み[F3]");
	CDebugProc::Print("-------------------------------");
	CDebugProc::Print("ワイヤーフレーム[F8]");
	CDebugProc::Print("テクスチャ　　　[F7]");
	CDebugProc::Print("法線の再計算　　[F5]");
	CDebugProc::Print("頂点を上げる　　[Y]");
	CDebugProc::Print("Ｘ軸分割数　　　[X+I][X+K]");
	CDebugProc::Print("Ｚ軸分割数　　　[Z+I][Z+K]");
	CDebugProc::Print("幅　　　　　　　[X+U][X+J]");
	CDebugProc::Print("奥行　　　　　　[Z+U][Z+J]");
	CDebugProc::Print("選択範囲　　　　[↑][↓]");
	CDebugProc::Print("上げる値　　　　[→][←]");
	CDebugProc::Print("SHIFTで数値を増加");
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
