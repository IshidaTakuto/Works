//=============================================================================
//
// インデックスバッファ3Dオブジェクト処理 [scene3DIndex.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE3DINDEX_H_
#define _SCENE3DINDEX_H_

#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene3DIndex : public CScene
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_FIELD,			// 地面
		TYPE_WALL,			// 壁
		TYPE_CYLINDER,		// シリンダー
		TYPE_MAX
	}TYPE;

	CScene3DIndex(int nPriority = 3);
	~CScene3DIndex();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 *pos, float fRadius);

	void MakeVertex(int meshX, int meshY, int meshZ, int nLengthX, int nLengthY, int nLengthZ, TYPE type);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;				// インデックスバッファへのポインタ
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス

	D3DXVECTOR3				m_pos;					// ポリゴンの位置
	D3DXVECTOR3				m_rot;					// 回転
	int						m_nLengthX;				// 頂点のX軸の距離
	int						m_nLengthY;				// 頂点のY軸の距離
	int						m_nLengthZ;				// 頂点のZ軸の距離
	int						m_nMeshX;				// X軸の分割数
	int						m_nMeshY;				// Y軸の分割数
	int						m_nMeshZ;				// Z軸の分割数
	int						m_nNumVtx;				// 頂点数
	int						m_nNumPolygon;			// ポリゴン数
	TYPE					m_type;					// ポリゴンの種類
};

#endif