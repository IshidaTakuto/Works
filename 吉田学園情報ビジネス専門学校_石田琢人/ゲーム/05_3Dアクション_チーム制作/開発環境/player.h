//=============================================================================
//
// モデル処理 [player.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _PLAYERL_H_
#define _PLAYERL_H_

#include "main.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
#define MAX_TEXT_NAME			(255)							//ファイル名の名前の最大数
#define FILE_NAME				"data\\TEXT\\Player1.txt"		//読み込むtxtファイルの名前
#define MAX_PLAYER_PARTS		(16)							//パーツ数
#define MODEL_SPEED				(1.0f)							//プレイヤースピード
#define MAX_PLAYERANIM			(8)								//アニメーション数
#define MAX_PLAYERKEY			(8)								//キーフレーム数

//*****************************************************************************
// モデルの構造体
//*****************************************************************************
typedef enum
{
	TYPE_HUMAN = 0,				//人間
	TYPE_DRAGON,				//ドラゴン
}PlayerType;

typedef enum
{
	PLAYERANIM_NEUTRAL = 0,		//ニュートラルモーション
	PLAYERANIM_WALK,			//歩くモーション
	PLAYERANIM_ATTACK,			//攻撃モーション
	PLALYER_MAX					//モーションの最大数
}PlayerAnim;

typedef struct
{
	char			FileName[MAX_TEXT_NAME];						//Xファイル名
	LPD3DXMESH		pMeshModel = NULL;								//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatModel = NULL;							//マテリアルの情報へのポインタ
	DWORD			nNumMatModel = 0;								//マテリアルの情報数
	D3DXMATRIX		mtxWorldModel;									//ワールドマトリックス
	D3DXVECTOR3		posModel;										//位置
	D3DXVECTOR3		InitPos;										//初期位置
	D3DXVECTOR3		posAnim[MAX_PLAYERANIM][MAX_PLAYERKEY];			//アニメーションpos
	D3DXVECTOR3		rotModel;										//向き
	D3DXVECTOR3		InitRot;										//初期向き
	D3DXVECTOR3		rotAnim[MAX_PLAYERANIM][MAX_PLAYERKEY];			//アニメーションrot
	D3DXVECTOR3		rotDest;										//目的の位置
	int				nIdxModelParent = 0;							//親モデルのインデックス　-1が親
	D3DXVECTOR3		vtxMax, vtxMin;									// 頂点の最大・最小
}PlayerModel;

typedef struct
{

	D3DXMATRIX		mtxWorldPlayer;									//ワールドマトリックス
	LPD3DXMESH		pMeshPlayer = NULL;								//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatPlayer = NULL;							//マテリアルの情報へのポインタ
	DWORD			nNumMatPlayer = 0;								//マテリアルの情報数
	D3DXVECTOR3		posOld;											//過去の位置
	D3DXVECTOR3		pos;											//位置
	D3DXVECTOR3		move;											//移動
	D3DXVECTOR3		rot;											//向き
	int				nMaxPartsNum;									//パーツの最大数
	PlayerModel		aModel[MAX_PLAYER_PARTS];								//[〇]ここにはパーツ数が入る
	int				nMaxAnimNum;									//アニメーションの最大数
	int				nAnimloop[MAX_PLAYERANIM];						//ループ
	int				nAnimKey[MAX_PLAYERANIM];						//キー数
	int				nAnimfram[MAX_PLAYERANIM][MAX_PLAYERKEY];		//フレーム数
	PlayerAnim		nAnimnow;										//現在のアニメーション
	int				nAnimKeynow;									//現在のキー
	bool			bAttack;										//使用しているかどうか
	bool			bUse;											//使用しているかどうか
	bool			bLand;											//捕まっているかどうか
	int				nIdxShadow;									//どの影を使っているか
	bool			bDamage;										// ダメージを受けたか
	int				nCntDamage;										// ダメージ状態のカウンタ
	int				nCntPlayer;
	PlayerType		type;											// 人かドラゴンか
	bool			bLegWhich;										// 最後に前に出した足（false:左　true:右）
}Player;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void DeletePlayer(void);
PlayerType GetPlayerType(void);

bool CollisionAttack(D3DXVECTOR3 pos, float vtxMax);
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange);		// プレイヤーと他のオブジェクト

#endif
