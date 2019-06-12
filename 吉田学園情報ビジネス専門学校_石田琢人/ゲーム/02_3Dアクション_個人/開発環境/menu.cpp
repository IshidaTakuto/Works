//=============================================================================
//
// メニュー処理 [menu.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "menu.h"
#include "fade.h"
#include "camera.h"
#include "rain.h"
#include "effect.h"
#include "sound.h"
#include "joypad.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MENU_TEXTURENAME1	"data/TEXTURE/menu_game.png"		// テクスチャ名
#define MENU_TEXTURENAME2	"data/TEXTURE/menu_tutorial.png"	// テクスチャ名
#define MENU_TEXTURENAME3	"data/TEXTURE/menu_title.png"		// テクスチャ名
#define MENU_SIZE_X	(300.0f)	// メニューの大きさ(X)
#define MENU_SIZE_Y	(50.0f)		// メニューの大きさ(Y)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMenu[MENUTYPE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenu = NULL;				// 頂点バッファへのポインタ
MENU g_Menu[MENUTYPE_MAX];
int g_nNumSelectMenu;

//=============================================================================
// 初期化処理
//=============================================================================
void InitMenu(void)
{
	// カメラの初期化処理
	InitCamera();

	// 雨の初期化処理
	InitRain();

	// エフェクト初期化処理
	InitEffect();
	SetEffect(D3DXVECTOR3(100.0f, 60.0f, 200.0f), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), 100.0f, 0, EFFECTSTATE_MOON);
	SetEffect(D3DXVECTOR3(100.0f, 60.0f, 200.0f), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), 120.0f, 0, EFFECTSTATE_NONE);

	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 情報の初期化
	g_Menu[MENUTYPE_GAME].pos = D3DXVECTOR3(640, 100, 0.0f);
	g_Menu[MENUTYPE_TUTORIAL].pos = D3DXVECTOR3(640, 350, 0.0f);
	g_Menu[MENUTYPE_TITLE].pos = D3DXVECTOR3(640, 600, 0.0f);

	g_Menu[MENUTYPE_GAME].nType = 0;
	g_Menu[MENUTYPE_TUTORIAL].nType = 1;
	g_Menu[MENUTYPE_TITLE].nType = 2;

	g_nNumSelectMenu = MENUTYPE_GAME;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME1, &g_pTextureMenu[MENUTYPE_GAME]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME2, &g_pTextureMenu[MENUTYPE_TUTORIAL]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME3, &g_pTextureMenu[MENUTYPE_TITLE]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MENUTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenu,
		NULL);

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < MENUTYPE_MAX; nCntMenu++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x - MENU_SIZE_X, g_Menu[nCntMenu].pos.y - MENU_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x + MENU_SIZE_X, g_Menu[nCntMenu].pos.y - MENU_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x - MENU_SIZE_X, g_Menu[nCntMenu].pos.y + MENU_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Menu[nCntMenu].pos.x + MENU_SIZE_X, g_Menu[nCntMenu].pos.y + MENU_SIZE_Y, 0.0f);

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
	g_pVtxBuffMenu->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMenu(void)
{
	// カメラの終了処理
	UninitCamera();

	// 雨の終了処理
	UninitRain();

	// エフェクトの終了処理
	UninitEffect();

	int nCntMenu;

	for (nCntMenu = 0; nCntMenu < MENUTYPE_MAX; nCntMenu++)
	{
		// テクスチャの破棄
		if (g_pTextureMenu[nCntMenu] != NULL)
		{
			g_pTextureMenu[nCntMenu]->Release();
			g_pTextureMenu[nCntMenu] = NULL;
		}
	}

	// 頂点バッファの破棄
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
	XINPUT_STATE state;
	state = GetJoyPad();

	// カメラの更新処理
	UpdateCamera();

	// 雨の更新処理
	UpdateRain();

	// エフェクトの更新処理
	UpdateEffect();

	SelectMenu();

	if (GetKeyboardRepeat(DIK_W) == true || GetKeyboardRepeat(DIK_UP) == true)
	{// 選択状態を１つ上げる
		PlaySound(SOUND_LABEL_BOTTON001);
		g_nNumSelectMenu = (g_nNumSelectMenu + (MENUTYPE_MAX - 1)) % MENUTYPE_MAX;	
		SelectMenu();
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{// 選択状態を１つ上げる
		PlaySound(SOUND_LABEL_BOTTON001);
		g_nNumSelectMenu = (g_nNumSelectMenu + (MENUTYPE_MAX - 1)) % MENUTYPE_MAX;
		SelectMenu();
	}

	if (GetKeyboardRepeat(DIK_S) == true || GetKeyboardRepeat(DIK_DOWN) == true)
	{// 選択状態を１つ下げる
		PlaySound(SOUND_LABEL_BOTTON001);
		g_nNumSelectMenu = (g_nNumSelectMenu + 1) % MENUTYPE_MAX;				
		SelectMenu();
	}
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{// 選択状態を１つ下げる
		PlaySound(SOUND_LABEL_BOTTON001);
		g_nNumSelectMenu = (g_nNumSelectMenu + 1) % MENUTYPE_MAX;
		SelectMenu();
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMenu(void)
{
	// エフェクトの描画処理
	DrawEffect();

	// 雨の描画処理
	DrawRain();

	int nCntMenu;

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntMenu = 0; nCntMenu < MENUTYPE_MAX; nCntMenu++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMenu[g_Menu[nCntMenu].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMenu, 2);
	}
}

//=============================================================================
// 選択処理
//=============================================================================
void SelectMenu(void)
{
	XINPUT_STATE state;
	state = GetJoyPad();

	FADE pFade;
	pFade = *GetFade();

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_nNumSelectMenu)
		{
		case MENUTYPE_GAME:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_DECIDE);
					SetFade(MODE_GAME);
				}
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
				{
					PlaySound(SOUND_LABEL_DECIDE);
					SetFade(MODE_GAME);
				}

			}
			break;

		case MENUTYPE_TUTORIAL:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_DECIDE);
					SetFade(MODE_TUTORIAL);
				}
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
				{
					PlaySound(SOUND_LABEL_DECIDE);
					SetFade(MODE_TUTORIAL);
				}

			}
			break;

		case MENUTYPE_TITLE:
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_DECIDE);
					SetFade(MODE_TITLE);
				}
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
				{
					PlaySound(SOUND_LABEL_DECIDE);
					SetFade(MODE_TITLE);
				}

			}
			break;
		}
	}

	if (g_nNumSelectMenu == MENUTYPE_GAME)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_GAME * 4);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelectMenu != MENUTYPE_GAME)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_GAME * 4);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}

	if (g_nNumSelectMenu == MENUTYPE_TUTORIAL)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_TUTORIAL * 4);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelectMenu != MENUTYPE_TUTORIAL)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_TUTORIAL * 4);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}

	if (g_nNumSelectMenu == MENUTYPE_TITLE)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_TITLE * 4);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}
	else if (g_nNumSelectMenu != MENUTYPE_TITLE)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (MENUTYPE_TITLE * 4);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		// 頂点バッファをアンロック
		g_pVtxBuffMenu->Unlock();
	}
}

