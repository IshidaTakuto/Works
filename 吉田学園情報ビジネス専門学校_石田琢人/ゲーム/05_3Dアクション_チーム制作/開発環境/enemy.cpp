//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author : 有馬　武志
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS		//scanfエラー解除
#include <stdio.h>					//stdio
#include "enemy.h"					//敵
#include "input.h"					//キーボード
#include "camera.h"					//カメラ
#include "shadow.h"					//影
#include "sound.h"					//サウンド
#include "player.h"					//プレイヤー
#include "object.h"					//オブジェクト
#include "object2.h"					//オブジェクト
#include "object3.h"					//オブジェクト
#include "Blood.h"
#include "particle.h"
#include "game.h"
#include "clear.h"
#include "fade.h"
#include "remnantsEnemy.h"
#include "combo.h"
#include "killLog.h"
#include "load.h"
#include "gauge.h"
#include "dragon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY1_SPEED			(5.0f)
#define MAX_ENEMY				(30)
#define PULS					(20)

#define ENEMY_LIFE				(3)		// 敵のライフ

#define ATTACK_PARTS			(5)
#define ATTACK_START			(3)			// 攻撃の開始キー
#define ATTACK_END				(4)			// 攻撃の終わりキー
#define DAMAGE_TIME_ENEMY		(30)		// ダメージ状態のフレーム

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Enemy		g_Enemy[MAX_ENEMY];					// 敵
float		g_fJoyStickAngleEnemy1;				//
int g_nNumEnemy;								// ステージの敵の総数
int g_nNumAppearEnemy;							// 出現中の敵の総数
int g_nClearMessage;							// クリアメッセージがでているか

//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイルのポインタ
	FILE *pFile;
	//敵カウンター
	int nCntEnemy;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 位置・向きの初期設定
		g_Enemy[nCntEnemy].pos = D3DXVECTOR3(-300.0f +(100 * nCntEnemy), 50, -40);
		g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0, 0, 0);
		g_Enemy[nCntEnemy].move = D3DXVECTOR3(0, 0, 0);
		g_Enemy[nCntEnemy].bUse = false;
		g_Enemy[nCntEnemy].bLand = false;
		g_Enemy[nCntEnemy].bAttack = false;
		g_Enemy[nCntEnemy].nAttackCount = 0;
		g_Enemy[nCntEnemy].nLife = 0;
		g_Enemy[nCntEnemy].bDamage = false;
		g_Enemy[nCntEnemy].nCntDamage = 0;
	}

	g_nNumEnemy = 0;
	g_nNumAppearEnemy = 0;
	g_nClearMessage = 0;

	for (int nCntEnemyAll = 0; nCntEnemyAll < MAX_ENEMY; nCntEnemyAll++)
	{
		//ファイル設定
		pFile = fopen(TEXT_NAME, "r");

		//敵の￥テキストデータの読み込み
		if (pFile != NULL)
		{
			//使うパーツ数
			fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].nMaxPartsNum);	//読み込み

			//ファイル名
			for (nCntEnemy = 0; nCntEnemy < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntEnemy++)
			{
				fscanf(pFile, "%s", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].FileName[0]); //読み込み
			}

			for (nCntEnemy = 0; nCntEnemy < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntEnemy++)
			{//初期値と向き
				fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].nIdxModelParent);//読み込み
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitPos.x);//読み込み
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitPos.y);//読み込み
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitPos.z);//読み込み
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitRot.x);//読み込み
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitRot.y);//読み込み
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitRot.z);//読み込み
			}

			//ファイル読み込み
			fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].nMaxAnimNum);

			for (int nCntEnemyAnim = 0; nCntEnemyAnim < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntEnemyAnim++)
			{
				//フレーム数
				fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].nAnimloop[nCntEnemyAnim]);

				//キー数
				fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].nAnimKey[nCntEnemyAnim]);

				for (int nCntEnemyAnimKey = 0; nCntEnemyAnimKey < g_Enemy[nCntEnemyAll].nAnimKey[nCntEnemyAnim]; nCntEnemyAnimKey++)
				{
					fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].nAnimfram[nCntEnemyAnim][nCntEnemyAnimKey]);//読み込み

					for (nCntEnemy = 0; nCntEnemy < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntEnemy++)
					{
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].posAnim[nCntEnemyAnim][nCntEnemyAnimKey].x);		//読み込み
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].posAnim[nCntEnemyAnim][nCntEnemyAnimKey].y);		//読み込み
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].posAnim[nCntEnemyAnim][nCntEnemyAnimKey].z);		//読み込み
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].rotAnim[nCntEnemyAnim][nCntEnemyAnimKey].x);		//読み込み
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].rotAnim[nCntEnemyAnim][nCntEnemyAnimKey].y);		//読み込み
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].rotAnim[nCntEnemyAnim][nCntEnemyAnimKey].z);		//読み込み
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
		for (nCntEnemy = 0; nCntEnemy < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntEnemy++)
		{
			// Xファイルの読み込み
			D3DXLoadMeshFromX
			(
				&g_Enemy[nCntEnemyAll].aModel[nCntEnemy].FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_Enemy[nCntEnemyAll].aModel[nCntEnemy].pBuffMatModel,
				NULL,
				&g_Enemy[nCntEnemyAll].aModel[nCntEnemy].nNumMatModel,
				&g_Enemy[nCntEnemyAll].aModel[nCntEnemy].pMeshModel
			);
		}

		int nNumVtx;			//頂点数
		DWORD sizeFVF;			//頂点フォーマットのサイズ
		BYTE *pVtxBuff;			//頂点バッファへのポインタ

		for (int nCntModel = 0; nCntModel < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntModel++)
		{
			//頂点数を取得
			nNumVtx = g_Enemy[nCntEnemyAll].aModel[nCntModel].pMeshModel->GetNumVertices();

			//頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(g_Enemy[nCntEnemyAll].aModel[nCntModel].pMeshModel->GetFVF());

			//頂点バッファをロック
			g_Enemy[nCntEnemyAll].aModel[nCntModel].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標の代入

				//範囲指定
				//最大値
				if (vtx.x < g_Enemy[nCntEnemyAll].VtxMin.x)
				{
					g_Enemy[nCntEnemyAll].VtxMin.x = vtx.x;
				}
				if (vtx.z < g_Enemy[nCntEnemyAll].VtxMin.z)
				{
					g_Enemy[nCntEnemyAll].VtxMin.z = vtx.z;
				}
				if (vtx.y < g_Enemy[nCntEnemyAll].VtxMin.y)
				{
					g_Enemy[nCntEnemyAll].VtxMin.y = vtx.y;
				}
				//最小値
				if (vtx.x > g_Enemy[nCntEnemyAll].VtxMax.x)
				{
					g_Enemy[nCntEnemyAll].VtxMax.x = vtx.x;
				}
				if (vtx.z > g_Enemy[nCntEnemyAll].VtxMax.z)
				{
					g_Enemy[nCntEnemyAll].VtxMax.z = vtx.z;
				}
				if (vtx.y > g_Enemy[nCntEnemyAll].VtxMax.y)
				{
					g_Enemy[nCntEnemyAll].VtxMax.y = vtx.y;
				}
				pVtxBuff += sizeFVF;		//サイズ分ポインタを進める
			}

			// 頂点バッファをアンロック
			g_Enemy[nCntEnemyAll].aModel[nCntModel].pMeshModel->UnlockVertexBuffer();
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		for (int nCount = 0; nCount < MAX_PARTS; nCount++)
		{
			// メッシュの開放
			if (g_Enemy[nCntEnemy].aModel[nCount].pMeshModel != NULL)
			{
				g_Enemy[nCntEnemy].aModel[nCount].pMeshModel->Release();
				g_Enemy[nCntEnemy].aModel[nCount].pMeshModel = NULL;
			}
		}
		for (int nCount = 0; nCount < MAX_PARTS; nCount++)
		{
			// マテリアルの開放
			if (g_Enemy[nCntEnemy].aModel[nCount].pBuffMatModel != NULL)
			{
				g_Enemy[nCntEnemy].aModel[nCount].pBuffMatModel->Release();
				g_Enemy[nCntEnemy].aModel[nCount].pBuffMatModel = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	bool bSwap = GetStageSwat();
	bool bFade = GetFadeNow();
	LOADFADE loadFade = GetLoadFade();

	//カメラの情報
	Camera *pCamera = GetCamera();

	//プレイヤーのポインタ
	Player *pPlayer = GetPlayer();

	if (g_nClearMessage == 1)
	{// 敵をすべて倒した
		SetClearMessage(TYPE_COMPLETE);
		g_nClearMessage = 2;
	}
	else if (g_nClearMessage == 0)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if (g_Enemy[nCntEnemy].bUse == true)
			{
				//フレームカウント
				g_Enemy[nCntEnemy].nCntTimeEnemy++;

				//前回の位置を保存
				g_Enemy[nCntEnemy].posOld = g_Enemy[nCntEnemy].pos;

				float nSpeed = 5.0f;	//速度
										//float fDiffAngle;

				//壁のあたり判定
				if (g_Enemy[nCntEnemy].pos.z >= 740)
				{//上
					g_Enemy[nCntEnemy].pos.z = 740;
				}
				if (g_Enemy[nCntEnemy].pos.z <= -740)
				{//下
					g_Enemy[nCntEnemy].pos.z = -740;
				}
				if (g_Enemy[nCntEnemy].pos.x >= 740)
				{//右
					g_Enemy[nCntEnemy].pos.x = 740;
				}
				if (g_Enemy[nCntEnemy].pos.x <= -740)
				{//左
					g_Enemy[nCntEnemy].pos.x = -740;
				}

				if (false == bSwap && false == bFade && LOADFADE_NONE == loadFade)
				{
					if (g_Enemy[nCntEnemy].pos.x <= pPlayer->pos.x + 80 && g_Enemy[nCntEnemy].pos.x >= pPlayer->pos.x - 80 && g_Enemy[nCntEnemy].pos.z <= pPlayer->pos.z + 80 && g_Enemy[nCntEnemy].pos.z >= pPlayer->pos.z - 80)
					{
						g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

						if (g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_NEUTRAL && g_Enemy[nCntEnemy].bAttack == false)
						{//歩くアニメーション
							g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_NEUTRAL;
							g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
							g_Enemy[nCntEnemy].nAnimKeynow = 0;
						}

						if (g_Enemy[nCntEnemy].nAttackCount >= 239)
						{
							if (g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_ATTACK && g_Enemy[nCntEnemy].bAttack == false)
							{//攻撃アニメーション
								g_Enemy[nCntEnemy].bAttack = true;

								g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_ATTACK;
								g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
								g_Enemy[nCntEnemy].nAnimKeynow = 0;
							}
						}

						g_Enemy[nCntEnemy].nAttackCount++;
					}
					//プレイヤーに追尾
					else if (g_Enemy[nCntEnemy].pos.x <= pPlayer->pos.x + 300 && g_Enemy[nCntEnemy].pos.x >= pPlayer->pos.x - 300 && g_Enemy[nCntEnemy].pos.z <= pPlayer->pos.z + 300 && g_Enemy[nCntEnemy].pos.z >= pPlayer->pos.z - 300 && g_Enemy[nCntEnemy].move.y <= 0.1f)
					{
						//プレイヤーの位置に向かって移動する
						g_Enemy[nCntEnemy].move = D3DXVECTOR3(sinf(atan2f(-g_Enemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_Enemy[nCntEnemy].pos.z - -pPlayer->pos.z)) * 2.5f, 0.0f, cosf(atan2f(-g_Enemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_Enemy[nCntEnemy].pos.z - -pPlayer->pos.z)) * 2.5f);

						//プレイヤーのほうに向く
						g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, atan2f(-g_Enemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_Enemy[nCntEnemy].pos.z - -pPlayer->pos.z) + D3DX_PI, 0.0f);

						if (g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_WALK && g_Enemy[nCntEnemy].bAttack == false)
						{//歩くアニメーション
							g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_WALK;
							g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
							g_Enemy[nCntEnemy].nAnimKeynow = 0;
						}
					}
					else if (g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_NEUTRAL && g_Enemy[nCntEnemy].bAttack == false)
					{//歩くアニメーション
						g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_NEUTRAL;
						g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
						g_Enemy[nCntEnemy].nAnimKeynow = 0;
					}
				}
				else
				{
					if (g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_NEUTRAL && g_Enemy[nCntEnemy].bAttack == false)
					{//待機モーション
						g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_NEUTRAL;
						g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
						g_Enemy[nCntEnemy].nAnimKeynow = 0;
					}
				}

				//影がついてくる
				SetPositionShadow(g_Enemy[nCntEnemy].nIdxShadow, g_Enemy[nCntEnemy].pos);

				//慣性をつける
				g_Enemy[nCntEnemy].move.x += (0.0f - g_Enemy[nCntEnemy].move.x) * 0.2f;
				g_Enemy[nCntEnemy].move.z += (0.0f - g_Enemy[nCntEnemy].move.z) * 0.2f;
				g_Enemy[nCntEnemy].blowMove.x += (0.0f - g_Enemy[nCntEnemy].blowMove.x) * 0.2f;
				g_Enemy[nCntEnemy].blowMove.y += (0.0f - g_Enemy[nCntEnemy].blowMove.y) * 0.2f;
				g_Enemy[nCntEnemy].blowMove.z += (0.0f - g_Enemy[nCntEnemy].blowMove.z) * 0.2f;

				//位置を移動に代入
				g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;
				g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].blowMove;

				g_Enemy[nCntEnemy].pos.y -= GRAVITY;	// 重力を加算

				if (g_Enemy[nCntEnemy].pos.y <= 50)
				{// 床
					g_Enemy[nCntEnemy].pos.y = 50;
				}

				//あたり判定
				CollisionObject(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move);
				CollisionObject2(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move);
				CollisionObject3(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move);
				CollisionEnemies(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move, 10, nCntEnemy);

				// ダメージを受けた
				if (CollisionAttack(g_Enemy[nCntEnemy].pos, g_Enemy[nCntEnemy].VtxMax.x) == true && g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_DAMAGE && !g_Enemy[nCntEnemy].bDamage)
				{
					PlaySound(SOUND_LABEL_SWORDHIT);	// ヒット音
					SetParticleEmitter(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 20, g_Enemy[nCntEnemy].pos.z), 1);	// 敵から血を出す
					AddCombo();
					g_Enemy[nCntEnemy].bDamage = true;
					g_Enemy[nCntEnemy].nCntDamage = 0;
					g_Enemy[nCntEnemy].nLife--;
					AddSubtractGauge(1);

					// 吹っ飛ぶ角度
					float fAngle = atan2f(pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x, pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z);

					// 吹っ飛ぶ距離
					g_Enemy[nCntEnemy].blowMove = D3DXVECTOR3(-sinf(fAngle) * 25.0f, 10.0f, -cosf(fAngle) * 25.0f);

				}

				PlayerType type = GetPlayerType();

				if(CollisionDragonAttack(g_Enemy[nCntEnemy].pos, 60) && g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_DAMAGE && !g_Enemy[nCntEnemy].bDamage && type == TYPE_DRAGON)
				{// ドラゴンの火
					PlaySound(SOUND_LABEL_HIT);	// ヒット音
					SetParticleEmitter(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 20, g_Enemy[nCntEnemy].pos.z), 1);	// 敵から血を出す
					AddCombo();
					g_Enemy[nCntEnemy].bDamage = true;
					g_Enemy[nCntEnemy].nCntDamage = 0;
					g_Enemy[nCntEnemy].nLife--;
					AddSubtractGauge(1);
				}

				if (g_Enemy[nCntEnemy].nLife <= 0)
				{// 死んだとき
					PlaySound(SOUND_LABEL_ENEMYVOICE);				// 鳴き声
					g_Enemy[nCntEnemy].bUse = false;				// 敵を消す
					DeleteShadow(g_Enemy[nCntEnemy].nIdxShadow);	// 影を消す
					g_nNumEnemy--;
					g_nNumAppearEnemy--;
					SetRemnants(g_nNumEnemy);
					if (g_nNumEnemy <= 0)
					{
						g_nClearMessage = 1;
					}
					AddKill();

					// 地面に血を出す
					SetBlood(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, 0.0f, g_Enemy[nCntEnemy].pos.z), D3DXVECTOR3(g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.y - (rand() % 30), g_Enemy[nCntEnemy].rot.z), 40.0f + (rand() % 10), 40.0f + (rand() % 10), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
				}

				// ダメージ状態
				if (g_Enemy[nCntEnemy].bDamage)
				{
					g_Enemy[nCntEnemy].nCntDamage++;

					if (g_Enemy[nCntEnemy].nCntDamage >= DAMAGE_TIME_ENEMY)
					{
						g_Enemy[nCntEnemy].bDamage = false;
					}
				}

				//差分のローカル変数
				float fDes;
				float frac;

				//キーを進める！
				if (g_Enemy[nCntEnemy].nCntTimeEnemy % g_Enemy[nCntEnemy].nAnimfram[g_Enemy[nCntEnemy].nAnimnow][g_Enemy[nCntEnemy].nAnimKeynow] == 0)
				{
					g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
					g_Enemy[nCntEnemy].nAnimKeynow = (g_Enemy[nCntEnemy].nAnimKeynow + 1) % g_Enemy[nCntEnemy].nAnimKey[g_Enemy[nCntEnemy].nAnimnow];

					if (g_Enemy[nCntEnemy].nAnimnow == ENEMYANIM_ATTACK && g_Enemy[nCntEnemy].nAnimKey[g_Enemy[nCntEnemy].nAnimnow] - 1 == g_Enemy[nCntEnemy].nAnimKeynow)
					{//歩くアニメーション
						g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_ATTACK;
						g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
						g_Enemy[nCntEnemy].nAnimKeynow = 0;
						g_Enemy[nCntEnemy].nAttackCount = 0;
						g_Enemy[nCntEnemy].bAttack = false;
					}
				}

				//差分の計算
				frac = (g_Enemy[nCntEnemy].nAnimfram[g_Enemy[nCntEnemy].nAnimnow][g_Enemy[nCntEnemy].nAnimKeynow] - g_Enemy[nCntEnemy].nCntTimeEnemy) * 1.0f;

				//アニメーション
				for (int nCntEnemyAnim = 0; nCntEnemyAnim < g_Enemy[nCntEnemy].nMaxPartsNum; nCntEnemyAnim++)
				{
					//===================
					// Xの角度
					//===================

					//目的の角度をしらべる
					g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest = g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotAnim[g_Enemy[nCntEnemy].nAnimnow][g_Enemy[nCntEnemy].nAnimKeynow];

					//x目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.x > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.x -= D3DX_PI * 2.0f;
					}
					//x目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.x < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.x += D3DX_PI * 2.0f;
					}

					//
					fDes = g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.x - g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x;

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
					g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x += fDes / frac;

					//x目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x -= D3DX_PI * 2.0f;
					}
					//x目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x += D3DX_PI * 2.0f;
					}

					//===================
					// Yの角度
					//===================

					//y目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.y > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.y -= D3DX_PI * 2.0f;
					}
					//y目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.y < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.y += D3DX_PI * 2.0f;
					}

					//
					fDes = g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.y - g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y;

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
					g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y += fDes / frac;

					//x目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y -= D3DX_PI * 2.0f;
					}
					//x目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y += D3DX_PI * 2.0f;
					}

					//===================
					// Zの角度
					//===================

					//z目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.z > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.z -= D3DX_PI * 2.0f;
					}
					//z目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.z < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.z += D3DX_PI * 2.0f;
					}

					//
					fDes = g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.z - g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z;

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
					g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z += fDes / frac;

					//x目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z -= D3DX_PI * 2.0f;
					}
					//x目的の位置の調整
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z += D3DX_PI * 2.0f;
					}
				}
				// パーツの位置
				D3DXVECTOR3 DestPos = g_Enemy[nCntEnemy].aModel[0].posAnim[g_Enemy[nCntEnemy].nAnimnow][g_Enemy[nCntEnemy].nAnimKeynow];
				D3DXVECTOR3 defPos;
				frac = (g_Enemy[nCntEnemy].nAnimfram[g_Enemy[nCntEnemy].nAnimnow][g_Enemy[nCntEnemy].nAnimKeynow] - g_Enemy[nCntEnemy].nCntTimeEnemy) * 1.0f;

				defPos = DestPos - g_Enemy[nCntEnemy].aModel[0].InitPos;
				g_Enemy[nCntEnemy].aModel[0].InitPos.x += defPos.x / frac;
				g_Enemy[nCntEnemy].aModel[0].InitPos.y += defPos.y / frac;
				g_Enemy[nCntEnemy].aModel[0].InitPos.z += defPos.z / frac;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアルを保存
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;						//

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//敵
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorldEnemy);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[nCntEnemy].rot.y, g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxTrans);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorldEnemy);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (g_Enemy[nCntEnemy].aModel[nCntParts].nIdxModelParent < 0)
			{//親モデル
				mtxParent = g_Enemy[nCntEnemy].mtxWorldEnemy;
			}
			else
			{//子モデル
				mtxParent = g_Enemy[nCntEnemy].aModel[g_Enemy[nCntEnemy].aModel[nCntParts].nIdxModelParent].mtxWorldModel;
			}
			{//親モデル
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel);
				// 回転を反映
				D3DXMatrixRotationYawPitchRoll
				(
					&mtxRot,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitRot.y,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitRot.x,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitRot.z
				);
				D3DXMatrixMultiply
				(
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&mtxRot
				);
				// 位置を反映
				D3DXMatrixTranslation
				(
					&mtxTrans,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitPos.x,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitPos.y,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitPos.z
				);
				D3DXMatrixMultiply
				(
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&mtxTrans
				);

				D3DXMatrixMultiply
				(
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&mtxParent
				);
				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].aModel[nCntParts].pBuffMatModel->GetBufferPointer();

				// ポリゴンの描画
				if (g_Enemy[nCntEnemy].bUse == true)
				{
					for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].aModel[nCntParts].nNumMatModel; nCntMat++)
					{
						// マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						// テクスチャをNULLにする
						pDevice->SetTexture(0, NULL);

						//pDevice->SetTexture(0, g_Enemy[nCntEnemy].nNumMatEnemy);

						// 敵(パーツ)の描画
						g_Enemy[nCntEnemy].aModel[nCntParts].pMeshModel->DrawSubset(nCntMat);
					}
				}
				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}
//=========================================================================================================================
// 敵の取得処理
//=========================================================================================================================
Enemy *GetEnemy(void)
{
	return &g_Enemy[0];
}

//=============================================================================
// 敵同士の当たり判定
//=============================================================================
bool CollisionEnemies(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange, int nIdx)
{
	int nCount;
	bool bEnemy = false;

	for (nCount = 0; nCount < MAX_ENEMY; nCount++)
	{// 全ての敵分回す
		if (true == g_Enemy[nCount].bUse)
		{// 使用されている敵のみ
			if (nCount != nIdx)
			{// 自分の番号のときはスキップ
				float fLengthOneself;		// 相手の円状範囲
				float fRange0 = fRange;		// 相手の範囲の半径の長さ

				fLengthOneself = powf(fRange0 * 3, 2);		// 相手のXZの範囲

				float fLengthX = pPos->x - g_Enemy[nCount].mtxWorldEnemy._41;	// Xの差
				float fLengthY = pPos->y - g_Enemy[nCount].mtxWorldEnemy._42;	// Yの差
				float fLengthZ = pPos->z - g_Enemy[nCount].mtxWorldEnemy._43;	// Zの差
				float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZYの差の二乗

				if (fLengthOneself >= fLengthTotal)
				{// 相手と自分の判定
					*pPos = *pPosold;							// 位置を戻す
					*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量を0にする

					bEnemy = true;
				}
			}
		}
	}
	return bEnemy;
}

//=============================================================================
// 敵との当たり判定
//=============================================================================
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange)
{
	int nCount;
	bool bEnemy = false;

	for (nCount = 0; nCount < MAX_ENEMY; nCount++)
	{// 全ての敵分回す
		if (true == g_Enemy[nCount].bUse)
		{// 使用されている敵のみ
			float fLengthOneself;		// 相手の円状範囲
			float fRange0 = fRange;		// 相手の範囲の半径の長さ

			fLengthOneself = powf(fRange0, 3);		// 相手のXZの範囲

			float fLengthX = pPos->x - g_Enemy[nCount].mtxWorldEnemy._41;	// Xの差
			float fLengthY = pPos->y - g_Enemy[nCount].mtxWorldEnemy._42;	// Yの差
			float fLengthZ = pPos->z - g_Enemy[nCount].mtxWorldEnemy._43;	// Zの差
			float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);		// XZYの差の二乗

			if (fLengthOneself >= fLengthTotal)
			{// 相手と自分の判定
				*pPos = *pPosold;							// 位置を戻す
				*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量を0にする

				bEnemy = true;
			}
		}
	}
	return bEnemy;
}

//=========================================================================================================================
// 攻撃の当たり判定
//=========================================================================================================================
bool CollisionAttackEnemy(D3DXVECTOR3 pos, float vtxMaxX)
{
	bool bAttack = false;
	int nCount;

	for (nCount = 0; nCount < MAX_ENEMY; nCount++)
	{// 全ての敵分回す
		if (true == g_Enemy[nCount].bUse)
		{// 使用されている敵のみ
			if (g_Enemy[nCount].nAnimnow == PLAYERANIM_ATTACK)
			{// 攻撃モーション中
				if (g_Enemy[nCount].nAnimKeynow >= ATTACK_START && g_Enemy[nCount].nAnimKeynow <= ATTACK_END)
				{// 判定ありのキー範囲中
					float fLengthOneself;
					float fRange0 = vtxMaxX;

					fLengthOneself = powf(fRange0 * 2, 2);		// 相手のXZの範囲

					float fLengthX = pos.x - g_Enemy[nCount].aModel[ATTACK_PARTS].mtxWorldModel._41;	// Xの差
					float fLengthY = pos.y - g_Enemy[nCount].aModel[ATTACK_PARTS].mtxWorldModel._42;	// Yの差
					float fLengthZ = pos.z - g_Enemy[nCount].aModel[ATTACK_PARTS].mtxWorldModel._43;	// Zの差
					float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZの差の二乗

					if (fLengthOneself >= fLengthTotal)
					{// 相手と自分の判定
						if (false == bAttack)
						{// 攻撃判定を有効にする
							bAttack = true;
						}
					}
				}
			}
		}
	}
	return bAttack;
}

//=============================================================================
// 敵の設定
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCount;

	for (nCount = 0; nCount < MAX_ENEMY; nCount++)
	{// 全ての敵分回す
		if (false == g_Enemy[nCount].bUse)
		{// 使用されていない敵のみ
			int nStage = GetNumState();

			if (nStage == 0)
			{// 0割防止
				nStage = 1;
			}

			g_Enemy[nCount].pos = pos;
			g_Enemy[nCount].nAnimnow = ENEMYANIM_NEUTRAL;
			g_Enemy[nCount].nCntTimeEnemy = 0;
			g_Enemy[nCount].nAnimKeynow = 0;
			g_Enemy[nCount].nAttackCount = 0;
			g_Enemy[nCount].bAttack = false;
			g_Enemy[nCount].nLife = 2 + rand() % nStage;
			g_Enemy[nCount].bUse = true;

			g_nNumAppearEnemy++;
			g_Enemy[nCount].nIdxShadow = SetShadow(g_Enemy[nCount].pos, g_Enemy[nCount].rot, 25, 25);

			SetParticleEmitter(g_Enemy[nCount].pos, 8);
			break;
		}
	}
}
//
//
//
void DeleteEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 位置・向きの初期設定
		g_Enemy[nCntEnemy].pos = D3DXVECTOR3(-300.0f + (100 * nCntEnemy), 0, -40);
		g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0, 0, 0);
		g_Enemy[nCntEnemy].move = D3DXVECTOR3(0, 0, 0);
		g_Enemy[nCntEnemy].bUse = false;
		g_Enemy[nCntEnemy].bLand = true;
		g_Enemy[nCntEnemy].bAttack = false;
		g_Enemy[nCntEnemy].nAttackCount = 0;
		DeleteShadow(g_Enemy[nCntEnemy].nIdxShadow);
		g_nClearMessage = 0;
		g_nNumAppearEnemy = 0;
	}
}

//=============================================================================
// 出現中の敵の総数の取得
//=============================================================================
int GetNumEnemy(void)
{
	return g_nNumAppearEnemy;
}

//=============================================================================
// 敵の総数の設定
//=============================================================================
void SetNumEnemy(int nNumEnemy)
{
	g_nNumEnemy = nNumEnemy;
}