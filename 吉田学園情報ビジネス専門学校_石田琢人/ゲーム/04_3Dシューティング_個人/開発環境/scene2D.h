//=============================================================================
//
// 2Dオブジェクト処理 [scene2D.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = 3);
	~CScene2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 pos, float vtxMaxX);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);

	float GetLength(void) { return m_fLength; };
	const void SetLength(float fLength);

	const void SetVtxPos(D3DXVECTOR3 *vtxPos);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	const void SetTexPos(D3DXVECTOR2 *tex);

	int GetLife(void) { return m_nLife; };
	const void SetLife(int nLife) { m_nLife = nLife; };
	const int SubtractLife(int nDamage);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;					// ポリゴンの位置
	//float					m_fCntRot;				// 角度の更新
	//float					m_fAngle;				// 頂点の角度
	float					m_fLength;				// 頂点の距離
	int						m_nLife;				// 寿命
};

#endif