//=============================================================================
//
// ポーズメニュー処理 [pausemenu.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

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
	MAX_PAUSEMENU
}PAUSEMENUSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nType;			//種類
	PAUSEMENUSTATE pmenu;		//状態
} PAUSEMENU;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPmenu(void);
void UninitPmenu(void);
void UpdatePmenu(void);
void DrawPmenu(void);
void SelectPmenu(void);
#endif