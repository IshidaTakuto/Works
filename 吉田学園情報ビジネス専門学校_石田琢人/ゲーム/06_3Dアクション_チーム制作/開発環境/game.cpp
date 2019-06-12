//*****************************************************************************
//
//     ゲームの処理[game.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "result.h"
#include "pause.h"
#include "renderer.h"
#include "camera.h"

#include "scene2D.h"
#include "scene3D.h"
#include "sceneBillboard.h"
#include "scene3DMesh.h"
#include "sceneX.h"
#include "player.h"
#include "field.h"
#include "shadow.h"
#include "object.h"
#include "cylinder.h"
#include "dome.h"
#include "map.h"
#include "balloon.h"
#include "timer.h"
#include "number.h"
#include "water.h"
#include "pause.h"
#include "logo.h"
#include "result.h"
#include "particle.h"

#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define GAME_MANAGER_INITFILENAME   "data/TEXT/MODE/game.ini"       // ゲームのシステムファイル名
#define GAME_MAPSELECT_FILENAME     "data/TEXT/MAP/map_select.txt"  // マップ選択画面のファイル名
#define GAME_MAPCLEAR_CHANGETIMING  (60)                            // マップクリア状態になってからマップ選択画面に戻るまでの時間
#define GAME_MODE_CHANGE_TIMING     (60)                            // 終了状態になってから画面遷移するまでの時間
#define GAME_PLAYER_RESPAWNTIMING   (120)                           // プレイヤーが死んだ状態になってからマップ攻略画面に戻るまでの時間

#define GAME_TEXTURE_NAME           "data/TEXTURE/GAME/sousa.png"
#define GAME_POLYGON_POS            (D3DXVECTOR3(150.0f,75.0f,0.0f))
#define GAME_POLYGON_COL            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_POLYGON_WIDTH          (150.0f)
#define GAME_POLYGON_HEIGHT         (75.0f)

// 値を読み取るパス名
#define PLAYER_FILENAME "PLAYER_FILENAME"
#define MAP_FILENAME    "MAP_FILENAME"

#define NUM_MODEL        "NUM_MODEL = "
#define MODEL_FILENAME   "MODEL_FILENAME = "

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CGame::CGame(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;        // テクスチャへのポインタ
	m_pScene2D = NULL;        // 操作方法表示用のポリゴン
	m_nStatementCounter = 0;  // 状態管理カウンター
	m_nNumClearMap = 0;       // クリアしたマップ数
	m_nMapClearBonus = 0;     // クリアボーナス
	m_State = STATE_NORMAL;   // 状態
	m_pMap = NULL;            // マップ情報へのポインタ
	m_bMapClear = false;      // マップをクリアしたかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGame *CGame::Create(void)
{
	CGame *pGame = NULL;  // ゲームクラス型のポインタ
	if (pGame == NULL)
	{// メモリが確保できる状態にある
		pGame = new CGame;
		if (pGame != NULL)
		{// メモリが確保できた
			// 初期化処理を行う
			pGame->Init();
		}
		else
		{// メモリが確保できなかった
			return NULL;
		}
	}
	else
	{// メモリが確保できる状態にない
		return NULL;
	}

	// インスタンスを返す
	return pGame;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	FILE *pFile = NULL;
	char aString[256];
	char aPlayerFileName[256];
	char aMapFileName[256];
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(GAME_MANAGER_INITFILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				if (strcmp(&aString[0], MAP_FILENAME) == 0)
				{// マップのスクリプトファイル名がある
					fscanf(pFile, "%s %s", &aString[0], &aMapFileName[0]);
				}
				if (strcmp(&aString[0], PLAYER_FILENAME) == 0)
				{// プレイヤーのスクリプトファイル名がある
					fscanf(pFile, "%s %s", &aString[0], &aPlayerFileName[0]);
				}
			}
		}
	}

	// データの読み込み
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, GAME_TEXTURE_NAME, &m_pTexture);
		}
	}
	CPlayer::Load(aPlayerFileName); // プレイヤー
	CField::Load();                 // フィールド
	CWater::Load();                 // 水
	CShadow::Load();                // 影
	CCylinder::Load();              // 山
	CDome::Load();                  // 空
	CNumber::Load();                // 数字
	CPause::Load();                 // ポーズ
	CLogoPause::Load();             // ポーズロゴ
	CBalloon::Load();               // 吹き出し

	// プレイヤーの生成
	CPlayer::Create(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_PRIORITY);

	// タイマーの生成
	CTimer::Create();

	// マップの生成
	if (m_pMap == NULL)
	{// メモリが確保できる状態にある
		m_pMap = CMap::Create(aMapFileName);
	}

	// 操作方法ポリゴンの生成
	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2DSquare::Create(GAME_POLYGON_POS, GAME_POLYGON_COL, GAME_POLYGON_WIDTH, GAME_POLYGON_HEIGHT, 7);
		if (m_pScene2D != NULL)
		{
			m_pScene2D->BindTexture(m_pTexture);
		}
	}

	// パーティクル管理クラスの生成
	CParticleManager::Create(PARTICLEDATA_FILENAME, PARTICLE_TEXTURE_FILENAME);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// データの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	CPlayer::UnLoad();             // プレイヤー
	CField::UnLoad();              // フィールド
	CWater::UnLoad();              // 水
	CShadow::UnLoad();             // 影
	CCylinder::UnLoad();           // 山
	CDome::UnLoad();               // 空
	CNumber::UnLoad();             // 数字
	CPause::UnLoad();              // ポーズ
	CLogoPause::UnLoad();          // ポーズロゴ
	CBalloon::UnLoad();            // 吹き出し

	// マップの破棄
	if (m_pMap != NULL)
	{// メモリが確保されている
		m_pMap->Uninit();

		// メモリの開放
		delete m_pMap;
		m_pMap = NULL;
	}

	// 操作方法ポリゴンの破棄
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	// リザルトに必要なスコアを渡す
	CResult::SetNumClearStage(m_nNumClearMap);
	CResult::SetStageBonus(m_nMapClearBonus);

	// ゲームオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGame::Update(void)
{
	switch (m_State)
	{// 状態によって処理わけ
	case STATE_NORMAL:        // 通常状態
		break;
	case STATE_MAPCHANGE:     // マップ切り替え状態
		CDebugProc::Print("マップ切り替え中\n");
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			CManager::GetFade()->SetFade(CFade::TYPE_SWITCHMAP);
		}
		break;
	case STATE_CHALLENGE:     // マップ挑戦状態
		CDebugProc::Print("マップ挑戦中\n");
		break;
	case STATE_PLAYERFALL:    // プレイヤーが落ちた状態
		CDebugProc::Print("プレイヤーがヤラレチャッタ\n");
		PlayerFall();
		break;
	case STATE_MAPCLEAR:      // マップクリア状態
		CDebugProc::Print("マップクリアー\n");
		ClearMapEvent();
		break;
	case STATE_END:           // 終了状態
		m_nStatementCounter++;
		if (m_nStatementCounter % GAME_MODE_CHANGE_TIMING)
		{// 画面遷移するタイミングになった
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// フェードの状態が使用されていない状態である
				CManager::GetFade()->SetFade(CManager::MODE_RESULT);
			}
		}
		break;
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
//    状態を取得する
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
//    マップをクリアしたかどうかを取得する
//=============================================================================
bool CGame::GetMapClear(void)
{
	return m_bMapClear;
}

//=============================================================================
//    状態設定処理
//=============================================================================
void CGame::SetState(STATE state)
{
	m_State = state;
	m_nStatementCounter = 0;  // 状態管理カウンターを戻す
}

//=============================================================================
//    マップをクリアしたかどうかを設定する
//=============================================================================
void CGame::SetMapClear(bool bMapClear)
{
	m_bMapClear = bMapClear;
}

//=============================================================================
//    マップのスクリプトファイル名設定処理
//=============================================================================
void CGame::SetMapName(char *aMapFileName)
{
	strcpy(m_aMapFileName, aMapFileName);
}

//=============================================================================
//    マップを切り替える処理
//=============================================================================
void CGame::SetMap(void)
{
	m_State = STATE_NORMAL;  // 状態を戻しておく

	// マップの破棄
	if (m_pMap != NULL)
	{// メモリが確保されている
		m_pMap->Uninit();

		// メモリの開放
		delete m_pMap;
		m_pMap = NULL;
	}

	// 吹き出しの削除
	// 吹き出しポリゴンクラスへのポインタを取得する
	CBalloon *pBalloon = NULL;
	CScene *pScene = NULL;
	for (int nCntScene = 0; nCntScene < MAX_PRIORITY_NUM; nCntScene++)
	{// 処理優先順位の数だけ繰り返し
		pScene = GetScene(nCntScene);   // 先頭アドレスを確保
		if (pScene != NULL)
		{// ポインタが取得できた
			CScene *pSceneNext;  // 次のポインタ保存用
			while (pScene != NULL)
			{// 次のポインタがなくなるまでループ
				pSceneNext = pScene->GetNext();
				if (pScene->GetObjType() == CScene::OBJTYPE_BALLOON)
				{// プレイヤークラスのポインタだった
					pBalloon = (CBalloon*)pScene;
					pBalloon->Uninit();
					pBalloon = NULL;
				}
				pScene = pSceneNext;
			}
		}
	}

	// マップの生成
	if (m_pMap == NULL)
	{// メモリが確保できる状態にある
		m_pMap = CMap::Create(m_aMapFileName);
		if (m_bMapClear == true)
		{// マップクリアしたら
			SetState(STATE_NORMAL);
		}
		else
		{// 攻略マップだったら
			SetState(STATE_CHALLENGE);
		}
	}

	m_bMapClear = false;    // マップを未クリア状態にする
}

//=============================================================================
//    マップをクリアしたときの処理
//=============================================================================
void CGame::ClearMapEvent(void)
{
	m_nStatementCounter++;
	if (m_nStatementCounter % GAME_MAPCLEAR_CHANGETIMING == 0)
	{// マップ切り替えのタイミングになった
	    // クリアボーナスを加算
		m_nNumClearMap++;

		char *pMapName = m_pMap->GetMapName();
		if (strcmp(pMapName, "data/TEXT/MAP/map_stage1-1.txt") == 0)
		{// ステージ1だったら
			m_nMapClearBonus += 3000;
		}
		else if (strcmp(pMapName, "data/TEXT/MAP/map_stage1-2.txt") == 0)
		{// ステージ2だったら
			m_nMapClearBonus += 5000;
		}
		else if (strcmp(pMapName, "data/TEXT/MAP/map_stage1-3.txt") == 0)
		{// ステージ3だったら
			m_nMapClearBonus += 15000;
		}
		else if (strcmp(pMapName, "data/TEXT/MAP/map_stage1-4.txt") == 0)
		{// ステージ4だったら
			m_nMapClearBonus += 30000;
		}
		else if (strcmp(pMapName, "data/TEXT/MAP/map_stage1-5.txt") == 0)
		{// ステージ5だったら
			m_nMapClearBonus += 50000;
		}

		SetMapName(GAME_MAPSELECT_FILENAME);
		SetState(CGame::STATE_MAPCHANGE);
	}
	// 文字を出して、フェードしてマップ選択に戻す

}

//=============================================================================
//    プレイヤーが水に落ちた時の処理
//=============================================================================
void CGame::PlayerFall(void)
{
	m_nStatementCounter++;
	if (m_nStatementCounter % GAME_PLAYER_RESPAWNTIMING == 0)
	{// プレイヤーをリスポーンさせるタイミングになった
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			CManager::GetFade()->SetFade(CFade::TYPE_RESTART);
		}
	}
}

//=============================================================================
//    プレイヤーをリスポーンさせる処理
//=============================================================================
void CGame::PlayerRespawnEvent(void)
{
	if (m_pMap != NULL)
	{// メモリが確保されている
		// プレイヤークラスへのポインタを取得する
		CPlayer *pPlayer = NULL;
		CScene *pScene = CScene::GetScene(PLAYER_PRIORITY);
		if (pScene != NULL)
		{// ポインタが取得できた
			CScene *pSceneNext;  // 次のポインタ保存用
			while (pScene != NULL)
			{// 次のポインタがなくなるまでループ
				pSceneNext = pScene->GetNext();
				if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
				{// プレイヤークラスのポインタだった
					pPlayer = (CPlayer*)pScene;
					break;
				}
				pScene = pSceneNext;
			}
		}

		// プレイヤーの位置をデフォルトに戻す
		if (pPlayer != NULL)
		{// ポインタが取得できた
			pPlayer->SetDefault(m_pMap->GetDefaultPlayerPos(), m_pMap->GetDefaultPlayerRot());
		}

		// カメラの向きをデフォルトに戻す
		CCamera *pCamera = CManager::GetCamera();
		if (pCamera != NULL)
		{// カメラが取得できた
			pCamera->SetDefaultCamera(m_pMap->GetDefaultCameraRot());
		}

		// マップの配置物をデフォルトの位置に戻す
		m_pMap->SetObjectDefault();
	}
}