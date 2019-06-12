//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_EMITTER			(16)							// エミッタの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rot;		// 現在の角度
	D3DXCOLOR col;			// エミッタの位置
	float fLength;			// 頂点の距離
	float fCntRot;			// 角度の更新
	float fRotAngle;		// 回転するときの角度
	float fAngle;			// 角度
	float fRadius;			// 大きさ
	int nLife;				// 表示時間
	int nType;				// テクスチャの種類
	float fGravity;			// 重力
	bool bUse;				// 使用されているか
	float fBouncing;		// バウンドする値
	int nIdx;				// エミッタの番号を記憶
	bool bDraw;				// 加算合成かどうか
	D3DXMATRIX mtxWorld;
}Particle;

typedef struct
{
	D3DXVECTOR3 pos;		// エミッタの位置
	int nWidth[2];			// 幅(最小値と最大値)
	int nHeight[2];			// 高さ(最小値と最大値)
	int nDepth[2];			// 奥行(最小値と最大値)
	D3DXCOLOR col;			// 色
	D3DXCOLOR flameCol;		// 毎フレームの色の変化
	int nRadius[2];			// 大きさ(最小値と最大値)
	float fFlameRadius;		// 毎フレームの半径の拡縮
	int nLife;				// エミッタの時間
	int nParLife[2];		// 表示時間(最小値と最大値)
	int nType;				// テクスチャの種類
	float fSpread;			// 広がり
	int nAppear;			// 出る間隔
	float fGravity;			// 重力
	int nLap;				// 1周に出す個数
	int nRot;				// 回転するかどうか(0:なし　1:時計回り　2:反時計回り 3:ランダム)
	bool bLoop;				// ループさせるか 0:なし 1:あり
	bool bBouncing;			// バウンドさせるか 0:なし 1:あり
	bool bDraw;				// 加算合成かどうか
	bool bUse;				// 使用されているか
}ParEmitter;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticleEmitter(D3DXVECTOR3 pos, int nType);
void SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos);
void DestroyParticle(int nIdx);

void DeleteParticle(void);

#endif