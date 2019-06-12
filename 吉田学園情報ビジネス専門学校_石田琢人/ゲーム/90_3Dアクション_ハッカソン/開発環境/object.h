//=============================================================================
//
// オブジェクトの処理 [object.h]
// Author : 佐藤亮太
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"       // メイン情報のヘッダー

#define MAX_OBJECT			(100)							// オブジェクトの最大数
#define MAX_ADDRESS			(256)						// アドレスを保存できる最大文字数

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{// モデルの構造体
	LPD3DXMESH			pMesh;							// メッシュ(頂点情報の集まり)情報へのポインタ
	LPD3DXBUFFER		pBuffMat;						// マテリアル情報へのポインタ
	DWORD				nNumMat;						// マテリアル情報の数
	char				cAddressModel[MAX_ADDRESS];		// モデルのアドレスを入れる変数
	D3DXVECTOR3			vtxMin, vtxMax;					// モデルの最小値・最大値
	LPDIRECT3DTEXTURE9	*pTexture;						// テクスチャへのポインタ
}ObjectModel;

typedef struct
{// 当たり判定のオフセットの構造体
	D3DXVECTOR3			pos;							// 位置
	D3DXVECTOR3			rot;							// 向き
	D3DXMATRIX			mtxWorld;						// ワールドマトリックス
	int					nIdxParent;						// 親モデルのインデックス
	float				fRadius;						// 半径
}COLLISION_OBJECT;

typedef struct
{// オブジェクトの構造体
	D3DXVECTOR3			pos;				// 位置
	D3DXVECTOR3			posOld;				// 前回の位置
	D3DXVECTOR3			move;				// 位置の移動量
	D3DXVECTOR3			rot;				// 向き
	D3DXVECTOR3			rotDest;			// 目的の向き
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
	int					nIdyShadow;			// 影の番号
	int					nCollisionType;		// 当たり判定のタイプ	0:矩形 1:半径 2:円柱
	float				fRadius;			// 半径
	bool				bUse;				// 使っているかどうか
	COLLISION_OBJECT	Collision;			// 当たり判定の構造体

	int					nIdxModel;			// 使用するモデル
}Object;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitObject(void);
void UninitObject(void);
void UpdateObject(void);
void DrawObject(void);
void FlowingObject(Object *pObject);		// 流れていく処理
void LoopObject(Object *pObject);			// 最後まで流れたら戻る処理
bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius);
void CubeCollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius, int nIdxObject, bool *bHit);		// 矩形の判定
bool SphiaCollisionObject(D3DXVECTOR3 *pPos,float fRadius,int nIdxObject);													// 円の判定
void CollisionObjectAttack(D3DXVECTOR3 *pPos, float fRadius);
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTypeModel, int nCollisionType);
void TextLoadObject(void);											// オブジェクト情報の読み込み
void FileLodeObject(int nIdxObject);								// Xファイルの読み込み
void MinMaxSearchObject(int nIdxObject);							// 幅や高さの最小最大検索
void SetPosOldObject(void);											// 前回の位置の保存
Object *GetObjectModel(void);										// オブジェクト情報の取得

#endif
