//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//出現状態
	PLAYERSTATE_NOMAL,		//通常状態
	PLAYERSTATE_DAMAGE,		//ダメージ状態
	PLAYERSTATE_DEATH,		//死亡状態
	PLAYERSTATE_BEAM,		//ビーム中
	PLAYERSTATE_MAX			//状態の総数
}PLAYERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	PLAYERSTATE state;	//プレイヤーの状態
	int nCounterState;	//状態管理のカウンター
	int nLife;			//プレイヤーの体力
	int stock;			//プレイヤーの残機
	bool bDisp;			//使用しているかどうか
}Player;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(int nDamage);
void MovePlayer(void);

#endif