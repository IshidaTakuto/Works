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
#define MESHFIELD_PRIORITY	(3)		// �n�ʂ̏����D�揇��

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

	D3DXVECTOR3 GetNor(D3DXVECTOR3 pos);

	float GetAngle(D3DXVECTOR3 pos, float fAngle);
	float GetSlipAngle(D3DXVECTOR3 pos);

	void RangeColor(D3DXVECTOR3 pos, float fRange);

private:
	int m_nType;			// �e�N�X�`���̎��
	int m_nWire;			// ���C���[�t���[����\�����邩
};

#endif
