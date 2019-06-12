//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 杉本涼
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS			//Fire Wall突破
#include <stdio.h>						//インクルドファイル
#include <math.h>
#include "model.h"						//モデル
#include "camera.h"						//カメラ
//#include "input.h"						//キーボード
#include "meshField.h"
#include "game.h"
#include "object.h"
#include "fade.h"
#include "joypad.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_MODEL_TYPE		(MODEL_TYPE_EMPTY)	//モデルの種類
#define	MAX_MODEL_TEX		(15)	//モデルの種類

#define	MAX_MODEL_INFO		(100)	//モデルの数
#define	MAX_MODEL_CHAR		(8)		//モーションの数
#define	MAX_MODEL_PARE		(16)	//モデルの数
#define	MAX_ENEMY_DATA		(16)	//モデルの数
#define	HIT_HIGHT			(20.0f)	//モデルの数

#define	MODEL_PMOVE			(0.00005f)//プレイヤーの移動速度 倍率
#define	MODEL_SMOVE			(25)//プレイヤーの	スタミナ 倍率
#define	MODEL_PMOVE0		(0.75f)	//プレイヤーの移動速度 初期値
#define	MODEL_RUN_MOVE		(2.25f)	//プレイヤーの移動速度 初期値
#define	INIT_EXP			(50000.0f)	//初期経験値
#define	EXP_UP				(1.1f)	//次の経験値の倍率

#define	MOTION_LAND			(3)		//着地モーション
#define	MOTION_LANDING		(4)		//ジャンプモーション
#define	MOTION_AIR			(5)		//空中モーション
#define	MOTION_STEP			(6)		//ステップモーション
#define	MOTION_STEPS		(7)		//ステップモーション
#define	MOTION_GUARD		(8)		//ガードモーション 成功時ガード初めのキーに戻る
#define	MOTION_SHRINK		(9)		//怯み
#define	MOTION_DOWN			(10)	//ダウン
#define	MOTION_GETUP		(11)	//復帰

#define	MAX_Gifted			(51)		//才能の振り幅

//*****************************************************************************
// グローバル変数
//*****************************************************************************
MODEL_DRAW		g_model_Draw[MAX_MODEL_TYPE];
LPDIRECT3DTEXTURE9	pTextureModel[MAX_MODEL_TEX] = {};	//テクスチャ

MODEL_INFO		g_model_Info[MAX_MODEL_INFO];
MODEL_MOTION	g_model_motion[MAX_MODEL_CHAR][MAX_MODEL_MOTION];

//=============================================================================
// 初期化処理
//=============================================================================
void InitModel(void)
{
	FILE *pFile;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char aStr[10][256];		//モデルの名前取得用
	int				nNumMotion;

	BreakModel(0);

	//モーションデータの初期化
	for (int nCountData = 0; nCountData < MAX_MODEL_MOTION; nCountData++)
	{
		g_model_motion[0][nCountData].nKey = 2;
		g_model_motion[0][nCountData].fFastFrame = 0.0f;
		g_model_motion[0][nCountData].nBranch[0] = 0;
		g_model_motion[0][nCountData].nBranch[1] = 0;
		g_model_motion[0][nCountData].nBranch[2] = 0;
		g_model_motion[0][nCountData].nBranch[3] = MOTION_LAND;
		g_model_motion[0][nCountData].nCancel[0] = 0;
		g_model_motion[0][nCountData].nCancel[1] = 0;
		g_model_motion[0][nCountData].fEndFreame = 0.0f;
		g_model_motion[0][nCountData].fCancelFreame = 0.0f;
		for (int nCountData0 = 0; nCountData0 < MAX_KEY; nCountData0++)
		{
			g_model_motion[0][nCountData].key[nCountData0].nMuse = 1;
			g_model_motion[0][nCountData].key[nCountData0].nAtkFrame[0] = 0;
			g_model_motion[0][nCountData].key[nCountData0].nAtkFrame[1] = 10;
			g_model_motion[0][nCountData].key[nCountData0].nInvincible = 10;
			g_model_motion[0][nCountData].key[nCountData0].nFloating = 0;
			g_model_motion[0][nCountData].key[nCountData0].nAtkEffect = -1;
			g_model_motion[0][nCountData].key[nCountData0].nShowEffect = -1;
			g_model_motion[0][nCountData].key[nCountData0].fAtkplus = 1.0f;
			g_model_motion[0][nCountData].key[nCountData0].blown = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			g_model_motion[0][nCountData].key[nCountData0].fFrame = 40.0f;
			g_model_motion[0][nCountData].key[nCountData0].fRange = 5.0f;
			g_model_motion[0][nCountData].key[nCountData0].fSlip = 1.0f;
			g_model_motion[0][nCountData].key[nCountData0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_model_motion[0][nCountData].key[nCountData0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			for (int nCountData1 = 0; nCountData1 < MAX_PARTS; nCountData1++)
			{
				g_model_motion[0][nCountData].key[nCountData0].rot[MAX_PARTS] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	pFile = fopen("data/model.txt", "r");

	if (pFile != NULL)//Load
	{//成功
		for (int nCountData = 0; nCountData < MODEL_TYPE_EMPTY; nCountData++)
		{// Xファイルの読み込み
			fscanf(pFile, "%s\n", &g_model_Draw[nCountData].aText[0]);//ファイル名
			fscanf(pFile, "%d\n", &g_model_Draw[nCountData].nTexture);//画像番号
			fscanf(pFile, "%f %f %f", &g_model_Draw[nCountData].addpos.x, &g_model_Draw[nCountData].addpos.y, &g_model_Draw[nCountData].addpos.z);//場所
			fscanf(pFile, "%f %f %f\n", &g_model_Draw[nCountData].addrot.x, &g_model_Draw[nCountData].addrot.y, &g_model_Draw[nCountData].addrot.z);//角度
		}
		fclose(pFile);
	}
	else
	{//失敗
		MessageBox(0, "", "", MB_OK);
	}

	for (int nCountData = 0; nCountData < MAX_MODEL_INFO; nCountData++)
	{//モデル情報初期化
		g_model_Info[nCountData].bUse = false;
		g_model_Info[nCountData].set.nCntChar = -1;
	}

	//モーションの読み込み----------------------------------------------------------
	int nCntMotion[3];
	int nCountData[3];

	pFile = fopen("data/motion.txt", "r");

	if (pFile != NULL)//Load
	{//成功
		fscanf(pFile, "%d", &nCntMotion[0]);//ファイル名
		for (int nCntChar = 0; nCntChar < nCntMotion[0]; nCntChar++)
		{
			fscanf(pFile, "%s", &aStr[nCntChar][0]);//ファイル名
		}

		fclose(pFile);
	}
	else
	{//失敗
		MessageBox(0, "モーション", "motion.txt無し！", MB_OK);
	}

	for (int nCntChar = 0; nCntChar < nCntMotion[0]; nCntChar++)
	{
		pFile = fopen(&aStr[nCntChar][0], "r");
		if (pFile != NULL)//Load
		{//モーションファイルLoad
			fscanf(pFile, "%d %d\n", &nNumMotion, &nCntMotion[2]);//モーション数 パーツ数
			for (nCountData[0] = 0; nCountData[0] < nNumMotion; nCountData[0]++)
			{//モーション数回転
				fscanf(pFile, "%d %d %d %d %d %d %d\n", &g_model_motion[nCntChar][nCountData[0]].nKey,
					&g_model_motion[nCntChar][nCountData[0]].nCancel[0],
					&g_model_motion[nCntChar][nCountData[0]].nCancel[1],
					&g_model_motion[nCntChar][nCountData[0]].nBranch[0],
					&g_model_motion[nCntChar][nCountData[0]].nBranch[1],
					&g_model_motion[nCntChar][nCountData[0]].nBranch[2],
					&g_model_motion[nCntChar][nCountData[0]].nBranch[3]);
				fscanf(pFile, "%f %f %f\n\n", &g_model_motion[nCntChar][nCountData[0]].fFastFrame,
					&g_model_motion[nCntChar][nCountData[0]].fEndFreame,
					&g_model_motion[nCntChar][nCountData[0]].fCancelFreame);

				for (nCountData[1] = 0; nCountData[1] < g_model_motion[nCntChar][nCountData[0]].nKey; nCountData[1]++)
				{//キー数回転
					fscanf(pFile, "%d %d %d %d %d %d %d %d\n", &g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].nMuse,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].nAtkFrame[0],
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].nAtkFrame[1],
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].nParts,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].nInvincible,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].nFloating,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].nAtkEffect,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].nShowEffect);//モーション数 パーツ数
					fscanf(pFile, "%f %f %f %f\n", &g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].fFrame,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].fRange,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].fAtkplus,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].fSlip);
					fscanf(pFile, "%f %f %f\n", &g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].blown.x,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].blown.y,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].blown.z);
					fscanf(pFile, "%f %f %f\n", &g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].move.x,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].move.y,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].move.z);
					fscanf(pFile, "%f %f %f\n\n", &g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].pos.x,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].pos.y,
						&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].pos.z);

					for (nCountData[2] = 0; nCountData[2] < nCntMotion[2]; nCountData[2]++)
					{//パーツ数回転
						fscanf(pFile, "%f %f %f\n", &g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].rot[nCountData[2]].x,
							&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].rot[nCountData[2]].y,
							&g_model_motion[nCntChar][nCountData[0]].key[nCountData[1]].rot[nCountData[2]].z);
					}
					fscanf(pFile, "\n");
				}

			}
			fclose(pFile);
		}
		else
		{//失敗
			MessageBox(0, "モーションデータ", "無いよ!?", MB_OK);
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{

	BreakModel(0);

	for (int nCount = 0; nCount < MODEL_TYPE_EMPTY; nCount++)
	{
		// メッシュの開放
		if (g_model_Draw[nCount].MeshModel != NULL)
		{
			g_model_Draw[nCount].MeshModel->Release();
			g_model_Draw[nCount].MeshModel = NULL;
		}

		// マテリアルの開放
		if (g_model_Draw[nCount].pBuffMatModel != NULL)
		{
			g_model_Draw[nCount].pBuffMatModel->Release();
			g_model_Draw[nCount].pBuffMatModel = NULL;
		}
	}

	for (int nCount = 0; nCount < MAX_MODEL_TEX; nCount++)
	{
		// テクスチャの開放
		if (pTextureModel[nCount] != NULL)
		{
			pTextureModel[nCount]->Release();
			pTextureModel[nCount] = NULL;
		}
	}
}
//=============================================================================
// モデルロード処理
//=============================================================================
void LoadModel(int type)
{
	int nLength = (int)strlen(&g_model_Draw[type].aText[0]);		//文字数確認

	if (nLength > 4)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		D3DXLoadMeshFromX(&g_model_Draw[type].aText[0],
			D3DXMESH_SYSTEMMEM, pDevice, 
			NULL, 
			&g_model_Draw[type].pBuffMatModel, 
			NULL, 
			&g_model_Draw[type].nNumMatModel, 
			&g_model_Draw[type].MeshModel);

		strcpy(&g_model_Draw[type].aText[0], "");

		//テクスチャ確認
		if (pTextureModel[g_model_Draw[type].nTexture] == NULL)
		{//読み込まれていない
			D3DXMATERIAL		*pMat;					//マテリアルデータへのポインタ

			pMat = (D3DXMATERIAL*)g_model_Draw[type].pBuffMatModel->GetBufferPointer();
			D3DXCreateTextureFromFile(pDevice, pMat[0].pTextureFilename, &pTextureModel[g_model_Draw[type].nTexture]);
		}
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{
	Camera *pCamera = GetCamera();

	D3DXCOLOR col;
	D3DXVECTOR3 SavePos[2];
	ENEMY *pEnemy = GetEnemy();
	int nCount;
	static int nMotion = 0;
	static bool bEnter = false;
	int nField;
	int nMField;
	int nRand;

	for (nCount = 0; nCount < MAX_MODEL_INFO; nCount++)
	{//用意したモデル枠分回る
		if (g_model_Info[nCount].bUse == true)
		{
			if (pCamera[0].ViewS != SCENE_VIEW_STOP)
			{
				//スキル時間経過
				g_model_Info[nCount].set.nSkillTime[0]--;
				g_model_Info[nCount].set.nSkillTime[1]--;

				//地面のあたり判定の誤魔化し
				if (g_model_Info[nCount].set.bJump == true)
				{
					g_model_Info[nCount].set.pos.y += (g_model_Info[nCount].set.fPos - g_model_Info[nCount].set.pos.y) * 0.8f;
				}

				g_model_Info[nCount].set.posold = g_model_Info[nCount].set.pos;
				if (g_model_Info[nCount].use == MODEL_USE_PLAYER && pCamera[0].state == CAMERA_STATE_GAME)
				{//使用中 && プレイヤーなら
					UpdatePlayer(nCount);
				}
				else if (g_model_Info[nCount].use == MODEL_USE_Title)
				{//Titleのモデル
					g_model_Info[nCount].set.move.y = -10.0f;
					g_model_Info[nCount].set.move.x = -3.0f;
					g_model_Info[nCount].set.pos.z = 0.0f;
					//g_model_Info[nCount].set.rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
					g_model_Info[nCount].set.nNumMotion[2] = -99;
					if (g_model_Info[nCount].set.pos.y < 0) { g_model_Info[nCount].set.pos.y = 0; }
					if (g_model_Info[nCount].set.pos.x < -225)
					{
						nRand = rand() % 3;
						if (nRand == 0) { g_model_Info[nCount].set.parent[15].type = MODEL_TYPE_RUN_Sword0; }
						else if (nRand == 1) { g_model_Info[nCount].set.parent[15].type = MODEL_TYPE_RUN_Sword1; }
						else { g_model_Info[nCount].set.parent[15].type = MODEL_TYPE_RUN_Sword2; }
						g_model_Info[nCount].set.pos.x = 225;
					}

				}

				//モーションによってスタミナ増減
				g_model_Info[nCount].data.nTimeStamina--;
				if ((g_model_Info[nCount].set.nNumMotion[1] <= 1 && g_model_Info[nCount].data.nTimeStamina < 0) || g_model_Info[nCount].set.nNumMotion[1] == MOTION_DOWN)
				{
					g_model_Info[nCount].data.nStamina += g_model_Info[nCount].data.nMaxStamina / 45;
				}
				else if (g_model_Info[nCount].set.nNumMotion[1] == 2)
				{
					g_model_Info[nCount].data.nStamina--;
					if (g_model_Info[nCount].data.nStamina < -1) { g_model_Info[nCount].data.nStamina = -30; }
				}
				if (g_model_Info[nCount].set.nNumMotion[1] > 1) { g_model_Info[nCount].data.nTimeStamina = 20; }
				if (g_model_Info[nCount].data.nStamina > g_model_Info[nCount].data.nMaxStamina) { g_model_Info[nCount].data.nStamina = g_model_Info[nCount].data.nMaxStamina; }
				if (g_model_Info[nCount].data.nStamina < -30) { g_model_Info[nCount].data.nStamina = -30; }

				g_model_Info[nCount].set.nCntHeel++;
				if (g_model_Info[nCount].set.nCntHeel > 120 && g_model_Info[nCount].data.nLifePoint > 0)
				{//回復
					g_model_Info[nCount].set.nCntHeel = 0;
					g_model_Info[nCount].data.nLifePoint += g_model_Info[nCount].data.nLifePoint / 100;
					g_model_Info[nCount].data.nSkillPoint += g_model_Info[nCount].data.nSkillPoint / 30;
					if (g_model_Info[nCount].data.nLifePoint > (int)g_model_Info[nCount].data.status.fLifePoint)
					{
						g_model_Info[nCount].data.nLifePoint = (int)g_model_Info[nCount].data.status.fLifePoint;
					}
					if (g_model_Info[nCount].data.nSkillPoint > (int)g_model_Info[nCount].data.status.fSkillPoint)
					{
						g_model_Info[nCount].data.nSkillPoint = (int)g_model_Info[nCount].data.status.fSkillPoint;
					}
				}

				UpdateMotion(nCount);
				
				//ステータス上昇のカウント
				if (g_model_Info[nCount].data.plus[0].fAttack > 0.0f)
				{//上昇中なら 攻撃
					g_model_Info[nCount].data.plus[0].fAttack--;
					if (g_model_Info[nCount].data.plus[0].fAttack == 0.0f) 
					{ 
						g_model_Info[nCount].data.plus[1].fAttack = 0.0f;
						GetStatus(&g_model_Info[nCount].data, true);
					}
				}
				if (g_model_Info[nCount].data.plus[0].fDefense > 0.0f)
				{//上昇中なら　防御
					g_model_Info[nCount].data.plus[0].fDefense--;
					if (g_model_Info[nCount].data.plus[0].fDefense == 0.0f) 
					{ 
						g_model_Info[nCount].data.plus[1].fDefense = 0.0f;
						GetStatus(&g_model_Info[nCount].data, true);
					}
				}
				if (g_model_Info[nCount].data.plus[0].fInt > 0.0f)
				{//上昇中なら　魔力
					g_model_Info[nCount].data.plus[0].fInt--;
					if (g_model_Info[nCount].data.plus[0].fInt == 0.0f)
					{
						g_model_Info[nCount].data.plus[1].fInt = 0.0f;
						GetStatus(&g_model_Info[nCount].data, true);
					}
				}
				if (g_model_Info[nCount].data.plus[0].fRes > 0.0f)
				{//上昇中なら　魔防
					g_model_Info[nCount].data.plus[0].fRes--;
					if (g_model_Info[nCount].data.plus[0].fRes == 0.0f)
					{ 
						g_model_Info[nCount].data.plus[1].fRes = 0.0f;
						GetStatus(&g_model_Info[nCount].data, true);
					}
				}
				if (g_model_Info[nCount].data.plus[0].fSpeed > 0.0f)
				{//上昇中なら　スピード
					g_model_Info[nCount].data.plus[0].fSpeed--;
					if (g_model_Info[nCount].data.plus[0].fSpeed == 0.0f) 
					{ 
						g_model_Info[nCount].data.plus[1].fSpeed = 0.0f;
						GetStatus(&g_model_Info[nCount].data, true);
					}
				}
				if (g_model_Info[nCount].data.plus[0].fLucky > 0.0f)
				{//上昇中なら　運
					g_model_Info[nCount].data.plus[0].fLucky--;
					if (g_model_Info[nCount].data.plus[0].fLucky == 0.0f)
					{
						g_model_Info[nCount].data.plus[1].fLucky = 0.0f;
						GetStatus(&g_model_Info[nCount].data, true);
					}
				}
				if (g_model_Info[nCount].data.plus[0].fLifePoint > 0.0f)
				{//上昇中なら　HP
					g_model_Info[nCount].data.plus[0].fLifePoint--;
					if (g_model_Info[nCount].data.plus[0].fLifePoint == 0.0f)
					{
						g_model_Info[nCount].data.plus[1].fLifePoint = 0.0f;
						GetStatus(&g_model_Info[nCount].data, true);
						if ((int)g_model_Info[nCount].data.status.fLifePoint < g_model_Info[nCount].data.nLifePoint)
						{
							g_model_Info[nCount].data.nLifePoint = (int)g_model_Info[nCount].data.status.fLifePoint;
						}
					}
				}
				if (g_model_Info[nCount].data.plus[0].fSkillPoint > 0.0f)
				{//上昇中なら　SP
					g_model_Info[nCount].data.plus[0].fSkillPoint--;
					if (g_model_Info[nCount].data.plus[0].fSkillPoint == 0.0f)
					{
						g_model_Info[nCount].data.plus[1].fSkillPoint = 0.0f;
						GetStatus(&g_model_Info[nCount].data, true);
						if ((int)g_model_Info[nCount].data.status.fSkillPoint < g_model_Info[nCount].data.nSkillPoint)
						{
							g_model_Info[nCount].data.nSkillPoint = (int)g_model_Info[nCount].data.status.fSkillPoint;
						}
					}
				}



				if (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].nFloating == 1)
				{//浮遊ありなら
					g_model_Info[nCount].set.move.y = 0.0f;
				}
				else { g_model_Info[nCount].set.move.y -= 0.25f; }	//浮遊なしなら
				g_model_Info[nCount].set.pos += g_model_Info[nCount].set.move;
				g_model_Info[nCount].set.fPos += g_model_Info[nCount].set.move.y;
				//SadnessError(nCount);

				//通常
				//CollisionMeshWall(&g_model_Info[nCount].set.pos, &g_model_Info[nCount].set.posold, &g_model_Info[nCount].set.move, 20.0f);

				//SavePos[0] = g_model_Info[nCount].set.pos;
				//g_model_Info[nCount].set.pos = g_model_Info[nCount].set.posold;
				//g_model_Info[nCount].set.pos += (SavePos[0] - g_model_Info[nCount].set.posold) * 0.5f;
				//SavePos[1] = g_model_Info[nCount].set.pos;
				////半減で再確認
				//CollisionMeshWall(&g_model_Info[nCount].set.pos, &g_model_Info[nCount].set.posold, &g_model_Info[nCount].set.move, 20.0f);
				//if (g_model_Info[nCount].set.pos == SavePos[1]) { g_model_Info[nCount].set.pos = SavePos[0]; }
				//else { MessageBox(0, "", "貫通してるよ", MB_OK); }

				//CollisionModel(nCount);
				//CollisionMeshWall(&g_model_Info[nCount].set.pos, &g_model_Info[nCount].set.posold, &g_model_Info[nCount].set.move, 20.0f);
				//nField = CollisionMeshField(&g_model_Info[nCount].set.pos, &g_model_Info[nCount].set.posold, &g_model_Info[nCount].set.move, &g_model_Info[nCount].set.fPos, g_model_Info[nCount].set.bJump, &g_model_Info[nCount].set);

				//オブジェの壁

				nField = CollisionObject(nCount);
				nMField = CollisionMeshField(&g_model_Info[nCount].set.pos, &g_model_Info[nCount].set.posold, &g_model_Info[nCount].set.move, &g_model_Info[nCount].set.fPos, g_model_Info[nCount].set.bJump, &g_model_Info[nCount].set);
				if (nMField != 0)
				{//メッシュフィールドにいる
					nField = nMField;
				}

				CollisionModel(nCount);
				CollisionObject(nCount);

				if (nField > 1)
				{//着地した
					if (g_model_Info[nCount].data.nLifePoint > 0)
					{
						if (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].nFloating == 0)
						{
							g_model_Info[nCount].set.bJump = true;
							g_model_Info[nCount].set.bStep = true;
							g_model_Info[nCount].set.nNumMotion[3] = -2;
							if (g_model_Info[nCount].set.nNumMotion[1] != MOTION_SHRINK)
							{//通常時
								g_model_Info[nCount].set.nNumMotion[1] =
									g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nBranch[3];
								if (g_model_Info[nCount].set.nNumMotion[1] == MOTION_LAND) { g_model_Info[nCount].set.state = MODEL_STATE_NONE; }	//ニュートラルなら
							}
							else
							{//怯みの場合
								g_model_Info[nCount].set.nNumMotion[1] = MOTION_DOWN;
								g_model_Info[nCount].set.nAirTime = 300;
								g_model_Info[nCount].set.nCntState = 30;
								if (g_model_Info[nCount].use >= MODEL_USE_ENEMY_Z)
								{//敵の場合
									if (pEnemy[g_model_Info[nCount].set.nCntChar].nDying == 1)
									{//瀕死の場合//瀕死のモーションへ
										g_model_Info[nCount].set.nNumMotion[1] = MOTION_DOWN + 2;
										g_model_Info[nCount].set.nAirTime = 15;
										g_model_Info[nCount].set.nCntState = 0;
									}
								}
							}
						}
					}
					else
					{//死んでいる
						g_model_Info[nCount].set.state = MODEL_STATE_DIE;
						g_model_Info[nCount].set.nCntState = 60;
						g_model_Info[nCount].set.nNumMotion[1] = MOTION_DOWN; g_model_Info[nCount].set.nAirTime = 300;
						g_model_Info[nCount].set.bJump = true;
						if (g_model_Info[nCount].use >= MODEL_USE_ENEMY_Z)
						{
							ChangeZWindow(
								pEnemy[g_model_Info[nCount].set.nCntChar].nCntHPGage[0], g_model_Info[nCount].data.status.fLifePoint,
								g_model_Info[nCount].data.nLifePoint * 1.0f, g_model_Info[nCount].set.pos, false);
							ChangeZWindow(
								pEnemy[g_model_Info[nCount].set.nCntChar].nCntHPGage[1], g_model_Info[nCount].data.status.fLifePoint,
								g_model_Info[nCount].data.nLifePoint * 1.0f, g_model_Info[nCount].set.pos, false);
							ChangeZWindow(
								pEnemy[g_model_Info[nCount].set.nCntChar].nCntHPGage[2], g_model_Info[nCount].data.status.fLifePoint,
								g_model_Info[nCount].data.nLifePoint * 1.0f, g_model_Info[nCount].set.pos, false);
						}
						BreakShadow(g_model_Info[nCount].nCntShadow);
						BreakObject(g_model_Info[nCount].nCntObject);
					}
				}
				else if(nField == 0)
				{
					if (g_model_Info[nCount].set.nNumMotion[1] < 3)
					{//ニュートラルの場合
						g_model_Info[nCount].set.nNumMotion[3] = -2;
						g_model_Info[nCount].set.nNumMotion[1] = 5;
					}
					g_model_Info[nCount].set.bJump = false;
				}

				if (g_model_Info[nCount].use >= MODEL_USE_ENEMY_Z)
				{//敵のHPゲージの移動
					ChangeZWindow(
						pEnemy[g_model_Info[nCount].set.nCntChar].nCntHPGage[0], g_model_Info[nCount].data.status.fLifePoint,
						g_model_Info[nCount].data.nLifePoint * 1.0f, g_model_Info[nCount].set.pos
						+ D3DXVECTOR3(sinf(pCamera[0].rot.y - D3DX_PI * 0.25f) * -28.0f, 95.0f, cosf(pCamera[0].rot.y - D3DX_PI * 0.25f) * -28.0f), true);
					ChangeZWindow(
						pEnemy[g_model_Info[nCount].set.nCntChar].nCntHPGage[1], g_model_Info[nCount].data.status.fLifePoint,
						g_model_Info[nCount].data.nLifePoint * 1.0f, g_model_Info[nCount].set.pos
						+ D3DXVECTOR3(sinf(pCamera[0].rot.y - D3DX_PI * 0.25f) * -28.0f, 95.0f, cosf(pCamera[0].rot.y - D3DX_PI * 0.25f) * -28.0f), true);
					ChangeZWindow(
						pEnemy[g_model_Info[nCount].set.nCntChar].nCntHPGage[2], g_model_Info[nCount].data.status.fLifePoint,
						g_model_Info[nCount].data.nLifePoint * 1.0f, g_model_Info[nCount].set.pos
						+ D3DXVECTOR3(sinf(pCamera[0].rot.y - D3DX_PI * 0.25f) * -28.0f, 95.0f, cosf(pCamera[0].rot.y - D3DX_PI * 0.25f) * -28.0f), true);
				}

				//影配置
				SettingShadow(g_model_Info[nCount].set.pos, g_model_Info[nCount].set.rot, g_model_Info[nCount].set.fSlength, g_model_Info[nCount].set.fSlength, g_model_Info[nCount].nCntShadow);
				SettingObject(g_model_Info[nCount].set.pos, g_model_Info[nCount].set.rot, g_model_Info[nCount].nCntObject);
			}
		}
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel_P(void)
{
	for (int nCount = 0; nCount < MAX_MODEL_INFO; nCount++)
	{//用意したモデル枠分回る
		if (g_model_Info[nCount].bUse == true && g_model_Info[nCount].use == MODEL_USE_Pause)
		{
			UpdateMotion(nCount);
		}
	}
}
//=============================================================================
// プレイヤー更新処理
//=============================================================================
void UpdatePlayer(int nCount)
{
	Camera *pCamera = GetCamera();
	PLAYER *pPlayer = GetPlayer();
	char aStr[48];
	int nLength;

	if (g_model_Info[nCount].data.nLifePoint > 0)
	{//生きている
	//マップ切り替え確認
		MAP_CHANGE	*pMap_Change = GetMap_Change();
		LOAD_MAP	*pLoad_Map = GetLoad_Map();
		for (int nCntMap = 0; nCntMap < MAX_MAP_CHANGE; nCntMap++)
		{
			if (pMap_Change[nCntMap].bUse == true)
			{//切り替えがあるなら
				if (pMap_Change[nCntMap].Entrance.x + pMap_Change[nCntMap].nLengthX > g_model_Info[nCount].set.pos.x &&
					pMap_Change[nCntMap].Entrance.x - pMap_Change[nCntMap].nLengthX < g_model_Info[nCount].set.pos.x &&
					pMap_Change[nCntMap].Entrance.z + pMap_Change[nCntMap].nLengthZ > g_model_Info[nCount].set.pos.z &&
					pMap_Change[nCntMap].Entrance.z - pMap_Change[nCntMap].nLengthZ < g_model_Info[nCount].set.pos.z)
				{//切り替え範囲が範囲内
					int *pTime = GetTime();
					pTime[0] = 0;	//これから暗くなる
					g_model_Info[nCount].set.state = MODEL_STATE_MAP;//攻撃当たらなくなる
					g_model_Info[nCount].set.move = D3DXVECTOR3(0.0f, g_model_Info[nCount].set.move.y, 0.0f);
					SetLoad_Map(pMap_Change[nCntMap].Export, pMap_Change[nCntMap].MapNumber);
					//画面暗くなる
					SetFadeWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 0.0f,0.0f,0.0f, 0.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 0, WINDOWSTATE_FADEIN, WINDOWUSE_NORMAL);
					pMap_Change[nCntMap].bUse = false;
					break;
				}
			}
		}

		//敵の出現地確認
		ENEMY_APPEAR *pEnemy_Appear = GetEnemy_Appear();
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_APPEAR; nCntEnemy++)
		{
			if (pEnemy_Appear[nCntEnemy].bUse == true)
			{//切り替えがあるなら
				if (pEnemy_Appear[nCntEnemy].Entrance.x + pEnemy_Appear[nCntEnemy].nLengthX > g_model_Info[nCount].set.pos.x &&
					pEnemy_Appear[nCntEnemy].Entrance.x - pEnemy_Appear[nCntEnemy].nLengthX < g_model_Info[nCount].set.pos.x &&
					pEnemy_Appear[nCntEnemy].Entrance.z + pEnemy_Appear[nCntEnemy].nLengthZ > g_model_Info[nCount].set.pos.z &&
					pEnemy_Appear[nCntEnemy].Entrance.z - pEnemy_Appear[nCntEnemy].nLengthZ < g_model_Info[nCount].set.pos.z)
				{//切り替え範囲が範囲内
					Enemy_Appear(pEnemy_Appear[nCntEnemy].Entrance, pEnemy_Appear[nCntEnemy].nNumber);
					pEnemy_Appear[nCntEnemy].bUse = false;
					break;
				}
			}
		}

		//会話更新
		UpdateSpeech();

		float fRot = g_model_Info[nCount].set.angle.y;

		if (pLoad_Map[0].nMapNumber != MAP_TYPE_EMPTY) { UpdateMap_Change(nCount, 0); }
		else if (g_model_Info[nCount].set.state != MODEL_STATE_MAP)
		{
			float	fRotX = 0.0f;
			float	fRotZ = 0.0f;
			bool	bMove = false;
			int nLock = 5;
			XINPUT_STATE state;
			XInputGetState(0, &state);

			//ドロップ枠
			switch (pPlayer[0].SDstate)
			{
			case SDROP_STATE_NONE:		//表示準備
				if (pPlayer[0].nDSkill[0] != -1)
				{//スキルが入っている
					//ウィンドウ表示と時間、スキル表示
					pPlayer[0].nCntDWindow[0] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 100, 40, 23, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_FADEIN, WINDOWUSE_GAME, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
					pPlayer[0].nCntDWindow[1] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 100, 40, 24, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_FADEIN, WINDOWUSE_GAME, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
					pPlayer[0].nCntDWindow[2] = SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.825f, SCREEN_HEIGHT * 0.7f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 100, 40, 25, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_FADEIN, WINDOWUSE_GAME, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
					SetFont(RECT{ 1000 , 297, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "10.0秒", 8, 0, DT_LEFT, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);
					wsprintf(&aStr[0], "");
					ReturnSkillName(&aStr[0], pPlayer[0].nDSkill[0]);
					nLength = (int)strlen(&aStr[0]);
					wsprintf(&aStr[nLength], "\n%d SP\n", MotionSkill(g_model_motion[g_model_Info[0].set.nNumMotion[0]][pPlayer[0].nDSkill[0]].key[0].nAtkEffect));

					SetFont(RECT{ 800 , 342, 1300, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 9, 0, DT_CENTER, -1, true, FONTSTATE_NONE, WINDOW_DRAW_1);
					pPlayer[0].SDstate = SDROP_STATE_SELECT;
					pPlayer[0].nCntSDstate = 600;
				}
				break;
			case SDROP_STATE_SELECT:	//選択
				pPlayer[0].nCntSDstate--;
				wsprintf(&aStr[0], "%d.%d秒", pPlayer[0].nCntSDstate / 60, (pPlayer[0].nCntSDstate / 6) % 10);
				ChangeFont(true, &aStr[0], 8, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				//フォントの変化
				if (pPlayer[0].bButton[BUTTON_LEFT] == true)
				{//Skill1入れ替え
					SwapSkill(0, nCount);
					g_model_Info[nCount].set.nSkillTime[0] = 0;
					pPlayer[0].SDstate = SDROP_STATE_END;
					pPlayer[0].nCntSDstate = 61;
					//選択windowの変化
					ChangeWindow(pPlayer[0].nCntDWindow[0], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_FADEOUT_B);
					ChangeWindow(pPlayer[0].nCntDWindow[1], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_FADEOUT);
					ChangeWindow(pPlayer[0].nCntDWindow[2], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_FADEOUT);
				}
				else if (pPlayer[0].bButton[BUTTON_RIGHT] == true)
				{//Skill2入れ替え
					SwapSkill(1, nCount);
					g_model_Info[nCount].set.nSkillTime[1] = 0;
					pPlayer[0].SDstate = SDROP_STATE_END;
					pPlayer[0].nCntSDstate = 61;
					//選択windowの変化
					ChangeWindow(pPlayer[0].nCntDWindow[0], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_FADEOUT);
					ChangeWindow(pPlayer[0].nCntDWindow[1], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_FADEOUT_B);
					ChangeWindow(pPlayer[0].nCntDWindow[2], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_FADEOUT);
				}
				else if (pPlayer[0].bButton[BUTTON_DOWN] == true || pPlayer[0].nCntSDstate == 0)
				{//キャンセル
					SwapSkill(-1, nCount);
					pPlayer[0].SDstate = SDROP_STATE_END;
					pPlayer[0].nCntSDstate = 61;
					//選択windowの変化
					ChangeWindow(pPlayer[0].nCntDWindow[0], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_FADEOUT);
					ChangeWindow(pPlayer[0].nCntDWindow[1], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_FADEOUT);
					ChangeWindow(pPlayer[0].nCntDWindow[2], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_FADEOUT_B);
				}
				break;
			case SDROP_STATE_END:		//決定
				pPlayer[0].nCntSDstate--;
				if (pPlayer[0].nCntSDstate == 0){ pPlayer[0].SDstate = SDROP_STATE_NONE; }
				else if (pPlayer[0].nCntSDstate == 60)
				{//戻る
					//windowとフォントの削除
					ChangeFont(false, &aStr[0], 8, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					ChangeFont(false, &aStr[0], 9, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				}
				break;
			}

			//スピード
			if (g_model_Info[nCount].set.nNumMotion[1] < MOTION_STEP || g_model_Info[nCount].set.nMotionFrame == 0)
			{
				if (g_model_Info[nCount].set.nNumMotion[1] < MOTION_LANDING)
				{
					if (pPlayer[0].bButton[BUTTON_LSHIFT] == true)
					{
						//g_model_Info[nCount].data.nLifePoint -= 100.0f;
						if (g_model_Info[nCount].data.nStamina > 0)
						{
							g_model_Info[nCount].set.fSpeed = (MODEL_PMOVE0 + g_model_Info[nCount].data.status.fSpeed * MODEL_PMOVE) * MODEL_RUN_MOVE;
						}
						else { g_model_Info[nCount].set.fSpeed = (MODEL_PMOVE0 + g_model_Info[nCount].data.status.fSpeed * MODEL_PMOVE); }
						g_model_Info[nCount].set.bRun = true;
					}
					else
					{
						g_model_Info[nCount].set.fSpeed = (MODEL_PMOVE0 + g_model_Info[nCount].data.status.fSpeed * MODEL_PMOVE);
						g_model_Info[nCount].set.bRun = false;
					}
				}//着地より上はSPEEDなし
				else if (g_model_Info[nCount].set.nNumMotion[1] > MOTION_LANDING) { g_model_Info[nCount].set.fSpeed = 0.0f; }
				else { g_model_Info[nCount].set.fSpeed = (MODEL_PMOVE0 + g_model_Info[nCount].data.status.fSpeed * MODEL_PMOVE); }
			}
			else { g_model_Info[nCount].set.fSpeed = 0.0f; }
			if (g_model_Info[nCount].set.nNumMotion[1] == MOTION_AIR) { g_model_Info[nCount].set.fSpeed = (1.0f + g_model_Info[nCount].data.status.fSpeed * 0.0001f); }
			if (g_model_Info[nCount].data.nStamina < 0) { g_model_Info[nCount].set.fSpeed *= 0.5f; }

			if (g_model_Info[nCount].set.nNumMotion[1] != MOTION_DOWN)
			{//現在のモーションがダウンでないなら
				if (pPlayer[0].bButton[BUTTON_Q] == true)
				{
					if (g_model_Info[nCount].data.nSkillPoint > MotionSkill(g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].data.nSkill[0][0]].key[0].nAtkEffect && g_model_Info[nCount].data.nSkill[0][0] > 0))
					{
						if (pPlayer[0].bButton[BUTTON_J] == true) { g_model_Info[nCount].set.nNumMotion[2] = -3; g_model_Info[nCount].set.nPlayTime = 30; }
					}//スキル弱
					if (g_model_Info[nCount].data.nSkillPoint > MotionSkill(g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].data.nSkill[1][0]].key[0].nAtkEffect && g_model_Info[nCount].data.nSkill[1][0] > 0))
					{
						if (pPlayer[0].bButton[BUTTON_I] == true) { g_model_Info[nCount].set.nNumMotion[2] = -4; g_model_Info[nCount].set.nPlayTime = 30; }
					}//スキル強
				}
				else
				{

					if (pPlayer[0].bButton[BUTTON_J] == true) { g_model_Info[nCount].set.nNumMotion[2] = -1; g_model_Info[nCount].set.nPlayTime = 30; }	//攻撃弱
					if (pPlayer[0].bButton[BUTTON_I] == true) { g_model_Info[nCount].set.nNumMotion[2] = -2; g_model_Info[nCount].set.nPlayTime = 30; }	//攻撃強
				}

				if (pPlayer[0].bButton[BUTTON_K] == true) { g_model_Info[nCount].set.nNumMotion[2] = MOTION_GUARD; }	//ガード
				if (pPlayer[0].bButton[BUTTON_L] == true) { g_model_Info[nCount].set.nNumMotion[2] = MOTION_STEP; g_model_Info[nCount].set.nPlayTime = 30; }	//ステップ

				if (pPlayer[0].bButton[BUTTON_SPACE] == true && g_model_Info[nCount].set.bJump == true) { g_model_Info[nCount].set.nNumMotion[2] = MOTION_LANDING; g_model_Info[nCount].set.nPlayTime = 30; }	//ジャンプ
																																																				//if (GetKeyboardTrigger(DIK_Y) == true) { g_model_Info[nCount].set.nNumMotion[2] = MOTION_SHRINK; g_model_Info[nCount].set.nPlayTime = 30; }	//被ダメ
			}
			else if (g_model_Info[nCount].set.nCntState < 1)
			{//ダウンしている && 起き上がっていいなら
				if (pPlayer[0].bButton[BUTTON_J] == true) { g_model_Info[nCount].set.nNumMotion[1] = MOTION_GETUP; g_model_Info[nCount].set.nNumMotion[3] = -1; }	//攻撃弱
				if (pPlayer[0].bButton[BUTTON_I] == true) { g_model_Info[nCount].set.nNumMotion[1] = MOTION_GETUP; g_model_Info[nCount].set.nNumMotion[3] = -1; }	//攻撃強
				if (pPlayer[0].bButton[BUTTON_K] == true) { g_model_Info[nCount].set.nNumMotion[1] = MOTION_GETUP; g_model_Info[nCount].set.nNumMotion[3] = -1; }	//ガード
				if (pPlayer[0].bButton[BUTTON_L] == true) { g_model_Info[nCount].set.nNumMotion[1] = MOTION_GETUP; g_model_Info[nCount].set.nNumMotion[3] = -1; }	//ステップ																															//Kロックオン
				if (pPlayer[0].bButton[BUTTON_SPACE] == true) { g_model_Info[nCount].set.nNumMotion[1] = MOTION_GETUP; g_model_Info[nCount].set.nNumMotion[3] = -1; }	//ジャンプ
			}

			//移動
			bMove = false;
			if (pPlayer[0].bButton[BUTTON_W] == true) { bMove = true; fRotZ = 1.0f; }
			else if (pPlayer[0].bButton[BUTTON_S] == true) { bMove = true; fRotZ = -1.0f; }
			if (pPlayer[0].bButton[BUTTON_A] == true) { bMove = true; fRotX = -1.0f; }
			else if (pPlayer[0].bButton[BUTTON_D] == true) { bMove = true; fRotX = 1.0f; }
			//ジョイパッド補正
			if (pPlayer[0].bButton[BUTTON_WS] == true) { fRotZ = state.Gamepad.sThumbLY; }
			if (pPlayer[0].bButton[BUTTON_SS] == true) { fRotZ = state.Gamepad.sThumbLY; }
			if (pPlayer[0].bButton[BUTTON_AS] == true) { fRotX = state.Gamepad.sThumbLX; }
			if (pPlayer[0].bButton[BUTTON_DS] == true) { fRotX = state.Gamepad.sThumbLX; }


			fRot = atan2f(fRotX, fRotZ) + pCamera[0].rot.y;
			if (bMove == true)
			{
				g_model_Info[nCount].set.angle.y = fRot;
				g_model_Info[nCount].set.move += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -g_model_Info[nCount].set.fSpeed;
			}

			if (g_model_Info[nCount].set.nLockChar > -1) { nLock = 0; }

			if (g_model_Info[nCount].set.nNumMotion[1] < MOTION_STEP || g_model_Info[nCount].set.nMotionFrame < nLock)
			{//現在のモーションが攻撃していない場合
				if (g_model_Info[nCount].set.move.x > 0.1f || g_model_Info[nCount].set.move.x < -0.1f || g_model_Info[nCount].set.move.z > 0.1f || g_model_Info[nCount].set.move.z < -0.1f)
				{//移動中 カメラの角度に自機の角度を合わせる
					float fRotPoor = g_model_Info[nCount].set.angle.y - g_model_Info[nCount].set.rot.y;
					if (fRotPoor > D3DX_PI) { fRotPoor -= D3DX_PI * 2.0f; }
					else if (fRotPoor < -D3DX_PI) { fRotPoor += D3DX_PI * 2.0f; }
					g_model_Info[nCount].set.rot.y += fRotPoor * 0.15f;
				}
				if (g_model_Info[nCount].set.rot.y > D3DX_PI) { g_model_Info[nCount].set.rot.y -= D3DX_PI * 2; }
				if (g_model_Info[nCount].set.rot.y < -D3DX_PI) { g_model_Info[nCount].set.rot.y += D3DX_PI * 2; }
			}

		}
		if (g_model_Info[nCount].set.nPlayTime > 0)
		{
			g_model_Info[nCount].set.nPlayTime--;
			if (g_model_Info[nCount].set.nPlayTime == 0) { g_model_Info[nCount].set.nNumMotion[2] = 0; }
		}

		//D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//D3DXVECTOR3 pos = pCamera[0].posV;
		//pCamera[0].targetpos += (g_model_Info[nCount].set.pos + (g_model_Info[nCount].set.pos - g_model_Info[nCount].set.posold) * 20.0f - pCamera[0].targetpos) * 0.08f;
		//if (pCamera[0].targetpos.y - g_model_Info[nCount].set.pos.y < -50) { pCamera[0].targetpos.y = g_model_Info[nCount].set.pos.y - 50; }
		//if (pCamera[0].targetpos.y - g_model_Info[nCount].set.pos.y > 50) { pCamera[0].targetpos.y = g_model_Info[nCount].set.pos.y + 50; }

		//CollisionMeshWall(&pos, &g_model_Info[nCount].set.pos, &move, 10.0f);
		//pCamera[0].posV += (pos - pCamera[0].posV) * 0.1f;

		LockOn_MODEL(nCount);
	}//生存しているなら

	if (pPlayer[0].bGame == true)
	{//ほかの更新項目
		if (g_model_Info[nCount].data.nLifePoint > 0)
		{
			if (pCamera[0].state == CAMERA_STATE_MOVIE) { ChangeWindow(pPlayer[0].nCntSkillWindow[0], D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), WINDOWSTATE_MAX); }
			else if (g_model_Info[nCount].set.nSkillTime[0] < 1 && g_model_Info[nCount].data.nSkillPoint > MotionSkill(g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].data.nSkill[0][0]].key[0].nAtkEffect))
			{
				ChangeWindow(pPlayer[0].nCntSkillWindow[0], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_MAX);
			}
			else { ChangeWindow(pPlayer[0].nCntSkillWindow[0], D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), WINDOWSTATE_MAX); }
			if (pCamera[0].state == CAMERA_STATE_MOVIE) { ChangeWindow(pPlayer[0].nCntSkillWindow[1], D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), WINDOWSTATE_MAX); }
			else if (g_model_Info[nCount].set.nSkillTime[1] < 1 && g_model_Info[nCount].data.nSkillPoint > MotionSkill(g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].data.nSkill[1][0]].key[0].nAtkEffect))
			{
				ChangeWindow(pPlayer[0].nCntSkillWindow[1], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_MAX);
			}
			else { ChangeWindow(pPlayer[0].nCntSkillWindow[1], D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), WINDOWSTATE_MAX); }
			ChangeZWindow(pPlayer[0].nCntStamina[0], g_model_Info[nCount].data.nMaxStamina * 1.0f, g_model_Info[nCount].data.nStamina * 1.0f
				, g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(pCamera[0].rot.y - D3DX_PI * 0.25f) * -28.0f, 95.0f, cosf(pCamera[0].rot.y - D3DX_PI * 0.25f) * -28.0f), true);
			ChangeZWindow(pPlayer[0].nCntStamina[1], g_model_Info[nCount].data.nMaxStamina * 1.0f, g_model_Info[nCount].data.nStamina * 1.0f
				, g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(pCamera[0].rot.y - D3DX_PI * 0.25f) * -28.0f, 95.0f, cosf(pCamera[0].rot.y - D3DX_PI * 0.25f) * -28.0f), true);
			CheckEXP(g_model_Info[nCount].data.nExperience * 1.0f, g_model_Info[nCount].data.nNextLevel * 1.0f, pPlayer[0].nCntEXPGage[1]);
		}
		CheckGage(g_model_Info[nCount].data.nLifePoint * 1.0f, g_model_Info[nCount].data.status.fLifePoint, pPlayer[0].nCntHPGage[1]);
		CheckGage(g_model_Info[nCount].data.nLifePoint * 1.0f, g_model_Info[nCount].data.status.fLifePoint, pPlayer[0].nCntHPGage[2]);
		CheckGage(g_model_Info[nCount].data.nSkillPoint * 1.0f, g_model_Info[nCount].data.status.fSkillPoint, pPlayer[0].nCntMPGage[1]);
		CheckGage(g_model_Info[nCount].data.nSkillPoint * 1.0f, g_model_Info[nCount].data.status.fSkillPoint, pPlayer[0].nCntMPGage[2]);
		pPlayer[0].nTime[1]++;
		if (pPlayer[0].nTime[1] == 6)
		{
			pPlayer[0].nTime[1] = 0;
			pPlayer[0].nTime[0]--;
		}

		//ステータス上昇確認
		if (g_model_Info[nCount].data.plus[1].fLifePoint > 0.0f) { ChangeWindow(pPlayer[0].nCntIcon[0], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_MAX); }
		else{ ChangeWindow(pPlayer[0].nCntIcon[0], D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f), WINDOWSTATE_MAX); }
		if (g_model_Info[nCount].data.plus[1].fSkillPoint > 0.0f) { ChangeWindow(pPlayer[0].nCntIcon[1], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_MAX); }
		else { ChangeWindow(pPlayer[0].nCntIcon[1], D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f), WINDOWSTATE_MAX); }
		if (g_model_Info[nCount].data.plus[1].fAttack > 0.0f) { ChangeWindow(pPlayer[0].nCntIcon[2], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_MAX); }
		else { ChangeWindow(pPlayer[0].nCntIcon[2], D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f), WINDOWSTATE_MAX); }
		if (g_model_Info[nCount].data.plus[1].fDefense > 0.0f) { ChangeWindow(pPlayer[0].nCntIcon[3], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_MAX); }
		else { ChangeWindow(pPlayer[0].nCntIcon[3], D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f), WINDOWSTATE_MAX); }
		if (g_model_Info[nCount].data.plus[1].fInt > 0.0f) { ChangeWindow(pPlayer[0].nCntIcon[4], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_MAX); }
		else { ChangeWindow(pPlayer[0].nCntIcon[4], D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f), WINDOWSTATE_MAX); }
		if (g_model_Info[nCount].data.plus[1].fRes > 0.0f) { ChangeWindow(pPlayer[0].nCntIcon[5], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_MAX); }
		else { ChangeWindow(pPlayer[0].nCntIcon[5], D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f), WINDOWSTATE_MAX); }
		if (g_model_Info[nCount].data.plus[1].fSpeed > 0.0f) { ChangeWindow(pPlayer[0].nCntIcon[6], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_MAX); }
		else { ChangeWindow(pPlayer[0].nCntIcon[6], D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f), WINDOWSTATE_MAX); }
		if (g_model_Info[nCount].data.plus[1].fLucky > 0.0f) { ChangeWindow(pPlayer[0].nCntIcon[7], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_MAX); }
		else { ChangeWindow(pPlayer[0].nCntIcon[7], D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f), WINDOWSTATE_MAX); }

		if (g_model_Info[nCount].data.nSkill[0][0] < 0) { g_model_Info[nCount].set.nSkillTime[0] = 9999; }
		if (g_model_Info[nCount].data.nSkill[1][0] < 0) { g_model_Info[nCount].set.nSkillTime[1] = 9999; }

		if (pPlayer[0].bButton[BUTTON_UP] == true)
		{
			pCamera[3].nCntState = (pCamera[3].nCntState + 1) % 3;
		}
		SettingObject(g_model_Info[nCount].set.pos, g_model_Info[nCount].set.rot, pPlayer[0].nCntModel_M);
		SettingPolygon(g_model_Info[nCount].set.pos);
	}
}

//=============================================================================
// 会話更新処理
//=============================================================================
void UpdateSpeech(void)
{
	PLAYER *pPlayer = GetPlayer();

	if (pPlayer[0].nScnSpeech > -1)
	{
		switch (pPlayer[0].nScnSpeech)
		{
		case 0:
			Speech_Tutorial(); 
			break;
		}
	}
}
//=============================================================================
// 会話更新処理 チュートリアル編
//=============================================================================
void Speech_Tutorial(void)
{
	PLAYER *pPlayer = GetPlayer();
	char aStr[84];

	switch (pPlayer[0].nNumSpeech)
	{
	case 0:
		pPlayer[0].nCntSTime = 0;
		pPlayer[0].nNumSpeech++;
		pPlayer[0].nCntSpeech = SetWindow(D3DXVECTOR3(255, SCREEN_HEIGHT * 0.65f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 0.0f }, 250, 70, 7, 0, 1, 1, 0, WINDOW_UPDATE_GAME, WINDOWSTATE_FADEIN, WINDOWUSE_GAME, WINDOWTYPE_CENTER, WINDOW_DRAW_1);
		strcpy(&aStr[0], "基本的な操作方法を説明するゴブ");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		SetFont(RECT{ 150 , 422, 1280, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &aStr[0], 24, 7, DT_LEFT, 60, true, FONTSTATE_NONE, WINDOW_DRAW_1);
		ChangeFont(true, &aStr[0], 24, 0, 0, 60, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case 1:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nCntSTime = 0; pPlayer[0].nNumSpeech++;
		strcpy(&aStr[0], "[Lスティック][WASD]で移動、\n[R1][L_SHIFT]を押しながらだと\n走れるゴブ");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		ChangeFont(true, &aStr[0], 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;

	case 2:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nCntSTime = 0; pPlayer[0].nNumSpeech++;
		strcpy(&aStr[0], "↑：マップ切り替え　↓：キャンセル\n←：スキルL2を捨てる\n→：スキルR2を捨てる");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		ChangeFont(true, &aStr[0], 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;
	case 3:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nCntSTime = 0; pPlayer[0].nNumSpeech++;
		strcpy(&aStr[0], "[L1][K]でガード\n[スタートボタン][P]でポーズ");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		ChangeFont(true, &aStr[0], 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;
	case 4:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nCntSTime = 0; pPlayer[0].nNumSpeech++;
		strcpy(&aStr[0], "[Xボタン][J]で弱攻撃\n[Yボタン][I]で\n捕食/捕食攻撃ができるゾ");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		ChangeFont(true, &aStr[0], 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;
	case 5:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nCntSTime = 0; pPlayer[0].nNumSpeech++;
		strcpy(&aStr[0], "[Bボタン][SPACE]でジャンプ\n[Aボタン][L]で\nブーストできるゴブ");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		ChangeFont(true, &aStr[0], 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;
	case 6:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nCntSTime = 0; pPlayer[0].nNumSpeech++;
		strcpy(&aStr[0], "[L2 + X][Q + J]でスキル１\n[L2 + Y][Q + I]でスキル２を発動だ！");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		ChangeFont(true, &aStr[0], 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;
	case 7:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nCntSTime = 0; pPlayer[0].nNumSpeech++;
		strcpy(&aStr[0], "[Bボタン][SPACE]でジャンプ\n[Aボタン][L]でブーストできるゴブ");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		ChangeFont(true, &aStr[0], 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;
	case 8:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nCntSTime = 0; pPlayer[0].nNumSpeech++;
		strcpy(&aStr[0], "[Rスティック]でカメラ操作\n[Y/H]が上下\n[U/O]で左右だゴブ");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		ChangeFont(true, &aStr[0], 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;
	case 9:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nCntSTime = 0; pPlayer[0].nNumSpeech++;
		strcpy(&aStr[0], "[R2][E]でロックオンだゴブ");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		ChangeFont(true, &aStr[0], 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;
	case 10:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nCntSTime = 0; pPlayer[0].nNumSpeech++;
		strcpy(&aStr[0], "これで操作方法の説明は終わりゴブ");
		pPlayer[0].nCntSStop = (strlen(&aStr[0]) / 2) * SPEECH_FLOW + SPEECH_STOP;
		ChangeFont(true, &aStr[0], 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;

	case 11:if (pPlayer[0].nCntSTime == pPlayer[0].nCntSStop) {
		pPlayer[0].nScnSpeech = -1;
		ChangeWindow(pPlayer[0].nCntSpeech, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WINDOWSTATE_FADEOUT);
		ChangeFont(false, NULL, 24, 0, 0, 3, FONTSTATE_NORMAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}break;
	}
	pPlayer[0].nCntSTime++;
}
//=============================================================================
// 敵の更新処理
//=============================================================================
void UpdateEnemy(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode)
{
	Camera *pCamera = GetCamera();

	if (pCamera[0].state == CAMERA_STATE_GAME)
	{
		switch (use)
		{
		case MODEL_USE_ENEMY_B://全開
			UpdateBEnemy(&set[0], &data[0], use, nMode);
			break;

		case MODEL_USE_ENEMY_Z://近づいて殴るだけ ごみ
			UpdateZEnemy(&set[0], &data[0], use, nMode);
			break;

		case MODEL_USE_ENEMY_ZE://近づいて殴るだけ　ごみ
			UpdateZEnemyE(&set[0], &data[0], use, nMode);
			break;

		case MODEL_USE_ENEMY_S://近づいて殴る解放 待ち時間基本無し
			UpdateSEnemy(&set[0], &data[0], use, nMode);
			break;

		case MODEL_USE_ENEMY_T://ステップ解放
			UpdateSEnemy(&set[0], &data[0], use, nMode);
			break;
		}
	}
}
//=============================================================================
// 敵の更新処理
//=============================================================================
void UpdateSubEnemy(MODEL_SETTING *set, CHAR_DATA *data, float fRot, bool bMove)
{
	if (set[0].nNumMotion[1] < MOTION_STEP || set[0].nMotionFrame == 0)
	{
		if (set[0].nNumMotion[1] < MOTION_LANDING)
		{
			if (set[0].bRun == true)
			{
				//g_model_Info[nCount].data.nLifePoint -= 100.0f;
				if (data[0].nStamina > 0)
				{
					set[0].fSpeed = (MODEL_PMOVE0 + data[0].status.fSpeed * MODEL_PMOVE) * MODEL_RUN_MOVE;
				}
				else { set[0].fSpeed = (MODEL_PMOVE0 + data[0].status.fSpeed * MODEL_PMOVE); }
			}
			else
			{
				set[0].fSpeed = (MODEL_PMOVE0 + data[0].status.fSpeed * MODEL_PMOVE);
				set[0].bRun = false;
			}
		}//着地より上はSPEEDなし
		else if (set[0].nNumMotion[1] > MOTION_LANDING) { set[0].fSpeed = 0.0f; }
		else { set[0].fSpeed = (MODEL_PMOVE0 + data[0].status.fSpeed * MODEL_PMOVE); }
	}
	else { set[0].fSpeed = 0.0f; }

	if (bMove == true)
	{
		if (data[0].nStamina < 50) { set[0].bRun = false; }
		set[0].angle.y = fRot;
		set[0].move += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -set[0].fSpeed;
	}

	if (set[0].nNumMotion[1] < MOTION_STEP || set[0].nMotionFrame < 1)
	{//現在のモーションが攻撃していない場合
		if (set[0].move.x > 0.1f || set[0].move.x < -0.1f || set[0].move.z > 0.1f || set[0].move.z < -0.1f)
		{//移動中 カメラの角度に自機の角度を合わせる
			float fRotPoor = set[0].angle.y - set[0].rot.y;
			if (fRotPoor > D3DX_PI) { fRotPoor -= D3DX_PI * 2.0f; }
			else if (fRotPoor < -D3DX_PI) { fRotPoor += D3DX_PI * 2.0f; }
			set[0].rot.y += fRotPoor * 0.15f;
		}
		if (set[0].rot.y > D3DX_PI) { set[0].rot.y -= D3DX_PI * 2; }
		if (set[0].rot.y < -D3DX_PI) { set[0].rot.y += D3DX_PI * 2; }
	}

}
//=============================================================================
// 敵の更新処理
//=============================================================================
void UpdateBEnemy(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode)
{
	if (set[0].nPlayTime > 0) { set[0].nPlayTime = -1; }
	ENEMY *pEnemy = GetEnemy();
	float fDistance;
	float fDistanceX;
	float fDistanceZ;
	//float fDistanceY;
	float fTarget = ReturnAngle(&set[0]);
	float fRot = fTarget;
	int nDistance;
	bool bMove = true;

	//距離計算posoldで
	fDistanceX = set[0].posold.x - g_model_Info[0].set.posold.x;
	fDistanceZ = set[0].posold.z - g_model_Info[0].set.posold.z;
	//fDistanceY = set[0].posold.y - g_model_Info[0].set.posold.y;
	//fDistance = 0;
	//if (fDistanceY < 0) { fDistanceY *= -1; }
	fDistance = fDistanceX * fDistanceX + fDistanceZ * fDistanceZ;

	if (fDistance < 150.0f * 150.0f) { nDistance = 0; }
	else if (fDistance < 250.0f * 250.0f) { nDistance = 1; }
	else if (fDistance < 500.0f * 500.0f) { nDistance = 2; }
	else { nDistance = 3; }
	if (nDistance != pEnemy[set[0].nCntChar].nDistance)
	{//距離が変わった
		pEnemy[set[0].nCntChar].nCntTime = 0;
		pEnemy[set[0].nCntChar].nMaxTime = 0;
		pEnemy[set[0].nCntChar].nDistance = nDistance;
		set[0].bRun = false;
	}

	pEnemy[set[0].nCntChar].nCntTime++;
	switch (set[0].state)
	{
		//攻撃して当たらなかった
	case MODEL_STATE_ATK:
		//強回避に以降
		if (set[0].nNumMotion[2] != -2) { set[0].nNumMotion[2] = -2; }
		else { set[0].nNumMotion[2] = -1; }
		break;

	case MODEL_STATE_ATKHIT:	//攻撃が当たっている
		//敵の無敵時間が一定以下
		if ((g_model_Info[0].set.nAirTime < 5 && g_model_Info[0].set.state != MODEL_STATE_GUARD) || nMode != 0)
		{
			if (set[0].nNumMotion[1] == MOTION_STEP || set[0].nNumMotion[1] == MOTION_STEPS)
			{//ステップしているなら　スキルにつなげる　nPlayTime = ...;
				if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[0][0]].key[0].nAtkEffect) && set[0].nSkillTime[0] < 1)
				{//弱スキル使用可能なら	MPが足りている&&使用時間
					set[0].nNumMotion[2] = -1; set[0].nPlayTime = 50;
					if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[1][0]].key[0].nAtkEffect) && set[0].nSkillTime[1] < 1)
					{//強スキル使用可能なら	MPが足りている&&使用時間
						set[0].nPlayTime = 65;
					}
				}
				else if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[1][0]].key[0].nAtkEffect) && set[0].nSkillTime[1] < 1)
				{//強スキル使用可能なら	MPが足りている&&使用時間
					set[0].nNumMotion[2] = -1; set[0].nPlayTime = 40;
				}
				else if (set[0].nSkillTime[0] > 0)
				{//コンボ中に弱攻撃を使用しているなら
					set[0].nNumMotion[2] = -2;
				}
				else
				{//MPがないなら
					set[0].nNumMotion[2] = -1;
				}
			}
			else
			{//現在がステップでない
				if (nMode != 0)
				{//前がステップだった
					if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[0][0]].key[0].nAtkEffect) && set[0].nSkillTime[0] < 1)
					{//弱スキル使用可能なら	MPが足りている&&使用時間
						set[0].nNumMotion[2] = -3;
					}
					else if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[1][0]].key[0].nAtkEffect) && set[0].nSkillTime[1] < 1)
					{//強スキル使用可能なら	MPが足りている&&使用時間
						set[0].nNumMotion[2] = -4;
					}
				}
				else
				{//コンボ中
					if (set[0].nNumMotion[2] != -1)
					{//弱攻撃を希望
						set[0].nNumMotion[2] = -1;	//弱攻撃に以降
					}
					else
					{//モーションが切り替わっていない
						if (data[0].nStamina > 30) { set[0].nNumMotion[2] = MOTION_STEP; }//ステップ使用可能なら　スタミナが安全圏													   
						else if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[0][0]].key[0].nAtkEffect) && set[0].nSkillTime[0] < 1)
						{//弱スキル使用可能なら	MPが足りている&&使用時間
							set[0].nNumMotion[2] = -3;
						}
						else if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[1][0]].key[0].nAtkEffect) && set[0].nSkillTime[1] < 1)
						{//強スキル使用可能なら	MPが足りている&&使用時間
							set[0].nNumMotion[2] = -4;
						}
					}
				}
			}
		}
		else if (g_model_Info[0].set.nAirTime < 3)
		{//距離を取る
			if (data[0].nStamina > 30)
			{
				set[0].nNumMotion[2] = MOTION_STEP;
				fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
			}
			else
			{
				if (set[0].nNumMotion[2] != -2){ set[0].nNumMotion[2] = -2; }
				else{ set[0].nNumMotion[2] = -1; }//モーションが切り替わっていない
				
			}
		}
		break;

	case MODEL_STATE_GUARD:		//ガード成功
		if (data[0].nStamina > 30)
		{
			set[0].nNumMotion[2] = MOTION_STEP;
			fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
		}
		break;

	case MODEL_STATE_NONE:	//通常時
		if (fDistance < 1000.0f * 1000.0f) { set[0].nLockChar = 0; }
		else if (use != MODEL_USE_ENEMY_B) { set[0].nLockChar = -1; }

		if (set[0].nLockChar != -1)
		{//プレイヤーを発見している
			if (g_model_Info[0].set.nAirTime > 0)
			{//プレイヤー無敵
				if (nDistance < 2)
				{//中距離後ろ方向ランドでステップ or ダッシュ or 歩き
					PointEnemy(&set[0], 2);
					if (pEnemy[set[0].nCntChar].nAction[0] == 0) { set[0].nNumMotion[2] = MOTION_STEPS; }
					if (data[0].nStamina > 100) { set[0].bRun = true; }
					fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
				}
				else
				{//遠距離 シャゲダン
					set[0].bRun = false;
					if (pEnemy[set[0].nCntChar].nCntTime % 4 < 2) { fRot = fTarget + D3DX_PI * 0.49f; }
					else { fRot = fTarget - D3DX_PI * 0.49f; }
				}
			}
			else
			{//無敵でない
				if (nDistance == 0)
				{//近距離なら　弱攻撃 or ガード or 回避
					if (set[0].nNumMotion[1] == MOTION_STEP || set[0].nNumMotion[1] == MOTION_STEPS)
					{//ステップ中
						if (rand() % 2 == 0)
						{ 
							set[0].nNumMotion[2] = -1;
							if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[0][0]].key[0].nAtkEffect) && set[0].nSkillTime[0] < 1)
							{//弱スキル使用可能なら	MPが足りている&&使用時間
								set[0].nPlayTime = 50;
								if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[1][0]].key[0].nAtkEffect) && set[0].nSkillTime[1] < 1)
								{//強スキル使用可能なら	MPが足りている&&使用時間
									set[0].nPlayTime = 60;
								}
							}
						}
						else { set[0].nNumMotion[2] = -2; }
					}
					else
					{//ステップしていない
						PointEnemy(&set[0], 4);

						fRot = fTarget;
						if (pEnemy[set[0].nCntChar].nAction[0] == 0) { set[0].nNumMotion[2] = -1; }
						else if (pEnemy[set[0].nCntChar].nAction[0] == 1) { set[0].nNumMotion[2] = -2; }
						else if (pEnemy[set[0].nCntChar].nAction[0] == 2) { set[0].nNumMotion[2] = MOTION_GUARD; }
						else
						{//避け
							if (data[0].nStamina > 30)
							{
								set[0].nNumMotion[2] = MOTION_STEP;
							}
							fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
						}
					}
				}
				else if (nDistance == 1)
				{//中距離なら　強攻撃 or 詰めろ(ステップ or ダッシュ) or 様子見(ステップ or ジャンプ or ダッシュ or 歩き) 
					if (set[0].nNumMotion[1] == MOTION_STEP || set[0].nNumMotion[1] == MOTION_STEPS)
					{//ステップ中
						if (rand() % 2 == 0)
						{ 
							set[0].nNumMotion[2] = -1;
							if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[0][0]].key[0].nAtkEffect) && set[0].nSkillTime[0] < 1)
							{//弱スキル使用可能なら	MPが足りている&&使用時間
								set[0].nPlayTime = 45;
								if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[1][0]].key[0].nAtkEffect) && set[0].nSkillTime[1] < 1)
								{//強スキル使用可能なら	MPが足りている&&使用時間
									set[0].nPlayTime = 55;
								}
							}
						}
						else { set[0].nNumMotion[2] = -2; }
					}
					else
					{//ステップしていない
						PointEnemy(&set[0], 3);

						if (pEnemy[set[0].nCntChar].nAction[0] == 0 && data[0].nStamina > 100)
						{//詰めろ
							if (rand() % 2 == 0) { set[0].nNumMotion[2] = MOTION_STEP; pEnemy[set[0].nCntChar].nCntTime += 200; }
							else { set[0].nNumMotion[2] = MOTION_LANDING; pEnemy[set[0].nCntChar].nCntTime += 200; }
							fRot = fTarget - D3DX_PI * 0.4f + (D3DX_PI * 0.8f) * (rand() % 2);

						}
						else if (pEnemy[set[0].nCntChar].nAction[0] == 2)
						{//強攻撃
							pEnemy[set[0].nCntChar].nCntTime += 10000;
							set[0].nNumMotion[2] = -2;
						}
						else if (pEnemy[set[0].nCntChar].nAction[0] == 3)
						{
							if (pEnemy[set[0].nCntChar].nCntTime % 4 < 2) { fRot = fTarget + D3DX_PI * 0.49f; }
							else { fRot = fTarget - D3DX_PI * 0.49f; }
							if (data[0].nStamina > 30) { pEnemy[set[0].nCntChar].nAction[0] = 1; }
						}
						else
						{//避け
							if (data[0].nStamina > 30)
							{//スタミナがある場合
								set[0].nNumMotion[2] = MOTION_STEP; pEnemy[set[0].nCntChar].nCntTime += 200;
								fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
							}
							else
							{//スタミナがない
								if (rand() % 2 == 0)
								{//ガード
									set[0].nNumMotion[2] = MOTION_GUARD; pEnemy[set[0].nCntChar].nCntTime += 200;
								}
								else
								{//シャゲダンへ
									pEnemy[set[0].nCntChar].nMaxTime = 120;
									pEnemy[set[0].nCntChar].nAction[0] = 3;
									set[0].bRun = false;
								}
							}
						}
					}
				}
				else if (nDistance == 2)
				{//中遠距離　詰めろ(ステップ or ダッシュ) 様子見(歩き　ダッシュ)
					PointEnemy(&set[0], 3);
					if (pEnemy[set[0].nCntChar].nAction[0] == 0)
					{//詰めろ
						if (pEnemy[set[0].nCntChar].nAction[1] < 0) { pEnemy[set[0].nCntChar].nAction[1] = rand() % 2; }
						pEnemy[set[0].nCntChar].nMaxTime = 120;

						if (pEnemy[set[0].nCntChar].nAction[1] == 0) { set[0].nNumMotion[2] = MOTION_STEP; pEnemy[set[0].nCntChar].nCntTime += 200; }

						if (data[0].nStamina > 200) { set[0].bRun = true; }
						else if (data[0].nStamina < 150) { set[0].bRun = false; }

						fRot = fTarget;
					}
					else if (pEnemy[set[0].nCntChar].nAction[0] == 1)
					{//様子見
						if (pEnemy[set[0].nCntChar].nAction[1] < 0) { pEnemy[set[0].nCntChar].nAction[1] = rand() % 2; }
						if (pEnemy[set[0].nCntChar].nAction[2] < 0) { pEnemy[set[0].nCntChar].nAction[2] = rand() % 2; }
						pEnemy[set[0].nCntChar].nMaxTime = 80;

						if (pEnemy[set[0].nCntChar].nAction[2] == 0) { set[0].nNumMotion[2] = MOTION_STEP; pEnemy[set[0].nCntChar].nCntTime += 100; }
						if (data[0].nStamina > 300) { set[0].bRun = true; }
						else if (data[0].nStamina < 250) { set[0].bRun = false; }

						if (pEnemy[set[0].nCntChar].nAction[1] == 0) { fRot = fTarget + D3DX_PI * 0.4f; }
						else { fRot = fTarget - D3DX_PI * 0.4f; }
					}
					else
					{//シャゲダン
						pEnemy[set[0].nCntChar].nMaxTime = 30;
						set[0].bRun = false;
						if (pEnemy[set[0].nCntChar].nCntTime % 4 < 2) { fRot = fTarget + D3DX_PI * 0.49f; }
						else { fRot = fTarget - D3DX_PI * 0.49f; }
					}
				}
				else
				{//遠距離　歩き　近づく
					if (data[0].nStamina > 300) { set[0].bRun = true; }
					else if (data[0].nStamina < 250) { set[0].bRun = false; }

					fRot = fTarget;
				}
			}
		}
		break;
	}
	UpdateSubEnemy(&set[0], &data[0], fRot, bMove);
}
//=============================================================================
// 敵の更新処理
//=============================================================================
void UpdateTEnemy(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode)
{
	if (set[0].nPlayTime > 0) { set[0].nPlayTime = -1; }
	ENEMY *pEnemy = GetEnemy();
	float fDistance;
	float fDistanceX;
	float fDistanceZ;
	float fTarget = ReturnAngle(&set[0]);
	float fRot = fTarget;
	int nDistance;
	bool bMove = true;

	//距離計算posoldで
	fDistanceX = set[0].posold.x - g_model_Info[0].set.posold.x;
	fDistanceZ = set[0].posold.z - g_model_Info[0].set.posold.z;
	fDistance = fDistanceX * fDistanceX + fDistanceZ * fDistanceZ;

	if (fDistance < 150.0f * 150.0f) { nDistance = 0; }
	else if (fDistance < 250.0f * 250.0f) { nDistance = 1; }
	else if (fDistance < 500.0f * 500.0f) { nDistance = 2; }
	else { nDistance = 3; }
	if (nDistance != pEnemy[set[0].nCntChar].nDistance)
	{//距離が変わった
		pEnemy[set[0].nCntChar].nCntTime = 0;
		pEnemy[set[0].nCntChar].nMaxTime = 0;
		pEnemy[set[0].nCntChar].nDistance = nDistance;
		set[0].bRun = false;
	}

	pEnemy[set[0].nCntChar].nCntTime++;
	switch (set[0].state)
	{
		//攻撃して当たらなかった
	case MODEL_STATE_ATK:
		//強回避に以降
		if (set[0].nNumMotion[2] != -2) { set[0].nNumMotion[2] = -2; }
		else { set[0].nNumMotion[2] = -1; }
		break;

	case MODEL_STATE_ATKHIT:	//攻撃が当たっている
		if (g_model_Info[0].set.nAirTime < 3)
		{//敵の無敵時間が一定以下
			set[0].nNumMotion[2] = -1;//殴る
		}
		break;

	case MODEL_STATE_NONE:	//通常時
		if (fDistance < 1000.0f * 1000.0f) { set[0].nLockChar = 0; }
		else if (use != MODEL_USE_ENEMY_B) { set[0].nLockChar = -1; }

		if (set[0].nLockChar != -1)
		{//プレイヤーを発見している
			if (g_model_Info[0].set.nAirTime > 0)
			{//プレイヤー無敵
				if (nDistance < 2)
				{//中距離後ろ方向ランドでステップ or ダッシュ or 歩き
					PointEnemy(&set[0], 2);
					if (pEnemy[set[0].nCntChar].nAction[0] == 0) { set[0].nNumMotion[2] = MOTION_STEPS; }
					if (data[0].nStamina > 100) { set[0].bRun = true; }
					fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
				}
				else
				{//遠距離 何もしない
					set[0].bRun = false;
					bMove = false;
				}
			}
			else
			{//無敵でない
				if (nDistance == 0)
				{//近距離なら　弱攻撃 or ガード or 回避
					if (set[0].nNumMotion[1] == MOTION_STEP || set[0].nNumMotion[1] == MOTION_STEPS)
					{//ステップ中
						if (rand() % 2 == 0) { set[0].nNumMotion[2] = -1; set[0].nPlayTime = 60; }
						else { set[0].nNumMotion[2] = -2; }
					}
					else
					{//ステップしていない
						PointEnemy(&set[0], 4);

						fRot = fTarget;
						if (pEnemy[set[0].nCntChar].nAction[0] == 0) { set[0].nNumMotion[2] = -1; }
						else if (pEnemy[set[0].nCntChar].nAction[0] == 1) { set[0].nNumMotion[2] = -2; }
						else if (pEnemy[set[0].nCntChar].nAction[0] == 2) { set[0].nNumMotion[2] = MOTION_GUARD; }
						else
						{//避け
							if (data[0].nStamina > 30)
							{
								set[0].nNumMotion[2] = MOTION_STEP;
							}
							fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
						}
					}
				}
				else if (nDistance == 1)
				{//中距離なら　強攻撃 or 詰めろ(ステップ or ダッシュ) or 様子見(ステップ or ジャンプ or ダッシュ or 歩き) 
					if (set[0].nNumMotion[1] == MOTION_STEP || set[0].nNumMotion[1] == MOTION_STEPS)
					{//ステップ中
						if (rand() % 2 == 0) { set[0].nNumMotion[2] = -1; set[0].nPlayTime = 60; }
						else { set[0].nNumMotion[2] = -2; }
					}
					else
					{//ステップしていない
						PointEnemy(&set[0], 10);

						if (pEnemy[set[0].nCntChar].nAction[0] < 3 && data[0].nStamina > 100)
						{//詰めろ
							if (rand() % 2 == 0) { set[0].nNumMotion[2] = MOTION_STEP; pEnemy[set[0].nCntChar].nCntTime += 200; }
							else { set[0].nNumMotion[2] = MOTION_LANDING; pEnemy[set[0].nCntChar].nCntTime += 200; }
							fRot = fTarget - D3DX_PI * 0.4f + (D3DX_PI * 0.8f) * (rand() % 2);

						}
						else if (pEnemy[set[0].nCntChar].nAction[0] < 3)
						{//強攻撃
							pEnemy[set[0].nCntChar].nCntTime += 10000;
							set[0].nNumMotion[2] = -2;
						}
						else if (pEnemy[set[0].nCntChar].nAction[0] < 6)
						{
							set[0].bRun = false;
							bMove = false;
							if (data[0].nStamina > 30) { pEnemy[set[0].nCntChar].nAction[0] = 1; }
						}
						else if (pEnemy[set[0].nCntChar].nAction[0] < 9)
						{//避け
							if (data[0].nStamina > 30)
							{//スタミナがある場合
								set[0].nNumMotion[2] = MOTION_STEP; pEnemy[set[0].nCntChar].nCntTime += 200;
								fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
							}
							else
							{//スタミナがない
								if (rand() % 2 == 0)
								{//ガード
									set[0].nNumMotion[2] = MOTION_GUARD; pEnemy[set[0].nCntChar].nCntTime += 200;
								}
								else
								{//通常へ
									pEnemy[set[0].nCntChar].nMaxTime = 120;
									pEnemy[set[0].nCntChar].nAction[0] = 3;
									set[0].bRun = false;
								}
							}
						}
						else
						{
							pEnemy[set[0].nCntChar].nCntTime += 10000;
							if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[0][0]].key[0].nAtkEffect) && set[0].nSkillTime[0] < 1)
							{//弱スキル使用可能なら	MPが足りている&&使用時間
								set[0].nNumMotion[2] = -3;
							}
						}
					}
				}
				else if (nDistance == 2)
				{//中遠距離　詰めろ(ステップ or ダッシュ) 様子見(歩き　ダッシュ)
					PointEnemy(&set[0], 3);
					if (pEnemy[set[0].nCntChar].nAction[0] == 0)
					{//詰めろ
						if (pEnemy[set[0].nCntChar].nAction[1] < 0) { pEnemy[set[0].nCntChar].nAction[1] = rand() % 2; }
						pEnemy[set[0].nCntChar].nMaxTime = 120;

						if (pEnemy[set[0].nCntChar].nAction[1] == 0) { set[0].nNumMotion[2] = MOTION_STEP; pEnemy[set[0].nCntChar].nCntTime += 200; }

						if (data[0].nStamina > 200) { set[0].bRun = true; }
						else if (data[0].nStamina < 150) { set[0].bRun = false; }

						fRot = fTarget;
					}
					else if (pEnemy[set[0].nCntChar].nAction[0] == 1)
					{//様子見
						if (pEnemy[set[0].nCntChar].nAction[1] < 0) { pEnemy[set[0].nCntChar].nAction[1] = rand() % 2; }
						if (pEnemy[set[0].nCntChar].nAction[2] < 0) { pEnemy[set[0].nCntChar].nAction[2] = rand() % 2; }
						pEnemy[set[0].nCntChar].nMaxTime = 80;

						if (pEnemy[set[0].nCntChar].nAction[2] == 0) { set[0].nNumMotion[2] = MOTION_STEP; pEnemy[set[0].nCntChar].nCntTime += 100; }
						if (data[0].nStamina > 300) { set[0].bRun = true; }
						else if (data[0].nStamina < 250) { set[0].bRun = false; }

						if (pEnemy[set[0].nCntChar].nAction[1] == 0) { fRot = fTarget + D3DX_PI * 0.4f; }
						else { fRot = fTarget - D3DX_PI * 0.4f; }
					}
					else
					{//通常
						pEnemy[set[0].nCntChar].nMaxTime = 30;
						set[0].bRun = false;
						bMove = false;
					}
				}
				else
				{//遠距離　歩き　近づく
					if (data[0].nStamina > 300) { set[0].bRun = true; }
					else if (data[0].nStamina < 250) { set[0].bRun = false; }

					fRot = fTarget;
				}
			}
		}
		else { bMove = false; }
		break;
	}

	UpdateSubEnemy(&set[0], &data[0], fRot, bMove);
}
//=============================================================================
// 敵の更新処理
//=============================================================================
void UpdateSEnemy(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode)
{
	if (set[0].nPlayTime > 0) { set[0].nPlayTime = -1; }
	ENEMY *pEnemy = GetEnemy();
	float fDistance;
	float fDistanceX;
	float fDistanceZ;
	float fTarget = ReturnAngle(&set[0]);
	float fRot = fTarget;
	int nDistance;
	bool bMove = true;

	//距離計算posoldで
	fDistanceX = set[0].posold.x - g_model_Info[0].set.posold.x;
	fDistanceZ = set[0].posold.z - g_model_Info[0].set.posold.z;
	fDistance = fDistanceX * fDistanceX + fDistanceZ * fDistanceZ;

	if (fDistance < 150.0f * 150.0f) { nDistance = 0; }
	else if (fDistance < 250.0f * 250.0f) { nDistance = 1; }
	else if (fDistance < 500.0f * 500.0f) { nDistance = 2; }
	else { nDistance = 3; }
	if (nDistance != pEnemy[set[0].nCntChar].nDistance)
	{//距離が変わった
		pEnemy[set[0].nCntChar].nCntTime = 0;
		pEnemy[set[0].nCntChar].nMaxTime = 0;
		pEnemy[set[0].nCntChar].nDistance = nDistance;
		set[0].bRun = false;
	}

	pEnemy[set[0].nCntChar].nCntTime++;
	switch (set[0].state)
	{
		//攻撃して当たらなかった
	case MODEL_STATE_ATK:
		//強回避に以降
		if (set[0].nNumMotion[2] != -2) { set[0].nNumMotion[2] = -2; }
		else { set[0].nNumMotion[2] = -1; }
		break;

	case MODEL_STATE_ATKHIT:	//攻撃が当たっている
		if (g_model_Info[0].set.nAirTime < 3)
		{//敵の無敵時間が一定以下
			set[0].nNumMotion[2] = -1;//殴る
		}
		break;

	case MODEL_STATE_NONE:	//通常時
		if (fDistance < 1000.0f * 1000.0f) { set[0].nLockChar = 0; }
		else if (use != MODEL_USE_ENEMY_B) { set[0].nLockChar = -1; }

		if (set[0].nLockChar != -1)
		{//プレイヤーを発見している
			if (g_model_Info[0].set.nAirTime > 0)
			{//プレイヤー無敵
				if (nDistance < 2)
				{//中距離後ろ方向ランドでステップ or ダッシュ or 歩き
					if (data[0].nStamina > 100) { set[0].bRun = true; }
					fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
				}
				else
				{//遠距離 何もしない
					set[0].bRun = false;
					bMove = false;
				}
			}
			else
			{//無敵でない
				if (nDistance == 0)
				{//近距離なら　弱攻撃 or ガード or 回避
					PointEnemy(&set[0], 2);

					fRot = fTarget;
					if (pEnemy[set[0].nCntChar].nAction[0] == 0) { set[0].nNumMotion[2] = -1; }
					else if (pEnemy[set[0].nCntChar].nAction[0] == 1) { set[0].nNumMotion[2] = -2; }
				}
				else if (nDistance == 1)
				{//中距離なら　強攻撃 or 詰めろ(ステップ or ダッシュ) or 様子見(ステップ or ジャンプ or ダッシュ or 歩き) 
					PointEnemy(&set[0], 7);

					if (pEnemy[set[0].nCntChar].nAction[0] < 3 && data[0].nStamina > 100)
					{//詰めろ
						set[0].nNumMotion[2] = MOTION_LANDING; pEnemy[set[0].nCntChar].nCntTime += 200;
						set[0].bRun = true;
						fRot = fTarget - D3DX_PI * 0.4f + (D3DX_PI * 0.8f) * (rand() % 2);

					}
					else if(pEnemy[set[0].nCntChar].nAction[0] < 6)
					{//強攻撃
						pEnemy[set[0].nCntChar].nCntTime += 10000;
						set[0].nNumMotion[2] = -2;
					}
					else
					{
						pEnemy[set[0].nCntChar].nCntTime += 10000;
						if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[0][0]].key[0].nAtkEffect) && set[0].nSkillTime[0] < 1)
						{//弱スキル使用可能なら	MPが足りている&&使用時間
							set[0].nNumMotion[2] = -3;
						}
					}
				}
				else if (nDistance == 2)
				{//中遠距離　詰めろ(ステップ or ダッシュ) 様子見(歩き　ダッシュ)
					PointEnemy(&set[0], 3);
					if (pEnemy[set[0].nCntChar].nAction[0] == 0)
					{//詰めろ
						pEnemy[set[0].nCntChar].nMaxTime = 120;

						if (data[0].nStamina > 200) { set[0].bRun = true; }
						else if (data[0].nStamina < 150) { set[0].bRun = false; }

						fRot = fTarget;
					}
					else if (pEnemy[set[0].nCntChar].nAction[0] == 1)
					{//様子見
						if (pEnemy[set[0].nCntChar].nAction[1] < 0) { pEnemy[set[0].nCntChar].nAction[1] = rand() % 2; }
						pEnemy[set[0].nCntChar].nMaxTime = 80;

						if (data[0].nStamina > 300) { set[0].bRun = true; }
						else if (data[0].nStamina < 250) { set[0].bRun = false; }

						if (pEnemy[set[0].nCntChar].nAction[1] == 0) { fRot = fTarget + D3DX_PI * 0.4f; }
						else { fRot = fTarget - D3DX_PI * 0.4f; }
					}
					else
					{//通常
						pEnemy[set[0].nCntChar].nMaxTime = 30;
						set[0].bRun = false;
						bMove = false;
					}
				}
				else
				{//遠距離　歩き　近づく
					if (data[0].nStamina > 300) { set[0].bRun = true; }
					else if (data[0].nStamina < 250) { set[0].bRun = false; }

					fRot = fTarget;
				}
			}
		}
		else { bMove = false; }
		break;
	}

	UpdateSubEnemy(&set[0], &data[0], fRot, bMove);
}

//=============================================================================
// 敵の更新処理 遠距離型
//=============================================================================
void UpdateZEnemyE(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode)
{
	if (set[0].nPlayTime > 0) { set[0].nPlayTime = -1; }
	ENEMY *pEnemy = GetEnemy();
	float fDistance;
	float fDistanceX;
	float fDistanceZ;
	float fTarget = ReturnAngle(&set[0]);
	float fRot = fTarget;
	int nDistance;
	bool bMove = true;

	//距離計算posoldで
	fDistanceX = set[0].posold.x - g_model_Info[0].set.posold.x;
	fDistanceZ = set[0].posold.z - g_model_Info[0].set.posold.z;
	fDistance = fDistanceX * fDistanceX + fDistanceZ * fDistanceZ;

	if (fDistance < 350.0f * 350.0f) { nDistance = 0; }
	else if (fDistance < 450.0f * 450.0f) { nDistance = 1; }
	else if (fDistance < 600.0f * 600.0f) { nDistance = 2; }
	else { nDistance = 3; }
	if (nDistance != pEnemy[set[0].nCntChar].nDistance)
	{//距離が変わった
		pEnemy[set[0].nCntChar].nCntTime = 0;
		pEnemy[set[0].nCntChar].nMaxTime = 0;
		pEnemy[set[0].nCntChar].nDistance = nDistance;
		set[0].bRun = false;
	}

	pEnemy[set[0].nCntChar].nCntTime++;
	switch (set[0].state)
	{
		//攻撃して当たらなかった
	case MODEL_STATE_ATK:
		//強回避に以降
		if (set[0].nNumMotion[2] != -2) { set[0].nNumMotion[2] = -2; }
		else { set[0].nNumMotion[2] = -1; }
		break;

	case MODEL_STATE_ATKHIT:	//攻撃が当たっている
		if (g_model_Info[0].set.nAirTime < 3)
		{//敵の無敵時間が一定以下
			set[0].nNumMotion[2] = -1;//殴る
		}
		break;

	case MODEL_STATE_NONE:	//通常時
		if (fDistance < 1000.0f * 1000.0f) { set[0].nLockChar = 0; }
		else if (use != MODEL_USE_ENEMY_B) { set[0].nLockChar = -1; }

		if (set[0].nLockChar != -1)
		{//プレイヤーを発見している
			if (g_model_Info[0].set.nAirTime > 0)
			{//プレイヤー無敵
				if (nDistance < 2)
				{//中距離後ろ方向ランドでステップ or ダッシュ or 歩き
					fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
				}
				else
				{//遠距離 何もしない
					bMove = false;
				}
			}
			else
			{//無敵でない
				if (nDistance == 0)
				{//近距離なら　弱攻撃 or ガード or 回避
						PointEnemy(&set[0], 2);

						fRot = fTarget;
						if (rand() % ENEMY_ATIME == 0)
						{
							if (pEnemy[set[0].nCntChar].nAction[0] == 0) { set[0].nNumMotion[2] = -1; }
							else if (pEnemy[set[0].nCntChar].nAction[0] == 1) { set[0].nNumMotion[2] = -2; }
						}
				}
				else if (nDistance == 1)
				{//中距離なら　強攻撃 or 詰めろ(ステップ or ダッシュ) or 様子見(ステップ or ジャンプ or ダッシュ or 歩き) 
					PointEnemy(&set[0], 7);

					if (pEnemy[set[0].nCntChar].nAction[0] < 3 && data[0].nStamina > 100)
					{//離れる
						pEnemy[set[0].nCntChar].nCntTime += 200;
						fRot = fTarget - D3DX_PI * 0.4f + (D3DX_PI * 0.8f) * (rand() % 2) + D3DX_PI * 0.5f;

					}
					else if (pEnemy[set[0].nCntChar].nAction[0] < 6)
					{//強攻撃
						if (rand() % ENEMY_ATIME == 0)
						{
							pEnemy[set[0].nCntChar].nCntTime += 10000;
							set[0].nNumMotion[2] = -2;
						}
					}
					else
					{
						pEnemy[set[0].nCntChar].nCntTime += 10000;
						if (data[0].nSkillPoint > MotionSkill(g_model_motion[set[0].nNumMotion[0]][data[0].nSkill[0][0]].key[0].nAtkEffect) && set[0].nSkillTime[0] < 1)
						{//弱スキル使用可能なら	MPが足りている&&使用時間
							set[0].nNumMotion[2] = -3;
						}
					}
				}
				else if (nDistance == 2)
				{//中遠距離　詰めろ(ステップ or ダッシュ) 様子見(歩き　ダッシュ)
					PointEnemy(&set[0], 3);
					if (pEnemy[set[0].nCntChar].nAction[0] == 0)
					{//詰めろ
						pEnemy[set[0].nCntChar].nMaxTime = 120;

						fRot = fTarget;
					}
					else if (pEnemy[set[0].nCntChar].nAction[0] == 1)
					{//様子見
						if (pEnemy[set[0].nCntChar].nAction[1] < 0) { pEnemy[set[0].nCntChar].nAction[1] = rand() % 2; }
						pEnemy[set[0].nCntChar].nMaxTime = 80;

						if (pEnemy[set[0].nCntChar].nAction[1] == 0) { fRot = fTarget + D3DX_PI * 0.4f; }
						else { fRot = fTarget - D3DX_PI * 0.4f; }
					}
					else
					{//通常
						pEnemy[set[0].nCntChar].nMaxTime = 30;
						bMove = false;
					}
				}
				else
				{//遠距離　歩き　近づく
					fRot = fTarget;
				}
			}
		}
		else { bMove = false; }
		break;
	}

	set[0].bRun = false;
	UpdateSubEnemy(&set[0], &data[0], fRot, bMove);
}
//=============================================================================
// 敵の更新処理
//=============================================================================
void UpdateZEnemy(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode)
{
	if (set[0].nPlayTime > 0) { set[0].nPlayTime = -1; }
	ENEMY *pEnemy = GetEnemy();
	float fDistance;
	float fDistanceX;
	float fDistanceZ;
	float fTarget = ReturnAngle(&set[0]);
	float fRot = fTarget;
	int nDistance;
	bool bMove = true;

	//距離計算posoldで
	fDistanceX = set[0].posold.x - g_model_Info[0].set.posold.x;
	fDistanceZ = set[0].posold.z - g_model_Info[0].set.posold.z;
	fDistance = fDistanceX * fDistanceX + fDistanceZ * fDistanceZ;

	if (fDistance < 120.0f * 120.0f) { nDistance = 0; }
	else if (fDistance < 250.0f * 250.0f) { nDistance = 1; }
	else if (fDistance < 500.0f * 500.0f) { nDistance = 2; }
	else { nDistance = 3; }
	if (nDistance != pEnemy[set[0].nCntChar].nDistance)
	{//距離が変わった
		pEnemy[set[0].nCntChar].nCntTime = 0;
		pEnemy[set[0].nCntChar].nMaxTime = 0;
		pEnemy[set[0].nCntChar].nDistance = nDistance;
		set[0].bRun = false;
	}

	pEnemy[set[0].nCntChar].nCntTime++;
	switch (set[0].state)
	{
		//攻撃して当たらなかった
	case MODEL_STATE_ATK:
		//強回避に以降
		if (set[0].nNumMotion[2] != -2) { set[0].nNumMotion[2] = -2; }
		else { set[0].nNumMotion[2] = -1; }
		break;

	case MODEL_STATE_ATKHIT:	//攻撃が当たっている
		if (g_model_Info[0].set.nAirTime < 3)
		{//敵の無敵時間が一定以下
			set[0].nNumMotion[2] = -1;//殴る
		}
		break;

	case MODEL_STATE_NONE:	//通常時
		if (fDistance < 1000.0f * 1000.0f) { set[0].nLockChar = 0; }
		else if (use != MODEL_USE_ENEMY_B) { set[0].nLockChar = -1; }

		if (set[0].nLockChar != -1)
		{//プレイヤーを発見している
			if (g_model_Info[0].set.nAirTime > 0)
			{//プレイヤー無敵
				if (nDistance < 2)
				{//中距離後ろ方向ランドでステップ or ダッシュ or 歩き
					fRot = fTarget - D3DX_PI * 0.75f - (D3DX_PI * 0.25f) * (rand() % 3);
				}
				else
				{//遠距離 何もしない
					bMove = false;
				}
			}
			else
			{//無敵でない
				if (nDistance == 0)
				{//近距離なら　弱攻撃 or ガード or 回避
					PointEnemy(&set[0], 2);
					bMove = false;

					fRot = fTarget;
					if (rand() % ENEMY_ATIME == 0)
					{
						if (pEnemy[set[0].nCntChar].nAction[0] == 0) { set[0].nNumMotion[2] = -1; }
						else if (pEnemy[set[0].nCntChar].nAction[0] == 1) { set[0].nNumMotion[2] = -2; }
					}
				}
				else if (nDistance == 1)
				{//中距離なら　強攻撃 or 詰めろ(ステップ or ダッシュ) or 様子見(ステップ or ジャンプ or ダッシュ or 歩き) 
					PointEnemy(&set[0], 2);

					if (pEnemy[set[0].nCntChar].nAction[0] == 0)
					{//詰めろ
						pEnemy[set[0].nCntChar].nCntTime += 200;
						fRot = fTarget - D3DX_PI * 0.4f + (D3DX_PI * 0.8f) * (rand() % 2);
					}
					else
					{//強攻撃
						if (rand() % ENEMY_ATIME == 0)
						{
							pEnemy[set[0].nCntChar].nCntTime += 10000;
							set[0].nNumMotion[2] = -2;
						}
					}
				}
				else if (nDistance == 2)
				{//中遠距離　詰めろ(ステップ or ダッシュ) 様子見(歩き　ダッシュ)
					PointEnemy(&set[0], 3);
					if (pEnemy[set[0].nCntChar].nAction[0] == 0)
					{//詰めろ
						pEnemy[set[0].nCntChar].nMaxTime = 120;
						fRot = fTarget;
					}
					else if (pEnemy[set[0].nCntChar].nAction[0] == 1)
					{//様子見
						if (pEnemy[set[0].nCntChar].nAction[1] < 0) { pEnemy[set[0].nCntChar].nAction[1] = rand() % 2; }
						pEnemy[set[0].nCntChar].nMaxTime = 80;

						if (pEnemy[set[0].nCntChar].nAction[1] == 0) { fRot = fTarget + D3DX_PI * 0.4f; }
						else { fRot = fTarget - D3DX_PI * 0.4f; }
					}
					else
					{//通常
						pEnemy[set[0].nCntChar].nMaxTime = 30;
						bMove = false;
					}
				}
				else
				{//遠距離　歩き　近づく
					fRot = fTarget;
				}
			}
		}
		else{ bMove = false; }
		break;
	}

	set[0].bRun = false;
	UpdateSubEnemy(&set[0], &data[0], fRot, bMove);
}
//=============================================================================
// 敵の更新処理
//=============================================================================
void PointEnemy(MODEL_SETTING *set, int nRand)
{
	ENEMY *pEnemy = GetEnemy();

	if (pEnemy[set[0].nCntChar].nCntTime > pEnemy[set[0].nCntChar].nMaxTime)
	{//思考切り替え
		pEnemy[set[0].nCntChar].nAction[0] = rand() % nRand;
		pEnemy[set[0].nCntChar].nAction[1] = -1;
		pEnemy[set[0].nCntChar].nAction[2] = -1;
		pEnemy[set[0].nCntChar].nCntTime = 0;
		pEnemy[set[0].nCntChar].nMaxTime = 0;
	}
}
//=============================================================================
// ロックオン処理
//=============================================================================
void LockOn_MODEL(int nCount)
{
	int nCntModel;
	int nLockModel = -1;
	float fRot;
	int nLockChange = 0;
	D3DXVECTOR3 pos;
	Camera *pCamera = GetCamera();
	PLAYER *pPlayer = GetPlayer();

	if (g_model_Info[nCount].set.nLockChar > -1)
	{//lock中のキャラが存在
		if (g_model_Info[g_model_Info[nCount].set.nLockChar].bUse == true && g_model_Info[g_model_Info[nCount].set.nLockChar].set.state != MODEL_STATE_DIE)
		{//ロックオンマーカー表示　カメラ補正
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			if(g_model_Info[g_model_Info[nCount].set.nLockChar].set.nAirTime > 5){ col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f); }
			SetZWindow(D3DXVECTOR3(g_model_Info[g_model_Info[nCount].set.nLockChar].set.pos.x
				, g_model_Info[g_model_Info[nCount].set.nLockChar].set.pos.y + 55.0f
				, g_model_Info[g_model_Info[nCount].set.nLockChar].set.pos.z)
				, D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, 30, 30, 2, 0, WINDOWSTATE_Delete0, 0.0f);

			if (pPlayer[0].bButton[BUTTON_U] == false && pPlayer[0].bButton[BUTTON_O] == false)
			{//旋回ボタンを押していないなら
			 //角度計算
				pos = (g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f)) - g_model_Info[g_model_Info[nCount].set.nLockChar].set.pos;
				fRot = atan2f(pos.x, pos.z);
				fRot -= pCamera[0].rot.y;

				if (fRot > D3DX_PI) { fRot -= D3DX_PI * 2.0f; }
				else if (fRot < -D3DX_PI) { fRot += D3DX_PI * 2.0f; }
				pCamera[0].rot.y += fRot * 0.1f;//差分を％で詰める
				if (pCamera[0].rot.y > D3DX_PI) { pCamera[0].rot.y -= D3DX_PI * 2.0f; }
				else if (pCamera[0].rot.y < -D3DX_PI) { pCamera[0].rot.y += D3DX_PI * 2.0f; }

				fRot = atan2f(powf(pos.y, 2), powf(pos.x, 2) + powf(pos.z, 2));
				fRot -= pCamera[0].rot.x;

				if (fRot > D3DX_PI) { fRot -= D3DX_PI * 2.0f; }
				else if (fRot < -D3DX_PI) { fRot += D3DX_PI * 2.0f; }
				pCamera[0].rot.x += fRot * 0.1f;//差分を％で詰める
				if (pCamera[0].rot.x > 0.3f) { pCamera[0].rot.x = 0.3f; }
				else if (pCamera[0].rot.x < -0.3f) { pCamera[0].rot.x = -0.3f; }
			}

			if (pPlayer[0].bButton[BUTTON_E] == true)
			{
				g_model_Info[nCount].set.nLockChar = -1;
				PlaySound(SOUND_LABEL_LOCKOFF);
			}
			else if (pPlayer[0].bButton[BUTTON_X] == true) { nLockChange = -1; }
			else if (pPlayer[0].bButton[BUTTON_C] == true) { nLockChange = 1; }

			if (nLockChange != 0)
			{//ターゲット変更
				nCntModel = g_model_Info[nCount].set.nLockChar;
				while (1)
				{
					nCntModel += nLockChange;

					if (nCntModel < 0) { nCntModel = MAX_MODEL_INFO - 1; }
					if (nCntModel >= MAX_MODEL_INFO) { nCntModel = 0; }

					if (g_model_Info[nCntModel].bUse == true && nCntModel != nCount && g_model_Info[nCntModel].data.nLifePoint > 0 && g_model_Info[nCntModel].use != MODEL_USE_Pause)
					{//trueなら
						g_model_Info[nCount].set.nLockChar = nCntModel;
						PlaySound(SOUND_LABEL_LOCKOFF);

						break;
					}

				}
			}
		}
		else { g_model_Info[nCount].set.nLockChar = -1; }
	}
	else
	{//ロックオンできるもの表示
	 //自分の場所から自分の向きに弾発射
		pos = g_model_Info[nCount].set.pos;
		pos -= D3DXVECTOR3(sinf(pCamera[0].rot.y), 0.0f, cosf(pCamera[0].rot.y)) * -100.0f;
		for (int nCntMove = 0; nCntMove < 3; nCntMove++)
		{
			pos += D3DXVECTOR3(sinf(pCamera[0].rot.y), 0.0f, cosf(pCamera[0].rot.y)) * -200.0f;
			for (nCntModel = 0; nCntModel < MAX_MODEL_INFO; nCntModel++)
			{//全モデル数回転
				if (g_model_Info[nCntModel].bUse == true && nCntModel != nCount && g_model_Info[nCntModel].data.nLifePoint > 0 && g_model_Info[nCntModel].use != MODEL_USE_Pause)
				{//trueなら
					if (g_model_Info[nCntModel].set.pos.x + 200.0f > pos.x && g_model_Info[nCntModel].set.pos.x - 200.0f < pos.x &&
						g_model_Info[nCntModel].set.pos.z + 200.0f > pos.z && g_model_Info[nCntModel].set.pos.z - 200.0f < pos.z)
					{
						nLockModel = nCntModel;
						nCntMove = 3;
						break;
					}
				}//存在確認
			}//for括弧
		}
		if (nLockModel > -1)
		{//当たっているなら　表示 and ロックオン
			if (pPlayer[0].bButton[BUTTON_E] == true)
			{
				g_model_Info[nCount].set.nLockChar = nLockModel;
				PlaySound(SOUND_LABEL_LOCKON);
			}
			SetZWindow(D3DXVECTOR3(g_model_Info[nLockModel].set.pos.x, g_model_Info[nLockModel].set.pos.y + 55.0f, g_model_Info[nLockModel].set.pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 30, 1, 0, WINDOWSTATE_Delete0, 0.0f);
		}
	}
}
//=============================================================================
// ロックオン　攻撃時の処理
//=============================================================================
float ReturnAngle(MODEL_SETTING *set)
{//目的への角度を返す
	float fRot = set[0].rot.y;

	if (set[0].nLockChar > -1)
	{//ロックオンしていれば
		MODEL_INFO *model = GetModel_Info();
		D3DXVECTOR3 pos = set[0].pos - model[set[0].nLockChar].set.pos;
		fRot = atan2f(pos.x, pos.z);
	}

	return fRot;
}
//=============================================================================
// キャラ同士のあたり判定
//=============================================================================
void CollisionModel(int nCount)
{
	D3DXVECTOR3 Extent;		//範囲計算用
	float		fLength;	//目的までの距離
	float		fExtent;	//自分と目的の当たる範囲
	float		fRot;		//角度計算用

	for (int nCntModel = 0; nCntModel < MAX_MODEL_INFO; nCntModel++)
	{
		if (g_model_Info[nCntModel].bUse == true && nCntModel != nCount && g_model_Info[nCntModel].use != MODEL_USE_Pause)
		{//使われているモデルなら
		 //Yが範囲内
			if (g_model_Info[nCntModel].set.pos.y < g_model_Info[nCount].set.pos.y + g_model_Info[nCount].set.fHeight &&
				g_model_Info[nCntModel].set.pos.y > g_model_Info[nCount].set.pos.y - g_model_Info[nCntModel].set.fHeight)
			{
				//自分の円内に入っていたら出す
				Extent = g_model_Info[nCntModel].set.pos - g_model_Info[nCount].set.pos;
				fLength = powf(Extent.x, 2) + powf(Extent.z, 2);
				fExtent = powf(g_model_Info[nCntModel].set.fSlength + g_model_Info[nCount].set.fSlength, 2);
				if (fLength < fExtent)
				{//範囲内
				 //自分と目的の角度計算
					fRot = atan2f(Extent.x, Extent.z);
					//moveの倍率低下
					g_model_Info[nCount].set.move.x *= 0.5f;
					g_model_Info[nCount].set.move.z *= 0.5f;

					if (g_model_Info[nCntModel].set.pos.y + g_model_Info[nCntModel].set.fHeight < g_model_Info[nCount].set.pos.y + (g_model_Info[nCntModel].set.fHeight / 2))
					{//上のほうならmoveだけ
						g_model_Info[nCntModel].set.move += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * ((fExtent - fLength) * 0.01f);
					}
					else
					{//角度に応じて場所の移動
						g_model_Info[nCount].set.pos =
							D3DXVECTOR3(g_model_Info[nCntModel].set.pos.x, g_model_Info[nCount].set.pos.y, g_model_Info[nCntModel].set.pos.z)
							+ D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -g_model_Info[nCntModel].set.fSlength	//目的の範囲分
							+ D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -g_model_Info[nCount].set.fSlength;	//自分の範囲分
					}
				}
			}
		}
	}
}
//=============================================================================
// 悲しみのエラー対処用
//=============================================================================
void SadnessError(int nCount)
{
	if (g_model_Info[nCount].set.pos.y < -1000)
	{
		g_model_Info[nCount].set.pos = g_model_Info[nCount].set.Initpos;
		g_model_Info[nCount].set.posold = g_model_Info[nCount].set.Initpos;
		g_model_Info[nCount].set.bJump = false;
	}
}
//=============================================================================
// モーションの更新
//=============================================================================
void UpdateMotion(int nCount)
{
	int nCntModel;
	D3DXVECTOR3 Move;
	float fFastPlus = 0.0;
	float fCancelPlus = 0.0f;
	float fEndPlus = 0.0f;

	if (g_model_Info[nCount].set.nNumMotion[0] >= 0)
	{//MOTIONを所有
		if (g_model_Info[nCount].set.nCntState > 0)
		{//ステート管理
			g_model_Info[nCount].set.nCntState--;
			if (g_model_Info[nCount].set.nCntState == 0 && g_model_Info[nCount].set.state == MODEL_STATE_DAMAGE && g_model_Info[nCount].set.bJump == true && g_model_Info[nCount].set.nNumMotion[1] == MOTION_SHRINK)
			{
				g_model_Info[nCount].set.state = MODEL_STATE_NONE; g_model_Info[nCount].set.nNumMotion[1] = 0; g_model_Info[nCount].set.nNumMotion[3] = -1;
			}
		}
		if (g_model_Info[nCount].set.state == MODEL_STATE_NONE)
		{//スキルの使用確認
			g_model_Info[nCount].data.nSkill[0][4] = -1;
			g_model_Info[nCount].data.nSkill[1][4] = -1;
		}
		if (g_model_Info[nCount].set.nAirTime > 0) { g_model_Info[nCount].set.nAirTime--; }

		if (g_model_Info[nCount].use >= MODEL_USE_ENEMY_Z)
		{
			g_model_Info[nCount].set.nPlayTime--;
			if (g_model_Info[nCount].set.nPlayTime == 0) { UpdateEnemy(&g_model_Info[nCount].set, &g_model_Info[nCount].data, g_model_Info[nCount].use, 1); }
		}//敵の思考

		if (g_model_Info[nCount].set.nNumMotion[2] == -3)
		{//スキル弱分岐
			if (g_model_Info[nCount].set.nNumMotion[1] != MOTION_SHRINK && g_model_Info[nCount].set.nNumMotion[1] != MOTION_DOWN)
			{//怯み, ダウンでないなら
				if (g_model_Info[nCount].set.nSkillTime[0] < 0 && g_model_Info[nCount].data.nSkillPoint >= MotionSkill(g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].data.nSkill[0][0]].key[0].nAtkEffect))
				{//時間経過済みなら
					g_model_Info[nCount].set.nNumMotion[1] = g_model_Info[nCount].data.nSkill[0][0]; g_model_Info[nCount].set.nNumMotion[3] = -11;
					g_model_Info[nCount].data.nSkill[0][4] = 1;
					if (g_model_Info[nCount].set.state != MODEL_STATE_ATKHIT) { g_model_Info[nCount].set.state = MODEL_STATE_ATK; }	//一度ヒットしていれば変えない
					g_model_Info[nCount].set.rot.y = ReturnAngle(&g_model_Info[nCount].set);//ロックオンしていれば角度を合わせる
					g_model_Info[nCount].set.angle.y = g_model_Info[nCount].set.rot.y;
				}
				else { g_model_Info[nCount].set.nNumMotion[2] = 0; }//失敗音
			}
			else { g_model_Info[nCount].set.nNumMotion[2] = 0; }
		}

		if (g_model_Info[nCount].set.nNumMotion[2] == -4)
		{//スキル強分岐
			if (g_model_Info[nCount].set.nNumMotion[1] != MOTION_SHRINK && g_model_Info[nCount].set.nNumMotion[1] != MOTION_DOWN)
			{//怯み, ダウンでないなら
				if (g_model_Info[nCount].set.nSkillTime[1] < 0 && g_model_Info[nCount].data.nSkillPoint >= MotionSkill(g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].data.nSkill[1][0]].key[0].nAtkEffect))
				{//時間経過済みなら
					g_model_Info[nCount].set.nNumMotion[1] = g_model_Info[nCount].data.nSkill[1][0]; g_model_Info[nCount].set.nNumMotion[3] = -12;
					g_model_Info[nCount].data.nSkill[1][4] = 1;
					if (g_model_Info[nCount].set.state != MODEL_STATE_ATKHIT) { g_model_Info[nCount].set.state = MODEL_STATE_ATK; }	//一度ヒットしていれば変えない
					g_model_Info[nCount].set.rot.y = ReturnAngle(&g_model_Info[nCount].set);//ロックオンしていれば角度を合わせる
					g_model_Info[nCount].set.angle.y = g_model_Info[nCount].set.rot.y;
				}
				else { g_model_Info[nCount].set.nNumMotion[2] = 0; }//失敗音
			}
			else { g_model_Info[nCount].set.nNumMotion[2] = 0; }
		}

		if (g_model_Info[nCount].set.nMotionFrame > g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nCancel[0] &&
			g_model_Info[nCount].set.nMotionFrame <= g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nCancel[1] &&
			g_model_Info[nCount].set.nNumMotion[3] >= 0 && g_model_Info[nCount].set.state != MODEL_STATE_POISED)
		{//可能フレーム && キャンセル予約無し

		 //	敵の行動
			if (g_model_Info[nCount].use >= MODEL_USE_ENEMY_Z) { UpdateEnemy(&g_model_Info[nCount].set, &g_model_Info[nCount].data, g_model_Info[nCount].use, 0); }//敵の思考

			if (g_model_Info[nCount].set.nNumMotion[2] != 0 && g_model_Info[nCount].set.nNumMotion[2] != -99 && g_model_Info[nCount].set.nNumMotion[2] != g_model_Info[nCount].set.nNumMotion[1])
			{//登録モーションがニュートラルでない && 登録モーションが現在と同じでない
				if (g_model_Info[nCount].set.nNumMotion[2] == -1)
				{//弱攻撃分岐
					if (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nBranch[1] != -1)
					{//キャンセル枠があれば 
						if (g_model_Info[nCount].set.state == MODEL_STATE_GUARD)
						{//ガード成功時空中と地上で分ける
							if(g_model_Info[nCount].set.bJump == true){ g_model_Info[nCount].set.nNumMotion[1] = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][0].nBranch[1]; g_model_Info[nCount].set.nNumMotion[3] = -1; }
							else					{ g_model_Info[nCount].set.nNumMotion[1] = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][MOTION_AIR].nBranch[1]; g_model_Info[nCount].set.nNumMotion[3] = -1; }
						}
						else { g_model_Info[nCount].set.nNumMotion[1] = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nBranch[1]; g_model_Info[nCount].set.nNumMotion[3] = -1; }
							if (g_model_Info[nCount].set.state != MODEL_STATE_ATKHIT) { g_model_Info[nCount].set.state = MODEL_STATE_ATK; }	//一度ヒットしていれば変えない
							g_model_Info[nCount].set.rot.y = ReturnAngle(&g_model_Info[nCount].set);//ロックオンしていれば角度を合わせる
							g_model_Info[nCount].set.angle.y = g_model_Info[nCount].set.rot.y;
					}
				}
				else if (g_model_Info[nCount].set.nNumMotion[2] == -2)
				{
					if (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nBranch[2] != -1)
					{//キャンセル枠があれば 
						if (g_model_Info[nCount].set.state == MODEL_STATE_GUARD)
						{//ガード成功時空中と地上で分ける
							if (g_model_Info[nCount].set.bJump == true) { g_model_Info[nCount].set.nNumMotion[1] = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][0].nBranch[2]; g_model_Info[nCount].set.nNumMotion[3] = -1; }
							else { g_model_Info[nCount].set.nNumMotion[1] = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][MOTION_AIR].nBranch[2]; g_model_Info[nCount].set.nNumMotion[3] = -1; }
						}
						else{ g_model_Info[nCount].set.nNumMotion[1] = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nBranch[2]; g_model_Info[nCount].set.nNumMotion[3] = -1; }
						if (g_model_Info[nCount].set.state != MODEL_STATE_ATKHIT) { g_model_Info[nCount].set.state = MODEL_STATE_ATK; }	//一度ヒットしていれば変えない
						g_model_Info[nCount].set.rot.y = ReturnAngle(&g_model_Info[nCount].set);//ロックオンしていれば角度を合わせる
						g_model_Info[nCount].set.angle.y = g_model_Info[nCount].set.rot.y;
					}
				}//狂攻撃分岐
				else
				{//スキル, ガード, ステップ分岐
					if (g_model_Info[nCount].set.nNumMotion[2] == MOTION_LANDING)
					{//ジャンプ分岐
						if (g_model_Info[nCount].set.state != MODEL_STATE_ATK && (g_model_Info[nCount].set.state != MODEL_STATE_ATKHIT || g_model_Info[nCount].set.nNumMotion[1] == MOTION_STEP))
						{//攻撃中はダメ　ヒット時ステップ中ならおけ
							if (g_model_Info[nCount].set.bJump == true)
							{
								g_model_Info[nCount].set.nNumMotion[1] = MOTION_LANDING; g_model_Info[nCount].set.nNumMotion[3] = -1; g_model_Info[nCount].data.nStamina -= 30;
								if (g_model_Info[nCount].data.nStamina < -1) { g_model_Info[nCount].data.nStamina = -20; }
							}
						}
					}
					else if (g_model_Info[nCount].set.nNumMotion[2] == MOTION_STEP)
					{//ステップ分岐
						if (g_model_Info[nCount].set.state != MODEL_STATE_ATK && g_model_Info[nCount].data.nStamina > -1)
						{//攻撃中はダメ　ヒット時はよろしい
							if (g_model_Info[nCount].set.bJump == false && g_model_Info[nCount].set.bStep == true)
							{
								g_model_Info[nCount].set.nNumMotion[1] = MOTION_STEPS; g_model_Info[nCount].set.nNumMotion[3] = -1; g_model_Info[nCount].set.bStep = false; g_model_Info[nCount].set.nLeg = 2;
								g_model_Info[nCount].data.nStamina -= 30; if (g_model_Info[nCount].data.nStamina < -1) { g_model_Info[nCount].data.nStamina = -30; }
							}
							else if (g_model_Info[nCount].set.bJump == true)
							{
								g_model_Info[nCount].set.nNumMotion[1] = MOTION_STEP; g_model_Info[nCount].set.nNumMotion[3] = -1; g_model_Info[nCount].set.nLeg = 2;
								g_model_Info[nCount].data.nStamina -= 30; if (g_model_Info[nCount].data.nStamina < -1) { g_model_Info[nCount].data.nStamina = -30; }
							}

						}
					}
					else if (g_model_Info[nCount].set.nNumMotion[2] == MOTION_GUARD)
					{//ガード分岐
						if (g_model_Info[nCount].set.state != MODEL_STATE_ATK)
						{//攻撃中はダメ　ヒット時はよろしい
							g_model_Info[nCount].set.nNumMotion[1] = MOTION_GUARD; g_model_Info[nCount].set.nNumMotion[3] = -1;
							g_model_Info[nCount].set.state = MODEL_STATE_POISED; g_model_Info[nCount].set.nCntState = 35;
							PlaySound(SOUND_LABEL_GUARD000);
						}
					}//それ以外
					else
					{
						g_model_Info[nCount].set.nNumMotion[1] = g_model_Info[nCount].set.nNumMotion[2]; g_model_Info[nCount].set.nNumMotion[3] = -1;
					}
				}
			}
			else if (g_model_Info[nCount].set.nNumMotion[1] <= MOTION_LAND)
			{//ニュートラル, 歩き, 走り, 着地なら moveで入れ替え

				if (g_model_Info[nCount].set.move.x * g_model_Info[nCount].set.move.x + g_model_Info[nCount].set.move.z * g_model_Info[nCount].set.move.z >= 0.3f)
				{
					if ((g_model_Info[nCount].set.nNumMotion[4] == 0) || g_model_Info[nCount].set.nNumMotion[1] == 0)
					{
						if ((g_model_Info[nCount].set.bRun == true && g_model_Info[nCount].set.nNumMotion[1] != 2) || g_model_Info[nCount].set.nNumMotion[2] == -99) { g_model_Info[nCount].set.nLeg = g_model_Info[nCount].set.nNumMotion[3]; g_model_Info[nCount].set.nNumMotion[1] = 2; g_model_Info[nCount].set.nNumMotion[3] = -1; }//走る
						else if (g_model_Info[nCount].set.bRun == false && g_model_Info[nCount].set.nNumMotion[1] != 1) { g_model_Info[nCount].set.nLeg = g_model_Info[nCount].set.nNumMotion[3]; g_model_Info[nCount].set.nNumMotion[1] = 1; g_model_Info[nCount].set.nNumMotion[3] = -1; }//歩く
					}
				}
				else if (g_model_Info[nCount].set.move.x * g_model_Info[nCount].set.move.x + g_model_Info[nCount].set.move.z * g_model_Info[nCount].set.move.z < 0.3f && g_model_Info[nCount].set.nNumMotion[1] != MOTION_LAND && g_model_Info[nCount].set.nNumMotion[1] != 0)
				{//ニュートラル && 着地モーションじゃない
					if (g_model_Info[nCount].set.nNumMotion[1] == 1 || g_model_Info[nCount].set.nNumMotion[1] == 2)
					{//歩きか走りなら
						if (g_model_Info[nCount].set.nNumMotion[4] < 2)
						{
							g_model_Info[nCount].set.nLeg = g_model_Info[nCount].set.nNumMotion[3];
							g_model_Info[nCount].set.nNumMotion[1] = 0; g_model_Info[nCount].set.nNumMotion[3] = -1;
						}
					}
					//else { set[0].nNumMotion[1] = 0; set[0].nNumMotion[3] = -1; }
				}//着地でないなら止まる
			}
		}

		if (g_model_Info[nCount].set.nNumMotion[3] < 0)
		{//キャンセルされたなら
			g_model_Info[nCount].set.nNumMotion[2] = 0;	//その次をニュートラルに
			g_model_Info[nCount].set.nNumMotion[4] = -1;
			if (g_model_Info[nCount].set.nNumMotion[1] != 0) { fCancelPlus = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[5]].fCancelFreame; }
			g_model_Info[nCount].set.nNumMotion[4] -= (int)fCancelPlus;	//フレームの初期化 +なら遅く-なら早くなる
			g_model_Info[nCount].set.nMotionFrame = 0;	//全フレームの初期化
			if (g_model_Info[nCount].set.nNumMotion[3] < -10)
			{//スキルなら
				g_model_Info[nCount].set.nNumMotion[4] = -1;
				PlaySound(SOUND_LABEL_SKILL);
				SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 250, 250, 10, 1, 5, 1, 1, ANIMATION_NORMAL);
				if (g_model_Info[nCount].set.nNumMotion[3] == -11)
				{
					g_model_Info[nCount].data.nSkillPoint -= MotionSkill(g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].data.nSkill[0][0]].key[0].nAtkEffect);
					g_model_Info[nCount].set.nSkillTime[0] = 200;
				}
				else
				{
					g_model_Info[nCount].data.nSkillPoint -= MotionSkill(g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].data.nSkill[1][0]].key[0].nAtkEffect);
					g_model_Info[nCount].set.nSkillTime[1] = 200;
				}

				if (g_model_Info[nCount].set.state == MODEL_STATE_ATKHIT) { g_model_Info[nCount].set.nNumMotion[3] = 1; g_model_Info[nCount].set.nNumMotion[4] = 9999;}
				g_model_Info[nCount].set.nNumMotion[3] = 0;	//キーの初期化	
			}
			else { g_model_Info[nCount].set.nNumMotion[3] = 0; }	//キーの初期化	

			if (g_model_Info[nCount].set.nNumMotion[1] == 1 || g_model_Info[nCount].set.nNumMotion[1] == 2) { g_model_Info[nCount].set.nNumMotion[3] = g_model_Info[nCount].set.nLeg; } //歩きと走りの場合
			if (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].fFastFrame < 0) { g_model_Info[nCount].set.nNumMotion[4] -= (int)g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].fFastFrame; }
			g_model_Info[nCount].set.addPosmove = (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].pos - g_model_Info[nCount].set.addpos)
				/ (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].fFrame
					+ g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].fFastFrame
					+ fCancelPlus);
			for (nCntModel = 0; nCntModel < g_model_Info[nCount].set.nMaxModel; nCntModel++)
			{//moveを計算して代入
				g_model_Info[nCount].set.parent[nCntModel].addRotmove = (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].rot[nCntModel] - g_model_Info[nCount].set.parent[nCntModel].addrot);

				//角度確認
				if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.x > D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.x -= D3DX_PI * 2; }
				else if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.x < -D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.x += D3DX_PI * 2; }
				if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.y > D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.y -= D3DX_PI * 2; }
				else if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.y < -D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.y += D3DX_PI * 2; }
				if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.z > D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.z -= D3DX_PI * 2; }
				else if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.z < -D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.z += D3DX_PI * 2; }

				g_model_Info[nCount].set.parent[nCntModel].addRotmove /= (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].fFrame
					+ g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].fFastFrame
					+ fCancelPlus);
			}
			MotionEffect_S(-1, nCount);
			g_model_Info[nCount].set.nNumMotion[5] = g_model_Info[nCount].set.nNumMotion[1]; //今のモーションに現在のモーションを入れる
			if (g_model_Info[nCount].set.nNumMotion[1] < 3) { g_model_Info[nCount].set.state = MODEL_STATE_NONE; }	//ニュートラルなら
			if (g_model_Info[nCount].set.nNumMotion[1] == MOTION_GETUP) { g_model_Info[nCount].set.nAirTime = 50; }	//起き上がりなら
			if (g_model_Info[nCount].set.nNumMotion[1] == MOTION_STEP || g_model_Info[nCount].set.nNumMotion[1] == MOTION_STEPS)
			{//ステップなら　角度合わせ 差によってモーション切り替え？
				g_model_Info[nCount].set.rot = g_model_Info[nCount].set.angle;
			}

		}

		g_model_Info[nCount].set.nNumMotion[4] += 1;	//フレームの進行

		while (1)
		{
			if (g_model_Info[nCount].set.nNumMotion[4] < g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].fFrame)
			{//フレームが範囲内なら
				if (g_model_Info[nCount].set.nNumMotion[4] > 0) { g_model_Info[nCount].set.nMotionFrame++; }//モーション全体フレームの進行
				g_model_Info[nCount].set.move.x -= g_model_Info[nCount].set.move.x * (0.3f * g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].fSlip);
				g_model_Info[nCount].set.move.z -= g_model_Info[nCount].set.move.z * (0.3f * g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].fSlip);
				MotionEffect_S(g_model_Info[nCount].set.nMotionFrame, nCount);

				//移動値加算有効フレームなら
				if (g_model_Info[nCount].set.nNumMotion[4] < g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].nMuse)
				{
					float fRot = g_model_Info[nCount].set.rot.y;
					if (g_model_Info[nCount].set.nNumMotion[1] != MOTION_STEP && g_model_Info[nCount].set.nNumMotion[1] != MOTION_STEPS)
					{
						fRot = ReturnAngle(&g_model_Info[nCount].set);
					}//ロックオンしていれば角度を合わせる
					Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					Move += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * -g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].move.z;
					Move += D3DXVECTOR3(sinf(fRot + D3DX_PI * 0.5f), 0.0f, cosf(fRot + D3DX_PI * 0.5f)) * -g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].move.x;
					Move.y += g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].move.y;
					if (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].move.y > 0.0f)
					{
						g_model_Info[nCount].set.bJump = false;
					}
					if (g_model_Info[nCount].set.nNumMotion[1] == MOTION_STEP || g_model_Info[nCount].set.nNumMotion[1] == MOTION_STEPS)
					{
						Move.x *= (1.0f + g_model_Info[nCount].data.status.fSpeed * 0.00003f);
						Move.z *= (1.0f + g_model_Info[nCount].data.status.fSpeed * 0.00003f);
					}
					if (g_model_Info[nCount].data.nStamina < 0) { Move *= 0.5f; }
					g_model_Info[nCount].set.move += Move;
				}

				g_model_Info[nCount].set.addpos += g_model_Info[nCount].set.addPosmove;		//本体の移動
				for (nCntModel = 0; nCntModel < g_model_Info[nCount].set.nMaxModel; nCntModel++)
				{//保存されたmove分動かす
				 //rotの移動値加算
					g_model_Info[nCount].set.parent[nCntModel].addrot += g_model_Info[nCount].set.parent[nCntModel].addRotmove;
					//ロット確認
					if (g_model_Info[nCount].set.parent[nCntModel].addrot.x > D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addrot.x -= D3DX_PI * 2; }
					else if (g_model_Info[nCount].set.parent[nCntModel].addrot.x < -D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addrot.x += D3DX_PI * 2; }
					if (g_model_Info[nCount].set.parent[nCntModel].addrot.y > D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addrot.y -= D3DX_PI * 2; }
					else if (g_model_Info[nCount].set.parent[nCntModel].addrot.y < -D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addrot.y += D3DX_PI * 2; }
					if (g_model_Info[nCount].set.parent[nCntModel].addrot.z > D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addrot.z -= D3DX_PI * 2; }
					else if (g_model_Info[nCount].set.parent[nCntModel].addrot.z < -D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addrot.z += D3DX_PI * 2; }
				}
				break;
			}
			else
			{//キーの終了
				g_model_Info[nCount].set.nNumMotion[4] = 0;	//フレームの初期化
				g_model_Info[nCount].set.nNumMotion[3]++;		//キーの進撃

				if (g_model_Info[nCount].set.nNumMotion[3] >= g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nKey)
				{//そのモーションが終了
					g_model_Info[nCount].set.angle = g_model_Info[nCount].set.rot;
					g_model_Info[nCount].set.nMotionFrame = 0;						//全フレームの初期化
					if (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nBranch[0] > -1)
					{
						g_model_Info[nCount].set.nNumMotion[3] = 0;						//キーの初期化
						if (g_model_Info[nCount].set.nNumMotion[1] != g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nBranch[0])
						{//ループじゃなければ
							fEndPlus = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].fEndFreame;
							g_model_Info[nCount].set.nNumMotion[4] = (int)fEndPlus;
							g_model_Info[nCount].set.nNumMotion[1] = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nBranch[0];	//次のモーションの付与
							if (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].fFastFrame < 0) { g_model_Info[nCount].set.nNumMotion[4] -= (int)g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].fFastFrame; }
							fFastPlus = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].fFastFrame;
							if (g_model_Info[nCount].set.nNumMotion[1] == 0) { g_model_Info[nCount].set.state = MODEL_STATE_NONE; }	//ニュートラルなら
							if (g_model_Info[nCount].set.nNumMotion[1] == MOTION_GETUP) { g_model_Info[nCount].set.nAirTime = 90; }	//起き上がりなら
						}
						else
						{
							g_model_Info[nCount].set.nNumMotion[1] = g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].nBranch[0];
							if (g_model_Info[nCount].set.nNumMotion[1] == 0 || g_model_Info[nCount].set.nNumMotion[1] == MOTION_AIR)
							{ 
								g_model_Info[nCount].set.state = MODEL_STATE_NONE;
							}	//ニュートラルなら
						}	//次のモーションの付与}
						g_model_Info[nCount].set.nNumMotion[5] = g_model_Info[nCount].set.nNumMotion[1]; //今のモーションに現在のモーションを入れる
					}
					else
					{//次のキー設定がされていないならそのまま放置
						g_model_Info[nCount].set.nNumMotion[3]--;						//キーの初期化
						g_model_Info[nCount].set.nNumMotion[4] = 0;
					}
				}
				MotionEffect_S(-1, nCount);

				g_model_Info[nCount].set.addPosmove = (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].pos - g_model_Info[nCount].set.addpos)
					/ (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].fFrame
						+ fFastPlus
						- fEndPlus);
				for (nCntModel = 0; nCntModel < g_model_Info[nCount].set.nMaxModel; nCntModel++)
				{//moveを計算して代入
					g_model_Info[nCount].set.parent[nCntModel].addRotmove = (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].rot[nCntModel] - g_model_Info[nCount].set.parent[nCntModel].addrot);

					//角度確認
					if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.x > D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.x -= D3DX_PI * 2; }
					else if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.x < -D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.x += D3DX_PI * 2; }
					if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.y > D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.y -= D3DX_PI * 2; }
					else if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.y < -D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.y += D3DX_PI * 2; }
					if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.z > D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.z -= D3DX_PI * 2; }
					else if (g_model_Info[nCount].set.parent[nCntModel].addRotmove.z < -D3DX_PI) { g_model_Info[nCount].set.parent[nCntModel].addRotmove.z += D3DX_PI * 2; }

					g_model_Info[nCount].set.parent[nCntModel].addRotmove /= g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].fFrame
						+ fFastPlus - fEndPlus;
				}
			}//キー終了
		}//更新while
	}//モーション使用確認
}
//=============================================================================
// モーションのReset
//=============================================================================
void ResetMotion(MODEL_SETTING *set)
{
	set[0].nNumMotion[1] = 0;
	set[0].nNumMotion[2] = 0;
	set[0].nNumMotion[3] = -1;
	set[0].nNumMotion[4] = 0;
	set[0].nNumMotion[5] = 0;

	set[0].addpos = g_model_motion[set[0].nNumMotion[0]][0].key[1].pos;
	for (int nCntModel = 0; nCntModel < set[0].nMaxModel; nCntModel++)
	{//moveを計算して代入
		set[0].parent[nCntModel].addrot = g_model_motion[set[0].nNumMotion[0]][0].key[1].rot[nCntModel];
	}
}
//=============================================================================
// モーションの効果 攻撃面
//=============================================================================
int MotionSkill(int nCntMotion)
{
	switch (nCntMotion)
	{
	case -2:
		return 920;
	case -3:
		return 680;
	case -4:
		return 800;
	case -5:
		return 2850;
	case -6:
		return 2550;
	case -7:
		return 2150;

	case -24:
		return 3000;
	case -25:
		return 12500;
	case -26:
		return 3000;
	case -27:
		return 6000;
	case -28:
		return 10000;
	case -29:
		return 12000;
	case -31://メテオ
		return 13000;
	case -32://放射
		return 8000;
	case -33://竜巻
		return 6000;
	case -34://爆発
		return 7000;
	case -35:
		return 0;
	case -36:
		return 8000;
	case -37:
		return 11000;
	case -38:
		return 12000;
	}
	return 0;
}

//=============================================================================
// モーションの効果 攻撃面
//=============================================================================
void MotionEffect_A(int nCount, int nEnemy, float fAtk, float *fHeel, float *fPredator)
{
	Camera *pCamera = GetCamera();

	switch (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].nAtkEffect)
	{//モーションごとの効果
	case 0://
		SetAnimation(g_model_Info[nEnemy].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f), 300, 300, 5, 2, 5, 4, 1, ANIMATION_NORMAL);
		break;
	case 1://
		SetAnimation(g_model_Info[nEnemy].set.pos + D3DXVECTOR3(0.0f, -50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 200, 200, 2, 10, 3, 5, 1, ANIMATION_NORMAL);
		PlaySound(SOUND_LABEL_ATTACK002);
		break;
	case 9:	//捕食
		fPredator[0] = 1000.0f;
		break;
	case 24:	//捕食
		fPredator[0] = 100000.0f;
		break;

	case 200:	//攻撃型
			SetZWindow(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(sinf(pCamera[0].rot.y), 0.0f, cosf(pCamera[0].rot.y)) * 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 250, 250, 4, 2, WINDOWSTATE_FADE_Z, 0.02f);
		break;
	}
}

//=============================================================================
// モーションの効果 サウンド, 見た目
//=============================================================================
void MotionEffect_S(int nMode,int nCount)
{
	Camera *pCamera = GetCamera();
	float fState;

	switch (g_model_motion[g_model_Info[nCount].set.nNumMotion[0]][g_model_Info[nCount].set.nNumMotion[1]].key[g_model_Info[nCount].set.nNumMotion[3]].nShowEffect)
	{//モーションごとの効果
	case 0:	//歩き
			//PlaySound(SOUND_LABEL_ATTACK000);
		break;
	case 1:	//走り
			//PlaySound(SOUND_LABEL_ATTACK000);
		break;
	case 2:	//攻撃効果音
		if (nMode == -1) { PlaySound(SOUND_LABEL_ATTACK000); }

		break;
	case 3:	//ジャンプ
		if (nMode == -1) { PlaySound(SOUND_LABEL_DASH000); }
		SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.93f, 0.1f), 200, 200, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
		SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f), 50, 50, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
		break;
	case 4:	//ブースト
		if (nMode == -1) { PlaySound(SOUND_LABEL_DASH000); }
		SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 0.98f, 0.0f, 0.1f), 200, 200, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
		SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f), 50, 50, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
		break;
	case 5:	//風切り効果音
		if (nMode == -1) { PlaySound(SOUND_LABEL_TitleSE000); }
		break;
	case 6:	//爆発
		if (nMode == -1)
		{
			PlaySound(SOUND_LABEL_ATTACK001);
			SetAnimation(D3DXVECTOR3(g_model_Info[nCount].set.parent[2].mtxWorldModel._41, g_model_Info[nCount].set.parent[2].mtxWorldModel._42, g_model_Info[nCount].set.parent[2].mtxWorldModel._43), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 250, 250, 8, 1, 2, 6, 1, ANIMATION_UP);
		}//SetAnimation(D3DXVECTOR3(set[0].parent[2].mtxWorldModel._41, set[0].parent[2].mtxWorldModel._42, set[0].parent[2].mtxWorldModel._43), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 100, 100, 8, 1, 2, 6, ANIMATION_UP);
		break;
	case 7:	//ジャンプ
		SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.93f, 0.1f), 200, 200, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
		SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f), 50, 50, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
		break;
	case 8:	//ブースト
		if (nMode % 2 == 0)
		{
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 0.98f, 0.0f, 0.1f), 200, 200, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f), 50, 50, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
		}
		break;
	case 9:	//捕食
		if (nMode == -1)
		{
			PlaySound(SOUND_LABEL_SKILL);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.02f, 1.0f), 150, 150, 10, 1, 3, 1, 1, ANIMATION_NORMAL);
		}
		break;
	case 10:	//追加効果
		if (nMode == -1)
		{
			if (g_model_Info[nCount].data.nSkill[0][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[0][0], nCount); }
			if (g_model_Info[nCount].data.nSkill[1][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[1][0], nCount); }
		}
		break;
	case 30:	//ジャンプ
		if (nMode == -1)
		{
			PlaySound(SOUND_LABEL_ATTACK400);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, 8, 1, 6, 6, 1, ANIMATION_NORMAL);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130, 130, 8, 1, 1, 6, 1, ANIMATION_UP);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 60.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, 8, 1, 6, 9, 10, ANIMATION_NORMAL);
		}
		break;
	case 40://メテオ？
		{
			if (nMode == -1)
			{
				if (g_model_Info[nCount].data.nSkill[0][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[0][0], nCount); }
				if (g_model_Info[nCount].data.nSkill[1][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[1][0], nCount); }

				SetBullet(g_model_Info[nCount].set.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(0.0f, 12.5f, 30.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 9.15f, 90.0f, 1.0f, 90, 0, 99999, 40, 40, -1, nCount);
			}
		}
		break;
	case 42:	//放射
		if (nMode == -1)
		{
			if (g_model_Info[nCount].data.nSkill[0][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[0][0], nCount); }
			if (g_model_Info[nCount].data.nSkill[1][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[1][0], nCount); }

			fState = g_model_Info[nCount].set.rot.y - D3DX_PI * 0.3f;
			for (int nCntBullet = 0; nCntBullet < 3; nCntBullet++)
			{
				fState += D3DX_PI * 0.15f;
				SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(fState) * -100.0f, 50.0f, (cosf(fState) * -100.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI * -0.5f, fState, 0.0f),
					D3DXVECTOR3(0.0f, 4.5f, 50.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 3.55f, 80.0f, 1.0f, 60, 6, 99999, 42, 42, -1, nCount);
			}
		}
		break;
	case 43:	//竜巻
		if (nMode == -1)
		{
			if (g_model_Info[nCount].data.nSkill[0][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[0][0], nCount); }
			if (g_model_Info[nCount].data.nSkill[1][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[1][0], nCount); }

			SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * 50.0f, 50.0f, cosf(g_model_Info[nCount].set.rot.y) * 50.0f), D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -8.0f, 0.0f, cosf(g_model_Info[nCount].set.rot.y) * -8.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 8.8f, 25.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 6.45f, 150.0f, 0.0f, 60, 4, 99999, 43, 43, -1, nCount);
		}
		break;
	case 44:	//爆発
		if (nMode == -1)
		{
			if (g_model_Info[nCount].data.nSkill[0][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[0][0], nCount); }
			if (g_model_Info[nCount].data.nSkill[1][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[1][0], nCount); }

			PlaySound(SOUND_LABEL_ATTACK400);
			fState = g_model_Info[nCount].set.rot.y;
			for (int nCntBullet = 0; nCntBullet < 4; nCntBullet++)
			{
				fState += D3DX_PI * 0.5f;
				SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(fState) * -100.0f, 50.0f, (cosf(fState) * -100.0f)), D3DXCOLOR(1.0f, 0.12f, 0.05f, 1.0f), 100, 100, 5, 5, 2, 8, 1, ANIMATION_NORMAL);
				SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(fState) * -100.0f, 50.0f, (cosf(fState) * -100.0f)), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100, 100, 8, 1, 4, 6, 1, ANIMATION_NORMAL);
				SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(fState) * -100.0f, 50.0f, (cosf(fState) * -100.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(0.0f, 7.5f, 80.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 9.15f, 80.0f, 1.0f, 31, 1, 99999, 400, 400, -1, nCount);
			}
		}
		break;
	case 45:	//自爆
		if (nMode == -1)
		{//自分のHP1に, NORMAL型へ 周囲の敵のHP1
			if (g_model_Info[nCount].data.nSkill[0][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[0][0], nCount); }
			if (g_model_Info[nCount].data.nSkill[1][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[1][0], nCount); }

			SetBullet(g_model_Info[nCount].set.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 16.0f, 0.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 0.0f, 9999.9f, 1.0f, 2, 1, 99999, 45, 45, -1, nCount);

			SetFadeWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 0, WINDOWSTATE_FADEOUT, WINDOWUSE_NORMAL);

			g_model_Info[nCount].data.nLifePoint = 1;
			g_model_Info[nCount].data.nSkillPoint = 0;

			//NORMAL型
			Set_P_NOR(&g_model_Info[nCount].set);
			//モーションのReset
			ResetMotion(&g_model_Info[nCount].set);
			CollisionObject_Plus(&g_model_Info[nCount].set);

			char aStr[48];
			int nLength;
			MODEL_MOTION *pMotion = GetModel_Motion();

			wsprintf(&aStr[0], "%d SP\n", MotionSkill(pMotion[g_model_Info[0].set.nNumMotion[0] * MAX_MODEL_MOTION + g_model_Info[0].data.nSkill[0][0]].key[0].nAtkEffect));
			ReturnSkillName(&aStr[0], g_model_Info[0].data.nSkill[0][0]);
			ChangeFont(true, &aStr[0], 5, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			wsprintf(&aStr[0], "");
			ReturnSkillName(&aStr[0], g_model_Info[0].data.nSkill[1][0]);
			nLength = (int)strlen(&aStr[0]);
			wsprintf(&aStr[nLength], "\n%d SP", MotionSkill(pMotion[g_model_Info[0].set.nNumMotion[0] * MAX_MODEL_MOTION + g_model_Info[0].data.nSkill[1][0]].key[0].nAtkEffect));
			ChangeFont(true, &aStr[0], 6, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		break;
	case 46:	//変身
		if (nMode == -1)
		{
			SetFadeWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 0, WINDOWSTATE_FADEOUT, WINDOWUSE_NORMAL);
			g_model_Info[nCount].data.nSkill[0][0] = -1;
			g_model_Info[nCount].data.nSkill[1][0] = -1;
			//ステータスで分岐
			int nEvolve = rand() % 8;
			switch (nEvolve)
			{
			case 0:
				Set_P_NOR(&g_model_Info[nCount].set);
				break;
			case 1:
				Set_P_ATK(&g_model_Info[nCount].set);
				break;
			case 2:
				Set_P_INT(&g_model_Info[nCount].set);
				break;
			case 3:
				Set_P_SPD(&g_model_Info[nCount].set);
				break;
			case 4:
				Set_E_KING(&g_model_Info[nCount].set);
				break;
			case 5:
				Set_E_WARRIOR(&g_model_Info[nCount].set);
				break;
			case 6:
				Set_E_NINJA(&g_model_Info[nCount].set);
				break;
			case 7:
				Set_E_WIZARD(&g_model_Info[nCount].set);
				break;
			}

			//モーションのReset
			ResetMotion(&g_model_Info[nCount].set);
			CollisionObject_Plus(&g_model_Info[nCount].set);

			char aStr[48];
			int nLength;
			MODEL_MOTION *pMotion = GetModel_Motion();

			wsprintf(&aStr[0], "%d SP\n", MotionSkill(pMotion[g_model_Info[0].set.nNumMotion[0] * MAX_MODEL_MOTION + g_model_Info[0].data.nSkill[0][0]].key[0].nAtkEffect));
			ReturnSkillName(&aStr[0], g_model_Info[0].data.nSkill[0][0]);
			ChangeFont(true, &aStr[0], 5, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			wsprintf(&aStr[0], "");
			ReturnSkillName(&aStr[0], g_model_Info[0].data.nSkill[1][0]);
			nLength = (int)strlen(&aStr[0]);
			wsprintf(&aStr[nLength], "\n%d SP", MotionSkill(pMotion[g_model_Info[0].set.nNumMotion[0] * MAX_MODEL_MOTION + g_model_Info[0].data.nSkill[1][0]].key[0].nAtkEffect));
			ChangeFont(true, &aStr[0], 6, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		break;

	case 50:	//ヒール
		if (g_model_Info[nCount].data.nSkill[0][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[0][0], nCount); }
		if (g_model_Info[nCount].data.nSkill[1][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[1][0], nCount); }
		g_model_Info[0].data.nLifePoint += 30000;
		if(g_model_Info[0].data.nLifePoint > (int)g_model_Info[0].data.status.fLifePoint)
		{ g_model_Info[0].data.nLifePoint = (int)g_model_Info[0].data.status.fLifePoint; }
		break;
	case 51:	//メガヒール
		if (g_model_Info[nCount].data.nSkill[0][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[0][0], nCount); }
		if (g_model_Info[nCount].data.nSkill[1][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[1][0], nCount); }
		g_model_Info[0].data.nLifePoint += 60000;
		if (g_model_Info[0].data.nLifePoint > (int)g_model_Info[0].data.status.fLifePoint)
		{ g_model_Info[0].data.nLifePoint = (int)g_model_Info[0].data.status.fLifePoint; }
		break;
	case 52:	//ギガヒール
		if (g_model_Info[nCount].data.nSkill[0][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[0][0], nCount); }
		if (g_model_Info[nCount].data.nSkill[1][4] > 0) { SkillEffect_S(&g_model_Info[nCount].data.nSkill[1][0], nCount); }
		g_model_Info[0].data.nLifePoint += 100000;
		if (g_model_Info[0].data.nLifePoint > (int)g_model_Info[0].data.status.fLifePoint)
		{ g_model_Info[0].data.nLifePoint = (int)g_model_Info[0].data.status.fLifePoint; }
		break;

	case 99:	//進化 全画面エフェクト
		if (nMode == -1)
		{
			SetExplosion(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH, SCREEN_HEIGHT / 2, 2, 8, 3, 5, ANIMATION_NORMAL, EXPLOSIONSTATE_EFFECT, D3DX_PI * 0.0f, 1);
				}
		break;
	case 100:	//進化　変更
		if (nMode == -1)
		{
			SetFadeWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 0, WINDOWSTATE_FADEOUT, WINDOWUSE_NORMAL);

			//ステータスで分岐
			int nEvolve = 2;
			if (g_model_Info[nCount].data.status.fAttack > g_model_Info[nCount].data.status.fInt
				&& g_model_Info[nCount].data.status.fAttack > g_model_Info[nCount].data.status.fSpeed * 1.5f) {
				nEvolve = 0;
			}
			else if (g_model_Info[nCount].data.status.fInt > g_model_Info[nCount].data.status.fAttack
				&& g_model_Info[nCount].data.status.fInt > g_model_Info[nCount].data.status.fSpeed * 1.5f) {
				nEvolve = 1;
			}
			else { nEvolve = 2; }


			switch (nEvolve)
			{
			case 0:
				Set_P_ATK(&g_model_Info[nCount].set);
				g_model_Info[nCount].data.gifted.fLifePoint += 1000.0f;
				g_model_Info[nCount].data.gifted.fAttack += 60.0f;
				g_model_Info[nCount].data.gifted.fDefense += 20.0f;
				g_model_Info[nCount].data.gifted.fSpeed += 20.0f;
				break;
			case 1:
				Set_P_INT(&g_model_Info[nCount].set);
				g_model_Info[nCount].data.gifted.fLifePoint += 600.0f;
				g_model_Info[nCount].data.gifted.fInt += 90.0f;
				g_model_Info[nCount].data.gifted.fRes += 20.0f;
				g_model_Info[nCount].data.gifted.fSpeed += 20.0f;
				break;
			case 2:
				Set_P_SPD(&g_model_Info[nCount].set);
				g_model_Info[nCount].data.gifted.fLifePoint += 800.0f;
				g_model_Info[nCount].data.gifted.fAttack += 40.0f;
				g_model_Info[nCount].data.gifted.fInt += 40.0f;
				g_model_Info[nCount].data.gifted.fRes += 40.0f;
				g_model_Info[nCount].data.gifted.fSpeed += 90.0f;
				break;
			}

			//モーションのReset
			ResetMotion(&g_model_Info[nCount].set);
			CollisionObject_Plus(&g_model_Info[nCount].set);

			char aStr[48];
			int nLength;
			MODEL_MOTION *pMotion = GetModel_Motion();

			wsprintf(&aStr[0], "%d SP\n", MotionSkill(pMotion[g_model_Info[0].set.nNumMotion[0] * MAX_MODEL_MOTION + g_model_Info[0].data.nSkill[0][0]].key[0].nAtkEffect));
			ReturnSkillName(&aStr[0], g_model_Info[0].data.nSkill[0][0]);
			ChangeFont(true, &aStr[0], 5, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			wsprintf(&aStr[0], "");
			ReturnSkillName(&aStr[0], g_model_Info[0].data.nSkill[1][0]);
			nLength = (int)strlen(&aStr[0]);
			wsprintf(&aStr[nLength], "\n%d SP", MotionSkill(pMotion[g_model_Info[0].set.nNumMotion[0] * MAX_MODEL_MOTION + g_model_Info[0].data.nSkill[1][0]].key[0].nAtkEffect));
			ChangeFont(true, &aStr[0], 6, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		break;

	case 200:	//攻撃型
		if (nMode == -1)
		{
			PlaySound(SOUND_LABEL_ATTACK001);
			SetAnimation(g_model_Info[nCount].set.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 300, 300, 8, 1, 2, 6, 1, ANIMATION_UP);
		}
		break;

	case 400:
		if (nMode == -1)
		{
			PlaySound(SOUND_LABEL_ATTACK400);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -80.0f, 50.0f, (cosf(g_model_Info[nCount].set.rot.y) * -80.0f)), D3DXCOLOR(1.0f, 0.12f, 0.05f, 1.0f), 150, 150, 5, 5, 2, 8, 1, ANIMATION_NORMAL);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -80.0f, 50.0f, (cosf(g_model_Info[nCount].set.rot.y) * -80.0f)), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, 8, 1, 4, 6, 1, ANIMATION_NORMAL);
			SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -80.0f, 50.0f, (cosf(g_model_Info[nCount].set.rot.y) * -80.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 7.5f, 80.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 9.15f, 120.0f, 1.0f, 30, 1, 99999, 400, 400, -1, nCount);
		}
		break;
	case 401:
		if (nMode == -1)
		{ 
			PlaySound(SOUND_LABEL_ATTACK001);
			SetBullet(g_model_Info[nCount].set.pos, D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 8.8f, 45.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 11.45f, 150.0f, 1.0f, 20, 1, 99999, 401, 401, -1, nCount);
			SetAnimation(g_model_Info[nCount].set.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 300, 300, 8, 1, 2, 6, 1, ANIMATION_UP);
		}
		break;
	case 402:
		if (nMode == -1)
		{
			SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -40.0f, 0.0f, (cosf(g_model_Info[nCount].set.rot.y) * -40.0f)), D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -10.0f, 0.0f, (cosf(g_model_Info[nCount].set.rot.y) * -10.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 10.5f, 10.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 7.35f, 80.0f, 0.0f, 21, 3, 99999, 402, 402, -1, nCount);
		}
		break;
	case 403:
		if (nMode % 10 == 2)
		{
			if (nMode < 30)
			{
				PlaySound(SOUND_LABEL_ATTACK403);
				SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -40.0f, 50.0f, (cosf(g_model_Info[nCount].set.rot.y) * -40.0f)), D3DXCOLOR(0.96f, 1.0f, 0.12f, 0.5f), 60, 60, 5, 2, 1, 4, 1, ANIMATION_NORMAL);
				SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -40.0f, 0.0f, (cosf(g_model_Info[nCount].set.rot.y) * -40.0f)), D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -30.0f - (g_model_Info[nCount].set.move.x * 3.0f), (3 - (nMode / 10)) * -5.0f - 10.0f, (cosf(g_model_Info[nCount].set.rot.y) * -30.0f - (g_model_Info[nCount].set.move.z * 3.0f))), D3DXVECTOR3(1.57f, 0.0f, 0.0f),
					D3DXVECTOR3(0.0f, 2.5f, 3.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 0.95f, 20.0f, 0.0f, 21, 2, 5, 403, 403, -1, nCount);
			}
			else
			{
				PlaySound(SOUND_LABEL_ATTACK404);
				SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -40.0f, 50.0f, (cosf(g_model_Info[nCount].set.rot.y) * -40.0f)), D3DXCOLOR(0.96f, 1.0f, 0.12f, 0.5f), 200, 200, 5, 2, 3, 4, 1, ANIMATION_NORMAL);
				SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -40.0f, 0.0f, (cosf(g_model_Info[nCount].set.rot.y) * -40.0f)), D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -30.0f - (g_model_Info[nCount].set.move.x * 3.0f), (3 - (nMode / 10)) * -5.0f - 10.0f, (cosf(g_model_Info[nCount].set.rot.y) * -30.0f - (g_model_Info[nCount].set.move.z * 3.0f))), D3DXVECTOR3(1.57f, 0.0f, 0.0f),
					D3DXVECTOR3(0.0f, 6.5f, 10.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 1.95f, 40.0f, 0.0f, 21, 2, 5, 404, 404, -1, nCount);
			}
		}
		break;
	case 404:
		if (nMode == -1)
		{
			SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y + D3DX_PI * 0.5f) * -20.0f, 80.0f, (cosf(g_model_Info[nCount].set.rot.y + D3DX_PI * 0.5f) * -20.0f)), D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -30.0f, 0.0f, (cosf(g_model_Info[nCount].set.rot.y) * -30.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 14.5f, 0.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 0.35f, 80.0f, 0.0f, 41, 3, 1, 405, 405, -1, nCount);
			SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y + D3DX_PI * 0.5f) * -40.0f, 60.0f, (cosf(g_model_Info[nCount].set.rot.y + D3DX_PI * 0.5f) * -40.0f)), D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -30.0f, 0.0f, (cosf(g_model_Info[nCount].set.rot.y) * -30.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 14.5f, 0.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 0.35f, 80.0f, 0.0f, 41, 3, 1, 405, 405, -1, nCount);
			SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y - D3DX_PI * 0.5f) * -20.0f, 80.0f, (cosf(g_model_Info[nCount].set.rot.y - D3DX_PI * 0.5f) * -20.0f)), D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -30.0f, 0.0f, (cosf(g_model_Info[nCount].set.rot.y) * -30.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 14.5f, 0.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 0.35f, 80.0f, 0.0f, 41, 3, 1, 405, 405, -1, nCount);
			SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y - D3DX_PI * 0.5f) * -40.0f, 60.0f, (cosf(g_model_Info[nCount].set.rot.y - D3DX_PI * 0.5f) * -40.0f)), D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -30.0f, 0.0f, (cosf(g_model_Info[nCount].set.rot.y) * -30.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 14.5f, 0.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 0.35f, 80.0f, 0.0f, 41, 3, 1, 405, 405, -1, nCount);
		}
	case 405:
		if (nMode == -1)
		{
			//if (set[0].nLockChar > 0)
			//{
			//	SetBullet(g_model_Info[set[0].nLockChar].set.pos, D3DXVECTOR3(0.0f, -50.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
			//		D3DXVECTOR3(0.0f, -50.0f, 0.0f), use, data[0].status.fInt, 25.5f, 150.0f, 0.0f, 10, 4, 999999, 405, 405, 25);
			//}

		}
	case 500:
		if (nMode == -1)
		{//魔法使い　魔法1
			PlaySound(SOUND_LABEL_ATTACK403);
			SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -100.0f, 50.0f, (cosf(g_model_Info[nCount].set.rot.y) * -100.0f)), D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y), 0.0f, cosf(g_model_Info[nCount].set.rot.y)) * -15.0f, D3DXVECTOR3(D3DX_PI * -0.5f, g_model_Info[nCount].set.rot.y, 0.0f),
				D3DXVECTOR3(0.0f, 4.5f, 15.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 0.55f, 40.0f, 1.0f, 90, 1, 99999, 500, 500, -1, nCount);
		}
		break;
	case 501:
		if (nMode == -1)
		{//魔法使い　魔法1			
			PlaySound(SOUND_LABEL_ATTACK400);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -80.0f, 50.0f, (cosf(g_model_Info[nCount].set.rot.y) * -80.0f)), D3DXCOLOR(1.0f, 0.12f, 0.05f, 1.0f), 150, 150, 5, 5, 2, 8, 1, ANIMATION_NORMAL);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -80.0f, 50.0f, (cosf(g_model_Info[nCount].set.rot.y) * -80.0f)), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, 8, 1, 4, 6, 1, ANIMATION_NORMAL);
			SetBullet(g_model_Info[nCount].set.pos + D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * -80.0f, 50.0f, (cosf(g_model_Info[nCount].set.rot.y) * -80.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 7.5f, 80.0f), g_model_Info[nCount].use, g_model_Info[nCount].data.status.fInt, 5.15f, 120.0f, 1.0f, 30, 1, 99999, 400, 400, -1, nCount);

		}
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX			mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9		matDef;					//現在のマテリアル保存用
	D3DXMATERIAL		*pMat;					//マテリアルデータへのポインタ
	D3DXVECTOR3			pos, rot;
	ENEMY *pEnemy = GetEnemy();

	int nCount;
	float fCola = 1.0f;

	for (nCount = 0; nCount < MAX_MODEL_INFO; nCount++)
	{//用意したモデル枠分回る
		if (g_model_Info[nCount].bUse == true && g_model_Info[nCount].use != MODEL_USE_Pause)
		{//使用中なら
			if (g_model_Info[nCount].set.state == MODEL_STATE_DIE)
			{
				fCola = ((0.5f / 60) * g_model_Info[nCount].set.nCntState);
				if (g_model_Info[nCount].set.nCntState == 0)
				{
					if (g_model_Info[nCount].use == MODEL_USE_ENEMY_B)
					{//ボスを倒した
						GAME_PLAY *pGame_Play = GetGame_Play();
						pGame_Play[0].bUse = true;
						pGame_Play[0].nResults = 17;
						SetGameState(GAMESTATE_END, MODE_ENDSTORY, 200);
					}
					else if (g_model_Info[nCount].use == MODEL_USE_PLAYER)
					{//プレイヤーなら終わり
						GAME_PLAY *pGame_Play = GetGame_Play();
						pGame_Play[0].bUse = true;
						pGame_Play[0].nResults = 16;
						SetGameState(GAMESTATE_END, MODE_GAMEOVER, 200);
					}
					g_model_Info[nCount].bUse = false;
					if (g_model_Info[nCount].use >= MODEL_USE_ENEMY_Z)
					{//敵なら
						pEnemy[g_model_Info[nCount].set.nCntChar].bUse = false;
					}
				}
			}
			else if (g_model_Info[nCount].set.nAirTime > 0 && g_model_Info[nCount].set.nAirTime < 11 && g_model_Info[nCount].set.nNumMotion[1] != MOTION_SHRINK) { fCola = 1.0f - (g_model_Info[nCount].set.nAirTime * 0.1f); }
			else if (g_model_Info[nCount].set.nAirTime > 0) { fCola = 0.5f; }
			else { fCola = 1.0f; }
			for (int nCntModel = 0; nCntModel < g_model_Info[nCount].set.nMaxModel; nCntModel++)
			{//パーツ分だけ回転

			 // ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel);

				if (g_model_Info[nCount].set.parent[nCntModel].nParent >= 0)
				{//子供なら加算分代入
					pos = g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos;
					rot = g_model_Info[nCount].set.parent[nCntModel].addrot;
				}
				else
				{//親なしなら原点から
					pos = g_model_Info[nCount].set.pos + g_model_Info[nCount].set.addpos;
					pos += D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos.z
						, g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos.y
						, cosf(g_model_Info[nCount].set.rot.y) * g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos.z);
					//pos += D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos.x
					//	, 0.0f
					//	, cosf(g_model_Info[nCount].set.rot.y) * g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos.x);
					rot = g_model_Info[nCount].set.parent[nCntModel].addrot + g_model_Info[nCount].set.rot;
				}

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
				D3DXMatrixMultiply(&g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &mtxTrans);

				if (g_model_Info[nCount].set.parent[nCntModel].nParent >= 0)
				{//親の位置と角度をプラス
					D3DXMatrixMultiply(&g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &g_model_Info[nCount].set.parent[g_model_Info[nCount].set.parent[nCntModel].nParent].mtxWorldModel);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);
				pMat = (D3DXMATERIAL*)g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].pBuffMatModel->GetBufferPointer();
				//テクスチャの設定
				pDevice->SetTexture(0, pTextureModel[g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].nTexture]);

				// マテリアルデータへのポインタを取得
				for (int nCountData = 0; nCountData < (int)g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].nNumMatModel; nCountData++)
				{
					// マテリアルの設定
					pMat = (D3DXMATERIAL*)g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].pBuffMatModel->GetBufferPointer();
					pMat[nCountData].MatD3D.Diffuse.a = fCola; //透明度設定

					pDevice->SetMaterial(&pMat[nCountData].MatD3D);

					// モデル(パーツ)の描画
					g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].MeshModel->DrawSubset(nCountData);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
				pDevice->SetTexture(0, NULL);

			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel_P(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX			mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9		matDef;					//現在のマテリアル保存用
	D3DXMATERIAL		*pMat;					//マテリアルデータへのポインタ
	D3DXVECTOR3			pos, rot;

	int nCount;

	for (nCount = 0; nCount < MAX_MODEL_INFO; nCount++)
	{//用意したモデル枠分回る
		if (g_model_Info[nCount].bUse == true && g_model_Info[nCount].use == MODEL_USE_Pause)
		{//使用中なら
			for (int nCntModel = 0; nCntModel < g_model_Info[nCount].set.nMaxModel; nCntModel++)
			{//パーツ分だけ回転

			 // ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel);

				if (g_model_Info[nCount].set.parent[nCntModel].nParent >= 0)
				{//子供なら加算分代入
					pos = g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos;
					rot = g_model_Info[nCount].set.parent[nCntModel].addrot;
				}
				else
				{//親なしなら原点から
					pos = g_model_Info[nCount].set.pos + g_model_Info[nCount].set.addpos;
					pos += D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos.z
						, g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos.y
						, cosf(g_model_Info[nCount].set.rot.y) * g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos.z);
					//pos += D3DXVECTOR3(sinf(g_model_Info[nCount].set.rot.y) * g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos.x
					//	, 0.0f
					//	, cosf(g_model_Info[nCount].set.rot.y) * g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].addpos.x);
					rot = g_model_Info[nCount].set.parent[nCntModel].addrot + g_model_Info[nCount].set.rot;
				}

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
				D3DXMatrixMultiply(&g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &mtxTrans);

				if (g_model_Info[nCount].set.parent[nCntModel].nParent >= 0)
				{//親の位置と角度をプラス
					D3DXMatrixMultiply(&g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel, &g_model_Info[nCount].set.parent[g_model_Info[nCount].set.parent[nCntModel].nParent].mtxWorldModel);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_model_Info[nCount].set.parent[nCntModel].mtxWorldModel);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);
				pMat = (D3DXMATERIAL*)g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].pBuffMatModel->GetBufferPointer();
				//テクスチャの設定
				pDevice->SetTexture(0, pTextureModel[g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].nTexture]);

				// マテリアルデータへのポインタを取得
				for (int nCountData = 0; nCountData < (int)g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].nNumMatModel; nCountData++)
				{
					// マテリアルの設定
					pMat = (D3DXMATERIAL*)g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].pBuffMatModel->GetBufferPointer();

					pDevice->SetMaterial(&pMat[nCountData].MatD3D);

					// モデル(パーツ)の描画
					g_model_Draw[g_model_Info[nCount].set.parent[nCntModel].type].MeshModel->DrawSubset(nCountData);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
				pDevice->SetTexture(0, NULL);

			}
		}
	}
}
//=============================================================================
// * 攻撃 当たり判定
//=============================================================================
void Attackjudge(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX	mtxParts;			//判定部位の保存用

	for (int nCntModel = 0; nCntModel < MAX_MODEL_INFO; nCntModel++)
	{
		if (g_model_Info[nCntModel].bUse == true && g_model_Info[nCntModel].use != MODEL_USE_Pause)
		{
			if (g_model_motion[g_model_Info[nCntModel].set.nNumMotion[0]][g_model_Info[nCntModel].set.nNumMotion[1]].key[g_model_Info[nCntModel].set.nNumMotion[3]].nAtkFrame[0] < g_model_Info[nCntModel].set.nNumMotion[4] &&
				g_model_motion[g_model_Info[nCntModel].set.nNumMotion[0]][g_model_Info[nCntModel].set.nNumMotion[1]].key[g_model_Info[nCntModel].set.nNumMotion[3]].nAtkFrame[1] > g_model_Info[nCntModel].set.nNumMotion[4])
			{//攻撃フレーム内なら

				//g_model_motion[g_model_Info[nCntModel].set.nNumMotion[0]][g_model_Info[nCntModel].set.nNumMotion[1]].key[g_model_Info[nCntModel].set.nNumMotion[3]].fRange;
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxParts);
				mtxParts = g_model_Info[nCntModel].set.parent[g_model_motion[g_model_Info[nCntModel].set.nNumMotion[0]][g_model_Info[nCntModel].set.nNumMotion[1]].key[g_model_Info[nCntModel].set.nNumMotion[3]].nParts / 100].mtxWorldModel;

				Hitjudge(nCntModel, 0
					, g_model_motion[g_model_Info[nCntModel].set.nNumMotion[0]][g_model_Info[nCntModel].set.nNumMotion[1]].key[g_model_Info[nCntModel].set.nNumMotion[3]].nParts % 100
					, g_model_motion[g_model_Info[nCntModel].set.nNumMotion[0]][g_model_Info[nCntModel].set.nNumMotion[1]].key[g_model_Info[nCntModel].set.nNumMotion[3]].nInvincible
					, g_model_motion[g_model_Info[nCntModel].set.nNumMotion[0]][g_model_Info[nCntModel].set.nNumMotion[1]].key[g_model_Info[nCntModel].set.nNumMotion[3]].fRange
					, g_model_Info[nCntModel].data.status.fAttack
					, g_model_motion[g_model_Info[nCntModel].set.nNumMotion[0]][g_model_Info[nCntModel].set.nNumMotion[1]].key[g_model_Info[nCntModel].set.nNumMotion[3]].fAtkplus
					, g_model_motion[g_model_Info[nCntModel].set.nNumMotion[0]][g_model_Info[nCntModel].set.nNumMotion[1]].key[g_model_Info[nCntModel].set.nNumMotion[3]].blown
					, mtxParts, 0);
			}//攻撃判定
		}
	}//for文
}

//=============================================================================
// * 攻撃 当たり判定
//=============================================================================
void Hitjudge(int nCntModel, int nCntBullet, int nCntAtk, int nInvincible, float fRange, float fAtk, float fAtkplus, D3DXVECTOR3 blown, D3DXMATRIX	mtxParts, int nMode)
{
	Camera *pCamera = GetCamera();
	D3DXMATRIX	mtxRot, mtxTrans;		//計算用マトリックス
	D3DXMATRIX mtxSave = mtxParts;
	float		fRange0;
	float		fDef, fLuk;	//ステータス計算用
	float		fDamage;			//ダメージ計算用
	float		fPredator;			//0.0f捕食無し, 倍率に応じてスキルドロップ
	float		fHeel;				//吸血の倍率
	float		fMoveY;
	bool		bSkill;
	float	fLengthOneself;
	int		nBattle[2];
	if (g_model_Info[nCntModel].use == MODEL_USE_PLAYER) { nBattle[0] = 0; }
	else { nBattle[0] = 1; }
	fRange0 = fRange;

	for (int nCntChar = 0; nCntChar < MAX_MODEL_INFO; nCntChar++)
	{//全モデル数回転
		if (g_model_Info[nCntChar].bUse == true && nCntModel != nCntChar && g_model_Info[nCntChar].set.nAirTime == 0 && g_model_Info[nCntChar].set.state != MODEL_STATE_MAP && g_model_Info[nCntChar].use != MODEL_USE_Pause)
		{//使われている && 自分と仲間じゃない
			if (g_model_Info[nCntChar].use == MODEL_USE_PLAYER) { nBattle[1] = 0; }
			else { nBattle[1] = 1; }
			if (nBattle[0] != nBattle[1])
			{
				//fRange = powf(fRange, 2) + powf(fRange, 2);
				fLengthOneself = powf(fRange0 + 15.0f, 2);		//目標ののXZの範囲

				for (int nCntLength = 0; nCntLength < nCntAtk; nCntLength++)
				{//判定個数分回る
					if ((mtxParts._42 - fRange0 <= g_model_Info[nCntChar].set.pos.y + g_model_Info[nCntChar].set.fHeight && mtxParts._42 + fRange0 >= g_model_Info[nCntChar].set.pos.y))
					{//Y軸範囲内なら
						float fLengthX = g_model_Info[nCntChar].set.pos.x - mtxParts._41;	//Xの差
						float fLengthZ = g_model_Info[nCntChar].set.pos.z - mtxParts._43;	//Zの差
						float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);
						if (fLengthOneself >= fLengthTotal)
						{//XZ範囲内なら
							fHeel = 0.0f; fPredator = 0.0f; bSkill = false;

							if ((g_model_Info[nCntChar].set.state == MODEL_STATE_POISED && g_model_Info[nCntChar].set.nCntState > 0) || g_model_Info[nCntChar].set.state == MODEL_STATE_GUARD)
							{
								//モーションの変化
								g_model_Info[nCntChar].set.nNumMotion[3] = -1;
								g_model_Info[nCntChar].set.nNumMotion[1] = MOTION_GUARD;
								g_model_Info[nCntChar].set.state = MODEL_STATE_GUARD;
								g_model_Info[nCntChar].set.nAirTime = nInvincible;
								if (g_model_Info[nCntChar].set.nAirTime > 5) { g_model_Info[nCntChar].set.nAirTime = 5; }
								fMoveY = 0.0f;
								PlaySound(SOUND_LABEL_LOCKON);
								SetAnimation(g_model_Info[nCntChar].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 50, 50, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
							}
							else
							{
								//ダメージ
								fAtk = (fAtk * (rand() % 31 + 95)) * 0.01f;
								if (nMode == 0) { fDef = (g_model_Info[nCntChar].data.status.fDefense * (rand() % 21 + 45)) * 0.01f; }
								else { fDef = (g_model_Info[nCntChar].data.status.fRes * (rand() % 21 + 45)) * 0.01f; }
								//ヒット時効果
								if (nMode == 0) { MotionEffect_A(nCntModel, nCntChar, fAtk, &fHeel, &fPredator); }
								else { UpdateABullet(nCntBullet, nCntChar, fAtk); }

								//持っているスキル確認
								if (g_model_Info[nCntModel].data.nSkill[0][4] > 0)
								{//スキルなら追加効果確認
									bSkill = true;
									SkillEffect_A(&g_model_Info[nCntModel].data.nSkill[0][0], nCntModel, nCntChar, &fAtk, &fHeel, &fPredator);
								}
								else if (g_model_Info[nCntModel].data.nSkill[1][4] > 0)
								{//スキルなら追加効果確認
									bSkill = true;
									SkillEffect_A(&g_model_Info[nCntModel].data.nSkill[1][0], nCntModel, nCntChar, &fAtk, &fHeel, &fPredator);
								}


								fLuk = (g_model_Info[nCntModel].data.status.fLucky - g_model_Info[nCntChar].data.status.fLucky) / 50;
								if (fLuk < 5.0f) { fLuk = 5.0f; }				//低すぎた時の処理　最低5％
								else if (fLuk > 90.0f) { fLuk = 90.0f; }		//高すぎた場合　最大90％
								if (rand() % 100 < (int)fLuk)
								{//会心
								 //会心音//倍率に応じて
									fDamage = 1.35f;
									fAtk *= 1.25f;
									//エフェクト
									PlaySound(SOUND_LABEL_Damage002);
									SetAnimation(g_model_Info[nCntChar].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 200, 200, 5, 2, 3, 2, 1, ANIMATION_NORMAL);
								}
								else
								{//通常音
								 //倍率に応じて
									fDamage = 1.0f;
									SetAnimation(g_model_Info[nCntChar].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.0f, 0.8f), 150, 150, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
									SetAnimation(g_model_Info[nCntChar].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 50, 50, 5, 1, 5, 0, 1, ANIMATION_NORMAL);
								}
								//攻撃倍率を含めてダメージ計算
								fDamage = (fAtk - fDef) * fDamage;
								if (fDamage < fAtk * 0.5f) { fDamage = fAtk * 0.5f; }
								fDamage *= fAtkplus;
								g_model_Info[nCntChar].data.nLifePoint -= (int)fDamage;

								//持っているスキルでなければ回復
								g_model_Info[nCntModel].data.nSkillPoint += (int)(fDamage * 0.015f);

								//吸血が入っているなら
								if (fHeel > 0.0f) { g_model_Info[nCntModel].data.nLifePoint += (int)(fDamage * fHeel); }

								if (fPredator > 0.0f)
								{//捕食 回復
									g_model_Info[nCntModel].data.nLifePoint += (int)(fDamage * 0.25f);
									g_model_Info[nCntModel].data.nSkillPoint += (int)(fDamage * 0.08f);
									SetZWindow(g_model_Info[nCntModel].set.pos + D3DXVECTOR3(0.0f, 80.0f, 0.0f), D3DXVECTOR3(sinf(pCamera[0].rot.y), 0.0f, cosf(pCamera[0].rot.y)) * 3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500, 500, 4, 2, WINDOWSTATE_FADE_Z, 0.02f);
									SetAnimation(g_model_Info[nCntModel].set.pos + D3DXVECTOR3((rand() % 2000) * 0.1f - 100.0f, (rand() % 1000) * 0.1f, (rand() % 2000) * 0.1f - 100.0f), D3DXCOLOR(0.4f, 1.0f, 1.0f, 0.8f), 250, 250, 10, 1, 5, 1, 2, ANIMATION_NORMAL);
									SetAnimation(g_model_Info[nCntModel].set.pos + D3DXVECTOR3((rand() % 2000) * 0.1f - 100.0f, (rand() % 1000) * 0.1f, (rand() % 2000) * 0.1f - 100.0f), D3DXCOLOR(0.4f, 1.0f, 1.0f, 0.7f), 350, 450, 10, 1, 6, 1, 2, ANIMATION_NORMAL);
									SetAnimation(g_model_Info[nCntModel].set.pos + D3DXVECTOR3((rand() % 2000) * 0.1f - 100.0f, (rand() % 1000) * 0.1f, (rand() % 2000) * 0.1f - 100.0f), D3DXCOLOR(0.4f, 1.0f, 1.0f, 0.6f), 450, 450, 10, 1, 7, 1, 2, ANIMATION_NORMAL);
								}

								if (g_model_Info[nCntModel].data.nSkillPoint > (int)g_model_Info[nCntModel].data.status.fSkillPoint) { g_model_Info[nCntModel].data.nSkillPoint = (int)g_model_Info[nCntModel].data.status.fSkillPoint; }
								if (g_model_Info[nCntModel].data.nLifePoint > (int)g_model_Info[nCntModel].data.status.fLifePoint) { g_model_Info[nCntModel].data.nLifePoint = (int)g_model_Info[nCntModel].data.status.fLifePoint; }

								//怯みと無敵
								g_model_Info[nCntChar].set.state = MODEL_STATE_DAMAGE;
								g_model_Info[nCntChar].set.nCntState = (int)(fAtkplus * 30.0f);
								//モーションの変化
								g_model_Info[nCntChar].set.nAirTime = nInvincible;
								g_model_Info[nCntChar].set.nNumMotion[3] = -1;
								g_model_Info[nCntChar].set.nNumMotion[1] = MOTION_SHRINK;

								fMoveY = blown.y;
							}
							//吹っ飛び
							g_model_Info[nCntChar].set.move = D3DXVECTOR3(sinf(g_model_Info[nCntModel].set.angle.y), 0.0f, cosf(g_model_Info[nCntModel].set.angle.y))
								* -blown.z;
							g_model_Info[nCntChar].set.move.y = fMoveY;
							if (g_model_Info[nCntChar].set.move.y > 0.0f) { g_model_Info[nCntChar].set.bJump = false; }

							//攻撃ヒット状態に
							g_model_Info[nCntModel].set.state = MODEL_STATE_ATKHIT;


							if (g_model_Info[nCntChar].data.nLifePoint < 0)
							{//死んだ場合
								g_model_Info[nCntChar].data.nLifePoint = 0;
								if (g_model_Info[nCntChar].set.bJump == true)
								{//浮いていない
									g_model_Info[nCntChar].set.move.y = 5.0f;
									g_model_Info[nCntChar].set.bJump = false;
								}
								g_model_Info[nCntChar].set.nAirTime = 9999;

								if (g_model_Info[nCntChar].use == MODEL_USE_PLAYER)
								{//プレイヤーなら終わり
									StopSound();
									PlaySound(SOUND_LABEL_DEAD_END);
								}
								else
								{//敵なら経験値回収, レベル上がる
									ENEMY *pEnemy = GetEnemy();

									if (pEnemy[g_model_Info[nCntChar].set.nCntChar].nDying == 2)
									{//通常なら　 //瀕死になってHP1
										pEnemy[g_model_Info[nCntChar].set.nCntChar].nDying = 1;
										g_model_Info[nCntChar].data.nLifePoint = 1;
										g_model_Info[nCntChar].set.move.x *= 0.1f;
										g_model_Info[nCntChar].set.move.z *= 0.1f;
										PlaySound(SOUND_LABEL_TitleSE001);
									}
									else
									{//瀕死ならor瀕死無視なら
										pEnemy[g_model_Info[nCntChar].set.nCntChar].nDying = 0;
										PLAYER *pPlayer = GetPlayer();
										CHAR_DATA	dataold = g_model_Info[nCntModel].data;
										char aStr[48];
										int nPlus = (g_model_Info[nCntChar].data.nStrength / 500);
										bool bPredator = false;

										pPlayer[0].nTime[0] += nPlus;
										wsprintf(&aStr[0], "+%d.%d", nPlus / 10, nPlus % 10);
										SetFont(RECT{ 540 , 35, 1200, 720 }, D3DXCOLOR{ 1.0f, 0.95f, 0.08f , 1.0f }, &aStr[0], 10, 3, DT_LEFT, -1, true, FONTSTATE_DELETE, WINDOW_DRAW_0);

										if (fPredator > 0.0f)
										{//捕食なら経験値なし
											switch (g_model_Info[nCntChar].set.parent[0].type)
											{
											case MODEL_TYPE_EWarrior_bodyD:	//攻撃力上昇
												bPredator = true;
												g_model_Info[nCntModel].data.gifted.fAttack += g_model_Info[nCntChar].data.gifted.fAttack * 0.25f;
												break;
											case MODEL_TYPE_EWizard_bodyD:	//魔力上昇
												bPredator = true;
												g_model_Info[nCntModel].data.gifted.fInt += g_model_Info[nCntChar].data.gifted.fInt * 0.25f;
												break;
											case MODEL_TYPE_ENinja_bodyD:	//スピード上昇
												bPredator = true;
												g_model_Info[nCntModel].data.gifted.fSpeed += g_model_Info[nCntChar].data.gifted.fSpeed * 0.25f;
												break;
											}
											if (bPredator == true)
											{
												GetStatus(&g_model_Info[nCntModel].data, true);
												ShowStatus(dataold, g_model_Info[nCntModel].data);

												if (rand() % pEnemy[g_model_Info[nCntChar].set.nCntChar].nDropS < (int)(fPredator * (1.0f + g_model_Info[nCntModel].data.status.fLucky * 0.001f)))
												{//スキルドロップ
													DropSkill(pEnemy[g_model_Info[nCntChar].set.nCntChar].nDSkill[rand() % 2]);
												}
											}
										}
										if (bPredator == false)
										{//捕食無し
											CHAR_DATA	data;
											float		fLife, fSkill;
											g_model_Info[nCntModel].data.nExperience += g_model_Info[nCntChar].data.nStrength;
											while (1)
											{
												if (g_model_Info[nCntModel].data.nExperience > g_model_Info[nCntModel].data.nNextLevel)
												{//レベルアップ
													SetAnimation(g_model_Info[nCntChar].set.pos + D3DXVECTOR3(0.0f, 40.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 200, 50, 1, 7, 5, 3, 1, ANIMATION_NORMAL);
													g_model_Info[nCntModel].data.nExperience -= g_model_Info[nCntModel].data.nNextLevel;
													g_model_Info[nCntModel].data.nNextLevel = (int)(INIT_EXP * powf(EXP_UP, g_model_Info[nCntModel].data.nLevel * 1.0f));
													g_model_Info[nCntModel].data.nLevel++;
													//回復
													data = g_model_Info[nCntModel].data;
													GetStatus(&data, true);
													fLife = data.status.fLifePoint - g_model_Info[nCntModel].data.status.fLifePoint;
													fSkill = data.status.fSkillPoint - g_model_Info[nCntModel].data.status.fSkillPoint;
													g_model_Info[nCntModel].data = data;
													g_model_Info[nCntModel].data.nLifePoint += (int)(fLife + g_model_Info[nCntModel].data.status.fLifePoint * 0.3f);
													g_model_Info[nCntModel].data.nSkillPoint += (int)(fSkill + g_model_Info[nCntModel].data.status.fSkillPoint * 0.5f);
													if (g_model_Info[nCntModel].data.nLifePoint > (int)g_model_Info[nCntModel].data.status.fLifePoint) { g_model_Info[nCntModel].data.nLifePoint = (int)g_model_Info[nCntModel].data.status.fLifePoint; }
													if (g_model_Info[nCntModel].data.nSkillPoint > (int)g_model_Info[nCntModel].data.status.fSkillPoint) { g_model_Info[nCntModel].data.nSkillPoint = (int)g_model_Info[nCntModel].data.status.fSkillPoint; }
												}
												else
												{
													if (dataold.nLevel < g_model_Info[nCntModel].data.nLevel)
													{
														ShowStatus(dataold, g_model_Info[nCntModel].data);
													}
													break;
												}
											}
										}//撃破else
										if (rand() % pEnemy[g_model_Info[nCntChar].set.nCntChar].nDropI < (int)(350.0f + g_model_Info[nCntModel].data.status.fLucky * 0.1f))
										{//アイテムドロップ

										}

										if (g_model_Info[nCntChar].use == MODEL_USE_ENEMY_B)
										{//ゲームクリアへ
											StopSound();
											PlaySound(SOUND_LABEL_BOSS_D);
										}
										if (g_model_Info[nCntChar].use == MODEL_USE_ENEMY_T)
										{//道ができる
											PlaySound(SOUND_LABEL_BOSS_D);
											DropSkill(25);
											SetMap_Change(D3DXVECTOR3(0.0f, 0.0f, -8850.0f), D3DXVECTOR3(0.0f, 0.0f, 2700.0f), 1000, 80, MAP_TYPE_301_CASTLE);
										}
										else { PlaySound(SOUND_LABEL_TitleSE001); }
									}
								}//	ダメージ音
							}
							else { PlaySound(SOUND_LABEL_Damage000); }
							break;
						}//XZ範囲
					}//Y範囲
					 //団子剣に
					 // 移動を反映
					D3DXMatrixTranslation(&mtxTrans, 0.0f, fRange0, 0.0f);
					D3DXMatrixMultiply(&mtxTrans, &mtxTrans, &mtxParts);
					mtxParts = mtxTrans;
				}//for文
				mtxParts = mtxSave;
			}//判定 敵味方
		}//判定 対象者
	}//for受け側
}
//=============================================================================
// スキルドロップ処理
//=============================================================================
void DropSkill(int nSkill)
{
	if (nSkill > 0)
	{//持っていないSkill
		PLAYER *pPlayer = GetPlayer();

		for (int nCntSkill = 0; nCntSkill < 3; nCntSkill++)
		{
			if (pPlayer[0].nDSkill[nCntSkill] == nSkill)
			{//同じものがドロップ
				break;
			}
			else if (pPlayer[0].nDSkill[nCntSkill] == -1)
			{//中身が空
				pPlayer[0].nDSkill[nCntSkill] = nSkill;
				break;
			}
		}
	}
}
//=============================================================================
// スキル交換処理
//=============================================================================
void SwapSkill(int nNumber, int nCount)
{
	PLAYER *pPlayer = GetPlayer();
	int nLength;

	if (nNumber > -1)
	{//スキル入れ替え
		MODEL_MOTION *pMotion = GetModel_Motion();
		char aStr[48];

		g_model_Info[0].data.nSkill[nNumber][0] = pPlayer[0].nDSkill[0];
		PlusSkill(&g_model_Info[0].data.nSkill[nNumber][0], nCount); //ランダムにスキル+
		if (nNumber == 0)
		{
			wsprintf(&aStr[0], "%d SP\n", MotionSkill(pMotion[g_model_Info[0].set.nNumMotion[0] * MAX_MODEL_MOTION + g_model_Info[0].data.nSkill[nNumber][0]].key[0].nAtkEffect));
			ReturnSkillName(&aStr[0], g_model_Info[0].data.nSkill[nNumber][0]);
		}
		else
		{
			wsprintf(&aStr[0], "", MotionSkill(pMotion[g_model_Info[0].set.nNumMotion[0] * MAX_MODEL_MOTION + g_model_Info[0].data.nSkill[nNumber][0]].key[0].nAtkEffect));
			ReturnSkillName(&aStr[0], g_model_Info[0].data.nSkill[nNumber][0]);
			nLength = (int)strlen(&aStr[0]);
			wsprintf(&aStr[nLength], "\n%d SP\n", MotionSkill(pMotion[g_model_Info[0].set.nNumMotion[0] * MAX_MODEL_MOTION + g_model_Info[0].data.nSkill[nNumber][0]].key[0].nAtkEffect));
		}
		ChangeFont(true, &aStr[0], 5 + nNumber, 0, -1, 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	for (int nCntSkill = 0; nCntSkill < 2; nCntSkill++)
	{
		pPlayer[0].nDSkill[nCntSkill] = pPlayer[0].nDSkill[nCntSkill + 1];
	}
	pPlayer[0].nDSkill[2] = -1;
}
//=============================================================================
// スキル追加処理
//=============================================================================
void PlusSkill(int *nSkill, int nCount)
{
	int nRand;
	int nRandL;

	int nSave[3] = { -1, -1, -1 };
	int nMap = 0;
	PLAYER  *pPlayer = GetPlayer();

	switch (pPlayer[0].maptype)
	{
	case MAP_TYPE_100_YEUNG:
		nMap = 1; break;
	case MAP_TYPE_200_CITY:
		nMap = 1; break;
	case MAP_TYPE_300_CASTLE:
		nMap = 2; break;
	case MAP_TYPE_301_CASTLE:
		nMap = 3; break;
	}
	for (int nCntSkill = 1; nCntSkill < 4; nCntSkill++)
	{//空にする
		nSkill[nCntSkill] = -1;
	}

	for (int nCntSkill = 1; nCntSkill < 4; nCntSkill++)
	{//スキル3つまで 0はスキル番号
		if (rand() % (4000 * nCntSkill) < (int)(5000.0f * (1.0f + g_model_Info[nCount].data.status.fLucky * 0.0002f)))
		{//追加
			nRand = rand() % 2; //攻撃時か常時か
			if (nSkill[0] > 30) { nRand = 0; }

			nRandL = rand() % 10000 - (int)(g_model_Info[nCount].data.status.fLucky * 0.08f);
			if (nRandL < 500 * nCntSkill + 500)			{ nRandL = 2; }	//☆5
			else if (nRandL < 2500 * nCntSkill + 2500)	{ nRandL = 1; }	//☆4
			else										{ nRandL = 0; }	//☆3


			if(nRand == 0)
			{//発動時スキル
				if(rand() % 8 == 0){ nRand = 100 + nRandL; }	//100とランク+
				else{ nRand = (rand() % 7) * 10 + (rand() % 2) * 3 + nRandL; }	//0～70と2種類+ランク
			}
			else
			{//Hit時スキル
				nRand = 1000 + (rand() % 3) * 10 + nRandL;
			}

			//被っていたら破棄
			if (nRand / 10 == nSave[0] / 10 || nRand / 10 == nSave[1] / 10) { break; }
			nSkill[nCntSkill] = nRand;
			nSave[nCntSkill - 1] = nSkill[nCntSkill];
		}
		else
		{ break; }
	}
}
//=============================================================================
// Skillの追加効果実行処理
//=============================================================================
void SkillEffect_S(int *nSkill, int nCount)
{
	for (int nCntSkill = 1; nCntSkill < 4; nCntSkill++)
	{//スキル3つまで 0はスキル番号
		switch (nSkill[nCntSkill])
		{
		case 0:	//HP小　上昇　短時間
			g_model_Info[nCount].data.plus[0].fLifePoint = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fLifePoint = 8000.0f;
			GetStatus(&g_model_Info[nCount].data, true);
			g_model_Info[nCount].data.nLifePoint += (int)8000.0f;
			break;
		case 1:	//HP中　上昇　短時間
			g_model_Info[nCount].data.plus[0].fLifePoint = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fLifePoint = 2000.0f;
			GetStatus(&g_model_Info[nCount].data, true);
			g_model_Info[nCount].data.nLifePoint += (int)2000.0f;
			break;
		case 2:	//HP大　上昇　短時間
			g_model_Info[nCount].data.plus[0].fLifePoint = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fLifePoint = 50000.0f;
			GetStatus(&g_model_Info[nCount].data, true);
			g_model_Info[nCount].data.nLifePoint += (int)50000.0f;
			break;
		case 3:	//HP小　上昇
			g_model_Info[nCount].data.plus[0].fLifePoint = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fLifePoint = 8000.0f;
			GetStatus(&g_model_Info[nCount].data, true);
			g_model_Info[nCount].data.nLifePoint += (int)8000.0f;
			break;
		case 4:	//HP中　上昇
			g_model_Info[nCount].data.plus[0].fLifePoint = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fLifePoint = 20000.0f;
			GetStatus(&g_model_Info[nCount].data, true);
			g_model_Info[nCount].data.nLifePoint += (int)20000.0f;
			break;
		case 5:	//HP大　上昇
			g_model_Info[nCount].data.plus[0].fLifePoint = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fLifePoint = 80000.0f;
			GetStatus(&g_model_Info[nCount].data, true);
			g_model_Info[nCount].data.nLifePoint += (int)50000.0f;
			break;
			//--------------------------------------------------------------------------------------------
		case 10:	//攻撃小　上昇　短時間
			g_model_Info[nCount].data.plus[0].fAttack = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fAttack = (g_model_Info[nCount].data.status.fAttack - g_model_Info[nCount].data.plus[1].fAttack) * 0.3f;
			break;
		case 11:	//攻撃中上昇　短時間
			g_model_Info[nCount].data.plus[0].fAttack = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fAttack = (g_model_Info[nCount].data.status.fAttack - g_model_Info[nCount].data.plus[1].fAttack) * 0.6f;
			break;
		case 12:	//攻撃大　上昇　短時間
			g_model_Info[nCount].data.plus[0].fAttack = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fAttack = (g_model_Info[nCount].data.status.fAttack - g_model_Info[nCount].data.plus[1].fAttack) * 1.0f;
			break;
		case 13:	//攻撃小　上昇
			g_model_Info[nCount].data.plus[0].fAttack = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fAttack = (g_model_Info[nCount].data.status.fAttack - g_model_Info[nCount].data.plus[1].fAttack) * 0.3f;
			break;
		case 14:	//攻撃中　上昇
			g_model_Info[nCount].data.plus[0].fAttack = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fAttack = (g_model_Info[nCount].data.status.fAttack - g_model_Info[nCount].data.plus[1].fAttack) * 0.6f;
			break;
		case 15:	//攻撃大　上昇
			g_model_Info[nCount].data.plus[0].fAttack = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fAttack = (g_model_Info[nCount].data.status.fAttack - g_model_Info[nCount].data.plus[1].fAttack) * 1.0f;
			break;
			//--------------------------------------------------------------------------------------------
		case 20:	//防御小　上昇　短時間
			g_model_Info[nCount].data.plus[0].fDefense = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fDefense = (g_model_Info[nCount].data.status.fDefense - g_model_Info[nCount].data.plus[1].fDefense) * 0.3f;
			break;
		case 21:	//防御中　上昇　短時間
			g_model_Info[nCount].data.plus[0].fDefense = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fDefense = (g_model_Info[nCount].data.status.fDefense - g_model_Info[nCount].data.plus[1].fDefense) * 0.6f;
			break;
		case 22:	//防御大　上昇　短時間
			g_model_Info[nCount].data.plus[0].fDefense = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fDefense = (g_model_Info[nCount].data.status.fDefense - g_model_Info[nCount].data.plus[1].fDefense) * 1.0f;
			break;
		case 23:	//防御小　上昇
			g_model_Info[nCount].data.plus[0].fDefense = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fDefense = (g_model_Info[nCount].data.status.fDefense - g_model_Info[nCount].data.plus[1].fDefense) * 0.3f;
			break;
		case 24:	//防御中　上昇
			g_model_Info[nCount].data.plus[0].fDefense = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fDefense = (g_model_Info[nCount].data.status.fDefense - g_model_Info[nCount].data.plus[1].fDefense) * 0.6f;
			break;
		case 25:	//防御大　上昇
			g_model_Info[nCount].data.plus[0].fDefense = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fDefense = (g_model_Info[nCount].data.status.fDefense - g_model_Info[nCount].data.plus[1].fDefense) * 1.0f;
			break;
			//--------------------------------------------------------------------------------------------
		case 30:	//魔攻小　上昇　短時間
			g_model_Info[nCount].data.plus[0].fInt = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fInt = (g_model_Info[nCount].data.status.fInt - g_model_Info[nCount].data.plus[1].fInt) * 0.3f;
			break;
		case 31:	//魔攻中　上昇　短時間
			g_model_Info[nCount].data.plus[0].fInt = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fInt = (g_model_Info[nCount].data.status.fInt - g_model_Info[nCount].data.plus[1].fInt)  * 0.6f;
			break;
		case 32:	//魔攻大　上昇　短時間
			g_model_Info[nCount].data.plus[0].fInt = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fInt = (g_model_Info[nCount].data.status.fInt - g_model_Info[nCount].data.plus[1].fInt)  * 1.0f;
			break;
		case 33:	//魔攻小　上昇
			g_model_Info[nCount].data.plus[0].fInt = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fInt = (g_model_Info[nCount].data.status.fInt - g_model_Info[nCount].data.plus[1].fInt)  * 0.3f;
			break;
		case 34:	//魔攻中　上昇
			g_model_Info[nCount].data.plus[0].fInt = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fInt = (g_model_Info[nCount].data.status.fInt - g_model_Info[nCount].data.plus[1].fInt)  * 0.6f;
			break;
		case 35:	//魔攻大　上昇
			g_model_Info[nCount].data.plus[0].fInt = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fInt = (g_model_Info[nCount].data.status.fInt - g_model_Info[nCount].data.plus[1].fInt)  * 1.0f;
			break;
			//--------------------------------------------------------------------------------------------
		case 40:	//魔防小　上昇　短時間
			g_model_Info[nCount].data.plus[0].fRes = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fRes = (g_model_Info[nCount].data.status.fRes - g_model_Info[nCount].data.plus[1].fRes) * 0.3f;
		case 41:	//魔防中　上昇　短時間
			g_model_Info[nCount].data.plus[0].fRes = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fRes = (g_model_Info[nCount].data.status.fRes - g_model_Info[nCount].data.plus[1].fRes) * 0.6f;
			break;
		case 42:	//魔防大　上昇　短時間
			g_model_Info[nCount].data.plus[0].fRes = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fRes = (g_model_Info[nCount].data.status.fRes - g_model_Info[nCount].data.plus[1].fRes) * 1.0f;
			break;
		case 43:	//魔防小　上昇
			g_model_Info[nCount].data.plus[0].fRes = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fRes = (g_model_Info[nCount].data.status.fRes - g_model_Info[nCount].data.plus[1].fRes) * 0.3f;
			break;
		case 44:	//魔防中　上昇
			g_model_Info[nCount].data.plus[0].fRes = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fRes = (g_model_Info[nCount].data.status.fRes - g_model_Info[nCount].data.plus[1].fRes) * 0.6f;
			break;
		case 45:	//魔防大　上昇
			g_model_Info[nCount].data.plus[0].fRes = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fRes = (g_model_Info[nCount].data.status.fRes - g_model_Info[nCount].data.plus[1].fRes) * 1.0f;
			break;
			//--------------------------------------------------------------------------------------------
		case 50:	//速度小　上昇　短時間
			g_model_Info[nCount].data.plus[0].fSpeed = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fSpeed = (g_model_Info[nCount].data.status.fSpeed - g_model_Info[nCount].data.plus[1].fSpeed) * 0.3f;
			break;
		case 51:	//速度中　上昇　短時間
			g_model_Info[nCount].data.plus[0].fSpeed = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fSpeed = (g_model_Info[nCount].data.status.fSpeed - g_model_Info[nCount].data.plus[1].fSpeed) * 0.6f;
			break;
		case 52:	//速度大　上昇　短時間
			g_model_Info[nCount].data.plus[0].fSpeed = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fSpeed = (g_model_Info[nCount].data.status.fSpeed - g_model_Info[nCount].data.plus[1].fSpeed) * 1.0f;
			break;
		case 53:	//速度小　上昇
			g_model_Info[nCount].data.plus[0].fSpeed = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fSpeed = (g_model_Info[nCount].data.status.fSpeed - g_model_Info[nCount].data.plus[1].fSpeed) * 0.3f;
			break;
		case 54:	//速度中　上昇
			g_model_Info[nCount].data.plus[0].fSpeed = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fSpeed = (g_model_Info[nCount].data.status.fSpeed - g_model_Info[nCount].data.plus[1].fSpeed) * 0.6f;
			break;
		case 55:	//速度大　上昇
			g_model_Info[nCount].data.plus[0].fSpeed = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fSpeed = (g_model_Info[nCount].data.status.fSpeed - g_model_Info[nCount].data.plus[1].fSpeed) * 1.0f;
			break;
			//--------------------------------------------------------------------------------------------
		case 60:	//速度小　上昇　短時間
			g_model_Info[nCount].data.plus[0].fLucky = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fLucky = (g_model_Info[nCount].data.status.fLucky - g_model_Info[nCount].data.plus[1].fLucky) * 0.5f;
			break;
		case 61:	//速度中　上昇　短時間
			g_model_Info[nCount].data.plus[0].fLucky = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fLucky = (g_model_Info[nCount].data.status.fLucky - g_model_Info[nCount].data.plus[1].fLucky) * 1.0f;
			break;
		case 62:	//速度大　上昇　短時間
			g_model_Info[nCount].data.plus[0].fLucky = SKILL_TIME;
			g_model_Info[nCount].data.plus[1].fLucky = (g_model_Info[nCount].data.status.fLucky - g_model_Info[nCount].data.plus[1].fLucky) * 2.0f;
			break;
		case 63:	//速度小　上昇
			g_model_Info[nCount].data.plus[0].fLucky = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fLucky = (g_model_Info[nCount].data.status.fLucky - g_model_Info[nCount].data.plus[1].fLucky) * 0.5f;
			break;
		case 64:	//速度中　上昇
			g_model_Info[nCount].data.plus[0].fLucky = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fLucky = (g_model_Info[nCount].data.status.fLucky - g_model_Info[nCount].data.plus[1].fLucky) * 1.0f;
			break;
		case 65:	//速度大　上昇
			g_model_Info[nCount].data.plus[0].fLucky = SKILL_TIME * 2;
			g_model_Info[nCount].data.plus[1].fLucky = (g_model_Info[nCount].data.status.fLucky - g_model_Info[nCount].data.plus[1].fLucky) * 2.0f;
			break;
			//--------------------------------------------------------------------------------------------
		case 100:	//HP小　回復
			g_model_Info[nCount].data.nLifePoint += (int)(10000 * (1.0f + g_model_Info[nCount].data.status.fRes * 0.0001f));
			break;
		case 101:	//HP中　回復
			g_model_Info[nCount].data.nLifePoint += (int)(30000 * (1.0f + g_model_Info[nCount].data.status.fRes * 0.0001f));
			break;
		case 102:	//HP大　回復
			g_model_Info[nCount].data.nLifePoint += (int)(65000 * (1.0f + g_model_Info[nCount].data.status.fRes * 0.0001f));
			break;
		}

		if (nSkill[nCntSkill] < 100 && nSkill[nCntSkill] > 0)
		{//ステ上昇
			PlaySound(SOUND_LABEL_UP);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 150, 150, 5, 2, 4, 12, 1, ANIMATION_NORMAL);
		}
		else if (nSkill[nCntSkill] < 1000 && nSkill[nCntSkill] > 0)
		{//回復
			PlaySound(SOUND_LABEL_HELL);
			SetAnimation(g_model_Info[nCount].set.pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 150, 150, 4, 2, 4, 11, 1, ANIMATION_NORMAL);
		}
	}

	GetStatus(&g_model_Info[nCount].data, true);
	if (g_model_Info[nCount].data.nLifePoint > (int)g_model_Info[nCount].data.status.fLifePoint)
	{//HP確認
		g_model_Info[nCount].data.nLifePoint = (int)g_model_Info[nCount].data.status.fLifePoint;
	}

}//=============================================================================
// Skillの追加効果実行処理 攻撃ヒット時
//=============================================================================
void SkillEffect_A(int *nSkill, int nCount, int nEnemy, float *fAtk, float *fHeel, float *fPredator)
{
	for (int nCntSkill = 1; nCntSkill < 4; nCntSkill++)
	{//スキル3つまで 0はスキル番号
		switch (nSkill[nCntSkill])
		{
		case 1000:	//スキル攻撃力UP　小
			fAtk[0] *= 1.2f;
			break;
		case 1001:	//スキル攻撃力UP　中
			fAtk[0] *= 1.5f;
			break;
		case 1002:	//スキル攻撃力UP　大
			fAtk[0] *= 1.8f;
			break;
			//--------------------------------------------------------------------------------------------
		case 1010:	//回復　小
			fHeel[0] += 0.1f;
			break;
		case 1011:	//回復　中
			fHeel[0] += 0.2f;
			break;
		case 1012:	//回復　大
			fHeel[0] += 0.35f;
			break;
			//--------------------------------------------------------------------------------------------
		case 1020:	//捕食　小
			fPredator[0] += 0.1f;
			break;
		case 1021:	//捕食　中
			fPredator[0] += 0.5f;
			break;
		case 1022:	//捕食　大
			fPredator[0] += 1.0f;
			break;
		}
	}
}
//=============================================================================
// 追加スキルネーム返信処理
//=============================================================================
void ReturnSkillName_P(char *aName, int *nSkill)
{
	//空にする
	strcpy(&aName[0], "");

	for (int nCntSkill = 1; nCntSkill < 4; nCntSkill++)
	{//スキル3つまで 0はスキル番号
		switch (nSkill[nCntSkill])
		{
		case 0:
			strcat(&aName[0], "+ 短時間HP上昇(小)\n");
			break;
		case 1:
			strcat(&aName[0], "+ 短時間HP上昇(中)\n");
			break;
		case 2:
			strcat(&aName[0], "+ 短時間HP上昇(大)\n");
			break;
		case 3:
			strcat(&aName[0], "+ 一定時間HP上昇(小)\n");
			break;
		case 4:
			strcat(&aName[0], "+ 一定時間HP上昇(中)\n");
			break;
		case 5:
			strcat(&aName[0], "+ 一定時間HP上昇(大)\n");
			break;
			//--------------------------------------------------------------------------------------------
		case 10:
			strcat(&aName[0], "+ 短時間ATK上昇(小)\n");
			break;
		case 11:
			strcat(&aName[0], "+ 短時間ATK上昇(中)\n");
			break;
		case 12:
			strcat(&aName[0], "+ 短時間ATK上昇(大)\n");
			break;
		case 13:
			strcat(&aName[0], "+ 一定時間ATK上昇(小)\n");
			break;
		case 14:
			strcat(&aName[0], "+ 一定時間ATK上昇(中)\n");
			break;
		case 15:
			strcat(&aName[0], "+ 一定時間ATK上昇(大)\n");
			break;
			//--------------------------------------------------------------------------------------------
		case 20:
			strcat(&aName[0], "+ 短時間DEF上昇(小)\n");
			break;
		case 21:
			strcat(&aName[0], "+ 短時間DEF上昇(中)\n");
			break;
		case 22:
			strcat(&aName[0], "+ 短時間DEF上昇(大)\n");
			break;
		case 23:
			strcat(&aName[0], "+ 一定時間DEF上昇(小)\n");
			break;
		case 24:
			strcat(&aName[0], "+ 一定時間DEF上昇(中)\n");
			break;
		case 25:
			strcat(&aName[0], "+ 一定時間DEF上昇(大)\n");
			break;
			//--------------------------------------------------------------------------------------------
		case 30:
			strcat(&aName[0], "+ 短時間INT上昇(小)\n");
			break;
		case 31:
			strcat(&aName[0], "+ 短時間INT上昇(中)\n");
			break;
		case 32:
			strcat(&aName[0], "+ 短時間INT上昇(大)\n");
			break;
		case 33:
			strcat(&aName[0], "+ 一定時間INT上昇(小)\n");
			break;
		case 34:
			strcat(&aName[0], "+ 一定時間INT上昇(中)\n");
			break;
		case 35:
			strcat(&aName[0], "+ 一定時間INT上昇(大)\n");
			break;
			//--------------------------------------------------------------------------------------------
		case 40:
			strcat(&aName[0], "+ 短時間RES上昇(小)\n");
			break;
		case 41:
			strcat(&aName[0], "+ 短時間RES上昇(中)\n");
			break;
		case 42:
			strcat(&aName[0], "+ 短時間RES上昇(大)\n");
			break;
		case 43:
			strcat(&aName[0], "+ 一定時間RES上昇(小)\n");
			break;
		case 44:
			strcat(&aName[0], "+ 一定時間RES上昇(中)\n");
			break;
		case 45:
			strcat(&aName[0], "+ 一定時間RES上昇(大)\n");
			break;
			//--------------------------------------------------------------------------------------------
		case 50:
			strcat(&aName[0], "+ 短時間SPD上昇(小)\n");
			break;
		case 51:
			strcat(&aName[0], "+ 短時間SPD上昇(中)\n");
			break;
		case 52:
			strcat(&aName[0], "+ 短時間SPD上昇(大)\n");
			break;
		case 53:
			strcat(&aName[0], "+ 一定時間SPD上昇(小)\n");
			break;
		case 54:
			strcat(&aName[0], "+ 一定時間SPD上昇(中)\n");
			break;
		case 55:
			strcat(&aName[0], "+ 一定時間SPD上昇(大)\n");
			break;
			//--------------------------------------------------------------------------------------------
		case 60:
			strcat(&aName[0], "+ 短時間LUK上昇(小)\n");
			break;
		case 61:
			strcat(&aName[0], "+ 短時間LUK上昇(中)\n");
			break;
		case 62:
			strcat(&aName[0], "+ 短時間LUK上昇(大)\n");
			break;
		case 63:
			strcat(&aName[0], "+ 一定時間LUK上昇(小)\n");
			break;
		case 64:
			strcat(&aName[0], "+ 一定時間LUK上昇(中)\n");
			break;
		case 65:
			strcat(&aName[0], "+ 一定時間LUK上昇(大)\n");
			break;
			//--------------------------------------------------------------------------------------------
		case 100:
			strcat(&aName[0], "+ HP回復(小)\n");
			break;
		case 101:
			strcat(&aName[0], "+ HP回復(中)\n");
			break;
		case 102:
			strcat(&aName[0], "+ HP回復(大)\n");
			break;
			//--------------------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------------------
		case 1000:
			strcat(&aName[0], "+ スキル攻撃力UP(小)\n");
			break;
		case 1001:
			strcat(&aName[0], "+ スキル攻撃力UP(中)\n");
			break;
		case 1002:
			strcat(&aName[0], "+ スキル攻撃力UP(大)\n");
			break;
			//--------------------------------------------------------------------------------------------
		case 1010:
			strcat(&aName[0], "+ 吸血(小)\n");
			break;
		case 1011:
			strcat(&aName[0], "+ 吸血(中)\n");
			break;
		case 1012:
			strcat(&aName[0], "+ 吸血(大)\n");
			break;
			//--------------------------------------------------------------------------------------------
		case 1020:
			strcat(&aName[0], "+ 捕食(小)\n");
			break;
		case 1021:
			strcat(&aName[0], "+ 捕食(中)\n");
			break;
		case 1022:
			strcat(&aName[0], "+ 捕食(大)\n");
			break;
		}
	}
}
//=============================================================================
// スキルネーム返信処理
//=============================================================================
void ReturnSkillName(char *aName, int nSkill)
{
	int nLength = (int)strlen(&aName[0]);

	strcpy(&aName[nLength], "未設定");

	switch (nSkill)
	{
	case -1:
		strcpy(&aName[nLength], "");
		break;
	case 10:
		strcpy(&aName[nLength], "ダウン");
		break;
	case 24:
		strcpy(&aName[nLength], "超捕食");
		break;
	case 25:
		switch (g_model_Info[0].set.parent[0].type)
		{
		case MODEL_TYPE_PNOR_hip:	strcpy(&aName[nLength], "進化");	break;
		case MODEL_TYPE_PATK_hip:	strcpy(&aName[nLength], "猛攻");	break;
		case MODEL_TYPE_PINT_body3: strcpy(&aName[nLength], "黒雷");	break;
		case MODEL_TYPE_PSPD_hip:	strcpy(&aName[nLength], "連打");	break;
		}
		break;
	case 26:
		strcpy(&aName[nLength], "はたく");
		break;
	case 27:
		strcpy(&aName[nLength], "突進");
		break;
	case 28:
		strcpy(&aName[nLength], "ジャンプ");
		break;
	case 29:
		strcpy(&aName[nLength], "撃ち落とす");
		break;
	case 31:
		strcpy(&aName[nLength], "メテオ");
		break;
	case 32:
		strcpy(&aName[nLength], "放射");
		break;
	case 33:
		strcpy(&aName[nLength], "竜巻");
		break;
	case 34:
		strcpy(&aName[nLength], "爆発");
		break;
	case 35:
		strcpy(&aName[nLength], "自爆");
		break;
	case 36:
		strcpy(&aName[nLength], "ヒール");
		break;
	case 37:
		strcpy(&aName[nLength], "メガヒール");
		break;
	case 38:
		strcpy(&aName[nLength], "ギガヒール");
		break;
	}
}
//=============================================================================
// * モデルセット処理
//=============================================================================
int SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL_USE use, MODEL_SETTING *pSet, CHAR_DATA *data, float fSlength, float fHeight)
{
	int nCount;

	for (nCount = 0; nCount < MAX_MODEL_INFO; nCount++)
	{//用意したモデル枠分回る
		if (g_model_Info[nCount].bUse == false)
		{//使用していないなら
			g_model_Info[nCount].data = data[0];
			g_model_Info[nCount].data.nSkill[0][4] = -1;
			g_model_Info[nCount].data.nSkill[1][4] = -1;
			g_model_Info[nCount].data.plus[0] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
			g_model_Info[nCount].data.plus[1] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

			g_model_Info[nCount].set = pSet[0];
			g_model_Info[nCount].set.nNumMotion[1] = 0;	//ニュートラルに設定
			g_model_Info[nCount].set.nNumMotion[2] = 0;	//次のキーをニュートラルに
			g_model_Info[nCount].set.nNumMotion[3] = -1;	//現在のキーを0に
			g_model_Info[nCount].set.nNumMotion[4] = 0;	//現在のフレームを0に
			g_model_Info[nCount].set.nNumMotion[5] = 0;	//前のモーションを0に
			g_model_Info[nCount].set.nMotionFrame = 0;	//MOTIONの初めは0F！
			g_model_Info[nCount].set.nLeg = 0;			//初めは右足！
			g_model_Info[nCount].set.fSpeed = 0.0f;		//速さ

			g_model_Info[nCount].set.state = MODEL_STATE_NONE;
			g_model_Info[nCount].set.nCntState = 0;
			g_model_Info[nCount].set.nCntHeel = 0;			//回復時間リセット
			g_model_Info[nCount].set.nLockChar = -1;		//lockしているキャラ無し
			g_model_Info[nCount].set.nPlayTime = 0;		//指示の待ち時間初期化
			g_model_Info[nCount].set.nAirTime = 0;		//無敵時間
			g_model_Info[nCount].set.nSkillTime[0] = 0;		//スキル待ち時間
			g_model_Info[nCount].set.nSkillTime[1] = 0;		//スキル待ち時間
			g_model_Info[nCount].use = use;
			g_model_Info[nCount].set.Initpos = pos;
			g_model_Info[nCount].set.pos = pos;
			g_model_Info[nCount].set.posold = pos;
			g_model_Info[nCount].set.fPos = pos.y;
			g_model_Info[nCount].set.move = D3DXVECTOR3(0.0f, -25.0f, 0.0f);
			g_model_Info[nCount].set.rot = rot;
			g_model_Info[nCount].set.angle = rot;
			g_model_Info[nCount].set.fSlength = fSlength;
			g_model_Info[nCount].set.fHeight = fHeight;
			g_model_Info[nCount].set.bJump = false;
			g_model_Info[nCount].set.bStep = false;
			g_model_Info[nCount].bUse = true;

			if (use != MODEL_USE_Pause)
			{//引数で影の設定 オブジェクト生成
				g_model_Info[nCount].nCntShadow = SetShadow(pos, rot, g_model_Info[nCount].set.fSlength, g_model_Info[nCount].set.fSlength, 0.0f, 0);
				if(use == MODEL_USE_PLAYER){ g_model_Info[nCount].nCntObject = SetObject(pos, rot, OBJECT_TYPE_999_PARROWS, OBJECT_USE_MAP, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2); }
				else { g_model_Info[nCount].nCntObject = SetObject(pos, rot, OBJECT_TYPE_999_EARROWS, OBJECT_USE_MAP, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2); }
			}

			g_model_Info[nCount].set.addPosmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ResetMotion(&g_model_Info[nCount].set);

			if (g_model_Info[nCount].use == MODEL_USE_PLAYER)
			{
				Camera *pCamera = GetCamera();
				pCamera[0].targetpos = g_model_Info[nCount].set.pos;
			}
			else if (g_model_Info[nCount].use >= MODEL_USE_ENEMY_Z)
			{
				//マップによって代入

				g_model_Info[nCount].set.nCntChar = SetEnemy(&g_model_Info[nCount].data, g_model_Info[nCount].set.pos, g_model_Info[nCount].set.parent[0].type);
			}
			break;
		}
	}

	return nCount;
}
//=============================================================================
// モーションの更新
//=============================================================================
void BreakModel(int nUse)
{
	ENEMY *pEnemy = GetEnemy();

	if (nUse == 0)
	{//全消去
		for (int nCount = 0; nCount < MAX_MODEL_INFO; nCount++)
		{//用意したモデル枠分回る
				g_model_Info[nCount].bUse = false;
				BreakShadow(g_model_Info[nCount].nCntShadow);
		}
	}
	else if (nUse == 1)
	{//PLAYER以外消去
		for (int nCount = 0; nCount < MAX_MODEL_INFO; nCount++)
		{//用意したモデル枠分回る
			if (g_model_Info[nCount].use != MODEL_USE_PLAYER && g_model_Info[nCount].bUse == true && g_model_Info[nCount].use != MODEL_USE_Pause)
			{
				g_model_Info[nCount].bUse = false;
				BreakShadow(g_model_Info[nCount].nCntShadow);
			}
		}
	}
}
//=============================================================================
// * モデルセット処理
//=============================================================================
int SetEnemy(CHAR_DATA *data, D3DXVECTOR3 pos, MODEL_TYPE type)
{
	ENEMY	*pEnemy = GetEnemy();
	int		nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{//全敵回転
		if (pEnemy[nCntEnemy].bUse == false)
		{//HP番号取得
			pEnemy[nCntEnemy].nCntHPGage[0] = SetZWindow(pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 50, 2.5f, 0, 0, WINDOWSTATE_MAXLIFEGAGE, 0.0f);
			pEnemy[nCntEnemy].nCntHPGage[1] = SetZWindow(pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.51f, 1.0f, 0.0f, 1.0f), 50, 2.5f, 0, 0, WINDOWSTATE_AUTOLIFEGAGE, 0.0f);
			pEnemy[nCntEnemy].nCntHPGage[2] = SetZWindow(pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.51f, 1.0f, 0.0f, 1.0f), 50, 2.5f, 0, 0, WINDOWSTATE_LIFEGAGE, 0.0f);
			pEnemy[nCntEnemy].bUse = true;

			pEnemy[nCntEnemy].nDying = 3;

			data[0].nSkill[0][0] = 27;
			data[0].nSkill[1][0] = 28;
			pEnemy[nCntEnemy].nDSkill[0] = -1;
			pEnemy[nCntEnemy].nDSkill[1] = -1;
			pEnemy[nCntEnemy].nItem = -1;
			pEnemy[nCntEnemy].nDropS = 10000;
			pEnemy[nCntEnemy].nDropI = 10000;

			SetEnemySkill(&data[0], type, nCntEnemy);

			switch (type)
			{
			case MODEL_TYPE_EWarrior_bodyD:	//瀕死あり
			case MODEL_TYPE_EWizard_bodyD:	//瀕死あり
			case MODEL_TYPE_ENinja_bodyD:	//瀕死あり
				pEnemy[nCntEnemy].nDying = 2;
				break;
			}
			break;
		}
	}

	return nCntEnemy;
}
//=============================================================================
// * モデルセット処理
//=============================================================================
void SetEnemySkill(CHAR_DATA *data, MODEL_TYPE type, int nCntEnemy)
{
	PLAYER *pPlayer = GetPlayer();

	switch (pPlayer[0].maptype)
	{
	case MAP_TYPE_100_YEUNG:
		SetEnemySkill_100(&data[0], type, nCntEnemy);
		break;
	case MAP_TYPE_200_CITY:
		SetEnemySkill_200(&data[0], type, nCntEnemy);
		break;
	case MAP_TYPE_300_CASTLE:
		SetEnemySkill_300(&data[0], type, nCntEnemy);
		break;
	case MAP_TYPE_301_CASTLE:
		SetEnemySkill_301(&data[0], type, nCntEnemy);
		break;
	case MAP_TYPE_400_THRONE:
		data[0].nSkill[0][0] = 27;
		data[0].nSkill[1][0] = 28;
		break;
	}
}
//=============================================================================
// * 森 Skill設定処理
//=============================================================================
void SetEnemySkill_100(CHAR_DATA *data, MODEL_TYPE type, int nCntEnemy)
{
	ENEMY	*pEnemy = GetEnemy();

	switch (type)
	{
	case MODEL_TYPE_EWarrior_bodyD:	//戦士
		data[0].nSkill[0][0] = 29;
		data[0].nSkill[1][0] = 29;
		pEnemy[nCntEnemy].nDSkill[0] = 26;
		pEnemy[nCntEnemy].nDSkill[1] = 27;
		break;
	case MODEL_TYPE_EWizard_bodyD:	//魔法使い
		data[0].nSkill[0][0] = 10;
		data[0].nSkill[1][0] = 10;
		pEnemy[nCntEnemy].nDSkill[0] = 10;
		pEnemy[nCntEnemy].nDSkill[1] = 10;
		break;
	case MODEL_TYPE_ENinja_bodyD:	//忍者
		data[0].nSkill[0][0] = 10;
		data[0].nSkill[1][0] = 10;
		pEnemy[nCntEnemy].nDSkill[0] = 10;
		pEnemy[nCntEnemy].nDSkill[1] = 10;
		break;
	}
}
//=============================================================================
// * 城下町 Skill設定処理
//=============================================================================
void SetEnemySkill_200(CHAR_DATA *data, MODEL_TYPE type, int nCntEnemy)
{
	ENEMY	*pEnemy = GetEnemy();
	pEnemy[nCntEnemy].nDropS = 1000;

	switch (type)
	{
	case MODEL_TYPE_EWarrior_bodyD:	//戦士
		data[0].nSkill[0][0] = 30;
		data[0].nSkill[1][0] = 30;
		pEnemy[nCntEnemy].nDSkill[0] = 26;
		pEnemy[nCntEnemy].nDSkill[1] = 27;
		break;
	case MODEL_TYPE_EWizard_bodyD:	//魔法使い
		data[0].nSkill[0][0] = 31;
		data[0].nSkill[1][0] = 31;
		pEnemy[nCntEnemy].nDSkill[0] = 32;
		pEnemy[nCntEnemy].nDSkill[1] = 36;
		break;
	case MODEL_TYPE_ENinja_bodyD:	//忍者
		data[0].nSkill[0][0] = 28;
		data[0].nSkill[1][0] = 28;
		pEnemy[nCntEnemy].nDSkill[0] = 28;
		pEnemy[nCntEnemy].nDSkill[1] = 33;
		break;
	}
}
//=============================================================================
// * 城1F Skill設定処理
//=============================================================================
void SetEnemySkill_300(CHAR_DATA *data, MODEL_TYPE type, int nCntEnemy)
{
	ENEMY	*pEnemy = GetEnemy();

	pEnemy[nCntEnemy].nDropS = 5000;

	switch (type)
	{
	case MODEL_TYPE_EWarrior_bodyD:	//戦士
		data[0].nSkill[0][0] = 31;
		data[0].nSkill[1][0] = 31;
		pEnemy[nCntEnemy].nDSkill[0] = 26;
		pEnemy[nCntEnemy].nDSkill[1] = 27;
		break;
	case MODEL_TYPE_EWizard_bodyD:	//魔法使い
		data[0].nSkill[0][0] = 30;
		data[0].nSkill[1][0] = 30;
		pEnemy[nCntEnemy].nDSkill[0] = 34;
		pEnemy[nCntEnemy].nDSkill[1] = 37;
		break;
	case MODEL_TYPE_ENinja_bodyD:	//忍者
		data[0].nSkill[0][0] = 30;
		data[0].nSkill[1][0] = 30;
		pEnemy[nCntEnemy].nDSkill[0] = 28;
		pEnemy[nCntEnemy].nDSkill[1] = 33;
		break;
	}
}
//=============================================================================
// * 城2F Skill設定処理
//=============================================================================
void SetEnemySkill_301(CHAR_DATA *data, MODEL_TYPE type, int nCntEnemy)
{
	ENEMY	*pEnemy = GetEnemy();

	switch (type)
	{
	case MODEL_TYPE_EWarrior_bodyD:	//戦士
		data[0].nSkill[0][0] = 32;
		data[0].nSkill[1][0] = 32;
		pEnemy[nCntEnemy].nDSkill[0] = 29;
		pEnemy[nCntEnemy].nDSkill[1] = 27;
		break;
	case MODEL_TYPE_EWizard_bodyD:	//魔法使い
		data[0].nSkill[0][0] = 32;
		data[0].nSkill[1][0] = 32;
		pEnemy[nCntEnemy].nDSkill[0] = 31;
		pEnemy[nCntEnemy].nDSkill[1] = 38;
		break;
	case MODEL_TYPE_ENinja_bodyD:	//忍者
		pEnemy[nCntEnemy].nDropS = 30000;
		data[0].nSkill[0][0] = 28;
		data[0].nSkill[1][0] = 28;
		pEnemy[nCntEnemy].nDSkill[0] = 35;
		pEnemy[nCntEnemy].nDSkill[1] = 35;
		break;
	}
}
//=============================================================================
// * モデルセット処理
//=============================================================================
void BreakEnemy(void)
{
	ENEMY	*pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{//全敵回転
			pEnemy[nCntEnemy].bUse = false;
	}
}
//=============================================================================
// * ステータス作成処理
//=============================================================================
CHAR_DATA GetGifted(char *Name)
{
	CHAR_DATA data;
	strcpy(&data.aName[0], &Name[0]);
	data.nExperience = 0;
	data.nLevel = 1;
	data.nNextLevel = (int)(INIT_EXP * powf(EXP_UP, 1));
	
	int nLength;	//名前の長さ
	int nValue;	//値 倍率
	float fConvert;	//値 倍率
	float fValue = 0;

	nLength = (int)strlen(&Name[0]);
	for (int nCntName = 0; nCntName < nLength; nCntName++)
	{
		nValue = (int)Name[nCntName];		//アスキーコードで変換
		if (nValue < 0) { nValue *= -1; }	//プラスの値しか取り込まない
		fConvert = nValue * (nCntName * 0.39f + 1.0f);

		fValue += fConvert;		//値の合計
	}
	for (int nCntSkill = 0; nCntSkill < 2; nCntSkill++)
	{
		for (int nCntName = 0; nCntName < 4; nCntName++)
		{
			data.nSkill[nCntSkill][nCntName] = -1;
		}
	}

	data.nCareer = (int)(fValue + nLength * nLength) % 14;
	data.gifted.fAttack = INIT_ATK * ((int)(fValue * powf(1.37f, 4)) % MAX_Gifted + 100) * 0.01f;
	data.gifted.fInt = INIT_INT * ((int)(fValue * powf(2.41f, 2)) % MAX_Gifted + 100) * 0.01f;
	data.gifted.fDefense = INIT_DFF * ((int)(fValue * powf(1.82f, 3)) % MAX_Gifted + 100) * 0.01f;
	data.gifted.fRes = INIT_RES * ((int)(fValue * powf(3.12f, 1)) % MAX_Gifted + 100) * 0.01f;
	data.gifted.fSpeed = INIT_SPD *  ((int)(fValue + nLength * powf(2.41f, 4)) % MAX_Gifted + 100) * 0.01f;
	data.gifted.fLucky = INIT_LUK *  ((int)(nLength + fValue * powf(1.09f, 3)) % MAX_Gifted + 100) * 0.01f;
	data.gifted.fLifePoint = INIT_HP * ((int)((fValue + nLength) * powf(1.43f, 5)) % MAX_Gifted + 100) * 0.01f;
	data.gifted.fSkillPoint = INIT_SP * ((int)((fValue + nLength) * powf(1.27f, 5)) % MAX_Gifted + 100) * 0.01f;
	if (strcmp(&Name[0], "ベルセレージュ") == 0)
	{
		data.nCareer = 99;
	}
	if (strcmp(&Name[0], "アーサー王") == 0)
	{
		data.nCareer = 100;
	}
	data.plus[0] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	data.plus[1] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	return data;
}
//=============================================================================
// * ステータス取得処理
//=============================================================================
void GetStatus(CHAR_DATA *data, bool bInit)
{
	char aStr[48];
	CHAR_STATUS status = GetCareer(data[0].nCareer, &aStr[0]);
	data[0].status.fAttack = (data[0].gifted.fAttack * (data[0].nLevel + 15) * powf(1.02f, data[0].nLevel * 1.0f)) * status.fAttack + data[0].plus[1].fAttack;
	data[0].status.fInt = (data[0].gifted.fInt * (data[0].nLevel + 15) * powf(1.02f, data[0].nLevel * 1.0f)) * status.fInt + data[0].plus[1].fInt;
	data[0].status.fDefense = (data[0].gifted.fDefense * (data[0].nLevel + 15) * powf(1.02f, data[0].nLevel * 1.0f)) * status.fDefense + data[0].plus[1].fDefense;
	data[0].status.fRes = (data[0].gifted.fRes * (data[0].nLevel + 15) * powf(1.02f, data[0].nLevel * 1.0f)) * status.fRes + data[0].plus[1].fRes;
	data[0].status.fSpeed = (data[0].gifted.fSpeed * (data[0].nLevel + 15) * powf(1.02f, data[0].nLevel * 1.0f)) * status.fSpeed + data[0].plus[1].fSpeed;
	data[0].status.fLucky = (data[0].gifted.fLucky * (data[0].nLevel + 15) * powf(1.02f, data[0].nLevel * 1.0f)) * status.fLucky + 1000.0f + data[0].plus[1].fLucky;
	data[0].status.fLifePoint = (data[0].gifted.fLifePoint * (data[0].nLevel + 15) * powf(1.02f, data[0].nLevel * 1.0f) * status.fLifePoint + 10000.0f) + data[0].plus[1].fLifePoint;
	data[0].status.fSkillPoint = (data[0].gifted.fSkillPoint * (data[0].nLevel + 15) * powf(1.02f, data[0].nLevel * 1.0f)) * status.fSkillPoint + 1000.0f + data[0].plus[1].fSkillPoint;
	data[0].status.fSkillPoint += data[0].status.fInt * 0.65f;

	data[0].nMaxStamina = 300 + ((int)data[0].status.fSpeed / MODEL_SMOVE);
	if (bInit == false)
	{//スタミナの初期化
		data[0].nLifePoint = (int)data[0].status.fLifePoint;
		data[0].nSkillPoint = (int)data[0].status.fSkillPoint;
		data[0].nStamina = data[0].nMaxStamina;
		data[0].nTimeStamina = 0;
	}

	data[0].nStrength = (int)(
		((data[0].status.fAttack - data[0].plus[1].fAttack) * 1.2f) +
		((data[0].status.fDefense - data[0].plus[1].fDefense) * 1.05f) +
		((data[0].status.fInt - data[0].plus[1].fInt) * 1.1f) +
		((data[0].status.fRes - data[0].plus[1].fRes) * 1.05f) +
		((data[0].status.fSpeed - data[0].plus[1].fSpeed) * 1.15f) +
		((data[0].status.fLucky - data[0].plus[1].fLucky) * 1.0f) +
		((data[0].status.fLifePoint - data[0].plus[1].fLifePoint) * 1.1f) +
		((data[0].status.fSkillPoint - data[0].plus[1].fSkillPoint) * 1.05f));

}
//=============================================================================
// * Status表示処理
//=============================================================================
void ShowStatus0(CHAR_DATA data, char *aStr)
{
	wsprintf(&aStr[0], "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d"
		, data.nLevel
		, (int)(data.status.fLifePoint - data.plus[1].fLifePoint)
		, (int)(data.status.fSkillPoint - data.plus[1].fSkillPoint)
		, (int)(data.status.fAttack - data.plus[1].fAttack)
		, (int)(data.status.fDefense - data.plus[1].fDefense)
		, (int)(data.status.fInt - data.plus[1].fInt)
		, (int)(data.status.fRes - data.plus[1].fRes)
		, (int)(data.status.fSpeed - data.plus[1].fSpeed)
		, (int)(data.status.fLucky - data.plus[1].fLucky)
		, data.nStrength);
}
//=============================================================================
// * レベルアップ時Status表示処理
//=============================================================================
void ShowStatus(CHAR_DATA data, CHAR_DATA data0)
{
	char aStr[256];

	wsprintf(&aStr[0], "LV\nHP\nSP\nATk\nDEF\nINT\nRES\nSPD\nLUK\n戦闘力");
	SetFont(RECT{ -395 , 3, 800, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 0.0f }, &aStr[0], 11, 1, DT_LEFT, -1, true, FONTSTATE_FLOW, WINDOW_DRAW_1);

	ShowStatus0(data, &aStr[0]);
	SetFont(RECT{ -285 , 3, 800, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 0.0f }, &aStr[0], 12, 1, DT_LEFT, -1, true, FONTSTATE_FLOW, WINDOW_DRAW_1);

	//wsprintf(&aStr[0], ">>\n>>\n>>\n>>\n>>\n>>\n>>\n>>\n>>\n>>");
	wsprintf(&aStr[0], "+%d\n+%d\n+%d\n+%d\n+%d\n+%d\n+%d\n+%d\n+%d\n+%d"
		, data0.nLevel - data.nLevel
		, (int)(data0.status.fLifePoint - data.status.fLifePoint)
		, (int)(data0.status.fSkillPoint - data.status.fSkillPoint)
		, (int)(data0.status.fAttack - data.status.fAttack)
		, (int)(data0.status.fDefense - data.status.fDefense)
		, (int)(data0.status.fInt - data.status.fInt)
		, (int)(data0.status.fRes - data.status.fRes)
		, (int)(data0.status.fSpeed - data.status.fSpeed)
		, (int)(data0.status.fLucky - data.status.fLucky)
		, data.nStrength - data.nStrength
	);
	SetFont(RECT{ -185 , 3, 800, 720 }, D3DXCOLOR{ 1.0f, 0.95f, 0.08f , 0.0f }, &aStr[0], 13, 1, DT_LEFT, -1, true, FONTSTATE_FLOW, WINDOW_DRAW_1);

	ShowStatus0(data0, &aStr[0]);
	SetFont(RECT{ -85 , 3, 800, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0f , 0.0f }, &aStr[0], 14, 1, DT_LEFT, -1, true, FONTSTATE_FLOW, WINDOW_DRAW_1);
}
//=============================================================================
// * 職業設定処理
//=============================================================================
CHAR_STATUS GetCareer(int nCareer, char *aCareer)
{
	CHAR_STATUS status;

	switch (nCareer)
	{
		//バランス型
	case 0:	//勇者
		strcpy(&aCareer[0], "魔族");
		status.fLifePoint = 1.5f;
		status.fSkillPoint = 1.25f;
		status.fAttack = 1.4f;
		status.fDefense = 1.4f;
		status.fInt = 1.4f;
		status.fRes = 1.4f;
		status.fSpeed = 1.25f;
		status.fLucky = 1.2f;
		break;
	case 1:	//剣士
		strcpy(&aCareer[0], "アンデッド");
		status.fLifePoint = 1.55f;
		status.fSkillPoint = 1.0f;
		status.fAttack = 1.25f;
		status.fDefense = 1.2f;
		status.fInt = 1.2f;
		status.fRes = 1.2f;
		status.fSpeed = 0.95f;
		status.fLucky = 1.0f;
		break;
	case 2:	//武士
		strcpy(&aCareer[0], "ゴースト");
		status.fLifePoint = 0.95f;
		status.fSkillPoint = 1.8f;
		status.fAttack = 0.8f;
		status.fDefense = 1.15f;
		status.fInt = 1.7f;
		status.fRes = 1.75f;
		status.fSpeed = 0.85f;
		status.fLucky = 0.95f;
		break;
	case 3:	//グラディエーター
		strcpy(&aCareer[0], "ヴァンパイア");
		status.fLifePoint = 1.45f;
		status.fSkillPoint = 1.45f;
		status.fAttack = 1.85f;
		status.fDefense = 1.25f;
		status.fInt = 0.6f;
		status.fRes = 0.6f;
		status.fSpeed = 1.7f;
		status.fLucky = 1.15f;
		break;
	case 4:	//バトルマスター
		strcpy(&aCareer[0], "ゴーレム");
		status.fLifePoint = 1.85f;
		status.fSkillPoint = 0.75f;
		status.fAttack = 1.3f;
		status.fDefense = 1.95f;
		status.fInt = 0.8f;
		status.fRes = 1.25f;
		status.fSpeed = 0.6f;
		status.fLucky = 1.0f;
		break;

		//攻撃型
	case 5:	//ダークナイト
		strcpy(&aCareer[0], "ウルフ");
		status.fLifePoint = 1.15f;
		status.fSkillPoint = 1.15f;
		status.fAttack = 1.65f;
		status.fDefense = 0.95f;
		status.fInt = 0.95f;
		status.fRes = 0.95f;
		status.fSpeed = 2.0f;
		status.fLucky = 1.8f;
		break;
	case 6:	//バーサーカー
		strcpy(&aCareer[0], "ワイバーン");
		status.fLifePoint = 1.95f;
		status.fSkillPoint = 1.15f;
		status.fAttack = 1.65f;
		status.fDefense = 1.15f;
		status.fInt = 1.75f;
		status.fRes = 0.85f;
		status.fSpeed = 1.35f;
		status.fLucky = 0.85f;
		break;
	case 7:	//ソードマスター
		strcpy(&aCareer[0], "デーモン");
		status.fLifePoint = 1.55f;
		status.fSkillPoint = 0.65f;
		status.fAttack = 1.75f;
		status.fDefense = 1.15f;
		status.fInt = 1.7f;
		status.fRes = 0.65f;
		status.fSpeed = 1.4f;
		status.fLucky = 0.45f;
		break;
	case 8:	//侍
		strcpy(&aCareer[0], "サキュバス");
		status.fLifePoint = 0.85f;
		status.fSkillPoint = 1.95f;
		status.fAttack = 1.25f;
		status.fDefense = 0.95f;
		status.fInt = 1.65f;
		status.fRes = 1.15f;
		status.fSpeed = 1.35f;
		status.fLucky = 1.55f;
		break;

		//防御型
	case 9:	//重装戦士
		strcpy(&aCareer[0], "デビル");
		status.fLifePoint = 1.35f;
		status.fSkillPoint = 0.95f;
		status.fAttack = 1.35f;
		status.fDefense = 1.35f;
		status.fInt = 1.8f;
		status.fRes = 0.65f;
		status.fSpeed = 1.25f;
		status.fLucky = 0.95f;
		break;
	case 10:	//パラディン
		strcpy(&aCareer[0], "スライム");
		status.fLifePoint = 0.5f;
		status.fSkillPoint = 1.0f;
		status.fAttack = 1.0f;
		status.fDefense = 1.0f;
		status.fInt = 1.95f;
		status.fRes = 1.95f;
		status.fSpeed = 1.0f;
		status.fLucky = 2.95f;
		break;
	case 11:	//騎士
		strcpy(&aCareer[0], "ネクロマンサー");
		status.fLifePoint = 1.8f;
		status.fSkillPoint = 1.0f;
		status.fAttack = 1.25f;
		status.fDefense = 1.55f;
		status.fInt = 1.0f;
		status.fRes = 1.3f;
		status.fSpeed = 0.9f;
		status.fLucky = 1.05f;
		break;
	case 12:	//傭兵
		strcpy(&aCareer[0], "ソウルイーター");
		status.fLifePoint = 1.75f;
		status.fSkillPoint = 0.9f;
		status.fAttack = 1.95f;
		status.fDefense = 0.45f;
		status.fInt = 0.45f;
		status.fRes = 0.35f;
		status.fSpeed = 1.75f;
		status.fLucky = 1.95f;
		break;
	case 13:	//傭兵
		strcpy(&aCareer[0], "スケルトン");
		status.fLifePoint = 0.75f;
		status.fSkillPoint = 0.95f;
		status.fAttack = 1.35f;
		status.fDefense = 1.35f;
		status.fInt = 1.0f;
		status.fRes = 1.4f;
		status.fSpeed = 0.95f;
		status.fLucky = 1.45f;
		break;

	case 99:	//村人
		strcpy(&aCareer[0], "魔王");
		status.fLifePoint = 1.2f;
		status.fSkillPoint = 1.0f;
		status.fAttack = 1.4f;
		status.fDefense = 1.0f;
		status.fInt = 1.0f;
		status.fRes = 1.0f;
		status.fSpeed = 1.0f;
		status.fLucky = 1.0f;
		break;

	case 100:	//剣王
		strcpy(&aCareer[0], "剣王");
		status.fLifePoint = 3.0f;
		status.fSkillPoint = 1.0f;
		status.fAttack = 1.8f;
		status.fDefense = 0.5f;
		status.fInt = 1.4f;
		status.fRes = 1.4f;
		status.fSpeed = 0.7f;
		status.fLucky = 1.2f;
		break;
	}

	return status;
}
//=============================================================================
// * ランニングマン排出処理
//=============================================================================
void SetRunningman(MODEL_SETTING *set, MODEL_TYPE type)
{
	set[0].nMaxModel = 16;
	set[0].parent[0] = MODEL_PARENT{ MODEL_TYPE_RUN_bodyD, -1 };
	set[0].parent[1] = MODEL_PARENT{ MODEL_TYPE_RUN_bodyU, 0 };
	set[0].parent[2] = MODEL_PARENT{ MODEL_TYPE_RUN_head, 1 };
	set[0].parent[3] = MODEL_PARENT{ MODEL_TYPE_RUN_armUR, 1 };
	set[0].parent[4] = MODEL_PARENT{ MODEL_TYPE_RUN_armDR, 3 };
	set[0].parent[5] = MODEL_PARENT{ MODEL_TYPE_RUN_handR, 4 };
	set[0].parent[6] = MODEL_PARENT{ MODEL_TYPE_RUN_armUL, 1 };
	set[0].parent[7] = MODEL_PARENT{ MODEL_TYPE_RUN_armDL, 6 };
	set[0].parent[8] = MODEL_PARENT{ MODEL_TYPE_RUN_handL, 7 };
	set[0].parent[9] = MODEL_PARENT{ MODEL_TYPE_RUN_legUR, 0 };
	set[0].parent[10] = MODEL_PARENT{ MODEL_TYPE_RUN_legDR, 9 };
	set[0].parent[11] = MODEL_PARENT{ MODEL_TYPE_RUN_footR, 10 };
	set[0].parent[12] = MODEL_PARENT{ MODEL_TYPE_RUN_legUL, 0 };
	set[0].parent[13] = MODEL_PARENT{ MODEL_TYPE_RUN_legDL, 12 };
	set[0].parent[14] = MODEL_PARENT{ MODEL_TYPE_RUN_footL, 13 };
	set[0].parent[15] = MODEL_PARENT{ type, 5 };
	set[0].nNumMotion[0] = 4;

	for (int nCount = MODEL_TYPE_RUN_bodyD; nCount <= MODEL_TYPE_RUN_footL; nCount++) { LoadModel(nCount); }
	LoadModel(MODEL_TYPE_RUN_Sword0);
	LoadModel(MODEL_TYPE_RUN_Sword1);
	LoadModel(MODEL_TYPE_RUN_Sword2);
}
//=============================================================================
// * ノーマル型排出処理
//=============================================================================
void Set_P_NOR(MODEL_SETTING *set)
{
	set[0].nMaxModel = 15;
	set[0].parent[0] = MODEL_PARENT{ MODEL_TYPE_PNOR_hip, -1 };
	set[0].parent[1] = MODEL_PARENT{ MODEL_TYPE_PNOR_chest, 0 };
	set[0].parent[2] = MODEL_PARENT{ MODEL_TYPE_PNOR_neck, 1 };
	set[0].parent[3] = MODEL_PARENT{ MODEL_TYPE_PNOR_head, 2 };
	set[0].parent[4] = MODEL_PARENT{ MODEL_TYPE_PNOR_chin, 3 };
	set[0].parent[5] = MODEL_PARENT{ MODEL_TYPE_PNOR_Rhand, 1 };
	set[0].parent[6] = MODEL_PARENT{ MODEL_TYPE_PNOR_Lhand, 1 };
	set[0].parent[7] = MODEL_PARENT{ MODEL_TYPE_PNOR_Rleg, 0 };
	set[0].parent[8] = MODEL_PARENT{ MODEL_TYPE_PNOR_Rknee, 7 };
	set[0].parent[9] = MODEL_PARENT{ MODEL_TYPE_PNOR_Rfoot, 8 };
	set[0].parent[10] = MODEL_PARENT{ MODEL_TYPE_PNOR_Lleg, 0 };
	set[0].parent[11] = MODEL_PARENT{ MODEL_TYPE_PNOR_Lknee, 10 };
	set[0].parent[12] = MODEL_PARENT{ MODEL_TYPE_PNOR_Lfoot, 11 };
	set[0].parent[13] = MODEL_PARENT{ MODEL_TYPE_PNOR_tail1, 0 };
	set[0].parent[14] = MODEL_PARENT{ MODEL_TYPE_PNOR_tail2, 13 };

	for (int nCount = MODEL_TYPE_PNOR_hip; nCount <= MODEL_TYPE_PNOR_tail2; nCount++) { LoadModel(nCount); }

	set[0].nNumMotion[0] = 0;
	set[0].fSlength = 20.0f;
}
//=============================================================================
// * 攻撃型排出処理
//=============================================================================
void Set_P_ATK(MODEL_SETTING *set)
{
	set[0].nMaxModel = 20;
	set[0].parent[0] = MODEL_PARENT{ MODEL_TYPE_PATK_hip, -1 };
	set[0].parent[1] = MODEL_PARENT{ MODEL_TYPE_PATK_body, 0 };
	set[0].parent[2] = MODEL_PARENT{ MODEL_TYPE_PATK_chest, 1 };
	set[0].parent[3] = MODEL_PARENT{ MODEL_TYPE_PATK_neck, 2 };
	set[0].parent[4] = MODEL_PARENT{ MODEL_TYPE_PATK_head, 3 };
	set[0].parent[5] = MODEL_PARENT{ MODEL_TYPE_PATK_chin, 4 };
	set[0].parent[6] = MODEL_PARENT{ MODEL_TYPE_PATK_legR, 0 };
	set[0].parent[7] = MODEL_PARENT{ MODEL_TYPE_PATK_kneeR, 6 };
	set[0].parent[8] = MODEL_PARENT{ MODEL_TYPE_PATK_footR, 7 };
	set[0].parent[9] = MODEL_PARENT{ MODEL_TYPE_PATK_legL, 0 };
	set[0].parent[10] = MODEL_PARENT{ MODEL_TYPE_PATK_kneeL, 9 };
	set[0].parent[11] = MODEL_PARENT{ MODEL_TYPE_PATK_footL, 10 };
	set[0].parent[12] = MODEL_PARENT{ MODEL_TYPE_PATK_shoulderR, 2 };
	set[0].parent[13] = MODEL_PARENT{ MODEL_TYPE_PATK_armR, 12 };
	set[0].parent[14] = MODEL_PARENT{ MODEL_TYPE_PATK_handR, 13 };
	set[0].parent[15] = MODEL_PARENT{ MODEL_TYPE_PATK_shoulderL, 2 };
	set[0].parent[16] = MODEL_PARENT{ MODEL_TYPE_PATK_armL, 15 };
	set[0].parent[17] = MODEL_PARENT{ MODEL_TYPE_PATK_handL, 16 };
	set[0].parent[18] = MODEL_PARENT{ MODEL_TYPE_PATK_tail1, 0 };
	set[0].parent[19] = MODEL_PARENT{ MODEL_TYPE_PATK_tail2, 18 };

	for (int nCount = MODEL_TYPE_PATK_hip; nCount <= MODEL_TYPE_PATK_tail2; nCount++) { LoadModel(nCount); }

	set[0].nNumMotion[0] = 1;
	set[0].fSlength = 40.0f;
}
//=============================================================================
// * 魔法型排出処理
//=============================================================================
void Set_P_INT(MODEL_SETTING *set)
{
	set[0].nMaxModel = 24;
	set[0].parent[0] = MODEL_PARENT{ MODEL_TYPE_PINT_body3, -1 };
	set[0].parent[1] = MODEL_PARENT{ MODEL_TYPE_PINT_body2, 0 };
	set[0].parent[2] = MODEL_PARENT{ MODEL_TYPE_PINT_body1, 1 };
	set[0].parent[3] = MODEL_PARENT{ MODEL_TYPE_PINT_body0, 2 };
	set[0].parent[4] = MODEL_PARENT{ MODEL_TYPE_PINT_neck, 3 };
	set[0].parent[5] = MODEL_PARENT{ MODEL_TYPE_PINT_head, 4 };
	set[0].parent[6] = MODEL_PARENT{ MODEL_TYPE_PINT_chin, 5 };
	set[0].parent[7] = MODEL_PARENT{ MODEL_TYPE_PINT_tail0, 0 };
	set[0].parent[8] = MODEL_PARENT{ MODEL_TYPE_PINT_tail1, 7 };
	set[0].parent[9] = MODEL_PARENT{ MODEL_TYPE_PINT_wingS, 3 };
	set[0].parent[10] = MODEL_PARENT{ MODEL_TYPE_PINT_wingL, 9 };
	set[0].parent[11] = MODEL_PARENT{ MODEL_TYPE_PINT_wingR, 9 };
	set[0].parent[12] = MODEL_PARENT{ MODEL_TYPE_PINT_armLU, 3 };
	set[0].parent[13] = MODEL_PARENT{ MODEL_TYPE_PINT_armLD, 12 };
	set[0].parent[14] = MODEL_PARENT{ MODEL_TYPE_PINT_handL, 13 };
	set[0].parent[15] = MODEL_PARENT{ MODEL_TYPE_PINT_armRU, 3 };
	set[0].parent[16] = MODEL_PARENT{ MODEL_TYPE_PINT_armRD, 15 };
	set[0].parent[17] = MODEL_PARENT{ MODEL_TYPE_PINT_handR, 16 };
	set[0].parent[18] = MODEL_PARENT{ MODEL_TYPE_PINT_legLU, 0 };
	set[0].parent[19] = MODEL_PARENT{ MODEL_TYPE_PINT_legLD, 18 };
	set[0].parent[20] = MODEL_PARENT{ MODEL_TYPE_PINT_footL, 19 };
	set[0].parent[21] = MODEL_PARENT{ MODEL_TYPE_PINT_legRU, 0 };
	set[0].parent[22] = MODEL_PARENT{ MODEL_TYPE_PINT_legRD, 21 };
	set[0].parent[23] = MODEL_PARENT{ MODEL_TYPE_PINT_footR, 22 };

	for (int nCount = MODEL_TYPE_PINT_body3; nCount <= MODEL_TYPE_PINT_footR; nCount++) { LoadModel(nCount); }

	set[0].nNumMotion[0] = 2;
	set[0].fSlength = 40.0f;
}
//=============================================================================
// * 攻撃型排出処理
//=============================================================================
void Set_P_SPD(MODEL_SETTING *set)
{
	set[0].nMaxModel = 20;
	set[0].parent[0] = MODEL_PARENT{ MODEL_TYPE_PSPD_hip, -1 };
	set[0].parent[1] = MODEL_PARENT{ MODEL_TYPE_PSPD_third_body, 0 };
	set[0].parent[2] = MODEL_PARENT{ MODEL_TYPE_PSPD_second_body, 1 };
	set[0].parent[3] = MODEL_PARENT{ MODEL_TYPE_PSPD_chest, 2 };
	set[0].parent[4] = MODEL_PARENT{ MODEL_TYPE_PSPD_head, 3 };
	set[0].parent[5] = MODEL_PARENT{ MODEL_TYPE_PSPD_chin, 4 };
	set[0].parent[6] = MODEL_PARENT{ MODEL_TYPE_PSPD_Rleg, 1 };
	set[0].parent[7] = MODEL_PARENT{ MODEL_TYPE_PSPD_Rknee, 6 };
	set[0].parent[8] = MODEL_PARENT{ MODEL_TYPE_PSPD_Rfoot, 7 };
	set[0].parent[9] = MODEL_PARENT{ MODEL_TYPE_PSPD_Lleg, 1 };
	set[0].parent[10] = MODEL_PARENT{ MODEL_TYPE_PSPD_Lknee, 9 };
	set[0].parent[11] = MODEL_PARENT{ MODEL_TYPE_PSPD_Lfoot, 10 };
	set[0].parent[12] = MODEL_PARENT{ MODEL_TYPE_PSPD_Rshoulder, 3 };
	set[0].parent[13] = MODEL_PARENT{ MODEL_TYPE_PSPD_Rarm, 12 };
	set[0].parent[14] = MODEL_PARENT{ MODEL_TYPE_PSPD_Rhand, 13 };
	set[0].parent[15] = MODEL_PARENT{ MODEL_TYPE_PSPD_Lshoulder, 3 };
	set[0].parent[16] = MODEL_PARENT{ MODEL_TYPE_PSPD_Larm, 15 };
	set[0].parent[17] = MODEL_PARENT{ MODEL_TYPE_PSPD_Lhand, 16 };
	set[0].parent[18] = MODEL_PARENT{ MODEL_TYPE_PSPD_tail1, 0 };
	set[0].parent[19] = MODEL_PARENT{ MODEL_TYPE_PSPD_tail2, 0 };

	for (int nCount = MODEL_TYPE_PSPD_hip; nCount <= MODEL_TYPE_PSPD_tail2; nCount++) { LoadModel(nCount); }

	set[0].nNumMotion[0] = 3;
	set[0].fSlength = 40.0f;
}
//=============================================================================
// * 剣王排出処理
//=============================================================================
void Set_E_KING(MODEL_SETTING *set)
{
	set[0].nMaxModel = 16;
	set[0].parent[0] = MODEL_PARENT{ MODEL_TYPE_EKING_bodyD, -1 };
	set[0].parent[1] = MODEL_PARENT{ MODEL_TYPE_EKING_bodyU, 0 };
	set[0].parent[2] = MODEL_PARENT{ MODEL_TYPE_EKING_head, 1 };
	set[0].parent[3] = MODEL_PARENT{ MODEL_TYPE_EKING_armUR, 1 };
	set[0].parent[4] = MODEL_PARENT{ MODEL_TYPE_EKING_armDR, 3 };
	set[0].parent[5] = MODEL_PARENT{ MODEL_TYPE_EKING_handR, 4 };
	set[0].parent[6] = MODEL_PARENT{ MODEL_TYPE_EKING_armUL, 1 };
	set[0].parent[7] = MODEL_PARENT{ MODEL_TYPE_EKING_armDL, 6 };
	set[0].parent[8] = MODEL_PARENT{ MODEL_TYPE_EKING_handL, 7 };
	set[0].parent[9] = MODEL_PARENT{ MODEL_TYPE_EKING_legUR, 0 };
	set[0].parent[10] = MODEL_PARENT{ MODEL_TYPE_EKING_legDR, 9 };
	set[0].parent[11] = MODEL_PARENT{ MODEL_TYPE_EKING_footR, 10 };
	set[0].parent[12] = MODEL_PARENT{ MODEL_TYPE_EKING_legUL, 0 };
	set[0].parent[13] = MODEL_PARENT{ MODEL_TYPE_EKING_legDL, 12 };
	set[0].parent[14] = MODEL_PARENT{ MODEL_TYPE_EKING_footL, 13 };
	set[0].parent[15] = MODEL_PARENT{ MODEL_TYPE_EKING_Sword, 5 };
	set[0].nNumMotion[0] = 4;
	set[0].fSlength = 20.0f;

	for (int nCount = MODEL_TYPE_EKING_bodyD; nCount <= MODEL_TYPE_EKING_Sword; nCount++) { LoadModel(nCount); }
}
//=============================================================================
// * 戦士排出処理
//=============================================================================
void Set_E_WARRIOR(MODEL_SETTING *set)
{
	set[0].nMaxModel = 16;
	set[0].parent[0] = MODEL_PARENT{ MODEL_TYPE_EWarrior_bodyD, -1 };
	set[0].parent[1] = MODEL_PARENT{ MODEL_TYPE_EWarrior_bodyU, 0 };
	set[0].parent[2] = MODEL_PARENT{ MODEL_TYPE_EWarrior_head, 1 };
	set[0].parent[3] = MODEL_PARENT{ MODEL_TYPE_EWarrior_armUR, 1 };
	set[0].parent[4] = MODEL_PARENT{ MODEL_TYPE_EWarrior_armDR, 3 };
	set[0].parent[5] = MODEL_PARENT{ MODEL_TYPE_EWarrior_handR, 4 };
	set[0].parent[6] = MODEL_PARENT{ MODEL_TYPE_EWarrior_armUL, 1 };
	set[0].parent[7] = MODEL_PARENT{ MODEL_TYPE_EWarrior_armDL, 6 };
	set[0].parent[8] = MODEL_PARENT{ MODEL_TYPE_EWarrior_handL, 7 };
	set[0].parent[9] = MODEL_PARENT{ MODEL_TYPE_EWarrior_legUR, 0 };
	set[0].parent[10] = MODEL_PARENT{ MODEL_TYPE_EWarrior_legDR, 9 };
	set[0].parent[11] = MODEL_PARENT{ MODEL_TYPE_EWarrior_footR, 10 };
	set[0].parent[12] = MODEL_PARENT{ MODEL_TYPE_EWarrior_legUL, 0 };
	set[0].parent[13] = MODEL_PARENT{ MODEL_TYPE_EWarrior_legDL, 12 };
	set[0].parent[14] = MODEL_PARENT{ MODEL_TYPE_EWarrior_footL, 13 };
	set[0].parent[15] = MODEL_PARENT{ MODEL_TYPE_EWarrior_Sword, 5 };
	set[0].nNumMotion[0] = 5;
	set[0].fSlength = 20.0f;

	for (int nCount = MODEL_TYPE_EWarrior_bodyD; nCount <= MODEL_TYPE_EWarrior_Sword; nCount++) { LoadModel(nCount); }
}
//=============================================================================
// * 忍者排出処理
//=============================================================================
void Set_E_NINJA(MODEL_SETTING *set)
{
	set[0].nMaxModel = 17;
	set[0].parent[0] = MODEL_PARENT{ MODEL_TYPE_ENinja_bodyD, -1 };
	set[0].parent[1] = MODEL_PARENT{ MODEL_TYPE_ENinja_bodyU, 0 };
	set[0].parent[2] = MODEL_PARENT{ MODEL_TYPE_ENinja_head, 1 };
	set[0].parent[3] = MODEL_PARENT{ MODEL_TYPE_ENinja_armUR, 1 };
	set[0].parent[4] = MODEL_PARENT{ MODEL_TYPE_ENinja_armDR, 3 };
	set[0].parent[5] = MODEL_PARENT{ MODEL_TYPE_ENinja_handR, 4 };
	set[0].parent[6] = MODEL_PARENT{ MODEL_TYPE_ENinja_armUL, 1 };
	set[0].parent[7] = MODEL_PARENT{ MODEL_TYPE_ENinja_armDL, 6 };
	set[0].parent[8] = MODEL_PARENT{ MODEL_TYPE_ENinja_handL, 7 };
	set[0].parent[9] = MODEL_PARENT{ MODEL_TYPE_ENinja_legUR, 0 };
	set[0].parent[10] = MODEL_PARENT{ MODEL_TYPE_ENinja_legDR, 9 };
	set[0].parent[11] = MODEL_PARENT{ MODEL_TYPE_ENinja_footR, 10 };
	set[0].parent[12] = MODEL_PARENT{ MODEL_TYPE_ENinja_legUL, 0 };
	set[0].parent[13] = MODEL_PARENT{ MODEL_TYPE_ENinja_legDL, 12 };
	set[0].parent[14] = MODEL_PARENT{ MODEL_TYPE_ENinja_footL, 13 };
	set[0].parent[15] = MODEL_PARENT{ MODEL_TYPE_ENinja_Sword1, 5 };
	set[0].parent[16] = MODEL_PARENT{ MODEL_TYPE_ENinja_Sword2, 8 };
	set[0].nNumMotion[0] = 6;
	set[0].fSlength = 20.0f;

	for (int nCount = MODEL_TYPE_ENinja_bodyD; nCount <= MODEL_TYPE_ENinja_Sword2; nCount++) { LoadModel(nCount); }
}
//=============================================================================
// * 魔法使い排出処理
//=============================================================================
void Set_E_WIZARD(MODEL_SETTING *set)
{
	set[0].nMaxModel = 16;
	set[0].parent[0] = MODEL_PARENT{ MODEL_TYPE_EWizard_bodyD, -1 };
	set[0].parent[1] = MODEL_PARENT{ MODEL_TYPE_EWizard_bodyU, 0 };
	set[0].parent[2] = MODEL_PARENT{ MODEL_TYPE_EWizard_head, 1 };
	set[0].parent[3] = MODEL_PARENT{ MODEL_TYPE_EWizard_armUR, 1 };
	set[0].parent[4] = MODEL_PARENT{ MODEL_TYPE_EWizard_armDR, 3 };
	set[0].parent[5] = MODEL_PARENT{ MODEL_TYPE_EWizard_handR, 4 };
	set[0].parent[6] = MODEL_PARENT{ MODEL_TYPE_EWizard_armUL, 1 };
	set[0].parent[7] = MODEL_PARENT{ MODEL_TYPE_EWizard_armDL, 6 };
	set[0].parent[8] = MODEL_PARENT{ MODEL_TYPE_EWizard_handL, 7 };
	set[0].parent[9] = MODEL_PARENT{ MODEL_TYPE_EWizard_legUR, 0 };
	set[0].parent[10] = MODEL_PARENT{ MODEL_TYPE_EWizard_legDR, 9 };
	set[0].parent[11] = MODEL_PARENT{ MODEL_TYPE_EWizard_footR, 10 };
	set[0].parent[12] = MODEL_PARENT{ MODEL_TYPE_EWizard_legUL, 0 };
	set[0].parent[13] = MODEL_PARENT{ MODEL_TYPE_EWizard_legDL, 12 };
	set[0].parent[14] = MODEL_PARENT{ MODEL_TYPE_EWizard_footL, 13 };
	set[0].parent[15] = MODEL_PARENT{ MODEL_TYPE_EWizard_Sword, 5 };
	set[0].nNumMotion[0] = 7;
	set[0].fSlength = 20.0f;

	for (int nCount = MODEL_TYPE_EWizard_bodyD; nCount <= MODEL_TYPE_EWizard_Sword; nCount++) { LoadModel(nCount); }
}
//=============================================================================
// * 戦士排出処理
//=============================================================================
void Set_E_SOLDIERS(MODEL_SETTING *set)
{
	set[0].nMaxModel = 16;
	set[0].parent[0] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_bodyD, -1 };
	set[0].parent[1] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_bodyU, 0 };
	set[0].parent[2] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_head, 1 };
	set[0].parent[3] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_armUR, 1 };
	set[0].parent[4] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_armDR, 3 };
	set[0].parent[5] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_handR, 4 };
	set[0].parent[6] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_armUL, 1 };
	set[0].parent[7] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_armDL, 6 };
	set[0].parent[8] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_handL, 7 };
	set[0].parent[9] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_legUR, 0 };
	set[0].parent[10] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_legDR, 9 };
	set[0].parent[11] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_footR, 10 };
	set[0].parent[12] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_legUL, 0 };
	set[0].parent[13] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_legDL, 12 };
	set[0].parent[14] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_footL, 13 };
	set[0].parent[15] = MODEL_PARENT{ MODEL_TYPE_ESoldiers_Sword, 5 };
	set[0].nNumMotion[0] = 5;
	set[0].fSlength = 20.0f;

	for (int nCount = MODEL_TYPE_ESoldiers_bodyD; nCount <= MODEL_TYPE_ESoldiers_Sword; nCount++) { LoadModel(nCount); }
}
//=============================================================================
// * モデル情報取得処理
//=============================================================================
MODEL_INFO *GetModel_Info(void)
{
	return &g_model_Info[0];
}
//=============================================================================
// * モーション情報取得処理
//=============================================================================
MODEL_MOTION *GetModel_Motion(void)
{
	return &g_model_motion[0][0];
}
//=============================================================================
// * プレイヤー情報取得処理
//=============================================================================
PLAYER *GetPlayer(void)
{
	static PLAYER Player;

	return &Player;
}
//=============================================================================
// * 敵情報取得処理
//=============================================================================
ENEMY *GetEnemy(void)
{
	static ENEMY Enemy[MAX_ENEMY];

	return &Enemy[0];
}
//=============================================================================
// * 敵データ取得処理
//=============================================================================
CHAR_DATA *GetChar_Data(void)
{
	static CHAR_DATA g_Enemy_Data[MAX_ENEMY_DATA];

	return &g_Enemy_Data[0];
}
//=============================================================================
// * 敵データ初期化処理
//=============================================================================
void InitChar_Data(int nLevel)
{
	CHAR_DATA *data = GetChar_Data();

	char aName[MAX_ENEMY_DATA][25];
	strcpy(&aName[0][0], "石田三成");
	strcpy(&aName[1][0], "徳川家康");
	strcpy(&aName[2][0], "真田幸村");
	strcpy(&aName[3][0], "伊達政宗");
	strcpy(&aName[4][0], "武田信玄");
	strcpy(&aName[5][0], "豊臣秀吉");
	strcpy(&aName[6][0], "織田信長");
	strcpy(&aName[7][0], "片倉小十郎");
	strcpy(&aName[8][0], "上杉景勝");
	strcpy(&aName[9][0], "藤堂高虎");
	strcpy(&aName[10][0], "明智光秀");
	strcpy(&aName[11][0], "猿飛佐助");
	strcpy(&aName[12][0], "武田勝頼");
	strcpy(&aName[13][0], "加藤清正");
	strcpy(&aName[14][0], "田中章");
	strcpy(&aName[15][0], "アーサー王");

	for (int nCntData = 0; nCntData < MAX_ENEMY_DATA; nCntData++)
	{
		data[nCntData] = GetGifted(&aName[nCntData][0]);
		data[nCntData].nLevel = nLevel;
		GetStatus(&data[nCntData], false);
		nLevel++;
	}

	//LoadChar_Data(&data[0]);
	SaveChar_Data(data[0]);
}
//=============================================================================
// * 敵データロード処理
//=============================================================================
void LoadChar_Data(CHAR_DATA *data)
{
	FILE *pFile;

	pFile = fopen("data/Enemy.bin", "rb");

	if (pFile != NULL)
	{//Save　成功
		fread(&data[0], sizeof(CHAR_DATA), MAX_ENEMY_DATA, pFile);

		fclose(pFile);
	}
	else
	{//失敗
		MessageBox(0, "敵データ", "ロードに失敗しました", MB_OK);
	}
}
//=============================================================================
// * 敵データセーブ処理
//=============================================================================
void SaveChar_Data(CHAR_DATA Pdata)
{
	//InitChar_Data();	//現在のキャラデータ取得

	CHAR_DATA *data = GetChar_Data();
	//for (int nCntData = 0; nCntData < MAX_ENEMY_DATA - 1; nCntData++)
	//{
	//	data[nCntData] = data[nCntData + 1];
	//}
	//data[MAX_ENEMY_DATA - 1] = Pdata;

	FILE *pFile;
	pFile = fopen("data/Enemy.bin", "wb");

	if (pFile != NULL)
	{//Save　成功
		fwrite(&data[0], sizeof(CHAR_DATA), MAX_ENEMY_DATA, pFile);

		fclose(pFile);
	}
	else
	{//失敗
		MessageBox(0, "敵データ", "セーブに失敗しました", MB_OK);
	}
}
//=============================================================================
// * 敵ステータス補正処理
//=============================================================================
void CorrectionChar_Data(void)
{
	CHAR_DATA *data = GetChar_Data();

	for (int nCntdata = 0; nCntdata < MAX_ENEMY_DATA; nCntdata++)
	{
		if (nCntdata + 1 == MAX_ENEMY_DATA)
		{//ボス補正
			data[nCntdata].status.fLifePoint = (int)data[nCntdata].status.fLifePoint * 4.0f;
			data[nCntdata].status.fAttack = data[nCntdata].status.fAttack * 0.25f;
		}
		else
		{
			data[nCntdata].status.fLifePoint = data[nCntdata].status.fLifePoint * (nCntdata * 0.085f + 0.15f);
			data[nCntdata].status.fAttack = data[nCntdata].status.fAttack * (nCntdata * 0.015f + 0.35f);
		}
		data[nCntdata].status.fDefense = data[nCntdata].status.fDefense * (nCntdata * 0.05f + 0.15f);
		data[nCntdata].status.fSpeed = data[nCntdata].status.fSpeed * (nCntdata * 0.05f + 0.35f);
		data[nCntdata].status.fLucky = data[nCntdata].status.fLucky * (nCntdata * 0.05f + 0.45f);
		data[nCntdata].nStrength *= (int)powf(1.1f, nCntdata * 1.0f);

		data[nCntdata].nLifePoint = (int)data[nCntdata].status.fLifePoint;
		data[nCntdata].nSkillPoint = (int)data[nCntdata].status.fSkillPoint;
	}
}
