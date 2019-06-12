//=============================================================================
//
// モデル処理 [player.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _DRAGON_H_
#define _DRAGON_H_

#include "main.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
#define MAX_TEXT_NAME			(255)							//ファイル名の名前の最大数
#define FILE_DRAGON_NAME		"data\\TEXT\\Player2.txt"		//読み込むtxtファイルの名前
#define MAX_DRAGON_PARTS		(22)							//パーツ数
#define MODEL_SPEED				(1.0f)							//プレイヤースピード
#define MAX_DRAGONANIM			(8)								//アニメーション数
#define MAX_DRAGONKEY			(30)							//キーフレーム数

//*****************************************************************************
// モデルの構造体
//*****************************************************************************
typedef enum
{
	DRAGONANIM_NEUTRAL = 0,		//ニュートラルモーション
	DRAGONANIM_WALK,			//歩くモーション
	DRAGONANIM_ATTACK,			//攻撃モーション
	DRAGON_MAX					//モーションの最大数
}DragonAnim;

typedef struct
{
	float fRadius;		// 当たり判定の団子の大きさ
	int nNumJudgment;	// 団子の数
}DragonAttack;

typedef struct
{
	char			FileName[MAX_TEXT_NAME];						//Xファイル名
	LPD3DXMESH		pMeshModel = NULL;								//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatModel = NULL;							//マテリアルの情報へのポインタ
	DWORD			nNumMatModel = 0;								//マテリアルの情報数
	D3DXMATRIX		mtxWorldModel;									//ワールドマトリックス
	D3DXVECTOR3		posModel;										//位置
	D3DXVECTOR3		InitPos;										//初期位置
	D3DXVECTOR3		posAnim[MAX_DRAGONANIM][MAX_DRAGONKEY];			//アニメーションpos
	D3DXVECTOR3		rotModel;										//向き
	D3DXVECTOR3		InitRot;										//初期向き
	D3DXVECTOR3		rotAnim[MAX_DRAGONANIM][MAX_DRAGONKEY];			//アニメーションrot
	D3DXVECTOR3		rotDest;										//目的の位置
	int				nIdxModelParent = 0;							//親モデルのインデックス　-1が親
	DragonAttack	playerAttack;									// 当たり判定
}DragonModel;

typedef struct
{

	D3DXMATRIX		mtxWorldDragon;									//ワールドマトリックス
	LPD3DXMESH		pMeshDragon = NULL;								//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatDragon = NULL;							//マテリアルの情報へのポインタ
	DWORD			nNumMatDragon = 0;								//マテリアルの情報数
	D3DXVECTOR3		posOld;											//過去の位置
	D3DXVECTOR3		pos;											//位置
	D3DXVECTOR3		move;											//移動
	D3DXVECTOR3		rot;											//向き
	int				nMaxPartsNum;									//パーツの最大数
	DragonModel		aModel[MAX_DRAGON_PARTS];								//[〇]ここにはパーツ数が入る
	int				nMaxAnimNum;									//アニメーションの最大数
	int				nAnimloop[MAX_DRAGONANIM];						//ループ
	int				nAnimKey[MAX_DRAGONANIM];						//キー数
	int				nAnimfram[MAX_DRAGONANIM][MAX_DRAGONKEY];		//フレーム数
	DragonAnim		nAnimnow;										//現在のアニメーション
	int				nAnimKeynow;									//現在のキー
	bool			bAttack;										//使用しているかどうか
	bool			bUse;											//使用しているかどうか
	bool			bLand;											//捕まっているかどうか
	int				nIdxShadow;									//どの影を使っているか
	int				nCntDragon;
}Dragon;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitDragon(void);
void UninitDragon(void);
void UpdateDragon(void);
void DrawDragon(void);
Dragon *GetDragon(void);
void DeleteDragon(void);
void SetDragon(D3DXVECTOR3 pos);
D3DXVECTOR3 GetPosDragon(void);

D3DXVECTOR3 GetNeckRot(void);

bool CollisionDragonAttack(D3DXVECTOR3 pos, float vtxMax);
bool CollisionDragon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange);		// プレイヤーと他のオブジェクト

#endif
