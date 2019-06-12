//=============================================================================
//
// ポーズメニュー処理 [pause.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "input.h"
#include "game.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	CONTINUE,
	RETRY,
	QUIT,
	MAX_PAUSE
}PAUSESTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nType;			//種類
	PAUSESTATE state;	//状態
} PAUSE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SelectPause(void);
#endif