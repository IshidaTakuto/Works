//*****************************************************************************
//
//     モーションの処理[motion.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
//    キーフレームクラスの定義
//*****************************************************************************
class CKeyframe
{
public:    // 誰でもアクセス可能
	CKeyframe();
	~CKeyframe();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetAddPos(D3DXVECTOR3 pos);
	void SetDestPos(D3DXVECTOR3 pos);
	void SetDestRot(D3DXVECTOR3 rot);
	void SetDiffRot(D3DXVECTOR3 rotDiff);

	D3DXVECTOR3 GetAddPos(void);
	D3DXVECTOR3 GetDestPos(void);
	D3DXVECTOR3 GetDestRot(void);
	D3DXVECTOR3 GetDiffRot(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_AddPos;       // 現在のオフセット位置に加える値
	D3DXVECTOR3 m_DestPos;      // 目的のオフセット位置
	D3DXVECTOR3 m_DestRot;      // 目的のオフセット向き
	D3DXVECTOR3 m_DiffRot;      // 差分の向き
};

//*****************************************************************************
//    モーションクラスの定義
//*****************************************************************************
class CMotion
{
public:    // 誰でもアクセス可能
	//------------------------
	//  モーションの状態
	//------------------------
	typedef enum
	{
		STATE_NORMAL = 0,  // 通常状態
		STATE_STOP,        // 停止状態
		STATE_SWITCH,      // 切り替え状態
		STATE_MAX
	}STATE;

	CMotion();
	~CMotion();

	static CMotion *Create(int nNumModel, int nNumKey, float fBlendMag, bool bLoop, int *pPlaybackKey);
	HRESULT Init(int nNumModel, int nNumKey, float fBlendMag, bool bLoop, int *pPlaybackKey);
	void Uninit(int nNumModel);
	void Update(CModel **pModel, int nNumModel, int *pKey, int *pCounter, int *pBlendCounter);

	CKeyframe ***GetKeyframe(void);
	STATE GetState(void);
	int GetNumKey(void);
	bool GetLoop(void);
	int GetPlayBackKey(int nIdx);
	float GetBlendMag(void);

	void SetState(STATE state);
	void SwitchKey(CModel **pModel, int nNumModel, int nKey);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CKeyframe   ***m_pKeyframe;    // キーフレームの情報(そのモーションのキーフレームの数とモデルの数分動的に確保)
	STATE       m_State;           // モーションの状態
	int         *m_pPlaybackKey;   // 再生フレームの総数
	int         m_nNumKey;         // キーフレームの数
	float       m_fBlendMag;       // モーションブレンド倍率
	bool        m_bLoop;           // ループするかしないか
};

//*****************************************************************************
//    モーション管理クラスの定義
//*****************************************************************************
class CMotionManager
{
public:    // 誰でもアクセス可能
	CMotionManager();
	~CMotionManager();

	static CMotionManager *Create(int nNumMotion, int nNumModel, char *aFileName);
	HRESULT Init(int nNumMotion, int nNumModel, char *aFileName);
	void Uninit(void);
	void Update(CModel **pModel);

	void SetKey(int nKey);
	void SetNumber(int nNumber);

	CMotion **GetMotion(void);
	int GetKey(void);
	int GetCounter(void);
	int GetNumModel(void);
	int GetNumber(void);

	void SwitchMotion(CModel **pModel, int nNumber);

	void Save(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CMotion **m_pMotion;     // モーションクラスへのポインタ
	int     m_nKey;          // 現在のキーフレーム
	int     m_nCounter;      // 現在の再生フレーム数
	int     m_nBlendCounter; // ブレンドを管理するカウンター
	int     m_nNumAll;       // モーションの総数
	int     m_nNumber;       // 現在のモーション番号
	int     m_nNumberOld;    // 前回のモーション番号
	int     m_nNumModel;     // モデルの数
};

#endif