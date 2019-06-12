//*************************************************************
//
// ランキングの処理[result.h]
//  Authot : 佐藤亮太
//
//*************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//=============================================================
// ランキングの構造体
//=============================================================

typedef enum
{
	RESULTSTATE_GAMECLEAR = 0,
	RESULTSTATE_GAMEOVER,
	RESULTSTATE_MAX
}RESULTSTATE;


//=============================================================
//プロトタイプ宣言
//=============================================================
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetRankTime(int nScore);

void SetResultState(RESULTSTATE state);
RESULTSTATE GetResultState(void);
#endif