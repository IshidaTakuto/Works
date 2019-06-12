//*****************************************************************************
//
//     プレイヤーの処理[player.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneX.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define PLAYER_PRIORITY (5)    // プレイヤーの処理優先順位

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CShadow;
class CModel;
class CMotionManager;

//*****************************************************************************
//    プレイヤークラスの定義
//*****************************************************************************
class CPlayer : public CScene
{
public:    // 誰でもアクセス可能
	//------------------------
	//  状態
	//------------------------
	typedef enum
	{
		STATE_NORMAL = 0,  // 通常状態
		STATE_MOVE_WALK,   // 歩く状態
		STATE_MOVE_RUN,    // 走る状態
		STATE_JUMP,        // ジャンプしている状態
		STATE_LANDING,     // 着地している状態
		STATE_PUSH,        // 何かを押している状態
		STATE_PANCH,       // パンチをしている状態
		STATE_KICK,        // キックをしている状態
		STATE_ZIPLINE,     // ジップラインにつかまっている状態
		STATE_MAX
	}STATE;

	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority = 3);
	static HRESULT Load(char *pFileName);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void SetDefault(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetMove(void);
	float GetColHeight(void);
	bool GetJump(void);

	void SetState(const STATE state);
	void SetMove(const D3DXVECTOR3 move);
	void SetJump(const bool bJump);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posOld);
	void SetRot(const D3DXVECTOR3 rot);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Movement(void);
	void AddMovement(void);
	bool Collision(void);
	void Action(void);
	void Statement(void);
	void Landing(void);
	bool AttackCollision(D3DXVECTOR3 pos, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax);
	void CreateEffect(void);
	void Sound(void);

	static LPD3DXMESH     *m_apMesh;         // メッシュ情報へのポインタ
	static LPD3DXBUFFER   *m_apBuffMat;      // マテリアル情報へのポインタ
	static DWORD          *m_anNumMat;       // マテリアル情報の数
	static D3DXVECTOR3    *m_aVtxMin;        // 頂点座標の最小値
	static D3DXVECTOR3    *m_aVtxMax;        // 頂点座標の最大値
	static int            m_nNumModel;       // プレイヤーの使用するパーツ数
	static char           m_aFileName[256];  // プレイヤーのデータファイル名

	CModel          **m_apModel;          // モデルクラスへのポインタ
	int             m_nNumPart;           // 使用するパーツ数
	CMotionManager  *m_pMotionManager;    // モーション管理クラスへのポインタ
	D3DXVECTOR3     m_Pos;                // 現在の座標
	D3DXVECTOR3     m_PosOld;             // 前回の座標
	D3DXVECTOR3     m_Move;               // 移動量
	D3DXVECTOR3     m_Rot;                // 現在の向き
	D3DXVECTOR3     m_DiffAngle;          // 現在の向きと目的の向きの差分
	D3DXVECTOR3     m_DestAngle;          // 目的の向き
	D3DXMATRIX      m_MtxWorld;           // ワールドマトリックス
	STATE           m_State;              // 状態
	float           m_fGravity;           // 重力
	float           m_fJumpPower;         // ジャンプ力
	bool            m_bJump;              // ジャンプしたかどうか
	bool            m_bAction;            // アクション状態であるかどうか
	float           m_fAccel;             // 加速度
	float           m_fInertia;           // 慣性
	float           m_fRivisionRot;       // 向きを補正する倍率
	float           m_fColHeight;         // 当たり判定をとる高さ
	float           m_fMaxWalkSpeed;      // プレイヤーが歩くスピードの最大値
	CShadow         *m_pShadow;           // 影クラスへのポインタ
	CScene          *m_pLandScene;        // プレイヤーが乗っているクラスへのポインタ
	bool            m_bMove;              // 移動しているかどうか
	bool            m_bLand;              // オブジェクトに乗っているかどうか
	bool            m_bPush;               // オブジェクトを押しているかどうか
	bool            m_bCol;
	D3DXVECTOR3     m_OffsetPosStd;       // オブジェクトからのオフセット基準位置
	D3DXVECTOR3     m_OffsetRot;          // オブジェクトからのオフセット向き
	D3DXVECTOR3     m_OffsetRotStart;     // オブジェクトに乗った時の向き(オブジェクトに乗ってから向きを加算する)
	D3DXVECTOR3     m_OffsetPosAdd;       // 現在のオブジェクトからのオフセット加算位置
	D3DXVECTOR3     m_OffsetPosAddOld;    // 前回のオブジェクトからのオフセット加算位置
};

#if 0 // モデル単体のプレイヤー処理なので今回これは使わないです
//*****************************************************************************
//    プレイヤー(モデル単体)クラスの定義
//*****************************************************************************
class CSceneXPlayer : public CSceneX
{
public:    // 誰でもアクセス可能
	CSceneXPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CSceneXPlayer();

	static CSceneXPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	D3DXVECTOR3 GetMove(void);
	bool GetJump(void);

	void SetMove(const D3DXVECTOR3 move);
	void SetJump(const bool bJump);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Movement(void);
	bool Collision(void);
	void Action(void);

	static LPD3DXMESH     m_apMesh;      // メッシュ情報へのポインタ
	static LPD3DXBUFFER   m_apBuffMat;   // マテリアル情報へのポインタ
	static DWORD          m_anNumMat;    // マテリアル情報の数

	D3DXVECTOR3 m_Move;       // 移動量
	D3DXVECTOR3 m_DiffAngle;  // 現在の向きと目的の向きの差分
	D3DXVECTOR3 m_DestAngle;  // 目的の向き
	float       m_fGravity;   // 重力
	float       m_fJumpPower; // ジャンプ力
	bool        m_bJump;      // ジャンプしたかどうか
	float       m_fAccel;     // 加速度
	float       m_fInertia;   // 慣性
	CShadow     *m_pShadow;   // 影クラスへのポインタ
};
#endif

#endif
