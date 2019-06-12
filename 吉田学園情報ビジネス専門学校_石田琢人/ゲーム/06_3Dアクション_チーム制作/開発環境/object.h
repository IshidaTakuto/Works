//*****************************************************************************
//
//     配置物の処理[object.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneX.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MAX_BALLOON	(5)		// バルーンオブジェクトの数

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CShadow;
class CBalloon;
class CPlayer;

//*****************************************************************************
//    オブジェクトクラスの定義
//*****************************************************************************
class CObject : public CSceneX
{
public:    // 誰でもアクセス可能
	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, bool bPush, bool bBreak, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, bool bPush, bool bBreak);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange);

	D3DXVECTOR3 GetRollSpeed(void);
	CShadow *GetShadow(void);
	bool GetCollision(void);
	bool GetBreak(void);
	bool GetPush(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPD3DXMESH     m_apMesh;      // メッシュ情報へのポインタ
	static LPD3DXBUFFER   m_apBuffMat;   // マテリアル情報へのポインタ
	static DWORD          m_anNumMat;    // マテリアル情報の数
	static D3DXVECTOR3    m_aVtxMin;     // 頂点座標の最小値
	static D3DXVECTOR3    m_aVtxMax;     // 頂点座標の最大値

	bool        m_bCollision; // 当たり判定を取るかどうか
	bool        m_bPush;      // 押せるかどうか
	bool        m_bBreak;     // 壊せるかどうか
	D3DXVECTOR3 m_RollSpeed;  // 向きを回転させるスピード
	CShadow     *m_pShadow;   // 影クラスへのポインタ
};

//*****************************************************************************
//    動くオブジェクトクラスの定義
//*****************************************************************************
class CObjectMove : public CObject
{
public:    // 誰でもアクセス可能
	CObjectMove(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTMOVE);
	~CObjectMove();

	static CObjectMove *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 move, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetMoveCurrent(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPD3DXMESH     m_apMesh;      // メッシュ情報へのポインタ
	static LPD3DXBUFFER   m_apBuffMat;   // マテリアル情報へのポインタ
	static DWORD          m_anNumMat;    // マテリアル情報の数
	static D3DXVECTOR3    m_aVtxMin;     // 頂点座標の最小値
	static D3DXVECTOR3    m_aVtxMax;     // 頂点座標の最大値

	D3DXVECTOR3 m_Move;          // 移動量
	D3DXVECTOR3 m_MoveCurrent;   // 現在の移動量
	int         m_nMoveCounter;  // 移動を管理するカウンター
	float       m_fMoveRivision; // 移動量を補正する倍率
};


//*****************************************************************************
//    吹き出し用オブジェクトクラスの定義
//*****************************************************************************
class CObjectBalloon : public CObject
{
public:    // 誰でもアクセス可能
	typedef enum
	{
		BALLOON_TYPE_0 = 0,
		BALLOON_TYPE_1,
		BALLOON_TYPE_2,
		BALLOON_TYPE_3,
		BALLOON_TYPE_4,
		BALLOON_TYPE_MAX
	}BALLOON;

	CObjectBalloon(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTBALLOON);
	~CObjectBalloon();

	static CObjectBalloon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, BALLOON MapBalloon, char *pNextMapName, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, char *pNextMapName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPD3DXMESH     m_apMesh;      // メッシュ情報へのポインタ
	static LPD3DXBUFFER   m_apBuffMat;   // マテリアル情報へのポインタ
	static DWORD          m_anNumMat;    // マテリアル情報の数
	static D3DXVECTOR3    m_aVtxMin;     // 頂点座標の最小値
	static D3DXVECTOR3    m_aVtxMax;     // 頂点座標の最大値

	int m_nType;						 //吹き出しの拡大か縮小かのタイプ
	int m_nCount;						 //吹き出しを1回だけ出すカウンター
	BALLOON m_MapBalloon;
	CBalloon *m_pBalloon[BALLOON_TYPE_MAX];
	char m_aNextMapName[256];
};


//*****************************************************************************
//    ゴール用オブジェクトクラスの定義
//*****************************************************************************
class CObjectGoal : public CObject
{
public:    // 誰でもアクセス可能
	CObjectGoal(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTGOAL);
	~CObjectGoal();

	static CObjectGoal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPD3DXMESH     m_apMesh;      // メッシュ情報へのポインタ
	static LPD3DXBUFFER   m_apBuffMat;   // マテリアル情報へのポインタ
	static DWORD          m_anNumMat;    // マテリアル情報の数
	static D3DXVECTOR3    m_aVtxMin;     // 頂点座標の最小値
	static D3DXVECTOR3    m_aVtxMax;     // 頂点座標の最大値
};


//*****************************************************************************
//    ジップライン用オブジェクトクラスの定義
//*****************************************************************************
class CObjectTarget : public CObject
{
public:    // 誰でもアクセス可能
	typedef enum
	{
		STATE_NONE = 0,  // 何もしていない状態
		STATE_CARRY,     // プレイヤーを運んでいる状態
		STATE_FINISH,    // 運び終わった状態
		STATE_MAX
	}STATE;

	CObjectTarget(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTTARGET);
	~CObjectTarget();

	static CObjectTarget *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 PosTarget, int nMaxCarry, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 PosTarget, int nMaxCarry);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange, CScene *pScene);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Carry(void);

	static LPD3DXMESH     m_apMesh;      // メッシュ情報へのポインタ
	static LPD3DXBUFFER   m_apBuffMat;   // マテリアル情報へのポインタ
	static DWORD          m_anNumMat;    // マテリアル情報の数
	static D3DXVECTOR3    m_aVtxMin;     // 頂点座標の最小値
	static D3DXVECTOR3    m_aVtxMax;     // 頂点座標の最大値

	int         m_nMaxCarryTime;     // いつまで運ぶかの時間
	int         m_nCounter;          // 運びを管理するカウンター
	STATE       m_State;             // 状態
	D3DXVECTOR3 m_PosTarget;         // プレイヤーを運ぶ位置
	D3DXVECTOR3 m_DiffPosTarget;     // 目的の位置との差分
	CPlayer     *m_pPlayer;          // プレイヤークラスへのポインタ
};

#endif
