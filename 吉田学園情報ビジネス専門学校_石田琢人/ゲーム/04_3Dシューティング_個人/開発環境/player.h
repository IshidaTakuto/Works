//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene3DModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_PRIORITY	(4)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CFormTime;
class CLife;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CScene3DModel
{
public:
	typedef enum
	{
		STATE_APPEAR = 0,	// 出現状態
		STATE_NORMAL,		// 通常状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_DEATH,		// 死亡状態
		STATE_MAX,
	}STATE;

	typedef enum
	{
		TYPE_AIRCRAFT = 0,		// 戦闘機
		TYPE_SHIP,		// 戦艦
		TYPE_TANK,		// 戦車
		TYPE_MAX,
	}TYPE;

	CPlayer(int nPriority = PLAYER_PRIORITY);
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static HRESULT Load(void);			// テクスチャを読み込む（managerにいれる）
	static void Unload(void);			// 読み込んだテクスチャを解放

	static CPlayer* Create(D3DXVECTOR3 pos, TYPE type);

	STATE GetState(void) { return m_state; };
	const void SetState(STATE state) { m_state = state; };

	TYPE GetType(void) { return m_type; };

	CLife *GetLife(void) { return m_pLife; };			// ライフのポインタを取得
	void SetLife(CLife *pLife) { m_pLife = pLife; };	// ライフのポインタを取得

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TYPE_MAX];	// 共有テクスチャへのポインタ
	static LPD3DXMESH			m_pMesh[TYPE_MAX];		// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[TYPE_MAX];	// マテリアル情報へのポインタ
	static DWORD				m_nNumMat[TYPE_MAX];	// マテリアル情報の数

	CFormTime					*m_pFormTime;			// 変形ゲージ

protected:
	CLife						*m_pLife;		// ライフゲージ

	D3DXVECTOR3					m_move;			// 移動量
	STATE						m_state;		// 現在の状態
	TYPE						m_type;			// 現在の形態
	int							m_nCntState;	// ステートのカウンタ
	bool						m_bDisp;		// 描画するかどうか
	int							m_nCntDamage;	// ダメージを受ける間隔
	int							m_nCntBullet;	// 弾の発射間隔
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 戦闘機
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CPlayerAircraft : public CPlayer
{
public:
	CPlayerAircraft(int nPriority = 4);
	~CPlayerAircraft();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 戦艦
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CPlayerShip : public CPlayer
{
public:
	CPlayerShip(int nPriority = 4);
	~CPlayerShip();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nCntSpBullet;		//
	bool m_bSpShoot;		// 特殊弾を撃っているかどうか
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 戦車
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CPlayerTank : public CPlayer
{
public:
	CPlayerTank(int nPriority = 4);
	~CPlayerTank();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};

#endif