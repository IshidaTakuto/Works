//*****************************************************************************
//
//     リザルトの処理[result.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "result.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "renderer.h"
#include "logo.h"
#include "number.h"
#include "ranking.h"
#include "map.h"

#include "debuglog.h"
#include "cylinder.h"
#include "dome.h"
#include "shadow.h"
#include "field.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define RESULT_LOGO_TEXTURENAME0	"data/TEXTURE/RESULT/Result.png"			    // ロゴのテクスチャ名
#define RESULT_LOGO_TEXTURENAME1	"data/TEXTURE/RESULT/ClearStage.png"	        // ロゴのテクスチャ名
#define RESULT_LOGO_TEXTURENAME2	"data/TEXTURE/RESULT/ClearBonus.png"	        // ロゴのテクスチャ名
#define RESULT_LOGO_TEXTURENAME3	"data/TEXTURE/RESULT/TotalScore.png"	        // ロゴのテクスチャ名
#define RESULT_LOGO_TEXTURENAME4	"data/TEXTURE/RESULT/Multiply.png"	             // ロゴのテクスチャ名

#define MAP_FILENAME				"MAP_FILENAME"									// マップファイル名
#define INITFILENAME				"data/TEXT/MODE/game.ini"						// ゲームのシステムファイル名

#define RESULT_MODE_CHANGE_TIMING	(800)	// ランキング画面に遷移するまでの時間
#define RESULT_LOGO_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 50.0f, 0.0f))				// リザルトロゴの位置
#define RESULT_TOTAL_LOGO_POS		(D3DXVECTOR3(400.0f, 530.0f, 0.0f))	// 各項目ロゴの位置
#define RESULT_ELSE_LOGO_POS		(D3DXVECTOR3(400.0f, 40.0f + (nCntLogo * 150.0f), 0.0f))	// 各項目ロゴの位置
#define RESULT_LOGO_PRESS_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f))				// プレスロゴの位置

#define RESULT_LOGO_SIZE			(25)	// リザルトロゴのサイズ
#define RESULT_ELSE_LOGO_SIZE		(100)	// 各項目ロゴのサイズ
#define RESULT_LOGO_PRESS_SIZE		(50)	// プレスロゴのサイズ

#define RESULT_NUMBER_WIDTH			(40.0f)	// 数字の幅
#define RESULT_NUMBER_HEIGHT		(35.0f)	// 数字の高さ

#define RESULT_NUMBER_COLOR			(D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f))	// 数字の色

#define RESULT_STAGE_CLEAR_BONUS    (10000)

#define RESULT_SCORE_SECOND			(5)		// 合計スコアが表示されるまでの秒数

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
int CResult::m_nNumClearStage = 0;	// クリアしたステージ数
int CResult::m_nStageBonus = 0;		// クリアしたステージのボーナス
LPDIRECT3DTEXTURE9 CResult::m_apTexture[CResult::LOGO_MAX] = {};	// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CResult::m_apTexture2 = {};	// テクスチャのポインタ

//=============================================================================
//    コンストラクタ
//=============================================================================
CResult::CResult(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	for (int nCntLogo = 0; nCntLogo < LOGO_MAX; nCntLogo++)
	{// ロゴのクリア
		m_apLogo[nCntLogo] = NULL;
	}

	for (int nCntNumber = 0; nCntNumber < NUMBER_MAX; nCntNumber++)
	{// 数字のポインタ変数をクリア
		m_apNumber[nCntNumber] = NULL;
	}

	m_pMulti = NULL;
	m_pMap = NULL;			// マップ
	m_nScoreTotal = 0;		// スコアの合計
	m_pLogoPress = NULL;	// プレスロゴのポインタ変数
	m_nCounter = 0;			// 遷移を管理するカウンター
	m_bChange = false;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CResult *CResult::Create(void)
{
	CResult *pResult = NULL;  // ゲームクラス型のポインタ
	if (pResult == NULL)
	{// メモリが確保できる状態にある
		pResult = new CResult;
		if (pResult != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pResult->Init();
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
	return pResult;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	// テクスチャの読み込み
	CLogoPress::Load();   // PRESSENTER
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
			D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TEXTURENAME0, &m_apTexture[LOGO_RESULT]);
			D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TEXTURENAME1, &m_apTexture[LOGO_CLEAR_STAGE]);
			D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TEXTURENAME2, &m_apTexture[LOGO_STAGE_BONUS]);
			D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TEXTURENAME3, &m_apTexture[LOGO_TOTAL]);
			D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TEXTURENAME4, &m_apTexture2);

			for (int nCntLogo = 0; nCntLogo < LOGO_MAX; nCntLogo++)
			{
				// ロゴの生成
				if (nCntLogo == LOGO_RESULT)
				{// リザルトのロゴ
					m_apLogo[nCntLogo] = CLogo::Create(RESULT_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), RESULT_LOGO_SIZE, 6);
				}
				else
				{// 各項目のロゴ
					m_apLogo[nCntLogo] = CLogo::Create(RESULT_ELSE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), RESULT_ELSE_LOGO_SIZE, 6);
				}

				// 頂点バッファの取得
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogo[nCntLogo]->GetVtxBuff();

				// 頂点情報
				VERTEX_2D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標の設定
				if (nCntLogo == LOGO_RESULT)
				{// リザルトのロゴ
					pVtx[0].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 8.0f, -RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_LOGO_POS;
					pVtx[1].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 8.0f, -RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_LOGO_POS;
					pVtx[2].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 8.0f, RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_LOGO_POS;
					pVtx[3].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 8.0f, RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_LOGO_POS;
				}
				else if (nCntLogo == LOGO_TOTAL)
				{
					pVtx[0].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 8.0f, -RESULT_LOGO_SIZE * 4.0f, 0.0f) + RESULT_TOTAL_LOGO_POS;
					pVtx[1].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 8.0f, -RESULT_LOGO_SIZE * 4.0f, 0.0f) + RESULT_TOTAL_LOGO_POS;
					pVtx[2].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 8.0f, RESULT_LOGO_SIZE * 4.0f, 0.0f) + RESULT_TOTAL_LOGO_POS;
					pVtx[3].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 8.0f, RESULT_LOGO_SIZE * 4.0f, 0.0f) + RESULT_TOTAL_LOGO_POS;
				}
				else
				{// 各項目のロゴ
					pVtx[0].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 5.6f, -RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_ELSE_LOGO_POS;
					pVtx[1].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 5.6f, -RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_ELSE_LOGO_POS;
					pVtx[2].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 5.6f, RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_ELSE_LOGO_POS;
					pVtx[3].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 5.6f, RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_ELSE_LOGO_POS;
				}

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();

				// 頂点バッファの設定
				m_apLogo[nCntLogo]->SetVtxBuff(pVtxBuff);

				// テクスチャを設定
				m_apLogo[nCntLogo]->BindTexture(m_apTexture[nCntLogo]);
			}
		}
	}

	// 数字を生成する
	if (m_apNumber[NUMBER_CLEAR_STAGE] == NULL)
	{// 数字の生成
		int nNumClearStage;
		nNumClearStage = (int)log10f((float)m_nNumClearStage) + 1;			// 桁数を求める
		if (nNumClearStage <= 0) { nNumClearStage = 1; }					// 桁数が0のとき１にする
		m_apNumber[NUMBER_CLEAR_STAGE] = new CNumber*[nNumClearStage];		// ナンバーの生成

		for (int nCntLogo = 0; nCntLogo < nNumClearStage; nCntLogo++)
		{// 桁数分ループ
			int nNumber = m_nNumClearStage % (int)powf(10.0f, (nCntLogo + 1) * 1.0f) / (int)powf(10.0f, nCntLogo * 1.0f);
			m_apNumber[NUMBER_CLEAR_STAGE][nCntLogo] = CNumber::Create(D3DXVECTOR3(RESULT_ELSE_LOGO_POS.x + 300.0f + ((nNumClearStage - 1) * (RESULT_NUMBER_WIDTH * 1.5f)) - (nCntLogo * RESULT_NUMBER_WIDTH * 1.5f), 180.0f, 0.0f),
				RESULT_NUMBER_COLOR, RESULT_NUMBER_WIDTH, RESULT_NUMBER_HEIGHT, nNumber, 6);

			// 数字の設定
			m_apNumber[NUMBER_CLEAR_STAGE][nCntLogo]->SetNumber(nNumber);
		}
	}
	if (m_apNumber[NUMBER_STAGE_BONUS] == NULL)
	{// 数字の生成
		int nStageBonus;
		nStageBonus = (int)log10f((float)m_nStageBonus) + 1;				// 桁数を求める
		if (nStageBonus <= 0) { nStageBonus = 1; }							// 桁数が0のとき１にする
		m_apNumber[NUMBER_STAGE_BONUS] = new CNumber*[nStageBonus];		// ナンバーの生成

		for (int nCntLogo = 0; nCntLogo < nStageBonus; nCntLogo++)
		{// 桁数分ループ
			int nNumber = m_nStageBonus % (int)powf(10.0f, (nCntLogo + 1) * 1.0f) / (int)powf(10.0f, nCntLogo * 1.0f);
			m_apNumber[NUMBER_STAGE_BONUS][nCntLogo] = CNumber::Create(D3DXVECTOR3(RESULT_ELSE_LOGO_POS.x + 300.0f + ((nStageBonus - 1) * (RESULT_NUMBER_WIDTH * 1.5f)) - (nCntLogo * RESULT_NUMBER_WIDTH * 1.5f), 340.0f, 0.0f),
				RESULT_NUMBER_COLOR, RESULT_NUMBER_WIDTH, RESULT_NUMBER_HEIGHT, nNumber, 6);

			// 数字の設定
			m_apNumber[NUMBER_STAGE_BONUS][nCntLogo]->SetNumber(nNumber);
		}
	}


	if (m_apNumber[NUMBER_CLEAR_STAGE_BONUS] == NULL)
	{// 数字の生成
		int nStageBonus;
		nStageBonus = (int)log10f((float)(RESULT_STAGE_CLEAR_BONUS)) + 1;	    // 桁数を求める
		if (nStageBonus <= 0) { nStageBonus = 1; }							    // 桁数が0のとき１にする
		m_apNumber[NUMBER_CLEAR_STAGE_BONUS] = new CNumber*[nStageBonus];		// ナンバーの生成

		for (int nCntLogo = 0; nCntLogo < nStageBonus; nCntLogo++)
		{// 桁数分ループ
			int nNumber = RESULT_STAGE_CLEAR_BONUS % (int)powf(10.0f, (nCntLogo + 1) * 1.0f) / (int)powf(10.0f, nCntLogo * 1.0f);
			m_apNumber[NUMBER_CLEAR_STAGE_BONUS][nCntLogo] = CNumber::Create(D3DXVECTOR3(RESULT_ELSE_LOGO_POS.x + 500.0f + ((nStageBonus - 1) * (RESULT_NUMBER_WIDTH * 1.5f)) - (nCntLogo * RESULT_NUMBER_WIDTH * 1.5f), 180.0f, 0.0f),
				RESULT_NUMBER_COLOR, RESULT_NUMBER_WIDTH, RESULT_NUMBER_HEIGHT, nNumber, 6);

			// 数字の設定
			m_apNumber[NUMBER_CLEAR_STAGE_BONUS][nCntLogo]->SetNumber(nNumber);
		}
	}

	if (m_pMulti == NULL)
	{
		m_pMulti = CScene2DSquare::Create(D3DXVECTOR3(810.0f, 180.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50.0f, 50.0f,6);
		if (m_pMulti != NULL)
		{
			m_pMulti->BindTexture(m_apTexture2);
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// テクスチャの破棄
	CLogoPress::UnLoad();   // PRESSENTER
	CNumber::UnLoad();      // 数字
	CCylinder::UnLoad();    // 山
	CDome::UnLoad();        // 空
	CField::UnLoad();       // フィールド
	CShadow::UnLoad();      // 影

	for (int nCntLogo = 0; nCntLogo < LOGO_MAX; nCntLogo++)
	{
		if (m_apLogo[nCntLogo] != NULL)
		{// プレスロゴの破棄
			m_apLogo[nCntLogo]->Uninit();
			m_apLogo[nCntLogo] = NULL;
		}

		if (m_apNumber[nCntLogo] != NULL)
		{// プレスロゴの破棄
			int nDigits = 0;

			// 使っている桁数を求める
			if (nCntLogo == NUMBER_TOTAL)		{ nDigits = (int)log10f((float)m_nScoreTotal) + 1; }	// 合計スコア
			if (nCntLogo == NUMBER_CLEAR_STAGE)	{ nDigits = (int)log10f((float)m_nNumClearStage) + 1; }	// クリアステージ数
			if (nCntLogo == NUMBER_STAGE_BONUS)	{ nDigits = (int)log10f((float)m_nStageBonus) + 1; }	// ステージボーナス

			if (nDigits <= 0)
			{// 0以下だったら1にする
				nDigits = 1;
			}

			for (int nCntNumber = 0; nCntNumber < nDigits; nCntNumber++)
			{// メモリが確保された分ループ
				m_apNumber[nCntLogo][nCntNumber]->Uninit();
				m_apNumber[nCntLogo][nCntNumber] = NULL;
			}

			delete[] m_apNumber[nCntLogo];
			m_apNumber[nCntLogo] = NULL;
		}
	}

	if (m_pLogoPress != NULL)
	{// プレスロゴの破棄
		m_pLogoPress->Uninit();
		m_pLogoPress = NULL;
	}

	// マップの破棄
	if (m_pMap != NULL)
	{// メモリが確保されている
		m_pMap->Uninit();

		// メモリの開放
		delete m_pMap;
		m_pMap = NULL;
	}

	// 文字の開放
	if (m_pMulti != NULL)
	{
		m_pMulti->Uninit();
		m_pMulti = NULL;
	}

	// ランキングスコアの設定
	CRanking::SetRankingScore(m_nScoreTotal);

	// リザルトオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CResult::Update(void)
{
	m_nCounter++;  // カウンターを進める

	// スコアの合計を出す
	const int nScoreTotal = (m_nNumClearStage * RESULT_STAGE_CLEAR_BONUS) + m_nStageBonus;
	int nScoreTotalOld = (int)log10f((float)m_nScoreTotal) + 1;		// 合計スコア桁数の保存

	if (m_nCounter % RESULT_MODE_CHANGE_TIMING == 0)
	{// 画面遷移するタイミングになった
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
			CRanking::SetRankingScore(nScoreTotal);		// ランキングにスコアを設定
		}
	}
	else
	{// 画面遷移するタイミングではない
		if (m_bChange == true)
		{
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// フェードの状態が使用されていない状態である
				if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
				{// 決定ボタンが押された
					CManager::GetFade()->SetFade(CManager::MODE_RANKING);
				}
				else
				{// キーボードの決定ボタンが押されていない
					for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_12; nCntButton++)
					{// 反応するボタン数の数だけ繰り返し
						if (CManager::GetJoyStick()->GetJoyStickDevice(0)->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
						{// ボタンが押された
							CManager::GetFade()->SetFade(CManager::MODE_RANKING);
						}
					}
				}
			}
		}
	}

	if (m_nCounter < (300))
	{// 指定した秒数までランダム
		m_nScoreTotal = rand() % 90000;
	}
	else
	{// 合計スコアを表示する
		m_nScoreTotal = nScoreTotal;
		m_bChange = true;

		// プレスロゴの生成
		if (m_pLogoPress == NULL)
		{// 生成する
			m_pLogoPress = CLogoPress::Create(RESULT_LOGO_PRESS_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), RESULT_LOGO_PRESS_SIZE, 3);

			if (m_pLogoPress != NULL)
			{// 生成に成功
			    // 頂点バッファの取得
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pLogoPress->GetVtxBuff();

				// 頂点情報
				VERTEX_2D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-RESULT_LOGO_PRESS_SIZE * 5.0f, -RESULT_LOGO_PRESS_SIZE, 0.0f) + RESULT_LOGO_PRESS_POS;
				pVtx[1].pos = D3DXVECTOR3(RESULT_LOGO_PRESS_SIZE * 5.0f, -RESULT_LOGO_PRESS_SIZE, 0.0f) + RESULT_LOGO_PRESS_POS;
				pVtx[2].pos = D3DXVECTOR3(-RESULT_LOGO_PRESS_SIZE * 5.0f, RESULT_LOGO_PRESS_SIZE, 0.0f) + RESULT_LOGO_PRESS_POS;
				pVtx[3].pos = D3DXVECTOR3(RESULT_LOGO_PRESS_SIZE * 5.0f, RESULT_LOGO_PRESS_SIZE, 0.0f) + RESULT_LOGO_PRESS_POS;

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();

				// 頂点バッファの設定
				m_pLogoPress->SetVtxBuff(pVtxBuff);
			}
		}
	}

	// 桁数を求める
	int nDigitsScoreTotal = (int)log10f((float)m_nScoreTotal) + 1;

	if (nDigitsScoreTotal <= 0)
	{// 桁数が0のとき１にする
		nDigitsScoreTotal = 1;
	}

	if (m_apNumber[NUMBER_TOTAL] != NULL)
	{// 合計スコアが使われているとき
		for(int nCnt = 0; nCnt < nScoreTotalOld;nCnt++)
		{// 桁数分ループ
			if (m_apNumber[NUMBER_TOTAL][nCnt] != NULL)
			{
				m_apNumber[NUMBER_TOTAL][nCnt]->Uninit();
				m_apNumber[NUMBER_TOTAL][nCnt] = NULL;
			}
		}

		// メモリの開放
		delete[] m_apNumber[NUMBER_TOTAL];
		m_apNumber[NUMBER_TOTAL] = NULL;
	}

	if (m_apNumber[NUMBER_TOTAL] == NULL)
	{// メモリが空のとき
		m_apNumber[NUMBER_TOTAL] = new CNumber*[nDigitsScoreTotal];	// ナンバーの生成

		if (m_apNumber[NUMBER_TOTAL] != NULL)
		{// 生成に成功
			for (int nCntLogo = 0; nCntLogo < nDigitsScoreTotal; nCntLogo++)
			{// 桁数分ループ
				int nNumber = m_nScoreTotal % (int)powf(10.0f, (nCntLogo + 1) * 1.0f) / (int)powf(10.0f, nCntLogo * 1.0f);
				m_apNumber[NUMBER_TOTAL][nCntLogo] = CNumber::Create(D3DXVECTOR3(RESULT_ELSE_LOGO_POS.x + 300.0f + ((nDigitsScoreTotal - 1) * (RESULT_NUMBER_WIDTH * 1.5f)) - (nCntLogo * RESULT_NUMBER_WIDTH * 1.5f), 540, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), RESULT_NUMBER_WIDTH, RESULT_NUMBER_HEIGHT, nNumber, 6);

				// 数字の設定
				m_apNumber[NUMBER_TOTAL][nCntLogo]->SetNumber(nNumber);
			}
		}
	}

	CDebugProc::Print("リザルトなう\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CResult::Draw(void)
{
}

//=============================================================================
//    クリアしたステージ数の設定
//=============================================================================
const void CResult::SetNumClearStage(int nNumClearStage)
{
	m_nNumClearStage = nNumClearStage;
}

//=============================================================================
//    クリアしたステージボーナスの設定
//=============================================================================
const void CResult::SetStageBonus(int nStageBonus)
{
	m_nStageBonus = nStageBonus;
}