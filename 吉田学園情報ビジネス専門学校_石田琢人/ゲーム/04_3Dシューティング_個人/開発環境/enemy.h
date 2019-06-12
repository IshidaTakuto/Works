//=============================================================================
//
// 敵処理 [enemy.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene3DModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY		(128)
#define ENEMY_PRIORITY	(4)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CShadow;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy : public CScene3DModel
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,	// 通常状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_DEATH,			// 死亡状態
		STATE_MAX,
	}STATE;

	typedef enum
	{// 敵の種類
		TYPE_AIRCRAFT = 0,
		TYPE_SHIP,
		TYPE_TANK,
		TYPE_MAX
	}TYPE;

	CEnemy(int nPriority = ENEMY_PRIORITY);
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemy* Create(D3DXVECTOR3 pos, TYPE type);

	static HRESULT Load(void);				// テクスチャを読み込む（managerにいれる）
	static void Unload(void);				// 読み込んだテクスチャを解放

	STATE GetState(void) { return m_state; };
	const void SetState(STATE state) { m_state = state; };

	TYPE GetType(void) { return m_type; };

	static int GetNumEnemy(void) { return m_nNumEnemy; };

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TYPE_MAX];	// 共有テクスチャへのポインタ
	static LPD3DXMESH			m_pMesh[TYPE_MAX];		// 共有メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[TYPE_MAX];	// 共有マテリアル情報へのポインタ
	static DWORD				m_nNumMat[TYPE_MAX];	// 共有マテリアル情報の数

	static int					m_nNumEnemy;			// 敵の総数
	D3DXVECTOR3					m_move;					// 移動量
	float						m_posOriginalZ;			// Ｚ軸の本来の位置
	int							m_nCntState;			// ステートのカウンタ
	int							m_nCntBullet;			// 弾を撃つまでのカウンタ
	bool						m_bMoveAngle;			// 移動の角度
	STATE						m_state;				// 現在の状態
	TYPE						m_type;					// 敵の種類
	CShadow						*m_pShadow;				// 影のポインタ変数
};

#endif