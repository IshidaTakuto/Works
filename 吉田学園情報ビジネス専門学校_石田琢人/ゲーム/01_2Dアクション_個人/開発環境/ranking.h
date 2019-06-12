//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	int nType;
	int nScore;
	D3DXCOLOR col;
}RANKING;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

void SetRankScore(int nScore);

#endif
