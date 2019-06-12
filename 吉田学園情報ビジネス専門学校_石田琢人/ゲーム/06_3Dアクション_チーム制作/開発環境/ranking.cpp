//*****************************************************************************
//
//     ランキングの処理[ranking.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "number.h"
#include "logo.h"
#include "input.h"
#include "scene2DSquare.h"
#include "scene2D.h"
#include "map.h"

#include "debuglog.h"
#include "cylinder.h"
#include "dome.h"
#include "shadow.h"
#include "field.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define RANKING_RANK_TEXTURENAME	"data/TEXTURE/RANKING/Rank.png"	        	// ランキング順位用のテクスチャのファイル名
#define RANKING_LOGO_TEXTURENAME	"data/TEXTURE/RANKING/RankingLogo.png"		// ランキング順位用のテクスチャのファイル名
#define MAP_FILENAME				"MAP_FILENAME"								// マップファイル名
#define INITFILENAME				"data/TEXT/MODE/game.ini"					// ゲームのシステムファイル名

#define RANKING_SCORE_INI          (100000 - nCntRanking * 20000)				// ランキングスコアの計算式

#define RANKING_MODE_CHANGETIMING  (600)										// どれくらいの時間で勝手に遷移するか

#define RANKING_SCORE_POS_INI      (D3DXVECTOR3(960.0f,180.0f,0.0f))			// ランキングスコアの座標(初期値)
#define RANKING_SCORE_POS_MAX      (1080.0f)									// ランキングスコアの座標の最大値
#define RANKING_SCORE_COL_INI      (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))				// ランキングスコアの色(初期値)
#define RANKING_SCORE_WIDTH_INI    (60.0f)										// ランキングスコアの幅(初期値)
#define RANKING_SCORE_HEIGHT_INI   (55.0f)										// ランキングスコアの高さ(初期値)
#define RANKING_SCORE_MOVE_INI     (D3DXVECTOR3(-15.0f,0.0f,0.0f))				// ランキングスコアの移動量(初期値)

#define RANKING_RANK_POS_INI       (D3DXVECTOR3(360.0f,180.0f,0.0f))			// ランキング順位の座標(初期値)
#define RANKING_RANK_COL_INI       (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))				// ランキング順位の色(初期値)
#define RANKING_RANK_WIDTH_INI     (100.0f)										// ランキング順位の幅(初期値)
#define RANKING_RANK_HEIGHT_INI    (70.0f)										// ランキング順位の高さ(初期値)
#define RANKING_RANK_POS_MAX       (260.0f)										// ランキング順位の座標の最大値

#define RANKING_LOGO_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 2,50.0f,0.0f))	// ランキングロゴの座標(初期値)
#define RANKING_LOGO_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))				// ランキングロゴの色(初期値)
#define RANKING_LOGO_SCALE_INI     (120.0f)										// ランキングロゴの大きさ(初期値)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
CRanking::STATE CRanking::m_State[MAX_RANKING];     // ランキングスコアの状態
int CRanking::m_nRankingScore[MAX_RANKING] = { 100000, 80000, 60000,40000,20000 };         // ランキングのスコア
LPDIRECT3DTEXTURE9 CRanking::m_apTexture[MAX_RANKING_TEXTURE] = {};   // ランキング順位用のテクスチャ
int CRanking::m_nScore = 0;		// 最新のスコア

//=============================================================================
//    コンストラクタ
//=============================================================================
CRanking::CRanking(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数だけ繰り返し
		m_apNumber[nCntRanking] = NULL;			// ランキングスコア
		m_apSceneSquare[nCntRanking] = NULL;	// 順位用のテクスチャ
		m_aScoreDigits[nCntRanking] = 0;		// スコアの桁数
	}

	m_pMap = NULL;							// マップクラスのポインタ変数
	m_pLogo = NULL;                         // ランキングロゴクラス型のポインタ
	m_nCounter = 0;                         // 画面遷移管理カウンター
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRanking *CRanking::Create(void)
{
	CRanking *pRanking = NULL;  // ランキングクラス型のポインタ
	if (pRanking == NULL)
	{// メモリが確保できる状態にある
		pRanking = new CRanking;
		if (pRanking != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pRanking->Init();
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
	return pRanking;
}

//=============================================================================
//    ランキングスコアの初期化
//=============================================================================
void CRanking::RankingScoreInit(void)
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数だけ繰り返し
		m_nRankingScore[nCntRanking] = RANKING_SCORE_INI;
	}
}

//=============================================================================
//    ランキングスコアの設定
//=============================================================================
void CRanking::SetRankingScore(int nScore)
{
	int nScoreChange = nScore; // 入れ替えるスコア
	int nScoreOld = 0;         // ランキングスコアの一時的な格納場所
	bool bSet = false;         // ランキングが更新されたかどうか

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングの桁数分繰り返し
		if (m_nRankingScore[nCntRanking] < nScoreChange)
		{// 登録されているスコアより入れ替えるスコアが大きい
			nScoreOld = m_nRankingScore[nCntRanking];    // 元のスコアを格納
			m_nRankingScore[nCntRanking] = nScoreChange; // スコア更新
			nScoreChange = nScoreOld;                    // 元のスコアを入れ替える対象に
			if (bSet == false)
			{// 更新されていなかったら
				m_State[nCntRanking] = STATE_UPDATE;     // 更新された状態に
				bSet = true;                             // 更新した状態に
			}
		}
	}

	// 最新のスコアを記憶
	m_nScore = nScore;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	// オブジェクトのタイプを指定
	SetObjType(OBJTYPE_RANKING);

	// テクスチャの読みこみ
	CNumber::Load();      // 数字
	CCylinder::Load();    // 山
	CDome::Load();        // 空
	CField::Load();       // フィールド
	CShadow::Load();      // 影

	FILE *pFile = NULL;
	char aString[256];
	char aMapFileName[256];
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(INITFILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				if (strcmp(&aString[0], MAP_FILENAME) == 0)
				{// マップのスクリプトファイル名がある
					fscanf(pFile, "%s %s", &aString[0], &aMapFileName[0]);
				}
			}
		}
	}

	// マップの生成
	if (m_pMap == NULL)
	{// メモリが確保できる状態にある
		m_pMap = CMap::Create(aMapFileName);
	}

	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		if (pDevice != NULL)
		{// デバイスが取得できた
			// テクスチャの生成
			D3DXCreateTextureFromFile(pDevice, RANKING_RANK_TEXTURENAME, &m_apTexture[0]);
			D3DXCreateTextureFromFile(pDevice, RANKING_LOGO_TEXTURENAME, &m_apTexture[1]);
		}
	}

	D3DXVECTOR3 NumberPos = RANKING_SCORE_POS_INI;   // 座標
	D3DXCOLOR NumberCol = RANKING_SCORE_COL_INI;     // 色
	float fNumberWidth = RANKING_SCORE_WIDTH_INI;    // 幅
	float fNumberHeight = RANKING_SCORE_HEIGHT_INI;  // 高さ
	int nNumber = 0;                                 // 表示する数字の計算用

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数だけ繰り返し
		m_aScoreDigits[nCntRanking] = (int)log10f((float)m_nRankingScore[nCntRanking]) + 1;	// 桁数を求める
		if (m_aScoreDigits[nCntRanking] <= 0) { m_aScoreDigits[nCntRanking] = 1; }			// 0以下のとき1にする
		m_apNumber[nCntRanking] = new CNumber*[m_aScoreDigits[nCntRanking]];				// 桁数分メモリ確保

		for (int nCntScore = 0; nCntScore < m_aScoreDigits[nCntRanking]; nCntScore++)
		{// ランキングスコアの桁数分繰り返し
		    // 表示する数字の計算
			nNumber = m_nRankingScore[nCntRanking] % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

			// 数字ポリゴンのインスタンス生成処理
			m_apNumber[nCntRanking][nCntScore] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, nNumber,6);

			// 横に進める
			NumberPos.x -= RANKING_SCORE_WIDTH_INI + (RANKING_SCORE_WIDTH_INI * 0.5f);
		}
		// 下に進める
		NumberPos = RANKING_SCORE_POS_INI;
		NumberPos.y += (RANKING_SCORE_HEIGHT_INI + (RANKING_SCORE_HEIGHT_INI * 1.2f)) * (nCntRanking + 1);
	}

	// ランキングロゴのインスタンス生成
	if(m_pLogo == NULL)
	{// メモリが確保できる状態にある
		m_pLogo = CLogo::Create(RANKING_LOGO_POS_INI, RANKING_LOGO_COL_INI, RANKING_LOGO_SCALE_INI, 6);
		m_pLogo->BindTexture(m_apTexture[1]);	// テクスチャの設定

		// 頂点バッファの取得
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pLogo->GetVtxBuff();

		// 頂点情報
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(-RANKING_LOGO_SCALE_INI * 3.0f, -RANKING_LOGO_SCALE_INI, 0.0f) + RANKING_LOGO_POS_INI;
		pVtx[1].pos = D3DXVECTOR3(RANKING_LOGO_SCALE_INI * 3.0f, -RANKING_LOGO_SCALE_INI, 0.0f) + RANKING_LOGO_POS_INI;
		pVtx[2].pos = D3DXVECTOR3(-RANKING_LOGO_SCALE_INI * 3.0f, RANKING_LOGO_SCALE_INI, 0.0f) + RANKING_LOGO_POS_INI;
		pVtx[3].pos = D3DXVECTOR3(RANKING_LOGO_SCALE_INI * 3.0f, RANKING_LOGO_SCALE_INI, 0.0f) + RANKING_LOGO_POS_INI;

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();

		// 頂点バッファの設定
		m_pLogo->SetVtxBuff(pVtxBuff);
	}

	// ランキング順位ポリゴンのインスタンス生成
	D3DXVECTOR3 RankPos = RANKING_RANK_POS_INI;   // 座標
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングの桁数分繰り返し
		if (m_apSceneSquare[nCntRanking] == NULL)
		{// メモリが確保できる状態にある
			m_apSceneSquare[nCntRanking] = CScene2DSquare::Create(RankPos, RANKING_RANK_COL_INI, RANKING_RANK_WIDTH_INI, RANKING_RANK_HEIGHT_INI, 6);
			if (m_apSceneSquare[nCntRanking] != NULL)
			{// メモリが確保できた
				// テクスチャを割り当て
				m_apSceneSquare[nCntRanking]->BindTexture(m_apTexture[0]);

				// テクスチャ座標をずらす
				VERTEX_2D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apSceneSquare[nCntRanking]->GetVtxBuff();

				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + (nCntRanking * (1.0f / MAX_RANKING)));
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + (nCntRanking * (1.0f / MAX_RANKING)));
				pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / MAX_RANKING) + (nCntRanking * (1.0f / MAX_RANKING)));
				pVtx[3].tex = D3DXVECTOR2(1.0f, (1.0f / MAX_RANKING) + (nCntRanking * (1.0f / MAX_RANKING)));

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();
			}
			RankPos = RANKING_RANK_POS_INI;
			RankPos.y += (RANKING_RANK_HEIGHT_INI + (RANKING_RANK_HEIGHT_INI * 0.75f)) * (nCntRanking + 1);
		}

		int nNumber;	// 計算結果を入れる
		for (int nCntDigits = 0; nCntDigits < m_aScoreDigits[nCntRanking]; nCntDigits++)
		{// 桁数分繰り返し
			nNumber = m_nRankingScore[nCntRanking] % (int)powf(10.0f, (nCntDigits + 1) * 1.0f) / (int)powf(10.0f, nCntDigits * 1.0f);
			m_apNumber[nCntRanking][nCntDigits]->SetNumber(nNumber);	// 数字の設定
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	// テクスチャの開放
	CNumber::UnLoad();      // 数字
	CCylinder::UnLoad();    // 山
	CDome::UnLoad();        // 空
	CField::UnLoad();       // フィールド
	CShadow::UnLoad();      // 影

	// マップの破棄
	if (m_pMap != NULL)
	{// メモリが確保されている
		m_pMap->Uninit();

		// メモリの開放
		delete m_pMap;
		m_pMap = NULL;
	}

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数だけ繰り返し
		if (m_apNumber[nCntRanking] != NULL)
		{
			for (int nCntScore = 0; nCntScore < m_aScoreDigits[nCntRanking]; nCntScore++)
			{// ランキングスコアの桁数分繰り返し
				if (m_apNumber[nCntRanking][nCntScore] != NULL)
				{// メモリが確保されている
					// 数字ポリゴンの終了処理
					m_apNumber[nCntRanking][nCntScore]->Uninit();
					m_apNumber[nCntRanking][nCntScore] = NULL;
				}
			}
			delete[] m_apNumber[nCntRanking];
			m_apNumber[nCntRanking] = NULL;
		}

		if (m_apSceneSquare[nCntRanking] != NULL)
		{// メモリが確保されている
			// 順位ポリゴンの終了処理
			m_apSceneSquare[nCntRanking]->Uninit();
			m_apSceneSquare[nCntRanking] = NULL;
		}

		// 状態を戻しておく
		m_State[nCntRanking] = STATE_NONE;
	}

	if (m_pLogo != NULL)
	{// メモリが確保されている
		// ランキングロゴの終了処理
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	// 最新のスコアを初期化
	m_nScore = 0;

	// ランキングオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRanking::Update(void)
{
	if (m_pLogo != NULL)
	{
		m_pLogo->Update();
	}

	// 画面遷移までのカウント
	m_nCounter++;

	if (m_nCounter % RANKING_MODE_CHANGETIMING == 0)
	{// 画面遷移するタイミングになった
		CManager::GetFade()->SetFade(CManager::MODE_TITLE);
	}
	else
	{// 画面遷移するタイミングではない
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// 決定ボタンが押された
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
			else
			{// キーボードの決定ボタンが押されていない
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_12; nCntButton++)
				{// 反応するボタン数の数だけ繰り返し
					if (CManager::GetJoyStick()->GetJoyStickDevice(0)->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// ボタンが押された
						CManager::GetFade()->SetFade(CManager::MODE_TITLE);
					}
				}
			}
		}
	}

	if (m_nCounter % 10 == 0)
	{// 点滅させる
		m_bFlash = m_bFlash ? false : true;
	}

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングの桁数分繰り返し
		if (m_nRankingScore[nCntRanking] == m_nScore)
		{// 最新のスコアを点滅させる
			for (int nCntDigits = 0; nCntDigits < m_aScoreDigits[nCntRanking]; nCntDigits++)
			{// 桁数分繰り返し
				if(m_bFlash)
				{// 点滅させる
					m_apNumber[nCntRanking][nCntDigits]->SetVtxBuffCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				}
				else
				{// 点滅させる
					m_apNumber[nCntRanking][nCntDigits]->SetVtxBuffCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}
			}
			break;
		}
	}

	CDebugProc::Print("ランキングなう\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CRanking::Draw(void)
{

}