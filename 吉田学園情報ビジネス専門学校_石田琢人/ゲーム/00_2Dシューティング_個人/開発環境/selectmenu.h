//=============================================================================
//
// セレクトメニュー処理 [selectmenu.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _SELECTMENU_H_
#define _SELECTMENU_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	SELECTMENUSTATE_CONTINUE = 0,
	SELECTMENUSTATE_RETRY,
	SELECTMENUSTATE_QUIT,
	SELECTMENUSTATE_MAX
}SELECTMENUSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	SELECTMENUSTATE state;
} SELECTMENU;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSelectmenu(void);
void UninitSelectmenu(void);
void UpdateSelectmenu(void);
void DrawSelectmenu(void);
void SetSelectmenu(D3DXVECTOR3 pos, D3DXCOLOR col, SELECTMENUSTATE state);

#endif