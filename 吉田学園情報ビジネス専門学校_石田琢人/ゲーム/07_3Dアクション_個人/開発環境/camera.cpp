//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "debugProc.h"
#include "player.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MOVE_HEIGHT	(100.0f)		// プレイヤーとの差で高さを動かす
#define CAMERA_HEIGHT_SPEED	(0.1f)			// 高さを動かす速度
#define CAMERA_ZOOM_LENGTH	(-500.0f)		// ズームしたときの距離
#define CAMERA_LENGTH		(-600.0f)		// カメラの距離
#define CAMERA_ZOOM			(5.0f)			// ズームする速度

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
	m_fLookHeight = 0.0f;

#ifdef _DEBUG
	m_bProj = false;
#endif
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 80.0f, CAMERA_LENGTH);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = m_posV.z;
	m_fLookHeight = m_posV.y;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
	// プレイヤーを探す
	CScene *pScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CPlayer *pPlayer = NULL;

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_PLAYER)
		{// プレイヤーだったとき
			pPlayer = (CPlayer*)pScene;

			// プレイヤーの位置と向き
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (pPlayer != NULL)
			{// プレイヤーの情報を取得
				pos = pPlayer->GetPos();
				rot = pPlayer->GetRot();
			}

			m_rot.y = D3DX_PI * 1.0f;

			if (m_bZoom)
			{// ズーム状態
				m_fLookHeight -= (m_posR.y - pos.y) * CAMERA_HEIGHT_SPEED;		// 高さの調整

				if (m_fLength <= CAMERA_ZOOM_LENGTH)
				{// カメラの距離を縮める
					m_fLength += CAMERA_ZOOM;
				}

				// 目的の注視点
				m_posRDest.x = pos.x + sinf(rot.y);
				m_posRDest.y = m_fLookHeight;

				// 目的の視点
				m_posVDest.x = pos.x + sinf(m_rot.y) * m_fLength;
				m_posVDest.y = m_fLookHeight;

				// 注視点
				m_posR.x += (m_posRDest.x - m_posR.x) * 1.0f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 1.0f;

				// 視点
				m_posV.x += (m_posVDest.x - m_posV.x) * 1.0f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 1.0f;
				m_posV.z = m_fLength;
			}
			else
			{// ズームでない
				m_fLookHeight = pos.y;

				if (m_fLength >= CAMERA_LENGTH)
				{// カメラの距離を離す
					m_fLength -= CAMERA_ZOOM;
				}

				// 目的の注視点
				m_posRDest.x = pos.x + sinf(rot.y);
				m_posRDest.y = m_fLookHeight;

				// 目的の視点
				m_posVDest.x = pos.x + sinf(m_rot.y) * m_fLength;
				m_posVDest.y = m_fLookHeight;

				// 注視点
				m_posR.x += (m_posRDest.x - m_posR.x) * 1.0f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 1.0f;

				// 視点
				m_posV.x += (m_posVDest.x - m_posV.x) * 1.0f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 1.0f;
				m_posV.z = m_fLength;
			}
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetRepeat(DIK_UP))
	{
		m_fLength += 5.0f;
	}
	else if (pKeyboard->GetRepeat(DIK_DOWN))
	{
		m_fLength -= 5.0f;
	}
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CCamera::Draw(void)
{
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
