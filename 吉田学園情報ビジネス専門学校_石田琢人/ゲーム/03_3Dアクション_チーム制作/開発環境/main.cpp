//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 杉本涼
//
//=============================================================================
#include "main.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "model.h"
#include "input.h"
#include "shadow.h"
#include "billboord.h"
#include "bullet.h"
#include "Title.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
#include "joypad.h"

#include "goburin.h"
#include "Selectmode.h"
#include "endstory.h"
#include "endroll.h"
#include "gameover.h"
#include "story.h"
#include "credit.h"

#include <stdlib.h>				//ランド用
#include <time.h>				//同じくランド用

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"勇者喰らい"		// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
LPD3DXFONT			g_pFont = NULL;			// フォントへのポインタ
void Uninit(void);
void Update(void);
void Draw(void);

void DrawSelTXIT(void);						//セレクト画面の文字
void DrawStoryTXIT(void);					//ストーリー画面文字
void DrawEndStoryTXIT(void);				//エンドストーリー画面文字
void DrawEndrollTXIT(void);					//エンドロール画面文字
void DrawSELECTTXIT(void);					//セレクト画面文字

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3Dオブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// デバイスオブジェクト(描画に必要)

SELECTSTATE g_aSelSTE;						//セレクトの状態
int g_aSelectMenuMain;						//セレクトのメニュー
int g_aSelCounter;							//セレクトの会話出すカウンター

int g_aStoryMenuMain;						//ストーリーのメニュー
int g_aStoryCounter;						//ストーリーの会話出すカウンター

int g_aEndStoryMenuMain;					//エンドストーリーのメニュー
int g_aEndStoryCounter;						//ストーリーの会話出すカウンター

//=============================================================================
// メイン関数
//=============================================================================
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
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	//int nTime;

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
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
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
			dwCurrentTime = timeGetTime();

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60秒経過

				//dwCurrentTime = 現在の時間
				//dwExecLastTime = 最後に通った時間

				//nTime = (dwCurrentTime - dwExecLastTime);
				//nTime = (1000 / 60);

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwExecLastTime = dwCurrentTime;
				dwFrameCount++;
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

//=============================================================================
// プロシージャ
//=============================================================================
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
	}
	g_aSelSTE = GetSelstate();
	if (g_aSelSTE == SELECT_QUIT)
	{
		PostQuitMessage(0);
		DestroyWindow(hWnd);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
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
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
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
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	D3DXCreateFont(g_pD3DDevice, 38/*文字の高さ*/, 16/*文字の幅*/, 0/*フォントの太さ*/, 0/*ミップマップレベル数*/, FALSE, SHIFTJIS_CHARSET,//HANGEUL_CHARSET韓国語//SHIFTJIS_CHARSET日本語
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("monospace")/* フォント名*/, &g_pFont);



	// 各種オブジェクトの初期化処理
	InitKeyboard(hInstance, hWnd);

	MODE *Mode;	//現在のモード確認のため
	GAME_PLAY *game_play = GetGame_Play();
	game_play[0].bUse = true;

	Mode = GetMode();	//現在のモードをもらう
	srand((unsigned int)time(0));	//ランドのため

	InitJoystick();
	InitSound(hWnd);
	InitFade(Mode[0]);
	InitWindow();
	InitFadeWindow();
	InitAnimation();
	InitExplosion();
	InitFont();
	StopSound();


	switch (Mode[0])
	{
	case MODE_TITLE:
		InitTitle();
		break;

	case MODE_RANKING:
		InitRanking();
		break;
	case MODE_SELCT:
		InitSelectmode();			//セレクトの初期化処理
		break;

	case MODE_GAME:
		InitGame(0);
		break;

	case MODE_START:
		InitStart();
		break;

	case MODE_STORY:
		InitStory();				//ストーリーの初期化処理
		break;

	case MODE_CREDIT:				//クレジットの
		InitCredit();
		break;

	case MODE_ENDSTORY:
		InitEndStory();				//エンドストーリーの初期化処理
		break;

	case MODE_ENDROLL:				//エンドロールの初期化処理
		InitEndRoll();
		break;

	case MODE_GAMEOVER:				//ゲームオーバーの初期化処理
		InitGameOver();
		break;

	case MODE_RESULT:
		InitResult();
		break;
	}


	HWND *hwnd;
	hwnd = GethWnd();
	hwnd[0] = hWnd;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	MODE *Mode;	//現在のモード確認のため

	Mode = GetMode();	//現在のモードをもらう

	StopSound();
	//各種オブジェクトの終了処理
	UninitKeyboard();

	// 各種オブジェクトの更新処理
	UninitKeyboard();

	switch (Mode[0])
	{
	case MODE_TITLE:

		UninitTitle();
		break;

	case MODE_RANKING:
		UninitRanking();
		break;

	case MODE_SELCT:
		UninitSelectmode();

	case MODE_GAME:
		UninitGame();
		break;

	case MODE_START:
		UninitStart();
		break;

	case MODE_STORY:
		UninitStory();
		break;

	case MODE_CREDIT:
		UninitCredit();
		break;

	case MODE_ENDSTORY:
		UninitEndStory();
		break;

	case MODE_ENDROLL:
		UninitEndRoll();
		break;

	case MODE_GAMEOVER:
		UninitGameOver();
		break;

	case MODE_RESULT:
		UninitResult();
		break;
	}
	UninitWindow();
	UninitFade();
	UninitFadeWindow();
	UninitFont();
	UninitSound();
	UninitAnimation();
	UninitExplosion();

	if (g_pFont != NULL)
	{// デバッグ表示用フォントの開放
		g_pFont->Release();
		g_pFont = NULL;
	}

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

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	MODE *Mode;	//現在のモード確認のため

	Mode = GetMode();	//現在のモードをもらう

	// 各種オブジェクトの更新処理
	ConnectJoystick();
	Trigger();

	UpdateKeyboard();
	ButtonKeybord();
	ButtonJoypad();
	GetKeyboardCount();

	switch (Mode[0])
	{
	case MODE_TITLE:
		UpdateTitle();			//タイトルの更新処理
		break;

	case MODE_RANKING:			//ランキングの更新処理
		UpdateRanking();
		break;

	case MODE_SELCT:
		UpdateSelectmode();		//セレクトの更新処理
		UpdateGOBURIN();		//ゴブリンの更新処理
		break;

	case MODE_GAME:
		UpdateGame();			//ゲームの更新処理
		break;

	case MODE_START:
		UpdateStart();			//キャラメイクの更新処理
		break;

	case MODE_STORY:
		UpdateStory();			//ストーリーの更新処理
		UpdateGOBURIN();		//ゴブリンの更新処理
		break;

	case MODE_CREDIT:			//クレジットの更新処理
		UpdateCredit();
		break;

	case MODE_ENDSTORY:
		UpdateEndStory();		//エンドストーリーの更新処理
		UpdateGOBURIN();		//ゴブリンの更新処理
		break;

	case MODE_ENDROLL:			//エンドロールの更新処理
		UpdateEndRoll();
		break;

	case MODE_GAMEOVER:			//ゲームオーバーの更新処理
		UpdateGameOver();
		break;

	case MODE_RESULT:			//リザルトの更新処理
		UpdateResult();
		break;

	}
	UpdateWindow(WINDOW_UPDATE_SELECT);
	UpdateFade();
	UpdateFadeWindow();
	UpdateAnimation();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{

	PLAYER *pPlayer = GetPlayer();

	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(20, 20, 20, 255), 1.0f, 0);

	// 描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

		D3DVIEWPORT9 ViewportDef[3];
		MODE *Mode;	//現在のモード確認のため

		Mode = GetMode();	//現在のモードをもらう

		switch (Mode[0])
		{
		case MODE_TITLE:					//タイトルの描画処理DrawRanking(void)
			DrawTitle();
			break;

		case MODE_RANKING:					//タイトルの描画処理
			DrawRanking();
			break;

		case MODE_SELCT:
			//ビューポートの取得する
			g_pD3DDevice->GetViewport(&ViewportDef[0]);
			DrawSelectmode();
			// カメラの設定
			SetCamera(4);					//カメラの設定[1]
											// バックバッファ＆Ｚバッファのクリア
			g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//　モデルの描画処理
			DrawGOBURIN();					//ゴブリンの描画処理
											//ビューポートの設定する
			g_pD3DDevice->SetViewport(&ViewportDef[0]);

			if ( pPlayer[0].bButton[BUTTON_S] == true || pPlayer[0].bButton[BUTTON_W] == true)
			{
				g_aSelCounter = 0;
			}
			else
			{
				DrawSelTXIT();				//セレクト文字出力
				g_aSelCounter++;			//セレクトの文字列カウンター++
			}
			DrawSELECTTXIT();

			break;

		case MODE_STORY:
			//ビューポートの取得する
			g_pD3DDevice->GetViewport(&ViewportDef[1]);

			// カメラの設定
			SetCamera(5);					//カメラの設定[2]
											// バックバッファ＆Ｚバッファのクリア
			g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//　モデルの描画処理
			DrawGOBURIN();					//ゴブリンの描画処理
			DrawStory();					//ストーリーの描画処理
											//ビューポートの設定する
			g_pD3DDevice->SetViewport(&ViewportDef[1]);

			if (pPlayer[0].bButton[BUTTON_ENTER] == true && g_aStoryCounter >= 120)
			{//エンター押したら

					g_aStoryCounter = 0;

			}
			else
			{
				DrawStoryTXIT();			//ストーリーの文字列
				g_aStoryCounter++;			//セレクトの文字列カウンター++
			}

			break;

		case MODE_GAME:
			DrawGame();
			break;

		case MODE_CREDIT:					//クレジットの描画処理
			DrawCredit();
			break;

		case MODE_START:
			DrawStart();
			break;

		case MODE_ENDSTORY:
			//ビューポートの取得する
			g_pD3DDevice->GetViewport(&ViewportDef[2]);
			// カメラの設定
			SetCamera(5);					//カメラの設定[2]
			// バックバッファ＆Ｚバッファのクリア
			g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//　モデルの描画処理
			DrawGOBURIN();					//ゴブリンの描画処理
			DrawEndStory();					//エンドストーリーの描画処理

			if ( pPlayer[0].bButton[BUTTON_ENTER] == true && g_aEndStoryCounter >= 120)
			{//エンター押したら

					g_aEndStoryCounter = 0;
			}
			else
			{
				DrawEndStoryTXIT();					//エンドストーリーの文字列
				g_aEndStoryCounter++;			//セレクトの文字列カウンター++
			}
			break;

		case MODE_ENDROLL:						//エンドロールの描画処理
			DrawEndRoll();
			DrawEndrollTXIT();
			break;

		case MODE_RESULT:
			DrawResult();
			break;

		case MODE_GAMEOVER:					//ゲームオーバーの描画処理
			DrawGameOver();
			break;
		}

		// カメラの設定を戻す
		SetCamera(0);

		DrawFont(WINDOW_DRAW_TIME);
		DrawFade();
		DrawFadeWindow();

		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=========================================================================================================//
// デバイスの取得
//=========================================================================================================//
void SetMode(MODE mode, int nFade)
{
	MODE *Mode;	//現在のモード確認のため

	Mode = GetMode();	//現在のモードをもらう


	switch (Mode[0])
	{//現在の処理を終了する
	case MODE_TITLE:
		StopSound();
		UninitTitle();
		break;

	case MODE_RANKING:
		StopSound();
		UninitRanking();
		break;

	case MODE_SELCT:
		StopSound();
		UninitSelectmode();			//セレクトの終了処理
		UninitGOBURIN();			//ゴブリンの終了処理

		break;

	case MODE_GAME:
		UninitGame();
		break;

	case MODE_START:
		UninitStart();
		break;

	case MODE_STORY:
		StopSound();
		UninitStory();				//ストーリーの終了処理
		UninitGOBURIN();			//ゴブリンの終了処理
		break;

	case MODE_CREDIT:				//クレジットの終了処理
		StopSound();
		UninitCredit();
		break;

	case MODE_ENDSTORY:
		StopSound();
		UninitEndStory();			//エンドストーリーの終了処理
		UninitGOBURIN();			//ゴブリンの終了処理
		break;

	case MODE_ENDROLL:				//エンドロールの終了処理
		StopSound();
		UninitEndRoll();
		break;

	case MODE_GAMEOVER:				//ゲームオーバーの終了処理
		StopSound();
		UninitGameOver();
		break;

	case MODE_RESULT:
	//	StopSound();
		UninitResult();
		break;


	}//switch括弧




	switch (mode)
	{//次に行う処理の初期化
	case MODE_TITLE:
		InitTitle();
		break;

	case MODE_RANKING:
		StopSound();
		PlaySound(SOUND_LABEL_CREDIT);
		InitRanking();
		break;
	case MODE_SELCT:

		InitSelectmode();			//セレクトの初期化処理
		InitGOBURIN();				//ゴブリンの初期化処理
		StopSound();
		PlaySound(SOUND_LABEL_SELECT_000);
		g_aSelCounter = 0;			//セレクトの文字列カウンターの初期化処理
		break;

	case MODE_GAME:
		InitGame(nFade);
		break;

	case MODE_START:
		InitStart();
		break;

	case MODE_STORY:
		InitStory();				//ストーリーの初期化処理
		InitGOBURIN();				//ゴブリンの初期化処理
		StopSound();

		g_aStoryCounter = 0;		//ストーリーの文字列カウンターの初期化処理
		PlaySound(SOUND_LABEL_STORY_000);
		break;

	case MODE_CREDIT:				//クレジットの
		StopSound();

		PlaySound(SOUND_LABEL_CREDIT);
		InitCredit();
		break;

	case MODE_ENDSTORY:

		InitEndStory();				//エンドストーリーの初期化処理
		InitGOBURIN();				//ゴブリンの初期化処理
		g_aEndStoryCounter = 0;		//ストーリーの文字列カウンターの初期化処理
		StopSound();

		PlaySound(SOUND_LABEL_ENDSTORY);
		break;

	case MODE_ENDROLL:				//エンドロールの初期化処理

		InitEndRoll();
		StopSound();

		PlaySound(SOUND_LABEL_ENDROLL);
		break;

	case MODE_GAMEOVER:				//ゲームオーバーの初期化処理

		InitGameOver();
		StopSound();

		PlaySound(SOUND_LABEL_GAMEOVER);
		break;

	case MODE_RESULT:
		InitResult();
		break;
	}//switch括弧
	Mode[0] = mode;
}

//=========================================================================================================//
// モードの取得
//=========================================================================================================//
int *GetTime(void)
{
	static int nTime = 0;

	return &nTime;
}
//=========================================================================================================//
// モードの取得
//=========================================================================================================//
int *GetStoryTXT(void)
{
	return &g_aStoryCounter;
}
//=========================================================================================================//
// モードの取得
//=========================================================================================================//
int *GetENDStoryTXT(void)
{
	return &g_aEndStoryCounter;
}

//=========================================================================================================//
// モードの取得
//=========================================================================================================//
MODE *GetMode(void)
{
	static MODE Mode = MODE_TITLE;

	return &Mode;
}
//=========================================================================================================//
// windowの取得
//=========================================================================================================//
HWND *GethWnd(void)
{
	static HWND hWnd;

	return &hWnd;
}
//==========================================================================================================================================================
// デバイスの取得
//==========================================================================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//==========================================================================================================================================================
// デバイスの取得
//==========================================================================================================================================================
GAME_PLAY *GetGame_Play(void)
{
	static GAME_PLAY g_Game_Play;

	return &g_Game_Play;
}
//==========================================================================================================================================================
// セレクト画面の文字
//==========================================================================================================================================================
void DrawSelTXIT(void)
{
	RECT rect = { 220, 510, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
	char aTX[256];
	g_aSelectMenuMain = GetSelMenu();

	// 文字列を代入//一文字につきスペース3つ
	if (g_aSelectMenuMain == 0)
	{
		if (g_aSelCounter >= 20) { wsprintf(&aTX[0], "こ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 25) { wsprintf(&aTX[0], "   れ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 30) { wsprintf(&aTX[0], "      は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 35) { wsprintf(&aTX[0], "         ス\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 100, 100)/*色*/); }
		if (g_aSelCounter >= 40) { wsprintf(&aTX[0], "            トー\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 100, 100)/*色*/); }
		if (g_aSelCounter >= 45) { wsprintf(&aTX[0], "                 リー\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 100, 100)/*色*/); }
		if (g_aSelCounter >= 50) { wsprintf(&aTX[0], "                       モー\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 100, 100)/*色*/); }
		if (g_aSelCounter >= 55) { wsprintf(&aTX[0], "                             ド\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 100, 100)/*色*/); }
		if (g_aSelCounter >= 60) { wsprintf(&aTX[0], "                               ゴ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 65) { wsprintf(&aTX[0], "                                  ブッ。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 70) { wsprintf(&aTX[0], "                                   \nゲ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 75) { wsprintf(&aTX[0], "                                  　 \n   -ム", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 80) { wsprintf(&aTX[0], "                                  　 \n        内容", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 85) { wsprintf(&aTX[0], "                                  　 \n              を", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 90) { wsprintf(&aTX[0], "                                  　 \n       　　      説", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 95) { wsprintf(&aTX[0], "                                  　 \n       　　         明", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 100) { wsprintf(&aTX[0], "                                   　\n       　　            して", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 105) { wsprintf(&aTX[0], "                                  　 \n       　　                  いく", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 110) { wsprintf(&aTX[0], "                                  　 \n       　　                       ゴブッ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 115) { wsprintf(&aTX[0], "                                  　 \n       　　                       　　　　!!", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	else if (g_aSelectMenuMain == 1)
	{
		if (g_aSelCounter >= 20) { wsprintf(&aTX[0], "こ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 25) { wsprintf(&aTX[0], "   れ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 30) { wsprintf(&aTX[0], "      は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 35) { wsprintf(&aTX[0], "         ゲ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(200, 100, 200)/*色*/); }
		if (g_aSelCounter >= 40) { wsprintf(&aTX[0], "            ーム\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 100, 200)/*色*/); }
		if (g_aSelCounter >= 45) { wsprintf(&aTX[0], "                  モー\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 100, 200)/*色*/); }
		if (g_aSelCounter >= 50) { wsprintf(&aTX[0], "                       ド\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 100, 200)/*色*/); }
		if (g_aSelCounter >= 55) { wsprintf(&aTX[0], "                          ゴ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 60) { wsprintf(&aTX[0], "                             ブッ。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 65) { wsprintf(&aTX[0], " \n　　　　スト", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 70) { wsprintf(&aTX[0], " \n    　　　　 ーリ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 85) { wsprintf(&aTX[0], " \n      　　　　     ーは", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 90) { wsprintf(&aTX[0], " \n       　　　　          見", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 95) { wsprintf(&aTX[0], " \n       　　　　             た", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 100) { wsprintf(&aTX[0], " \n        　　　　               ゴ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 105) { wsprintf(&aTX[0], " \n         　　　　                 ブッ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 110) { wsprintf(&aTX[0], " \n         　　　　                      ？？", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	else if (g_aSelectMenuMain == 2)
	{
		if (g_aSelCounter >= 20) { wsprintf(&aTX[0], "こ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 25) { wsprintf(&aTX[0], "   れ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 30) { wsprintf(&aTX[0], "      は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 35) { wsprintf(&aTX[0], "         制\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 40) { wsprintf(&aTX[0], "            作\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 45) { wsprintf(&aTX[0], "               ス\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 50) { wsprintf(&aTX[0], "                  タ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 55) { wsprintf(&aTX[0], "                     ッ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 60) { wsprintf(&aTX[0], "                        フ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 65) { wsprintf(&aTX[0], "                           や\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 70) { wsprintf(&aTX[0], "                              \n著", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 75) { wsprintf(&aTX[0], "                              \n   作", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 80) { wsprintf(&aTX[0], "                              \n      物", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 85) { wsprintf(&aTX[0], "                              \n         の", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 90) { wsprintf(&aTX[0], "                              \n            詳", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 95) { wsprintf(&aTX[0], "                              \n               細", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 100) { wsprintf(&aTX[0], "                              \n                  が", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 105) { wsprintf(&aTX[0], "                              \n                     載", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 110) { wsprintf(&aTX[0], "                              \n                         って", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 115) { wsprintf(&aTX[0], "                              \n                              い", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 120) { wsprintf(&aTX[0], "                              \n                                 る", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 125) { wsprintf(&aTX[0], "                              \n                                    ゴブッ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	else if (g_aSelectMenuMain == 3)
	{
		if (g_aSelCounter >= 20) { wsprintf(&aTX[0], "こ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 25) { wsprintf(&aTX[0], "   れ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 30) { wsprintf(&aTX[0], "      は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 35) { wsprintf(&aTX[0], "         ゲ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 40) { wsprintf(&aTX[0], "            ーム\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 45) { wsprintf(&aTX[0], "                  を\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 50) { wsprintf(&aTX[0], "                     終\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 55) { wsprintf(&aTX[0], "                        了\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 60) { wsprintf(&aTX[0], "                           し\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 65) { wsprintf(&aTX[0], "                              ま\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aSelCounter >= 70) { wsprintf(&aTX[0], "                                 す\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
	}
}
//==========================================================================================================================================================
// ストーリー画面の文字
//==========================================================================================================================================================
void DrawStoryTXIT(void)
{
	RECT rect = { 220, 520, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
	char aTX[256];
	g_aStoryMenuMain = GetStoryMenu();


	if (g_aStoryMenuMain == 0)
	{
		RECT rect = { SCREEN_WIDTH / 2 - 380, SCREEN_HEIGHT / 2 - 100, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
																									 // 文字列を代入
		wsprintf(&aTX[0], "それは人間界と魔界の激闘の争いから始まった...\n", aTX);
		// 描画
		g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/);
	}
	if (g_aStoryMenuMain == 1)
	{
		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 210) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
	}
	if (g_aStoryMenuMain == 2)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "や\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   あ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         僕\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               ゴ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  ブ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                     リ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                        ン\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                           、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                              、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                 、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	if (g_aStoryMenuMain == 3)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "こ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      世\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         界\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               昔\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  話\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                     を\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                        君\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                           に\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                              聞\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                 か\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 70) { wsprintf(&aTX[0], "                                    せ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 75) { wsprintf(&aTX[0], "                                       て\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 80) { wsprintf(&aTX[0], "                                          あ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 85) { wsprintf(&aTX[0], "                                             げ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 90) { wsprintf(&aTX[0], "                                                る\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 95) { wsprintf(&aTX[0], "                                                   ゴ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "                                                      ブ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 105) { wsprintf(&aTX[0], "                                                        ！！\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aStoryMenuMain == 4)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "こ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      世\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         界\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            に\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  昔\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                     人\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                        間\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                           界\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                              と\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                 魔\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 70) { wsprintf(&aTX[0], "                                    界\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 75) { wsprintf(&aTX[0], "                                       が\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 80) { wsprintf(&aTX[0], "                                          あ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 85) { wsprintf(&aTX[0], "                                             っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 90) { wsprintf(&aTX[0], "                                                た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 95) { wsprintf(&aTX[0], "                                                   ん\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "                                                      だ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 105) { wsprintf(&aTX[0], "                                                         ！！\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	if (g_aStoryMenuMain == 5)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "二\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   つ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         世\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            界\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  別\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                     々\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                        に\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                           く\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                              ら\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                 し\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 70) { wsprintf(&aTX[0], "                                    て\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 75) { wsprintf(&aTX[0], "                                       い\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 80) { wsprintf(&aTX[0], "                                          た。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	if (g_aStoryMenuMain == 6)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "だ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   が\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      あ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         る\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            日\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                     、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	if (g_aStoryMenuMain == 7)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "二\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "    つ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "       の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "          世\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "             界\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "                が\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                   争\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                      い\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                         を\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                            始\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                               め\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                  て\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 70) { wsprintf(&aTX[0], "                                     し\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 75) { wsprintf(&aTX[0], "                                        ま\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 80) { wsprintf(&aTX[0], "                                           う\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 85) { wsprintf(&aTX[0], "                                              。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 90) { wsprintf(&aTX[0], "\nつ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 95) { wsprintf(&aTX[0], "\n   い", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "\n      に", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 105) { wsprintf(&aTX[0], "\n         は", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 110) { wsprintf(&aTX[0], "\n              ...", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	if (g_aStoryMenuMain == 8)
	{

		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "魔\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   界\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      が\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         滅\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            ん\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               で\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  し\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                     ま\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                        っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                           た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                              。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "\n人", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 70) { wsprintf(&aTX[0], "\n   間", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 75) { wsprintf(&aTX[0], "\n      界", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 80) { wsprintf(&aTX[0], "\n         の", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 85) { wsprintf(&aTX[0], "\n            象", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 90) { wsprintf(&aTX[0], "\n               徴", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 95) { wsprintf(&aTX[0], "\n                  「剣", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 100, 100)/*色*/); }
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "\n                       王」", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(55, 50, 198)/*色*/); }
		if (g_aStoryCounter >= 105) { wsprintf(&aTX[0], "\n                            に", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 110) { wsprintf(&aTX[0], "\n                               よ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 115) { wsprintf(&aTX[0], "\n                                  っ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 120) { wsprintf(&aTX[0], "\n                                     て。", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	if (g_aStoryMenuMain == 9)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "で\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   も\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      唯\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         一\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            生\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               き\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  残\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                     っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                        た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                           魔\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                              物\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                 が\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 70) { wsprintf(&aTX[0], "                                    い\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 75) { wsprintf(&aTX[0], "                                       た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 80) { wsprintf(&aTX[0], "                                          ん\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 85) { wsprintf(&aTX[0], "                                             だ。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	if (g_aStoryMenuMain == 10)
	{

		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "そ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      名\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            「\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "              魔\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                 王\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                    の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                       子\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                          」\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                             で\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                凶\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aStoryCounter >= 70) { wsprintf(&aTX[0], "                                   悪\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aStoryCounter >= 75) { wsprintf(&aTX[0], "                                      と\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 80) { wsprintf(&aTX[0], "                                         称\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 85) { wsprintf(&aTX[0], "                                            さ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 90) { wsprintf(&aTX[0], "                                               れ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 95) { wsprintf(&aTX[0], "                                                  た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "                                                     ...\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
	}
	if (g_aStoryMenuMain == 11)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "そ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      名\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            べ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               ル\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  セ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                     レ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                        ー\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                           ジュ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                                ！\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                   ！\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aStoryMenuMain == 12)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "復\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   讐\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      す\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         る\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            べ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "                く\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                   人\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                      間\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                         界\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                            の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                               象\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                  徴\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 70) { wsprintf(&aTX[0], "                                     で\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 75) { wsprintf(&aTX[0], "                                        あ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 80) { wsprintf(&aTX[0], "                                           る。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 85) { wsprintf(&aTX[0], "\n剣", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aStoryCounter >= 90) { wsprintf(&aTX[0], "\n   王", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aStoryCounter >= 95) { wsprintf(&aTX[0], "\n      を", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "\n         倒", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 105) { wsprintf(&aTX[0], "\n            し", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 110) { wsprintf(&aTX[0], "\n               に", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 115) { wsprintf(&aTX[0], "\n                  向", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 120) { wsprintf(&aTX[0], "\n                     か", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 125) { wsprintf(&aTX[0], "\n                        っ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 130) { wsprintf(&aTX[0], "\n                           た", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 135) { wsprintf(&aTX[0], "\n                              の", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 140) { wsprintf(&aTX[0], "\n                                 だ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 145) { wsprintf(&aTX[0], "\n                                    が", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 150) { wsprintf(&aTX[0], "\n                                       ...", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	if (g_aStoryMenuMain == 13)
	{

		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "あ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   ぁ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         な\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            ん\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               だ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  か\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                     眠\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                        く\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                           な\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                              っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                 て\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 70) { wsprintf(&aTX[0], "                                    き\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 75) { wsprintf(&aTX[0], "                                       た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 80) { wsprintf(&aTX[0], "                                          ゴ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 85) { wsprintf(&aTX[0], "                                             ブ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 90) { wsprintf(&aTX[0], "                                               ...\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
	}
	if (g_aStoryMenuMain == 14)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "続\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   き\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         ゲ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 255, 150)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            ー\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 255, 150)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               ム\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 255, 150)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  モ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 255, 150)/*色*/); }
		if (g_aStoryCounter >= 45) { wsprintf(&aTX[0], "                     ー\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 255, 150)/*色*/); }
		if (g_aStoryCounter >= 50) { wsprintf(&aTX[0], "                        ド\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 255, 150)/*色*/); }
		if (g_aStoryCounter >= 55) { wsprintf(&aTX[0], "                           を\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 60) { wsprintf(&aTX[0], "                              進\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 65) { wsprintf(&aTX[0], "                                 め\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 70) { wsprintf(&aTX[0], "                                    て\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 75) { wsprintf(&aTX[0], "                                       い\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 80) { wsprintf(&aTX[0], "                                          け\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 85) { wsprintf(&aTX[0], "                                             ば\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 90) { wsprintf(&aTX[0], "                                                分\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 95) { wsprintf(&aTX[0], "                                                   か\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "                                                      る\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 105) { wsprintf(&aTX[0], "                                                         ゴ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 110) { wsprintf(&aTX[0], "                                                            ブ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 115) { wsprintf(&aTX[0], "                                                               ...\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
	}
	if (g_aStoryMenuMain == 15)
	{
		if (g_aStoryCounter >= 10) { wsprintf(&aTX[0], "そ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 15) { wsprintf(&aTX[0], "   、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 20) { wsprintf(&aTX[0], "      そ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 25) { wsprintf(&aTX[0], "         れ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 30) { wsprintf(&aTX[0], "            で\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 150)/*色*/); }
		if (g_aStoryCounter >= 35) { wsprintf(&aTX[0], "               は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aStoryCounter >= 40) { wsprintf(&aTX[0], "                  ........\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
	}
	if (g_aStoryMenuMain == 16)
	{
		// 文字列を代入
		wsprintf(&aTX[0], "......................\n", aTX);
		// 描画
		g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/);
	}
	if (g_aStoryMenuMain != 0)
	{
		rect = { 0, 670, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		wsprintf(&aTX[0], "START/P:SKIP\n", aTX);
		g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(80, 80, 80)/*色*/);
	}

		 rect = { 0, 470, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		//char aTX[256];

		wsprintf(&aTX[0], "[B]BUTTON/ENTRE：NEXT \n", aTX);
		// 描画
		g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(80, 80, 80)/*色*/);



}
//==========================================================================================================================================================
// エンドストーリー画面の文字
//==========================================================================================================================================================
void DrawEndStoryTXIT(void)
{
	RECT rect = { 220, 520, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
	char aTX[256];
	g_aEndStoryMenuMain = GetEndStoryMenu();

	if (g_aEndStoryMenuMain == 0)
	{
		RECT rect = { SCREEN_WIDTH / 2 - 230 , SCREEN_HEIGHT / 2 - 100, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
																									  // 文字列を代入
		wsprintf(&aTX[0], "「剣王」を倒してからその後...\n", aTX);
		// 描画
		g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/);
	}
	if (g_aEndStoryMenuMain == 1)
	{
		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 210) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
	}
	if (g_aEndStoryMenuMain == 2)
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "や\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   あ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      久\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         し\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "            ぶ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "               り\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "                  だ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                     ね\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                        ...\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
	}
	if (g_aEndStoryMenuMain == 3)
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "そ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      後\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "            お\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "               話\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                  を\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                     す\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 50) { wsprintf(&aTX[0], "                        る\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 55) { wsprintf(&aTX[0], "                           ね。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aEndStoryMenuMain == 4)
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "結\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   末\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      か\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         ら\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "            言\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "               う\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                  と\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                     人\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 50) { wsprintf(&aTX[0], "                        間\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 55) { wsprintf(&aTX[0], "                           界\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 60) { wsprintf(&aTX[0], "                              も\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 65) { wsprintf(&aTX[0], "                                 滅\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 50,50)/*色*/); }
		if (g_aEndStoryCounter >= 70) { wsprintf(&aTX[0], "                                    ん\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 75) { wsprintf(&aTX[0], "                                       で\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 80) { wsprintf(&aTX[0], "                                          し\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 85) { wsprintf(&aTX[0], "                                             ま\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 90) { wsprintf(&aTX[0], "                                                っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 95) { wsprintf(&aTX[0], "                                                   た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "                                                      ん\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 105) { wsprintf(&aTX[0], "                                                         だ！！\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

	}
	if (g_aEndStoryMenuMain == 5)
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "「剣\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "     王」\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 150, 150)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "          が\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "             べ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "                ル\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "                   セ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                      レ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                         ー\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 50) { wsprintf(&aTX[0], "                            ジ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 55) { wsprintf(&aTX[0], "                               ュ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 60) { wsprintf(&aTX[0], "                                  に\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 65) { wsprintf(&aTX[0], "                                     よ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 70) { wsprintf(&aTX[0], "                                        っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 75) { wsprintf(&aTX[0], "                                           て\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 80) { wsprintf(&aTX[0], "                                              倒\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 85) { wsprintf(&aTX[0], "                                                 さ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 90) { wsprintf(&aTX[0], "                                                    れ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 95) { wsprintf(&aTX[0], "                                                       て\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "                                                          か\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 105) { wsprintf(&aTX[0], "                                                             ら...\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aEndStoryMenuMain == 6)
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "市\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   民\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      同\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         士\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "            が\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "               争\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                  い\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                     合\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 50) { wsprintf(&aTX[0], "                        い。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 55) { wsprintf(&aTX[0], "\nべ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 60) { wsprintf(&aTX[0], "\n   ル", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 65) { wsprintf(&aTX[0], "\n      セ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 70) { wsprintf(&aTX[0], "\n         レ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 75) { wsprintf(&aTX[0], "\n            ー", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 80) { wsprintf(&aTX[0], "\n               ジ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 85) { wsprintf(&aTX[0], "\n                  ュ", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 90) { wsprintf(&aTX[0], "\n                     も", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 95) { wsprintf(&aTX[0], "\n                        人", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "\n                           間", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 105) { wsprintf(&aTX[0], "\n                              界", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 110) { wsprintf(&aTX[0], "\n                                 を", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 115) { wsprintf(&aTX[0], "\n                                    破", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 80, 80)/*色*/); }
		if (g_aEndStoryCounter >= 120) { wsprintf(&aTX[0], "\n                                       壊", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 80, 80)/*色*/); }
		if (g_aEndStoryCounter >= 125) { wsprintf(&aTX[0], "\n                                          し", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 130) { wsprintf(&aTX[0], "\n                                            て", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 135) { wsprintf(&aTX[0], "\n                                               い", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 135) { wsprintf(&aTX[0], "\n                                                  き。", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aEndStoryMenuMain == 7)
	{

		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "人\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   々\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         誰\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "            も\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "               い\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                  な\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                     く\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 50) { wsprintf(&aTX[0], "                       な\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 55) { wsprintf(&aTX[0], "                          っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 60) { wsprintf(&aTX[0], "                             た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 65) { wsprintf(&aTX[0], "                                ...\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aEndStoryMenuMain == 8)
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "そ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   し\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      て\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         べ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "            ル\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "               セ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                  レ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                     ー\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 50) { wsprintf(&aTX[0], "                        ジ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 55) { wsprintf(&aTX[0], "                           ュ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(150, 150, 255)/*色*/); }
		if (g_aEndStoryCounter >= 60) { wsprintf(&aTX[0], "                              も\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 65) { wsprintf(&aTX[0], "                                 こ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 70) { wsprintf(&aTX[0], "                                    の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 75) { wsprintf(&aTX[0], "                                       世\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 80) { wsprintf(&aTX[0], "                                          か\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 85) { wsprintf(&aTX[0], "                                             ら\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 90) { wsprintf(&aTX[0], "                                                .....\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aEndStoryMenuMain == 9)//ストーリーモード
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "こ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      物\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         語\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "            は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "               長\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                  ら\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                     く\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 50) { wsprintf(&aTX[0], "                        語\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 55) { wsprintf(&aTX[0], "                           ら\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 60) { wsprintf(&aTX[0], "                              れ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 65) { wsprintf(&aTX[0], "                                 て\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 70) { wsprintf(&aTX[0], "                                    い\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 75) { wsprintf(&aTX[0], "                                       る\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 80) { wsprintf(&aTX[0], "                                          お\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 85) { wsprintf(&aTX[0], "                                             話\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 90) { wsprintf(&aTX[0], "                                                な\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 95) { wsprintf(&aTX[0], "                                                   ん\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "                                                      だ。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aEndStoryMenuMain == 10)
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "え\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      お\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         い\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "            ら\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "               は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                  誰\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                     か\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 50) { wsprintf(&aTX[0], "                        っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 55) { wsprintf(&aTX[0], "                           て\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 60) { wsprintf(&aTX[0], "                              そ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 65) { wsprintf(&aTX[0], "                                 れ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 70) { wsprintf(&aTX[0], "                                    は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 75) { wsprintf(&aTX[0], "                                       ...\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aEndStoryMenuMain == 11)
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "新\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      に\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         で\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "            き\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "               た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                  ゴ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(88, 166, 31)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                     ブ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(88, 166, 31)/*色*/); }
		if (g_aEndStoryCounter >= 50) { wsprintf(&aTX[0], "                        リ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(88, 166, 31)/*色*/); }
		if (g_aEndStoryCounter >= 55) { wsprintf(&aTX[0], "                           ン\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(88, 166, 31)/*色*/); }
		if (g_aEndStoryCounter >= 60) { wsprintf(&aTX[0], "                              界\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(88, 166, 31)/*色*/); }
		if (g_aEndStoryCounter >= 65) { wsprintf(&aTX[0], "                                 の\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 70) { wsprintf(&aTX[0], "                                    王\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 75) { wsprintf(&aTX[0], "                                       様\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 80) { wsprintf(&aTX[0], "                                          さ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 80) { wsprintf(&aTX[0], "                                             !!!!\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }



	}
	if (g_aEndStoryMenuMain == 12)
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "わ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "            は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "               っ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                  は。\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aEndStoryMenuMain == 13)
	{
		if (g_aEndStoryCounter >= 10) { wsprintf(&aTX[0], "ま\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 15) { wsprintf(&aTX[0], "   あ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 20) { wsprintf(&aTX[0], "      、\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 25) { wsprintf(&aTX[0], "         ま\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 30) { wsprintf(&aTX[0], "            た\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 35) { wsprintf(&aTX[0], "               ど\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 40) { wsprintf(&aTX[0], "                  こ\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 45) { wsprintf(&aTX[0], "                     か\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 50) { wsprintf(&aTX[0], "                        で\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 55) { wsprintf(&aTX[0], "                           会\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 60) { wsprintf(&aTX[0], "                              お\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 65) { wsprintf(&aTX[0], "                                 う\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 70) { wsprintf(&aTX[0], "                                   で\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 75) { wsprintf(&aTX[0], "                     　　　　　　    は\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 80) { wsprintf(&aTX[0], "                      　　　　          な\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 85) { wsprintf(&aTX[0], "                                           い\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 90) { wsprintf(&aTX[0], "                                              か\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }
		if (g_aEndStoryCounter >= 95) { wsprintf(&aTX[0], "                                                  ...\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }

		RECT rect = { 950, 650, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
		if (g_aEndStoryCounter >= 100) { wsprintf(&aTX[0], "NEXT\n", aTX); g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/); }


	}
	if (g_aEndStoryMenuMain == 14)
	{
		// 文字列を代入
		wsprintf(&aTX[0], "......................\n", aTX);
		// 描画
		g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255)/*色*/);
	}

		rect = { 0, 470, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
													   //char aTX[256];

		wsprintf(&aTX[0], "[B]BUTTON/ENTRE：NEXT \n", aTX);
		// 描画
		g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(80, 80, 80)/*色*/);

}
//=============================================================================
// エンドロール画面の文字
//=============================================================================
void DrawEndrollTXIT(void)
{
	RECT rect = { 0, 680, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
	char aTX[256];

	wsprintf(&aTX[0], "START/P・・・早送り\n", aTX);
	// 描画
	g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(80, 80, 80)/*色*/);

}
//=============================================================================
// セレクト画面の文字
//=============================================================================
void DrawSELECTTXIT(void)
{
	RECT rect = { 240, 660, SCREEN_WIDTH, SCREEN_HEIGHT };// 左上のx座標// 左上のy座標// 右下のx座標// 右下のy座標
	char aTX[256];

	wsprintf(&aTX[0], "[ 十字キー：選択 :「B」BUTTON：決定 ]\n", aTX);
	// 描画
	g_pFont->DrawText(NULL, &aTX[0]/*文字列へのポインタ*/, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(80, 80, 80)/*色*/);

}
