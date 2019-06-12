//=============================================================================
//
// �e���� [bullet.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "scene3DBill.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBullet : public CScene3DBill
{
public:
	typedef enum
	{
		TYPE_PLAYERNORMAL = 0,	// �v���C���[�̒ʏ�e
		TYPE_PLAYERSPECIAL,		// �v���C���[�̓���e
		TYPE_ENEMY,				// �G�̒e
		TYPE_MAX
	}TYPE;

	CBullet(int nPriority = 3);
	virtual ~CBullet();
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static CBullet* Create(D3DXVECTOR3 pos, CBullet::TYPE type);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ��ށimanager�ɂ����j
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

protected:
	D3DXVECTOR3	m_move;					// �ړ���

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// ���L�e�N�X�`���ւ̃|�C���^
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �v���C���[�̒e
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
{// �v���C���[�̒ʏ�e
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
{// �v���C���[�̓���e
public:
	CBulletPlayerSpecial();
	~CBulletPlayerSpecial();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(D3DXVECTOR3 move) { m_move = move; };

private:
	static int m_nCntAngle;			// �p�x�����炷�̂ɕK�v
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �G�̒e
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
	int	  m_nCntAngle;			// �ǔ����鎞��
	float m_fAngle;				// �v���C���[�ւ̊p�x
};

#endif