//=============================================================================
//
// �v���C���[���� [player.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene3DModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_PRIORITY	(4)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CFormTime;
class CLife;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CScene3DModel
{
public:
	typedef enum
	{
		STATE_APPEAR = 0,	// �o�����
		STATE_NORMAL,		// �ʏ���
		STATE_DAMAGE,		// �_���[�W���
		STATE_DEATH,		// ���S���
		STATE_MAX,
	}STATE;

	typedef enum
	{
		TYPE_AIRCRAFT = 0,		// �퓬�@
		TYPE_SHIP,		// ���
		TYPE_TANK,		// ���
		TYPE_MAX,
	}TYPE;

	CPlayer(int nPriority = PLAYER_PRIORITY);
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static HRESULT Load(void);			// �e�N�X�`����ǂݍ��ށimanager�ɂ����j
	static void Unload(void);			// �ǂݍ��񂾃e�N�X�`�������

	static CPlayer* Create(D3DXVECTOR3 pos, TYPE type);

	STATE GetState(void) { return m_state; };
	const void SetState(STATE state) { m_state = state; };

	TYPE GetType(void) { return m_type; };

	CLife *GetLife(void) { return m_pLife; };			// ���C�t�̃|�C���^���擾
	void SetLife(CLife *pLife) { m_pLife = pLife; };	// ���C�t�̃|�C���^���擾

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TYPE_MAX];	// ���L�e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH			m_pMesh[TYPE_MAX];		// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat[TYPE_MAX];	// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat[TYPE_MAX];	// �}�e���A�����̐�

	CFormTime					*m_pFormTime;			// �ό`�Q�[�W

protected:
	CLife						*m_pLife;		// ���C�t�Q�[�W

	D3DXVECTOR3					m_move;			// �ړ���
	STATE						m_state;		// ���݂̏��
	TYPE						m_type;			// ���݂̌`��
	int							m_nCntState;	// �X�e�[�g�̃J�E���^
	bool						m_bDisp;		// �`�悷�邩�ǂ���
	int							m_nCntDamage;	// �_���[�W���󂯂�Ԋu
	int							m_nCntBullet;	// �e�̔��ˊԊu
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �퓬�@
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
// ���
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
	bool m_bSpShoot;		// ����e�������Ă��邩�ǂ���
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ���
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