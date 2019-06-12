//=============================================================================
//
// ゲーム処理 [game.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "stage.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAME_PRIORITY		(7)				// 優先番号
#define MAX_ENEMY_APPEAR	(32)			// 敵の出現数
#define GRAVITY				(0.5f)			// 重力
#define GAME_NUM_BG			(2)				// 背景の数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPause;
class CBg;
class CTimer;
class CLogoTutorial;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,	// 何もしていない状態
		STATE_NORMAL,	// 通常状態
		STATE_END,		// ゲームオーバー
		STATE_CLEAR,	// ゲームクリア
		STATE_MAX
	}STATE;

	CGame(int nPriority = GAME_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_GAME);
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static const void SetGameState(STATE state);
	STATE GetGameState(void) { return m_state; };

	const void SetPauseBool(bool bPause) { m_bPause = bPause; };
	bool GetPauseBool(void) { return m_bPause; };

	CPause *GetPause(void) { return m_pPause; };

	static int GetStage(void) { return m_nCurStage; };
	static void NextStage(void);
	static void SetStage(int stage);

	static void MoveMountain(float fMove);

private:
	CPause *m_pPause;				// ポーズクラスのポインタ変数
	static STATE m_state;			// ゲームの状態
	bool m_bPause;					// ポーズON/OFFの切替
	int m_modeCounter;				// 遷移するまでのカウンタ
	int	m_nCntEnemyAppear;			// 敵を出現させるカウンタ
	CBg *m_pBg[GAME_NUM_BG];		// 背景のポインタ変数
	static CBg *m_pMountain;		// 背景の山
	CTimer *m_pTimer;				// タイマークラスのポインタ変数
	static int m_nCurStage;			// 現在のステージ
	CLogoTutorial	*m_pTutorial;	// 操作説明表示クラスのポインタ変数
};

#endif