//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "sceneX.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTS_PLAYER	(1)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CSceneX
{
public:
	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);				// テクスチャを読み込む（managerにいれる）
	static void Unload(void);				// 読み込んだテクスチャを解放

	static CPlayer *Create(void);
	D3DXVECTOR3 GetPos(void) { return m_pos; };

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_PARTS_PLAYER];	// テクスチャへのポインタ
	static LPD3DXMESH				m_pMesh[MAX_PARTS_PLAYER];		// メッシュ情報へのポインタ
	static LPD3DXBUFFER				m_pBuffMat[MAX_PARTS_PLAYER];	// マテリアル情報へのポインタ
	static DWORD					m_nNumMat[MAX_PARTS_PLAYER];	// マテリアル情報の数

	D3DXVECTOR3	m_pos;		// 位置
	float		m_fSpeed;	// 速度調整
	float		m_fRange;	// 選択範囲
	float		m_fHeight;	// 上げる高さ
};

#endif
