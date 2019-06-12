//=============================================================================
//
// 敵処理 [enemy.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NONE = 0,
	ENEMYSTATE_NORMAL,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	float fDestAngle;
	ENEMYSTATE state;
	int nCounterState;
	int nLife;
	int nIdxShadow;			// 影の番号
	bool bUse;
}Enemy;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnenmy(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void HitEnemy(int nCntEnemy, int nDamage);
Enemy *GetEnemy(void);
#endif
