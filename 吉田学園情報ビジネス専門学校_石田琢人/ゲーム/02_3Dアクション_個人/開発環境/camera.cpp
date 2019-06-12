//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_ANGLE	(0.01f)
#define TITLE_MOVE	(0.001f)
#define MOVE_CAMERA (1.0f)
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
	g_Camera.posV = D3DXVECTOR3(0.0f, 180.0f, -50.0f);
	g_Camera.posR = D3DXVECTOR3(0.0f, 150.0f, 0.0f);
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.fLength = -300.0f;
	g_Camera.type = CAMERATYPE_TITLE;
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
	switch (g_Camera.type)
	{
	case CAMERATYPE_TITLE:
		//g_Camera.rot.y -= TITLE_MOVE;
		//if (g_Camera.rot.y < -D3DX_PI)
		//{
		//	g_Camera.rot.y = D3DX_PI;
		//}

		//g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fLength;
		//g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fLength;
		break;

	case CAMERATYPE_MENU:
		g_Camera.posR.y = 50.0f;
		break;

	case CAMERATYPE_GAME:
		Player *pPlayer;
		pPlayer = GetPlayer();

		if (pPlayer->pos.x <= 400)
		{// 最初の横スクロール
			g_Camera.posV.x = pPlayer->pos.x;
			g_Camera.posV.y = 50.0f;
			g_Camera.posV.z = -250.0f;
			g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * 1.0f;
			g_Camera.posRDest.y = pPlayer->pos.y;
			g_Camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * 1.0f;

			g_Camera.posVDest.x = pPlayer->pos.x + sinf(g_Camera.rot.y) * (g_Camera.fLength - (((pPlayer->move.x * pPlayer->move.x) + (pPlayer->move.z * pPlayer->move.z)) * 5.0f));
			g_Camera.posVDest.y = pPlayer->pos.y + 80.0f;
			g_Camera.posVDest.z = pPlayer->pos.z + cosf(g_Camera.rot.y) * (g_Camera.fLength - (((pPlayer->move.x * pPlayer->move.x) + (pPlayer->move.z * pPlayer->move.z)) * 5.0f));

			g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * 0.1f;
			g_Camera.posR.y += (g_Camera.posRDest.y - g_Camera.posR.y) * 0.1f;
			g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * 0.1f;

			g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * 0.15f;
			g_Camera.posV.y += (g_Camera.posVDest.y - g_Camera.posV.y) * 0.15f;
			g_Camera.posV.z += (g_Camera.posVDest.z - g_Camera.posV.z) * 0.15f;
		}
		else
		{// プレイヤーの背後にカメラをつける
			//g_Camera.rotDest.y = D3DX_PI + pPlayer->rot.y - g_Camera.rot.y;
			//if (g_Camera.rotDest.y > D3DX_PI)
			//{
			//	g_Camera.rotDest.y -= D3DX_PI * 2;
			//}
			//else if (g_Camera.rotDest.y < -D3DX_PI)
			//{
			//	g_Camera.rotDest.y += D3DX_PI * 2;
			//}

			//g_Camera.rot.y += g_Camera.rotDest.y * MOVE_ANGLE;

			g_Camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * 1.0f;
			g_Camera.posRDest.y = pPlayer->pos.y;
			g_Camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * 1.0f;

			g_Camera.posVDest.x = pPlayer->pos.x + (g_Camera.fLength + (pPlayer->move.x * 5.0f));
			g_Camera.posVDest.y = 80.0f;
			g_Camera.posVDest.z = 0;

			g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * 0.1f;
			g_Camera.posR.y += (g_Camera.posRDest.y - g_Camera.posR.y) * 0.1f;
			g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * 0.1f;

			g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * 0.15f;
			g_Camera.posV.y += (g_Camera.posVDest.y - g_Camera.posV.y) * 0.15f;
			g_Camera.posV.z += (g_Camera.posVDest.z - g_Camera.posV.z) * 0.15f;
		}


		break;

	case CAMERATYPE_RESULT:
		g_Camera.posR.y = 50.0f;

		break;
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
								D3DXToRadian(45.0f),							// 画角
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// アスペクト比
								10.0f,											// カメラの範囲（手前）
								4000.0f);										// カメラの範囲（奥行）

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

Camera *GetCamera(void)
{
	return &g_Camera;
}