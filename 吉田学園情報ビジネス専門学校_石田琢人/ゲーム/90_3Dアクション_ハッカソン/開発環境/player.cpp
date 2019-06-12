//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// プレイヤーの処理 [player.cpp]
// Author :
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "player.h"
#include "input.h"
#include "Gamepad.h"
#include "camera.h"
#include "line.h"
#include "object.h"

#include "particle.h"
#include "sound.h"
#include "gauge.h"
#include "result.h"

#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_FILE					"data/TEXTFILE/player.txt"		// ファイル名
#define PLAY_MOTION					(true)							// モーション [On,Off]
#define MOTIONBREADFRAME			(10)							// モーションブレンドのフレーム数

#define PLAYER_ROT_SPEED			(0.3f)							// 回転の速度

#define MODEL_MOVESPEED				(1.5f)							// モデルの移動速度
#define PLAYER_RUN_STADBY_STD		(fabs(WATER_SPEED) + 0.1f)		// プレイヤーの移動状態を判断する基準
#define MODEL_ANGLESPEED			(0.07f)							// モデルの方向転換速度
#define PLAYER_MOVEACCELERATION		(0.15f)							// 減速割合
#define GRAVITY						(0.3f)							// 重力

#define DEC_STAMINA					(5)								// スタミナの減少量
#define HEAL_STAMINA				(1)								// スタミナの回復量
#define HEAL_SPEED					(2)								// スタミナの回復速度

#define GOAL_POSITION				(3000.0f)						// ゴール位置

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void PlayerMotion(void);		// プレイヤーモーション
void CulFrame(void);			// フレーム計算

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LoadModel	g_LoadPlayer[MAX_MODEL];		// 読み込んだモデルの情報
LoadMotion	g_Motion;						// 読み込んだモーション情報
Player		g_aPlayer;						// プレイヤーの情報
int			g_nCounterKeySet;				// キーセットのカウンター
int			g_nCounterFrame;				// フレーム数のカウンター
int			g_nMotionBlendFrame;			// モーションブレンド用のフレーム数
D3DXVECTOR3	g_PosDiff[MAX_MODEL];			// １フレームあたりの差分 [Pos]
D3DXVECTOR3	g_RotDiff[MAX_MODEL];			// １フレームあたりの差分 [Rot]
int g_nHealStaminaCounter = 0;				// スタミナの回復カウンター

D3DXVECTOR3 g_Water;						// 水の情報

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitPlayer(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//++++++++++++++++++++
	// ----- 初期化 -----
	//++++++++++++++++++++
	//--- プレイヤーの情報 ---//
	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の位置
	g_aPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
	g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);		// 向き
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	g_aPlayer.MoveSpeed = 0.0f;								// 移動速度
	g_aPlayer.fRadius = 0.0f;								// 半径
	g_aPlayer.fHeight = 0.0f;								// 高さ
	g_aPlayer.IdxLine = 0;									// 線の使用番号 [インデックス]
	g_aPlayer.fDestAngle = D3DX_PI;							// 目的の向き
	g_aPlayer.nStamina = MAX_STAMINA;						// スタミナ量
	g_aPlayer.bRight = false;								// 右漕ぎ
	g_aPlayer.bLeft = false;								// 左漕ぎ
	/* モデル */
	g_aPlayer.nMaxParts = 0;								// 使用するするモデルの数

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{// モデルの情報
		g_aPlayer.aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// モデルの位置
		g_aPlayer.aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// モデルの向き
		g_aPlayer.aModel[nCntModel].nType = 0;												// モデルの種類
		g_aPlayer.aModel[nCntModel].nIdxModelParent = -1;									// 親モデルのインデックス
	}
	/* モーション */
	g_aPlayer.nMotionTypeOld = MOTION_MAX;			// 前モーションの種類
	g_aPlayer.nMotionType = MOTION_NEUTRAL;			// モーションの種類
	g_aPlayer.nFrameAll = 0;						// モーションの最大フレーム数
	g_aPlayer.nNowFrame = 0;						// モーションの現在のフレーム数


	g_nCounterKeySet = 0;							// キーセットのカウンター
	g_nCounterFrame = 0;							// フレーム数のカウンター
	g_nMotionBlendFrame = 0;						// モーションブレンド用のフレーム数

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{// モデルの最大数
		g_PosDiff[nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// １フレームあたりの差分 [Pos]
		g_RotDiff[nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// １フレームあたりの差分 [Rot]
	}

	g_Water = D3DXVECTOR3(0.0f, 0.0f, WATER_SPEED);		// 水の情報

	//++++++++++++++++++++++++++++++
	// ----- ファイル読み込み -----
	//++++++++++++++++++++++++++++++
	LoadPlayer();
#if 1
	//++++++++++++++++++++++++++++++
	// ----- Xファイル読み込み -----
	//++++++++++++++++++++++++++++++
	for (int nCntModel = 0; nCntModel < g_aPlayer.nLoadParts; nCntModel++)
	{// 読み込むモデルの最大数
		D3DXLoadMeshFromX(g_LoadPlayer[nCntModel].pXFileName,		// 読み込むXファイル名
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_LoadPlayer[nCntModel].pBuffMat,		// マテリアル情報ポインタ
			NULL,
			&g_LoadPlayer[nCntModel].NumMat,		// マテリアル情報の数
			&g_LoadPlayer[nCntModel].pMesh);		// メッシュ情報ポインタ

	// -----  テクスチャの読み込み  ----- //
		D3DXMATERIAL *pMat;// Xファイルから取り出したマテリアル情報を入れる

		//	 Xファイルからマテリアル情報を取得
		pMat = (D3DXMATERIAL*)g_LoadPlayer[nCntModel].pBuffMat->GetBufferPointer();

		// テクスチャのポインタをマテリアル情報の数分配列化
		g_LoadPlayer[nCntModel].pTexture = new LPDIRECT3DTEXTURE9[(int)g_LoadPlayer[nCntModel].NumMat];	// int型に直す

		// 取得したファイル名を入力
		for (int nCntNumMat = 0; nCntNumMat < (int)g_LoadPlayer[nCntModel].NumMat; nCntNumMat++)	// DWORD型からint型に変換する
		{
			// テクスチャのポインタ初期化
			g_LoadPlayer[nCntModel].pTexture[nCntNumMat] = NULL;

			// テクスチャファイル名が入っている場合、読み込む
			if (pMat[nCntNumMat].pTextureFilename != NULL)
			{
				D3DXCreateTextureFromFile(pDevice, pMat[nCntNumMat].pTextureFilename, &g_LoadPlayer[nCntModel].pTexture[nCntNumMat]);
			}
		}
	}
#endif

#if _DEBUG
	// ライン
	g_aPlayer.IdxLine = SetColisionLine2(g_aPlayer.pos, g_aPlayer.fRadius, g_aPlayer.fHeight);
#endif
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitPlayer(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		// メッシュの開放
		if (g_LoadPlayer[nCntModel].pMesh != NULL)
		{
			g_LoadPlayer[nCntModel].pMesh->Release();
			g_LoadPlayer[nCntModel].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_LoadPlayer[nCntModel].pBuffMat != NULL)
		{
			g_LoadPlayer[nCntModel].pBuffMat->Release();
			g_LoadPlayer[nCntModel].pBuffMat = NULL;
		}

		// テクスチャの開放
		for (int nTex = 0; nTex < (int)g_LoadPlayer[nCntModel].NumMat; nTex++)
		{
			if (g_LoadPlayer[nCntModel].pTexture[nTex] != NULL)
			{
				g_LoadPlayer[nCntModel].pTexture[nTex]->Release();
				g_LoadPlayer[nCntModel].pTexture[nTex] = NULL;
			}
		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdatePlayer(void)
{
	float fAngle;	// モデルの向きと目的の方向の差分

	MODE pMode = GetMode();

	if (pMode == MODE_GAME || pMode == MODE_TUTORIAL)
	{
		// ゲームの状態を取得
		GAMESTATE gameState = GetGameState();
		// カメラの取得


		Camera *pCamera = GetCamera();
		// 前回の位置を記憶
		g_aPlayer.posOld = g_aPlayer.pos;
		// 前回のモーションを記憶
		g_aPlayer.nMotionTypeOld = g_aPlayer.nMotionType;

		//++++++++++++++++++++++++++++++++++++++++
		// スタミナ
		//++++++++++++++++++++++++++++++++++++++++
		// スタミナの回復
		HealStamina();

		//++++++++++++++++++++++++++++++++++++++++
		// 移動 & 向き
		//++++++++++++++++++++++++++++++++++++++++
		if (gameState == GAMESTATE_NORMAL || pMode == MODE_TUTORIAL)
		{
			if (g_aPlayer.nStamina >= DEC_STAMINA)
			{// スタミナが消費量以上ある場合
				if (GetKeyboardTrigger(DIK_A) || GetGamapadKeyTrigger(ELECOM_LB))
				{// 左を漕ぐ[前]
					g_aPlayer.move.x += sinf((-D3DX_PI * 0.75f) + pCamera->rot.y) * g_aPlayer.MoveSpeed;
					g_aPlayer.move.z += cosf((-D3DX_PI * 0.75f) + pCamera->rot.y) * g_aPlayer.MoveSpeed;

					// 目的の方向
					//g_aPlayer.fDestAngle = (-D3DX_PI * 0.85f);
					g_aPlayer.fDestAngle += (PLAYER_ROT_SPEED);

					// スタミナの減少
					SubStamina();

					// パーティクル
					SetParticleEmitter(g_aPlayer.pos, 1);		// オールを漕ぐしぶき

					// フラグ
					g_aPlayer.bLeft = true;
					g_aPlayer.bRight = false;
				}
				else if (GetKeyboardTrigger(DIK_D) || GetGamapadKeyTrigger(ELECOM_RB))
				{// 右を漕ぐ[前]
					g_aPlayer.move.x += sinf((D3DX_PI * 0.75f) + pCamera->rot.y) * g_aPlayer.MoveSpeed;
					g_aPlayer.move.z += cosf((D3DX_PI * 0.75f) + pCamera->rot.y) * g_aPlayer.MoveSpeed;

					// 目的の方向
					//g_aPlayer.fDestAngle = (D3DX_PI * 0.85f);
					g_aPlayer.fDestAngle -= (PLAYER_ROT_SPEED);

					// スタミナの減少
					SubStamina();

					// パーティクル
					SetParticleEmitter(g_aPlayer.pos, 1);		// オールを漕ぐしぶき

					// フラグ
					g_aPlayer.bRight = true;
					g_aPlayer.bLeft = false;
				}
				if (GetKeyboardTrigger(DIK_Z) || GetGamapadKeyTrigger(ELECOM_LT))
				{// 左を漕ぐ[後ろ]
					g_aPlayer.move.x -= sinf((D3DX_PI * 0.75f) + pCamera->rot.y) * g_aPlayer.MoveSpeed;
					g_aPlayer.move.z -= cosf((D3DX_PI * 0.75f) + pCamera->rot.y) * g_aPlayer.MoveSpeed;

					// 目的の方向
					//g_aPlayer.fDestAngle = (-D3DX_PI * 0.85f);
					g_aPlayer.fDestAngle -= (PLAYER_ROT_SPEED);

					// スタミナの減少
					SubStamina();

					// パーティクル
					SetParticleEmitter(g_aPlayer.pos, 1);		// オールを漕ぐしぶき

					// フラグ
					g_aPlayer.bLeft = true;
					g_aPlayer.bRight = false;
				}
				else if (GetKeyboardTrigger(DIK_C) || GetGamapadKeyTrigger(ELECOM_RT))
				{// 右を漕ぐ[後ろ]
					g_aPlayer.move.x -= sinf((-D3DX_PI * 0.75f) + pCamera->rot.y) * g_aPlayer.MoveSpeed;
					g_aPlayer.move.z -= cosf((-D3DX_PI * 0.75f) + pCamera->rot.y) * g_aPlayer.MoveSpeed;

					// 目的の方向
					//g_aPlayer.fDestAngle = (D3DX_PI * 0.85f);
					g_aPlayer.fDestAngle += (PLAYER_ROT_SPEED);

					// スタミナの減少
					SubStamina();

					// パーティクル
					SetParticleEmitter(g_aPlayer.pos, 1);		// オールを漕ぐしぶき

					// フラグ
					g_aPlayer.bRight = true;
					g_aPlayer.bLeft = false;
				}
			}

			// --------------- 向き --------------- //
			// 目的の向きの調整
			if (g_aPlayer.fDestAngle > D3DX_PI)
			{
				g_aPlayer.fDestAngle -= D3DX_PI * 2;
			}
			else if (g_aPlayer.fDestAngle < -D3DX_PI)
			{
				g_aPlayer.fDestAngle += D3DX_PI * 2;
			}

			// 差分の調整
			fAngle = g_aPlayer.fDestAngle - g_aPlayer.rot.y;

			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}
			else if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}

			g_aPlayer.rot.y += fAngle * MODEL_ANGLESPEED;

			if (g_aPlayer.rot.y > D3DX_PI)
			{
				g_aPlayer.rot.y -= D3DX_PI * 2;
			}
			else if (g_aPlayer.rot.y < -D3DX_PI)
			{
				g_aPlayer.rot.y += D3DX_PI * 2;
			}
			//++++++++++++++++++++++++++++++++++++++++
			// 重力
			//++++++++++++++++++++++++++++++++++++++++
			g_aPlayer.move.y -= GRAVITY;

			//++++++++++++++++++++++++++++++++++++++++
			// 水の流れ
			//++++++++++++++++++++++++++++++++++++++++
			g_aPlayer.move += g_Water;

			//--- 終了位置 ---//
			if (g_aPlayer.pos.z >= GOAL_POSITION)
			{
				//
				SetResultState(RESULTSTATE_GAMECLEAR);

				// ゲーム終了
				SetGameState(GAMESTATE_END);

				// フラグを戻す
				g_aPlayer.bLeft = false;
				g_aPlayer.bRight = false;
			}
			else if (g_aPlayer.pos.z <= -200.0f)
			{
				//
				SetResultState(RESULTSTATE_GAMEOVER);

				// ゲーム終了
				SetGameState(GAMESTATE_END);

				// フラグを戻す
				g_aPlayer.bLeft = false;
				g_aPlayer.bRight = false;
			}
		}

		//++++++++++++++++++++++++++++++++++++++++
		// 位置の更新
		//++++++++++++++++++++++++++++++++++++++++
		g_aPlayer.pos += g_aPlayer.move;

		if (g_aPlayer.posOld.y >= 0.0f && g_aPlayer.pos.y < 0.0f)
		{
			g_aPlayer.pos.y = 0.0f;
		}

		// 慣性処理
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * PLAYER_MOVEACCELERATION;			// 減速する割合 [x方向]
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * PLAYER_MOVEACCELERATION;			// 減速する割合 [z方向]
		//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * PLAYER_MOVEACCELERATION;			// 減速する割合 [y方向]

		//++++++++++++++++++++++++++++++++++++++++
		// 当たり判定
		//++++++++++++++++++++++++++++++++++++++++
		if (pMode != MODE_TUTORIAL)
		{
			bool bFlowingObject = CollisionObject(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, g_aPlayer.fRadius);

			if (bFlowingObject)
			{
				// パーティクル
				SetParticleEmitter(g_aPlayer.pos, 2);		// 衝突
			}
		}

		if (g_aPlayer.pos.x <= -500.0f)
		{
			g_aPlayer.pos.x = -500.0f;
		}
		else if (g_aPlayer.pos.x >= 500.0f)
		{

			g_aPlayer.pos.x = 500.0f;
		}
		//++++++++++++++++++++++++++++++++++++++++
		// 影, 線の移動
		//++++++++++++++++++++++++++++++++++++++++
#if _DEBUG
		SetPositionLine(g_aPlayer.IdxLine, g_aPlayer.pos);			// ラインの更新
#endif

		//++++++++++++++++++++++++++++++++++++++++
		// モーション
		//++++++++++++++++++++++++++++++++++++++++
		if (!g_aPlayer.bRight  && !g_aPlayer.bLeft)
		{// 待機
			g_aPlayer.nMotionType = MOTION_NEUTRAL;
		}
		if (g_aPlayer.bLeft)
		{// 左漕ぎ
			g_aPlayer.nMotionType = MOTION_LEFT;

			if (g_aPlayer.nMotionType != g_aPlayer.nMotionTypeOld)
			{
				CulFrame();
			}

			if (g_aPlayer.nNowFrame == g_aPlayer.nFrameAll)
			{// 最後まで再生された場合
				g_aPlayer.bLeft = false;
			}
		}
		if (g_aPlayer.bRight)
		{// 右漕ぎ
			g_aPlayer.nMotionType = MOTION_RIGHT;

			if (g_aPlayer.nMotionType != g_aPlayer.nMotionTypeOld)
			{
				CulFrame();
			}

			if (g_aPlayer.nNowFrame == g_aPlayer.nFrameAll)
			{// 最後まで再生された場合
				g_aPlayer.bRight = false;
			}
		}

		// モーション
#if PLAY_MOTION
		PlayerMotion();
#endif
	}
	else if (pMode == MODE_RESULT)
	{
		RESULTSTATE state = GetResultState();

		if (state == RESULTSTATE_GAMEOVER)
		{
			g_aPlayer.rot.z = D3DX_PI;
		}

		g_aPlayer.pos.z = -200.0f;
		g_aPlayer.pos.y = -20.0f;

		// 過去のモーション
		g_aPlayer.nMotionTypeOld = g_aPlayer.nMotionType;

		// 待機モーション
		g_aPlayer.nMotionType = MOTION_NEUTRAL;

		// モーション
		PlayerMotion();
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();		// デバイスの取得
	D3DXMATRIX			mtxRot, mtxTrans;			// 計算用マトリックス
	D3DMATERIAL9		matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL		*pMat;						// マテリアルへのポインタ
	D3DXMATRIX			mtxParent;					// 親のマトリックス

	//============================
	// -----  プレイヤー  ----- //
	//============================
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
	D3DXMatrixMultiply(&g_aPlayer.mtxWorld,
		&g_aPlayer.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
	D3DXMatrixMultiply(&g_aPlayer.mtxWorld,
		&g_aPlayer.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

	//============================
	// -----  モデル  ----- //
	//============================
	// かけ合わせるマトリックスの設定
	for (int nCntModel = 0; nCntModel < g_aPlayer.nMaxParts; nCntModel++)
	{
		if (g_aPlayer.aModel[nCntModel].nIdxModelParent == -1)
		{
			mtxParent = g_aPlayer.mtxWorld;
		}
		else
		{
			mtxParent = g_aPlayer.aModel[g_aPlayer.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCntModel].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_aPlayer.aModel[nCntModel].rot.y, g_aPlayer.aModel[nCntModel].rot.x, g_aPlayer.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCntModel].mtxWorld,
			&g_aPlayer.aModel[nCntModel].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_aPlayer.aModel[nCntModel].pos.x, g_aPlayer.aModel[nCntModel].pos.y, g_aPlayer.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCntModel].mtxWorld,
			&g_aPlayer.aModel[nCntModel].mtxWorld, &mtxTrans);

		// 親モデルのマトリックスをかけ合わせる
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCntModel].mtxWorld,
			&g_aPlayer.aModel[nCntModel].mtxWorld, &mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCntModel].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_LoadPlayer[g_aPlayer.aModel[nCntModel].nType].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[g_aPlayer.aModel[nCntModel].nType].NumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの反映
			pDevice->SetTexture(0, 0);

			// モデル(パーツ)の描画
			g_LoadPlayer[g_aPlayer.aModel[nCntModel].nType].pMesh->DrawSubset(nCntMat);
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// プレイヤーモーション
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void PlayerMotion(void)
{
#if 1
	//++++++++++++++++++++++++++++++
	// モーションの初期化
	//++++++++++++++++++++++++++++++
	if (g_aPlayer.nMotionTypeOld != g_aPlayer.nMotionType)
	{// モーションが前回と異なる
		// キーフレーム
		g_nCounterKeySet = 0;
		// フレーム数
		g_nCounterFrame = 0;
		// モーションブレンドのフレーム数を決める
		g_nMotionBlendFrame = MOTIONBREADFRAME;
		// フレーム数の計算
		CulFrame();
	}

	//++++++++++++++++++++++++++++++
	// キーの切替
	//++++++++++++++++++++++++++++++
	switch (g_Motion.Motion[g_aPlayer.nMotionType].nLoop)
	{// モーションの種類のループ別
	case 0:		// ループしない
		if (g_nCounterFrame == g_nMotionBlendFrame && g_Motion.Motion[g_aPlayer.nMotionType].nNumKey == g_nCounterKeySet + 1)
		{// 最終フレーム　&& 最終キーになった
			// フレーム数初期化
			g_nCounterFrame = 0;
			// キーセットを進める
			g_nCounterKeySet = g_Motion.Motion[g_aPlayer.nMotionType].nNumKey - 1;		// 最後のキーセットを設定する
			// 分割フレーム数を決める
			g_nMotionBlendFrame = g_Motion.Motion[g_aPlayer.nMotionType].nKeySet[g_nCounterKeySet].frame;
		}
		else if (g_nCounterFrame == g_nMotionBlendFrame && g_Motion.Motion[g_aPlayer.nMotionType].nNumKey != g_nCounterKeySet)
		{// 最後のフレーム && 最終キーではない
		 // フレーム数初期化
			g_nCounterFrame = 0;
			// キーセットを進める
			g_nCounterKeySet = (g_nCounterKeySet + 1) % g_Motion.Motion[g_aPlayer.nMotionType].nNumKey;
			// 分割フレーム数を決める
			g_nMotionBlendFrame = g_Motion.Motion[g_aPlayer.nMotionType].nKeySet[g_nCounterKeySet].frame;
		}
		break;

	case 1:		// ループする
		if (g_nCounterFrame == g_nMotionBlendFrame)
		{// 最後のフレーム
			// フレーム数初期化
			g_nCounterFrame = 0;
			// キーセットを進める
			g_nCounterKeySet = (g_nCounterKeySet + 1) % g_Motion.Motion[g_aPlayer.nMotionType].nNumKey;
			// 分割フレーム数を決める
			g_nMotionBlendFrame = g_Motion.Motion[g_aPlayer.nMotionType].nKeySet[g_nCounterKeySet].frame;
		}
		break;
	}

	if (g_nCounterFrame == 0)
	{// 最初のフレームになったら、次のキーセットとの差分計算
		for (int nCntParts = 0; nCntParts < MAX_MODEL; nCntParts++)
		{
			// 各パーツの位置の差分 = ([次]のキーセットの各パーツの位置 + 各パーツの初期オフセット[位置]) - [現在]の各パーツの位置
			g_PosDiff[nCntParts] = (g_Motion.Motion[g_aPlayer.nMotionType].nKeySet[g_nCounterKeySet].pos[nCntParts] + g_Motion.Offset[nCntParts].pos) - g_aPlayer.aModel[nCntParts].pos;
			// 各パーツの向きの差分 = [次]のキーセットの各パーツの向き + 各パーツの初期オフセット[向き] - [現在]の各パーツの向き
			g_RotDiff[nCntParts] = g_Motion.Motion[g_aPlayer.nMotionType].nKeySet[g_nCounterKeySet].rot[nCntParts] + g_Motion.Offset[nCntParts].rot - g_aPlayer.aModel[nCntParts].rot;

			// 差分をフレーム数で割る
			// １フレームあたりの差分 = (各パーツの位置[x]の差分 / [次]のキーセットのフレーム数, 各パーツの位置[y]の差分 / [次]のキーセットのフレーム数, 各パーツの位置[z]の差分 / [次]のキーセットのフレーム数)
			g_PosDiff[nCntParts] = D3DXVECTOR3(g_PosDiff[nCntParts].x / g_nMotionBlendFrame, g_PosDiff[nCntParts].y / g_nMotionBlendFrame, g_PosDiff[nCntParts].z / g_nMotionBlendFrame);
			g_RotDiff[nCntParts] = D3DXVECTOR3(g_RotDiff[nCntParts].x / g_nMotionBlendFrame, g_RotDiff[nCntParts].y / g_nMotionBlendFrame, g_RotDiff[nCntParts].z / g_nMotionBlendFrame);
		}
	}

	//++++++++++++++++++++++++++++++
	// モーション更新
	//++++++++++++++++++++++++++++++
	for (int nCntParts = 0; nCntParts < MAX_MODEL; nCntParts++)
	{
		g_aPlayer.aModel[nCntParts].pos += g_PosDiff[nCntParts];
		g_aPlayer.aModel[nCntParts].rot += g_RotDiff[nCntParts];

		if (g_aPlayer.aModel[nCntParts].rot.x > D3DX_PI)
		{
			g_aPlayer.aModel[nCntParts].rot.x -= D3DX_PI * 2;
		}
		else if (g_aPlayer.aModel[nCntParts].rot.x < -D3DX_PI)
		{
			g_aPlayer.aModel[nCntParts].rot.x += D3DX_PI * 2;
		}
		if (g_aPlayer.aModel[nCntParts].rot.y > D3DX_PI)
		{
			g_aPlayer.aModel[nCntParts].rot.y -= D3DX_PI * 2;
		}
		else if (g_aPlayer.aModel[nCntParts].rot.y < -D3DX_PI)
		{
			g_aPlayer.aModel[nCntParts].rot.y += D3DX_PI * 2;
		}
		if (g_aPlayer.aModel[nCntParts].rot.y > D3DX_PI)
		{
			g_aPlayer.aModel[nCntParts].rot.z -= D3DX_PI * 2;
		}
		else if (g_aPlayer.aModel[nCntParts].rot.z < -D3DX_PI)
		{
			g_aPlayer.aModel[nCntParts].rot.z += D3DX_PI * 2;
		}
	}
	// フレーム数の加算
	g_nCounterFrame++;

	if (g_aPlayer.nNowFrame < g_aPlayer.nFrameAll)
	{
		// 現在のフレーム数を加算
		g_aPlayer.nNowFrame++;
	}
#endif
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// スタミナの減少
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SubStamina(void)
{
	if (g_aPlayer.nStamina >= DEC_STAMINA)
	{// スタミナが消費量以上ある場合
		// スタミナの減少
		g_aPlayer.nStamina -= DEC_STAMINA;
	}

	//--- ゲージ ---//
	AddSubtractGauge(-DEC_STAMINA);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// スタミナの回復
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void HealStamina(void)
{
	// スタミナ回復カウンタの加算
	g_nHealStaminaCounter++;

	if (g_nHealStaminaCounter % HEAL_SPEED == 0)
	{// 回復タイミングの場合
		if (g_aPlayer.nStamina < MAX_STAMINA)
		{// スタミナの最大値以下の場合

			g_aPlayer.nStamina += HEAL_STAMINA;

			// カウンタリセット
			g_nHealStaminaCounter = 0;

			//--- ゲージ ---//
			AddSubtractGauge(HEAL_STAMINA);
		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// モーションのフレーム数計算
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void CulFrame(void)
{
	// 現在のフレーム数をリセット
	g_aPlayer.nNowFrame = 0;
	// モーションの総フレーム数をリセット
	g_aPlayer.nFrameAll = 0;
	// モーションの総フレーム数を計算
	for (int nCntKey = 0; nCntKey < g_Motion.Motion[g_aPlayer.nMotionType].nNumKey; nCntKey++)
	{
		g_aPlayer.nFrameAll += g_Motion.Motion[g_aPlayer.nMotionType].nKeySet[nCntKey].frame;
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// プレイヤーの読み込み
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void LoadPlayer(void)
{
	//--- 読み込んだモデルの情報 ---//
	for (int nCntModel = 0; nCntModel < MAX_LOADMODEL; nCntModel++)
	{// モデルの最大数
		for (int nText = 0; nText < MAX_CHAR; nText++)
		{
			g_LoadPlayer[nCntModel].pXFileName[nText] = NULL;		// ファイル名
		}
		g_LoadPlayer[nCntModel].pMesh = NULL;					// メッシュ情報へのポインタ
		g_LoadPlayer[nCntModel].pBuffMat = NULL;				// マテリアル情報へのポインタ
		g_LoadPlayer[nCntModel].NumMat = 0;						// マテリアルの数
		g_LoadPlayer[nCntModel].pTexture = NULL;				// テクスチャへのポインタ
	}

	//--- 読み込んだモーション情報 ---//
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{// モデルの最大数
		g_Motion.Offset[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// オフセット[位置]
		g_Motion.Offset[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// オフセット[向き]
	}
	for (int nCntMotion = 0; nCntMotion < MOTION_MAX; nCntMotion++)
	{// モーションの種類
		g_Motion.Motion[nCntMotion].nLoop;				// ループ情報 [0 : ループしない, 1 : ループする]
		g_Motion.Motion[nCntMotion].nNumKey = 0;		// キー数
		/* キー情報 */
		for (int nNumKeySet = 0; nNumKeySet < MAX_KEYSET; nNumKeySet++)
		{// キーセットの最大数
			g_Motion.Motion[nCntMotion].nKeySet[nNumKeySet].frame = 0;				// フレーム数
			for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
			{// モデルの最大数
				g_Motion.Motion[nCntMotion].nKeySet[nNumKeySet].pos[nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
				g_Motion.Motion[nCntMotion].nKeySet[nNumKeySet].rot[nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
			}
		}
	}

#if 1
	// ローカル変数宣言
	FILE *pFile;				// ファイルのポインタ
	char ReadText[256];			// 読み込んだ文字列を入れておく
	char HeadText[256];			// 比較用
	char DustBox[256];			// 使用しないものを入れておく
	int nCntText = 0;			// モデルファイル名
	int nCntModel = 0;			// モデル数
	int nCntMotion = 0;			// モーション数
	int nCntKeySet = 0;			// キーセット数

	// ファイルオープン
	pFile = fopen(PLAYER_FILE, "r");		// ファイルを開く[読み込み型]

	if (pFile != NULL)		// ファイルが空ではない
	{//ファイルが開けた場合
		while (strcmp(HeadText, "SCRIPT") != 0)
		{// "SCRIPT" が読み込まれるまで繰り返し文字列を読み取る
			fgets(ReadText, sizeof(ReadText), pFile);
			sscanf(ReadText, "%s", &HeadText);
		}

		if (strcmp(HeadText, "SCRIPT") == 0)
		{// "SCRIPT" が読み取れた場合、処理開始
			while (strcmp(HeadText, "END_SCRIPT") != 0)
			{// "END_SCRIPT" が読み込まれるまで繰り返し文字列を読み取る
				fgets(ReadText, sizeof(ReadText), pFile);
				sscanf(ReadText, "%s", &HeadText);

				if (strcmp(HeadText, "\n") == 0)
				{// 文字列の先頭が [\n](改行) の場合処理しない
					int nCnt = 0;
				}
				else if (strcmp(HeadText, "NUM_MODEL") == 0)
				{// 読み込むモデルの最大数
					sscanf(ReadText, "%s %c %d", &DustBox, &DustBox, &g_aPlayer.nLoadParts);
				}
				else if (strcmp(HeadText, "MODEL_FILENAME") == 0)
				{// モデルファイル名
					if (g_aPlayer.nLoadParts > nCntText)
					{// 最大数以上処理しない
						sscanf(ReadText, "%s %c %s", &DustBox, &DustBox, &g_LoadPlayer[nCntText].pXFileName[0]);
						nCntText++;
					}
				}
				else if (strcmp(HeadText, "CHARACTERSET") == 0)
				{//--- キャラクター情報 ---//
					while (strcmp(HeadText, "END_CHARACTERSET") != 0)
					{// "END_CHARACTERSET" が読み取れるまで繰り返し文字列を読み取る
						fgets(ReadText, sizeof(ReadText), pFile);
						sscanf(ReadText, "%s", &HeadText);

						if (strcmp(HeadText, "MOVE") == 0)
						{// 移動量
							sscanf(ReadText, "%s %c %f", &DustBox, &DustBox, &g_aPlayer.MoveSpeed);
						}
						else if (strcmp(HeadText, "POS") == 0)
						{// 移動量
							sscanf(ReadText, "%s %c %f %f %f", &DustBox, &DustBox, &g_aPlayer.pos.x, &g_aPlayer.pos.y, &g_aPlayer.pos.z);
						}
						else if (strcmp(HeadText, "RADIUS") == 0)
						{// 半径
							sscanf(ReadText, "%s %c %f", &DustBox, &DustBox, &g_aPlayer.fRadius);
						}
						else if (strcmp(HeadText, "HEIGHT") == 0)
						{// 高さ
							sscanf(ReadText, "%s %c %f", &DustBox, &DustBox, &g_aPlayer.fHeight);
						}
						else if (strcmp(HeadText, "NUM_PARTS") == 0)
						{// プレイヤー１体に対するモデルの使用数
							sscanf(ReadText, "%s %c %d", &DustBox, &DustBox, &g_aPlayer.nMaxParts);
						}
						else if (strcmp(HeadText, "PARTSSET") == 0)
						{//--- モデルの配置情報 ---//
							if (g_aPlayer.nMaxParts > nCntModel)
							{
								while (strcmp(HeadText, "END_PARTSSET") != 0)
								{// "END_PARTSSET" が読み取れるまで繰り返し文字列を読み取る
									fgets(ReadText, sizeof(ReadText), pFile);
									sscanf(ReadText, "%s", &HeadText);

									if (strcmp(HeadText, "INDEX") == 0)
									{// 部位
										sscanf(ReadText, "%s %c %d", &DustBox, &DustBox, &g_aPlayer.aModel[nCntModel].nType);
									}
									else if (strcmp(HeadText, "PARENT") == 0)
									{// 親のインデックス
										sscanf(ReadText, "%s %c %d", &DustBox, &DustBox, &g_aPlayer.aModel[nCntModel].nIdxModelParent);
									}
									else if (strcmp(HeadText, "POS") == 0)
									{// 位置
										sscanf(ReadText, "%s %c %f %f %f", &DustBox, &DustBox, &g_aPlayer.aModel[nCntModel].pos.x, &g_aPlayer.aModel[nCntModel].pos.y, &g_aPlayer.aModel[nCntModel].pos.z);
										sscanf(ReadText, "%s %c %f %f %f", &DustBox, &DustBox, &g_Motion.Offset[nCntModel].pos.x, &g_Motion.Offset[nCntModel].pos.y, &g_Motion.Offset[nCntModel].pos.z);
									}
									else if (strcmp(HeadText, "ROT") == 0)
									{// 向き
										sscanf(ReadText, "%s %c %f %f %f", &DustBox, &DustBox, &g_aPlayer.aModel[nCntModel].rot.x, &g_aPlayer.aModel[nCntModel].rot.y, &g_aPlayer.aModel[nCntModel].rot.z);
									}
								}
								nCntModel++;		// モデル数を進める
							}
						}
					}
				}
				else if (strcmp(HeadText, "MOTIONSET") == 0)
				{//--- モーション情報 ---//
					nCntModel = 0;		// モデル数のリセット

					while (strcmp(HeadText, "END_MOTIONSET") != 0)
					{// "END_MOTIONSET" が読み取れるまで繰り返し文字列を読み取る
						fgets(ReadText, sizeof(ReadText), pFile);
						sscanf(ReadText, "%s", &HeadText);

						if (strcmp(HeadText, "LOOP") == 0)
						{// ループ情報
							sscanf(ReadText, "%s %c %d", &DustBox, &DustBox, &g_Motion.Motion[nCntMotion].nLoop);
						}
						else if (strcmp(HeadText, "NUM_KEY") == 0)
						{// キー数
							sscanf(ReadText, "%s %c %d", &DustBox, &DustBox, &g_Motion.Motion[nCntMotion].nNumKey);
						}
						else if (strcmp(HeadText, "KEYSET") == 0)
						{// キーセット
							while (strcmp(HeadText, "END_KEYSET") != 0)
							{// "END_KEYSET" が読み取れるまで繰り返し文字列を読み取る
								fgets(ReadText, sizeof(ReadText), pFile);
								sscanf(ReadText, "%s", &HeadText);

								if (strcmp(HeadText, "FRAME") == 0)
								{// フレーム数
									sscanf(ReadText, "%s %c %d", &DustBox, &DustBox, &g_Motion.Motion[nCntMotion].nKeySet[nCntKeySet].frame);
								}
								else if (strcmp(HeadText, "KEY") == 0)
								{// キー
									while (strcmp(HeadText, "END_KEY") != 0)
									{// "END_KEY" が読み取れるまで繰り返し文字列を読み取る
										fgets(ReadText, sizeof(ReadText), pFile);
										sscanf(ReadText, "%s", &HeadText);

										if (strcmp(HeadText, "POS") == 0)
										{// 位置
											sscanf(ReadText, "%s %c %f %f %f", &DustBox, &DustBox, &g_Motion.Motion[nCntMotion].nKeySet[nCntKeySet].pos[nCntModel].x, &g_Motion.Motion[nCntMotion].nKeySet[nCntKeySet].pos[nCntModel].y, &g_Motion.Motion[nCntMotion].nKeySet[nCntKeySet].pos[nCntModel].z);
										}
										else if (strcmp(HeadText, "ROT") == 0)
										{// 向き
											sscanf(ReadText, "%s %c %f %f %f", &DustBox, &DustBox, &g_Motion.Motion[nCntMotion].nKeySet[nCntKeySet].rot[nCntModel].x, &g_Motion.Motion[nCntMotion].nKeySet[nCntKeySet].rot[nCntModel].y, &g_Motion.Motion[nCntMotion].nKeySet[nCntKeySet].rot[nCntModel].z);
										}
									}
									nCntModel++;		// モデル数を進める
								}
							}
							nCntKeySet++;		// キーセット数を進める
							nCntModel = 0;		// モデル数のリセット
						}
					}
					nCntMotion++;			// モーションの種類を進める
					nCntKeySet = 0;			// キーセット数のリセット
				}
			}
		}
		// ファイルクローズ
		fclose(pFile);
#if 0
		MessageBox(NULL, "キャラクター情報ファイルを正しく読み込みました。", NULL, MB_OK);
#endif
	}
	else
	{// 開けなかった場合
		MessageBox(NULL, "キャラクター情報ファイルが開けませんでした。", NULL, MB_OK);
	}
#endif
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// プレイヤーの破棄
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UnloadPlayer(void)
{

}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// プレイヤーの取得
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Player *GetPlayer(void)
{
	return &g_aPlayer;
}