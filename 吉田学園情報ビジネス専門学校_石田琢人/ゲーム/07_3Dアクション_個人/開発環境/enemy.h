//=============================================================================
//
// 敵処理 [enemy.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene.h"
#include "motion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_PRIORITY	(4)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CShadow;
class CLife3D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy : public CScene
{
public:
	CEnemy(int nPriority = ENEMY_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_ENEMY);
	~CEnemy();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemy* Create(D3DXVECTOR3 pos, int nType);

	static HRESULT Load(void);				// テクスチャを読み込む（managerにいれる）
	static void Unload(void);				// 読み込んだテクスチャを解放

	int GetType(void) { return m_nType; };

	float GetAttackRange(void) { return m_fAttackRange; };
	void SetAttackRange(float fAttackRange) { m_fAttackRange = fAttackRange; };

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	void Damage(int nDamage, int nMotion);		// 攻撃を受けたときの処理
	bool Collision(D3DXVECTOR3 pos, float fRange);	// 当たり判定

private:
	typedef struct
	{// ファイル読み込み構造体
		int nIdx;			// モデル番号
		int nParent;		// 親番号
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
	}DATA;

	void GetModelData(CModel **&pModel);

	static int		*m_pNumParts;				// パーツ数
	static char		***m_pModelName;			// モデル名
	static DATA		**m_pFileData;				// ファイル読み取り
	static int		m_nNumType;					// 種類の数

	D3DXMATRIX		m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3		m_pos;						// 位置
	D3DXVECTOR3		m_rot;						// 回転
	float			m_fRotDest;					// 目的の角度
	D3DXVECTOR3		m_move;						// 移動量
	int				m_nCntDamage[ATTACK_MOTION];	// ダメージのカウンタ
	int				m_nCntBullet;				// 弾を撃つまでのカウンタ
	int				m_nType;					// 敵の種類
	CShadow			*m_pShadow;					// 影のポインタ変数
	CModel			**m_pModel;					// モデルクラスのポインタ変数
	CMotion			*m_pMotion;					// モーションクラスのポインタ変数
	float			m_fWalkRange;				// 索敵範囲
	float			m_fAttackRange;				// 攻撃をする範囲
	int				m_nIntervalAttack;			// 攻撃間隔
	bool			m_bAttack;					// 攻撃フラグ
	CLife3D			*m_pLife;					// 3D体力クラスのポインタ変数
	bool			m_bDeath;					// 死亡フラグ
	float			m_fAlpha;					// 透明度
	bool			m_bDamage[ATTACK_MOTION];	// ダメージ状態
};

#endif