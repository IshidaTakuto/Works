//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_PRIORITY		(4)		// 処理の優先番号

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CMotion;
class CShadow;
class CLife2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CScene
{
public:
	typedef struct
	{// キー要素
		float posX;
		float posY;
		float posZ;
		float rotX;
		float rotY;
		float rotZ;
	}KEY;

	typedef struct
	{// キー情報
		int nFrame;		// フレーム
		KEY *pKey;	// キー要素
	}KEY_INFO;

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// 位置の取得
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// 位置の設定

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// 角度の取得
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// 角度の設定

	D3DXVECTOR3 GetMove(void) { return m_move; };				// 移動量の取得
	const void SetMove(D3DXVECTOR3 move) { m_move = move; };	// 移動量の設定

	bool GetSlipFlag(void) { return m_bSlip; };					// 滑走状態の取得
	void SetSlipFlag(bool bSlip) { m_bSlip = bSlip; };			// 滑走状態の設定

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };					// ワールドマトリックスの取得
	const void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ワールドマトリックスの設定

	bool Collision(D3DXVECTOR3 pos, float fRange);

	void Damage(int nDamage);

private:
	void Tilt(void);

	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_rot;				// 回転
	D3DXVECTOR3		m_move;				// 移動量
	float			m_rotDest;			// 目的の角度
	int				m_nCntState;		// ステートのカウンタ
	float			m_fSpeed;			// 速度
	float			m_fSlipSpeed;		// 滑る速度
	float			m_fAngleSlip;		// 滑る方向
	float			m_fJumpSpeed;		// ジャンプ時の速度
	bool			m_bJump;			// ジャンプフラグ
	bool			m_bSlip;			// 滑ってるかどうか
	CModel			**m_pModel;			// モデルクラスのポインタ変数
	int				m_nNumParts;		// パーツ数
	int				m_nCurMotion;		// 現在のモーション
	CMotion			*m_pMotion;			// モーションクラスのポインタ変数
	CShadow			*m_pShadow;			// 影のポインタ変数
	CLife2D			*m_pLife;			// ライフクラスのポインタ変数
	bool			m_bDamage;			// ダメージを受けたかどうか
	bool			m_bMove;			// 動けるかどうか
	int				m_nCntParticle;		// パーティクルを出すまでのカウンタ
	float			m_fAngle;			// 地面と向きの交点の角度
	bool			m_bDash;			// ジャンプ中に空中ダッシュを使ったかどうか
	bool			m_bStageFade;		// ステージ遷移中
};

#endif