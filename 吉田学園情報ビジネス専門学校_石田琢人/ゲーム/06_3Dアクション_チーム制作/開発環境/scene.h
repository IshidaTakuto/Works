//*****************************************************************************
//
//     シーンの処理[scene.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MAX_PRIORITY_NUM (8)      // 処理の優先順位の最大数
#define MAX_SCENE        (5000)   // シーンを作る数

//*****************************************************************************
//    シーンクラスの定義
//*****************************************************************************
class CScene
{
public:    // 誰でもアクセス可能
	//------------------------
	//  オブジェクトの種類
	//------------------------
	typedef enum
	{
		OBJTYPE_NONE = -1,        // 無し
		OBJTYPE_TITLE,            // タイトル
		OBJTYPE_TUTORIAL,         // チュートリアル
		OBJTYPE_GAME,             // ゲーム
		OBJTYPE_RESULT,           // リザルト
		OBJTYPE_RANKING,          // ランキング
		OBJTYPE_PAUSE,            // ポーズ
		OBJTYPE_NUMBER,           // 数字ポリゴン(2D)
		OBJTYPE_LOGO,             // 文字ポリゴン(2D)
		OBJTYPE_2DPOLYGON,        // 2Dポリゴン
		OBJTYPE_TIMER,            // タイマー
		OBJTYPE_3DPOLYGON,        // 3Dポリゴン
		OBJTYPE_MODEL,            // モデル
		OBJTYPE_BILLBOARD,        // ビルボード
		OBJTYPE_MESH,             // メッシュ
		OBJTYPE_CYLINDER,         // シリンダー
		OBJTYPE_DOME,             // ドーム
		OBJTYPE_EFFECT,           // エフェクト
		OBJTYPE_SHADOW,           // 影
		OBJTYPE_PLAYER,           // プレイヤー
		OBJTYPE_BULLET,           // 弾
		OBJTYPE_FIELD,            // フィールド
		OBJTYPE_WATER,            // 水
		OBJTYPE_OBJECT,           // 配置物
		OBJTYPE_OBJECTMOVE,       // 動く配置物
		OBJTYPE_OBJECTBALLOON,    // 吹き出しが出る配置物
		OBJTYPE_BALLOON,	      // 吹き出し
		OBJTYPE_OBJECTGOAL,	      // ゴール用のオブジェクト
		OBJTYPE_OBJECTTARGET,     // ジップライン用のオブジェクト
		OBJTYPE_PARTICLEMANAGER,  // パーティクル管理用
		OBJTYPE_PARTICLEEMITTER,  // パーティクルエミッタ
		OBJTYPE_PARTICLE,         // パーティクル
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static int GetNumAll(void);

	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);
	static void DeathCheck(void);

	static CScene *GetScene(int nPriority);

	void SetObjType(OBJTYPE ObjType);
	OBJTYPE GetObjType(void);
	CScene *GetNext(void);
	CScene *GetPrev(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void Release(void);

private:   // このクラスだけがアクセス可能

	static int    m_nNumAll;                      // シーンの最大数
	static CScene *m_apTop[MAX_PRIORITY_NUM];     // 先頭のシーンクラスへのポインタ(開始位置がわからなくなるので保存する)
	static CScene *m_apCur[MAX_PRIORITY_NUM];     // 現在の最後尾シーンクラスへのポインタ(新しいシーンクラスへのポインタを追加するために保存)
	CScene        *m_pPrev;                       // 自分の前のシーンクラスへのポインタ
	CScene        *m_pNext;                       // 自分の次のシーンクラスへのポインタ
	bool          m_bDeath;                       // 死亡フラグ
	OBJTYPE       m_ObjType;                      // オブジェクトの種類
	int           m_nPriority;                    // 優先順位の番号
};

#endif