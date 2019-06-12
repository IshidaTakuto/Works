//=============================================================================
//
// 壁処理 [wall.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_WALL_ROCK	(30)	// 1つの壁にある岩の数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene3DModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CWall : public CScene
{
public:

	CWall(int nPriority = 3);
	~CWall();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CWall *Create(int nPos);

	static HRESULT Load(void);		// テクスチャを読み込む
	static void Unload(void);		// 読み込んだテクスチャを解放

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// 共有テクスチャへのポインタ
	static LPD3DXMESH			m_pMesh;		// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat;		// マテリアル情報へのポインタ
	static DWORD				m_nNumMat;		// マテリアル情報の数

	CScene3DModel				*m_pModel[MAX_WALL_ROCK];	// モデルクラスへのポインタ変数
	int							m_nPos;						// 基準のX座標
};

#endif