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
	CScene3DBill(int nPriority = 3);
	~CScene3DBill();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 pos, float fRange);
	bool CollisionXY(D3DXVECTOR3 pos, float fRange);
	bool CollisionXZ(D3DXVECTOR3 pos, float fRange);
	bool CollisionY(float posY, float fRange);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	int GetLife(void) { return m_nLife; };
	const void SetLife(int nLife) { m_nLife = nLife; };
	const int SubtractLife(int nDamage);

	const void SetLength(float fLength0, float fLength1);
	float GetLengthX(void) { return m_fLengthX; };
	float GetLengthY(void) { return m_fLengthY; };

	const void SetTexPos(D3DXVECTOR2 *tex);
	const void SetVtxPos(D3DXVECTOR3 *vtxPos);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス

	D3DXVECTOR3				m_pos;					// ポリゴンの位置
	D3DXVECTOR3				m_rot;					// 回転
	float					m_fLengthX;				// 頂点のX軸の距離
	float					m_fLengthY;				// 頂点のY軸の距離
	float					m_fLengthZ;				// 頂点のZ軸の距離

	int						m_nLife;				// 寿命
};

#endif