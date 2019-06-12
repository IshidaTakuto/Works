//*****************************************************************************
//
//     パーティクルの処理[particle.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define PARTICLEDATA_FILENAME     "data/TEXT/EFFECT/particle.txt"           // パーティクルデータが格納されたファイルのパス名
#define PARTICLE_TEXTURE_FILENAME "data/TEXT/EFFECT/particle_texture.txt"   // パーティクルで使用するテクスチャのファイルパス名

//*****************************************************************************
//    前方宣言(実体がない宣言)
//*****************************************************************************

//*****************************************************************************
//    パーティクル用データクラスの作成
//*****************************************************************************
class CParticleData
{
public:     // 誰でもアクセス可能
	CParticleData();
	~CParticleData();

	void SetMaxMove(D3DXVECTOR3 maxMove) { m_MaxMove = maxMove; };
	void SetMinMove(D3DXVECTOR3 minMove) { m_MinMove = minMove; };
	void SetInitCol(D3DXCOLOR InitCol) { m_InitCol = InitCol; };
	void SetChangeCol(D3DXCOLOR ChangeCol) { m_ChangeCol = ChangeCol; };
	void SetMinRadius(float fMinRadius) { m_fMinRadius = fMinRadius; };
	void SetMaxRadius(float fMaxRadius) { m_fMaxRadius = fMaxRadius; };
	void SetChangeRadius(float fChangeRadius) { m_fChangeRadius = fChangeRadius; };
	void SetRotSpeed(float fRotSpeed) { m_fRotSpeed = fRotSpeed; };
	void SetSpread(float fSpread) { m_fSpread = m_fSpread; };
	void SetGravity(float fGravity) { m_fGravity = fGravity; };
	void SetLap(int nLap) { m_nLap = nLap; };
	void SetLife(int nLife) { m_nLife = nLife; };
	void SetAppear(int nAppear) { m_nAppear = nAppear; };
	void SetMinLife(int nMinLife) { m_nMinLife = nMinLife; };
	void SetMaxLife(int nMaxLife) { m_nMaxLife = nMaxLife; };
	void SetTexIdx(int nTexIdx) { m_nTexIdx = nTexIdx; };
	void SetRotPattern(int nRotPattern) { m_nRotPattern = nRotPattern; };
	void SetLoop(bool bLoop) { m_bLoop = bLoop; };
	void SetBouncing(bool bBouncing) { m_bBouncing = bBouncing; };
	void SetDrawAddtive(bool bDrawAddtive) { m_bDrawAddtive = bDrawAddtive; };

	D3DXVECTOR3 GetMaxMove(void) { return m_MaxMove; };
	D3DXVECTOR3 GetMinMove(void) { return m_MinMove; };
	D3DXCOLOR GetInitCol(void) { return m_InitCol; };
	D3DXCOLOR GetChangeCol(void) { return m_ChangeCol; };
	float GetMinRadius(void) { return m_fMinRadius; };
	float GetMaxRadius(void) { return m_fMaxRadius; };
	float GetChangeRadius(void) { return m_fChangeRadius; };
	float GetRotSpeed(void) { return m_fRotSpeed; };
	float GetSpread(void) { return m_fSpread; };
	float GetGravity(void) { return m_fGravity; };
	int GetLap(void) { return m_nLap; };
	int GetLife(void) { return m_nLife; };
	int GetAppear(void) { return m_nAppear; };
	int GetMinLife(void) { return m_nMinLife; };
	int GetMaxLife(void) { return m_nMaxLife; };
	int GetTexIdx(void) { return m_nTexIdx; };
	int GetRotPattern(void) { return m_nRotPattern; };
	bool GetLoop(void) { return m_bLoop; };
	bool GetBouncing(void) { return m_bBouncing; };
	bool GetDrawAddtive(void) { return m_bDrawAddtive; };

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	D3DXVECTOR3   m_MaxMove;         // 移動量の最大値
	D3DXVECTOR3   m_MinMove;         // 移動量の最小値
	D3DXCOLOR     m_InitCol;         // 初期値の色
	D3DXCOLOR     m_ChangeCol;       // 毎フレーム加算する色の変化量
	float         m_fMinRadius;      // 生成時の半径最小値
	float         m_fMaxRadius;      // 生成時の半径最大値
	float         m_fChangeRadius;   // 毎フレーム加算するポリゴンの半径
	float         m_fRotSpeed;       // 毎フレーム加算するポリゴンの回転量
	float         m_fSpread;         // 毎フレーム加算する広がり
	float         m_fGravity;        // 毎フレーム加算する重力
	int           m_nLap;            // １フレームに生成するポリゴン数
	int           m_nLife;			 // エミッタの寿命
	int           m_nAppear;         // パーティクルを生成する間隔
	int           m_nMinLife;		 // 表示時間の最小値
	int           m_nMaxLife;		 // 表示時間の最大値
	int           m_nTexIdx;         // 使用するテクスチャの番号
	int           m_nRotPattern;     // 回転の種類分け用[ 0:なし　1:時計回り　2:反時計回り 3:ランダム ]
	bool          m_bLoop;           // ループするかしないか[ 0ならループしない / 1ならループする ]
	bool          m_bBouncing;		 // バウンドさせるか[ 0:なし 1:あり ]
	bool          m_bDrawAddtive;	 // 加算合成かどうか[ 0:なし 1:あり ]
};

//*****************************************************************************
//    パーティクルマネージャークラスの作成
//*****************************************************************************
class CParticleManager : public CScene
{
public:     // 誰でもアクセス可能
	CParticleManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_PARTICLEMANAGER);
	~CParticleManager();

	static CParticleManager *Create(char *pParticleFileName, char *pTextureFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CParticleData *GetParticleDate(int nIdx);
	LPDIRECT3DTEXTURE9 GetTexture(int nIdx);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	HRESULT ParticleDataLoad(void);
	HRESULT TextureLoad(void);
	void ReleaseParticleData(void);
	void ReleaseTexture(void);

	CParticleData      **m_pParticleData;          // パーティクルデータクラスへのポインタ
	LPDIRECT3DTEXTURE9 *m_pTexture;                // テクスチャへのポインタ
	char               m_aParticleFileName[256];   // パーティクルデータが格納されたファイルのパス名
	char               m_aTextureFileName[256];    // パーティクルで使用するテクスチャ情報が格納されたファイルのパス名
	int                m_nNumEmitter;              // パーティクルの種類
	int                m_nNumTexture;              // 読み込むテクスチャの数
};

//*****************************************************************************
//    パーティクルエミッタクラスの作成
//*****************************************************************************
class CParEmitter : public CScene
{
public:     // 誰でもアクセス可能
	CParEmitter(int nPriority = 3,OBJTYPE objType = OBJTYPE_PARTICLEEMITTER);
	~CParEmitter();

	static CParEmitter *Create(D3DXVECTOR3 pos, int nType, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(int nType);
	void SetPos(D3DXVECTOR3 pos);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void SetParticle(void);

	LPDIRECT3DTEXTURE9 m_pTexture;        // パーティクルに設定するテクスチャ
	D3DXVECTOR3        m_Pos;             // エミッタの座標
	CParticleData      *m_pParticleData;  // パーティクルデータクラスへのポインタ
	int			       m_nLife;			  // エミッタの寿命
	int			       m_nTexture;	      // テクスチャの種類
	int			       m_nAppear;         // 放出する間隔
	int			       m_nLap;            // 1回の放出で出すパーティクルの個数
	int                m_nType;           // 放出するパーティクルの種類
	int                m_nCntEmitter;     // 放出するタイミングを管理するカウンター
	bool		       m_bLoop;           // ループの有無
};

//*****************************************************************************
//    パーティクルクラスの作成
//*****************************************************************************
class CParticle : public CSceneBillboard
{
public:     // 誰でもアクセス可能
	CParticle(int nPriority = 3, OBJTYPE objType = OBJTYPE_PARTICLE);
	~CParticle();

	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fAngle,
		                     D3DXCOLOR ChangeCol, float fGravity, float fChangeRadius,
		                     float fChangeRot, float fBouncing, int nLife, int nRotPattern,
		                     int nType, bool bBound, bool bDrawAddtive, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fAngle,
		         D3DXCOLOR ChangeCol, float fGravity, float fChangeRadius,
		         float fChangeRot, float fBouncing, int nLife, int nRotPattern,
		         int nType, bool bBound, bool bDrawAddtive);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	D3DXCOLOR   m_ChangeCol;     // どれくらい色が変化するか
	D3DXVECTOR3 m_Move;          // 移動量
	float       m_fGravity;      // 重力
	float       m_fChangeRadius; // どれくらい変形するか
	float       m_fChangeRot;    // どれくらい回転するか
	float       m_fBouncing;     // 地面についた際にバウンドする値
	int         m_nLife;         // 寿命
	int         m_nRotPattern;   // 回転の種類
	int         m_nType;         // 種類
	bool        m_bBound;        // バウンドするかどうか
	bool        m_bDrawAddtive;  // 加算合成で描画するかどうか
};

#endif