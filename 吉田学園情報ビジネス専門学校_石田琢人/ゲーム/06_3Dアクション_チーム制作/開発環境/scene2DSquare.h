//*****************************************************************************
//
//     シーン(2D)スクエアの処理[scene2DSuqare.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE2DSQUARE_H_
#define _SCENE2DSQUARE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    シーン2Dスクエアクラスの定義
//*****************************************************************************
class CScene2DSquare : public CScene2D
{
public:    // 誰でもアクセス可能
	CScene2DSquare(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScene2DSquare();
	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetVtxBuffPos(const D3DXVECTOR3 pos);
	void SetVtxBuffCol(const D3DXCOLOR col);

	float GetHeight(void);
	float GetWidth(void);

	static CScene2DSquare *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fWidth;   // 幅
	float m_fHeight;  // 高さ
};

#endif