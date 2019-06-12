//=============================================================================
//
// �|���S������ [shadow.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_SHADOW_TEXTURE	(1)
#define SHADOW_PRIORITY		(3)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CShadow : public CScene3D
{
public:
	CShadow(int nPriority = SHADOW_PRIORITY);
	~CShadow();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShadow *Create(D3DXVECTOR3 pos);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	void Delelte(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[NUM_SHADOW_TEXTURE];			// �e�N�X�`���ւ̃|�C���^
};

#endif
