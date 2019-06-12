//=============================================================================
//
// ���b�V���V�����_�[���� [meshCylinder.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "scene3DIndex.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CYLINDER_TEXTURE	(1)
#define CYLINDER_PRIORITY	(2)
#define MAX_CYLINDER		(4)		// �V�����_�[�̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCylinder : public CScene3DIndex
{
public:
	CCylinder(int nPriority = CYLINDER_PRIORITY);
	~CCylinder();
	HRESULT Init(D3DXVECTOR3 pos, int nWidth, int nHeight, int nDepth, int nMeshX, int nMeshY, int nMeshZ);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CCylinder* Create(D3DXVECTOR3 pos, int nWidth, int nHeight, int nDepth, int nMeshX, int nMeshY, int nMeshZ, int nTexture);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	bool Collision(D3DXVECTOR3 *pos, float fVtxMinY);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[CYLINDER_TEXTURE];	// ���L�e�N�X�`���ւ̃|�C���^
	int m_nTexture;		// �e�N�X�`���̎��
};

#endif
