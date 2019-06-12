//=============================================================================
//
// 数字処理 [number.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMBER_PRIORITY		(6)		// 処理の優先順位

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNumber : CScene2D
{
public:
	CNumber(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_NUMBER);
	~CNumber();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CNumber* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

	int GetNumber(void) { return m_nNumber; };
	const void SetNumber(int nNumber);

	D3DXVECTOR3 GetPos(void) { return CScene2D::GetPos(); };
	const void SetPos(D3DXVECTOR3 pos) { CScene2D::SetPos(pos); };

	D3DXCOLOR GetColor(void) { return CScene2D::GetColor(); };
	const void SetColor(D3DXCOLOR col) { CScene2D::SetColor(col); };

	void GetSize(float *pWidth, float *pHeight);
	const void SetSize(float fWidth, float fHeight) { CScene2D::SetSize(fWidth, fHeight); };

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャへのポインタ

	int			m_nNumber;			// 数字
};

#endif