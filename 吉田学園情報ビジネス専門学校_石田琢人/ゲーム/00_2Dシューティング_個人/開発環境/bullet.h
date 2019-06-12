//=============================================================================
//
// 弾処理 [bullet.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	BULLETTYPE_PLAYER = 0,		//プレイヤーの弾
	BULLETTYPE_ENEMY,			//敵の弾
	BULLETTYPE_PLAYER_BEAM,		//プレイヤーのビーム
	BULLETTYPE_MAX
}BULLETTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	BULLETTYPE type;	//弾の種類
	D3DXCOLOR col;		//色
	int nLife;			//寿命
	bool bUse;			//使用しているかどうか
} Bullet;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type);

Bullet *GetBullet(void);
#endif
