//*****************************************************************************
//
//     レンダリングの処理[renderer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "pause.h"
#include "fade.h"
#include "sound.h"


#include "debuglog.h"

//=============================================================================
//    コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	// 各種値のクリア
	m_pD3DDevice = NULL;   // デバイスへのポインタ
	m_pD3D = NULL;         // Direct3Dオブジェクトへのポインタ
	m_pPause = NULL;       // ポーズクラス型のポインタ
	m_bPause = false;      // ポーズ中かどうか
	m_bPauseRelease = false;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							            // バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				            // ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				            // ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;			            	// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		                // 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						    // デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					    // デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						                // ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	    // インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);              // 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                      // Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);             // αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                     // ライティングを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);        // αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // αデスティネーションカラーの指定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // アルファブレンディング処理を設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); // ２番目のアルファ引数

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
	// ポーズクラスの破棄
	if (m_pPause != NULL)
	{
		// 終了処理
		m_pPause->Uninit();
		m_pPause = NULL;
	}

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CRenderer::Update(void)
{
	CManager::MODE mode = CManager::GetMode();  // 現在のモードを取得

	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	if (mode == CManager::MODE_GAME)
	{// ゲーム中だったら
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードしていなかったら
			if (CManager::GetKeyboard()->GetTrigger(DIK_P) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_11) == TRUE)
			{// ポーズON/OFF
				m_bPause = m_bPause ? false : true;
				if (m_pPause == NULL && m_bPause == true)
				{// ポーズ中にメモリが確保されていない
					m_pPause = CPause::Create();
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);
				}
				else if (m_pPause != NULL)
				{// ポーズ解除時にメモリが確保されている
				    // ポーズの開放
					ReleasePause();
				}
			}
		}
	}

	if (m_bPause == false)
	{// ポーズ中でない
	    // シーンクラスの更新処理
		CScene::UpdateAll();
	}
	else
	{// ポーズ中である
		if (m_pPause != NULL)
		{// メモリが確保されている
		    // ポーズの更新処理
			m_pPause->Update();

			if (m_pPause == NULL)
			{
				// 死亡フラグを確認する
				CScene::DeathCheck();
			}

			CDebugProc::Print("ポーズ中\n");
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// カメラの設定
		CManager::GetCamera()->SetCamera();

		// シーンクラスの描画処理
		CScene::DrawAll();

		// デバック情報表示
		CDebugProc::Draw();

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
//    デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}

//=============================================================================
//    ポーズの開放
//=============================================================================
void CRenderer::ReleasePause(void)
{
	if (m_pPause != NULL)
	{// ポーズ解除時にメモリが確保されている
	   // ポーズの終了処理
		m_pPause->Uninit();
		m_pPause = NULL;
	}
}

//=============================================================================
//    ポーズの設定
//=============================================================================
void CRenderer::SetPause(bool bPause)
{
	m_bPause = bPause;
}

//=============================================================================
//    ポーズ中かどうか取得する
//=============================================================================
bool CRenderer::GetPause(void)
{
	return m_bPause;
}