//=============================================================================
//
// �e���� [shadow.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOW_PRIORITY		(6)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CShadow : public CScene
{
public:
	CShadow(int nPriority = SHADOW_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_SHADOW);
	~CShadow();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShadow *Create(void);

	void Set(D3DXMATRIX mtxWorld, LPD3DXMESH mesh, D3DXVECTOR3 posObj);

private:
};

#endif
