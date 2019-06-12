//*****************************************************************************
//
//     シーン(3D)メッシュの処理[scene3DMesh.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE3DMESH_H_
#define _SCENE3DMESH_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene3D.h"

//*****************************************************************************
//    シーン(3D)メッシュクラスの定義
//*****************************************************************************
class CScene3DMesh : public CScene3D
{
public:    // 誰でもアクセス可能
	CScene3DMesh(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESH);
	~CScene3DMesh();
	static CScene3DMesh *Create(int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetNumVertex(const int nNumVertex);
	void SetNumIndex(const int nNumIndex);
	void SetNumPolygon(const int nNumPolygon);

	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9 m_apTexture;  // テクスチャへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // インデックスバッファへのポインタ
	int                     m_nNumVertex;   // 頂点数
	int                     m_nNumIndex;    // インデックス数
	int                     m_nNumPolygon;  // ポリゴン数
};

#endif