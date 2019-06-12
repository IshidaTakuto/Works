//*****************************************************************************
//
//     半球の処理[dome.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _DOME_H_
#define _DOME_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene3DMesh.h"

//*****************************************************************************
//    半球クラスの定義
//*****************************************************************************
class CDome : public CScene3DMesh
{
public:    // 誰でもアクセス可能
	CDome(int nPriority = 3,OBJTYPE objType = OBJTYPE_DOME);
	~CDome();
	static CDome *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9 m_apTexture;  // テクスチャへのポインタ
	float m_fRadius;                        // 半球の半径
	int   m_nXBlock;                        // 横の分割数
	int   m_nYBlock;                        // 縦の分割数
};

#endif