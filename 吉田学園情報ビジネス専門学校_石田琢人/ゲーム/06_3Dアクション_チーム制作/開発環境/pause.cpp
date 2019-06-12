//*****************************************************************************
//
//     ポーズの処理[pause.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "logo.h"
#include "scene2DSquare.h"
#include "scene2DRot.h"
#include "fade.h"
#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define PAUSE_PRIORITY         (7)                                                     // ポーズの描画優先順位

#define PAUSEBLACK_COL_INI     (D3DXCOLOR(0.0f,0.0f,0.0f,0.8f))                        // ポーズの黒いポリゴンの色

#define PAUSEBG_TEXTURE_NAME   "data/TEXTURE/GAME/pause.png"                           // ポーズの背景のテクスチャファイスパス名
#define PAUSEBG_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f))  // ポーズの背景の座標
#define PAUSEBG_WIDTH_INI      (300.0f)                                                // ポーズの背景の幅
#define PAUSEBG_HEIGHT_INI     (310.0f)                                                // ポーズの背景の高さ

#define PAUSESELECT_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH / 2,150.0f,0.0f))             // ポーズ項目の座標(初期値)
#define PAUSESELECT_COL_INI    (D3DXCOLOR(0.5f,0.5f,0.5f,1.0f))                        // ポーズ項目の色(初期値)
#define PAUSESELECT_SCALE_INI  (240.0f)                                                // ポーズ項目の大きさ(初期値)
#define PAUSESELECT_SCALE_NONE (150.0f)                                                // 選択されていないポーズ項目の大きさ(初期値)
#define PAUSESELECT_COL_NONE   (D3DXCOLOR(0.1f,0.1f,0.1f,1.0f))                        // 選択されていないポーズ項目の色(初期値)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPause::m_pTexture = NULL;    // テクスチャへのポインタ

//=============================================================================
//    コンストラクタ
//=============================================================================
CPause::CPause(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値の初期化
	m_pBlackPolygon = NULL;                // ポーズに使用する黒いポリゴン
	m_pPauseBg = NULL;                     // ポーズの背景用ポリゴン
	m_nSelect = SELECT_CONTINUE;           // 現在選択している項目
	m_nCounter = 0;                        // 描画管理カウンター
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// ポーズ画面の選択項目の数だけ繰り返し
		m_apLogoPause[nCntPause] = NULL;  // ポーズロゴクラス型のポインタ
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CPause *CPause::Create(void)
{
	CPause *pPause = NULL;  // ポーズクラス型のポインタ
	if (pPause == NULL)
	{// メモリが確保できる状態にある
		pPause = new CPause;
		if (pPause != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pPause->Init();
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
	return pPause;
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CPause::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if(pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, PAUSEBG_TEXTURE_NAME, &m_pTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    テクスチャの解放処理
//=============================================================================
void CPause::UnLoad(void)
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
HRESULT CPause::Init(void)
{
	// 各種値の設定
	SetObjType(OBJTYPE_PAUSE);  // ポーズ

	// ポーズの黒いポリゴンインスタンス生成処理
	if(m_pBlackPolygon == NULL)
	{// メモリが確保できる状態にある
		m_pBlackPolygon = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), PAUSEBLACK_COL_INI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, PAUSE_PRIORITY);
	}

	// ポーズの背景用のポリゴンインスタンス生成処理
	if (m_pPauseBg == NULL)
	{// メモリが確保できる状態にある
		m_pPauseBg = CScene2DSquare::Create(PAUSEBG_POS_INI, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSEBG_WIDTH_INI, PAUSEBG_HEIGHT_INI, PAUSE_PRIORITY);
		if (m_pPauseBg != NULL)
		{// メモリが確保できた
			// テクスチャの割り当て
			m_pPauseBg->BindTexture(m_pTexture);
		}
	}

	D3DXVECTOR3 pos = PAUSESELECT_POS_INI;                // 座標
	D3DXCOLOR col = PAUSESELECT_COL_INI;                  // 色
	float fScale = PAUSESELECT_SCALE_INI;                 // 大きさ
	CLogoPause::STATE state = CLogoPause::STATE_SELECT;   // 状態

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// ポーズ画面の選択項目の数だけ繰り返し
		if (m_apLogoPause[nCntPause] == NULL)
		{// メモリが確保できる状態にある
			m_apLogoPause[nCntPause] = CLogoPause::Create(pos, col, fScale, (CLogoPause::TYPE)nCntPause, state, PAUSE_PRIORITY);
			if (m_apLogoPause[nCntPause] != NULL)
			{
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogoPause[nCntPause]->GetVtxBuff(); // 頂点情報取得
				if (pVtxBuff != NULL)
				{// 頂点情報を取得できた
				    // 頂点情報設定
					VERTEX_2D *pVtx;
					// 頂点バッファをロックし,頂点データへのポインタを取得
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// 頂点座標
					pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.65f) * fScale, pos.y + cosf(-D3DX_PI * 0.65f) * fScale, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.65f) * fScale, pos.y + cosf(D3DX_PI * 0.65f) * fScale, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.35f) * fScale, pos.y + cosf(-D3DX_PI * 0.35f) * fScale, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.35f) * fScale, pos.y + cosf(D3DX_PI * 0.35f) * fScale, 0.0f);

					// 頂点バッファをアンロックする
					pVtxBuff->Unlock();

					// 頂点バッファを設定する
					m_apLogoPause[nCntPause]->SetVtxBuff(pVtxBuff);
				}
			}
		}

		if (nCntPause == 0)
		{// 1回目の繰り返しが終わった
			fScale = PAUSESELECT_SCALE_NONE;
			col = PAUSESELECT_COL_NONE;
			state = CLogoPause::STATE_NONE;
		}
		pos.y += 200.0f;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CPause::Uninit(void)
{
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// ポーズ画面の選択項目の数だけ繰り返し
		if (m_apLogoPause[nCntPause] != NULL)
		{// メモリが確保されている
			// ポーズロゴの終了処理
			m_apLogoPause[nCntPause]->Uninit();
			m_apLogoPause[nCntPause] = NULL;
		}
	}

	// 背景ポリゴンの開放
	if (m_pPauseBg != NULL)
	{// メモリが確保されている
		m_pPauseBg->Uninit();
		m_pPauseBg = NULL;
	}

	// 黒いポリゴンの開放処理
	if (m_pBlackPolygon != NULL)
	{// メモリが確保されている
		m_pBlackPolygon->Uninit();
		m_pBlackPolygon = NULL;
	}

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CPause::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();  // キーボードの取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// ポーズ画面の選択項目の数だけ繰り返し
		if (m_apLogoPause[nCntPause] != NULL)
		{// メモリが確保されている
		    // ポーズロゴの終了処理
			m_apLogoPause[nCntPause]->Update();
		}
	}


	// 選択項目の処理
	if (pKeyboard->GetTrigger(DIK_W) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_20) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_28) == TRUE
		|| pKeyboard->GetRepeat(DIK_W) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_20) == TRUE || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_28) == TRUE)
	{// 上方向の入力がされた

		if (m_apLogoPause[m_nSelect] != NULL)
		{
			// 選択されていた項目の設定
			m_apLogoPause[m_nSelect]->SetState(CLogoPause::STATE_NONE);                 // 状態
			m_apLogoPause[m_nSelect]->SetVtxBuffCol(PAUSESELECT_COL_NONE);              // 色
			m_apLogoPause[m_nSelect]->SetScale(PAUSESELECT_SCALE_NONE);                 // 大きさ
			m_apLogoPause[m_nSelect]->SetVtxBuffPos(m_apLogoPause[m_nSelect]->GetPos());// 座標

			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogoPause[m_nSelect]->GetVtxBuff(); // 頂点情報取得
			if (pVtxBuff != NULL)
			{// 頂点情報を取得できた
			    // 頂点情報設定
				VERTEX_2D *pVtx;
				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				D3DXVECTOR3 pos = m_apLogoPause[m_nSelect]->GetPos();

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, 0.0f);

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();

				// 頂点バッファを設定する
				m_apLogoPause[m_nSelect]->SetVtxBuff(pVtxBuff);
			}
		}

		// 次に選択される項目の設定
		if (m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX] != NULL)
		{
			m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetState(CLogoPause::STATE_SELECT);                                                 // 状態
			m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetVtxBuffCol(PAUSESELECT_COL_INI);                                                 // 色
			m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetScale(PAUSESELECT_SCALE_INI);                                                    // 大きさ
			m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetVtxBuffPos(m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->GetPos());// 座標

			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->GetVtxBuff(); // 頂点情報取得
			if (pVtxBuff != NULL)
			{// 頂点情報を取得できた
			    // 頂点情報設定
				VERTEX_2D *pVtx;
				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				D3DXVECTOR3 pos = m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->GetPos();

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, pos.y + cosf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, pos.y + cosf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, pos.y + cosf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, pos.y + cosf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, 0.0f);

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();

				// 頂点バッファを設定する
				m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetVtxBuff(pVtxBuff);
			}
		}

		// 番号を設定
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}
	else if (pKeyboard->GetTrigger(DIK_S) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_21) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_29) == TRUE
		|| pKeyboard->GetRepeat(DIK_S) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_21) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_29) == TRUE)
	{// 下方向の入力がされた
	    // 選択されていた項目の設定
		if (m_apLogoPause[m_nSelect] != NULL)
		{
			m_apLogoPause[m_nSelect]->SetState(CLogoPause::STATE_NONE);                 // 状態
			m_apLogoPause[m_nSelect]->SetVtxBuffCol(PAUSESELECT_COL_NONE);              // 色
			m_apLogoPause[m_nSelect]->SetScale(PAUSESELECT_SCALE_NONE);                 // 大きさ
			m_apLogoPause[m_nSelect]->SetVtxBuffPos(m_apLogoPause[m_nSelect]->GetPos());// 座標

			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogoPause[m_nSelect]->GetVtxBuff(); // 頂点情報取得
			if (pVtxBuff != NULL)
			{// 頂点情報を取得できた
			    // 頂点情報設定
				VERTEX_2D *pVtx;
				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				D3DXVECTOR3 pos = m_apLogoPause[m_nSelect]->GetPos();

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, 0.0f);

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();

				// 頂点バッファを設定する
				m_apLogoPause[m_nSelect]->SetVtxBuff(pVtxBuff);
			}
		}

		// 次に選択される項目の設定
		if (m_apLogoPause[(m_nSelect + 1) % SELECT_MAX] != NULL)
		{
			m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetState(CLogoPause::STATE_SELECT);                                  // 状態
			m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetVtxBuffCol(PAUSESELECT_COL_INI);                                  // 色
			m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetScale(PAUSESELECT_SCALE_INI);                                     // 大きさ
			m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetVtxBuffPos(m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->GetPos());// 座標

			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->GetVtxBuff(); // 頂点情報取得
			if (pVtxBuff != NULL)
			{// 頂点情報を取得できた
			    // 頂点情報設定
				VERTEX_2D *pVtx;
				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				D3DXVECTOR3 pos = m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->GetPos();

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, pos.y + cosf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, pos.y + cosf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, pos.y + cosf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, pos.y + cosf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, 0.0f);

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();

				// 頂点バッファを設定する
				m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetVtxBuff(pVtxBuff);
			}
		}
		// 番号を設定
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;

		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == TRUE)
	{// 決定ボタンが押された
		// レンダリングクラスの取得
		CRenderer *pRenderer = CManager::GetRenderer();

		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);

		switch (m_nSelect)
		{// 現在選択されていた項目によって処理わけ
		case CPause::SELECT_CONTINUE:  // 再開
			pRenderer->SetPause(false);
			break;
		case CPause::SELECT_RETRY:     // やり直し
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
			break;
		case CPause::SELECT_QUIT:      // 終了
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			break;
		}

		// ポーズの開放
		pRenderer->ReleasePause();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
//    選択項目の取得
//=============================================================================
int CPause::GetSelect(void)
{
	return m_nSelect;
}