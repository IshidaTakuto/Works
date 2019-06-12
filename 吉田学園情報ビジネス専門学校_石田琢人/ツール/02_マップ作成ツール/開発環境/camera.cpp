//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "setPlayer.h"

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
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
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
	m_posV = D3DXVECTOR3(0.0f, 300.0f, -300.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_fLength = m_posV.z;
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
	CInputKeyboard *pKeyboard = CManager::GetInput();
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (CManager::GetSetPlayer() != NULL)
	{
		posPlayer = CManager::GetSetPlayer()->GetPos();
	}

	//===================================
	// 注視点をプレイヤーに追従ようにする
	//===================================
	m_posR.x = posPlayer.x;
	m_posR.y = posPlayer.y;
	m_posR.z = posPlayer.z;
	m_posV.x = posPlayer.x + sinf(m_fAngle) * m_fLength;
	m_posV.y = posPlayer.y + 300.0f;
	m_posV.z = posPlayer.z + cosf(m_fAngle) * m_fLength;

	if (pKeyboard->GetPress(DIK_NUMPAD8) == true)
	{// 距離を小さくする
		if (m_fLength < -20.0f)
		{
			m_fLength += 5.0f;
		}
	}
	else if (pKeyboard->GetPress(DIK_NUMPAD2) == true)
	{// 距離を大きくする
		//if (m_fLength > -1000.0f)
		{
			m_fLength -= 5.0f;
		}
	}
	if (pKeyboard->GetPress(DIK_NUMPAD4) == true)
	{// 距離を小さくする
		m_fAngle += 0.02f;
	}
	else if (pKeyboard->GetPress(DIK_NUMPAD6) == true)
	{// 距離を大きくする
		m_fAngle -= 0.02f;
	}

	if (pKeyboard->GetPress(DIK_NUMPAD1) == true)
	{// 距離を小さくする
		m_posV.y -= 5.0f;
	}
	else if (pKeyboard->GetPress(DIK_NUMPAD3) == true)
	{// 距離を大きくする
		m_posV.y += 5.0f;
	}

	if (pKeyboard->GetPress(DIK_NUMPAD0) == true)
	{// カメラリセット
		Init();
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
