//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// プレイヤーの処理 [model.h]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LOADMODEL	(16)		// 読み込むモデルの最大数
#define MAX_MODEL		(16)		// 表示するモデルの最大数
#define MAX_KEYSET		(10)		// モーション一つ当たりの最大キーセット数
#define MAX_STAMINA		(100)							// スタミナの最大値
#define WATER_SPEED					(-0.07f)						// 水の流れの速度

//*****************************************************************************
// 構造体
//*****************************************************************************
//--- モーションの種類 ---//
typedef enum
{
	MOTION_NEUTRAL = 0,		// ニュートラル
	MOTION_RIGHT,			// 右漕ぎ
	MOTION_LEFT,			// 左漕ぎ
	MOTION_MAX
}MOTIONTYPE;

//--- 読み込んだモデルの構造体 ---//
typedef struct
{
	char				pXFileName[MAX_CHAR];			// ファイル名
	LPD3DXMESH			pMesh;							// メッシュ情報へのポインタ
	LPD3DXBUFFER		pBuffMat;						// マテリアル情報へのポインタ
	DWORD				NumMat;							// マテリアルの数
	LPDIRECT3DTEXTURE9	*pTexture;						// テクスチャへのポインタ
}LoadModel;

//--- モデルの構造体 ---//
typedef struct
{
	D3DXVECTOR3 pos;					// モデルの位置
	D3DXVECTOR3 rot;					// モデルの向き
	D3DXMATRIX	mtxWorld;				// モデルのワールドマトリックス
	int			nType;					// モデルの種類
	int			nIdxModelParent;		// 親モデルのインデックス
}Model;

//--- プレイヤーの構造体 ---//
typedef struct
{
	D3DXVECTOR3 pos;					// 現在の位置
	D3DXVECTOR3 posOld;					// 前回の位置
	D3DXVECTOR3 rot;					// 向き
	D3DXVECTOR3 move;					// 移動量
	float		MoveSpeed;				// 移動速度
	D3DXMATRIX	mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 vtxMin;					// 頂点座標の最小値
	D3DXVECTOR3 vtxMax;					// 頂点座標の最大値
	float		fRadius;				// 半径
	float		fHeight;				// 高さ
	int			IdxLine;				// 線の使用番号 [インデックス]
	float		fDestAngle;				// 目的の向き
	int			nStamina;				// スタミナ
	bool		bRight;					// 右漕ぎ
	bool		bLeft;					// 左漕ぎ
	/* モデル */
	int			nLoadParts;				// 読み込むモデルの数
	int			nMaxParts;				// 使用するするモデルの数
	Model		aModel[MAX_MODEL];		// モデルの情報
	/* モーション */
	MOTIONTYPE		nMotionTypeOld;		// 前モーションの種類
	MOTIONTYPE		nMotionType;		// モーションの種類
	int				nFrameAll;			// モーションの最大フレーム数
	int				nNowFrame;			// モーションの現在のフレーム数
}Player;

//--- オフセット情報 ---//
typedef struct
{
	D3DXVECTOR3 pos;			// モデルの位置
	D3DXVECTOR3 rot;			// モデルの向き
}Offset;

//--- モーションのキー情報 ---//
typedef struct
{
	int			frame;					// フレーム数
	D3DXVECTOR3 pos[MAX_MODEL];			// 位置
	D3DXVECTOR3 rot[MAX_MODEL];			// 向き
}KeySet;

//--- モーションの構造体 ---//
typedef struct
{
	int			nLoop;					// ループ情報 [0 : ループしない, 1 : ループする]
	int			nNumKey;				// キー数
	KeySet		nKeySet[MAX_KEYSET];	// キー情報
}Motion;

//--- 読み込んだキャラクターの構造体 ---//
typedef struct
{
	Motion Motion[MOTION_MAX];	// モーションの情報
	Offset Offset[MAX_MODEL];	// モデルのオフセット
}LoadMotion;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void SubStamina(void);
void HealStamina(void);

void LoadPlayer(void);
void UnloadPlayer(void);

Player *GetPlayer(void);
#endif
