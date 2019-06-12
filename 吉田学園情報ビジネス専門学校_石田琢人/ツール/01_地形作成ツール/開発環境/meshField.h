//=============================================================================
//
// ���b�V���t�B�[���h���� [meshField.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "scene3DIndex.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMeshField : public CScene3DIndex
{
public:
	CMeshField();
	~CMeshField();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshField* Create(void);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ��ށimanager�ɂ����j
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

private:
	static LPDIRECT3DTEXTURE9 *m_pTexture;	// ���L�e�N�X�`���ւ̃|�C���^
	static int m_nNumTex;	// �e�N�X�`����
	int m_nType;			// �e�N�X�`���̎��
	int m_nWire;			// ���C���[�t���[����\�����邩
};

#endif
