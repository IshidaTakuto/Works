//=============================================================================
//
// レンダリング処理 [renderer.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

//==================================
// 静的メンバ変数宣言
//==================================

//=========================================
// コンストラクタ
//=========================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;			// Direct3Dオブジェクト
	m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)

#ifdef _DEBUG
	m_pFont = NULL;			// フォントへのポインタ
#endif

}

//=========================================
// デストラクタ
//=========================================
CRenderer::~CRenderer()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
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
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

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
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	//ハードウェアの能力を表す構造体
	D3DCAPS9 caps;

	//フォグパラメータ
	const FLOAT fogStart = 300;
	const FLOAT fogEnd = 7800;

	ZeroMemory(&caps, sizeof(D3DCAPS9));

	m_pD3DDevice->GetDeviceCaps(&caps);

	//フォグを使用する
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//フォグの色を設定する
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(255, 130, 130, 130));

	//頂点フォグを使用しない
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);

	//フォグの適用方法
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	//フォグの開始位置
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fogStart));

	//フォグの終了位置
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&fogEnd));

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

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

//=========================================
// 更新処理
//=========================================
void CRenderer::Update(void)
{
	CScene::UpdateAll();
}

//=========================================
// 描画処理
//=========================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CCamera *pCamera = NULL;			// カメラの情報を格納
		pCamera = CManager::GetCamera();	// カメラの情報を取得

		if (NULL != pCamera)
		{// ヌルチェック
			pCamera->Set();	// カメラのセット
		}

		CManager::MODE mode = CManager::GetMode();

		CTitle *pTitle = NULL;
		CTutorial *pTutorial = NULL;
		CGame *pGame = NULL;
		CResult *pResult = NULL;
		CRanking *pRanking = NULL;
		CScene *pScene = NULL;

		switch (mode)
		{
		case MODE_TITLE:	// タイトル
			pTitle = CManager::GetTitle();	// タイトルの取得
			if (pTitle != NULL)
			{
				pTitle->Draw();
			}

			CScene::DrawAll();	// オブジェクトを描画する

			break;

		case MODE_TUTORIAL:	// チュートリアル
			// フェード以外のオブジェクトの描画
			for (int nCntPriority = 0; nCntPriority < 7; nCntPriority++)
			{
				for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
				{
					pScene = CScene::GetScene(nCntPriority, nCntObject);

					if (pScene != NULL)
					{// オブジェクトの描画
						pScene->Draw();
					}
				}
			}

			pTutorial = CManager::GetTutorial();	// チュートリアルの取得
			if (pTutorial != NULL)
			{// チュートリアルクラスの描画
				pTutorial->Draw();
			}

			for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
			{
				pScene = CScene::GetScene(NUM_PRIORITY - 1, nCntObject);

				if (pScene != NULL)
				{// オブジェクトの描画
					pScene->Draw();
				}
			}

			break;

		case MODE_GAME:		// ゲーム
			// フェード以外のオブジェクトの描画
			for (int nCntPriority = 0; nCntPriority < 7; nCntPriority++)
			{
				for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
				{
					pScene = CScene::GetScene(nCntPriority, nCntObject);

					if (pScene != NULL)
					{// オブジェクトの描画
						pScene->Draw();
					}
				}
			}

			pGame = CManager::GetGame();	// ゲームの取得
			if (pGame != NULL)
			{// ゲームの描画
				pGame->Draw();
			}

			for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
			{
				pScene = CScene::GetScene(NUM_PRIORITY - 1, nCntObject);

				if (pScene != NULL)
				{// オブジェクトの描画
					pScene->Draw();
				}
			}

			break;

		case MODE_RESULT:		// リザルト
			pResult = CManager::GetResult();	// ゲームの取得
			if (pResult != NULL)
			{
				pResult->Draw();
			}

			CScene::DrawAll();	// オブジェクトを描画する
			break;

		case MODE_GAMEOVER:
			pResult = CManager::GetResult();	// ゲームの取得
			if (pResult != NULL)
			{
				pResult->Draw();
			}

			CScene::DrawAll();	// オブジェクトを描画する
			break;

		case MODE_RANKING:		// リザルト
			pRanking = CManager::GetRanking();	// ゲームの取得
			if (pRanking != NULL)
			{
				pRanking->Draw();
			}

			CScene::DrawAll();	// オブジェクトを描画する
			break;
		}

#ifdef _DEBUG
		// FPS表示
		DrawFPS();
#endif
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void CRenderer::DrawFPS(void) const
{
	int nCntFPS;

	nCntFPS = GetFPS();

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", nCntFPS);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 50, 200, 0xff));
}
#endif
