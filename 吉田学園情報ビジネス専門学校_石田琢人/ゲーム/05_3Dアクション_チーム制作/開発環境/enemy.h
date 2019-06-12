//=============================================================================
//
// 敵の処理 [enemy.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
#define MAX_TEXT_NAME			(255)							//ファイル名の名前の最大数
#define TEXT_NAME				"data\\TEXT\\Enemy1.txt"		//読み込むtxtファイルの名前
#define MAX_PARTS				(15)							//パーツ数
#define MODEL_SPEED				(1.0f)							//プレイヤースピード
#define MAX_ENEMYANIM			(8)								//アニメーション数
#define MAX_ENEMYKEY			(15)								//キーフレーム数

//*****************************************************************************
// モデルの構造体
//*****************************************************************************
typedef enum
{
	ENEMYANIM_NEUTRAL = 0,		//ニュートラルモーション
	ENEMYANIM_WALK,				//歩くモーション
	ENEMYANIM_ATTACK,			//攻撃モーション
	ENEMYANIM_DAMAGE,			// ダメージモーション
	ENEMY_MAX					//モーションの最大数
}EnemyAnim;

typedef struct
{
	char			FileName[MAX_TEXT_NAME];						//Xファイル名
	LPD3DXMESH		pMeshModel = NULL;								//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatModel = NULL;							//マテリアルの情報へのポインタ
	DWORD			nNumMatModel = 0;								//マテリアルの情報数
	D3DXMATRIX		mtxWorldModel;									//ワールドマトリックス
	D3DXVECTOR3		posModel;										//位置
	D3DXVECTOR3		InitPos;										//初期位置
	D3DXVECTOR3		posAnim[MAX_ENEMYANIM][MAX_ENEMYKEY];			//アニメーションpos
	D3DXVECTOR3		rotModel;										//向き
	D3DXVECTOR3		InitRot;										//初期向き
	D3DXVECTOR3		rotAnim[MAX_ENEMYANIM][MAX_ENEMYKEY];			//アニメーションrot
	D3DXVECTOR3		rotDest;										//目的の位置
	int				nIdxModelParent = 0;							//親モデルのインデックス　-1が親
}EnemyModel;

typedef struct
{

	D3DXMATRIX		mtxWorldEnemy;									//ワールドマトリックス
	LPD3DXMESH		pMeshEnemy = NULL;								//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatEnemy = NULL;							//マテリアルの情報へのポインタ
	DWORD			nNumMatEnemy = 0;								//マテリアルの情報数
	D3DXVECTOR3		posOld;											//過去の位置
	D3DXVECTOR3		pos;											//位置
	D3DXVECTOR3		move;											//移動
	D3DXVECTOR3		rot;											//向き
	int				nMaxPartsNum;									//パーツの最大数
	EnemyModel		aModel[MAX_PARTS];								//[〇]ここにはパーツ数が入る
	int				nMaxAnimNum;									//アニメーションの最大数
	int				nAnimloop[MAX_ENEMYANIM];						//ループ
	int				nAnimKey[MAX_ENEMYANIM];						//キー数
	int				nAnimfram[MAX_ENEMYANIM][MAX_ENEMYKEY];			//フレーム数
	EnemyAnim		nAnimnow;										//現在のアニメーション
	int				nAnimKeynow;									//現在のキー
	bool			bAttack;										//攻撃しているかどうか
	bool			bUse;											//使用しているかどうか
	bool			bLand;											//捕まっているかどうか
	int				nIdxShadow;										//どの影を使っているか
	int				nAttackCount;									//攻撃の時間のカウント
	int				nCntTimeEnemy;									//敵カウンター
	D3DXVECTOR3		VtxMin, VtxMax;									//モデルの最小値・最大値
	int				nLife;											//ライフ

	D3DXVECTOR3     blowMove;										//吹き飛ぶ移動量
	bool			bDamage;										// ダメージを受けたか
	int				nCntDamage;										// ダメージ状態のカウンタ
}Enemy;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
void DeleteEnemy(void);
void SetEnemy(D3DXVECTOR3 pos,int nType);	// 位置と敵の種類
int GetNumEnemy(void);
void SetNumEnemy(int nNumEnemy);

bool CollisionEnemies(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange, int nIdx);	// 敵同士
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange);				// 敵と他のオブジェクト
bool CollisionAttackEnemy(D3DXVECTOR3 pos, float vtxMaxX);

#endif
