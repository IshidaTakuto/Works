//=============================================================================
//
// モデル処理 [model.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
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

	static HRESULT Load(void);	// テクスチャを読み込む
	static void Unload(void);	// 読み込んだテクスチャを解放

	static void GetInfo(int nIdx, LPD3DXMESH &m_pMesh, LPD3DXBUFFER &m_pBuffMat, DWORD *m_nNumMat);

private:
	static char **m_pStr;		// モデルファイルの名前
	static int	m_NumModel;		// モデル数
};

#endif
