//=============================================================================
//
// �v���C���[���� [player.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "sceneX.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PARTS_PLAYER	(1)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CSceneX
{
public:
	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ��ށimanager�ɂ����j
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	static CPlayer *Create(void);
	D3DXVECTOR3 GetPos(void) { return m_pos; };

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_PARTS_PLAYER];	// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH				m_pMesh[MAX_PARTS_PLAYER];		// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER				m_pBuffMat[MAX_PARTS_PLAYER];	// �}�e���A�����ւ̃|�C���^
	static DWORD					m_nNumMat[MAX_PARTS_PLAYER];	// �}�e���A�����̐�

	D3DXVECTOR3	m_pos;		// �ʒu
	float		m_fSpeed;	// ���x����
	float		m_fRange;	// �I��͈�
	float		m_fHeight;	// �グ�鍂��
};

#endif
