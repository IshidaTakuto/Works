//=============================================================================
//
// メイン処理 [main.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "debugProc.h"
#include "resource.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"landscape"			// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HICON hIcon;
	hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	HICON hIconSml;
	hIconSml = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEX構造体のサイズ
		CS_CLASSDC,						// ウインドウスタイル
		WndProc,						// ウィンドウプロシージャのアドレス
		0,								// 0固定
		0,								// 0固定
		hInstance,						// WinMain()のインスタンスハンドル
		hIcon,							// 使用するアイコン
		LoadCursor(NULL, IDC_ARROW),	// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		// ウインドウのクライアント領域の背景色
		"MENUID",						// メニュー
		CLASS_NAME,						// Windowクラスの名前
		hIconSml						// 使用する小さいアイコン
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
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

	CManager *pManager = NULL;

	pManager = new CManager;

	if (NULL != pManager)
	{
		// 初期化処理
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))	//FALSEでフルスクリーン
		{
			return -1;
		}
	}
	else
	{
		MessageBox(0, "マネージャークラスのメモリが確保できませんでした", "失敗", MB_OK);
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwCurrentTime =
	dwFrameCount = 0;
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// FPS数
	//static int nCntFPS = 0;

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
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();	// 現在の時間を取得
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒ごとに実行
				//nCntFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

				if (NULL != pManager)
				{
					// FPSを算出
					//CDebugProc::Print("FPS　:　%d\n", nCntFPS);

					// マネージャーの更新処理
					pManager->Update();

					// マネージャーの描画処理
					pManager->Draw();
				}
				else
				{
					MessageBox(0, "メモリが確保できていません", "失敗", MB_OK);
				}

				dwFrameCount++;
			}
		}
	}

	if (pManager != NULL)
	{// マネージャーの破棄
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch(uMsg)
	{
	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDNO)
		{// ウインドウを破棄しない
			uMsg = WA_ACTIVE;
		}
		break;

	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case MENU_OPEN:		// 開く
			break;

		case MENU_CLOSE:	// 閉じる
			PostQuitMessage(0);
			break;

		case MENU_SETTING:	// 設定
			break;

		case MENU_OPTION:	// オプション
			break;
		}
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{// ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
