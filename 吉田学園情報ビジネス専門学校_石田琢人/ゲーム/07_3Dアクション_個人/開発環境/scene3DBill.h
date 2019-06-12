//=============================================================================
//
// 3Dビルボードオブジェクト処理 [scene3DBill.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE3DBILL_H_
#define _SCENE3DBILL_H_

#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene3DBill : public CScene
{
public:
	CScene3DBill(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_NONE);
	~CScene3DBill();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene3DBill *Create(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f); };
	const void SetSize(D3DXVECTOR3 size);

	float GetWidth(void) { return m_fWidth; };
	const void SetWidth(float fWidth);

	float GetHeight(void) { return m_fHeight; };
	const void SetHeight(float fHeight);

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; };
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 vtxBuff) { m_pVtxBuff = vtxBuff; };

	void SetZBuffer(D3DCMPFUNC cmpFunc);

	void SetLighting(bool bLighting) { m_bLighting = bLighting; };

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;			// ワールドマトリックス

	D3DXVECTOR3				m_pos;				// ポリゴンの位置
	D3DXVECTOR3				m_rot;				// 回転
	float					m_fWidth;			// 頂点のX軸の距離
	float					m_fHeight;			// 頂点のY軸の距離
	D3DXCOLOR				m_col;				// 色
	D3DCMPFUNC				m_cmpFunc;			// Zバッファの設定用
	bool					m_bZBuffer;			// Zバッファの設定をするかどうか
	bool					m_bLighting;		// ライトが有効か無効か
};

#endif