//*****************************************************************************
//
//     モデルの処理[model.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MODEL_H_
#define _MODEL_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************

//*****************************************************************************
//    モデル管轄クラスの定義
//*****************************************************************************
class CModelManager
{
public:    // 誰でもアクセス可能
	CModelManager();
	~CModelManager();
	static CModelManager *Create(int nNumModel);
	HRESULT Init(void);
	void Uninit(void);

	void SetNumModel(int nNumModel);
	void SetMesh(LPD3DXMESH pMesh, int nIdx);
	void SetBuffMat(LPD3DXBUFFER pBuffMat, int nIdx);
	void SetNumMat(DWORD nNumMat, int nIdx);
	void SetVtxMax(D3DXVECTOR3 nNumMat, int nIdx);
	void SetVtxMin(D3DXVECTOR3 nNumMat, int nIdx);

	LPD3DXMESH GetMesh(int nIdx);
	LPD3DXBUFFER GetBuffMat(int nIdx);
	DWORD GetNumMat(int nIdx);
	D3DXVECTOR3 GetVtxMax(int nIdx);
	D3DXVECTOR3 GetVtxMin(int nIdx);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void ReleaseMesh(void);
	void ReleaseBuffMat(void);
	void ReleaseNumMat(void);
	void ReleaseVtxMax(void);
	void ReleaseVtxMin(void);

	int                   m_nNumModel;    // 読み込んだモデルの数
	LPD3DXMESH            *m_pMesh;       // メッシュ情報へのポインタ
	LPD3DXBUFFER          *m_pBuffMat;    // マテリアル情報へのポインタ
	DWORD                 *m_pNumMat;     // マテリアル情報の数
	D3DXVECTOR3           *m_pVtxMax;     // 頂点座標の最大値へのポインタ
	D3DXVECTOR3           *m_pVtxMin;     // 頂点座標の最小値へのポインタ
};

#endif