//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "title.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "debugProc.h"
#include "logo.h"
#include "stage.h"
#include "modelSet.h"
#include "bg.h"
#include "camera.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TEXTURE00	"data/TEXTURE/title_logo.png"

#define TITLE_TIME		(1200)		// ランキング画面までの時間

#define TITLE_LOGO_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 250.0f, 0.0f))	// タイトルロゴの位置
#define TITLE_LOGO_WIDTH	(700.0f)	// タイトルロゴの幅
#define TITLE_LOGO_HEIGHT	(300.0f)	// タイトルロゴの高さ

#define TITLE_PRESSANY_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 600.0f, 0.0f))	// 入力待ち表示の位置
#define TITLE_PRESSANY_WIDTH	(500.0f)	// 入力待ちの幅
#define TITLE_PRESSANY_HEIGHT	(100.0f)	// 入力待ちの高さ
#define TITLE_BEFORE_FLASH		(60)		// ボタンを入力する前の点滅
#define TITLE_AFTER_FLASH		(5)			// ボタンを入力した後の点滅

#define TITLE_MOVE_CAMERA		(1.0f)		// カメラの動く速度

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=========================================
// コンストラクタ
//=========================================
CTitle::CTitle()
{
	m_nCntTitle = 0;	// ランキングまでのカウンタ
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
	// テクスチャの読み込み
	CBg::Load();
	CWeed::Load();

	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_TITLE);

	// ロゴの生成
	m_pLogo = CLogo::Create(TITLE_LOGO_POS, TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT);
	m_pPressButton = CLogoPressButton::Create(TITLE_PRESSANY_POS, TITLE_PRESSANY_WIDTH, TITLE_PRESSANY_HEIGHT);

	// 色の設定
	m_pLogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_pPressButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE00, &pTexture);		// テクスチャの生成
	m_pLogo->BindTexture(pTexture);										// テクスチャの設定

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
	pCamera->SetPosV(D3DXVECTOR3(-2800.0f, 4000.0f, 0.0f));
	pCamera->SetPosR(D3DXVECTOR3(-2800.0f, 0.0f, 0.0f));

	// 点滅する間隔
	m_nFlash = TITLE_BEFORE_FLASH;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// テクスチャの破棄
	CBg::Unload();
	CModelSetManager::Unload();
	CWeed::Unload();

	if (NULL != m_pLogo)
	{// ロゴの破棄
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	if (NULL != m_pPressButton)
	{// ロゴの破棄
		m_pPressButton->Uninit();
		m_pPressButton = NULL;
	}

	for (int nCntBg = 0; nCntBg < TITLE_NUM_BG; nCntBg++)
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

	// 自分の破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	// タイトルの経過時間
	m_nCntTitle++;

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
		posCameraV.x += TITLE_MOVE_CAMERA;
		posCameraR.x += TITLE_MOVE_CAMERA;
	}
	else
	{// ランキングに遷移
		CFade::Create(CManager::MODE_RANKING);
	}

	// カメラの位置の設定
	pCamera->SetPosV(posCameraV);
	pCamera->SetPosR(posCameraR);

	// 入力の情報を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// サウンドの取得
	CSound *pSound = CManager::GetSound();

	// 色の取得
	D3DXCOLOR col = m_pLogo->GetColor();

	if (col.a >= 1.0f)
	{// 透明でない状態

		m_pPressButton->Flashing(m_nFlash);	// 入力待ちテクスチャを点滅

		if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
		{// 画面遷移
			if (CFade::GetFade() == CFade::FADE_NONE)
			{// フェードがないとき
				CFade::Create(CManager::MODE_TUTORIAL);
				pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

				// 点滅する速度を上げる
				m_nFlash = TITLE_AFTER_FLASH;
			}
		}
		else if (m_nCntTitle >= TITLE_TIME)
		{// 一定時間経過
			CFade::Create(CManager::MODE_RANKING);
		}
	}
	else
	{// 透明な状態
		col.a += 0.01f;
	}

	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{// ボタンを押すとすぐに表示される
		col.a = 1.0f;
	}

	// 色の設定
	m_pLogo->SetColor(col);

	CDebugProc::Print("タイトル");
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
}
