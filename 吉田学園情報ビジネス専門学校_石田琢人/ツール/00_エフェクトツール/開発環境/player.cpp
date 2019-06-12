//=============================================================================
//
// プレイヤー処理 [Player.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "meshField.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MAXMAT	(1)
#define PLAYER_MAXMESH	(2)
#define PLAYER_SLOW		(0.4f)	// 慣性

#define GRAVITY		(1)			// 重力

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Player g_Player;		// プレイヤーの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	// 位置・向きの初期設定
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Player.angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;
	g_Player.fDestAngle = 0.0f;
	g_Player.fDiffAngle = 0.0f;
	g_Player.bJump = false;
	g_Player.fSpeed = 2.5f;			// 速度調整
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	Camera *pCamera;
	pCamera = GetCamera();

	float fRot = g_Player.fDiffAngle;
	D3DXVECTOR3 posOld;

	posOld = g_Player.pos;

	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//移動
	if (GetKeyboardPress(DIK_D) == true)
	{//右方向入力時
		if (GetKeyboardPress(DIK_S) == true) { g_Player.fDiffAngle = D3DX_PI * -0.25f + pCamera[0].rot.y; }
		else if (GetKeyboardPress(DIK_W) == true) { g_Player.fDiffAngle = D3DX_PI * -0.75f + pCamera[0].rot.y; }
		else { g_Player.fDiffAngle = D3DX_PI * -0.5f + pCamera[0].rot.y; }
		g_Player.move = D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -g_Player.fSpeed;
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{//左方向入力時
		if (GetKeyboardPress(DIK_S) == true) { g_Player.fDiffAngle = D3DX_PI * 0.25f + pCamera[0].rot.y; }
		else if (GetKeyboardPress(DIK_W) == true) { g_Player.fDiffAngle = D3DX_PI * 0.75f + pCamera[0].rot.y; }
		else { g_Player.fDiffAngle = D3DX_PI * 0.5f + pCamera[0].rot.y; }
		g_Player.move = D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -g_Player.fSpeed;
	}//上下単独入力
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_Player.fDiffAngle = D3DX_PI * 0.0f + pCamera[0].rot.y;
		g_Player.move = D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -g_Player.fSpeed;
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		g_Player.fDiffAngle = D3DX_PI * 1.0f + pCamera[0].rot.y;
		g_Player.move = D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -g_Player.fSpeed;
	}

	if (g_Player.rot.y > D3DX_PI)
	{
		g_Player.rot.y -= D3DX_PI * 2;
	}

	if (g_Player.rot.y < -D3DX_PI)
	{
		g_Player.rot.y += D3DX_PI * 2;
	}

	if (GetKeyboardPress(DIK_O) == true)
	{// 上昇
		g_Player.move.y = 10.0f;
	}
	if (GetKeyboardPress(DIK_L) == true)
	{// 下降
		g_Player.move.y = -10.0f;
	}


	//減速
	//g_Player.move.x += (0.0f - g_Player.move.x) * PLAYER_SLOW;
	//g_Player.move.z += (0.0f - g_Player.move.z) * PLAYER_SLOW;

	//g_Player.move.y -= GRAVITY;	// 重力加算

	//位置更新(移動)
	g_Player.pos += g_Player.move;

	// 地面についているかの判定
	if (CollisionField(&g_Player.pos, &posOld, &g_Player.move) == true)
	{
		g_Player.bJump = false;
	}
	else
	{
		g_Player.bJump = true;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
Player *GetPlayer(void)
{
	return &g_Player;
}