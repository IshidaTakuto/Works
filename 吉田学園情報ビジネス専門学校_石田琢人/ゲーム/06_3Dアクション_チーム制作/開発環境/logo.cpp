//*****************************************************************************
//
//     ロゴの処理[logo.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "logo.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define PAUSELOGO_TEXTURE_NAME_0  "data/TEXTURE/GAME/Continue.png"          // テクスチャのファイル名(CONTINUE)
#define PAUSELOGO_TEXTURE_NAME_1  "data/TEXTURE/GAME/Retry.png"             // テクスチャのファイル名(RETRY)
#define PAUSELOGO_TEXTURE_NAME_2  "data/TEXTURE/GAME/Quit.png"              // テクスチャのファイル名(QUIT)

#define SELECTLOGO_TEXTURE_NAME_0  "data/TEXTURE/SELECT/select000.png"            // テクスチャのファイル名(GAME)
#define SELECTLOGO_TEXTURE_NAME_1  "data/TEXTURE/SELECT/select001.png"            // テクスチャのファイル名(RANKING)
#define SELECTLOGO_TEXTURE_NAME_2  "data/TEXTURE/SELECT/select002.png"            // テクスチャのファイル名(TITLE)

#define SELECT_ANIMATION_PATTERN   (1)                                            // テクスチャアニメーションのパターン数
#define SELECT_ANIMATION_TEX_U_INI (1.0f / SELECT_ANIMATION_PATTERN)              // テクスチャアニメーションU座標の初期値
#define SELECT_ANIMATION_CALC_U    (SELECT_ANIMATION_TEX_U_INI * m_nPatternAnimU) // テクスチャアニメーションU座標の計算式
#define SELECT_ANIMATION_SWITCH    (6)                                            // テクスチャ座標を切り替えるタイミング

#define PRESSLOGO_TEXTURE_NAME     "data/TEXTURE/COMMON/press.png"                // テクスチャのファイル名(何かボタンを押してください)
#define PRESSLOGO_DRAW_TIMING      (80)                                           // PRESSLOGOを描画する時間
#define PRESSLOGO_RETURN_TIMING    (100)                                          // PRESSLOGOのカウンターを戻す時間
#define PRESSLOGO_PUSH_DRAW_TIMING (4)                                            // 決定ボタンが押されたときにPRESSLOGOを描画するタイミング

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLogoSelect::m_apTexture[TYPE_MAX] = {};  // テクスチャへのポインタ(セレクトロゴ用)
LPDIRECT3DTEXTURE9 CLogoPause::m_apTexture[TYPE_MAX] = {};   // テクスチャへのポインタ(ポーズロゴ用)
LPDIRECT3DTEXTURE9 CLogoPress::m_pTexture = NULL;            // テクスチャへのポインタ(PRESSENTER用)

float              CLogoSelect::m_fColTrans = 0.01f;         // 色の変化に使用する
float              CLogoPause::m_fColTrans = 0.01f;          // 色の変化に使用する

//*****************************************************************************
//    CLogoの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CLogo::CLogo(int nPriority, OBJTYPE objType) : CScene2DRot(nPriority, objType)
{
	// 各種値の初期化
}

//=============================================================================
//    デストラクタ
//=============================================================================
CLogo::~CLogo()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CLogo *CLogo::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, int nPriority)
{
	CLogo *pLogo = NULL;   // ロゴクラス型のポインタ
	if (pLogo == NULL)
	{// メモリが確保できる状態にある
		pLogo = new CLogo(nPriority); // ロゴクラスのインスタンス生成
		if (pLogo != NULL)
		{// メモリが確保できた
		    // 初期化処理
			if (FAILED(pLogo->Init(pos, col, fScale)))
			{// 初期化に失敗した
				return NULL;
			}
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

	return pLogo;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale)
{
	// 共通の初期化処理
	if (FAILED(CScene2DRot::Init(pos, col, fScale, 0.0f)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CLogo::Uninit(void)
{
	// 共通の描画処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CLogo::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CLogo::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}

//*****************************************************************************
//    CLogoSelectの処理
//*****************************************************************************
//=============================================================================
//    生成処理
//=============================================================================
CLogoSelect *CLogoSelect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state, int nPriority)
{
	CLogoSelect *pLogoSelect = NULL;   // セレクトロゴクラス型のポインタ
	if (pLogoSelect == NULL)
	{// メモリが確保できる状態にある
		pLogoSelect = new CLogoSelect(nPriority); // セレクトロゴクラスのインスタンス生成
		if (pLogoSelect != NULL)
		{// メモリが確保できた
		    // 初期化処理
			if (FAILED(pLogoSelect->Init(pos, col, fScale, type, state)))
			{// 初期化に失敗した
				return NULL;
			}
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

	return pLogoSelect;
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CLogoSelect::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		char aTextureName[TYPE_MAX][256] =
		{// テクスチャのファイルパス名
			SELECTLOGO_TEXTURE_NAME_0,
			SELECTLOGO_TEXTURE_NAME_1,
			SELECTLOGO_TEXTURE_NAME_2
		};
		for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
		{// 読み込むテクスチャの数だけ繰り返し
			D3DXCreateTextureFromFile(pDevice, aTextureName[nCntTex], &m_apTexture[nCntTex]);
		}
	}

	return S_OK;
}

//=============================================================================
//    テクスチャの解放処理
//=============================================================================
void CLogoSelect::UnLoad(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    コンストラクタ
//=============================================================================
CLogoSelect::CLogoSelect(int nPriority) : CScene2DRot(nPriority)
{
	// 各種値の初期化
	m_fColTrans = 0.01f;     // 色の変化に使用
	m_State = STATE_NONE;    // 状態
	m_Type = TYPE_GAME;      // 種類
	m_nCounterAnim = 0;      // アニメーションカウンター
	m_nPatternAnimU = 0;     // 現在のテクスチャU座標
}

//=============================================================================
//    デストラクタ
//=============================================================================
CLogoSelect::~CLogoSelect()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CLogoSelect::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state)
{
	// 各種値の設定
	SetObjType(OBJTYPE_LOGO);       // オブジェクトの種類
	BindTexture(m_apTexture[type]); // テクスチャ
	SetState(state);                // 状態
	SetType(type);                  // 種類

	// 共通の初期化処理
	if (FAILED(CScene2DRot::Init(pos, col, fScale, 0.0f)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CLogoSelect::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CLogoSelect::Update(void)
{
	if (m_State == STATE_SELECT)
	{// 選択されている状態だったら
		m_nCounterAnim++;  // アニメーションカウンターを進める

		D3DXCOLOR col = GetCol();  // 色を取得

		// 色を変化させる
		col.r += m_fColTrans;  // 赤色
		col.g += m_fColTrans;  // 緑色
		col.b += m_fColTrans;  // 青色

		if (col.r < 0.3f || col.r > 1.0f
			|| col.g < 0.3f || col.g > 1.0f
			|| col.b < 0.3f || col.b > 1.0f)
		{// 色が既定の値を超えた
			m_fColTrans *= -1;  // 色の変化を逆転させる
		}

		// 色の設定
		SetCol(col);

		// 頂点情報の設定
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファの取得
		VERTEX_2D *pVtx;                                  // 頂点バッファへのポインタ

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラー
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		if (m_nCounterAnim % SELECT_ANIMATION_SWITCH == 0)
		{// アニメーション切り替えタイミングに到達した
			m_nPatternAnimU = m_nPatternAnimU + 1 % SELECT_ANIMATION_PATTERN;

			// テクスチャ座標
			pVtx[0].tex.x = 0.0f + SELECT_ANIMATION_CALC_U;
			pVtx[1].tex.x = SELECT_ANIMATION_TEX_U_INI + SELECT_ANIMATION_CALC_U;
			pVtx[2].tex.x = 0.0f + SELECT_ANIMATION_CALC_U;
			pVtx[3].tex.x = SELECT_ANIMATION_TEX_U_INI + SELECT_ANIMATION_CALC_U;
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CLogoSelect::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}

//=============================================================================
//    種類の設定
//=============================================================================
void CLogoSelect::SetType(TYPE type)
{
	m_Type = type;
}

//=============================================================================
//    状態の設定
//=============================================================================
void CLogoSelect::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
//    種類の取得
//=============================================================================
CLogoSelect::TYPE CLogoSelect::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    状態の取得
//=============================================================================
CLogoSelect::STATE CLogoSelect::GetState(void)
{
	return m_State;
}

//=============================================================================
//    アニメーションパターンを戻す処理
//=============================================================================
void CLogoSelect::ResetTexU(void)
{
	// 頂点情報の設定
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファの取得

	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
		VERTEX_2D *pVtx;  // 頂点バッファへのポインタ

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標
		pVtx[0].tex.x = 0.0f;
		pVtx[1].tex.x = SELECT_ANIMATION_TEX_U_INI;
		pVtx[2].tex.x = 0.0f;
		pVtx[3].tex.x = SELECT_ANIMATION_TEX_U_INI;

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//*****************************************************************************
//    CLogoPauseの処理
//*****************************************************************************
//=============================================================================
//    生成処理
//=============================================================================
CLogoPause *CLogoPause::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state, int nPriority)
{
	CLogoPause *pLogoPause = NULL;   // ポーズロゴクラス型のポインタ
	if (pLogoPause == NULL)
	{// メモリが確保できる状態にある
		pLogoPause = new CLogoPause(nPriority); // ポーズロゴクラスのインスタンス生成
		if (pLogoPause != NULL)
		{// メモリが確保できた
            // 初期化処理
			if (FAILED(pLogoPause->Init(pos, col, fScale, type, state)))
			{// 初期化に失敗した
				return NULL;
			}
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

	return pLogoPause;
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CLogoPause::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		char aTextureName[TYPE_MAX][256] =
		{// テクスチャのファイルパス名
			PAUSELOGO_TEXTURE_NAME_0,
			PAUSELOGO_TEXTURE_NAME_1,
			PAUSELOGO_TEXTURE_NAME_2
		};
		for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
		{// 読み込むテクスチャの数だけ繰り返し
			D3DXCreateTextureFromFile(pDevice, aTextureName[nCntTex], &m_apTexture[nCntTex]);
		}
	}

	return S_OK;
}

//=============================================================================
//    テクスチャの解放処理
//=============================================================================
void CLogoPause::UnLoad(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    コンストラクタ
//=============================================================================
CLogoPause::CLogoPause(int nPriority) : CScene2DRot(nPriority)
{
	// 各種値の初期化
	m_fColTrans = 0.01f;     // 色の変化に使用
	m_State = STATE_NONE;    // 状態
	m_Type = TYPE_CONTINUE;  // 種類
}

//=============================================================================
//    デストラクタ
//=============================================================================
CLogoPause::~CLogoPause()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CLogoPause::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state)
{
	// 各種値の設定
	SetObjType(OBJTYPE_LOGO);       // オブジェクトの種類
	BindTexture(m_apTexture[type]); // テクスチャ
	SetState(state);                // 状態
	SetType(type);                  // 種類

	// 共通の初期化処理
	if (FAILED(CScene2DRot::Init(pos, col, fScale, 0.0f)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CLogoPause::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CLogoPause::Update(void)
{
	if (m_State == STATE_SELECT)
	{// 選択されている状態だったら
		D3DXCOLOR col = GetCol();  // 色を取得

		// 色を変化させる
		col.r += m_fColTrans;  // 赤色
		col.g += m_fColTrans;  // 緑色
		col.b += m_fColTrans;  // 青色

		if (col.r < 0.3f || col.r > 1.0f
			|| col.g < 0.3f || col.g > 1.0f
			|| col.b < 0.3f || col.b > 1.0f)
		{// 色が既定の値を超えた
			m_fColTrans *= -1;  // 色の変化を逆転させる
		}

		// 色の設定
		SetCol(col);

		// 頂点情報の設定
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファの取得

		if (pVtxBuff != NULL)
		{
			VERTEX_2D *pVtx;                                  // 頂点バッファへのポインタ

			// 頂点バッファをロックし,頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点カラー
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			// 頂点バッファをアンロックする
			pVtxBuff->Unlock();
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CLogoPause::Draw(void)
{
	// 共通の終了処理
	CScene2D::Draw();
}

//=============================================================================
//    種類の設定
//=============================================================================
void CLogoPause::SetType(TYPE type)
{
	m_Type = type;
}

//=============================================================================
//    状態の設定
//=============================================================================
void CLogoPause::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
//    種類の取得
//=============================================================================
CLogoPause::TYPE CLogoPause::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    状態の取得
//=============================================================================
CLogoPause::STATE CLogoPause::GetState(void)
{
	return m_State;
}

//*****************************************************************************
//    CLogoPressの処理
//*****************************************************************************
//=============================================================================
//    生成処理
//=============================================================================
CLogoPress *CLogoPress::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, int nPriority)
{
	CLogoPress *pLogoPress = NULL;   // PRESSENTERクラス型のポインタ
	if (pLogoPress == NULL)
	{// メモリが確保できる状態にある
		pLogoPress = new CLogoPress(nPriority); // PRESSENTERロゴクラスのインスタンス生成
		if (pLogoPress != NULL)
		{// メモリが確保できた
		 // 初期化処理
			if (FAILED(pLogoPress->Init(pos, col, fScale)))
			{// 初期化に失敗した
				return NULL;
			}
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

	return pLogoPress;
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CLogoPress::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, PRESSLOGO_TEXTURE_NAME, &m_pTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    テクスチャの解放処理
//=============================================================================
void CLogoPress::UnLoad(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//    コンストラクタ
//=============================================================================
CLogoPress::CLogoPress(int nPriority) : CScene2DRot(nPriority)
{
	// 各種値の初期化
	m_State = STATE_NONE;    // 状態
	m_nCounter = 0;          // 描画を管理するカウンター
}

//=============================================================================
//    デストラクタ
//=============================================================================
CLogoPress::~CLogoPress()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CLogoPress::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale)
{
	// 各種値の設定
	SetObjType(OBJTYPE_LOGO);   // オブジェクトの種類
	BindTexture(m_pTexture);    // テクスチャ

	// 共通の初期化処理
	if (FAILED(CScene2DRot::Init(pos, col, fScale, 0.0f)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CLogoPress::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CLogoPress::Update(void)
{
	m_nCounter++;  // カウンターを進める

	if (m_nCounter >= PRESSLOGO_RETURN_TIMING)
	{// カウンターが既定の値を超えた
		m_nCounter = 0;
	}

	if (m_State == STATE_NONE)
	{// 何もボタンが押されていない状態だったら
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードが使用されていない状態である
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// 決定ボタンが押された
				m_State = STATE_PUSH;
			}
			else
			{// キーボードの決定ボタンが押されていない
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_12; nCntButton++)
				{// 反応するボタン数の数だけ繰り返し
					if (CManager::GetJoyStick()->GetJoyStickDevice(0)->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// ボタンが押された
						m_State = STATE_PUSH;
					}
				}
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CLogoPress::Draw(void)
{
	if (m_State == STATE_NONE)
	{// 何もボタンが押されていない状態だったら
		if (m_nCounter / PRESSLOGO_DRAW_TIMING == 0)
		{// カウンターが既定の値に達した
            // 共通の描画処理
			CScene2D::Draw();
		}
	}
	else if (m_State == STATE_PUSH)
	{// 決定ボタンが押された状態だったら
		if (m_nCounter % PRESSLOGO_PUSH_DRAW_TIMING == 0)
		{// カウンターが既定の値になった
		    // 共通の描画処理
			CScene2D::Draw();
		}
	}
}

//=============================================================================
//    状態の設定
//=============================================================================
void CLogoPress::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
//    状態の取得
//=============================================================================
CLogoPress::STATE CLogoPress::GetState(void)
{
	return m_State;
}