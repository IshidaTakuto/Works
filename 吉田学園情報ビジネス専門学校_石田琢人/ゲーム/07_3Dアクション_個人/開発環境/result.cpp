//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "result.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "ranking.h"
#include "camera.h"
#include "debugProc.h"
#include "number.h"
#include "logo.h"
#include "stage.h"
#include "modelSet.h"
#include "bg.h"
#include "ranking.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_LOGO00			"data/TEXTURE/result_logo00.png"
#define RESULT_LOGO01			"data/TEXTURE/result_logo01.png"
#define RESULT_LOGO_TIME		"data/TEXTURE/result_logo02.png"

#define RESULT_TIME				(420)			// タイトル画面までの時間
#define RESULT_NUMBER_SIZE		(200.0f)		// 数字の大きさ
#define RESULT_LOGO_WIDTH		(600.0f)		// ロゴの幅
#define RESULT_LOGO_HEIGHT		(150.0f)		// ロゴの高さ

#define RESULT_PRESS_WIDTH		(500.0f)		// 入力待ち表示の幅
#define RESULT_PRESS_HEIGHT		(100.0f)		// 入力待ち表示の高さ
#define RESULT_BEFORE_FLASH		(60)			// ボタンを入力する前の点滅
#define RESULT_AFTER_FLASH		(5)				// ボタンを入力した後の点滅

#define RESULT_MOVE_CAMERA		(1.0f)			// カメラの動く速度

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CResult::m_nTime = 0;		// 時間

//=========================================
// コンストラクタ
//=========================================
CResult::CResult()
{
	m_nCntResult = 0;
	m_nTimeDigit = 0;
	m_nFlash = 0;
	m_pLogo = NULL;
	m_pPressButton = NULL;
	m_pNumber = NULL;
	m_pPolygon = NULL;

	for (int nCntTexture = 0; nCntTexture < RESULT_MAX_TEXTURE; nCntTexture++)
	{// テクスチャ数分ループ
		m_pTexture[nCntTexture] = NULL;
	}
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
	// テクスチャの読み込み
	CBg::Load();
	CWeed::Load();
	CNumber::Load();

	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_RESULT);

	// モードの取得
	CManager::MODE mode = CManager::GetMode();

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	if (CManager::MODE_RESULT == mode) { D3DXCreateTextureFromFile(pDevice, RESULT_LOGO00, &m_pTexture[0]); }
	else if (CManager::MODE_GAMEOVER == mode) { D3DXCreateTextureFromFile(pDevice, RESULT_LOGO01, &m_pTexture[0]); }
	D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TIME, &m_pTexture[1]);

	// ロゴを生成
	m_pLogo = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2,100.0f,0.0f), RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT);
	m_pLogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色の設定

	if (NULL != m_pLogo)
	{// メモリ確保成功
		m_pLogo->BindTexture(m_pTexture[0]);
	}

	// 入力待ち表示を生成
	m_pPressButton = CLogoPressButton::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), RESULT_PRESS_WIDTH, RESULT_PRESS_HEIGHT);
	m_pPressButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色の設定

	m_nTimeDigit = (int)log10f((float)m_nTime) + 1;			// 時間の桁数
	if (m_nTimeDigit == 0) { m_nTimeDigit = 1; }			// 0だったとき
	m_pNumber = new CNumber*[m_nTimeDigit];					// 数字クラスの生成
	for (int nCntDigit = 0; nCntDigit < m_nTimeDigit; nCntDigit++)
	{// 時間
		m_pNumber[nCntDigit] = CNumber::Create(D3DXVECTOR3((700 + (RESULT_NUMBER_SIZE * m_nTimeDigit) - RESULT_NUMBER_SIZE) - (nCntDigit * (RESULT_NUMBER_SIZE * 0.5f)), 250, 0.0f), RESULT_NUMBER_SIZE, RESULT_NUMBER_SIZE);

		// 数字の設定
		int aNumber = m_nTime % (int)powf(10.0f, (float)(nCntDigit + 1)) / (int)powf(10.0f, (float)nCntDigit);
		m_pNumber[nCntDigit]->SetNumber(aNumber);
	}

	// ポリゴンの生成
	m_pPolygon = CScene2D::Create();

	if (NULL != m_pPolygon)
	{// メモリ確保成功
		m_pPolygon->BindTexture(m_pTexture[1]);
		m_pPolygon->SetPos(D3DXVECTOR3(300.0f, 300.0f, 0.0f));
		m_pPolygon->SetSize(RESULT_NUMBER_SIZE, RESULT_NUMBER_SIZE / 2.0f);
	}

	// 入力待ち表示の点滅速度
	m_nFlash = RESULT_BEFORE_FLASH;

	// 背景の生成
	CBg::Create(0, false);
	m_pBg[0] = CBg::Create(1, true);
	m_pBg[1] = CBg::Create(1, true);
	m_pMountain = CBg::Create(2, false);

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = m_pBg[1]->GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	// 頂点バッファをアンロック
	vtxBuff->Unlock();

	// ポリゴンの位置を設定
	m_pBg[1]->SetVtxBuff(vtxBuff);

	// 頂点情報の取得
	vtxBuff = m_pMountain->GetVtxBuff();

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

	// 頂点バッファをアンロック
	vtxBuff->Unlock();

	// ポリゴンの位置を設定
	m_pMountain->SetVtxBuff(vtxBuff);

	// ステージの生成
	CStage::Create(0);

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	// カメラの初期位置を設定
	pCamera->SetPosV(D3DXVECTOR3(-2800.0f, 80.0f, -500.0f));
	pCamera->SetPosR(D3DXVECTOR3(-2800.0f, 80.0f, 0.0f));

	if (CManager::MODE_RESULT == mode)
	{// ランキングにスコアを渡す
		CRanking::Set(m_nTime);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// テクスチャの破棄
	CBg::Unload();
	CModelSetManager::Unload();
	CWeed::Unload();
	CNumber::Unload();

	for (int nCntTexture = 0; nCntTexture < RESULT_MAX_TEXTURE; nCntTexture++)
	{// テクスチャ数分ループ
		if (NULL != m_pTexture[nCntTexture])
		{// テクスチャの開放
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}

	if (NULL != m_pLogo)
	{// ロゴの破棄
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	for (int nCntNumber = 0; nCntNumber < m_nTimeDigit; nCntNumber++)
	{// 桁数分ループ
		if (NULL != m_pNumber[nCntNumber])
		{// 数字の破棄
			m_pNumber[nCntNumber]->Uninit();
			m_pNumber[nCntNumber] = NULL;

		}
	}

	if (NULL != m_pNumber)
	{// 数字の破棄
		delete m_pNumber;
		m_pNumber = NULL;
	}

	if (NULL != m_pPolygon)
	{// ポリゴンの破棄
		m_pPolygon->Uninit();
		m_pPolygon = NULL;
	}

	if (NULL != m_pPressButton)
	{// ポリゴンの破棄
		m_pPressButton->Uninit();
		m_pPressButton = NULL;
	}

	for (int nCntBg = 0; nCntBg < RESULT_NUM_BG; nCntBg++)
	{
		if (NULL != m_pBg[nCntBg] != NULL)
		{// 背景の破棄
			m_pBg[nCntBg]->Uninit();
			m_pBg[nCntBg] = NULL;
		}
	}

	if (NULL != m_pMountain)
	{// 背景の破棄
		m_pMountain->Uninit();
		m_pMountain = NULL;
	}

	// 静的変数を初期値にする
	m_nTime = 0;

	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	// リザルトの経過時間をカウント
	m_nCntResult++;

	// 点滅
	m_pPressButton->Flashing(m_nFlash);

	if (m_pBg[0] != NULL)
	{// テクスチャ座標を動かす
		m_pBg[0]->TexMove(0.0001f);
	}

	if (m_pBg[1] != NULL)
	{// テクスチャ座標を動かす
		m_pBg[1]->TexMove(0.0003f);
	}

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	// 位置の取得
	D3DXVECTOR3 posCameraV = pCamera->GetPosV();
	D3DXVECTOR3 posCameraR = pCamera->GetPosR();

	if (CModelSetManager::GetGoalPos().x > posCameraV.x)
	{// カメラを動かす
		posCameraV.x += RESULT_MOVE_CAMERA;
		posCameraR.x += RESULT_MOVE_CAMERA;
	}
	else
	{// ランキングに遷移
		CFade::Create(CManager::MODE_RANKING);
	}

	// カメラの位置の設定
	pCamera->SetPosV(posCameraV);
	pCamera->SetPosR(posCameraR);

	// 入力の情報を取得
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CXInput			*pXInput = CManager::GetXInput();			// XInputの取得
	CSound			*pSound = CManager::GetSound();				// サウンドの取得


	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{// 画面遷移
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき
			CFade::Create(CManager::MODE_RANKING);
			pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

			// 点滅する速度を上げる
			m_nFlash = RESULT_AFTER_FLASH;
		}
	}

	if (m_nCntResult >= RESULT_TIME)
	{// 画面遷移
		CFade::Create(CManager::MODE_RANKING);
	}

	CDebugProc::Print("リザルト");

}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
}
