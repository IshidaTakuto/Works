//=============================================================================
//
// �Ǐ��� [wall.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_WALL_ROCK	(30)	// 1�̕ǂɂ����̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene3DModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CWall : public CScene
{
public:

	CWall(int nPriority = 3);
	~CWall();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CWall *Create(int nPos);

	static HRESULT Load(void);		// �e�N�X�`����ǂݍ���
	static void Unload(void);		// �ǂݍ��񂾃e�N�X�`�������

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// ���L�e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH			m_pMesh;		// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat;		// �}�e���A�����̐�

	CScene3DModel				*m_pModel[MAX_WALL_ROCK];	// ���f���N���X�ւ̃|�C���^�ϐ�
	int							m_nPos;						// ���X���W
};

#endif