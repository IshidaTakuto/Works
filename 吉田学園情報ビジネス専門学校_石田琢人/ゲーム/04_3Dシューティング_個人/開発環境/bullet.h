//=============================================================================
//
// 弾処理 [bullet.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "scene3DBill.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBullet : public CScene3DBill
{
public:
	typedef enum
	{
		TYPE_PLAYERNORMAL = 0,	// プレイヤーの通常弾
		TYPE_PLAYERSPECIAL,		// プレイヤーの特殊弾
		TYPE_ENEMY,				// 敵の弾
		TYPE_MAX
	}TYPE;

	CBullet(int nPriority = 3);
	virtual ~CBullet();
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static CBullet* Create(D3DXVECTOR3 pos, CBullet::TYPE type);

	static HRESULT Load(void);				// テクスチャを読み込む（managerにいれる）
	static void Unload(void);				// 読み込んだテクスチャを解放

protected:
	D3DXVECTOR3	m_move;					// 移動量

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャへのポインタ
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// プレイヤーの弾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CBulletPlayer : public CBullet
{
public:
	CBulletPlayer();
	virtual ~CBulletPlayer();
	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

private:
};

class CBulletPlayerNormal : public CBulletPlayer
{// プレイヤーの通常弾
public:
	CBulletPlayerNormal();
	~CBulletPlayerNormal();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};

class CBulletPlayerSpecial : public CBulletPlayer
{// プレイヤーの特殊弾
public:
	CBulletPlayerSpecial();
	~CBulletPlayerSpecial();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(D3DXVECTOR3 move) { m_move = move; };

private:
	static int m_nCntAngle;			// 角度をずらすのに必要
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 敵の弾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CBulletEnemy : public CBullet
{
public:
	CBulletEnemy();
	~CBulletEnemy();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int	  m_nCntAngle;			// 追尾する時間
	float m_fAngle;				// プレイヤーへの角度
};

#endif