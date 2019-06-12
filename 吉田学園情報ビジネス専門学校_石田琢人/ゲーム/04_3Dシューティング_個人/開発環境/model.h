//=============================================================================
//
// モデル処理 [model.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "scene3DModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MODEL	(1)

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
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

	static HRESULT Load(void);				// テクスチャを読み込む（managerにいれる）
	static void Unload(void);				// 読み込んだテクスチャを解放

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_MODEL];	// テクスチャへのポインタ
	static LPD3DXMESH				m_pMesh[MAX_MODEL];		// メッシュ情報へのポインタ
	static LPD3DXBUFFER				m_pBuffMat[MAX_MODEL];	// マテリアル情報へのポインタ
	static DWORD					m_nNumMat[MAX_MODEL];	// マテリアル情報の数
};

#endif
