//*****************************************************************************
//
//     Xファイルモデルの処理[sceneX.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENEX_H_
#define _SCENEX_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    Xファイルモデルクラスの定義
//*****************************************************************************
class CSceneX : public CScene
{
public:    // 誰でもアクセス可能
	CSceneX(int nPriority = 3, OBJTYPE objType = OBJTYPE_MODEL);
	~CSceneX();
	static CSceneX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void BindModel(const LPDIRECT3DDEVICE9 pDevice, const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posOld);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	D3DXVECTOR3 GetVtxMin(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	LPDIRECT3DTEXTURE9 GetTexture(int nIdxTex);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPD3DXMESH     m_apMesh;      // メッシュ情報へのポインタ
	static LPD3DXBUFFER   m_apBuffMat;   // マテリアル情報へのポインタ
	static DWORD          m_anNumMat;    // マテリアル情報の数
	static D3DXVECTOR3    m_aVtxMin;     // 頂点座標の最小値
	static D3DXVECTOR3    m_aVtxMax;     // 頂点座標の最大値

	LPDIRECT3DTEXTURE9    *m_pTexture;  // テクスチャへのポインタ
	LPD3DXMESH            m_pMesh;      // メッシュ情報へのポインタ
	LPD3DXBUFFER          m_pBuffMat;   // マテリアル情報へのポインタ
	DWORD                 m_nNumMat;    // マテリアル情報の数
	D3DXVECTOR3           m_VtxMin;     // 頂点座標の最小値
	D3DXVECTOR3           m_VtxMax;     // 頂点座標の最大値
	D3DXVECTOR3           m_Pos;        // 現在の座標
	D3DXVECTOR3           m_PosOld;     // 前回の座標
	D3DXVECTOR3           m_Rot;        // 向き
	D3DXMATRIX            m_MtxWorld;   // ワールドマトリックス
};

#endif