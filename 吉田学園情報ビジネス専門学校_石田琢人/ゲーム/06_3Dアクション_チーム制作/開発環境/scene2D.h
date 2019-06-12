//*****************************************************************************
//
//     シーン(2D)の処理[scene2D.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    シーン2Dクラスの定義
//*****************************************************************************
class CScene2D : public CScene
{
public:     // 誰でもアクセス可能
	//------------------------
	//  テクスチャの種類
	//------------------------
	typedef enum
	{
		TEXTURE_TYPE_0 = 0,
		TEXTURE_TYPE_MAX
	}TEXTURE_TYPE;

	CScene2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScene2D();

	static CScene2D *Create(TEXTURE_TYPE type, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9 m_apTexture[TEXTURE_TYPE_MAX];   // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	  // テクスチャへのポインタ
	D3DXVECTOR3				m_pos;	      // ポリゴンの位置
	D3DXCOLOR               m_Col;        // ポリゴンの色
};

#endif