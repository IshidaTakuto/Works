//=============================================================================
//
// インデックス3Dオブジェクト処理 [scene3DIndex.h]
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
		TYPE_FIELD = 0,
		TYPE_CYLINDER,
		TYPE_MAX
	}TYPE;

	CScene3DIndex(int nPriority = 3);
	~CScene3DIndex();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	float GetHeight(D3DXVECTOR3 pos);
	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);		// 頂点の高さの設定

	void SaveVtx(void);					// 頂点情報をファイルに出力する
	void LoadVtx(void);					// 頂点情報をファイルから読み込む

	void Save(void);						// ファイルに出力する
	void Load(void);						// ファイルから読み込む

	int GetSplitX(void) { return m_nSplitX; };		// Xの分割数の取得
	void SetSplitX(int nValue);						// Xの分割数の設定

	int GetSplitZ(void) { return m_nSplitZ; };		// Zの分割数の取得
	void SetSplitZ(int nValue);						// Zの分割数の設定

	float GetLengthX(void) { return m_fLengthX; };	// Xの長さの取得
	void SetLengthX(float fValue);					// Xの長さの設定

	float GetLengthZ(void) { return m_fLengthZ; };	// Zの長さの取得
	void SetLengthZ(float fValue);					// Zの長さの設定

	int GetNumVtx(void) { return m_nNumVtx; };		// 頂点数の取得

	void SetVtx(void);						// 頂点情報の設定
	void SetNor(VERTEX_3D *pVtx);			// 法線の計算

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; };
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; };

	void GetInfo(float *pWidth, float *pHeight, float *pDepth, int *pMeshX, int *pMeshY, int *pMeshZ);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;				// インデックスバッファへのポインタ
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス

	D3DXVECTOR3				m_pos;					// ポリゴンの位置
	D3DXVECTOR3				m_rot;					// 回転
	float					m_fLengthX;				// 頂点のX軸の距離
	float					m_fLengthY;				// 頂点のY軸の距離
	float					m_fLengthZ;				// 頂点のZ軸の距離
	int						m_nSplitX;				// X軸の分割数
	int						m_nSplitY;				// Y軸の分割数
	int						m_nSplitZ;				// Z軸の分割数
	int						m_nNumVtx;				// 頂点数
	int						m_nNumPolygon;			// ポリゴン数

	char					m_aFileName[64];		// 頂点情報のファイル名
	char					m_aTexFileName[64];		// テクスチャ名
};

#endif