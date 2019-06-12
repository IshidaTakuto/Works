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

	static HRESULT Load(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	float GetHeight(D3DXVECTOR3 pos);
	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);		// 頂点の高さの設定

	void OutputVtx(void);					// 頂点情報をファイルに出力する
	void InputVtx(void);					// 頂点情報をファイルから読み込む

	void Output(void);						// ファイルに出力する
	void Input(void);						// ファイルから読み込む

	void SetNumMeshX(int nValue);				// Xの分割数の変更
	void SetNumMeshZ(int nValue);				// Zの分割数の変更
	void SetLengthX(float fValue);				// Xの長さの変更
	void SetLengthZ(float fValue);				// Zの長さの変更

	void SetVtx(void);						// 頂点情報の設定
	void SetNor(VERTEX_3D *pVtx);			// 法線の計算

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; };
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; };

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;				// インデックスバッファへのポインタ
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス

	D3DXVECTOR3				m_pos;					// ポリゴンの位置
	D3DXVECTOR3				m_rot;					// 回転
	float					m_fLengthX;				// 頂点のX軸の距離
	float					m_fLengthZ;				// 頂点のZ軸の距離
	int						m_nMeshX;				// X軸の分割数
	int						m_nMeshZ;				// Z軸の分割数
	int						m_nNumVtx;				// 頂点数
	int						m_nNumPolygon;			// ポリゴン数

	char					m_aFileName[64];		// 頂点情報のファイル名
	static char				m_aTxtFileName[64];		// 読み込むテキストファイル名
};

#endif