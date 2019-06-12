//=============================================================================
//
// サメの処理 [destroyer.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _DESTROYER_H_
#define _DESTROYER_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	DESTROYERSTATE_NONE = 0,
	DESTROYERSTATE_NORMAL,
	DESTROYERSTATE_MOVE,
	DESTROYERSTATE_MAX
}DESTROYERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	DESTROYERSTATE state;
}DESTROYER;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitDestroyer(void);
void UninitDestroyer(void);
void UpdateDestroyer(void);
void DrawDestroyer(void);

bool CollisionDestroyer(D3DXVECTOR3 *pPos, float pHeight, float pWidth);
DESTROYER GetDestroyer(void);

#endif