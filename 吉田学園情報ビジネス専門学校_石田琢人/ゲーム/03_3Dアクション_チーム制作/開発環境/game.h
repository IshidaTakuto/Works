#pragma once
//=========================================================================================================//
//
// ポリゴン：プロトタイプ宣言 [Scan.h]
// Author : Ryo Sugimoto
//
//=========================================================================================================//
#ifndef _GAME_H_
#define _GAME_H_

#include"model.h"
#include"main.h"

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define	MAX_MAP_CHANGE	(3)			//マップ出入り口の数

#define SPEECH_FLOW	(5)		//会話で1文字がでる時間
#define SPEECH_STOP	(120)	//会話で文字が全て出てから次の会話に移るまでのF

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,
	GAMESTATE_START,		//エンター待ち
	GAMESTATE_NORMAL,
	GAMESTATE_NORMAL1,		//キャラステ確認
	GAMESTATE_END,
	GAMESTATE_PAUSE

}GAMESTATE;

typedef struct
{
	GAMESTATE now;		//現在の状態
	MODE CHANGE;		//変えるモード
	int nCntGameState;	//カウント用

}GAME;

typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_INVINCBLE,		//無敵状態
	PLAYERSTATE_DNORMAL,		//しゃがみ後ジャンプが一回しかできないのを防ぐ
	PLAYERSTATE_FALL,			//すり抜け状態
	PLAYERSTATE_ATTACK,			//攻撃状態　連打中
	PLAYERSTATE_SATTACK,		//空中攻撃状態　連打中
	PLAYERSTATE_SKYATTACK,		//攻撃状態への移行　回転中
	PLAYERSTATE_WAIT,			//攻撃状態への待機　回転後
	PLAYERSTATE_CATCH,			//崖つかまり状態
	PLAYERSTATE_CLIMBR,			//崖を上っている状態
	PLAYERSTATE_CLIMBL,			//崖を上っている状態
	PLAYERSTATE_BACK,			//ノックバック状態
	PLAYERSTATE_DOWN,			//しゃがみ状態
	PLAYERSTATE_KNOCKBACK,		//ノックバック状態
	PLAYERSTATE_DAMAGE,			//ダメージ状態

}PLAYERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動値の残り火
	D3DXCOLOR col;			// 色
	float fSteep;			//角度
	float fSpeed;
	float fRadius;			// 半径(大きさ)
	bool bUse;				// 使用しているかどうか

} EFFECT;

typedef enum
{
	ANIMTYPE_COIN = 0,	//コイン
	ANIMTYPE_CHECK0,		//復活地点更新
	ANIMTYPE_CHECK1,		//使用済み復活地点
	ANIMTYPE_POWERUP,		//パワーアップItem
	ANIMTYPE_HEEL,			//回復
	ANIMTYPE_ALLHEEL,		//全回復
	ANIMTYPE_CLOCK,			//時計
	ANIMTYPE_BOARD,		//看板
	ANIMTYPE_MESSAGE,	//看板から出るメッセージ
	ANIMTYPE_NONE,		//スクロールしないもの

}ANIMTYPE;
//typedef enum
//{//アニメーションの使用状態
//	ANIMATION_NORMAL = 0,
//	ANIMATION_LOOP,
//	ANIMATION_PLAYER,			//プレイヤーのパターンに応じて変更
//	ANIMATION_MAX,
//
//}ANIMATION;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 initpos;//初期位置
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//位置
	D3DXVECTOR3 col;	//色
	int nLengthX;		//Xの長さ
	int nLengthY;		//Yの長さ
	int nCntAnim;		//Animationのカウント
	int nPtnAnim;		//指定された座標で表示
	int nPtnHeight;		//画像の縦の分割数
	int nPtnWide;		//画像の横の分割数
	int nPtnSpeed;		//Animation速度
	int nType;			//画像の指定
	int nCntState;
	ANIMATION anim;		//アニメーションの使い方
	ANIMTYPE type;	//種類
	bool bUse;
}Animation;

//*****************************************************************************
// 影の構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 rot;				//向き
	D3DXMATRIX	mtxWorldShadow;		//ワールドマトリックス
	float		fLengthX;			//長さX
	float		fLengthZ;			//長さY
	int			nType;
	bool		bUse;				//使用中 or 待機中

}Shadow;

//*****************************************************************************
// 影の構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 rot;				//向き
	D3DXCOLOR col;	//色
	D3DXMATRIX	mtxWorldShadow;		//ワールドマトリックス
	float		fLengthX;			//長さX
	float		fLengthZ;			//長さY
	int			nType;
	bool		bUse;				//使用中 or 待機中

}ZPolygon;

//*****************************************************************************
// ビルボードの構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posBillBoord;		//位置
	D3DXVECTOR3 rotBillBoord;		//向き
	D3DXMATRIX mtxWorldBillBoord;	//ワールドマトリックス
	WINDOWTYPE	ViewType;			//表示方法
	int			nTexType;			//画像の種類
	float		fLengthX;			//長さX
	float		fLengthY;			//長さY
	bool bUse;						//使用してるかどうか

}BillBoord;

typedef struct
{
	D3DXVECTOR3 Entrance;		//入口
	int			nNumber;		//出現の番号
	int			nLengthX;		//範囲X
	int			nLengthZ;		//範囲Z
	bool		bUse;			//使用してるかどうか

}ENEMY_APPEAR;

//*********************************************************************************************************//
// プロトタイプ宣言　Game
//*********************************************************************************************************//
void InitGame(int nFadeType);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void InitGame0(void);
GAME *GetGameState(void);
void SetGameState(GAMESTATE game, MODE mode, int nCntGameState);
void ButtonKeybord(void);
void ButtonJoypad(void);

//*****************************************************************************
// プロトタイプ宣言　地面
//*****************************************************************************
void InitMeshField(char *aMapName);
void BreakMeshField(void);
int CollisionMeshField(D3DXVECTOR3 *pos, D3DXVECTOR3 *posold, D3DXVECTOR3 *move, float *fPos, bool bJump, MODEL_SETTING *set);

//*****************************************************************************
// プロトタイプ宣言　壁
//*****************************************************************************
void InitMeshBack(char *aMapName, int nPlus);
void BreakMeshWall(void);
void CollisionMeshWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posold, D3DXVECTOR3 *move, float nLength);
void CollisionVertical(D3DXVECTOR3 *clsPos, D3DXVECTOR3 *pos, D3DXVECTOR3 *posold, D3DXVECTOR3 *move, int nCntVertical, int nCntCross, int nCntVtx, int nCntWall, float nLength);
void CollisionCross(D3DXVECTOR3 *clsPos, D3DXVECTOR3 *pos, D3DXVECTOR3 *posold, D3DXVECTOR3 *move, int nCntVertical, int nCntCross, int nCntVtx, int nCntWall, float nLength);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLengthX, float fLengthZ, float fHeight, int nShadow);
void SettingShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLengthX, float fLengthZ, int nNumber);
void BreakShadow(int nNumber);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);
int SetPolygon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLengthX, float fLengthZ, int nType);
void SettingPolygon(D3DXVECTOR3 pos);
void BreakPolygon(int nNumber);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void UpdateBullet(void);
void UpdateEBullet(int nCntBullet);
void UpdateABullet(int nCntBullet, int nEnemy, float fAtk);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR3 blown, MODEL_USE use, float fAttack, float fAtkplus, float fRange,
	float fSlip, float fLife, int nCntAtk, int nInvincible, int nType, int nAtkType, int nCntStop, int nCntModel);
void BreakBullet(void);
#endif