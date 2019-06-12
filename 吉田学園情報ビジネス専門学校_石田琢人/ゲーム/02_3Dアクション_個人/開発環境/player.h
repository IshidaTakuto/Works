//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_NONE = 0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_2D,
	PLAYERSTATE_DAETH,
	PLAYERSTATE_CLEAR,
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nIdxModelParent;	// 親モデルのインデックス
}PlayerModel;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;		// 現在の角度
	D3DXVECTOR3 angle;		// 角度
	D3DXMATRIX mtxWorld;
	PlayerModel aModel[2];	// パーツ数
	PLAYERSTATE state;
	float fSpeed;
	int nCntState;			// ステートのカウンタ
	float fDestAngle;		// 目的の角度
	float fDiffAngle;		// 差分の角度
	int nIdxShadow;			// 影の番号
	int nCntParticle;		// パーティクルの変更
	bool bJump;				// ジャンプをしているか
	D3DXVECTOR3 vtxMin;		// プレイヤーの最小値
	D3DXVECTOR3 vtxMax;		// プレイヤーの最大値
}Player;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

Player *GetPlayer(void);

void PlayerMotion(void);
#endif
