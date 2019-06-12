//=============================================================================
//
// プレイヤー処理 [Player.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "effect.h"
#include "meshWall.h"
#include "particle.h"
#include "model.h"
#include "meshField.h"
#include "spiral.h"
#include "magic.h"
#include "joypad.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYERNAME00	"data/MODEL/player00.x"			// 読み込むxファイル名
#define PLAYERNAME01	"data/MODEL/player01.x"			// 読み込むxファイル名

#define PLAYER_MAXMAT	(1)
#define PLAYER_MAXMESH	(2)

#define PLAYER_SLOW	(0.4f)
#define GRAVITY		(1)
#define MOVE_SPEED	(2.5f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Player g_Player;						// プレイヤーの情報
D3DXCOLOR g_colPlayer;

//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・向きの初期設定
	g_Player.pos = D3DXVECTOR3(-1000.0f, 0.0f, 0.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Player.fDestAngle = 0.0f;
	g_Player.nCntParticle = 0;
	g_Player.state = PLAYERSTATE_2D;

	g_colPlayer = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// パーツのオフセット
	g_Player.aModel[0].nIdxModelParent = -1;
	g_Player.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.aModel[1].nIdxModelParent = 0;
	g_Player.aModel[1].pos = D3DXVECTOR3(0.0f, 40.0f, 0.0f);

		// Xファイルの読み込み
		D3DXLoadMeshFromX(PLAYERNAME00,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Player.aModel[0].pBuffMat,
			NULL,
			&g_Player.aModel[0].nNumMat,
			&g_Player.aModel[0].pMesh);

		// Xファイルの読み込み
		D3DXLoadMeshFromX(PLAYERNAME01,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Player.aModel[1].pBuffMat,
			NULL,
			&g_Player.aModel[1].nNumMat,
			&g_Player.aModel[1].pMesh);

	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファへのポインタ

	for (int nCntPlayer = 0; nCntPlayer < PLAYER_MAXMESH; nCntPlayer++)
	{
		// 頂点数を取得
		nNumVtx = g_Player.aModel[nCntPlayer].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Player.aModel[nCntPlayer].pMesh->GetFVF());

		// 頂点バッファをロック
		g_Player.aModel[nCntPlayer].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{// 頂点座標の比較
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

			// Xの値の比較
			if (g_Player.vtxMin.x > vtx.x)
			{// 最小値と比較
				g_Player.vtxMin.x = vtx.x;	// 最小値より小さければ代入
			}

			if (g_Player.vtxMax.x < vtx.x)
			{// 最大値と比較
				g_Player.vtxMax.x = vtx.x;	// 最大値より大きければ代入
			}

			// Yの値の比較
			if (g_Player.vtxMin.y > vtx.y)
			{// 最小値と比較
				g_Player.vtxMin.y = vtx.y;	// 最小値より小さければ代入
			}

			if (g_Player.vtxMax.y < vtx.y)
			{// 最大値と比較
				g_Player.vtxMax.y = vtx.y;	// 最大値より大きければ代入
			}

			// Zの値の比較
			if (g_Player.vtxMin.z > vtx.z)
			{// 最小値と比較
				g_Player.vtxMin.z = vtx.z;	// 最小値より小さければ代入
			}

			if (g_Player.vtxMax.z < vtx.z)
			{// 最大値と比較
				g_Player.vtxMax.z = vtx.z;	// 最大値より大きければ代入
			}

			pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
		}

		// 頂点バッファをアンロック
		g_Player.aModel[nCntPlayer].pMesh->UnlockVertexBuffer();
	}

	// 影を設定
	g_Player.nIdxShadow = SetShadow(g_Player.pos, g_Player.rot, 15.0f, 15.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// メッシュの開放
	for (int nCntMesh = 0; nCntMesh < PLAYER_MAXMESH; nCntMesh++)
	{
		if (g_Player.aModel[nCntMesh].pMesh != NULL)
		{
			g_Player.aModel[nCntMesh].pMesh->Release();
			g_Player.aModel[nCntMesh].pMesh = NULL;
		}
	}

	// マテリアルの開放
	for (int nCntMat = 0; nCntMat < PLAYER_MAXMAT; nCntMat++)
	{
		if (g_Player.aModel[nCntMat].pBuffMat != NULL)
		{
			g_Player.aModel[nCntMat].pBuffMat->Release();
			g_Player.aModel[nCntMat].pBuffMat = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	Camera *pCamera;
	pCamera = GetCamera();

	XINPUT_STATE state;
	state = GetJoyPad();

	float fRot = g_Player.fDiffAngle;

	g_Player.posOld = g_Player.pos;

	switch (g_Player.state)
	{
	case PLAYERSTATE_NONE:
		break;

	case PLAYERSTATE_NORMAL:
		// キー入力による移動---------------------------------------------------------------------------------------------
		if (GetKeyboardPress(DIK_A) == true)
		{// ←方向へ移動
			if (GetKeyboardPress(DIK_W) == true)
			{// 左上に移動
				g_Player.move.x += sinf(D3DX_PI * 0.25) * MOVE_SPEED;
				g_Player.move.z += cosf(D3DX_PI * 0.25) * MOVE_SPEED;
				g_Player.fDiffAngle = D3DX_PI * -0.75f + pCamera[0].rot.y;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{// 左下に移動
				g_Player.move.x += sinf(-D3DX_PI * 0.25) * MOVE_SPEED;
				g_Player.move.z += cosf(-D3DX_PI * 0.25) * MOVE_SPEED;
				g_Player.fDiffAngle = D3DX_PI * 0.75f + pCamera[0].rot.y;
			}
			else
			{//左に移動
				g_Player.move.z += MOVE_SPEED;	//ポリゴンを左に移動
				g_Player.fDiffAngle = D3DX_PI * 1.0f + pCamera[0].rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)	//-------------------------- → ------------------------------
		{// →方向へ移動
			if (GetKeyboardPress(DIK_W) == true)
			{// 右上に移動
				g_Player.move.x += sinf(D3DX_PI * 0.75f) * MOVE_SPEED;
				g_Player.move.z += cosf(D3DX_PI * 0.75f) * MOVE_SPEED;
				g_Player.fDiffAngle = D3DX_PI * -0.25f + pCamera[0].rot.y;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{// 右下に移動
				g_Player.move.x += sinf(-D3DX_PI * 0.75f) * MOVE_SPEED;
				g_Player.move.z += cosf(-D3DX_PI * 0.75f) * MOVE_SPEED;
				g_Player.fDiffAngle = D3DX_PI * 0.25f + pCamera[0].rot.y;
			}
			else
			{//右に移動
				g_Player.move.z -= MOVE_SPEED;	//ポリゴンを右に移動
				g_Player.fDiffAngle = D3DX_PI * 0.0f + pCamera[0].rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true)	//-------------------------- ↑ ------------------------------
		{// 上に移動
			g_Player.move.x += MOVE_SPEED;
			g_Player.fDiffAngle = D3DX_PI * -0.5f + pCamera[0].rot.y;
		}
		else if (GetKeyboardPress(DIK_S) == true)	//-------------------------- ↓ ------------------------------
		{// 下に移動
			g_Player.move.x -= MOVE_SPEED;
			g_Player.fDiffAngle = D3DX_PI * 0.5f + pCamera[0].rot.y;
		}

		//移動
		if (state.Gamepad.sThumbLX > 0.0f)
		{// 左方向入力時
			if (state.Gamepad.sThumbLY < 0.0f)
			{
				g_Player.move.x += sinf(-D3DX_PI * 0.25) * MOVE_SPEED;
				g_Player.move.z += cosf(-D3DX_PI * 0.25) * MOVE_SPEED;
				g_Player.fDiffAngle = D3DX_PI * 0.75f + pCamera[0].rot.y;
			}
			else if (state.Gamepad.sThumbLY > 0.0f)
			{
				g_Player.move.x += sinf(D3DX_PI * 0.25) * MOVE_SPEED;
				g_Player.move.z += cosf(D3DX_PI * 0.25) * MOVE_SPEED;
				g_Player.fDiffAngle = D3DX_PI * -0.75f + pCamera[0].rot.y;
			}
			else
			{
				g_Player.move.z += MOVE_SPEED;	//ポリゴンを左に移動
				g_Player.fDiffAngle = D3DX_PI * 1.0f + pCamera[0].rot.y;
			}
		}
		else if (state.Gamepad.sThumbLX < 0.0f)
		{// 右方向入力時
			if (state.Gamepad.sThumbLY < 0.0f)
			{
				g_Player.move.x += sinf(-D3DX_PI * 0.75f) * MOVE_SPEED;
				g_Player.move.z += cosf(-D3DX_PI * 0.75f) * MOVE_SPEED;
				g_Player.fDiffAngle = D3DX_PI * 0.25f + pCamera[0].rot.y;
			}
			else if (state.Gamepad.sThumbLY > 0.0f)
			{
				g_Player.move.x += sinf(D3DX_PI * 0.75f) * MOVE_SPEED;
				g_Player.move.z += cosf(D3DX_PI * 0.75f) * MOVE_SPEED;
				g_Player.fDiffAngle = D3DX_PI * -0.25f + pCamera[0].rot.y;
			}
			else
			{
				g_Player.move.z -= MOVE_SPEED;	//ポリゴンを右に移動
				g_Player.fDiffAngle = D3DX_PI * 0.0f + pCamera[0].rot.y;
			}
		}//上下単独入力
		else if (state.Gamepad.sThumbLY < 0.0f)
		{// 下
			g_Player.move.x -= MOVE_SPEED;
			g_Player.fDiffAngle = D3DX_PI * 0.5f + pCamera[0].rot.y;
		}
		else if (state.Gamepad.sThumbLY > 0.0f)
		{// 上
			g_Player.move.x += MOVE_SPEED;
			g_Player.fDiffAngle = D3DX_PI * -0.5f + pCamera[0].rot.y;
		}

		break;

	case PLAYERSTATE_2D:
		//移動
		if (GetKeyboardPress(DIK_D) == true)
		{//右方向入力時
			g_Player.fDiffAngle = D3DX_PI * -0.5f + pCamera[0].rot.y;
			g_Player.move += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//左方向入力時
			g_Player.fDiffAngle = D3DX_PI * 0.5f + pCamera[0].rot.y;
			g_Player.move += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -MOVE_SPEED;
		}

		//移動
		if (state.Gamepad.sThumbLX > 0.0f)
		{// 左方向入力時
			g_Player.fDiffAngle = D3DX_PI * -0.5f + pCamera[0].rot.y;
			g_Player.move += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -MOVE_SPEED;
		}
		else if (state.Gamepad.sThumbLX < 0.0f)
		{// 右方向入力時
			g_Player.fDiffAngle = D3DX_PI * 0.5f + pCamera[0].rot.y;
			g_Player.move += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -MOVE_SPEED;
		}

		if (g_Player.pos.x > 400)
		{// x軸が一定まで行くと3Dにする
			g_Player.state = PLAYERSTATE_NORMAL;
		}

		break;

	case PLAYERSTATE_DAETH:
		break;

	case PLAYERSTATE_CLEAR:
		g_Player.nCntState++;
		if (g_Player.nCntState >= 60)
		{
			g_Player.state = PLAYERSTATE_NONE;

			SetGameState(GAMESTATE_CLEAR);
		}
		break;
	}

	// 移動時のエフェクト
	if (false == g_Player.bJump)
	{// 地面についてるとき
		if (g_Player.pos.x < 1250)
		{// 春エリア
			if (g_Player.move.x > 0.05f || g_Player.move.x < -0.05f || g_Player.move.z > 0.05f || g_Player.move.z < -0.05f)
			{
				SetParticle(g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.2f, 20, PARTICLETYPE_FLOWERS);
			}
		}
		else if (g_Player.pos.x < 3750)
		{// 夏エリア
			if (g_Player.move.x > 0.05f || g_Player.move.x < -0.05f || g_Player.move.z > 0.05f || g_Player.move.z < -0.05f)
			{
				SetEffect(g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5.0f, 0, EFFECTSTATE_RIPPLES);
			}
		}
		else if (g_Player.pos.x <= 6250)
		{// 秋エリア
			if (g_Player.move.x > 0.05f || g_Player.move.x < -0.05f || g_Player.move.z > 0.05f || g_Player.move.z < -0.05f)
			{
				SetParticle(g_Player.pos, D3DXCOLOR(1.0f, rand() % 10 + 0.1f, 0.0f, 1.0f), 1.5f, 20, PARTICLETYPE_FLOWERS);
			}
		}
		else if (g_Player.pos.x > 6250)
		{// 冬エリア
			if (g_Player.move.x > 0.05f || g_Player.move.x < -0.05f || g_Player.move.z > 0.05f || g_Player.move.z < -0.05f)
			{
				SetParticle(g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2.2f, 60, PARTICLETYPE_NORMAL);
			}
		}
	}

	// 移動範囲の制限
	if (g_Player.pos.z < -165.0f)
	{
		g_Player.pos.z = -165.0f;
	}
	else if (g_Player.pos.z > 165.0f)
	{
		g_Player.pos.z = 165.0f;
	}

	if (g_Player.pos.x < -1250.0f)
	{
		g_Player.pos.x = -1250.0f;
	}
	else if (g_Player.pos.x > 8750.0f)
	{
		g_Player.pos.x = 8750.0f;
	}

	if (g_Player.move.x > 0.1f || g_Player.move.x < -0.1f || g_Player.move.z > 0.1f || g_Player.move.z < -0.1f)
	{//移動中 カメラの角度に自機の角度を合わせる
		g_Player.fDestAngle = g_Player.fDiffAngle - g_Player.rot.y;
		if (g_Player.fDestAngle > D3DX_PI)
		{
			g_Player.fDestAngle -= D3DX_PI * 2.0f;
		}
		else if (g_Player.fDestAngle < -D3DX_PI)
		{
			g_Player.fDestAngle += D3DX_PI * 2.0f;
		}
		g_Player.rot.y += g_Player.fDestAngle * 0.2f;
	}

	// 回転しないように角度を調節
	if (g_Player.rot.y > D3DX_PI)
	{
		g_Player.rot.y -= D3DX_PI * 2;
	}
	if (g_Player.rot.y < -D3DX_PI)
	{
		g_Player.rot.y += D3DX_PI * 2;
	}

	if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player.bJump == false && g_Player.state != PLAYERSTATE_NONE)
	{// ジャンプ
		g_Player.move.y = 15.0f;
		PlaySound(SOUND_LABEL_JUMP000);
	}

	XInputGetState(0, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A && g_Player.bJump == false && g_Player.state != PLAYERSTATE_NONE)
	{// プレイヤー1の A ボタンが押されている。
		g_Player.move.y = 15.0f;
		PlaySound(SOUND_LABEL_JUMP000);

	}

	//減速
	g_Player.move.x += (0.0f - g_Player.move.x) * PLAYER_SLOW;
	g_Player.move.z += (0.0f - g_Player.move.z) * PLAYER_SLOW;

	g_Player.move.y -= GRAVITY;	// 重力加算

	//位置更新(移動)
	g_Player.pos += g_Player.move;

	if (CollisionModel(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.vtxMax.z) == true)
	{// モデルに当たった時
	}

	if (CollisionField(&g_Player.pos, &g_Player.posOld, &g_Player.move) == true)
	{
		g_Player.bJump = false;
	}
	else
	{
		g_Player.bJump = true;
	}

	if (g_Player.pos.x > 8750.0f && g_Player.pos.z > -100.0f && g_Player.pos.z < 100.0f && g_Player.state == PLAYERSTATE_NORMAL)
	{// ゴール
		g_Player.state = PLAYERSTATE_CLEAR;
	}

	// 高さによる影の頂点座標
	SetVtxShadow(g_Player.nIdxShadow, g_Player.pos, 0.7f);

	// 影の位置
	SetPositionShadow(g_Player.nIdxShadow, g_Player.pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;
	int nCntModel;

	// プレイヤー
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	for (nCntModel = 0; nCntModel < PLAYER_MAXMESH; nCntModel++)
	{
		if (g_Player.aModel[nCntModel].nIdxModelParent < 0)
		{
			mtxParent = g_Player.mtxWorld;
		}
		else
		{
			mtxParent = g_Player.aModel[g_Player.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}

		// モデル（親）--------------------------------------------------------------------------------------------------------

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player.aModel[nCntModel].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.aModel[nCntModel].rot.y, g_Player.aModel[nCntModel].rot.x, g_Player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTrans, g_Player.aModel[nCntModel].pos.x, g_Player.aModel[nCntModel].pos.y, g_Player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxTrans);

		D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntModel].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntMat].nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// プレイヤー(パーツ)の描画
			g_Player.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// モーション
//=============================================================================
void PlayerMotion(void)
{

}

//=============================================================================
// プレイヤーの取得
//=============================================================================
Player *GetPlayer(void)
{
	return &g_Player;
}