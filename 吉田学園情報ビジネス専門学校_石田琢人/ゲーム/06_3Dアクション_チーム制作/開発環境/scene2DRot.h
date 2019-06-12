//*****************************************************************************
//
//     シーン(2D)回転の処理[scene2DRot.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE2DROT_H_
#define _SCENE2DROT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    シーン2D回転クラスの定義
//*****************************************************************************
class CScene2DRot : public CScene2D
{
public:     // 誰でもアクセス可能
	CScene2DRot(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScene2DRot();

	static CScene2DRot *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fRotation, int nPriority);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fRotation);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetVtxBuffPos(D3DXVECTOR3 pos);
	void SetVtxBuffCol(D3DXCOLOR col);
	void SetScale(const float fScale);
	void SetRotation(const float fRotation);

	float GetScale(void);
	float GetRotation(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fRotation; // 向き
	float m_fScale;    // ポリゴンの大きさ
};

#endif