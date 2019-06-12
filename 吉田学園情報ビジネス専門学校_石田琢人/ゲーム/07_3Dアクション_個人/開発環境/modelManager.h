//=============================================================================
//
// ���f������ [model.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CModelManager : public CScene
{
public:
	CModelManager();
	~CModelManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);	// �e�N�X�`����ǂݍ���
	static void Unload(void);	// �ǂݍ��񂾃e�N�X�`�������

	static void GetInfo(int nIdx, LPD3DXMESH &m_pMesh, LPD3DXBUFFER &m_pBuffMat, DWORD *m_nNumMat);

private:
	static char **m_pStr;		// ���f���t�@�C���̖��O
	static int	m_NumModel;		// ���f����
};

#endif
