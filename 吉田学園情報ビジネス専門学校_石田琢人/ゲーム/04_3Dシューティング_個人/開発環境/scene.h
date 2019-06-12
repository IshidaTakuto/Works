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
#define MAX_OBJECT		(2000)		// ポリゴンの最大数
#define NUM_PRIORITY	(8)			// 優先順位の数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene
{
public:
	typedef enum
	{// 種類
		TYPE_2D = 0,
		TYPE_MAX
	}TYPE;

	typedef enum
	{
		OBJTYPE_NONE = 0,		// 初期値
		OBJTYPE_PLAYER,			// プレイヤー
		OBJTYPE_ENEMY,			// 敵
		OBJTYPE_BULLET,			// 弾
		OBJTYPE_FADE,			// フェード
		OBJTYPE_FIELD,			// 地面
		OBJTYPE_CYLINDER,		// シリンダー
		OBJTYPE_EFFECT,			// エフェクト
		OBJTYPE_MODEL,			// モデル
		OBJTYPE_PARTICLE,		// パーティクル
		OBJTYPE_UI,				// ＵＩ
		OBJTYPE_WALL,			// 壁
		OBJTYPE_SHADOW,			// 影
		OBJTYPE_MAX				// オブジェクトの種類の最大数
	}OBJTYPE;

	CScene(int nPriority = 3);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	void SetObjType(OBJTYPE objType) { m_objType = objType; };
	OBJTYPE GetObjType(void) { return m_objType; };
	static CScene *GetScene(int nPriority, int nIndex) { return m_apScene[nPriority][nIndex]; };

protected:
	void Release(void);

private:
	static CScene *m_apScene[NUM_PRIORITY][MAX_OBJECT];	// 情報
	static int m_nNumAll;								// 総数
	int	m_nIndex;										// 自分自身の番号
	int m_nPriority;									// 優先順位
	OBJTYPE m_objType;									// オブジェクトの種類
};

#endif