//=============================================================================
//
// メッシュシリンダー処理 [meshCylinder.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "scene3DIndex.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CYLINDER_TEXTURE	(1)
#define CYLINDER_PRIORITY	(2)
#define MAX_CYLINDER		(4)		// シリンダーの数

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
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

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

	bool Collision(D3DXVECTOR3 *pos, float fVtxMinY);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[CYLINDER_TEXTURE];	// 共有テクスチャへのポインタ
	int m_nTexture;		// テクスチャの種類
};

#endif
