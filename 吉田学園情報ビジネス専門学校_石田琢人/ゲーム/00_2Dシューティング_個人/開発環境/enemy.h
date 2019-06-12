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
// マクロ定義
//*****************************************************************************
#define	MAX_ENEMY	(128)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NOMAL = 0,	//通常状態
	ENEMYSTATE_DAMAGE,		//ダメージ状態
	ENEMYSTATE_MAX			//敵の状態の総数
}ENEMYSTATE;

typedef struct
{
	int nCounterAnim;	//アニメーションカウンター
	int nPatternAnim;	//アニメーションパターンNo.
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動
	int nType;			//種類
	ENEMYSTATE state;	//敵の状態
	int nCounterState;	//状態管理のカウンター
	int nLife;			//敵の体力
	int nPoint;			//敵の撃破ポイント
	bool bUse;			//使用しているかどうか
}Enemy;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);

#endif
