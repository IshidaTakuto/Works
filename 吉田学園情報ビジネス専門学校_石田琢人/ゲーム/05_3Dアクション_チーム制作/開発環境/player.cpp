//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 有馬　武志
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS		//scanfエラー解除
#include <stdio.h>					//stdio
#include "player.h"					//プレイヤー
#include "input.h"					//キーボード
#include "camera.h"					//カメラ
#include "shadow.h"					//影
#include "sound.h"					//サウンド
#include "object.h"					//オブジェクト
#include "object2.h"					//オブジェクト
#include "object3.h"					//オブジェクト
#include "enemy.h"
#include "killLog.h"
#include "game.h"
#include "fade.h"
#include "life.h"
#include "Blood.h"
#include "load.h"
#include "gauge.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER1_SPEED			(5.0f)

#define ATTACK_PARTS			(6)
#define ATTACK_START			(0)			// 攻撃の開始キー
#define ATTACK_END				(8)			// 攻撃の終わりキー
#define ATTACK_RADIUS			(30.0f)		// 団子の半径
#define ATTACK_NUM				(3)			// 団子の数
#define PLAYER_DAMAGE_TIME		(60)		// 攻撃を受けたときの無敵時間
#define DRAGON_TIME				(600)		// ドラゴン状態の時間

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Player		g_Player;							//プレイヤー
int			g_nCntTimePlayer;					//プレイヤーカウンター
float		g_fJoyStickAnglePlayer1;			//
int			g_nCntPlayerType;					// ドラゴン状態のカウンタ

//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイルのポインタ
	FILE *pFile;
	//プレイヤーカウンター
	int nCntPlayer;

	// 位置・向きの初期設定
	g_Player.pos = D3DXVECTOR3(0, 0, 40);
	g_Player.rot = D3DXVECTOR3(0, 0, 0);
	g_Player.move = D3DXVECTOR3(0, 0, 0);
	g_Player.nCntPlayer = 1;
	g_Player.bUse = true;
	g_Player.bLand = true;
	g_Player.bAttack = false;
	g_Player.bDamage = false;
	g_Player.type = TYPE_HUMAN;
	g_Player.nAnimnow = PLAYERANIM_NEUTRAL;
	g_Player.bLegWhich = false;

	g_nCntPlayerType = 0;
	//ファイル設定
	pFile = fopen(FILE_NAME, "r");

	//プレイヤーの￥テキストデータの読み込み
	if (pFile != NULL)
	{
		//使うパーツ数
		fscanf(pFile, "%d", &g_Player.nMaxPartsNum);	//読み込み

		//ファイル名
		for (nCntPlayer = 0; nCntPlayer < g_Player.nMaxPartsNum; nCntPlayer++)
		{
			fscanf(pFile, "%s", &g_Player.aModel[nCntPlayer].FileName[0]); //読み込み
		}

		for (nCntPlayer = 0; nCntPlayer < g_Player.nMaxPartsNum; nCntPlayer++)
		{//初期値と向き
			fscanf(pFile, "%d", &g_Player.aModel[nCntPlayer].nIdxModelParent);	//読み込み
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitPos.x);		//読み込み
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitPos.y);		//読み込み
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitPos.z);		//読み込み
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitRot.x);		//読み込み
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitRot.y);		//読み込み
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitRot.z);		//読み込み
		}

		//ファイル読み込み
		fscanf(pFile, "%d", &g_Player.nMaxAnimNum);

		for (int nCntPlayerAnim = 0; nCntPlayerAnim < g_Player.nMaxAnimNum; nCntPlayerAnim++)
		{
			//フレーム数
			fscanf(pFile, "%d", &g_Player.nAnimloop[nCntPlayerAnim]);

			//キー数
			fscanf(pFile, "%d", &g_Player.nAnimKey[nCntPlayerAnim]);

			for (int nCntPlayerAnimKey = 0; nCntPlayerAnimKey < g_Player.nAnimKey[nCntPlayerAnim]; nCntPlayerAnimKey++)
			{
				fscanf(pFile, "%d", &g_Player.nAnimfram[nCntPlayerAnim][nCntPlayerAnimKey]);//読み込み

				for (nCntPlayer = 0; nCntPlayer < g_Player.nMaxPartsNum; nCntPlayer++)
				{
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].posAnim[nCntPlayerAnim][nCntPlayerAnimKey].x);		//読み込み
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].posAnim[nCntPlayerAnim][nCntPlayerAnimKey].y);		//読み込み
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].posAnim[nCntPlayerAnim][nCntPlayerAnimKey].z);		//読み込み
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].rotAnim[nCntPlayerAnim][nCntPlayerAnimKey].x);		//読み込み
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].rotAnim[nCntPlayerAnim][nCntPlayerAnimKey].y);		//読み込み
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].rotAnim[nCntPlayerAnim][nCntPlayerAnimKey].z);		//読み込み

					g_Player.aModel[nCntPlayer].InitRot = g_Player.aModel[nCntPlayer].rotAnim[PLAYERANIM_NEUTRAL][0];
					g_Player.aModel[nCntPlayer].rotDest = g_Player.aModel[nCntPlayer].rotAnim[PLAYERANIM_NEUTRAL][0];
				}
			}
		}

		g_Player.aModel[0].InitPos.y = g_Player.pos.y;

		fclose(pFile);	//ファイルを閉じる
	}
	else
	{
		MessageBox(0, "セーブファイルのチェックに失敗!", "警告", MB_ICONWARNING);
	}

	//xファイルの読み込み
	for (nCntPlayer = 0; nCntPlayer < g_Player.nMaxPartsNum; nCntPlayer++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX
		(
			&g_Player.aModel[nCntPlayer].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Player.aModel[nCntPlayer].pBuffMatModel,
			NULL,
			&g_Player.aModel[nCntPlayer].nNumMatModel,
			&g_Player.aModel[nCntPlayer].pMeshModel
		);
	}

	//影の初期化設定
	g_Player.nIdxShadow = SetShadow(g_Player.pos,g_Player.rot,25, 25);

	// 頂点座標の最大・最小
	for (nCntPlayer = 0; nCntPlayer < g_Player.nMaxPartsNum; nCntPlayer++)
	{
		g_Player.aModel[nCntPlayer].vtxMin = D3DXVECTOR3(10000, 10000, 10000);
		g_Player.aModel[nCntPlayer].vtxMax = D3DXVECTOR3(1, 1, 1);

		int nNumVtx;		// 頂点数
		DWORD sizeFVF;		// 頂点フォーマットのサイズ
		BYTE *pVtxBuff;		// 頂点バッファへのポインタ

		// 頂点数を取得
		nNumVtx = g_Player.aModel[nCntPlayer].pMeshModel->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Player.aModel[nCntPlayer].pMeshModel->GetFVF());

		// 頂点バッファをロック
		g_Player.aModel[nCntPlayer].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// Xの値の比較
		if (g_Player.aModel[nCntPlayer].vtxMin.x > vtx.x)
		{// 最小値と比較
			g_Player.aModel[nCntPlayer].vtxMin.x = vtx.x;	// 最小値より小さければ代入
		}

		if (g_Player.aModel[nCntPlayer].vtxMax.x < vtx.x)
		{// 最大値と比較
			g_Player.aModel[nCntPlayer].vtxMax.x = vtx.x;	// 最大値より大きければ代入
		}

		// Yの値の比較
		if (g_Player.aModel[nCntPlayer].vtxMin.y > vtx.y)
		{// 最小値と比較
			g_Player.aModel[nCntPlayer].vtxMin.y = vtx.y;	// 最小値より小さければ代入
		}

		if (g_Player.aModel[nCntPlayer].vtxMax.y < vtx.y)
		{// 最大値と比較
			g_Player.aModel[nCntPlayer].vtxMax.y = vtx.y;	// 最大値より大きければ代入
		}

		// Zの値の比較
		if (g_Player.aModel[nCntPlayer].vtxMin.z > vtx.z)
		{// 最小値と比較
			g_Player.aModel[nCntPlayer].vtxMin.z = vtx.z;	// 最小値より小さければ代入
		}

		if (g_Player.aModel[nCntPlayer].vtxMax.z < vtx.z)
		{// 最大値と比較
			g_Player.aModel[nCntPlayer].vtxMax.z = vtx.z;	// 最大値より大きければ代入
		}

		// 頂点バッファをアンロック
		g_Player.aModel[nCntPlayer].pMeshModel->UnlockVertexBuffer();
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	DeletePlayer();

	for (int nCount = 0; nCount < MAX_PLAYER_PARTS; nCount++)
	{
		// メッシュの開放
		if (g_Player.aModel[nCount].pMeshModel != NULL)
		{
			g_Player.aModel[nCount].pMeshModel->Release();
			g_Player.aModel[nCount].pMeshModel = NULL;
		}
	}
	for (int nCount = 0; nCount < MAX_PLAYER_PARTS; nCount++)
	{
		// マテリアルの開放
		if (g_Player.aModel[nCount].pBuffMatModel != NULL)
		{
			g_Player.aModel[nCount].pBuffMatModel->Release();
			g_Player.aModel[nCount].pBuffMatModel = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	bool bSwap = GetStageSwat();
	bool bFade = GetFadeNow();
	LOADFADE loadFade = GetLoadFade();

	//モードのポインタ
	MODE pMode = GetMode();

	//カメラの情報
	Camera *pCamera = GetCamera();

	//前回の位置を保存
	g_Player.posOld = g_Player.pos;

	float nSpeed = 5.0f;	//速度
	//float fDiffAngle;

	//壁のあたり判定
	if (g_Player.pos.z >= 740)
	{//上
		g_Player.pos.z = 740;
	}
	if (g_Player.pos.z <= -740)
	{//下
		g_Player.pos.z = -740;
	}
	if (g_Player.pos.x >= 740)
	{//右
		g_Player.pos.x = 740;
	}
	if (g_Player.pos.x <= -740)
	{//左
		g_Player.pos.x = -740;
	}

	//ゲームだったら
	if (pMode == MODE_GAME)
	{
		//フレームカウント
		g_nCntTimePlayer++;

		if (false == bSwap && false == bFade && LOADFADE_NONE == loadFade)
		{// ステージ切り替え中などは動けなくする
			if (GetJoyPadPress(DIJS_BUTTON_10, 0) == true || GetJoyPadPress(DIJS_BUTTON_11, 0) == true || GetJoyPadPress(DIJS_BUTTON_12, 0) == true || GetJoyPadPress(DIJS_BUTTON_13, 0) == true)
			{//ジョイコン
				g_Player.pos.x -= sinf(pCamera->fAngle - GetJotPadLeftStick(0)) * PLAYER1_SPEED;
				g_Player.pos.z -= cosf(pCamera->fAngle - GetJotPadLeftStick(0)) * PLAYER1_SPEED;

				g_Player.rot.y = pCamera->fAngle - GetJotPadLeftStick(0);

				if (g_Player.nAnimnow != PLAYERANIM_WALK && g_Player.bAttack == false)
				{//歩くアニメーション
					g_Player.nAnimnow = PLAYERANIM_WALK;
					g_nCntTimePlayer = 0;

					if (g_Player.bLegWhich == true)
					{// 前回左足を出した
						g_Player.nAnimKeynow = 0;
					}
					else
					{// 前回右足を出した
						g_Player.nAnimKeynow = 3;
					}
				}
			}
			else
			{
				if (g_Player.nAnimnow != PLAYERANIM_NEUTRAL && g_Player.bAttack == false)
				{//待機モーション
					g_Player.nAnimnow = PLAYERANIM_NEUTRAL;
					g_nCntTimePlayer = 0;
					g_Player.nAnimKeynow = 0;
				}
			}

			if (GetJoyPadTrigger(DIJS_BUTTON_2, 0) == true && g_Player.type == TYPE_HUMAN)
			{
				if (g_Player.nAnimnow != PLAYERANIM_ATTACK && g_Player.bAttack == false)
				{//攻撃アニメーション
					g_Player.bAttack = true;

					g_Player.nAnimnow = PLAYERANIM_ATTACK;
					g_nCntTimePlayer = 0;
					g_Player.nAnimKeynow = 0;

					PlaySound(SOUND_LABEL_SWORDGESTURE);
				}
			}

			int nGauge = GetGauge();	// ゲージの取得

			if (nGauge >= MAX_GAUGE)
			{// ゲージが最大値
				if (GetJoyPadTrigger(DIJS_BUTTON_1, 0) == true)
				{// 変身する
					SetParticleEmitter(g_Player.pos, 6);
					g_Player.type = TYPE_DRAGON;
					AddSubtractGauge(-MAX_GAUGE);
				}
			}

			if (g_nCntPlayerType >= DRAGON_TIME)
			{// ドラゴンから人へ
				SetParticleEmitter(g_Player.pos, 6);
				g_Player.type = TYPE_HUMAN;
				g_nCntPlayerType = 0;
			}

			if (g_Player.type == TYPE_DRAGON)
			{
				g_nCntPlayerType++;
			}
		}
		else
		{
			if (g_Player.nAnimnow != PLAYERANIM_NEUTRAL && g_Player.bAttack == false)
			{//待機モーション
				g_Player.nAnimnow = PLAYERANIM_NEUTRAL;
				g_nCntTimePlayer = 0;
				g_Player.nAnimKeynow = 0;
			}
		}

		//位置を移動に代入
		g_Player.pos += g_Player.move;

		//慣性をつける
		g_Player.move.x += (0.0f - g_Player.move.x) * 0.2f;
		g_Player.move.z += (0.0f - g_Player.move.z) * 0.2f;

		//影がついてくる
		SetPositionShadow(g_Player.nIdxShadow, g_Player.pos);

		//あたり判定
		CollisionObject(&g_Player.pos, &g_Player.posOld, &g_Player.move);
		CollisionObject2(&g_Player.pos, &g_Player.posOld, &g_Player.move);
		CollisionObject3(&g_Player.pos, &g_Player.posOld, &g_Player.move);
		CollisionEnemy(&g_Player.pos, &g_Player.posOld, &g_Player.move, 18.0f);
		if (CollisionAttackEnemy(g_Player.pos, 30) && g_Player.type == TYPE_HUMAN)
		{// 敵の攻撃判定
			if (false == g_Player.bDamage)
			{// ダメージ状態でないとき
				PlaySound(SOUND_LABEL_HIT);
				AddSubtractLife(-1);
				g_Player.bDamage = true;
				g_Player.nCntDamage = 0;

				int nLife = GetLife();
				if (nLife <= 0)
				{// 死んだ
					SetBlood(g_Player.pos, g_Player.rot, 50, 50, D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f));
					SetGameState(GAMESTATE_END);
				}
			}
		}

		if (true == g_Player.bDamage)
		{// 状態を戻す
			g_Player.nCntDamage++;	// 無敵時間のカウント

			if (g_Player.nCntDamage >= PLAYER_DAMAGE_TIME)
			{// 一定時間経過
				g_Player.bDamage = false;	// 状態を戻す
			}
		}

		//差分のローカル変数
		float fDes;
		float frac;

		//キーを進める！
		if (g_nCntTimePlayer % g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] == 0)
		{
			g_nCntTimePlayer = 0;
			g_Player.nAnimKeynow = (g_Player.nAnimKeynow + 1) % g_Player.nAnimKey[g_Player.nAnimnow];

			if (g_Player.nAnimnow == PLAYERANIM_ATTACK && g_Player.nAnimKey[g_Player.nAnimnow] - 1 == g_Player.nAnimKeynow)
			{//攻撃アニメーション
				g_Player.nAnimnow = PLAYERANIM_ATTACK;
				g_nCntTimePlayer = 0;
				g_Player.nAnimKeynow = 0;

				g_Player.bAttack = false;
			}

			if (g_Player.nAnimnow == PLAYERANIM_WALK && g_Player.bAttack == false)
			{//歩くアニメーション
			 	if ((g_Player.nAnimKeynow == 1 || g_Player.nAnimKeynow == 4) && g_Player.type == TYPE_HUMAN)
				{// 地面を蹴ったとき
					SetParticleEmitter(D3DXVECTOR3(g_Player.pos.x, 0.0f, g_Player.pos.z), 2);
					PlaySound(SOUND_LABEL_FOOTSTEPS);
				}
				else if(g_Player.nAnimKeynow == 0 || g_Player.nAnimKeynow == 3)
				{// 足音を止める
					StopSound(SOUND_LABEL_FOOTSTEPS);
				}

				if (g_Player.nAnimKeynow < 3)
				{// 左足が前に出た
					g_Player.bLegWhich = false;
				}
				else if (g_Player.nAnimKeynow >= 3)
				{// 右足が前に出た
					g_Player.bLegWhich = true;
				}
			}
			else if(g_Player.nAnimnow != PLAYERANIM_WALK)
			{// 歩行モーション以外で足音を止める
				StopSound(SOUND_LABEL_FOOTSTEPS);
			}
		}

		if (g_Player.pos.y <= 50)
		{// 床
			g_Player.pos.y = 50;
		}

		//差分の計算
		frac = (g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] - g_nCntTimePlayer) * 1.0f;

		//アニメーション
		for (int nCntPlayerAnim = 0; nCntPlayerAnim < g_Player.nMaxPartsNum; nCntPlayerAnim++)
		{
			//===================
			// Xの角度
			//===================

			//目的の角度をしらべる
			g_Player.aModel[nCntPlayerAnim].rotDest = g_Player.aModel[nCntPlayerAnim].rotAnim[g_Player.nAnimnow][g_Player.nAnimKeynow];

			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.x > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.x -= D3DX_PI * 2.0f;
			}
			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.x < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.x += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.x - g_Player.aModel[nCntPlayerAnim].InitRot.x;

			//差分の調節
			if (fDes > D3DX_PI)
			{
				fDes -= D3DX_PI * 2.0f;
			}
			if (fDes > D3DX_PI)
			{
				fDes += D3DX_PI * 2.0f;
			}

			//現在の向きの設定
			g_Player.aModel[nCntPlayerAnim].InitRot.x += fDes / frac;

			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.x > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.x -= D3DX_PI * 2.0f;
			}
			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.x < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.x += D3DX_PI * 2.0f;
			}

			//===================
			// Yの角度
			//===================

			//y目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.y > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.y -= D3DX_PI * 2.0f;
			}
			//y目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.y < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.y += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.y - g_Player.aModel[nCntPlayerAnim].InitRot.y;

			//差分の調節
			if (fDes > D3DX_PI)
			{
				fDes -= D3DX_PI * 2.0f;
			}
			if (fDes > D3DX_PI)
			{
				fDes += D3DX_PI * 2.0f;
			}

			//現在の向きの設定
			g_Player.aModel[nCntPlayerAnim].InitRot.y += fDes / frac;

			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.y > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.y -= D3DX_PI * 2.0f;
			}
			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.y < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.y += D3DX_PI * 2.0f;
			}

			//===================
			// Zの角度
			//===================

			//z目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.z > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.z -= D3DX_PI * 2.0f;
			}
			//z目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.z < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.z += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.z - g_Player.aModel[nCntPlayerAnim].InitRot.z;

			//差分の調節
			if (fDes > D3DX_PI)
			{
				fDes -= D3DX_PI * 2.0f;
			}
			if (fDes > D3DX_PI)
			{
				fDes += D3DX_PI * 2.0f;
			}

			//現在の向きの設定
			g_Player.aModel[nCntPlayerAnim].InitRot.z += fDes / frac;

			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.z > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.z -= D3DX_PI * 2.0f;
			}
			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.z < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.z += D3DX_PI * 2.0f;
			}
		}

		// パーツの位置
		D3DXVECTOR3 DestPos = g_Player.aModel[0].posAnim[g_Player.nAnimnow][g_Player.nAnimKeynow];
		D3DXVECTOR3 defPos;
		frac = (g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] - g_Player.nCntPlayer) * 1.0f;

		defPos = DestPos - g_Player.aModel[0].InitPos;
		g_Player.aModel[0].InitPos.x += defPos.x / frac;
		g_Player.aModel[0].InitPos.y += defPos.y / frac;
		g_Player.aModel[0].InitPos.z += defPos.z / frac;
	}
	else if(pMode == MODE_TITLE)
	{
		//フレームカウント
		g_nCntTimePlayer++;

		if (g_Player.nAnimnow != PLAYERANIM_NEUTRAL && g_Player.bAttack == false)
		{//待機モーション
			g_Player.nAnimnow = PLAYERANIM_NEUTRAL;
			g_nCntTimePlayer = 0;
			g_Player.nAnimKeynow = 0;
		}

		//差分のローカル変数
		float fDes;
		float frac;

		//キーを進める！
		if (g_nCntTimePlayer % g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] == 0)
		{
			g_nCntTimePlayer = 0;
			g_Player.nAnimKeynow = (g_Player.nAnimKeynow + 1) % g_Player.nAnimKey[g_Player.nAnimnow];

			if (g_Player.nAnimnow == PLAYERANIM_ATTACK && g_Player.nAnimKey[g_Player.nAnimnow] - 1 == g_Player.nAnimKeynow)
			{//攻撃アニメーション
				g_Player.nAnimnow = PLAYERANIM_ATTACK;
				g_nCntTimePlayer = 0;
				g_Player.nAnimKeynow = 0;

				g_Player.bAttack = false;
			}
		}

		if (g_Player.pos.y <= 50)
		{// 床
			g_Player.pos.y = 50;
		}

		//差分の計算
		frac = (g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] - g_nCntTimePlayer) * 1.0f;

		//アニメーション
		for (int nCntPlayerAnim = 0; nCntPlayerAnim < g_Player.nMaxPartsNum; nCntPlayerAnim++)
		{
			//===================
			// Xの角度
			//===================

			//目的の角度をしらべる
			g_Player.aModel[nCntPlayerAnim].rotDest = g_Player.aModel[nCntPlayerAnim].rotAnim[g_Player.nAnimnow][g_Player.nAnimKeynow];

			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.x > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.x -= D3DX_PI * 2.0f;
			}
			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.x < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.x += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.x - g_Player.aModel[nCntPlayerAnim].InitRot.x;

			//差分の調節
			if (fDes > D3DX_PI)
			{
				fDes -= D3DX_PI * 2.0f;
			}
			if (fDes > D3DX_PI)
			{
				fDes += D3DX_PI * 2.0f;
			}

			//現在の向きの設定
			g_Player.aModel[nCntPlayerAnim].InitRot.x += fDes / frac;

			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.x > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.x -= D3DX_PI * 2.0f;
			}
			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.x < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.x += D3DX_PI * 2.0f;
			}

			//===================
			// Yの角度
			//===================

			//y目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.y > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.y -= D3DX_PI * 2.0f;
			}
			//y目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.y < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.y += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.y - g_Player.aModel[nCntPlayerAnim].InitRot.y;

			//差分の調節
			if (fDes > D3DX_PI)
			{
				fDes -= D3DX_PI * 2.0f;
			}
			if (fDes > D3DX_PI)
			{
				fDes += D3DX_PI * 2.0f;
			}

			//現在の向きの設定
			g_Player.aModel[nCntPlayerAnim].InitRot.y += fDes / frac;

			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.y > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.y -= D3DX_PI * 2.0f;
			}
			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.y < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.y += D3DX_PI * 2.0f;
			}

			//===================
			// Zの角度
			//===================

			//z目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.z > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.z -= D3DX_PI * 2.0f;
			}
			//z目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].rotDest.z < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.z += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.z - g_Player.aModel[nCntPlayerAnim].InitRot.z;

			//差分の調節
			if (fDes > D3DX_PI)
			{
				fDes -= D3DX_PI * 2.0f;
			}
			if (fDes > D3DX_PI)
			{
				fDes += D3DX_PI * 2.0f;
			}

			//現在の向きの設定
			g_Player.aModel[nCntPlayerAnim].InitRot.z += fDes / frac;

			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.z > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.z -= D3DX_PI * 2.0f;
			}
			//x目的の位置の調整
			if (g_Player.aModel[nCntPlayerAnim].InitRot.z < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.z += D3DX_PI * 2.0f;
			}
		}

		// パーツの位置
		D3DXVECTOR3 DestPos = g_Player.aModel[0].posAnim[g_Player.nAnimnow][g_Player.nAnimKeynow];
		D3DXVECTOR3 defPos;
		frac = (g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] - g_Player.nCntPlayer) * 1.0f;

		defPos = DestPos - g_Player.aModel[0].InitPos;
		g_Player.aModel[0].InitPos.x += defPos.x / frac;
		g_Player.aModel[0].InitPos.y += defPos.y / frac;
		g_Player.aModel[0].InitPos.z += defPos.z / frac;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアルを保存
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;						//

	{//プレイヤー
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player.mtxWorldPlayer);
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
		D3DXMatrixMultiply(&g_Player.mtxWorldPlayer, &g_Player.mtxWorldPlayer, &mtxRot);
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		D3DXMatrixMultiply(&g_Player.mtxWorldPlayer, &g_Player.mtxWorldPlayer, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorldPlayer);
	}
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{
		if (g_Player.aModel[nCntParts].nIdxModelParent < 0)
		{//親モデル
			mtxParent = g_Player.mtxWorldPlayer;
		}
		else
		{//子モデル
			mtxParent = g_Player.aModel[g_Player.aModel[nCntParts].nIdxModelParent].mtxWorldModel;
		}
		{//親モデル
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Player.aModel[nCntParts].mtxWorldModel);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_Player.aModel[nCntParts].InitRot.y,
				g_Player.aModel[nCntParts].InitRot.x,
				g_Player.aModel[nCntParts].InitRot.z
			);
			D3DXMatrixMultiply
			(
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&mtxRot
			);
			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_Player.aModel[nCntParts].InitPos.x,
				g_Player.aModel[nCntParts].InitPos.y,
				g_Player.aModel[nCntParts].InitPos.z
			);
			D3DXMatrixMultiply
			(
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&mtxTrans
			);

			D3DXMatrixMultiply
			(
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&mtxParent
			);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntParts].mtxWorldModel);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Player.aModel[nCntParts].pBuffMatModel->GetBufferPointer();

			// ポリゴンの描画
			if (g_Player.bUse == true)
			{
				for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntParts].nNumMatModel; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// テクスチャをNULLにする
					pDevice->SetTexture(0, NULL);

					//pDevice->SetTexture(0, g_Player.nNumMatPlayer);

					// プレイヤー(パーツ)の描画
					g_Player.aModel[nCntParts].pMeshModel->DrawSubset(nCntMat);
				}
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=========================================================================================================================
// プレイヤーの攻撃の当たり判定
//=========================================================================================================================
bool CollisionAttack(D3DXVECTOR3 pos, float fRange)
{
	bool bAttack = false;

	// 攻撃の当たり判定
	if (g_Player.nAnimnow == PLAYERANIM_ATTACK)
	{// 攻撃モーション中
		if (g_Player.nAnimKeynow >= ATTACK_START && g_Player.nAnimKeynow <= ATTACK_END)
		{// 判定ありのキー範囲中
			float fLengthOneself;

			fLengthOneself = powf(fRange, 3);		// 相手のXZの範囲

			float fLengthX = pos.x - g_Player.aModel[ATTACK_PARTS].mtxWorldModel._41;	// Xの差
			float fLengthY = pos.y - (g_Player.aModel[ATTACK_PARTS].mtxWorldModel._42 + g_Player.aModel[ATTACK_PARTS].vtxMax.y);	// Yの差
			float fLengthZ = pos.z - g_Player.aModel[ATTACK_PARTS].mtxWorldModel._43;	// Zの差
			float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZYの差の二乗

			if (fLengthOneself >= fLengthTotal)
			{// 相手と自分の判定
				if (false == bAttack)
				{// 攻撃判定を有効にする
					bAttack = true;
					return bAttack;
				}
			}
		}
	}

	return bAttack;
}

//=========================================================================================================================
// プレイヤーの取得処理
//=========================================================================================================================
Player *GetPlayer(void)
{
	return &g_Player;
}

//=========================================================================================================================
// プレイヤーの当たり判定
//=========================================================================================================================
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange)
{
	bool bPlayer = false;			// プレイヤーに触れているか
	float fLengthOneself;			// 相手の円状範囲
	float fRange0 = fRange;			// 相手の範囲の半径の長さ

	fLengthOneself = powf(fRange0 * 3, 2);		// 相手のXZの範囲

	float fLengthX = pPos->x - g_Player.mtxWorldPlayer._41;			// Xの差
	float fLengthY = pPos->y - g_Player.mtxWorldPlayer._42;			// Yの差
	float fLengthZ = pPos->z - g_Player.mtxWorldPlayer._43;			// Zの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZYの差の二乗

	if (fLengthOneself >= fLengthTotal)
	{// 相手と自分の判定
		*pPos = *pPosold;									// 位置を戻す
		*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動量を0にする

		bPlayer = true;		// プレイヤーに触れている状態
	}

	return bPlayer;
}
//=========================================================================================================================
// プレイヤーの初期化
//=========================================================================================================================
void DeletePlayer(void)
{
	// 位置・向きの初期設定
	g_Player.pos = D3DXVECTOR3(0, 0, 40);
	g_Player.rot = D3DXVECTOR3(0, 0, 0);
	g_Player.move = D3DXVECTOR3(0, 0, 0);
	g_Player.nCntPlayer = 1;
	g_Player.bUse = true;
	g_Player.bLand = true;
	g_Player.bAttack = false;
	g_Player.nAnimnow = PLAYERANIM_NEUTRAL;
	g_nCntTimePlayer = 0;
	g_Player.nAnimKeynow = 0;
}

//=========================================================================================================================
// 人かドラゴンかの取得
//=========================================================================================================================
PlayerType GetPlayerType(void)
{
	return g_Player.type;
}