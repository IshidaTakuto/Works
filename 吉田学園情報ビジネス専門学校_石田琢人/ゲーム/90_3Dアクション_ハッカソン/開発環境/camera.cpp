//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// カメラ処理 [camera.cpp]
// Author :
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "camera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_DRAW_DISTANCE			(5.0f)		// カメラの描画できる距離の倍率
#define CAMERA_POSVX_DEFAULT			(0.0f)
#define CAMERA_POSVY_DEFAULT			(200.0f)
#define CAMERA_POSVZ_DEFAULT			(-300.0f)
#define CAMERA_POSRX_DEFAULT			(0.0f)
#define CAMERA_POSRY_DEFAULT			(0.0f)
#define CAMERA_POSRZ_DEFAULT			(0.0f)
#define CAMERA_SPEED					(1.0f)		// カメラの移動速度
#define TURNING_SPEED					(0.2f)		// カメラの旋回速度
#define CAMERA_POSR_PLAYER_DIRECTION	(30.0f)		// 注視点とプレイヤーの位置の間隔
#define CAMERA_POSR_COMPLIANCESPEED		(0.1f)		// 注視点の慣性の割合
#define CAMERA_POSV_COMPLIANCESPEED		(0.2f)		// 視点の慣性の割合
#define CAMERA_UPDATE_STANDARDSPEED		(0.01f)		// カメラが回り込み開始するプレイヤーの速度の基準


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Camera g_camera;		// カメラの情報

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// カメラの初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(CAMERA_POSVX_DEFAULT, CAMERA_POSVY_DEFAULT, CAMERA_POSVZ_DEFAULT);		// 初期位置 [視点]
	g_camera.posR = D3DXVECTOR3(CAMERA_POSRX_DEFAULT, CAMERA_POSRY_DEFAULT, CAMERA_POSRZ_DEFAULT);		// 初期位置 [注視点]
	g_camera.posVDest = D3DXVECTOR3(CAMERA_POSVX_DEFAULT, CAMERA_POSVY_DEFAULT, CAMERA_POSVZ_DEFAULT);
	g_camera.posRDest = D3DXVECTOR3(CAMERA_POSRX_DEFAULT, CAMERA_POSRY_DEFAULT, CAMERA_POSRZ_DEFAULT);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き [現在]
	g_camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き [目的]
	g_camera.fLength = g_camera.posR.z - g_camera.posV.z;		// 視点と注視点の距離
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// カメラの終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitCamera(void)
{
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// カメラの更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdateCamera(void)
{
	// ローカル変数
	D3DXVECTOR3	rotDivision;						// 回り込み処理に必要
	D3DXVECTOR3 posVDirection, posRDirection;		// 追従処理に必要
	int nLength = 0;									// カメラの距離に必要

	MODE pMode = GetMode();
	if (pMode == MODE_GAME || pMode == MODE_TUTORIAL)
	{

		// プレイヤーの位置を取得
		Player pPlayer = *GetPlayer();

		//++++++++++++++++++++++++++++++++++++++++
		// カメラの処理
		//++++++++++++++++++++++++++++++++++++++++
		// =====  カメラ回り込み処理  ===== //
		// 回り込む場所を設定 [プレイヤーの後ろ]
		g_camera.rotDest = pPlayer.rot;

		// 差分
		rotDivision.y = g_camera.rotDest.y - g_camera.rot.y;

		if (rotDivision.y > D3DX_PI)
		{
			rotDivision.y += -D3DX_PI * 2;
		}
		else if (rotDivision.y < -D3DX_PI)
		{
			rotDivision.y += D3DX_PI * 2;
		}

		// 向きの更新
		g_camera.rot.y += rotDivision.y * TURNING_SPEED;

		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y += -D3DX_PI * 2;
		}
		else if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2;
		}

		// =====  カメラ追従処理  ===== //
		// -----  注視点  ----- //
		// プレイヤーの位置を基準に注視点の位置 [目的]を決める
		g_camera.posRDest.x = pPlayer.pos.x + sinf(-pPlayer.rot.y) * CAMERA_POSR_PLAYER_DIRECTION;
		g_camera.posRDest.y = pPlayer.pos.y;
		g_camera.posRDest.z = pPlayer.pos.z + cosf(-pPlayer.rot.y + D3DX_PI) * CAMERA_POSR_PLAYER_DIRECTION;

		// 差分
		posRDirection = D3DXVECTOR3(g_camera.posRDest.x - g_camera.posR.x, g_camera.posRDest.y - g_camera.posR.y, g_camera.posRDest.z - g_camera.posR.z);

		// 位置の更新
		g_camera.posR += posRDirection * CAMERA_POSR_COMPLIANCESPEED;

		// -----  視点  ----- //
		// 注視点を元に視点を追従させる
		g_camera.posVDest.x = pPlayer.pos.x + sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posVDest.y = pPlayer.pos.y + CAMERA_POSVY_DEFAULT;
		g_camera.posVDest.z = pPlayer.pos.z + cosf(g_camera.rot.y) * g_camera.fLength;

		// 差分
		posVDirection = D3DXVECTOR3(g_camera.posVDest.x - g_camera.posV.x, g_camera.posVDest.y - g_camera.posV.y, g_camera.posVDest.z - g_camera.posV.z);

		// 位置の更新
		g_camera.posV += posVDirection * CAMERA_POSV_COMPLIANCESPEED;

#if 0
		// =====  カメラ視点旋回処理  ===== //
		if (GetKeyboardPress(DIK_Z) == true)
		{// 左に旋回
			g_camera.rot.y += TURNING_SPEED;

			if (g_camera.rot.y > D3DX_PI)
			{
				g_camera.rot.y -= D3DX_PI * 2;
			}
			g_camera.posVDest.x = g_camera.posR.x + sinf(g_camera.rot.y) * g_camera.fLength;
			g_camera.posVDest.z = g_camera.posR.z + cosf(g_camera.rot.y) * g_camera.fLength;
		}
		else if (GetKeyboardPress(DIK_C) == true)
		{// 右に旋回
			g_camera.rot.y -= TURNING_SPEED;

			if (g_camera.rot.y < -D3DX_PI)
			{
				g_camera.rot.y += D3DX_PI * 2;
			}
			g_camera.posVDest.x = g_camera.posR.x + sinf(g_camera.rot.y) * g_camera.fLength;
			g_camera.posVDest.z = g_camera.posR.z + cosf(g_camera.rot.y) * g_camera.fLength;
		}

		if (pGamePad->lZ > GAMEPAD_DEADZONE || pGamePad->lZ < -GAMEPAD_DEADZONE)
		{// コントローラーVer.
			g_camera.rot.y += pGamePad->lZ * 0.01f;

			if (g_camera.rot.y < -D3DX_PI)
			{
				g_camera.rot.y += D3DX_PI * 2;
			}
			g_camera.posVDest.x = g_camera.posR.x + sinf(g_camera.rot.y) * g_camera.fLength;
			g_camera.posVDest.z = g_camera.posR.z + cosf(g_camera.rot.y) * g_camera.fLength;
		}
#endif
#if 0
		//++++++++++++++++++++++++++++++++++++++++
		// 移動処理 [キー入力]
		//++++++++++++++++++++++++++++++++++++++++
		// -----  視点＆注視点 [X&Z方向移動]  ----- //
		if (GetKeyboardPress(DIK_A) == true)
		{// キー(A)が押された場合(左移動)
			// 視点
			if (GetKeyboardPress(DIK_W) == true)
			{// 同時に(W)が押された(上)
				g_camera.posV.x += sinf((D3DX_PI * 0.75f) + g_camera.rot.y) * CAMERA_SPEED;
				g_camera.posV.z += cosf((D3DX_PI * 0.75f) + g_camera.rot.y) * CAMERA_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{// 同時に(S)が押された(下)
				g_camera.posV.x += sinf((D3DX_PI * 0.25f) + g_camera.rot.y) * CAMERA_SPEED;
				g_camera.posV.z += cosf((D3DX_PI * 0.25f) + g_camera.rot.y) * CAMERA_SPEED;
			}
			else
			{// (A)のみ押された
				g_camera.posV.x += sinf((D3DX_PI * 0.5f) + g_camera.rot.y) * CAMERA_SPEED;
				g_camera.posV.z += cosf((D3DX_PI * 0.5f) + g_camera.rot.y) * CAMERA_SPEED;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{// キー(D)が押された場合(右移動)
			// 視点
			if (GetKeyboardPress(DIK_W) == true)
			{// 同時に(W)が押された(上)
				g_camera.posV.x += sinf((-D3DX_PI * 0.75f) + g_camera.rot.y) * CAMERA_SPEED;
				g_camera.posV.z += cosf((-D3DX_PI * 0.75f) + g_camera.rot.y) * CAMERA_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{// 同時に(S)が押された(下)
				g_camera.posV.x += sinf((-D3DX_PI * 0.25f) + g_camera.rot.y) * CAMERA_SPEED;
				g_camera.posV.z += cosf((-D3DX_PI * 0.25f) + g_camera.rot.y) * CAMERA_SPEED;
			}
			else
			{// (D)のみ押された
				g_camera.posV.x += sinf((-D3DX_PI * 0.5f) + g_camera.rot.y) * CAMERA_SPEED;
				g_camera.posV.z += cosf((-D3DX_PI * 0.5f) + g_camera.rot.y) * CAMERA_SPEED;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{// キー(W)が押された場合(上移動)
			// 視点
			g_camera.posV.x += sinf((D3DX_PI * 1.0f) + g_camera.rot.y) * CAMERA_SPEED;
			g_camera.posV.z += cosf((D3DX_PI * 1.0f) + g_camera.rot.y) * CAMERA_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{// キー(S)が押された場合(下移動)
			// 視点
			g_camera.posV.x += sinf((D3DX_PI * 0.0f) + g_camera.rot.y) * CAMERA_SPEED;
			g_camera.posV.z += cosf((D3DX_PI * 0.0f) + g_camera.rot.y) * CAMERA_SPEED;
		}
		// 注視点
		g_camera.posR.x = g_camera.posV.x - sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posR.z = g_camera.posV.z - cosf(g_camera.rot.y) * g_camera.fLength;
		// -----  注視点旋回  ----- //
		if (GetKeyboardPress(DIK_Q) == true)
		{// 左に旋回
			g_camera.rot.y -= TURNING_SPEED;

			if (g_camera.rot.y < -D3DX_PI)
			{
				g_camera.rot.y += D3DX_PI * 2;
			}
			g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y - D3DX_PI) * g_camera.fLength;
			g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y - D3DX_PI) * g_camera.fLength;
		}
		else if (GetKeyboardPress(DIK_E) == true)
		{// 右に旋回
			g_camera.rot.y += TURNING_SPEED;

			if (g_camera.rot.y > D3DX_PI)
			{
				g_camera.rot.y -= D3DX_PI * 2;
			}
			g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y - D3DX_PI) * g_camera.fLength;
			g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y - D3DX_PI) * g_camera.fLength;
		}

		// ----- 視点 [Y方向移動]  ----- //
		if (GetKeyboardPress(DIK_T) == true)
		{
			g_camera.posV.y += CAMERA_SPEED;
		}
		else if (GetKeyboardPress(DIK_B) == true)
		{
			g_camera.posV.y += -CAMERA_SPEED;
		}

		// ----- 注視点 [Y方向移動]  ----- //
		if (GetKeyboardPress(DIK_Y) == true)
		{
			g_camera.posR.y += -CAMERA_SPEED;
		}
		else if (GetKeyboardPress(DIK_N) == true)
		{
			g_camera.posR.y += CAMERA_SPEED;
		}
#endif

		// =====  カメラの距離変更  ===== //

		// =====  カメラのリセット  ===== //

	}
	else if (pMode == MODE_TITLE)
	{
		g_camera.posR.z += 1.0f;
		g_camera.posV.z += 1.0f;
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// カメラの設定処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 1000.0f * CAMERA_DRAW_DISTANCE);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_camera.mtxView, &g_camera.posV, &g_camera.posR, &g_camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// カメラの取得
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Camera *GetCamera(void)
{
	return &g_camera;
}