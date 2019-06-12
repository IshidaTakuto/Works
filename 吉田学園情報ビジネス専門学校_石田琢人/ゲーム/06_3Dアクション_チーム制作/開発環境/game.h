//*****************************************************************************
//
//     ゲームの処理[game.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMap;
class CScene2DSquare;

//*****************************************************************************
//    ゲームクラスの定義
//*****************************************************************************
class CGame : public CScene
{
public:     // 誰でもアクセス可能
	//------------------------
	//  ゲームの状態
	//------------------------
	typedef enum
	{
		STATE_NORMAL = 0,    // 通常状態
		STATE_CHALLENGE,     // マップに挑戦している状態
		STATE_PLAYERFALL,    // プレイヤーが落ちた状態
		STATE_MAPCHANGE,     // マップ切り替え状態
		STATE_MAPCLEAR,      // マップクリア状態
		STATE_END,           // 終了状態
		STATE_MAX
	}STATE;

	CGame(int nPriority = 0, OBJTYPE objType = OBJTYPE_GAME);
	~CGame();

	static CGame *Create(void);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	STATE GetState(void);
	bool GetMapClear(void);

	void SetState(STATE state);
	void SetMapClear(bool bMapClear);
	void SetMapName(char *aMapFileName);
	void SetMap(void);
	void ClearMapEvent(void);
	void PlayerFall(void);
	void PlayerRespawnEvent(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9 m_pTexture;
	CScene2DSquare *m_pScene2D; // 操作方法表示用のポリゴン
	CMap  *m_pMap;              // マップ情報へのポインタ
	STATE m_State;              // ゲームの状態
	char  m_aMapFileName[256];  // マップのスクリプトファイル名
	int   m_nStatementCounter;  // 状態を管理するカウンター
	int   m_nNumClearMap;       // クリアしたマップ数
	int   m_nMapClearBonus;     // マップクリアボーナス
	bool  m_bMapClear;          // マップをクリアしたかどうか
};

#endif