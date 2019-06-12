//=============================================================================
//
// ゴブリン処理 [goburin.h]
// Author : ABE YUUTAROU
//
//=============================================================================
#ifndef _GOBURIN_H_
#define _GOBURIN_H_

#include "main.h"

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
typedef enum
{// ゴブリンのモーションの種類
	GOBURINMOTIONTYPE_NEUTRAL = 0,								// ニュートラル
	GOBURINMOTIONTYPE_MOVE,										// 説明モーション
	GOBURINMOTIONTYPE_MAX										// 種類の総数
}GOBURINMOTIONTYPE;

typedef struct
{// ゴブリンのモデルの情報
	LPD3DXMESH		pMesh = NULL;								// メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMat = NULL;							// マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9	*pTex = NULL;							// テクスチャ情報へのポインタ
	D3DMATERIAL9	*pCol = NULL;								// 現在の色へのポインタ
	D3DMATERIAL9	*pColDef = NULL;							// デフォルトの色へのポインタ
	DWORD			nNumMat = 0;								// マテリアル情報の数
	D3DXMATRIX		mtxWorld;									// ワールドマトリックス
	D3DXVECTOR3		posFarst;									// 初期の位置
	D3DXVECTOR3		pos;										// 位置
	D3DXVECTOR3		rot;										// 向き
	int				nIdxModelParent;							// 親モデルのインデックス
	char			aName[128];									// 読み込むモデルの名前
} GOBURINModel;

typedef struct
{// ゴブリンのキーの情報
	D3DXVECTOR3	*aPos = NULL;									// 位置
	D3DXVECTOR3	*aRot = NULL;									// 向き
	int			nNumFrame;										// 更新するフレーム数
} GOBURINKey;

typedef struct
{// ゴブリンのモーションの情報
	int			nLoop;											// ループするかどうか[ しない = 0, する = 1 ]
	int			nNumKey;										// キーの数
	GOBURINKey	*aGOBURINKey = NULL;							// キーの情報
	
} GOBURINMotion;

typedef struct
{// ゴブリンの情報
	D3DXMATRIX			mtxWorld;								// ワールドマトリックス
	D3DXVECTOR3			pos;									// 現在の位置
	D3DXVECTOR3			rot;									// 向き
	D3DXVECTOR3			rotDest;								// 目的の向き
	int					nNumModel;								// モデル数
	GOBURINModel		*aGOBURINModel = NULL;				// モデルの情報へのポインタ
	GOBURINMotion		aGOBURINMotion[GOBURINMOTIONTYPE_MAX];	// モーションの情報
	GOBURINMOTIONTYPE	motionType;								// 現在のモーションの種類
	int					nKey;									// 現在のキー
	int					nCntFrame;								// 経過フレーム
	int					nMaxCntFrame;							// 総合経過フレーム
	bool				bDisp;									// 出現してるかどうか
	bool				bMove;									// 動き中かどうか
} GOBURIN;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGOBURIN(void);
void UninitGOBURIN(void);
void UpdateGOBURIN(void);
void DrawGOBURIN(void);
GOBURIN *GetGOBURIN(void);

#endif
