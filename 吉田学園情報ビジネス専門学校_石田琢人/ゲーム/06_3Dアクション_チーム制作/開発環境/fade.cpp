//*****************************************************************************
//
//     フェードの処理[fade.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "scene2DSquare.h"

#include "game.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define FADE_TEXTURENAME    "data/TEXTURE/COMMON/fadelogo.png"   // テクスチャのファイル名(準備中)

#define FADE_STOP_PERIOD    (60)                             // フェードをどれだけ止めるか
#define FADE_PRIORITY       (MAX_PRIORITY_NUM - 1)           // フェードの描画優先順位
#define FADE_OUT_ALPHA      (0.02f)                          // フェードアウトするときのα値の減算値
#define FADE_IN_ALPHA       (-0.02f)                         // フェードインするときのα値の加算値
#define FADE_COLOR          (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f)) // フェードポリゴンの色

#define FADE_PERIOD_WIDTH   (300.0f)                         // 準備中ポリゴンの幅
#define FADE_PERIOD_HEIGHT  (150.0f)                         // 準備中ポリゴンの高さ

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFade::m_pTexture = NULL;  // テクスチャへのポインタ

//=============================================================================
//    コンストラクタ
//=============================================================================
CFade::CFade()
{
	// 各種値のクリア
	m_pScene2DSquare = NULL;           // フェードに使用するポリゴン
	m_State = STATE_IN;                // 状態はフェードイン状態に
	m_Type = TYPE_MODECHANGE;          // 種類は画面遷移切り替えに
	m_NextMode = CManager::MODE_TITLE; // モードはタイトル状態に
	m_pPreparation = NULL;             // 準備中のポリゴン
	m_nCounter = 0;                    // 状態を管理するカウンター
}

//=============================================================================
//    デストラクタ
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
//    聴衆のテクスチャ読み込み処理
//=============================================================================
HRESULT CFade::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, FADE_TEXTURENAME, &m_pTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    聴衆のテクスチャ解放処理
//=============================================================================
void CFade::UnLoad(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CFade::Init(void)
{
	// フェードに使用するポリゴンを生成
	if (m_pScene2DSquare == NULL)
	{// メモリが確保できる状態にある
		m_pScene2DSquare = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, FADE_PRIORITY);
	}
	else
	{// メモリが確保できる状態にない
		return E_FAIL;
	}

	// 準備中ポリゴンを生成
	if (m_pPreparation == NULL)
	{// メモリが確保できる状態にある
		m_pPreparation = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH - FADE_PERIOD_WIDTH - 10.0f, SCREEN_HEIGHT - FADE_PERIOD_HEIGHT - 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FADE_PERIOD_WIDTH, FADE_PERIOD_HEIGHT, FADE_PRIORITY);
		if (m_pPreparation != NULL)
		{// メモリが確保できた
			// テクスチャの割り当て
			m_pPreparation->BindTexture(m_pTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CFade::Uninit(void)
{
	if (m_pScene2DSquare != NULL)
	{// メモリが確保されている
	    // 終了処理
	    m_pScene2DSquare->Uninit();
		m_pScene2DSquare = NULL;
	}

	if (m_pPreparation != NULL)
	{// メモリが確保されている
	    // 終了処理
		m_pPreparation->Uninit();
		m_pPreparation = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CFade::Update(void)
{
	if (m_State != STATE_NONE)
	{// フェードが使われる状態なら
		if (m_pScene2DSquare != NULL)
		{// フェードに使用するポリゴンが確保されている
			if (m_State == STATE_IN)
			{// フェードイン状態ならば
				FadeIn();
			}
			else if (m_State == STATE_STOP)
			{// フェードを止める状態だったら
				FadeStop();
			}
			else if(m_State == STATE_OUT)
			{// フェードアウト状態ならば
				FadeOut();
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CFade::Draw(void)
{

}

//=============================================================================
//    フェードの設定(画面切り替え専用)
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	if (m_State == STATE_NONE)
	{// フェードの状態が使用されていない状態である
		m_nCounter = 0;           // カウンターを戻しておく
		m_NextMode = modeNext;    // 次のモードを設定
		m_State = STATE_OUT;      // 状態をフェードアウト状態に
		m_Type = TYPE_MODECHANGE; // 種類を画面遷移切り替えに

		// フェードに使用するポリゴンを生成
		if (m_pScene2DSquare == NULL)
		{// メモリが確保できる状態にある
			D3DXCOLOR col = FADE_COLOR;
			col.a = 0.0f;
			m_pScene2DSquare = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), col, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, FADE_PRIORITY);
		}

		// 準備中ポリゴンを生成
		if (m_pPreparation == NULL)
		{// メモリが確保できる状態にある
			D3DXCOLOR colPre = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			m_pPreparation = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH - FADE_PERIOD_WIDTH - 10.0f, SCREEN_HEIGHT - FADE_PERIOD_HEIGHT - 10.0f, 0.0f), colPre, FADE_PERIOD_WIDTH, FADE_PERIOD_HEIGHT,FADE_PRIORITY);
			if (m_pPreparation != NULL)
			{// メモリが確保できた
			    // テクスチャの割り当て
				m_pPreparation->BindTexture(m_pTexture);
			}
		}
	}
}

//=============================================================================
//    フェードの設定(画面切り替え以外の場合)
//=============================================================================
void CFade::SetFade(TYPE type)
{
	if (m_State == STATE_NONE)
	{// フェードの状態が使用されていない状態である
		m_nCounter = 0;           // カウンターを戻しておく
		m_State = STATE_OUT;      // 状態をフェードアウト状態に
		m_Type = type;            // 種類を設定

		// フェードに使用するポリゴンを生成
		if (m_pScene2DSquare == NULL)
		{// メモリが確保できる状態にある
			D3DXCOLOR col = FADE_COLOR;
			col.a = 0.0f;
			m_pScene2DSquare = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), col, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, FADE_PRIORITY);
		}

		// 準備中ポリゴンを生成
		if (m_pPreparation == NULL)
		{// メモリが確保できる状態にある
			D3DXCOLOR colPre = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			m_pPreparation = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH - FADE_PERIOD_WIDTH - 10.0f, SCREEN_HEIGHT - FADE_PERIOD_HEIGHT - 10.0f, 0.0f), colPre, FADE_PERIOD_WIDTH, FADE_PERIOD_HEIGHT, FADE_PRIORITY);
			if (m_pPreparation != NULL)
			{// メモリが確保できた
			 // テクスチャの割り当て
				m_pPreparation->BindTexture(m_pTexture);
			}
		}
	}
}


//=============================================================================
//    フェードイン状態の更新処理
//=============================================================================
void CFade::FadeIn(void)
{
	// 現在の色を取得
	D3DXCOLOR col = m_pScene2DSquare->GetCol();
	D3DXCOLOR colPre = m_pPreparation->GetCol();

	// 透明度を減算する
	col.a += FADE_IN_ALPHA;
	colPre.a += FADE_IN_ALPHA;
	if (m_pScene2DSquare != NULL)
	{// メモリが確保されている
		m_pScene2DSquare->SetVtxBuffCol(col);
	}
	if (m_pPreparation != NULL)
	{// メモリが確保されている
		m_pPreparation->SetVtxBuffCol(colPre);
	}

	if (col.a <= 0.0f)
	{// 透明になり切った
		m_State = STATE_NONE;  // 状態を戻す
		// 使用したポリゴンを開放する
		if (m_pScene2DSquare != NULL)
		{// メモリが確保されている
		    // 終了処理
			m_pScene2DSquare->Uninit();
			m_pScene2DSquare = NULL;
		}

		if (m_pPreparation != NULL)
		{// メモリが確保されている
		    // 終了処理
			m_pPreparation->Uninit();
			m_pPreparation = NULL;
		}
	}
}

//=============================================================================
//    フェードを止める状態の更新処理
//=============================================================================
void CFade::FadeStop(void)
{
	m_nCounter++;
	if (m_nCounter >= FADE_STOP_PERIOD)
	{// フェード終了時
	    // 使用したポリゴンを開放する
		if (m_pScene2DSquare != NULL)
		{// メモリが確保されている
		    // 終了処理
			m_pScene2DSquare->Uninit();
			m_pScene2DSquare = NULL;
		}
		if (m_pPreparation != NULL)
		{// メモリが確保されている
		    // 終了処理
			m_pPreparation->Uninit();
			m_pPreparation = NULL;
		}

		if (m_Type == TYPE_MODECHANGE)
		{// 画面遷移をするフェードだったら
			// 次のモードに遷移
			CManager::SetMode(m_NextMode);
		}
		else if (m_Type == TYPE_SWITCHMAP)
		{// マップ切り替え状態ならば
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{// ゲームが取得できた
				pGame->SetMap();
			}
		}
		else if (m_Type == TYPE_RESTART)
		{// ゲームリスタート状態ならば
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{// ゲームが取得できた
				pGame->PlayerRespawnEvent();
				pGame->SetState(CGame::STATE_CHALLENGE);
			}
		}

		// フェードに使用するポリゴンを確保しなおす
		if (m_pScene2DSquare == NULL)
		{// メモリが確保できる状態にある
			m_pScene2DSquare = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, FADE_PRIORITY);
		}

		// 準備中ポリゴンを生成
		if (m_pPreparation == NULL)
		{// メモリが確保できる状態にある
			m_pPreparation = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH - FADE_PERIOD_WIDTH - 10.0f, SCREEN_HEIGHT - FADE_PERIOD_HEIGHT - 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FADE_PERIOD_WIDTH, FADE_PERIOD_HEIGHT, FADE_PRIORITY);
			if (m_pPreparation != NULL)
			{// メモリが確保できた
			    // テクスチャの割り当て
				m_pPreparation->BindTexture(m_pTexture);
			}
		}

		// フェードイン状態に
		m_State = STATE_IN;
	}
}

//=============================================================================
//    フェードアウト状態の更新処理
//=============================================================================
void CFade::FadeOut(void)
{
	// 現在の色を取得
	D3DXCOLOR col = m_pScene2DSquare->GetCol();
	D3DXCOLOR colPre = m_pPreparation->GetCol();

	// 透明度を加算する
	col.a += FADE_OUT_ALPHA;
	colPre.a += FADE_OUT_ALPHA;
	if (m_pScene2DSquare != NULL)
	{// メモリが確保されている
		m_pScene2DSquare->SetVtxBuffCol(col);
	}
	if (m_pPreparation != NULL)
	{// メモリが確保されている
		m_pPreparation->SetVtxBuffCol(colPre);
	}

	if (col.a >= 1.0f)
	{// 透明になり切った
		m_State = STATE_STOP;      // 状態をフェードを止める状態に
		col.a = 1.0f;              // 色を戻す
	}
}

//=============================================================================
//    フェードの状態を取得
//=============================================================================
CFade::STATE CFade::GetState(void)
{
	return m_State;
}