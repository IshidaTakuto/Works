//=============================================================================
//
// ポリゴン処理 [MeshField.cpp]
// Author : 杉本涼
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <stdio.h>				//インクルドファイル セーブロードのため
#include "input.h"
#include "main.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		TEXTURE_EFFECT		"data/TEXTURE/1207_stone_s.jpg"
#define		BACK_WALL			(100.0f)
#define		Distance			(230.0f)
#define		SPLIT				(8)
#define		MAX_CNTVERTEXW		(8000)
//#define		XZ_LENGTH			(15.0f)
#define		XZ_LENGTH1			(1.0f)
#define		XZ_LENGTH2			(4)
#define		XZ_Range			(800.0f)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
//typedef struct
//{//整頓時　セーブの時
//	int		nCross;
//	int		nVertical;
//
//}GWall;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int						g_nMaxWall;
int						g_nPlus;		//プラスの壁
WallInfo				wallinfo[100];
D3DXVECTOR3				vtxwall[MAX_CNTVERTEXW];

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshBack(char *aMapName, int nPlus)
{
	FILE *pFile;			//ロード
	int nCntSave = 1;
	char aMapfllName[48];
	strcpy(&aMapfllName[0], &aMapName[0]);
	strcat(&aMapfllName[0], "W.bin");
	g_nPlus = nPlus;

	pFile = fopen(&aMapfllName[0], "rb");

	if (pFile != NULL)
	{//Save　成功
		fread(&g_nMaxWall, sizeof(int), 1, pFile);
		fread(&wallinfo[0], sizeof(WallInfo), g_nMaxWall, pFile);
		fread(&nCntSave, sizeof(int), 1, pFile);
		fread(&vtxwall[0], sizeof(D3DXVECTOR3), nCntSave, pFile);

		fclose(pFile);
	}
	else
	{//失敗
		MessageBox(0, "", "セーブに失敗しました", MB_OK);
	}
}
//=============================================================================
// 初期化処理
//=============================================================================
void BreakMeshWall(void)
{//壁をなくす
	g_nMaxWall = 0;
}
//=============================================================================
// 壁のあたり判定
//=============================================================================
void CollisionMeshWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posold, D3DXVECTOR3 *move, float nLength)
{
	if (g_nPlus == 0)
	{
		D3DXVECTOR3 clsPos[5];
		D3DXVECTOR3	length;		//現在の場所から引く値
								//WallInfo *wallinfo = GetWallInfo();
								//D3DXVECTOR3 *vtxwall = GetVtxWall();

		int		nCntVtx;
		int		nCntWall;
		int		nCntCross;
		int		nCntVertical;
		int		nFlip = 0;
		//vtxwall[0] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		//clsPos[0] = vtxwall[2];
		//if (clsPos[0].x == 0.0f) { MessageBox(0, "", "", MB_OK); }

		for (nCntVtx = 0, nCntWall = 0; nCntVtx < g_nMaxWall; nCntVtx++)
		{
			for (nCntVertical = 0; nCntVertical < wallinfo[nCntVtx].nVertical; nCntVertical++)
			{//縦分
				for (nCntCross = 0; nCntCross < wallinfo[nCntVtx].nCross; nCntCross++)
				{//横分
				 //Z軸
					//if (vtxwall[nCntVertical * (wallinfo[nCntVtx].nCross + 1) + nCntCross + nCntWall].x >= vtxwall[nCntVertical * (wallinfo[nCntVtx].nCross + 1) + nCntCross + 1 + nCntWall].x)
					//{
					//	nFlip = 0;
					//}//0番のほうが値が大きい場合
					//else { nFlip = 1; }		//1番のほうが値が大きい場合
					clsPos[0 + nFlip] = vtxwall[nCntVertical * (wallinfo[nCntVtx].nCross + 1) + nCntCross + nFlip + nCntWall];
					clsPos[1 - nFlip] = vtxwall[nCntVertical * (wallinfo[nCntVtx].nCross + 1) + nCntCross + 1 - nFlip + nCntWall];
					clsPos[3 + nFlip] = vtxwall[(nCntVertical + 1) * (wallinfo[nCntVtx].nCross + 1) + nCntCross + nFlip + nCntWall];
					clsPos[4 - nFlip] = vtxwall[(nCntVertical + 1) * (wallinfo[nCntVtx].nCross + 1) + nCntCross + 1 - nFlip + nCntWall];

					//if (move[0].x * move[0].x - move[0].z * move[0].z < 0.2f)
					//{
					//	CollisionVertical(&clsPos[0], &pos[0], &posold[0], &move[0], nCntVertical, nCntCross, nCntVtx, nCntWall);
					//	CollisionCross(&clsPos[0], &pos[0], &posold[0], &move[0], nCntVertical, nCntCross, nCntVtx, nCntWall);
					//}
					if (move[0].x * move[0].x > move[0].z * move[0].z)
					{
						CollisionCross(&clsPos[0], &pos[0], &posold[0], &move[0], nCntVertical, nCntCross, nCntVtx, nCntWall, nLength);
						CollisionVertical(&clsPos[0], &pos[0], &posold[0], &move[0], nCntVertical, nCntCross, nCntVtx, nCntWall, nLength);
					}
					else
					{
						CollisionVertical(&clsPos[0], &pos[0], &posold[0], &move[0], nCntVertical, nCntCross, nCntVtx, nCntWall, nLength);
						CollisionCross(&clsPos[0], &pos[0], &posold[0], &move[0], nCntVertical, nCntCross, nCntVtx, nCntWall, nLength);
					}
				}
			}

			nCntWall += (1 + wallinfo[nCntVtx].nCross) * (1 + wallinfo[nCntVtx].nVertical);
		}
	}
	else
	{
		float fPosX = pos[0].x * pos[0].x;
		float fPosZ = pos[0].z * pos[0].z;
		if (XZ_Range * XZ_Range < fPosX + fPosZ)
		{
			float fRot = atan2f(pos[0].x, pos[0].z);
			pos[0] = D3DXVECTOR3(sinf(fRot) * XZ_Range, pos[0].y, cosf(fRot) * XZ_Range);
		}
	}
}
//=============================================================================
// Xの壁判定
//=============================================================================
void CollisionCross(D3DXVECTOR3 *clsPos, D3DXVECTOR3 *pos, D3DXVECTOR3 *posold, D3DXVECTOR3 *move, int nCntVertical, int nCntCross, int nCntVtx, int nCntWall, float nLength)
{
	float	fPercentX;
	float	fPercentY;
	float	fWidthPos[2];

	if (clsPos[0].z > clsPos[1].z) { fWidthPos[0] = clsPos[0].z; fWidthPos[1] = clsPos[1].z; }
	else { fWidthPos[0] = clsPos[1].z; fWidthPos[1] = clsPos[0].z; }

	if (fWidthPos[0] >= pos[0].z - (nLength / XZ_LENGTH2) && fWidthPos[1] <= pos[0].z + (nLength / XZ_LENGTH2))
	{//X判定 Z軸が範囲内
		fPercentX = clsPos[0].z - pos[0].z;						//浸食したXの長さを算出
		fPercentX = fPercentX / (clsPos[1].z - clsPos[0].z);	//真四角ポリゴン内のXの浸食率を算出

		clsPos[2].x = fPercentX * (clsPos[0].x - clsPos[1].x);	//Z軸の角度に応じたX軸の値を獲得

		if (clsPos[0].y - ((clsPos[1].y - clsPos[0].y) * fPercentX) >= pos[0].y - move[0].y && clsPos[3].y - ((clsPos[1].y - clsPos[0].y) * fPercentX) <= pos[0].y - move[0].y + 5.0f)
		{//Y軸が範囲内
			if (clsPos[0].y - clsPos[3].y == 0.0f) { clsPos[2].y = 0.0f; }
			else
			{//左上と左下でY軸が重なっていないなら　Y軸変化文Z計算
				if (clsPos[0].y < clsPos[1].y) { clsPos[0] = clsPos[1]; }
				fPercentY = clsPos[0].y - (pos[0].y - move[0].y);	//浸食したYの長さを算出
				fPercentY = fPercentY / (clsPos[0].y - clsPos[3].y);	//真四角ポリゴン内のZの浸食率を算出
				clsPos[2].y = fPercentY * (vtxwall[nCntVertical * (wallinfo[nCntVtx].nCross + 1) + nCntCross + nCntWall].x - vtxwall[(nCntVertical + 1) * (wallinfo[nCntVtx].nCross + 1) + nCntCross + nCntWall].x);	//Y軸の角度に応じたZ軸の値を獲得
			}

			//if (vtxwall[nCntCross + 1 + nCntWall].x > vtxwall[nCntCross + nCntWall].x) { fWall = vtxwall[nCntCross + nCntWall].x; }
			//else { fWall = vtxwall[nCntCross + 1 + nCntWall].x; }

			if (posold[0].x > clsPos[0].x + clsPos[2].x - clsPos[2].y)
			{//前回のZ軸が後ろの場合
				if (pos[0].x - nLength - XZ_LENGTH1 <= clsPos[0].x + clsPos[2].x - clsPos[2].y)
				{//現在の位置が前になっている場合
					pos[0].x = (clsPos[0].x + clsPos[2].x - clsPos[2].y + nLength);
					move[0].x = 0.0f;
				}
			}
			else
			{//前回のZ軸が前の場合
				if (pos[0].x + nLength - XZ_LENGTH1 >= clsPos[0].x + clsPos[2].x - clsPos[2].y)
				{//現在の位置が前になっている場合
					pos[0].x = (clsPos[0].x + clsPos[2].x - clsPos[2].y - nLength);
					move[0].x = 0.0f;

				}
			}
		}//Y範囲
	}//X判定
}
//=============================================================================
// Zの壁判定
//=============================================================================
void CollisionVertical(D3DXVECTOR3 *clsPos, D3DXVECTOR3 *pos, D3DXVECTOR3 *posold, D3DXVECTOR3 *move, int nCntVertical, int nCntCross, int nCntVtx, int nCntWall, float nLength)
{
	float	fPercentY;
	float	fPercentZ;

	if (clsPos[0].x + (nLength / XZ_LENGTH2) >= pos[0].x && clsPos[1].x <= pos[0].x - (nLength / XZ_LENGTH2))
	{//Z判定 X軸が範囲内
		fPercentZ = clsPos[0].x - pos[0].x;						//浸食したXの長さを算出
		fPercentZ = fPercentZ / (clsPos[1].x - clsPos[0].x);	//真四角ポリゴン内のZの浸食率を算出

		clsPos[2].z = fPercentZ * (clsPos[0].z - clsPos[1].z);	//X軸の角度に応じたZ軸の値を獲得

		if (clsPos[0].y - ((clsPos[1].y - clsPos[0].y) * fPercentZ) >= pos[0].y - move[0].y && clsPos[3].y - ((clsPos[1].y - clsPos[0].y) * fPercentZ) <= pos[0].y - move[0].y + 5.0f)
		{//Y軸が範囲内
			if (clsPos[0].y - clsPos[3].y == 0.0f) { clsPos[2].y = 0.0f; }
			else
			{//左上と左下でY軸が重なっていないなら　Y軸変化文Z計算
				if (clsPos[0].y < clsPos[1].y) { clsPos[0] = clsPos[1]; }
				fPercentY = clsPos[0].y - pos[0].y;	//浸食したYの長さを算出
				fPercentY = fPercentY / (clsPos[0].y - clsPos[3].y);	//真四角ポリゴン内のZの浸食率を算出
				clsPos[2].y = fPercentY * (vtxwall[nCntVertical * (wallinfo[nCntVtx].nCross + 1) + nCntCross + nCntWall].z - vtxwall[(nCntVertical + 1) * (wallinfo[nCntVtx].nCross + 1) + nCntCross + nCntWall].z);	//Y軸の角度に応じたZ軸の値を獲得
			}

			//if (vtxwall[nCntCross + 1 + nCntWall].z > vtxwall[nCntCross + nCntWall].z) { fWall = vtxwall[nCntCross + nCntWall].z; }
			//else { fWall = vtxwall[nCntCross + 1 + nCntWall].z; }

			if (posold[0].z > clsPos[0].z + clsPos[2].z - clsPos[2].y)
			{//前回のZ軸が後ろの場合
				if (pos[0].z - nLength - XZ_LENGTH1 <= clsPos[0].z + clsPos[2].z - clsPos[2].y)
				{//現在の位置が前になっている場合
					pos[0].z = (clsPos[0].z + clsPos[2].z - clsPos[2].y + nLength);
					move[0].z = 0.0f;

				}
			}
			else
			{//前回のZ軸が前の場合
				if (pos[0].z + nLength - XZ_LENGTH1 >= clsPos[0].z + clsPos[2].z - clsPos[2].y)
				{//現在の位置が前になっている場合
					pos[0].z = (clsPos[0].z + clsPos[2].z - clsPos[2].y - nLength);
					move[0].z = 0.0f;

				}
			}
		}//Y範囲
	}//Z判定
}
