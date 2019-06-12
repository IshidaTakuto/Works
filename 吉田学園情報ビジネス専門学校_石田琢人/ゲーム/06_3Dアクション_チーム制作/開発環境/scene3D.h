//*****************************************************************************
//
//     シーン(3D)の処理[scene3D.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    シーン(3D)クラスの定義
//*****************************************************************************
class CScene3D : public CScene
{
public:    // 誰でもアクセス可能
	CScene3D(int nPriority = 3, OBJTYPE objType = OBJTYPE_3DPOLYGON);
	~CScene3D();

	static CScene3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rotation);
	void SetCol(const D3DXCOLOR col);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	D3DXMATRIX GetMtxWorld(void);
	float GetPolyHeight(D3DXVECTOR3 pos);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9 m_apTexture;  // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	    // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;          // 座標
	D3DXVECTOR3             m_Rot;          // 向き
	D3DXCOLOR               m_Col;          // 色
	D3DXMATRIX              m_MtxWorld;     // ワールドマトリックス
};

#endif