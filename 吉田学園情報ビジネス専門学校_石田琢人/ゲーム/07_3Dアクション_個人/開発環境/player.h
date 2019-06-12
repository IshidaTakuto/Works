//=============================================================================
//
// �v���C���[���� [player.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_PRIORITY		(4)		// �����̗D��ԍ�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CMotion;
class CShadow;
class CLife2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CScene
{
public:
	typedef struct
	{// �L�[�v�f
		float posX;
		float posY;
		float posZ;
		float rotX;
		float rotY;
		float rotZ;
	}KEY;

	typedef struct
	{// �L�[���
		int nFrame;		// �t���[��
		KEY *pKey;	// �L�[�v�f
	}KEY_INFO;

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// �ʒu�̎擾
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// �ʒu�̐ݒ�

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// �p�x�̎擾
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// �p�x�̐ݒ�

	D3DXVECTOR3 GetMove(void) { return m_move; };				// �ړ��ʂ̎擾
	const void SetMove(D3DXVECTOR3 move) { m_move = move; };	// �ړ��ʂ̐ݒ�

	bool GetSlipFlag(void) { return m_bSlip; };					// ������Ԃ̎擾
	void SetSlipFlag(bool bSlip) { m_bSlip = bSlip; };			// ������Ԃ̐ݒ�

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };					// ���[���h�}�g���b�N�X�̎擾
	const void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ���[���h�}�g���b�N�X�̐ݒ�

	bool Collision(D3DXVECTOR3 pos, float fRange);

	void Damage(int nDamage);

private:
	void Tilt(void);

	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_rot;				// ��]
	D3DXVECTOR3		m_move;				// �ړ���
	float			m_rotDest;			// �ړI�̊p�x
	int				m_nCntState;		// �X�e�[�g�̃J�E���^
	float			m_fSpeed;			// ���x
	float			m_fSlipSpeed;		// ���鑬�x
	float			m_fAngleSlip;		// �������
	float			m_fJumpSpeed;		// �W�����v���̑��x
	bool			m_bJump;			// �W�����v�t���O
	bool			m_bSlip;			// �����Ă邩�ǂ���
	CModel			**m_pModel;			// ���f���N���X�̃|�C���^�ϐ�
	int				m_nNumParts;		// �p�[�c��
	int				m_nCurMotion;		// ���݂̃��[�V����
	CMotion			*m_pMotion;			// ���[�V�����N���X�̃|�C���^�ϐ�
	CShadow			*m_pShadow;			// �e�̃|�C���^�ϐ�
	CLife2D			*m_pLife;			// ���C�t�N���X�̃|�C���^�ϐ�
	bool			m_bDamage;			// �_���[�W���󂯂����ǂ���
	bool			m_bMove;			// �����邩�ǂ���
	int				m_nCntParticle;		// �p�[�e�B�N�����o���܂ł̃J�E���^
	float			m_fAngle;			// �n�ʂƌ����̌�_�̊p�x
	bool			m_bDash;			// �W�����v���ɋ󒆃_�b�V�����g�������ǂ���
	bool			m_bStageFade;		// �X�e�[�W�J�ڒ�
};

#endif