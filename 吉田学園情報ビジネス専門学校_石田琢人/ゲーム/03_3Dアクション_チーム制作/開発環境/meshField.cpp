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
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		TEXTURE_EFFECT		"data/TEXTURE/1021_brick_s.jpg"
#define		SPLIT_FIELD			(100)		//テクスチャの細かさ
#define		MAX_CNTVERTEX		(800)
#define		MAX_CLS_F			(50)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{//整頓時　セーブの時
	int		nCross;
	int		nVertical;
	float	fLengthX;
	float	fLengthZ;

}Field;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//float					g_fDistortion;						//歪みの量
//float					g_fPercent;							//歪みの使用量

D3DXVECTOR3 vertex[MAX_CNTVERTEX];		//判定用のモデルを模る頂点
Field	collision[MAX_CLS_F];	//判定分情報
int			g_nCntCLS_F;

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshField(char *aMapName)
{
	FILE *pFile;			//ロード
	int nCntSave;
	g_nCntCLS_F = 0;

	if (aMapName != NULL)
	{
		char aMapfllName[48];
		strcpy(&aMapfllName[0], &aMapName[0]);
		//strcat(&aMapfllName[0], "F.bin");


		pFile = fopen(&aMapfllName[0], "rb");

		if (pFile != NULL)
		{//Save　成功
			fread(&g_nCntCLS_F, sizeof(int), 1, pFile);
			fread(&collision[0], sizeof(Field), g_nCntCLS_F, pFile);
			fread(&nCntSave, sizeof(int), 1, pFile);
			fread(&vertex[0], sizeof(D3DXVECTOR3), nCntSave, pFile);

			fclose(pFile);
		}
		else
		{//失敗
			MessageBox(0, "", "セーブに失敗しました", MB_OK);
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
void BreakMeshField(void)
{
	g_nCntCLS_F = 0;
}

//=============================================================================
// 地面のあたり判定
//=============================================================================
int CollisionMeshField(D3DXVECTOR3 *pos, D3DXVECTOR3 *posold, D3DXVECTOR3 *move, float *fPos, bool bJump, MODEL_SETTING *set)
{
	int	nField = 0;
	//float	fRotCross;										//面の角度X
	//float	fRotVertical;									//面の角度Z
	float	fPercentX;
	float	fPercentZ;
	int		nCntCross;
	int		nCntVertical;
	int		nCntVtx;
	int		nCntVtxP = 0;
	D3DXVECTOR3	length = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (nCntVtx = 0; nCntVtx < g_nCntCLS_F; nCntVtx++)
	{
		if (pos[0].x < vertex[nCntVtxP].x && pos[0].x > vertex[nCntVtxP + collision[nCntVtx].nCross].x &&
			pos[0].z > vertex[nCntVtxP].z && pos[0].z < vertex[nCntVtxP + collision[nCntVtx].nVertical * (collision[nCntVtx].nCross + 1)].z)
		{//Fieldの左端から右端、手前から奥で判定
			nCntCross = (int)((vertex[nCntVtxP].x - pos[0].x) / collision[nCntVtx].fLengthX);		//横の頂点の列を算出
			nCntVertical = (int)((pos[0].z - vertex[nCntVtxP].z) / collision[nCntVtx].fLengthZ);	//縦の頂点の列を算出

			length.x = vertex[nCntVtxP + nCntVertical * (collision[nCntVtx].nCross + 1) + nCntCross].x - pos[0].x;	//浸食したXの長さを算出
			length.z = pos[0].z - vertex[nCntVtxP + nCntVertical * (collision[nCntVtx].nCross + 1) + nCntCross].z;	//浸食したZの長さを算出
			fPercentZ = length.z / collision[nCntVtx].fLengthZ;											//真四角ポリゴン内のZの浸食率を算出
			if (collision[nCntVtx].fLengthX * (1.0f - fPercentZ) > length.x)
			{//三角形区分で左上側
				fPercentX = length.x / collision[nCntVtx].fLengthX;
				fPercentZ = length.z / collision[nCntVtx].fLengthZ;

				length.x = fPercentX * (vertex[nCntVtxP + nCntVertical * (collision[nCntVtx].nCross + 1) + nCntCross + 1].y - vertex[nCntVtxP + nCntVertical * (collision[nCntVtx].nCross + 1) + nCntCross].y);
				length.z = fPercentZ * (vertex[nCntVtxP + (nCntVertical + 1) * (collision[nCntVtx].nCross + 1) + nCntCross].y - vertex[nCntVtxP + nCntVertical * (collision[nCntVtx].nCross + 1) + nCntCross].y);
				length.y = vertex[nCntVtxP + nCntVertical * (collision[nCntVtx].nCross + 1) + nCntCross].y + length.x + length.z;

				//MessageBox(0, "", "", MB_OK);
			}
			else
			{//三角形区分で右下側
			 //出た角度と長さをもとにY軸を算出
				length.x = collision[nCntVtx].fLengthX - length.x;	//浸食したXの長さを算出
				length.z = collision[nCntVtx].fLengthZ - length.z;	//浸食したZの長さを算出
				fPercentX = length.x / collision[nCntVtx].fLengthX;
				fPercentZ = length.z / collision[nCntVtx].fLengthZ;

				length.x = fPercentX * (vertex[nCntVtxP + (nCntVertical + 1) * (collision[nCntVtx].nCross + 1) + nCntCross].y - vertex[nCntVtxP + (nCntVertical + 1) * (collision[nCntVtx].nCross + 1) + nCntCross + 1].y);
				length.z = fPercentZ * (vertex[nCntVtxP + nCntVertical * (collision[nCntVtx].nCross + 1) + nCntCross + 1].y - vertex[nCntVtxP + (nCntVertical + 1) * (collision[nCntVtx].nCross + 1) + nCntCross + 1].y);
				length.y = vertex[nCntVtxP + (nCntVertical + 1) * (collision[nCntVtx].nCross + 1) + nCntCross + 1].y + length.x + length.z;
			}
			//wsprintf(&aStr[0], "CROSS %d : VERTICAL %d", nCntCross, nCntVertical);
			//ChangeFont(true, &aStr[0], 0, FONTSTATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			//面の角度によって処理
			if (bJump == true && pos[0].y - length.y < 50.0f &&  pos[0].y - length.y > -50.0f)
			{//地面にいる
				fPos[0] = length.y;
				nField = 1;
			}
			else if (bJump == false)
			{//空中
				if (posold[0].y + 60 >= length.y && pos[0].y <= length.y)
				{//着地
					pos[0].y = length.y;
					fPos[0] = pos[0].y;
					nField = 2;
				}
				//ふぉｒ文一回目なら貫通対処
			}
		}
		nCntVtxP += (collision[nCntVtx].nVertical + 1) * (collision[nCntVtx].nCross + 1);
	}
	if (nField != 0) { move[0].y = 0.0f; }

	return nField;
}
