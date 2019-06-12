//=============================================================================
//
// �G���� [enemy.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene3DModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY		(128)
#define ENEMY_PRIORITY	(4)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CShadow;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemy : public CScene3DModel
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_DAMAGE,		// �_���[�W���
		STATE_DEATH,			// ���S���
		STATE_MAX,
	}STATE;

	typedef enum
	{// �G�̎��
		TYPE_AIRCRAFT = 0,
		TYPE_SHIP,
		TYPE_TANK,
		TYPE_MAX
	}TYPE;

	CEnemy(int nPriority = ENEMY_PRIORITY);
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemy* Create(D3DXVECTOR3 pos, TYPE type);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ��ށimanager�ɂ����j
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	STATE GetState(void) { return m_state; };
	const void SetState(STATE state) { m_state = state; };

	TYPE GetType(void) { return m_type; };

	static int GetNumEnemy(void) { return m_nNumEnemy; };

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TYPE_MAX];	// ���L�e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH			m_pMesh[TYPE_MAX];		// ���L���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat[TYPE_MAX];	// ���L�}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat[TYPE_MAX];	// ���L�}�e���A�����̐�

	static int					m_nNumEnemy;			// �G�̑���
	D3DXVECTOR3					m_move;					// �ړ���
	float						m_posOriginalZ;			// �y���̖{���̈ʒu
	int							m_nCntState;			// �X�e�[�g�̃J�E���^
	int							m_nCntBullet;			// �e�����܂ł̃J�E���^
	bool						m_bMoveAngle;			// �ړ��̊p�x
	STATE						m_state;				// ���݂̏��
	TYPE						m_type;					// �G�̎��
	CShadow						*m_pShadow;				// �e�̃|�C���^�ϐ�
};

#endif