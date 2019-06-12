//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "ranking.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "debugProc.h"
#include "number.h"
#include "logo.h"
#include "bg.h"
#include "stage.h"
#include "modelSet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKING_LOGO			"data/TEXTURE/ranking_logo00.png"
#define RANKING_RANK1			"data/TEXTURE/rank_01.png"
#define RANKING_RANK2			"data/TEXTURE/rank_02.png"
#define RANKING_RANK3			"data/TEXTURE/rank_03.png"
#define RANKING_RANK4			"data/TEXTURE/rank_04.png"
#define RANKING_RANK5			"data/TEXTURE/rank_05.png"

#define RANKING_LOGO_WIDTH		(600.0f)		// ロゴの幅
#define RANKING_LOGO_HEIGHT		(100.0f)		// ロゴの高さ

#define RANKING_PRESS_WIDTH		(500.0f)		// 入力待ち表示の幅
#define RANKING_PRESS_HEIGHT	(100.0f)		// 入力待ち表示の高さ
#define RANKING_BEFORE_FLASH	(60)			// ボタンを入力する前の点滅
#define RANKING_AFTER_FLASH		(5)				// ボタンを入力した後の点滅

#define RANKING_TIME			(420)			// タイトル画面までの時間
#define RANKINGNUM_SIZE			(80.0f)			// 数字の大きさ

#define RANKING_MOVE_CAMERA		(-1.0f)			// カメラの動く速度

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CRanking::m_nRanking[MAX_RANKING] = { 150, 200, 250, 300, 350 };
int CRanking::m_nScore = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CRanking::CRanking()
{
	int nCntRanking;
	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// 数字を初期化
		m_apNumber[nCntRanking] = NULL;
		m_apPolygon[nCntRanking] = NULL;
		m_apTexture[nCntRanking] = NULL;
	}

	for (; nCntRanking < RANKING_MAX_TEXTURE; nCntRanking++)
	{// 残りのテクスチャを初期化
		m_apTexture[nCntRanking] = NULL;
	}

	m_nCntFade = 0;
	m_nCntHighScore = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	// テクスチャの読み込み
	CNumber::Load();
	CBg::Load();
	CWeed::Load();

	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_RANKING);

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ロゴの生成
	m_pLogo = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 50.0f, 0.0f), RANKING_LOGO_WIDTH, RANKING_LOGO_HEIGHT);
	m_pLogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色の設定

	// 入力待ち表示を生成
	m_pPress = CLogoPressButton::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f), RANKING_PRESS_WIDTH, RANKING_PRESS_HEIGHT);
	m_pPress->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色の設定

	// ランキングの順位ロゴ
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{// ランキングスコアの数ループ
		m_apPolygon[nCntRank] = CScene2D::Create();
		m_apPolygon[nCntRank]->SetPos(D3DXVECTOR3(100.0f, 125.0f + (nCntRank * RANKINGNUM_SIZE * 1.2f), 0.0f));
		m_apPolygon[nCntRank]->SetSize(RANKINGNUM_SIZE, RANKINGNUM_SIZE);
	}

	// 順位のテクスチャに色を付ける
	m_apPolygon[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));	// 1位
	m_apPolygon[1]->SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));	// 2位
	m_apPolygon[2]->SetColor(D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f));	// 3位
	m_apPolygon[3]->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));	// 4位
	m_apPolygon[4]->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));	// 5位

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, RANKING_LOGO, &m_apTexture[0]);
	m_pLogo->BindTexture(m_apTexture[0]);

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, RANKING_RANK1, &m_apTexture[1]);
	m_apPolygon[0]->BindTexture(m_apTexture[1]);

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, RANKING_RANK2, &m_apTexture[2]);
	m_apPolygon[1]->BindTexture(m_apTexture[2]);

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, RANKING_RANK3, &m_apTexture[3]);
	m_apPolygon[2]->BindTexture(m_apTexture[3]);

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, RANKING_RANK4, &m_apTexture[4]);
	m_apPolygon[3]->BindTexture(m_apTexture[4]);

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, RANKING_RANK5, &m_apTexture[5]);
	m_apPolygon[4]->BindTexture(m_apTexture[5]);

	// ランキングのスコア(数字)
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数ループ
		m_nDigits[nCntRanking] = (int)log10f((float)m_nRanking[nCntRanking]) + 1;
		if (0 >= m_nDigits[nCntRanking]) { m_nDigits[nCntRanking] = 1; }

		m_apNumber[nCntRanking] = new CNumber*[m_nDigits[nCntRanking]];

		for (int nCntDigit = 0; nCntDigit < m_nDigits[nCntRanking]; nCntDigit++)
		{// ランキングスコアの桁数ループ
			m_apNumber[nCntRanking][nCntDigit] = CNumber::Create(D3DXVECTOR3((700 + (RANKINGNUM_SIZE * m_nDigits[nCntRanking]) - RANKINGNUM_SIZE) - (nCntDigit * RANKINGNUM_SIZE * 2), 125.0f + (nCntRanking * RANKINGNUM_SIZE * 1.2f), 0.0f), RANKINGNUM_SIZE, RANKINGNUM_SIZE);

			// 数字の設定
			int aNumber = m_nRanking[nCntRanking] % (int)powf(10.0f, (float)(nCntDigit + 1)) / (int)powf(10.0f, (float)nCntDigit);
			m_apNumber[nCntRanking][nCntDigit]->SetNumber(aNumber);
		}
	}

	// 入力待ち表示の点滅速度
	m_nFlash = RANKING_BEFORE_FLASH;

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
	pCamera->SetPosV(D3DXVECTOR3(2800.0f, 80.0f, -500.0f));
	pCamera->SetPosR(D3DXVECTOR3(2800.0f, 80.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	// テクスチャの破棄
	CBg::Unload();
	CModelSetManager::Unload();
	CWeed::Unload();
	CNumber::Unload();

	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < RANKING_MAX_TEXTURE; nCntTexture++)
	{// テクスチャ数分ループ
		if (NULL != m_apTexture[nCntTexture])
		{// テクスチャの開放
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数ループ
		for (int nCntNumber = 0; nCntNumber < m_nDigits[nCntRanking]; nCntNumber++)
		{// 桁数分ループ
			if (NULL != m_apNumber[nCntRanking][nCntNumber])
			{// 数字の破棄
				m_apNumber[nCntRanking][nCntNumber]->Uninit();
				m_apNumber[nCntRanking][nCntNumber] = NULL;

			}
		}

		if (NULL != m_apNumber[nCntRanking])
		{// 数字の破棄
			delete m_apNumber[nCntRanking];
			m_apNumber[nCntRanking] = NULL;
		}

		if (NULL != m_apPolygon[nCntRanking])
		{// ポリゴンの破棄
			m_apPolygon[nCntRanking]->Uninit();
			m_apPolygon[nCntRanking] = NULL;
		}
	}

	for (int nCntBg = 0; nCntBg < RANKING_NUM_BG; nCntBg++)
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

	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	// 点滅用カウントアップ
	m_nCntHighScore++;
	m_nCntHighScore = m_nCntHighScore % 4;

	// 自分の取得したスコアを点滅させる
	if (m_nScore > -1)
	{// スコアが入っている
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			if (m_nRanking[nCntRanking] == m_nScore)
			{// 自分のスコアを点滅させる
				for (int nCntNumber = 0; nCntNumber < m_nDigits[nCntRanking]; nCntNumber++)
				{// 桁数分まわす
					if (m_nCntHighScore < 2)
					{
						m_apNumber[nCntRanking][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					}
					else if (m_nCntHighScore >= 2)
					{
						m_apNumber[nCntRanking][nCntNumber]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
					}
				}
				break;	// 同じスコアがあった場合の保険
			}
		}
	}

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
		posCameraV.x += RANKING_MOVE_CAMERA;
		posCameraR.x += RANKING_MOVE_CAMERA;
	}
	else
	{// ランキングに遷移
		CFade::Create(CManager::MODE_RANKING);
	}

	// カメラの位置の設定
	pCamera->SetPosV(posCameraV);
	pCamera->SetPosR(posCameraR);

	// 入力待ちの点滅
	m_pPress->Flashing(m_nFlash);

	// タイトルへフェードするまでのカウント
	m_nCntFade++;
	if (m_nCntFade >= RANKING_TIME)
	{//モード選択
		CFade::Create(CManager::MODE_TITLE);
	}

	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CXInput			*pXInput = CManager::GetXInput();			// XInputの取得
	CSound			*pSound = CManager::GetSound();				// サウンドの取得

	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{// 画面遷移
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき
			CFade::Create(CManager::MODE_TITLE);
			pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

			// 点滅する速度を上げる
			if (CFade::FADE_NONE != CFade::GetFade()) { m_nFlash = RANKING_AFTER_FLASH; }
		}
	}

	CDebugProc::Print("ランキング");

}

//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{
}

//=============================================================================
// スコアのセット
//=============================================================================
void CRanking::Set(int nScore)
{
	int nSwap;							// 入れ替え用
	int aEmptyData[MAX_RANKING + 1];	// 入れ替え用

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{// 今までの記録を空のデータに入れる
		aEmptyData[nCntNum] = m_nRanking[nCntNum];
	}

	aEmptyData[MAX_RANKING] = nScore;	// 最後の配列に最新のスコアを入れる

	// 入れ替え
	for (int nCntNum = 1; nCntNum < MAX_RANKING + 1; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aEmptyData[nCntNum1 - 1] >= aEmptyData[nCntNum1])
			{// 入れ替え
				nSwap = aEmptyData[nCntNum1];
				aEmptyData[nCntNum1] = aEmptyData[nCntNum1 - 1];
				aEmptyData[nCntNum1 - 1] = nSwap;
			}
		}
	}

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{// 入れ替えが終わったデータをランキングに反映する
		m_nRanking[nCntNum] = aEmptyData[nCntNum];

	}

	m_nScore = nScore;	// 最新のスコアを更新する
}
