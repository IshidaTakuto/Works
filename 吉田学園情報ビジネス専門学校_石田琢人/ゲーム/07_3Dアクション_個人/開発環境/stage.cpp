//=============================================================================
//
// ステージ管理処理 [stage.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "stage.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "scene2D.h"
#include "meshField.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "logo.h"
#include "modelSet.h"
#include "camera.h"

#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGE_FADE			(0.02f)		// フェードの透明度の増減値

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=========================================
// 生成処理
//=========================================
CStage *CStage::Create(int nStage)
{
	CStage *pStage = NULL;

	// メモリ確保
	pStage = new CStage();

	if (pStage != NULL)
	{// メモリ確保成功
		pStage->m_nStage = nStage;
		pStage->Init();
	}

	return pStage;
}

//=========================================
// コンストラクタ
//=========================================
CStage::CStage(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_nStage = 0;
	m_fade = FADE_NONE;
}

//=========================================
// デストラクタ
//=========================================
CStage::~CStage()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CStage::Init(void)
{
	m_fade = FADE_OUT;	// フェードアウト状態

	// 初期化処理
	CScene2D::Init();

	// 位置の設定
	CScene2D::SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 大きさの設定
	CScene2D::SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (0 == m_nStage)
	{// 初期ステージ
		CScene2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{// ２ステージ目以降
		CScene2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CStage::Uninit(void)
{
	// オブジェクトを破棄
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CStage::Update(void)
{
	bool bDelete = false;

	// 色の取得
	D3DXCOLOR col = CScene2D::GetColor();

	if (m_fade != FADE_NONE)
	{// フェードの状態が何もしていないときはスルー
		if (m_fade == FADE_IN)		// フェードイン状態
		{
			col.a -= STAGE_FADE;	// 画面を透明にしていく

			if (col.a <= 0.0f)
			{// 透明になった
				col.a = 0.0f;
				m_fade = FADE_NONE;	// 何もしていない状態

				bDelete = true;		// 死亡フラグを立てる
			}
		}
		else if (m_fade == FADE_OUT)
		{
			col.a += STAGE_FADE;	// 画面を不透明にしていく

			if (col.a >= 1.0f)
			{// 不透明になった
				col.a = 1.0f;
				Stage();			// ステージ移動
				m_fade = FADE_IN;	// フェードイン状態に
			}
		}
	}

	// 色の設定
	CScene2D::SetColor(col);

	if (bDelete)
	{// 破棄する
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CStage::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// ステージ移動
//=============================================================================
void CStage::Stage(void)
{
	// ステージ移動用破棄処理
	CScene::StageUninit();

	// 現在のステージを設定
	CGame::SetStage(m_nStage);

	// 地面の生成
	CMeshField::Create(m_nStage);

	CEnemy::Unload();
	CModelSetManager::Unload();
	CModelSetManager::Load(m_nStage);

	// プレイヤーオブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(PLAYER_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();		// 次のオブジェクトを保管
		CScene::OBJTYPE objType = pScene->GetObjType();		// オブジェクトの種類を取得

		if (objType == CScene::OBJTYPE_PLAYER)
		{// プレイヤーだったとき
			CPlayer *pPlayer = (CPlayer*)pScene;
			D3DXVECTOR3 posPlayer = pPlayer->GetPos();

			pPlayer->SetPos(CModelSetManager::GetStartPos());
			if (pPlayer->GetSlipFlag())
			{// 滑走状態のとき
				pPlayer->SetSlipFlag(false);
			}

			// カメラの設定
			D3DXVECTOR3 posV = CManager::GetCamera()->GetPosV();
			posV.y = posPlayer.y + 80.0f;
			CManager::GetCamera()->SetPosV(posV);

			D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();
			posR.y = posPlayer.y + 80.0f;
			CManager::GetCamera()->SetPosR(posR);

			break;
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}
}
