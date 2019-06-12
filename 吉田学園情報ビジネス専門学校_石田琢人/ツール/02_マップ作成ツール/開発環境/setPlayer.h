//=============================================================================
//
// �v���C���[���� [player.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SETPLAYER_H_
#define _SETPLAYER_H_

#include "scene.h"
#include "modelObject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SETPLAYER_PRIORITY		(4)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSetPlayer : public CScene
{
public:
	CSetPlayer(int nPriority = SETPLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_SETPLAYER);
	~CSetPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CSetPlayer *Create(CModelObjectManager::TYPE type);
	void SetModel(char **pModelName, int nNumModel);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	bool Range(D3DXVECTOR3 pos);

private:
	void SetObject(void);
	const void SetModelInfo(CModelObjectManager::TYPE typeOld, int nTypeOld);

	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXMATRIX	m_mtxWorld;					// ���[���h�}�g���b�N�X
	float		m_fSpeed;					// ���x����
	float		m_fRange;					// �I��͈�
	float		m_fHeight;					// �グ�鍂��
	int			m_nType;					// ���f���ԍ�
	CModelObjectManager::TYPE	m_type;		// �ݒu����I�u�W�F�N�g�̎��
	CModel		**m_pModel;					// ���f���N���X�̃|�C���^�ϐ�
	int			m_nNumModel;				// ���f����
};

#endif
