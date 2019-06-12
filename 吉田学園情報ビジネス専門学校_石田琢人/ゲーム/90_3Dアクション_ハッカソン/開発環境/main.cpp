//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// メイン処理 [main.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "main.h"
#include "input.h"
#include "Gamepad.h"
#include "fade.h"
#include "game.h"
#include "sound.h"

#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "object.h"

#if _DEBUG
#include "Debug_Log.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"					// ウインドウのクラス名
#define WINDOW_NAME		"一寸先は滝"				// ウインドウのキャプション名
#define UPDATE_FRAME	(60)						// １秒間に更新する回数
#define WINDOW_MODE		(true)						// ウィンドウモード

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D			= NULL;				// Direct3Dオブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice	= NULL;				// デバイスオブジェクト(描画に必要)
MODE				g_mode			= MODE_TITLE;		// モードの設定
int					g_nCountFPS		= 0;				// FPSカウンタ

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// メイン関数
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwExecLastTime;
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
							CLASS_NAME,
							WINDOW_NAME,
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							(rect.right - rect.left),
							(rect.bottom - rect.top),
							NULL,
							NULL,
							hInstance,
							NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, WINDOW_MODE)))
	{
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// 各カウンターを初期化
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// システム時刻を取得(ミリ秒単位)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// システム時刻を取得(ミリ秒単位)

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// システム時刻を取得
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
			 // FPSを測定
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		// FPSを測定した時刻を保存
				dwFrameCount = 0;					// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / UPDATE_FRAME))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;		// 処理を実行した時刻を保存

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;		// カウントを加算
			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// プロシージャ
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount			 = 1;							// バックバッファの数
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil	 = true;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成	[Zテストで使う]
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed					 = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	//++++++++++++++++++++++++++++++++++++++++
	// ファイル読み込み
	//++++++++++++++++++++++++++++++++++++++++

	// キーボード
	InitKeyboard(hInstance, hWnd);

	// ゲームパッド
	InitGamapad(hInstance, hWnd);

	// サウンド
	InitSound(hWnd);

	// フェード
	InitFade(g_mode);

	// モード
	SetMode(g_mode);

#if _DEBUG
	// デバッグログ
	InitDebug_Log();
#endif

	// randの値を更新する
	srand((unsigned int)time(0));

	return S_OK;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void Uninit(void)
{
	//++++++++++++++++++++++++++++++++++++++++
	// ファイル読み込み情報の破棄
	//++++++++++++++++++++++++++++++++++++++++

	// キーボード
	UninitKeyboard();

	// ゲームパッド
	UninitGamapad();

	// フェード
	UninitFade();

	// サウンド
	UninitSound();

#if _DEBUG
	// デバッグログ
	UninitDebug_Log();
#endif

	if(g_pD3DDevice != NULL)
	{// デバイスオブジェクトの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void Update(void)
{
	// キーボード
	UpdateKeyboard();

	// ゲームパッド
	UpdateGamapad();

	// 各画面の更新処理
	switch (g_mode)
	{
	case MODE_TITLE:		// タイトル
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		// チュートリアル
		UpdateTutorial();
		break;

	case MODE_GAME:			// ゲーム本編
		UpdateGame();
		break;

	case MODE_RESULT:		// リザルト
		UpdateResult();
		break;
	}

	// フェード
	UpdateFade();

#if _DEBUG
	// デバッグログ
	UpdateDebug_Log();
#endif
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.0f) , 1.0f, 0);		//D3DCOLOR_RGBA(0, 0, 0, 0)

	// 描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

		// 各画面の描画処理
		switch (g_mode)
		{
		case MODE_TITLE:		// タイトル
			DrawTitle();
			break;

		case MODE_TUTORIAL:		// チュートリアル
			DrawTutorial();
			break;

		case MODE_GAME:			// ゲーム本編
			DrawGame();
			break;

		case MODE_RESULT:		// リザルト
			DrawResult();
			break;
		}

		// フェード
		DrawFade();

#if _DEBUG
		// デバッグログ
		DrawDebug_Log();
#endif

		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// デバイスの取得
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}


//=============================================================================
// モード設定処理
//=============================================================================
void SetMode(MODE mode)
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 各画面の終了処理  -----  現在の状態 [モード]  -----
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	switch (g_mode)
	{
	case MODE_TITLE:		// タイトル
		UninitTitle();
		// 音声停止
		StopSound();
		break;

	case MODE_TUTORIAL:		// チュートリアル
		UninitTutorial();
		// 音声停止
		StopSound();
		break;

	case MODE_GAME:			// ゲーム本編
		UninitGame();
		// 音声停止
		StopSound();
		break;
	case MODE_RESULT:		// リザルト
		UninitResult();
		// 音声停止
		StopSound();
		break;
	}

	// 次のモードを設定する
	g_mode = mode;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 各画面の初期化処理  -----  次の状態 [モード]  -----
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	switch (mode)
	{
	case MODE_TITLE:		// タイトル
		InitTitle();
		// BGM [タイトル]
		PlaySound(SOUND_LABEL_BGM000);
		break;

	case MODE_TUTORIAL:		// チュートリアル
		InitTutorial();
		PlaySound(SOUND_LABEL_BGM000);

		break;

	case MODE_GAME:			// ゲーム本編
		InitGame();
		// BGM [ゲーム]
		PlaySound(SOUND_LABEL_BGM005);
		PlaySound(SOUND_LABEL_BGM_WATER);		// 川の音
		break;

	case MODE_RESULT:		// リザルト
		InitResult();
		// BGM [リザルト]
		PlaySound(SOUND_LABEL_BGM002);
		break;
	}
	// 次のモードを設定する
	g_mode = mode;
}

//=============================================================================
// モード取得処理
//=============================================================================
MODE GetMode(void)
{
	return g_mode;		// 現在の状態 [モード] を返す
}

//=============================================================================
// FPS取得処理
//=============================================================================
int GetFPS(void)
{
	return g_nCountFPS;		// FPSを返す
}