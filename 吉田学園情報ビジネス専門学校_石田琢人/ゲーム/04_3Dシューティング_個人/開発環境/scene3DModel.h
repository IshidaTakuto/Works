//=============================================================================
//
// 3Dモデルオブジェクト処理 [scene3DModel.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE3DMODEL_H_
#define _SCENE3DMODEL_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTS	(2)		// パーツ数

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene3DModel : public CScene
{
public:
	CScene3DModel(int nPriority = 3);
	~CScene3DModel();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);

	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange);
	bool CollisionAttack(D3DXVECTOR3 pos, float fRange);

	int GetLife(void) { return m_nLife; };
	const void SetLife(int nLife) { m_nLife = nLife; };
	const int SubtractLife(int nDamage);

	float GetLength(void) { return m_vtxMax.x; };
	float GetMinVtxY(void) { return m_vtxMin.y; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };

	const void CScene3DModel::BindInfo(LPDIRECT3DTEXTURE9 pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
	LPD3DXMESH				m_pMesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER			m_pBuffMat;			// マテリアル情報へのポインタ
	DWORD					m_nNumMat;			// マテリアル情報の数

	D3DXMATERIAL			*m_pMat;				// マテリアルデータへのポインタ

	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3				m_vtxMin, m_vtxMax;		// モデルの最小値、最大値

	D3DXVECTOR3				m_pos;				// 位置
	D3DXVECTOR3				m_rot;				// 回転
	D3DXVECTOR3				m_posModel;				// モデルの位置
	D3DXVECTOR3				m_rotModel;				// モデルの回転
	int						m_nIdxShadow;					// 影の番号
	int						m_nLife;						// 寿命
};

#endif