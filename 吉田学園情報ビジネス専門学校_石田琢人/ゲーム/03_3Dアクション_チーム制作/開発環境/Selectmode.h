//=============================================================================
//
// セレクト画面処理 [Selectmode.h]
// Author : ABE YUUTAROU
//
//=============================================================================
#ifndef _SELECTMODE_H_			//	2重インクルード防止のマクロ定義
#define _SELECTMODE_H_

#include "main.h"

//*****************************************************************************
//構造体
//*****************************************************************************
typedef enum
{
	SELECT_NOMAL = 0,		//通常
	SELECT_END,				//END
	SELECT_QUIT,
	SELECT_GAME,
	SELECT_STORY,
	SELECT_SOUND,
	SELECT_MAX
}SELECTSTATE;

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
void InitSelectmode(void);		//初期化処理ライフ外付け
void UninitSelectmode(void);	//終了処理ライフ外付け
void UpdateSelectmode(void);	//更新処理ライフ外付け
void DrawSelectmode(void);		//描画処理ライフ外付け
SELECTSTATE GetSelstate(void);
int GetSelMenu(void);

#endif // !
