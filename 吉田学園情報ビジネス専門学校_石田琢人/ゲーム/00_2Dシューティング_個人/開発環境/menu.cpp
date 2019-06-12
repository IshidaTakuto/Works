//=============================================================================
//
// ポーズメニュー処理 [menu.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "menu.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MENU_TEXTURENAME1	"data/TEXTURE/pause000.png"	//ポーズのテクスチャ名
#define MENU_TEXTURENAME2	"data/TEXTURE/pause001.png"	//ポーズのテクスチャ名
#define MENU_TEXTURENAME3	"data/TEXTURE/pause002.png"	//ポーズのテクスチャ名
#define MAX_SELECTMENU	(3)
#define MENU_SIZE_X	(200.0f)		//メニューの大きさ(X)
#define MENU_SIZE_Y	(45.0f)			//メニューの大きさ(Y)
#define MENU_FLASHING	(10)		//メニューの点滅

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMenu[MAX_MENU] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenu = NULL;	//頂点バッファへのポインタ
MENU g_Menu[MAX_MENU];
int nSelect;	//現在の状態
int g_nNumSelect;

//=============================================================================
// 初期化処理
//=============================================================================
void InitMenu(void)
{
	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//情報の初期化
	g_Menu[CONTINUE].pos = D3DXVECTOR3(640, 250, 0.0f);	
	g_Menu[RETRY].pos = D3DXVECTOR3(640, 350, 0.0f);
	g_Menu[QUIT].pos = D3DXVECTOR3(640, 450, 0.0f);

	g_Menu[CONTINUE].nType = 0;
	g_Menu[RETRY].nType = 1;
	g_Menu[QUIT].nType = 2;

	g_nNumSelect = CONTINUE;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME1, &g_pTextureMenu[CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME2, &g_pTextureMenu[RETRY]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME3, &g_pTextureMenu[QUIT]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenu,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < MAX_MENU; nCntMenu++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x - MENU_SIZE_X, g_Menu[nCntMenu].pos.y - MENU_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x + MENU_SIZE_X, g_Menu[nCntMenu].pos.y - MENU_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x - MENU_SIZE_X, g_Menu[nCntMenu].pos.y + MENU_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x + MENU_SIZE_X, g_Menu[nCntMenu].pos.y + MENU_SIZE_Y, 0.0f);

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffMenu->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMenu(void)
{
	int nCntMenu;

	for (nCntMenu = 0; nCntMenu < MAX_MENU; nCntMenu++)
	{
		//テクスチャの破棄
		if (g_pTextureMenu[nCntMenu] != NULL)
		{
			g_pTextureMenu[nCntMenu]->Release();
			g_pTextureMenu[nCntMenu] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMenu != NULL)
	{
		g_pVtxBuffMenu->Release();
		g_pVtxBuffMenu = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMenu(void)
{
	SelectMenu();

	if (GetKeyboardRepeat(DIK_UP) == true)
	{
		g_nNumSelect = (g_nNumSelect + (MAX_MENU - 1)) % MAX_MENU;	//選択状態を１つ上げる
		SelectMenu();
	}

	if (GetKeyboardRepeat(DIK_DOWN) == true)
	{
		g_nNumSelect = (g_nNumSelect + 1) % MAX_MENU;				//選択状態を１つ下げる
		SelectMenu();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMenu(void)
{
	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntMenu = 0; nCntMenu < MAX_MENU; nCntMenu++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMenu[g_Menu[nCntMenu].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMenu, 2);
	}
}

//=============================================================================
// 選択処理
//=============================================================================
void SelectMenu(void)
{
	PAUSECHECK *pPause;
	pPause = GetPauseMenu();

	FADE pFade;
	pFade = *GetFade();

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_nNumSelect)
		{
		case CONTINUE:
			pPause->bPause = false;
			break;

		case RETRY:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{

					SetFade(MODE_GAME);
				}
			}
			break;

		case QUIT:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					SetFade(MODE_TITLE);
				}
			}
			break;
		}
	}

	if (g_nNumSelect == CONTINUE)
	{
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (CONTINUE * 4);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelect != CONTINUE)
	{
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (CONTINUE * 4);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}

	if (g_nNumSelect == RETRY)
	{
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (RETRY * 4);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelect != RETRY)
	{
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (RETRY * 4);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}

	if (g_nNumSelect == QUIT)
	{
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (QUIT * 4);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelect != QUIT)
	{
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (QUIT * 4);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}
}

