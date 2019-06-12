//=============================================================================
//
// 背景処理 [bg.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BG		(3)		// 背景の最大数
#define BG_PRIORITY	(1)		// 処理の優先番号

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBg : public CScene2D
{
public:
	CBg(CScene::OBJTYPE objType = CScene::OBJTYPE_BG);
	~CBg();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBg* Create(int nTexture, bool bAdd, int nPriority = BG_PRIORITY);

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

	void TexMove(float fMove);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_BG];	// 共有テクスチャへのポインタ
	bool m_bAdd;									// 加算合成かどうか
	static int m_nPriority;							// 優先番号
};

#endif
