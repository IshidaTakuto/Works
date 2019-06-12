//=============================================================================
//
// ゲーム処理 [game.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "enemy.h"
#include "field.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_HALF			(SCREEN_WIDTH / 2)		// 画面の横半分
#define MAX_ENEMY_APPEAR	(32)					// 敵の出現数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;
class CPause;
class CPolygon;
class CString;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame
{
public:
	typedef enum
	{
		STATE_NONE = 0,	// 何もしていない状態
		STATE_NORMAL,	// 通常状態
		STATE_END,		// ゲームオーバー
		STATE_CLEAR,	// ゲームクリア
		STATE_MAX
	}State;

	CGame();
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void NullPlayer(void);	// プレイヤーの削除

	static const void SetGameState(State state);
	State GetGameState(void) { return m_state; };

	const static void SetPause(bool bPause) { m_bPause = bPause; };
	static bool GetPause(void) { return m_bPause; };

	const static void SetPlayer(CPlayer *pPlayer);
	static CPlayer *GetPlayer(void) { return m_pPlayer; };				// プレイヤーの取得
	static CField *GetField(int nIndex) { return m_pField[nIndex]; };	// フィールドの取得

private:
	static CPlayer	*m_pPlayer;				// プレイヤークラスのポインタ変数
	static CField	*m_pField[MAX_FIELD];	// フィールドクラスのポインタ変数
	CPolygon *m_apPolygon;					// ポリゴンクラスのポインタ変数
	CString *m_pString;						// 文字表示クラスのポインタ変数
	CPause *m_pause;						// ポーズクラスのポインタ変数
	static bool m_bPause;					// ポーズON/OFFの切替
	static State m_state;					// ゲームの状態
	static int m_stateCounter;				// 遷移するまでのカウンタ
	int	m_nCntEnemyAppear;					// 敵を出現させるカウンタ
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 敵の情報
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CEnemyInfo
{
public:
	CEnemyInfo();
	~CEnemyInfo();

	static HRESULT LoadEnemy(void);		// 敵の情報を読み込む

	static CEnemyInfo *GetInfo(int nInfo) { return &m_aEnemyInfo[nInfo]; };

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// 位置
	int GetAppearTime(void) { return m_nAppearTime; };	// 出現する時間
	CEnemy::TYPE GeType(void) { return m_type; };		// 種類
	bool GetUse(void) { return m_bUse; };
	void SetUse(bool bUse) { m_bUse = bUse; };

private:
	static CEnemyInfo m_aEnemyInfo[MAX_ENEMY];	// 敵の情報

	D3DXVECTOR3 m_pos;		// 位置
	int m_nAppearTime;		// 出現する時間
	CEnemy::TYPE m_type;	// 種類
	bool m_bUse;

};

#endif