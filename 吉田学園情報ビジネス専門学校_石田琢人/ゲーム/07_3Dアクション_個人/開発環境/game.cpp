//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "camera.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "debugProc.h"
#include "result.h"
#include "sound.h"
#include "life.h"
#include "modelSet.h"
#include "player.h"
#include "enemy.h"
#include "meshField.h"
#include "modelManager.h"
#include "motion.h"
#include "bg.h"
#include "pause.h"
#include "stage.h"
#include "timer.h"
#include "number.h"
#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CGame::STATE CGame::m_state = CGame::STATE_NONE;
int CGame::m_nCurStage = 0;
CBg *CGame::m_pMountain = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_modeCounter = 0;

	for (int nCntBg = 0; nCntBg < GAME_NUM_BG; nCntBg++)
	{// 背景の初期化
		m_pBg[nCntBg] = NULL;
	}
	m_pPause = NULL;
	m_bPause = false;
	m_nCurStage = 0;
	m_pTimer = NULL;
	m_pTutorial = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	// 読み込み
	CModelManager::Load();
	CMotionManager::Load();
	CBg::Load();
	CLifeManager::Load();
	CWeed::Load();
	CNumber::Load();
	CPause::Load();

	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_GAME);

	// ステージ移動クラス
	CStage::Create(m_nCurStage);

	// タイマーの生成
	m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 10.0f, 0.0f), 0);

	// 操作説明の生成
	m_pTutorial = CLogoTutorial::Create(D3DXVECTOR3(1100.0f, 80.0f, 0.0f), 300.0f, 150.0f);

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

	// プレイヤーの生成
	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// データの破棄
	CModelManager::Unload();
	CMotionManager::Unload();
	CBg::Unload();
	CEnemy::Unload();
	CModelSetManager::Unload();
	CLifeManager::Unload();
	CWeed::Unload();
	CNumber::Unload();
	CPause::Unload();

	for (int nCntBg = 0; nCntBg < GAME_NUM_BG; nCntBg++)
	{
		if (m_pBg[nCntBg] != NULL)
		{// 背景の破棄
			m_pBg[nCntBg]->Uninit();
			m_pBg[nCntBg] = NULL;
		}
	}

	if (NULL != m_pTutorial)
	{// 操作説明表示クラスの破棄
		m_pTutorial->Uninit();
		m_pTutorial = NULL;
	}

	if (NULL != m_pMountain)
	{// 背景の破棄
		m_pMountain->Uninit();
		m_pMountain = NULL;
	}

	// オブジェクトを破棄
	CScene::Release();
}

//=============================================================================
// ゲーム更新処理
//=============================================================================
void CGame::Update(void)
{
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CXInput			*pXInput = CManager::GetXInput();			// XInputの取得
	CSound *pSound = CManager::GetSound();						// サウンドの取得

#ifdef _DEBUG
	if (CDebugProc::GetDisp())
	{
		if (pKeyboard->GetTrigger(DIK_1))
		{
			CStage::Create(0);
		}
		if (pKeyboard->GetTrigger(DIK_2))
		{
			CStage::Create(1);
		}
		if (pKeyboard->GetTrigger(DIK_3))
		{
			CGame::SetGameState(CGame::STATE_CLEAR);
		}
	}
#endif

	if (m_pBg[0] != NULL)
	{// テクスチャ座標を動かす
		m_pBg[0]->TexMove(0.0001f);
	}

	if (m_pBg[1] != NULL)
	{// テクスチャ座標を動かす
		m_pBg[1]->TexMove(0.0003f);
	}

	// フェードの取得
	CFade::FADE fade = CFade::GetFade();

	if (fade == CFade::FADE_NONE)
	{// フェード中はポーズにできなくする
		if (pKeyboard->GetTrigger(DIK_P) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_4))
		{// ポーズON/OFF
			m_bPause = m_bPause ? false : true;

			if (m_bPause)
			{// ポーズになったとき
				if (m_pPause == NULL)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_PAUSEON);
					m_pPause = CPause::Create();	// ポーズクラスを生成
				}
			}
		}

		if(!m_bPause)
		{// ポーズを閉じるとき
			if (m_pPause != NULL)
			{// ポーズでなくなったら破棄
				m_pPause->Uninit();
				m_pPause = NULL;
				pSound->PlaySound(CSound::SOUND_LABEL_PAUSEOFF);
			}
		}
	}

	switch (m_state)
	{
	case STATE_NORMAL:
		break;

	case STATE_END:
		m_modeCounter++;
		if (m_modeCounter >= 60)
		{
			m_state = STATE_NONE;

			// 時間の取得
			CResult::SetTime(m_pTimer->GetTime());

			// 画面(モード)の設定
			CFade::Create(CManager::MODE_GAMEOVER);
		}
		break;

	case STATE_CLEAR:
		m_modeCounter++;
		if (m_modeCounter >= 60)
		{
			m_state = STATE_NONE;

			// 時間の取得
			CResult::SetTime(m_pTimer->GetTime());

			// 画面(モード)の設定
			CFade::Create(CManager::MODE_RESULT);
		}
		break;
	}

	CDebugProc::Print("ゲーム");
	CDebugProc::Print("ステージ%d", m_nCurStage + 1);
}

//=============================================================================
// ゲーム描画処理
//=============================================================================
void CGame::Draw(void)
{
}

//=============================================================================
// ゲームの状態の設定
//=============================================================================
const void CGame::SetGameState(STATE state)
{
	m_state = state;
}

//=============================================================================
// ステージの設定
//=============================================================================
void CGame::SetStage(int nStage)
{
	// 現在のステージを設定
	m_nCurStage = nStage;
}

//=============================================================================
// 次のステージへ
//=============================================================================
void CGame::NextStage(void)
{
	// ステージをカウントアップ
	m_nCurStage++;

	if (CManager::GetNumStage() > m_nCurStage)
	{// ステージの生成
		CStage::Create(m_nCurStage);
	}
	else
	{// ゲームクリア
		SetGameState(STATE_CLEAR);
	}
}

//=============================================================================
// 山のテクスチャを動かす
//=============================================================================
void CGame::MoveMountain(float fMove)
{
	if (m_pMountain != NULL)
	{// テクスチャ座標を動かす
		m_pMountain->TexMove(fMove * 0.00002f);
	}
}