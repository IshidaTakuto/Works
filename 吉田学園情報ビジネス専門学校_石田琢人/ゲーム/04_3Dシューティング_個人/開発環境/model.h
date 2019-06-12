//=============================================================================
//
// ���f������ [model.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "scene3DModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MODEL	(1)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CModel : public CScene3DModel
{
public:
	typedef enum
	{
		TYPE_OBJECT = 0,
		//TYPE_ITEM,
		TYPE_MAX
	}TYPE;

	CModel();
	~CModel();
	HRESULT Init(D3DXVECTOR3 pos, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModel* Create(D3DXVECTOR3 pos, TYPE type);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ��ށimanager�ɂ����j
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_MODEL];	// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH				m_pMesh[MAX_MODEL];		// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER				m_pBuffMat[MAX_MODEL];	// �}�e���A�����ւ̃|�C���^
	static DWORD					m_nNumMat[MAX_MODEL];	// �}�e���A�����̐�
};

#endif
