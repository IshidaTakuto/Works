//=============================================================================
//
// メニュー処理 [menu.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

#include "main.h"
#include "input.h"
#include "game.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	MENUTYPE_GAME,
	MENUTYPE_TUTORIAL,
	MENUTYPE_TITLE,
	MENUTYPE_MAX
}MENUTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nType;			//種類
	MENUTYPE menu;		//状態
} MENU;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMenu(void);
void UninitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);
void SelectMenu(void);
#endif