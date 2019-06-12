//=============================================================================
//
// スコア処理 [Ranking.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	RANKINGTYPE_NUMBER = 0, //数字
	RANKINGTYPE_CHARACTER,
	RANKINGTYPE_MAX
}RANKINGTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;
	bool bUse;
	int nType;
	int g_aRankingNumber;
	int nRankScore;
}RANKING;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

void SetRanking(int nScore);

#endif
