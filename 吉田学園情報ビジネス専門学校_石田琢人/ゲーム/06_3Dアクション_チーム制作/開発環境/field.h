//*****************************************************************************
//
//     地面の処理[field.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _FIELD_H_
#define _FIELD_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene3DMesh.h"

//*****************************************************************************
//    フィールドクラスの定義
//*****************************************************************************
class CField : public CScene3DMesh
{
public:    // 誰でもアクセス可能
	CField(int nPriority = 3, OBJTYPE objType = OBJTYPE_FIELD);
	~CField();
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	float GetPolyHeight(const D3DXVECTOR3 pos, D3DXVECTOR3 *pPolygonNormal);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9 m_apTexture;  // テクスチャへのポインタ
	float m_fWidth;                // 1ブロック分の幅
	float m_fDepth;                // 1ブロック分の奥行
	int   m_nXBlock;               // 横の分割数
  	int   m_nZBlock;               // 奥行の分割数
	char  m_aVertexFileName[256];  // 頂点情報へのスクリプトファイル名
};

#endif