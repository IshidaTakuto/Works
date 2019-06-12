//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_ANGLE	(0.01f)
#define TITLE_MOVE	(0.001f)
#define MOVE_CAMERA (160.0f)
#define MOVE_START	(90)	// 動くまでの時間

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Camera g_Camera;		// カメラの情報
int nCntAngle;			// カメラが動くまでのカウンタ

//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	g_Camera.posV = D3DXVECTOR3(0.0f, 150.0f, -200.0f);
	g_Camera.posR = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.fLength = -300.0f;
	g_Camera.type = CAMERATYPE_NONE;
	g_Camera.fAngle = 0.0f;
	nCntAngle = 0;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	Player *pPlayer;
	pPlayer = GetPlayer();

	bool bOn = false;

#if 1
	switch (g_Camera.type)
	{
	case CAMERATYPE_TITLE:

		pPlayer->pos = D3DXVECTOR3(60.0f, 0.0f, 0.0f);
		pPlayer->rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.8f, 0.0f);

		g_Camera.posV = D3DXVECTOR3(120.0f, 50.0f, -50.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 70.0f, 0.0f);
		g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		break;

	case CAMERATYPE_GAME:

		//if (bOn == false)
	{
		g_Camera.posV = D3DXVECTOR3(0.0f, 150.0f, -200.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
		g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//bOn = true;
	}

	if (GetJoyPadPress(DIJS_BUTTON_17, 0) == true)
	{// カメラの移動
		g_Camera.fAngle -= 0.02f;
	}
	else if (GetJoyPadPress(DIJS_BUTTON_16, 0) == true)
	{
		g_Camera.fAngle += 0.02f;
	}

	//===================================
	//注視点をプレイヤーに追従ようにする
	//===================================
	g_Camera.posR.x = pPlayer->pos.x;
	g_Camera.posR.z = pPlayer->pos.z;
	//視点をプレイヤーに追従ようにする
	g_Camera.posV.x = pPlayer->pos.x - sinf(g_Camera.fAngle)* MOVE_CAMERA;
	g_Camera.posV.z = pPlayer->pos.z - cosf(g_Camera.fAngle)* MOVE_CAMERA;
	break;

	case CAMERATYPE_RANKING:

		break;
	}
#endif
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(CAMERATYPE cameratype)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_Camera.type = cameratype;

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
								D3DXToRadian(45.0f),							// 画角
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// アスペクト比
								10.0f,											// カメラの範囲（手前）
								3000.0f);										// カメラの範囲（奥行）

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_Camera.mtxView,
		&g_Camera.posV,
		&g_Camera.posR,
		&g_Camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}
void DeleteCamera(void)
{
	g_Camera.posV = D3DXVECTOR3(0.0f, 150.0f, -200.0f);
	g_Camera.posR = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.fLength = -300.0f;
	g_Camera.fAngle = 0.0f;
	g_Camera.type = CAMERATYPE_NONE;
	nCntAngle = 0;
}

Camera *GetCamera(void)
{
	return &g_Camera;
}