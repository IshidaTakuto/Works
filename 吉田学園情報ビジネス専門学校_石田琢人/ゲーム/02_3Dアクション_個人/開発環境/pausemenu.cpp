//=============================================================================
//
// ポーズメニュー処理 [menu.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "pausemenu.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PMENU_TEXTURENAME1	"data/TEXTURE/pause000.png"	// ポーズのテクスチャ名
#define PMENU_TEXTURENAME2	"data/TEXTURE/pause001.png"	// ポーズのテクスチャ名
#define PMENU_TEXTURENAME3	"data/TEXTURE/pause002.png"	// ポーズのテクスチャ名
#define PMENU_SIZE_X	(200.0f)		// メニューの大きさ(X)
#define PMENU_SIZE_Y	(45.0f)			// メニューの大きさ(Y)
#define PMENU_FLASHING	(10)			// メニューの点滅

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePmenu[MAX_PAUSEMENU] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPmenu = NULL;				// 頂点バッファへのポインタ
PAUSEMENU g_Pmenu[MAX_PAUSEMENU];
int g_nNumSelect;		// 現在の状態

//=============================================================================
// 初期化処理
//=============================================================================
void InitPmenu(void)
{
	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 情報の初期化
	g_Pmenu[CONTINUE].pos = D3DXVECTOR3(640, 250, 0.0f);	
	g_Pmenu[RETRY].pos = D3DXVECTOR3(640, 350, 0.0f);
	g_Pmenu[QUIT].pos = D3DXVECTOR3(640, 450, 0.0f);

	g_Pmenu[CONTINUE].nType = 0;
	g_Pmenu[RETRY].nType = 1;
	g_Pmenu[QUIT].nType = 2;

	g_nNumSelect = CONTINUE;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PMENU_TEXTURENAME1, &g_pTexturePmenu[CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, PMENU_TEXTURENAME2, &g_pTexturePmenu[RETRY]);
	D3DXCreateTextureFromFile(pDevice, PMENU_TEXTURENAME3, &g_pTexturePmenu[QUIT]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSEMENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPmenu,
		NULL);

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPmenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < MAX_PAUSEMENU; nCntMenu++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_Pmenu[nCntMenu].pos.x - PMENU_SIZE_X, g_Pmenu[nCntMenu].pos.y - PMENU_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Pmenu[nCntMenu].pos.x + PMENU_SIZE_X, g_Pmenu[nCntMenu].pos.y - PMENU_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Pmenu[nCntMenu].pos.x - PMENU_SIZE_X, g_Pmenu[nCntMenu].pos.y + PMENU_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Pmenu[nCntMenu].pos.x + PMENU_SIZE_X, g_Pmenu[nCntMenu].pos.y + PMENU_SIZE_Y, 0.0f);

		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffPmenu->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPmenu(void)
{
	int nCntMenu;

	for (nCntMenu = 0; nCntMenu < MAX_PAUSEMENU; nCntMenu++)
	{
		// テクスチャの破棄
		if (g_pTexturePmenu[nCntMenu] != NULL)
		{
			g_pTexturePmenu[nCntMenu]->Release();
			g_pTexturePmenu[nCntMenu] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPmenu != NULL)
	{
		g_pVtxBuffPmenu->Release();
		g_pVtxBuffPmenu = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePmenu(void)
{
	SelectPmenu();

	PAUSECHECK *pPause;
	pPause = GetPauseMenu();

	FADE pFade;
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_P) == true)
		{
			g_nNumSelect = CONTINUE;
		}

		if (GetKeyboardRepeat(DIK_W) == true || GetKeyboardRepeat(DIK_UP) == true)
		{
			PlaySound(SOUND_LABEL_BOTTON001);
			g_nNumSelect = (g_nNumSelect + (MAX_PAUSEMENU - 1)) % MAX_PAUSEMENU;	// 選択状態を１つ上げる
			SelectPmenu();
		}

		if (GetKeyboardRepeat(DIK_S) == true || GetKeyboardRepeat(DIK_DOWN) == true)
		{
			PlaySound(SOUND_LABEL_BOTTON001);
			g_nNumSelect = (g_nNumSelect + 1) % MAX_PAUSEMENU;						// 選択状態を１つ下げる
			SelectPmenu();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPmenu(void)
{
	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPmenu, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntMenu = 0; nCntMenu < MAX_PAUSEMENU; nCntMenu++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePmenu[g_Pmenu[nCntMenu].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMenu, 2);
	}
}

//=============================================================================
// 選択処理
//=============================================================================
void SelectPmenu(void)
{
	PAUSECHECK *pPause;
	pPause = GetPauseMenu();

	FADE pFade;
	pFade = *GetFade();

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

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
					PlaySound(SOUND_LABEL_DECIDE);
					SetFade(MODE_GAME);
				}
			}
			break;

		case QUIT:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_DECIDE);
					SetFade(MODE_TITLE);
				}
			}
			break;
		}
	}

	if (g_nNumSelect == CONTINUE)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffPmenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (CONTINUE * 4);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffPmenu->Unlock();
	}
	else if (g_nNumSelect != CONTINUE)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffPmenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (CONTINUE * 4);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffPmenu->Unlock();
	}

	if (g_nNumSelect == RETRY)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffPmenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (RETRY * 4);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffPmenu->Unlock();
	}
	else if (g_nNumSelect != RETRY)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffPmenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (RETRY * 4);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffPmenu->Unlock();
	}

	if (g_nNumSelect == QUIT)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffPmenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (QUIT * 4);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffPmenu->Unlock();
	}
	else if (g_nNumSelect != QUIT)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffPmenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (QUIT * 4);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffPmenu->Unlock();
	}
}

