//=============================================================================
//
// プレイヤー処理 [dragon.cpp]
// Author : 有馬　武志
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS		//scanfエラー解除
#include <stdio.h>					//stdio
#include "dragon.h"					//プレイヤー
#include "input.h"					//キーボード
#include "camera.h"					//カメラ
#include "shadow.h"					//影
#include "sound.h"					//サウンド
#include "object.h"					//オブジェクト
#include "object2.h"				//オブジェクト
#include "object3.h"				//オブジェクト
#include "enemy.h"
#include "killLog.h"
#include "game.h"
#include "fade.h"
#include "player.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DRAGON1_SPEED			(5.0f)

#define ATTACK_PARTS			(5)
#define ATTACK_START			(0)			// 攻撃の開始キー
#define ATTACK_END				(8)			// 攻撃の終わりキー
#define ATTACK_RADIUS			(10.0f)		// 団子の半径
#define ATTACK_NUM				(3)			// 団子の数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Dragon		g_Dragon;							//プレイヤー
int			g_nCntTimeDragon;					//プレイヤーカウンター
float		g_fJoyStickAngleDragon1;			//
float		g_Dragonboxrot = 0;
bool		g_bOnOffDragn = false;
int			g_nCntAttack;		// 攻撃時間

//=============================================================================
// 初期化処理
//=============================================================================
void InitDragon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイルのポインタ
	FILE *pFile;
	//プレイヤーカウンター
	int nCntDragon;

	// 位置・向きの初期設定
	g_Dragon.pos = D3DXVECTOR3(0, 0, 40);
	g_Dragon.rot = D3DXVECTOR3(0, 0, 0);
	g_Dragon.move = D3DXVECTOR3(0, 0, 0);
	g_Dragon.nCntDragon = 1;
	g_Dragon.bUse = true;
	g_Dragon.bLand = true;
	g_Dragon.bAttack = false;

	g_nCntAttack = 0;

	//ファイル設定
	pFile = fopen(FILE_DRAGON_NAME, "r");

	//プレイヤーの￥テキストデータの読み込み
	if (pFile != NULL)
	{
		//使うパーツ数
		fscanf(pFile, "%d", &g_Dragon.nMaxPartsNum);	//読み込み

		//ファイル名
		for (nCntDragon = 0; nCntDragon < g_Dragon.nMaxPartsNum; nCntDragon++)
		{
			fscanf(pFile, "%s", &g_Dragon.aModel[nCntDragon].FileName[0]); //読み込み
		}

		for (nCntDragon = 0; nCntDragon < g_Dragon.nMaxPartsNum; nCntDragon++)
		{//初期値と向き
			fscanf(pFile, "%d", &g_Dragon.aModel[nCntDragon].nIdxModelParent);	//読み込み
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitPos.x);		//読み込み
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitPos.y);		//読み込み
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitPos.z);		//読み込み
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitRot.x);		//読み込み
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitRot.y);		//読み込み
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitRot.z);		//読み込み
		}

		//ファイル読み込み
		fscanf(pFile, "%d", &g_Dragon.nMaxAnimNum);

		for (int nCntDragonAnim = 0; nCntDragonAnim < g_Dragon.nMaxPartsNum; nCntDragonAnim++)
		{
			//フレーム数
			fscanf(pFile, "%d", &g_Dragon.nAnimloop[nCntDragonAnim]);

			//キー数
			fscanf(pFile, "%d", &g_Dragon.nAnimKey[nCntDragonAnim]);

			for (int nCntDragonAnimKey = 0; nCntDragonAnimKey < g_Dragon.nAnimKey[nCntDragonAnim]; nCntDragonAnimKey++)
			{
				fscanf(pFile, "%d", &g_Dragon.nAnimfram[nCntDragonAnim][nCntDragonAnimKey]);//読み込み

				for (nCntDragon = 0; nCntDragon < g_Dragon.nMaxPartsNum; nCntDragon++)
				{
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].posAnim[nCntDragonAnim][nCntDragonAnimKey].x);		//読み込み
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].posAnim[nCntDragonAnim][nCntDragonAnimKey].y);		//読み込み
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].posAnim[nCntDragonAnim][nCntDragonAnimKey].z);		//読み込み
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].rotAnim[nCntDragonAnim][nCntDragonAnimKey].x);		//読み込み
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].rotAnim[nCntDragonAnim][nCntDragonAnimKey].y);		//読み込み
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].rotAnim[nCntDragonAnim][nCntDragonAnimKey].z);		//読み込み
				}
			}
		}

		fclose(pFile);	//ファイルを閉じる
	}
	else
	{
		MessageBox(0, "セーブファイルのチェックに失敗!", "警告", MB_ICONWARNING);
	}

	//xファイルの読み込み
	for (nCntDragon = 0; nCntDragon < g_Dragon.nMaxPartsNum; nCntDragon++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX
		(
			&g_Dragon.aModel[nCntDragon].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Dragon.aModel[nCntDragon].pBuffMatModel,
			NULL,
			&g_Dragon.aModel[nCntDragon].nNumMatModel,
			&g_Dragon.aModel[nCntDragon].pMeshModel
		);
	}

	Player *pPlayer = GetPlayer();

	if (pPlayer->type == TYPE_DRAGON)
	{
		//影の初期化設定
		g_Dragon.nIdxShadow = SetShadow(g_Dragon.pos, g_Dragon.rot, 25, 25);
	}

	g_Dragon.aModel[ATTACK_PARTS].playerAttack.fRadius = ATTACK_RADIUS;
	g_Dragon.aModel[ATTACK_PARTS].playerAttack.nNumJudgment = ATTACK_NUM;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDragon(void)
{
	for (int nCount = 0; nCount < MAX_DRAGON_PARTS; nCount++)
	{
		// メッシュの開放
		if (g_Dragon.aModel[nCount].pMeshModel != NULL)
		{
			g_Dragon.aModel[nCount].pMeshModel->Release();
			g_Dragon.aModel[nCount].pMeshModel = NULL;
		}
	}
	for (int nCount = 0; nCount < MAX_DRAGON_PARTS; nCount++)
	{
		// マテリアルの開放
		if (g_Dragon.aModel[nCount].pBuffMatModel != NULL)
		{
			g_Dragon.aModel[nCount].pBuffMatModel->Release();
			g_Dragon.aModel[nCount].pBuffMatModel = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDragon(void)
{
	bool bSwap = GetStageSwat();
	bool bFade = GetFadeNow();

	//カメラの情報
	Camera *pCamera = GetCamera();

	Player *pPlayer = GetPlayer();

	//フレームカウント
	g_nCntTimeDragon++;

	//ドラゴンの位置とプレイヤーの位置を合わせる
	g_Dragon.pos = pPlayer->pos;

	//前回の位置を保存
	g_Dragon.posOld = g_Dragon.pos;

	float nSpeed = 5.0f;	//速度
	//float fDiffAngle;

	//壁のあたり判定
	if (g_Dragon.pos.z >= 740)
	{//上
		g_Dragon.pos.z = 740;
	}
	if (g_Dragon.pos.z <= -740)
	{//下
		g_Dragon.pos.z = -740;
	}
	if (g_Dragon.pos.x >= 740)
	{//右
		g_Dragon.pos.x = 740;
	}
	if (g_Dragon.pos.x <= -740)
	{//左
		g_Dragon.pos.x = -740;
	}

	//差分のローカル変数
	//float fDes;
	//float frac;
	int nCount = 0;

	//モードのポインタ
	MODE pMode = GetMode();

	if (pMode == MODE_GAME)
	{
		if (false == bSwap && false == bFade)
		{
			PlayerType type = GetPlayerType();

			if (GetJoyPadPress(DIJS_BUTTON_10, 0) == true || GetJoyPadPress(DIJS_BUTTON_11, 0) == true || GetJoyPadPress(DIJS_BUTTON_12, 0) == true || GetJoyPadPress(DIJS_BUTTON_13, 0) == true)
			{//ジョイコン
				g_Dragon.pos.x -= sinf(pCamera->fAngle - GetJotPadLeftStick(0)) * DRAGON1_SPEED;
				g_Dragon.pos.z -= cosf(pCamera->fAngle - GetJotPadLeftStick(0)) * DRAGON1_SPEED;

				g_Dragon.rot.y = pCamera->fAngle - GetJotPadLeftStick(0);

				if (g_Dragon.nAnimnow != DRAGONANIM_WALK && g_Dragon.bAttack == false)
				{//歩くアニメーション
					g_Dragon.nAnimnow = DRAGONANIM_WALK;
					g_nCntTimeDragon = 0;
					g_Dragon.nAnimKeynow = 0;
				}
			}
			else if (GetJoyPadPress(DIJS_BUTTON_2, 0) == true && type == TYPE_DRAGON)
			{
				if (g_Dragon.nAnimnow != DRAGONANIM_ATTACK && g_Dragon.bAttack == false)
				{//攻撃アニメーション
					g_Dragon.bAttack = true;

					g_Dragon.nAnimnow = DRAGONANIM_ATTACK;
					g_nCntTimeDragon = 0;
					g_Dragon.nAnimKeynow = 0;
					PlaySound(SOUND_LABEL_BREATH);
				}
			}
			else
			{
				if (g_Dragon.nAnimnow != DRAGONANIM_NEUTRAL && g_Dragon.bAttack == false)
				{//待機モーション
					g_Dragon.nAnimnow = DRAGONANIM_NEUTRAL;
					g_nCntTimeDragon = 0;
					g_Dragon.nAnimKeynow = 0;
				}
			}
		}
		else
		{
			if (g_Dragon.nAnimnow != DRAGONANIM_NEUTRAL && g_Dragon.bAttack == false)
			{//待機モーション
				g_Dragon.nAnimnow = DRAGONANIM_NEUTRAL;
				g_nCntTimeDragon = 0;
				g_Dragon.nAnimKeynow = 0;
			}
		}

		if (pPlayer->type == TYPE_DRAGON)
		{// ドラゴン
			//影がついてくる
			SetPositionShadow(g_Dragon.nIdxShadow, g_Dragon.pos);

			//あたり判定
			CollisionObject(&g_Dragon.pos, &g_Dragon.posOld, &g_Dragon.move);
			CollisionObject2(&g_Dragon.pos, &g_Dragon.posOld, &g_Dragon.move);
			CollisionObject3(&g_Dragon.pos, &g_Dragon.posOld, &g_Dragon.move);
			CollisionEnemy(&g_Dragon.pos, &g_Dragon.posOld, &g_Dragon.move, 18.0f);

			// 攻撃の火
			if (g_Dragon.nAnimnow == DRAGONANIM_ATTACK)
			{// 口から火を出す
				if (g_Dragon.nAnimKeynow > 3 && g_Dragon.nAnimKeynow < g_Dragon.nAnimKey[DRAGONANIM_ATTACK])
				{// モーションに合わせる
					SetParticleEmitter(D3DXVECTOR3(g_Dragon.aModel[4].mtxWorldModel._41, g_Dragon.aModel[4].mtxWorldModel._42, g_Dragon.aModel[4].mtxWorldModel._43), 7);
				}
			}

			//差分のローカル変数
			float fDes;
			float frac;

			//キーを進める！
			if (g_nCntTimeDragon % g_Dragon.nAnimfram[g_Dragon.nAnimnow][g_Dragon.nAnimKeynow] == 0)
			{
				g_nCntTimeDragon = 0;
				g_Dragon.nAnimKeynow = (g_Dragon.nAnimKeynow + 1) % g_Dragon.nAnimKey[g_Dragon.nAnimnow];

				if (g_Dragon.nAnimnow == DRAGONANIM_ATTACK && g_Dragon.nAnimKey[g_Dragon.nAnimnow] - 1 == g_Dragon.nAnimKeynow)
				{//攻撃アニメーション
					g_Dragon.nAnimnow = DRAGONANIM_ATTACK;
					g_nCntTimeDragon = 0;
					g_Dragon.nAnimKeynow = 0;
				}
			}

			if (g_Dragon.bAttack)
			{// 攻撃中
				g_nCntAttack++;

				if (g_nCntAttack > 90)
				{
					g_Dragon.bAttack = false;
					g_nCntAttack = 0;
				}
			}

			//差分の計算
			frac = (g_Dragon.nAnimfram[g_Dragon.nAnimnow][g_Dragon.nAnimKeynow] - g_nCntTimeDragon) * 1.0f;

			//アニメーション
			for (int nCntDragonAnim = 0; nCntDragonAnim < g_Dragon.nMaxPartsNum; nCntDragonAnim++)
			{
				//===================
				// Xの角度
				//===================

				//目的の角度をしらべる
				g_Dragon.aModel[nCntDragonAnim].rotDest = g_Dragon.aModel[nCntDragonAnim].rotAnim[g_Dragon.nAnimnow][g_Dragon.nAnimKeynow];

				//x目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.x > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.x -= D3DX_PI * 2.0f;
				}
				//x目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.x < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.x += D3DX_PI * 2.0f;
				}

				//
				fDes = g_Dragon.aModel[nCntDragonAnim].rotDest.x - g_Dragon.aModel[nCntDragonAnim].InitRot.x;

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
				g_Dragon.aModel[nCntDragonAnim].InitRot.x += fDes / frac;

				//x目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.x > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.x -= D3DX_PI * 2.0f;
				}
				//x目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.x < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.x += D3DX_PI * 2.0f;
				}

				//===================
				// Yの角度
				//===================

				//y目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.y > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.y -= D3DX_PI * 2.0f;
				}
				//y目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.y < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.y += D3DX_PI * 2.0f;
				}

				//
				fDes = g_Dragon.aModel[nCntDragonAnim].rotDest.y - g_Dragon.aModel[nCntDragonAnim].InitRot.y;

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
				g_Dragon.aModel[nCntDragonAnim].InitRot.y += fDes / frac;

				//x目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.y > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.y -= D3DX_PI * 2.0f;
				}
				//x目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.y < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.y += D3DX_PI * 2.0f;
				}

				//===================
				// Zの角度
				//===================

				//z目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.z > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.z -= D3DX_PI * 2.0f;
				}
				//z目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.z < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.z += D3DX_PI * 2.0f;
				}

				//
				fDes = g_Dragon.aModel[nCntDragonAnim].rotDest.z - g_Dragon.aModel[nCntDragonAnim].InitRot.z;

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
				g_Dragon.aModel[nCntDragonAnim].InitRot.z += fDes / frac;

				//x目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.z > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.z -= D3DX_PI * 2.0f;
				}
				//x目的の位置の調整
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.z < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.z += D3DX_PI * 2.0f;
				}
			}
			// パーツの位置
			D3DXVECTOR3 DestPos = g_Dragon.aModel[0].posAnim[g_Dragon.nAnimnow][g_Dragon.nAnimKeynow];
			D3DXVECTOR3 defPos;
			frac = (g_Dragon.nAnimfram[g_Dragon.nAnimnow][g_Dragon.nAnimKeynow] - g_Dragon.nCntDragon) * 1.0f;

			defPos = DestPos - g_Dragon.aModel[0].InitPos;
			g_Dragon.aModel[0].InitPos.x += defPos.x / frac;
			g_Dragon.aModel[0].InitPos.y += defPos.y / frac;
			g_Dragon.aModel[0].InitPos.z += defPos.z / frac;
		}
		//位置を移動に代入
		g_Dragon.pos += g_Dragon.move;

		//慣性をつける
		g_Dragon.move.x += (0.0f - g_Dragon.move.x) * 0.2f;
		g_Dragon.move.z += (0.0f - g_Dragon.move.z) * 0.2f;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDragon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアルを保存
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;						//

	{//プレイヤー
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Dragon.mtxWorldDragon);
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Dragon.rot.y, g_Dragon.rot.x, g_Dragon.rot.z);
		D3DXMatrixMultiply(&g_Dragon.mtxWorldDragon, &g_Dragon.mtxWorldDragon, &mtxRot);
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Dragon.pos.x, g_Dragon.pos.y, g_Dragon.pos.z);
		D3DXMatrixMultiply(&g_Dragon.mtxWorldDragon, &g_Dragon.mtxWorldDragon, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Dragon.mtxWorldDragon);
	}
	for (int nCntParts = 0; nCntParts < MAX_DRAGON_PARTS; nCntParts++)
	{
		if (g_Dragon.aModel[nCntParts].nIdxModelParent < 0)
		{//親モデル
			mtxParent = g_Dragon.mtxWorldDragon;
		}
		else
		{//子モデル
			mtxParent = g_Dragon.aModel[g_Dragon.aModel[nCntParts].nIdxModelParent].mtxWorldModel;
		}
		{//親モデル
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Dragon.aModel[nCntParts].mtxWorldModel);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_Dragon.aModel[nCntParts].InitRot.y,
				g_Dragon.aModel[nCntParts].InitRot.x,
				g_Dragon.aModel[nCntParts].InitRot.z
			);
			D3DXMatrixMultiply
			(
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&mtxRot
			);
			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_Dragon.aModel[nCntParts].InitPos.x,
				g_Dragon.aModel[nCntParts].InitPos.y,
				g_Dragon.aModel[nCntParts].InitPos.z
			);
			D3DXMatrixMultiply
			(
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&mtxTrans
			);

			D3DXMatrixMultiply
			(
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&mtxParent
			);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Dragon.aModel[nCntParts].mtxWorldModel);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Dragon.aModel[nCntParts].pBuffMatModel->GetBufferPointer();

			// ポリゴンの描画
			if (g_Dragon.bUse == true)
			{
				for (int nCntMat = 0; nCntMat < (int)g_Dragon.aModel[nCntParts].nNumMatModel; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// テクスチャをNULLにする
					pDevice->SetTexture(0, NULL);

					//pDevice->SetTexture(0, g_Dragon.nNumMatDragon);

					// プレイヤー(パーツ)の描画
					g_Dragon.aModel[nCntParts].pMeshModel->DrawSubset(nCntMat);
				}
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=========================================================================================================================
// プレイヤーの取得処理
//=========================================================================================================================
Dragon *GetDragon(void)
{
	return &g_Dragon;
}

//=========================================================================================================================
// プレイヤーの当たり判定
//=========================================================================================================================
bool CollisionDragon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange)
{
	bool bDragon = false;					// プレイヤーに触れているか
	float fLengthOneself;			// 相手の円状範囲
	float fRange0 = fRange;			// 相手の範囲の半径の長さ

	fLengthOneself = powf(fRange0 * 3, 2);		// 相手のXZの範囲

	float fLengthX = pPos->x - g_Dragon.mtxWorldDragon._41;			// Xの差
	float fLengthY = pPos->y - g_Dragon.mtxWorldDragon._42;	// Yの差
	float fLengthZ = pPos->z - g_Dragon.mtxWorldDragon._43;			// Zの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZYの差の二乗

	if (fLengthOneself >= fLengthTotal)
	{// 相手と自分の判定
		*pPos = *pPosold;									// 位置を戻す
		*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動量を0にする

		bDragon = true;		// プレイヤーに触れている状態
	}

	return bDragon;
}
//=========================================================================================================================
// プレイヤーの初期化
//=========================================================================================================================
void DeleteDragon(void)
{
	// 位置・向きの初期設定
	g_Dragon.pos = D3DXVECTOR3(0, 0, 40);
	g_Dragon.rot = D3DXVECTOR3(0, 0, 0);
	g_Dragon.move = D3DXVECTOR3(0, 0, 0);
	g_Dragon.nCntDragon = 1;
	g_Dragon.bUse = true;
	g_Dragon.bLand = true;
	g_Dragon.bAttack = false;
}

//=========================================================================================================================
// ドラゴンの位置の設定
//=========================================================================================================================
void SetPosDragon(D3DXVECTOR3 pos)
{
	g_Dragon.pos = pos;
}

//=========================================================================================================================
// ドラゴンの位置の取得
//=========================================================================================================================
D3DXVECTOR3 GetPosDragon(void)
{
	return g_Dragon.pos;
}

//=========================================================================================================================
// 首の角度の取得
//=========================================================================================================================
D3DXVECTOR3 GetNeckRot(void)
{
	//return g_Dragon.aModel[3].mtxWorldModel._32;

	return g_Dragon.rot;
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CollisionDragonAttack(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// 攻撃の当たり判定
	if (g_Dragon.nAnimnow == PLAYERANIM_ATTACK)
	{// 攻撃モーション中
		if (g_Dragon.nAnimKeynow >= 3 && g_Dragon.nAnimKeynow <= g_Dragon.nAnimKey[g_Dragon.nAnimnow])
		{// 判定ありのキー範囲中

			float fLengthOneself;		// 相手の円状範囲
			float fRange0 = fRange;		// 相手の範囲の半径の長さ

			fLengthOneself = powf(fRange0 * 2, 2);		// 相手のXZの範囲

			float fLengthX = pos.x - (g_Dragon.mtxWorldDragon._41 + (sinf(g_Dragon.rot.y) * 1.0f));	// Xの差
			float fLengthY = pos.y - g_Dragon.mtxWorldDragon._42;	// Yの差
			float fLengthZ = pos.z - (g_Dragon.mtxWorldDragon._43 + (cosf(g_Dragon.rot.y) * 1.0f));	// Zの差
			float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZYの差の二乗

			if (fLengthOneself >= fLengthTotal)
			{// 相手と自分の判定
				bHit = true;
			}
		}
	}
	return bHit;
}