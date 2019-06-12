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
#define CYLINDER_PRIORITY	(3)

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
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CCylinder *Create(D3DXVECTOR3 pos);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	void SetRange(float fRange) { m_fRange = fRange; };
	float GetRange(void) { return m_fRange; };

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[CYLINDER_TEXTURE];	// ���L�e�N�X�`���ւ̃|�C���^
	float m_fRange;		// ���a
};

#endif
