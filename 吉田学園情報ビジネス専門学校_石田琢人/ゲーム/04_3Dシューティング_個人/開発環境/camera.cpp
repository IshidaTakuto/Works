//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "player.h"
#include "game.h"
#include "scene3DIndex.h"
#include "meshCylinder.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_ANGLE	(0.01f)
#define TITLE_MOVE	(0.001f)
#define MOVE_CAMERA (1.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// カメラの初期化処理
//=============================================================================
void CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 245.0f, -500.0f);
	m_posR = D3DXVECTOR3(0.0f, 200.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 300.0f;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void)
{
	// シリンダーをカメラの位置に合わせて動かす
	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		CScene3DIndex *pSceneIdx;					// オブジェクトのポインタ変数
		pSceneIdx = (CScene3DIndex*)CScene::GetScene(CYLINDER_PRIORITY, nCntScene);		// インデックス順に取得

		if (pSceneIdx != NULL)
		{// 使われているかのチェック
			CScene::OBJTYPE objType;					// オブジェクトの種類判別用の変数
			objType = pSceneIdx->GetObjType();			// オブジェクトの種類を取得

			if (objType == CScene::OBJTYPE_CYLINDER)
			{// オブジェクトの種類がプレイヤーのとき
				CCylinder *pCylinder = (CCylinder*)CScene::GetScene(CYLINDER_PRIORITY, nCntScene);

				// シリンダーの位置更新
				pCylinder->SetPos(D3DXVECTOR3(m_posV.x, -1000.0f, 0.0f));
				break;
			}
		}
	}

	CManager::MODE mode = CManager::GetMode();

	switch (mode)
	{
	case CManager::MODE_TITLE:
		// カメラの向きを変える
		m_rot.y = D3DX_PI * -0.5f;

		// X軸は常に動かす
		m_posV.x += 1.0f;
		m_posR.x = m_posV.x + 100;
		m_posV.y = 100.0f;
		m_posV.z = 0.0f;

		if (m_posV.x > 1000)
		{// 一定まで進んだら
			if (m_posR.y < m_posV.y)
			{// 一定までカメラの注視点を上げる
				m_posR.y += 5;
			}
			else if (m_posR.y >= m_posV.y)
			{// 値が超えてしまったとき
				m_posR.y = m_posV.y;
			}
		}
		else
		{
			m_posR.y = 0.0f;
		}
		break;

	case CManager::MODE_RESULT:
		// カメラの向きを変える
		m_rot.y = 0.0f;

		m_posV.x += 2.0f;
		m_posR = D3DXVECTOR3(m_posV.x, m_posV.y, 0.0f);

		break;

	case CManager::MODE_GAMEOVER:
		// カメラの向きを変える
		m_rot.y = 0.0f;

		m_posR = D3DXVECTOR3(0.0f, 0.0f, 10.0f);
		m_posV = D3DXVECTOR3(0.0f, 50.0f, -100.0f);

		break;

	case CManager::MODE_RANKING:
		// カメラの向きを変える
		m_rot.y = D3DX_PI * -0.5f;

		// X軸は常に動かす
		m_posV.x += 1.0f;
		m_posV.y = 100.0f;
		m_posV.z = 0.0f;
		m_posR.x = m_posV.x + 100;
		m_posR.y = m_posV.y;
		m_posR.z = 0.0f;

		break;
	}

	CPlayer *pPlayer = CGame::GetPlayer();	// プレイヤーの情報を取得

	if (NULL != pPlayer)
	{// プレイヤーの情報の取得
		D3DXVECTOR3 pos = pPlayer->GetPos();
		D3DXVECTOR3 rot = pPlayer->GetRot();
		CPlayer::TYPE type = pPlayer->GetType();

		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

		switch (type)
		{
		case CPlayer::TYPE_AIRCRAFT:
			if (m_posV.z > -500)
			{// Z座標が決めた位置に行くまで動かす
				m_posV.z -= 5;
			}
			if (m_posV.z < -490)
			{// Z座標が決めた位置に行くまで動かす
				m_posV.z += 5;
			}

			if (m_posV.y < 200)
			{// Y座標が決めた位置に行くまで動かす
				m_posV.y += 5;
			}
			if (m_posV.y > 245)
			{// Y座標が決めた位置に行くまで動かす
				m_posV.y -= 5;
			}

			// カメラの向きを変える
			m_rot.y = 0.0f;

			// X軸は常に動かす
			m_posV.x -= sinf((D3DX_PI * -0.5f) + m_rot.y) * 1.1f;
			m_posR.x = m_posV.x - sinf(m_rot.y);

			m_posR.y = 200.0f;
			m_posR.z = 0;

			m_fLength = 300.0f;
			break;

		case CPlayer::TYPE_SHIP:
			if (m_posV.y <= 745)
			{// Y座標が決めた位置に行くまで動かす
				m_posV.y += 5;
			}

			// カメラの向きを変える
			m_rot.y = D3DX_PI * -0.5f;

			m_posR.y = 0;
			m_posV.z = pos.z;
			m_posR.z = pos.z;

			// X軸は常に動かす
			m_posV.x += 1.0f;
			m_posR.x = m_posV.x - sinf(m_rot.y);
			break;

		case CPlayer::TYPE_TANK:
			// カメラの向きを変える
			m_rot.y = D3DX_PI * -0.5f;

			m_posRDest.x = pos.x + sinf(rot.y) * 1.0f;
			m_posRDest.y = pos.y + 30;
			m_posRDest.z = pos.z + cosf(rot.y) * 1.0f;

			m_posVDest.x = pos.x + sinf(m_rot.y) * (m_fLength * 0.7f);
			m_posVDest.y = pos.y + 60.0f;
			m_posVDest.z = pos.z + cosf(m_rot.y) * (m_fLength * 0.7f);

			m_posR.x += (m_posRDest.x - m_posR.x) * 0.1f;
			m_posR.y += (m_posRDest.y - m_posR.y) * 0.1f;
			m_posR.z += (m_posRDest.z - m_posR.z) * 0.1f;

			m_posV.x += (m_posVDest.x - m_posV.x) * 0.15f;
			m_posV.y += (m_posVDest.y - m_posV.y) * 0.15f;
			m_posV.z += (m_posVDest.z - m_posV.z) * 0.15f;

			break;
		}
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CCamera::Set(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();		// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),							// 画角
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// アスペクト比
								10.0f,											// カメラの範囲（手前）
								8000.0f);										// カメラの範囲（奥行）

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}
