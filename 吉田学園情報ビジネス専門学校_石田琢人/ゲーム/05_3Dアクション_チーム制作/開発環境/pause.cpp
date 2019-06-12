//=============================================================================
//
// ポーズメニュー処理 [menu.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "pause.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PMENU_TEXTURENAME1	"data/TEXTURE/pause_resume.png"	// ポーズのテクスチャ名
#define PMENU_TEXTURENAME2	"data/TEXTURE/pause_retry.png"	// ポーズのテクスチャ名
#define PMENU_TEXTURENAME3	"data/TEXTURE/pause_quit.png"	// ポーズのテクスチャ名
#define PMENU_TEXTURENAME4	"data/TEXTURE/pause_logo.png"	// ポーズのテクスチャ名
#define PMENU_SIZE_X	(200.0f)		// メニューの大きさ(X)
#define PMENU_SIZE_Y	(60.0f)			// メニューの大きさ(Y)
#define PMENU_FLASHING	(10)			// メニューの点滅
#define PMENU_MAX		(MAX_PAUSE + 1)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePmenu[PMENU_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPmenu = NULL;				// 頂点バッファへのポインタ
PAUSE g_Pause[PMENU_MAX];
int g_nNumSelect;		// 現在の状態

//=============================================================================
// 初期化処理
//=============================================================================
void InitPause(void)
{
	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 情報の初期化
	g_Pause[CONTINUE].pos = D3DXVECTOR3(640, 250, 0.0f);
	g_Pause[RETRY].pos = D3DXVECTOR3(640, 400, 0.0f);
	g_Pause[QUIT].pos = D3DXVECTOR3(640, 550, 0.0f);

	g_Pause[CONTINUE].nType = 0;
	g_Pause[RETRY].nType = 1;
	g_Pause[QUIT].nType = 2;

	g_Pause[MAX_PAUSE].pos = D3DXVECTOR3(640, 100, 0.0f);
	g_Pause[MAX_PAUSE].nType = 3;

	g_nNumSelect = CONTINUE;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PMENU_TEXTURENAME1, &g_pTexturePmenu[CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, PMENU_TEXTURENAME2, &g_pTexturePmenu[RETRY]);
	D3DXCreateTextureFromFile(pDevice, PMENU_TEXTURENAME3, &g_pTexturePmenu[QUIT]);
	D3DXCreateTextureFromFile(pDevice, PMENU_TEXTURENAME4, &g_pTexturePmenu[MAX_PAUSE]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (PMENU_MAX + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPmenu,
		NULL);

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPmenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < MAX_PAUSE; nCntMenu++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_Pause[nCntMenu].pos.x - PMENU_SIZE_X, g_Pause[nCntMenu].pos.y - PMENU_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Pause[nCntMenu].pos.x + PMENU_SIZE_X, g_Pause[nCntMenu].pos.y - PMENU_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Pause[nCntMenu].pos.x - PMENU_SIZE_X, g_Pause[nCntMenu].pos.y + PMENU_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Pause[nCntMenu].pos.x + PMENU_SIZE_X, g_Pause[nCntMenu].pos.y + PMENU_SIZE_Y, 0.0f);

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

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_Pause[nCntMenu].pos.x - (PMENU_SIZE_X + 60), g_Pause[nCntMenu].pos.y - PMENU_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Pause[nCntMenu].pos.x + (PMENU_SIZE_X + 60), g_Pause[nCntMenu].pos.y - PMENU_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Pause[nCntMenu].pos.x - (PMENU_SIZE_X + 60), g_Pause[nCntMenu].pos.y + PMENU_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Pause[nCntMenu].pos.x + (PMENU_SIZE_X + 60), g_Pause[nCntMenu].pos.y + PMENU_SIZE_Y, 0.0f);

	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(0.0f		  , 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f		  , SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffPmenu->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	int nCntMenu;

	for (nCntMenu = 0; nCntMenu < MAX_PAUSE; nCntMenu++)
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
void UpdatePause(void)
{
	SelectPause();

	FADE pFade;
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadTrigger(DIJS_BUTTON_7, 0) == true)
		{
			PlaySound(SOUND_LABEL_PAUSE);
			g_nNumSelect = CONTINUE;
		}

		if (GetJoyPadRepeat(DIJS_BUTTON_10, 0) == true || GetJoyPadRepeat(DIJS_BUTTON_18, 0) == true || GetKeyboardRepeat(DIK_UP) == true)
		{
			PlaySound(SOUND_LABEL_SELECT2);
			g_nNumSelect = (g_nNumSelect + (MAX_PAUSE - 1)) % MAX_PAUSE;	// 選択状態を１つ上げる
			SelectPause();
		}

		if (GetJoyPadRepeat(DIJS_BUTTON_11, 0) == true || GetJoyPadRepeat(DIJS_BUTTON_19, 0) == true || GetKeyboardRepeat(DIK_DOWN) == true)
		{
			PlaySound(SOUND_LABEL_SELECT2);
			g_nNumSelect = (g_nNumSelect + 1) % MAX_PAUSE;						// 選択状態を１つ下げる
			SelectPause();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPmenu, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntMenu = PMENU_MAX; nCntMenu >= 0; nCntMenu--)
	{
		if (nCntMenu == PMENU_MAX)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}
		else
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePmenu[g_Pause[nCntMenu].nType]);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMenu, 2);
	}
}

//=============================================================================
// 選択処理
//=============================================================================
void SelectPause(void)
{
	PAUSECHECK *pPause;
	pPause = GetPauseMenu();

	FADE pFade;
	pFade = *GetFade();

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(DIJS_BUTTON_1, 0) == true)
	{
		switch (g_nNumSelect)
		{
		case CONTINUE:
			pPause->bPause = false;
			PlaySound(SOUND_LABEL_SELECT);
			break;

		case RETRY:
			if (pFade == FADE_NONE)
			{
				PlaySound(SOUND_LABEL_SELECT);
				SetFadeMode(MODE_GAME);
			}
			break;

		case QUIT:
			if (pFade == FADE_NONE)
			{
				PlaySound(SOUND_LABEL_SELECT);
				SetFadeMode(MODE_TITLE);
			}
			break;
		}

		// 現在のステージを取得
		STAGENUM stageNum = GetNumState();

		if (g_nNumSelect != CONTINUE)
		{// コンテニュー以外を選択
			// ステージのBGMを止める
			if (stageNum == STAGENUM_1)
			{// 1
				StopSound(SOUND_LABEL_STAGE1);
				StopSound(SOUND_LABEL_RAIN);
			}
			else if (stageNum == STAGENUM_2)
			{// 2
				StopSound(SOUND_LABEL_STAGE2);
			}
			else if (stageNum == STAGENUM_3)
			{// 3
				StopSound(SOUND_LABEL_STAGE3);
			}
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

