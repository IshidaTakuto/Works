//=============================================================================
//
// �G���� [enemy.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_PRIORITY	(4)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CMotion;
class CShadow;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemy : public CScene
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_WALK,			// �ړ����
		STATE_ATTACK,		// �U�����
		STATE_DAMAGE,		// �_���[�W���
		STATE_DEATH,		// ���S���
		STATE_MAX,
	}STATE;

	CEnemy(int nPriority = ENEMY_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_ENEMY);
	~CEnemy();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemy* Create(D3DXVECTOR3 pos, int nType);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	static char **GetModelName(int nType) { return m_pModelName[nType]; };
	static int GetNumParts(int nType) { return m_pNumParts[nType]; };

	static void GetInfo(int nType, CModel **pModel);		// ���f�����̐ݒ�

	int GetType(void) { return m_nType; };

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// �ʒu�̐ݒ�
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// �ʒu�̎擾

	D3DXVECTOR3 GetRot(void) { return m_rot; };			// �����̐ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// �����̎擾

	void Range(void);

private:
	typedef struct
	{// �t�@�C���ǂݍ��ݍ\����
		int nIdx;			// ���f���ԍ�
		int nParent;		// �e�ԍ�
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
	}DATA;

	bool Collision(D3DXVECTOR3 pos, float fRange);	// �����蔻��

	static int		*m_pNumParts;				// �p�[�c��
	static char		***m_pModelName;			// ���f����
	static DATA		**m_pFileData;				// �t�@�C���ǂݎ��

	D3DXMATRIX		m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;						// �ʒu
	D3DXVECTOR3		m_rot;						// ��]
	int				m_nType;					// �G�̎��
	CShadow			*m_pShadow;					// �e�̃|�C���^�ϐ�
	CModel			**m_pModel;					// ���f���N���X�̃|�C���^�ϐ�
	CMotion			*m_pMotion;					// ���[�V�����N���X�̃|�C���^�ϐ�
	bool			m_bRange;					// �͈͓����ǂ���
};

#endif