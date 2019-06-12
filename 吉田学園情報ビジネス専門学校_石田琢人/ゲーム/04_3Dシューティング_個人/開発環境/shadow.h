//=============================================================================
//
// ポリゴン処理 [shadow.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_SHADOW_TEXTURE	(1)
#define SHADOW_PRIORITY		(3)

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
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

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

	void Delelte(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[NUM_SHADOW_TEXTURE];			// テクスチャへのポインタ
};

#endif
