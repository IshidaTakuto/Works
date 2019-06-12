//=============================================================================
//
//ゴブリン処理 [GOBURIN.cpp]
// Author : ABE YUUTAROU
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "goburin.h"
#include "camera.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GOBURIN_FILENAME_INPUT	"data/TEXTFILE/goburinnmotion.txt"			// ロードするファイルの名前

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetMotionGOBURIN(void);
void MotionGOBURIN(void);
void InputGOBURIN(void);					
void InitCol(int nCntModel);		

//*****************************************************************************
// グローバル変数
//*****************************************************************************
GOBURIN		g_GOBURIN;								//ゴブリンの情報
int MotionAB = 0;
//=============================================================================
// 初期化処理
//=============================================================================
void InitGOBURIN(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_GOBURIN = {};

	// ロード処理
	InputGOBURIN();

	for (int nCntModel = 0; nCntModel < g_GOBURIN.nNumModel; nCntModel++)
	{
		
		// Xファイルの読み込み
		D3DXLoadMeshFromX(&g_GOBURIN.aGOBURINModel[nCntModel].aName[0]
			, D3DXMESH_SYSTEMMEM
			, pDevice
			, NULL
			, &g_GOBURIN.aGOBURINModel[nCntModel].pBuffMat
			, NULL
			, &g_GOBURIN.aGOBURINModel[nCntModel].nNumMat
			, &g_GOBURIN.aGOBURINModel[nCntModel].pMesh);
		InitCol(nCntModel);
	}

	// 情報の初期化
	g_GOBURIN.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_GOBURIN.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_GOBURIN.rotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	for (int nCntModel = 0; nCntModel < g_GOBURIN.nNumModel; nCntModel++)
	{
		g_GOBURIN.aGOBURINModel[nCntModel].rot = g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].aGOBURINKey[0].aRot[nCntModel];
	}
	g_GOBURIN.bDisp = true;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGOBURIN(void)
{
	if (g_GOBURIN.aGOBURINModel != NULL)
	{
		for (int nCntModel = 0; nCntModel < g_GOBURIN.nNumModel; nCntModel++)
		{
			// メッシュの開放
			if (g_GOBURIN.aGOBURINModel[nCntModel].pMesh != NULL)
			{
				g_GOBURIN.aGOBURINModel[nCntModel].pMesh->Release();
				g_GOBURIN.aGOBURINModel[nCntModel].pMesh = NULL;
			}

			// マテリアルの開放
			if (g_GOBURIN.aGOBURINModel[nCntModel].pBuffMat != NULL)
			{
				g_GOBURIN.aGOBURINModel[nCntModel].pBuffMat->Release();
				g_GOBURIN.aGOBURINModel[nCntModel].pBuffMat = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGOBURIN(void)
{
	MODE g_mode;
	PLAYER *pPlayer = GetPlayer();
	g_mode = *GetMode();

	switch (g_mode)
	{
	case MODE_SELCT:
		if (pPlayer[0].bButton[BUTTON_S] == true || pPlayer[0].bButton[BUTTON_W] == true)
		{
			g_GOBURIN.bMove = true;
			MotionAB = 0;
		}
		else
		{
			g_GOBURIN.bMove = false;
		}
			break;
	case MODE_STORY:
		if (pPlayer[0].bButton[BUTTON_ENTER] == true)
		{
			g_GOBURIN.bMove = true;
				MotionAB = 0;
		}
		else
		{
			g_GOBURIN.bMove = false;
		}
		
		break;
	case MODE_ENDSTORY:
		if (pPlayer[0].bButton[BUTTON_ENTER] == true)
		{
			g_GOBURIN.bMove = true;
			MotionAB = 0;
		}
		else
		{
			g_GOBURIN.bMove = false;
		}

		break;
	}
// 目的の向きの修正
		if (g_GOBURIN.rotDest.y < -D3DX_PI)
		{
			g_GOBURIN.rotDest.y += D3DX_PI * 2.0f;
		}
		else if (g_GOBURIN.rotDest.y > D3DX_PI)
		{
			g_GOBURIN.rotDest.y -= D3DX_PI * 2.0f;
		}

	SetMotionGOBURIN();
	MotionGOBURIN();
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGOBURIN(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_GOBURIN.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_GOBURIN.rot.y, g_GOBURIN.rot.x, g_GOBURIN.rot.z);
	D3DXMatrixMultiply(&g_GOBURIN.mtxWorld, &g_GOBURIN.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_GOBURIN.pos.x, g_GOBURIN.pos.y, g_GOBURIN.pos.z);
	D3DXMatrixMultiply(&g_GOBURIN.mtxWorld, &g_GOBURIN.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_GOBURIN.mtxWorld);

	for (int nCntModel = 0; nCntModel < g_GOBURIN.nNumModel; nCntModel++)
	{
		if (g_GOBURIN.aGOBURINModel[nCntModel].nIdxModelParent == -1)
		{// 親が設定されていない場合
			mtxParent = g_GOBURIN.mtxWorld;
		}
		else
		{// 親が設定されている場合
			mtxParent = g_GOBURIN.aGOBURINModel[g_GOBURIN.aGOBURINModel[nCntModel].nIdxModelParent].mtxWorld;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_GOBURIN.aGOBURINModel[nCntModel].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_GOBURIN.aGOBURINModel[nCntModel].rot.y, g_GOBURIN.aGOBURINModel[nCntModel].rot.x, g_GOBURIN.aGOBURINModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_GOBURIN.aGOBURINModel[nCntModel].mtxWorld, &g_GOBURIN.aGOBURINModel[nCntModel].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_GOBURIN.aGOBURINModel[nCntModel].pos.x, g_GOBURIN.aGOBURINModel[nCntModel].pos.y, g_GOBURIN.aGOBURINModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_GOBURIN.aGOBURINModel[nCntModel].mtxWorld, &g_GOBURIN.aGOBURINModel[nCntModel].mtxWorld, &mtxTrans);
		D3DXMatrixMultiply(&g_GOBURIN.aGOBURINModel[nCntModel].mtxWorld, &g_GOBURIN.aGOBURINModel[nCntModel].mtxWorld, &mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_GOBURIN.aGOBURINModel[nCntModel].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		for (int nCntMat = 0; nCntMat < (int)g_GOBURIN.aGOBURINModel[nCntModel].nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&g_GOBURIN.aGOBURINModel[nCntModel].pCol[nCntMat]);

			// テクスチャの設定
			pDevice->SetTexture(0, g_GOBURIN.aGOBURINModel[nCntModel].pTex[nCntMat]);

			if (g_GOBURIN.bDisp == true)
			{
				// モデルの描画
				g_GOBURIN.aGOBURINModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}
//=============================================================================
//カラーの初期化
//=============================================================================
void InitCol(int nCntModel)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ
												// マテリアルの設定
	pMat = (D3DXMATERIAL*)g_GOBURIN.aGOBURINModel[nCntModel].pBuffMat->GetBufferPointer();
	g_GOBURIN.aGOBURINModel[nCntModel].pCol = new D3DMATERIAL9[(int)g_GOBURIN.aGOBURINModel[nCntModel].nNumMat];
	g_GOBURIN.aGOBURINModel[nCntModel].pColDef = new D3DMATERIAL9[(int)g_GOBURIN.aGOBURINModel[nCntModel].nNumMat];
	g_GOBURIN.aGOBURINModel[nCntModel].pTex = new LPDIRECT3DTEXTURE9[(int)g_GOBURIN.aGOBURINModel[nCntModel].nNumMat];

	for (int nCntMat = 0; nCntMat < (int)g_GOBURIN.aGOBURINModel[nCntModel].nNumMat; nCntMat++)
	{
		// デフォルトの色の保持
		g_GOBURIN.aGOBURINModel[nCntModel].pColDef[nCntMat] = pMat[nCntMat].MatD3D;
		g_GOBURIN.aGOBURINModel[nCntModel].pCol[nCntMat] = g_GOBURIN.aGOBURINModel[nCntModel].pColDef[nCntMat];

		// テクスチャの設定
		g_GOBURIN.aGOBURINModel[nCntModel].pTex[nCntMat] = NULL;
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャがある場合
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_GOBURIN.aGOBURINModel[nCntModel].pTex[nCntMat]);
		}
	}
}


//=============================================================================
//ゴブリンのモーションの設定
//=============================================================================
void SetMotionGOBURIN(void)
{
	GOBURINMOTIONTYPE motionType = g_GOBURIN.motionType;
	PLAYER *pPlayer = GetPlayer();
	MODE g_mode;
	g_mode = *GetMode();
	switch (g_GOBURIN.motionType)
	{
	case GOBURINMOTIONTYPE_NEUTRAL:

		switch (g_mode)
		{
		case MODE_SELCT:
			if (g_GOBURIN.bMove == true && pPlayer[0].bButton[BUTTON_S] == true || pPlayer[0].bButton[BUTTON_W] == true)
			{
				motionType = GOBURINMOTIONTYPE_MOVE;
			}
			break;
		case MODE_STORY:
			if (g_GOBURIN.bMove == true && pPlayer[0].bButton[BUTTON_ENTER] == true)
			{
				motionType = GOBURINMOTIONTYPE_MOVE;
			}
			break;
		case MODE_ENDSTORY:
			if (g_GOBURIN.bMove == true && pPlayer[0].bButton[BUTTON_ENTER] == true)
			{
				motionType = GOBURINMOTIONTYPE_MOVE;
			}
			break;
		}
		
		break;
	case GOBURINMOTIONTYPE_MOVE	:
	
			switch (g_mode)
			{
			case MODE_SELCT:
				if (g_GOBURIN.bMove == false && pPlayer[0].bButton[BUTTON_S] == false || pPlayer[0].bButton[BUTTON_W] == false)
				{
					MotionAB++;
					if (MotionAB >= 98)
					{//ニュートラルモーションに戻す
						motionType = GOBURINMOTIONTYPE_NEUTRAL;
					}
				}
				break;
			case MODE_STORY:
				if (g_GOBURIN.bMove == false && pPlayer[0].bButton[BUTTON_ENTER] == false)
				{
					MotionAB++;
					if (MotionAB >= 120)
					{//ニュートラルモーションに戻す
						motionType = GOBURINMOTIONTYPE_NEUTRAL;
					}
				}
			case MODE_ENDSTORY:
				if (g_GOBURIN.bMove == false && pPlayer[0].bButton[BUTTON_ENTER] == false)
				{
					MotionAB++;
					if (MotionAB >= 120)
					{//ニュートラルモーションに戻す
						motionType = GOBURINMOTIONTYPE_NEUTRAL;
					}
				}
				break;
			}
		break;
	}


	if (g_GOBURIN.motionType != motionType)
	{// 現在のモーションと違った場合
		g_GOBURIN.nKey = 0;
		g_GOBURIN.motionType = motionType;
		for (int nCntModel = 0; nCntModel < g_GOBURIN.nNumModel; nCntModel++)
		{
			g_GOBURIN.aGOBURINModel[nCntModel].pos = g_GOBURIN.aGOBURINModel[nCntModel].posFarst;
			g_GOBURIN.aGOBURINModel[nCntModel].rot = g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].aGOBURINKey[0].aRot[nCntModel];
		}
		g_GOBURIN.nCntFrame = 0;
		g_GOBURIN.nMaxCntFrame = 0;
	}
}

//=============================================================================
// モーション処理
//=============================================================================
void MotionGOBURIN(void)
{
	if (g_GOBURIN.nCntFrame == g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].aGOBURINKey[g_GOBURIN.nKey].nNumFrame)
	{// 一定フレーム経過した場合[キー更新]
		if (g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].nLoop == false)
		{// ループしない場合
			if (g_GOBURIN.nKey < g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].nNumKey)
			{// キー数を越えていない場合
				g_GOBURIN.nKey++;
			}
		}
		else
		{// ループする場合
			g_GOBURIN.nKey = (g_GOBURIN.nKey + 1) % g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].nNumKey;
			if (g_GOBURIN.nKey == 0)
			{// 最初のキーに戻った場合
				g_GOBURIN.nMaxCntFrame = 0;
			}
		}
		g_GOBURIN.nCntFrame = 0;
	}

	// モーション更新
	if (g_GOBURIN.nKey < g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].nNumKey)
	{// キー数を越えていない場合
		D3DXVECTOR3 angle;
		for (int nCntModel = 0; nCntModel < g_GOBURIN.nNumModel; nCntModel++)
		{
			g_GOBURIN.aGOBURINModel[nCntModel].pos += (g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].aGOBURINKey[(g_GOBURIN.nKey + 1) % g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].nNumKey].aPos[nCntModel] - g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].aGOBURINKey[g_GOBURIN.nKey].aPos[nCntModel]) / (float)g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].aGOBURINKey[g_GOBURIN.nKey].nNumFrame;
			angle = g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].aGOBURINKey[(g_GOBURIN.nKey + 1) % g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].nNumKey].aRot[nCntModel] - g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].aGOBURINKey[g_GOBURIN.nKey].aRot[nCntModel];
			if (angle.x  < -D3DX_PI)
			{
				angle.x += D3DX_PI * 2.0f;
			}
			else if (angle.x > D3DX_PI)
			{
				angle.x -= D3DX_PI * 2.0f;
			}
			if (angle.y  < -D3DX_PI)
			{
				angle.y += D3DX_PI * 2.0f;
			}
			else if (angle.y > D3DX_PI)
			{
				angle.y -= D3DX_PI * 2.0f;
			}
			if (angle.z  < -D3DX_PI)
			{
				angle.z += D3DX_PI * 2.0f;
			}
			else if (angle.z > D3DX_PI)
			{
				angle.z -= D3DX_PI * 2.0f;
			}

			g_GOBURIN.aGOBURINModel[nCntModel].rot += angle / (float)g_GOBURIN.aGOBURINMotion[g_GOBURIN.motionType].aGOBURINKey[g_GOBURIN.nKey].nNumFrame;
			if (g_GOBURIN.aGOBURINModel[nCntModel].rot.x  < -D3DX_PI)
			{
				g_GOBURIN.aGOBURINModel[nCntModel].rot.x += D3DX_PI * 2.0f;
			}
			else if (g_GOBURIN.aGOBURINModel[nCntModel].rot.x > D3DX_PI)
			{
				g_GOBURIN.aGOBURINModel[nCntModel].rot.x -= D3DX_PI * 2.0f;
			}
			if (g_GOBURIN.aGOBURINModel[nCntModel].rot.y  < -D3DX_PI)
			{
				g_GOBURIN.aGOBURINModel[nCntModel].rot.y += D3DX_PI * 2.0f;
			}
			else if (g_GOBURIN.aGOBURINModel[nCntModel].rot.y > D3DX_PI)
			{
				g_GOBURIN.aGOBURINModel[nCntModel].rot.y -= D3DX_PI * 2.0f;
			}
			if (g_GOBURIN.aGOBURINModel[nCntModel].rot.z  < -D3DX_PI)
			{
				g_GOBURIN.aGOBURINModel[nCntModel].rot.z += D3DX_PI * 2.0f;
			}
			else if (g_GOBURIN.aGOBURINModel[nCntModel].rot.z > D3DX_PI)
			{
				g_GOBURIN.aGOBURINModel[nCntModel].rot.z -= D3DX_PI * 2.0f;
			}
		}
	}

	g_GOBURIN.nCntFrame++;			//経過フレームの加算
	g_GOBURIN.nMaxCntFrame++;		//総合経過フレームの加算
}

//=============================================================================
// ロード処理
//=============================================================================
void InputGOBURIN(void)
{
	FILE *pFile;			//アドレス代入リード　*オープン*
	char read[128];			//読み込む文字の確保
	int nCntModel = 0, nCntMotion = 0, nCntKey;		//ゴブリンモデルカウント//ゴブリンモーションカウント////ゴブリンキーカウント

	pFile = fopen(GOBURIN_FILENAME_INPUT, "r");		//”ｒ”ファイルの読み取りができる
	if (pFile != NULL)
	{// ファイルが開けた場合
		do{
			fscanf(pFile, "%s", &read[0]);
			if (strcmp(&read[0], "NUM_MODEL") == 0)	//書き込んだのが右と同じなら==0にする//書き込んだのがNUM_MODELの場合
			{//モデルの数の読み込み設定
				fscanf(pFile, "%s%d", &read[0], &g_GOBURIN.nNumModel);	//文字(NUM_MODEL)とモデルの数
				g_GOBURIN.aGOBURINModel = new GOBURINModel[g_GOBURIN.nNumModel];	//モデルの情報を指定して配列に初期化する
			}
			else if (strcmp(&read[0], "MODEL_FILENAME") == 0 && nCntModel < g_GOBURIN.nNumModel)	//書き込んだのがMODEL_FILENAMEの場合
			{//モデルのファイㇽ名
			 //ここでMODEL_FILENAMEがｘファイルを読み込む
				fscanf(pFile, "%s%s", &read[0], &g_GOBURIN.aGOBURINModel[nCntModel].aName[0]);
				nCntModel++;	//これをしないとパーツ分読み込めない
			}
			else if (strcmp(&read[0], "CHARACTERSET") == 0)	//書き込んだのがCHARACTERSETの場合
			{//キャラクターの設定開始
				nCntModel = 0;//初期化
				do{
					fscanf(pFile, "%s", &read[0]);
					if (strcmp(&read[0], "PARTSSET") == 0 && nCntModel < g_GOBURIN.nNumModel)
					{//キャラクターのパーツの設定開始
						do{
							fscanf(pFile, "%s", &read[0]);
							if (strcmp(&read[0], "PARENT") == 0)	//書き込んだのがPARENTの場合
							{//親の設定
								fscanf(pFile, "%s%d", &read[0], &g_GOBURIN.aGOBURINModel[nCntModel].nIdxModelParent);	//親のインデックス取得
							}
							else if (strcmp(&read[0], "POS") == 0)	//書き込んだのがPOSの場合
							{//位置の設定
								fscanf(pFile, "%s%f%f%f", &read[0], &g_GOBURIN.aGOBURINModel[nCntModel].pos.x, &g_GOBURIN.aGOBURINModel[nCntModel].pos.y, &g_GOBURIN.aGOBURINModel[nCntModel].pos.z);
								g_GOBURIN.aGOBURINModel[nCntModel].posFarst = g_GOBURIN.aGOBURINModel[nCntModel].pos;
							}
							else if (strcmp(&read[0], "ROT") == 0)	//書き込んだのがROTの場合
							{//向きの設定
								fscanf(pFile, "%s%f%f%f", &read[0], &g_GOBURIN.aGOBURINModel[nCntModel].rot.x, &g_GOBURIN.aGOBURINModel[nCntModel].rot.y, &g_GOBURIN.aGOBURINModel[nCntModel].rot.z);
							}
						} while (strcmp(&read[0], "END_PARTSSET") != 0);	//パーツの設定終了
						nCntModel++;
					}
				} while (strcmp(&read[0], "END_CHARACTERSET") != 0);	//キャラクターの設定終了
			}
			else if (strcmp(&read[0], "MOTIONSET") == 0 && nCntMotion < GOBURINMOTIONTYPE_MAX)
			{
				nCntKey = 0;
				do{
					fscanf(pFile, "%s", &read[0]);
					if (strcmp(&read[0], "LOOP") == 0)	//ループの設定確認
					{//ループのなし[0]あり[1]
						fscanf(pFile, "%s%d", &read[0], &g_GOBURIN.aGOBURINMotion[nCntMotion].nLoop);
					}
					else if (strcmp(&read[0], "NUM_KEY") == 0)	//キーの最大数の設定
					{
						fscanf(pFile, "%s%d", &read[0], &g_GOBURIN.aGOBURINMotion[nCntMotion].nNumKey);
						g_GOBURIN.aGOBURINMotion[nCntMotion].aGOBURINKey = new GOBURINKey[g_GOBURIN.aGOBURINMotion[nCntMotion].nNumKey];
					}
					else if (strcmp(&read[0], "KEYSET") == 0 && nCntKey < g_GOBURIN.aGOBURINMotion[nCntMotion].nNumKey)
					{//キーの設定を行う
						nCntModel = 0;
						g_GOBURIN.aGOBURINMotion[nCntMotion].aGOBURINKey[nCntKey].aPos = new D3DXVECTOR3[g_GOBURIN.nNumModel];
						g_GOBURIN.aGOBURINMotion[nCntMotion].aGOBURINKey[nCntKey].aRot = new D3DXVECTOR3[g_GOBURIN.nNumModel];
						do{
							fscanf(pFile, "%s", &read[0]);
							if (strcmp(&read[0], "#") == 0)
							{
								do{
									fscanf(pFile, "%c", &read[0]);
								} while (strcmp(&read[0], "\n") != 0);
							}
							else if (strcmp(&read[0], "FRAME") == 0)
							{//各フレームの取得
								fscanf(pFile, "%s%d", &read[0], &g_GOBURIN.aGOBURINMotion[nCntMotion].aGOBURINKey[nCntKey].nNumFrame);
							}
							else if (strcmp(&read[0], "KEY") == 0 && nCntModel < g_GOBURIN.nNumModel)
							{//位置と向きの設定
								do{
									fscanf(pFile, "%s", &read[0]);
									if (strcmp(&read[0], "POS") == 0)
									{//位置の設定
										fscanf(pFile, "%s%f%f%f", &read[0], &g_GOBURIN.aGOBURINMotion[nCntMotion].aGOBURINKey[nCntKey].aPos[nCntModel].x, &g_GOBURIN.aGOBURINMotion[nCntMotion].aGOBURINKey[nCntKey].aPos[nCntModel].y, &g_GOBURIN.aGOBURINMotion[nCntMotion].aGOBURINKey[nCntKey].aPos[nCntModel].z);
									}
									else if (strcmp(&read[0], "ROT") == 0)
									{// 向きの設定
										fscanf(pFile, "%s%f%f%f", &read[0], &g_GOBURIN.aGOBURINMotion[nCntMotion].aGOBURINKey[nCntKey].aRot[nCntModel].x, &g_GOBURIN.aGOBURINMotion[nCntMotion].aGOBURINKey[nCntKey].aRot[nCntModel].y, &g_GOBURIN.aGOBURINMotion[nCntMotion].aGOBURINKey[nCntKey].aRot[nCntModel].z);
									}
								} while (strcmp(&read[0], "END_KEY") != 0);	//キーの設定終了
								nCntModel++;
							}
						} while (strcmp(&read[0], "END_KEYSET") != 0); // キーセット設定終了
						nCntKey++;
					}
				} while (strcmp(&read[0], "END_MOTIONSET") != 0);	//モーションオ設定
				nCntMotion++;
			}
		} while (strcmp(&read[0], "END_SCRIPT") != 0);	//スクリプト終了//
		fclose(pFile);
	}
}


//=============================================================================
//ゴブリンの取得
//=============================================================================
GOBURIN *GetGOBURIN(void)
{
	return &g_GOBURIN;
}
