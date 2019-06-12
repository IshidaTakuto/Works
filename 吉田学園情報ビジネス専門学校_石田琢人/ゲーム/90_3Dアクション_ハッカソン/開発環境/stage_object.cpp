//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// ステージの配置物の処理 [stage_object.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "stage_object.h"
#include "matrixShadow.h"
#include "line.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOAD_STAGE_OBJ_FILE "data/TEXTFILE/Stage_Object.txt"	// 読み込みファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ScanVertexStage_obj(LPDIRECT3DDEVICE9 pDevice);		// 頂点座標の最大値・最小値

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Stage_objInfo_Number g_aStage_objInfo_Number;		// ファイル読み込み情報 [種類,最大数]
Stage_objInfo g_aStage_objInfo[MAX_STAGE_OBJ];		// ファイル読み込み情報 [共通部分]
Stage_obj g_aStage_obj[MAX_STAGE_OBJ];				// オブジェクトの情報

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitStage_obj(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//++++++++++++++++++++
	// ----- 初期化 -----
	//++++++++++++++++++++
	// オブジェクトの情報
	for (int nCntStage_Obj = 0; nCntStage_Obj < MAX_STAGE_OBJ; nCntStage_Obj++)
	{
		g_aStage_obj[nCntStage_Obj].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置の初期設定
		g_aStage_obj[nCntStage_Obj].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きの初期設定
		g_aStage_obj[nCntStage_Obj].fHeight = 0.0f;								// 高さ
		g_aStage_obj[nCntStage_Obj].fRadius = 0.0f;								// 半径
		g_aStage_obj[nCntStage_Obj].nType = 0;									// 種類
#if _DEBUG
		g_aStage_obj[nCntStage_Obj].IdxLine = 0;								// 線
#endif
	}

	//--- ファイル読み込み ---//
	LoadStage_obj();	// オブジェクト情報

	//++++++++++++++++++++++++++++++
	// ----- Xファイル読み込み -----
	//++++++++++++++++++++++++++++++
	for (int nCntStage_Obj = 0; nCntStage_Obj < g_aStage_objInfo_Number.nTypeModel; nCntStage_Obj++)
	{
		D3DXLoadMeshFromX(g_aStage_objInfo[nCntStage_Obj].pXFileName,		// 読み込むXファイル名
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aStage_objInfo[nCntStage_Obj].pBuffMat,		// マテリアル情報ポインタ
			NULL,
			&g_aStage_objInfo[nCntStage_Obj].NumMat,		// マテリアル情報の数
			&g_aStage_objInfo[nCntStage_Obj].pMesh);		// メッシュ情報ポインタ

		//+++++++++++++++++++++++++++++++++++
		// ----- テクスチャの読み込み -----
		//+++++++++++++++++++++++++++++++++++
		// Xファイルから取り出したマテリアル情報を入れる
		D3DXMATERIAL *pMat;

		// Xファイルからマテリアル情報を取得
		pMat = (D3DXMATERIAL*)g_aStage_objInfo[nCntStage_Obj].pBuffMat->GetBufferPointer();

		// テクスチャのポインタをマテリアル情報の数分配列化
		g_aStage_objInfo[nCntStage_Obj].pTexture = new LPDIRECT3DTEXTURE9[(int)g_aStage_objInfo[nCntStage_Obj].NumMat];	// int型に直す

		// 取得したファイル名を入力
		for (int nCntNumMat = 0; nCntNumMat < (int)g_aStage_objInfo[nCntStage_Obj].NumMat; nCntNumMat++)	// DWORD型からint型に変換する
		{
			// テクスチャのポインタ初期化
			g_aStage_objInfo[nCntStage_Obj].pTexture[nCntNumMat] = NULL;

			// テクスチャファイル名が入っている場合、読み込む
			if (pMat[nCntNumMat].pTextureFilename != NULL)
			{
				D3DXCreateTextureFromFile(pDevice, pMat[nCntNumMat].pTextureFilename, &g_aStage_objInfo[nCntStage_Obj].pTexture[nCntNumMat]);
			}
		}
	}

	//++++++++++++++++++++++++++++++++++++++++
	// ----- 頂点座標の最大値・最小値 -----
	//++++++++++++++++++++++++++++++++++++++++
	ScanVertexStage_obj(pDevice);

	// モードを取得
	MODE pMode = GetMode();

	if (pMode == MODE_GAME)
	{
		for (int nCntStage_Obj = 0; nCntStage_Obj < g_aStage_objInfo_Number.nMaxModel; nCntStage_Obj++)
		{
#if _DEBUG
			// 線の設定
			SetColisionLine(g_aStage_obj[nCntStage_Obj].pos, g_aStage_objInfo[g_aStage_obj[nCntStage_Obj].nType].vtxMax, g_aStage_objInfo[g_aStage_obj[nCntStage_Obj].nType].vtxMin);
#endif
		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitStage_obj(void)
{
	//--- 読み込み情報の破棄 ---//
	UnloadStage_obj();	// オブジェクト情報
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdateStage_obj(void)
{
	// モードを取得
	MODE pMode = GetMode();

	if (pMode == MODE_GAME)
	{
		for (int nCntStage_Obj = 0; nCntStage_Obj < g_aStage_objInfo_Number.nMaxModel; nCntStage_Obj++)
		{
#if _DEBUG
			// 線の移動
			SetPositionLine(g_aStage_obj[nCntStage_Obj].IdxLine, g_aStage_obj[nCntStage_Obj].pos);
#endif
		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawStage_obj(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス
	D3DMATERIAL9 matDef;							// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;								// マテリアルへのポインタ

	for (int nCntStage_Obj = 0; nCntStage_Obj < g_aStage_objInfo_Number.nMaxModel; nCntStage_Obj++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aStage_obj[nCntStage_Obj].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_aStage_obj[nCntStage_Obj].rot.y, g_aStage_obj[nCntStage_Obj].rot.x, g_aStage_obj[nCntStage_Obj].rot.z);
		D3DXMatrixMultiply(&g_aStage_obj[nCntStage_Obj].mtxWorld,
			&g_aStage_obj[nCntStage_Obj].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_aStage_obj[nCntStage_Obj].pos.x, g_aStage_obj[nCntStage_Obj].pos.y, g_aStage_obj[nCntStage_Obj].pos.z);
		D3DXMatrixMultiply(&g_aStage_obj[nCntStage_Obj].mtxWorld,
			&g_aStage_obj[nCntStage_Obj].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aStage_obj[nCntStage_Obj].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aStage_objInfo[g_aStage_obj[nCntStage_Obj].nType].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aStage_objInfo[g_aStage_obj[nCntStage_Obj].nType].NumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの反映
			pDevice->SetTexture(0, g_aStage_objInfo[g_aStage_obj[nCntStage_Obj].nType].pTexture[nCntMat]);

			// モデル(パーツ)の描画
			g_aStage_objInfo[g_aStage_obj[nCntStage_Obj].nType].pMesh->DrawSubset(nCntMat);
#if 0
			// 投影処理
			SetmatrixShadow(g_aStage_obj[nCntStage_Obj].mtxWorld, g_aStage_objInfo[g_aStage_obj[nCntStage_Obj].nType].pMesh,
				g_aStage_objInfo[g_aStage_obj[nCntStage_Obj].nType].NumMat, CheckShadowPosition(g_aStage_obj[nCntStage_Obj].pos));
#endif
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 頂点の最大値・最小値
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void ScanVertexStage_obj(LPDIRECT3DDEVICE9 pDevice)
{
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファへのポインタ

	for (int nCntStage_Obj = 0; nCntStage_Obj < g_aStage_objInfo_Number.nTypeModel; nCntStage_Obj++)
	{
		// 頂点数を取得
		nNumVtx = g_aStage_objInfo[nCntStage_Obj].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_aStage_objInfo[nCntStage_Obj].pMesh->GetFVF());

		// 頂点バッファをロック
		g_aStage_objInfo[nCntStage_Obj].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

			// X
			if (g_aStage_objInfo[nCntStage_Obj].vtxMin.x > vtx.x)
			{
				g_aStage_objInfo[nCntStage_Obj].vtxMin.x = vtx.x;
			}
			else if (g_aStage_objInfo[nCntStage_Obj].vtxMax.x < vtx.x)
			{
				g_aStage_objInfo[nCntStage_Obj].vtxMax.x = vtx.x;
			}

			// Z
			if (g_aStage_objInfo[nCntStage_Obj].vtxMin.z > vtx.z)
			{
				g_aStage_objInfo[nCntStage_Obj].vtxMin.z = vtx.z;
			}
			else if (g_aStage_objInfo[nCntStage_Obj].vtxMax.z < vtx.z)
			{
				g_aStage_objInfo[nCntStage_Obj].vtxMax.z = vtx.z;
			}

			// Y
			if (g_aStage_objInfo[nCntStage_Obj].vtxMin.y > vtx.y)
			{
				g_aStage_objInfo[nCntStage_Obj].vtxMin.y = vtx.y;
			}
			else if (g_aStage_objInfo[nCntStage_Obj].vtxMax.y < vtx.y)
			{
				g_aStage_objInfo[nCntStage_Obj].vtxMax.y = vtx.y;
			}

			pVtxBuff += sizeFVF;		// サイズ分ポインタを進める
		}
		// 頂点バッファをアンロック
		g_aStage_objInfo[nCntStage_Obj].pMesh->UnlockVertexBuffer();
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// オブジェクトの当たり判定
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool CollisionStage_obj(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius, float fHeight)
{
	// オブジェクトに乗っているかどうか
	bool bOnTheObj = false;

	for (int nCntFieldStage_obj = 0; nCntFieldStage_obj < g_aStage_objInfo_Number.nMaxModel; nCntFieldStage_obj++)
	{
		/* X - Z */
		if (g_aStage_obj[nCntFieldStage_obj].pos.y + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.y <= pPos->y + fHeight &&
			pPos->y <= g_aStage_obj[nCntFieldStage_obj].pos.y + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.y)
		{// [Y]の範囲内の場合
			if (g_aStage_obj[nCntFieldStage_obj].pos.x + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.x <= pPos->x + fRadius &&
				pPos->x - fRadius <= g_aStage_obj[nCntFieldStage_obj].pos.x + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.x)
			{// [X]の範囲内の場合
				// [Z]の当たり判定
				if (pPosOld->z + fRadius <= g_aStage_obj[nCntFieldStage_obj].pos.z + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.z &&
					pPos->z + fRadius > g_aStage_obj[nCntFieldStage_obj].pos.z + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.z)
				{// Z [-]からぶつかった場合
					pMove->z = 0.0f;
					pPos->z = g_aStage_obj[nCntFieldStage_obj].pos.z + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.z - fRadius;
				}
				else if (pPosOld->z - fRadius >= g_aStage_obj[nCntFieldStage_obj].pos.z + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.z &&
					pPos->z - fRadius < g_aStage_obj[nCntFieldStage_obj].pos.z + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.z)
				{// Z [+]からぶつかった場合
					pMove->z = 0.0f;
					pPos->z = g_aStage_obj[nCntFieldStage_obj].pos.z + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.z + fRadius;
				}
			}
			if (g_aStage_obj[nCntFieldStage_obj].pos.z + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.z <= pPos->z + fRadius &&
				pPos->z - fRadius <= g_aStage_obj[nCntFieldStage_obj].pos.z + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.z)
			{// [Z]の範囲内の場合
				// [X]の当たり判定
				if (pPosOld->x + fRadius <= g_aStage_obj[nCntFieldStage_obj].pos.x + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.x &&
					pPos->x + fRadius > g_aStage_obj[nCntFieldStage_obj].pos.x + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.x)
				{// X [-]からぶつかった場合
					pMove->x = 0.0f;
					pPos->x = g_aStage_obj[nCntFieldStage_obj].pos.x + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.x - fRadius;
				}
				else if (pPosOld->x - fRadius >= g_aStage_obj[nCntFieldStage_obj].pos.x + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.x &&
					pPos->x - fRadius < g_aStage_obj[nCntFieldStage_obj].pos.x + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.x)
				{// X [+]からぶつかった場合
					pMove->x = 0.0f;
					pPos->x = g_aStage_obj[nCntFieldStage_obj].pos.x + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.x + fRadius;
				}
			}

		}
		/* Y */
		if (g_aStage_obj[nCntFieldStage_obj].pos.x + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.x <= pPos->x + fRadius &&
			pPos->x - fRadius <= g_aStage_obj[nCntFieldStage_obj].pos.x + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.x &&
			g_aStage_obj[nCntFieldStage_obj].pos.z + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.z <= pPos->z + fRadius &&
			pPos->z - fRadius < g_aStage_obj[nCntFieldStage_obj].pos.z + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.z)
		{// [X - Z]の範囲内の場合
				// [Y]の当たり判定
			if (pPosOld->y >= g_aStage_obj[nCntFieldStage_obj].pos.y + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.y &&
				pPos->y < g_aStage_obj[nCntFieldStage_obj].pos.y + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.y)
			{// Y [+]からぶつかった場合
				// 着地状態
				bOnTheObj = true;
				pMove->y = 0.0f;
				pPos->y = g_aStage_obj[nCntFieldStage_obj].pos.y + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMax.y;
			}
			else if (pPosOld->y + fHeight <= g_aStage_obj[nCntFieldStage_obj].pos.y + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.y &&
				pPos->y + fHeight > g_aStage_obj[nCntFieldStage_obj].pos.y + g_aStage_objInfo[g_aStage_obj[nCntFieldStage_obj].nType].vtxMin.y)
			{// Y [-]からぶつかった場合
				pMove->y = 0.0f;
			}
		}
	}

	return bOnTheObj;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// オブジェクト読み込み
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void LoadStage_obj(void)
{
	//--- 読み込むオブジェクト情報の初期化 ---//
	// ファイル読み込み情報 [種類,最大数] の初期化
	g_aStage_objInfo_Number.nTypeModel = 0;		// 読み込むモデルの最大数
	g_aStage_objInfo_Number.nMaxModel = 0;		// 表示するモデルの最大数
	// ファイル読み込み情報 [共通部分]
	for (int nCntStage_ObjInfo = 0; nCntStage_ObjInfo < MAX_STAGE_OBJINFO; nCntStage_ObjInfo++)
	{
		for (int nText = 0; nText < MAX_CHAR; nText++)
		{
			g_aStage_objInfo[nCntStage_ObjInfo].pXFileName[nText] = NULL;
		}
		g_aStage_objInfo[nCntStage_ObjInfo].pMesh = NULL;
		g_aStage_objInfo[nCntStage_ObjInfo].pBuffMat = NULL;
		g_aStage_objInfo[nCntStage_ObjInfo].NumMat = 0;
		g_aStage_objInfo[nCntStage_ObjInfo].pTexture = NULL;
		g_aStage_objInfo[nCntStage_ObjInfo].vtxMin = D3DXVECTOR3(1000000.0f, 1000000.0f, 1000000.0f);
		g_aStage_objInfo[nCntStage_ObjInfo].vtxMax = D3DXVECTOR3(-1000000.0f, -1000000.0f, -1000000.0f);
	}

	// ローカル変数宣言
	FILE *pFile;				// ファイルのポインタ
	char ReadText[256];			// 読み込んだ文字列を入れておく
	char HeadText[256];			// 比較用
	char DustBox[256];			// 使用しないものを入れておく
	int nCntText = 0;

	// ファイルオープン
	pFile = fopen(LOAD_STAGE_OBJ_FILE, "r");		// ファイルを開く[読み込み型]

	if (pFile != NULL)		// ファイルが空ではない
	{//ファイルが開けた場合
		while (strcmp(HeadText, "SCRIPT") != 0)
		{// "SCRIPT" が読み込まれるまで繰り返し文字列を読み取る
			fgets(ReadText, sizeof(ReadText), pFile);
			sscanf(ReadText, "%s", HeadText);
		}

		if (strcmp(HeadText, "SCRIPT") == 0)
		{// "SCRIPT" が読み取れた場合、処理開始
			while (strcmp(HeadText, "END_SCRIPT") != 0)
			{// "END_SCRIPT" が読み込まれるまで繰り返し文字列を読み取る
				fgets(ReadText, sizeof(ReadText), pFile);
				sscanf(ReadText, "%s", HeadText);

				if (strcmp(HeadText, "\n") == 0)
				{// 文4字列の先頭が [\n](改行) の場合処理しない
					int nCnt = 0;
				}
				else if (strcmp(HeadText, "NUM_MODEL") == 0)
				{// 読み込むモデルの最大数
					sscanf(ReadText, "%s %c %d", &DustBox, &DustBox, &g_aStage_objInfo_Number.nTypeModel);
				}
				else if (strcmp(HeadText, "MODEL_FILENAME") == 0)
				{// モデルファイル名
					if (g_aStage_objInfo_Number.nTypeModel > nCntText)
					{// 最大数以上処理しない
						sscanf(ReadText, "%s %c %s", &DustBox, &DustBox, &g_aStage_objInfo[nCntText].pXFileName[0]);
						nCntText++;		// 次の場所に進める
					}
				}
				else if (strcmp(HeadText, "MODELSET") == 0)
				{// 配置情報
					while (strcmp(HeadText, "END_MODELSET") != 0)
					{// "END_MODELSET" が読み取れるまで繰り返し文字列を読み取る
						fgets(ReadText, sizeof(ReadText), pFile);
						sscanf(ReadText, "%s", HeadText);

						if (strcmp(HeadText, "TYPE") == 0)
						{// 種類
							sscanf(ReadText, "%s %c %d", &DustBox, &DustBox, &g_aStage_obj[g_aStage_objInfo_Number.nMaxModel].nType);
						}
						else if (strcmp(HeadText, "POS") == 0)
						{// 位置
							sscanf(ReadText, "%s %c %f %f %f", &DustBox, &DustBox, &g_aStage_obj[g_aStage_objInfo_Number.nMaxModel].pos.x, &g_aStage_obj[g_aStage_objInfo_Number.nMaxModel].pos.y, &g_aStage_obj[g_aStage_objInfo_Number.nMaxModel].pos.z);
						}
						else if (strcmp(HeadText, "ROT") == 0)
						{// 向き
							sscanf(ReadText, "%s %c %f %f %f", &DustBox, &DustBox, &g_aStage_obj[g_aStage_objInfo_Number.nMaxModel].rot.x, &g_aStage_obj[g_aStage_objInfo_Number.nMaxModel].rot.y, &g_aStage_obj[g_aStage_objInfo_Number.nMaxModel].rot.z);
						}
						else if (strcmp(HeadText, "HEIGHT") == 0)
						{// 高さ
							sscanf(ReadText, "%s %c %f", &DustBox, &DustBox, &g_aStage_obj[g_aStage_objInfo_Number.nMaxModel].fHeight);
						}
						else if (strcmp(HeadText, "RADIUS") == 0)
						{// 半径
							sscanf(ReadText, "%s %c %f", &DustBox, &DustBox, &g_aStage_obj[g_aStage_objInfo_Number.nMaxModel].fRadius);
						}
					}
					g_aStage_objInfo_Number.nMaxModel++;
				}
			}
			// ファイルクローズ
			fclose(pFile);
		}
	}
	else
	{// 開けなかった場合
		MessageBox(NULL, "ステージ配置情報ファイルが開けませんでした。", NULL, MB_OK);
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// オブジェクト破棄
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UnloadStage_obj(void)
{
	//--- 読み込んだオブジェクト情報の破棄 ---//
	// ファイル読み込み情報 [種類,最大数]
	g_aStage_objInfo_Number.nTypeModel = 0;		// 読み込むモデルの最大数
	g_aStage_objInfo_Number.nMaxModel = 0;		// 表示するモデルの最大数
	// ファイル読み込み情報 [共通部分]
	for (int nCntStage_ObjInfo = 0; nCntStage_ObjInfo < MAX_STAGE_OBJINFO; nCntStage_ObjInfo++)
	{
		for (int nText = 0; nText < MAX_CHAR; nText++)
		{
			if (g_aStage_objInfo[nCntStage_ObjInfo].pXFileName[nText] != NULL)
			{
				g_aStage_objInfo[nCntStage_ObjInfo].pXFileName[nText] = NULL;
			}
		}
		if (g_aStage_objInfo[nCntStage_ObjInfo].pMesh != NULL)
		{// メッシュ情報へのポインタ
			g_aStage_objInfo[nCntStage_ObjInfo].pMesh->Release();
			g_aStage_objInfo[nCntStage_ObjInfo].pMesh = NULL;
		}
		if (g_aStage_objInfo[nCntStage_ObjInfo].pBuffMat != NULL)
		{// マテリアル情報へのポインタ
			g_aStage_objInfo[nCntStage_ObjInfo].pBuffMat->Release();
			g_aStage_objInfo[nCntStage_ObjInfo].pBuffMat = NULL;
		}
		for (int nCntMat = 0; nCntMat < (int)g_aStage_objInfo[nCntStage_ObjInfo].NumMat; nCntMat++)
		{// マテリアルの数
			if (g_aStage_objInfo[nCntStage_ObjInfo].pTexture[nCntMat] != NULL)
			{// テクスチャへのポインタ
				g_aStage_objInfo[nCntStage_ObjInfo].pTexture[nCntMat]->Release();
				g_aStage_objInfo[nCntStage_ObjInfo].pTexture[nCntMat] = NULL;
			}
		}
		g_aStage_objInfo[nCntStage_ObjInfo].NumMat = 0;
		g_aStage_objInfo[nCntStage_ObjInfo].vtxMin = D3DXVECTOR3(1000000.0f, 1000000.0f, 1000000.0f);
		g_aStage_objInfo[nCntStage_ObjInfo].vtxMax = D3DXVECTOR3(-1000000.0f, -1000000.0f, -1000000.0f);
	}

}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// オブジェクトの取得
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Stage_obj *GetStage_obj(void)
{
	return &g_aStage_obj[0];
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// オブジェクトの取得
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Stage_objInfo *GetStage_objInfo(void)
{
	return &g_aStage_objInfo[0];
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// オブジェクトの総数の取得
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int GetNumStage_obj(void)
{
	return g_aStage_objInfo_Number.nMaxModel;
}