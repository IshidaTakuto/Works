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
typedef enum
{
	PARTICLE_SELEMITTER = 0,	// 選択中のエミッタ
	PARTICLE_MAXMOVE,			// 移動量の最大値
	PARTICLE_MINMOVE,			// 移動量の最小値
	PARTICLE_COLOR,				// 色
	PARTICLE_FLAMECOLOR,		// 毎フレームの色の変化
	PARTICLE_RADIUS,			// 半径(大きさ)
	PARTICLE_FLAMERADIUS,		// 毎フレームの半径の拡縮
	PARTICLE_LIFE,				// 寿命
	PARTICLE_PARLIFE,			// パーティクルの表示時間
	PARTICLE_APPEAR,			// 出る間隔
	PARTICLE_SPREAD,			// 広がり
	PARTICLE_LAP,				// 1周に出す個数
	PARTICLE_LOOP,				// ループさせるか
	PARTICLE_GRAVITY,			// 重力
	PARTICLE_BOUNCING,			// バウンドさせるか
	PARTICLE_ROTATE,			// 回転させるかどうか
	PARTICLE_ROTATE_SPEED,		// 回転する速度
	PARTICLE_TEXTURE,			// テクスチャの種類
	PARTICLE_DRAW,				// 加算合成かどうか
	PARTICLE_MAX
}PARTICLE_TYPE;

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
	float fGravity;			// 重力
	int nType;				// テクスチャの種類
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
	int nLap;				// 1周に出す個数
	float fGravity;			// 重力
	int nRot;				// 回転するかどうか(0:なし　1:時計回り　2:反時計回り 3:ランダム)
	float fRotSpeed;		// 回転する速度
	bool bLoop;				// ループさせるか 0:なし 1:あり
	bool bBouncing;			// バウンドさせるか 0:なし 1:あり
	bool bUse;				// 使用されているか
	bool bDraw;				// 加算合成かどうか 0:普通 1:加算
}ParEmitter;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticleEmitter(D3DXVECTOR3 pos);
void SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos);
void DestroyParticle(int nIdx);

#endif