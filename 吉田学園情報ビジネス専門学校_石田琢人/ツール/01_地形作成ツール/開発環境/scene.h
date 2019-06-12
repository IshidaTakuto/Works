//=============================================================================
//
// オブジェクト処理 [scene.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_OBJECT		(128)		// ポリゴンの最大数
#define NUM_PRIORITY	(8)			// 優先順位の数

//=====================================
// クラス定義
//=====================================
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,		// 初期値
		OBJTYPE_PLAYER,			// プレイヤー
		OBJTYPE_MODEL,			// モデル
		OBJTYPE_FIELD,			// フィールド
		OBJTYPE_BULLET,			// 弾
		OBJTYPE_SHADOW,			// 影
		OBJTYPE_CYLINDER,		// シリンダー
		OBJTYPE_MAX				// オブジェクトの種類の最大数
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseDeath(void);

	void SwapPriority(int nPriority);

	void SetObjType(OBJTYPE objType) { m_objType = objType; };
	OBJTYPE GetObjType(void) { return m_objType; };

	//CScene *GetScene(int nIdx) {};

protected:
	void Release(void);

private:
	static CScene *m_apTop[NUM_PRIORITY];		// 先頭のオブジェクトへのポインタ
	static CScene *m_apCur[NUM_PRIORITY];		// 現在のオブジェクトへのポインタ（最後尾）
	CScene *m_pPrev;			// 前のオブジェクトへのポインタ
	CScene *m_pNext;			// 次のオブジェクトへのポインタ
	int m_nPriority;			// 優先順位
	static int m_nNumAll;		// 総数
	OBJTYPE m_objType;			// オブジェクトの種類
	bool m_bDeath;				// 死亡フラグ
	int m_nID;					// デバッグ用自分の番号
};

#endif