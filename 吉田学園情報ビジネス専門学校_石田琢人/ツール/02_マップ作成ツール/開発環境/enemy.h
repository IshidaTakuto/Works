//=============================================================================
//
// 敵処理 [enemy.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_PRIORITY	(4)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CMotion;
class CShadow;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy : public CScene
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,	// 通常状態
		STATE_WALK,			// 移動状態
		STATE_ATTACK,		// 攻撃状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_DEATH,		// 死亡状態
		STATE_MAX,
	}STATE;

	CEnemy(int nPriority = ENEMY_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_ENEMY);
	~CEnemy();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemy* Create(D3DXVECTOR3 pos, int nType);

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

	static char **GetModelName(int nType) { return m_pModelName[nType]; };
	static int GetNumParts(int nType) { return m_pNumParts[nType]; };

	static void GetInfo(int nType, CModel **pModel);		// モデル情報の設定

	int GetType(void) { return m_nType; };

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// 位置の設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// 位置の取得

	D3DXVECTOR3 GetRot(void) { return m_rot; };			// 向きの設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// 向きの取得

	void Range(void);

private:
	typedef struct
	{// ファイル読み込み構造体
		int nIdx;			// モデル番号
		int nParent;		// 親番号
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
	}DATA;

	bool Collision(D3DXVECTOR3 pos, float fRange);	// 当たり判定

	static int		*m_pNumParts;				// パーツ数
	static char		***m_pModelName;			// モデル名
	static DATA		**m_pFileData;				// ファイル読み取り

	D3DXMATRIX		m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3		m_pos;						// 位置
	D3DXVECTOR3		m_rot;						// 回転
	int				m_nType;					// 敵の種類
	CShadow			*m_pShadow;					// 影のポインタ変数
	CModel			**m_pModel;					// モデルクラスのポインタ変数
	CMotion			*m_pMotion;					// モーションクラスのポインタ変数
	bool			m_bRange;					// 範囲内かどうか
};

#endif