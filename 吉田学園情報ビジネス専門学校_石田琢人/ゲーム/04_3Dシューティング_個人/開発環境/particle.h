//=============================================================================
//
// パーティクル処理 [particle->h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "scene.h"
#include "scene3DBill.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_EMITTER			(16)			// エミッタの最大数
#define MAX_EMITTER_TYPE	(32)			// パーティクルの種類の最大数
#define MAX_TEXTURE			(32)			// テクスチャの最大数
#define PARTICLE_PRIORITY	(5)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CParticle : public CScene
{
public:
	CParticle(int nPriority = 7);
	virtual ~CParticle();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static CParticle *Create(D3DXVECTOR3 pos, int nType);

private:

protected:
	D3DXVECTOR3 m_pos;				// 位置
	int m_nType;					// 種類
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// パーティクルのエミッタ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CParEmitter : public CParticle
{
public:
	CParEmitter();
	~CParEmitter();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(D3DXVECTOR3 pos, int nType);
	void SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos);
	void DestroyParticle(int nIdx);

private:
	int m_nCntAngle;		// パーティクルの角度計算用カウンタ
	int m_nCntParticle;		// 出現させる間隔のカウンタ

	int			m_nLife;			// エミッタの時間
	int			m_nTexture;			// テクスチャの種類
	int			m_nAppear;			// 出る間隔
	int			m_nLap;				// 1周に出す個数
	bool		m_bLoop;			// ループの有無
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// パーティクルの一枚
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CParticlePolygon : public CScene3DBill
{
public:
	CParticlePolygon(int nPriority = PARTICLE_PRIORITY);
	~CParticlePolygon();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CParticlePolygon *Create(D3DXVECTOR3 pos, int nType, int nTexture);

	static HRESULT Load(void);		// テクスチャを読み込む
	static void Unload(void);		// 読み込んだテクスチャを解放

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];	// 共有テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffParticle;	// 頂点バッファへのポインタ

	int m_nNumParticleTexture;		// テクスチャの数

	D3DXVECTOR3 m_move;		// 移動量
	D3DXCOLOR m_col;		// エミッタの位置
	float m_fLength;		// 頂点の距離
	float m_fCntRot;		// 角度の更新
	float m_fRotAngle;		// 回転するときの角度
	float m_fAngle;			// 角度
	float m_fRadius;		// 大きさ
	int m_nLife;			// 表示時間
	int m_nType;			// エミッタの種類
	float m_fGravity;		// 重力
	float m_fBouncing;		// バウンドする値
	bool m_bDraw;			// 加算合成かどうか
	int m_nCntAngle;		// 角度計算用に必要
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// パーティクルのデータ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CParData
{
public:
	CParData();
	~CParData();
	HRESULT Init(void);
	void Uninit(void);

	static HRESULT Load(void);		// データを読み込む
	static void Unload(void);		// 読み込んだデータを解放

	static D3DXCOLOR GetFlameColor(int nIdx) { return m_apParticleData[nIdx].m_flameCol; };
	static D3DXCOLOR GetColor(int nIdx) { return m_apParticleData[nIdx].m_col; };
	static float GetFlameRadius(int nIdx) { return m_apParticleData[nIdx].m_fFlameRadius; };
	static bool GetBouncing(int nIdx) { return m_apParticleData[nIdx].m_bBouncing; };
	static int GetRotType(int nIdx) { return m_apParticleData[nIdx].m_nRot; };
	static float GetRotSpeed(int nIdx) { return m_apParticleData[nIdx].m_fRotSpeed; };
	static int GetLap(int nIdx) { return m_apParticleData[nIdx].m_nLap; };
	static int *GetParLife(int nIdx) { return &m_apParticleData[nIdx].m_nParLife[0]; };
	static int *GetRadius(int nIdx) { return &m_apParticleData[nIdx].m_nRadius[0]; };
	static int *GetWidth(int nIdx) { return &m_apParticleData[nIdx].m_nWidth[0]; };
	static int *GetHeight(int nIdx) { return &m_apParticleData[nIdx].m_nHeight[0]; };
	static int *GetDepth(int nIdx) { return &m_apParticleData[nIdx].m_nDepth[0]; };
	static float GetSpread(int nIdx) { return m_apParticleData[nIdx].m_fSpread; };
	static bool GetDraw(int nIdx) { return m_apParticleData[nIdx].m_bDraw; };
	static float GetGravity(int nIdx) { return m_apParticleData[nIdx].m_fGravity; };
	static int GetAppear(int nIdx) { return m_apParticleData[nIdx].m_nAppear; };
	static int GetLife(int nIdx) { return m_apParticleData[nIdx].m_nLife; };
	static int GetTextureIdx(int nIdx) { return m_apParticleData[nIdx].m_nTexture; };

private:
	static CParData m_apParticleData[MAX_EMITTER_TYPE];

	int m_nWidth[2];		// 幅(最小値と最大値)
	int m_nHeight[2];		// 高さ(最小値と最大値)
	int m_nDepth[2];		// 奥行(最小値と最大値)
	D3DXCOLOR m_col;		// 色
	D3DXCOLOR m_flameCol;	// 毎フレームの色の変化
	int m_nRadius[2];		// 大きさ(最小値と最大値)
	float m_fFlameRadius;	// 毎フレームの半径の拡縮
	int m_nLife;			// エミッタの時間
	int m_nParLife[2];		// 表示時間(最小値と最大値)
	int m_nTexture;			// テクスチャの種類
	float m_fSpread;		// 広がり
	int m_nAppear;			// 出る間隔
	float m_fGravity;		// 重力
	int m_nLap;				// 1周に出す個数
	int m_nRot;				// 回転するかどうか(0:なし　1:時計回り　2:反時計回り 3:ランダム)
	float m_fRotSpeed;		// 回転する速度
	bool m_bLoop;			// ループさせるか 0:なし 1:あり
	bool m_bBouncing;		// バウンドさせるか 0:なし 1:あり
	bool m_bDraw;			// 加算合成かどうか
};

#endif
