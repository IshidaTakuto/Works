//=============================================================================
//
// ゲームオーバー処理 [gameover.cpp]
// Author : 
//
//=============================================================================
#include "gameover.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAMEOVER_TEXTURENAME1	"data/TEXTURE/GAMEOVER/gameover_g.png"		// 読み込むテクスチャの名前１
#define GAMEOVER_TEXTURENAME2	"data/TEXTURE/GAMEOVER/gameover_a.png"		// 読み込むテクスチャの名前１
#define GAMEOVER_TEXTURENAME3	"data/TEXTURE/GAMEOVER/gameover_m.png"		// 読み込むテクスチャの名前１
#define GAMEOVER_TEXTURENAME4	"data/TEXTURE/GAMEOVER/gameover_e.png"		// 読み込むテクスチャの名前１
#define GAMEOVER_TEXTURENAME5	"data/TEXTURE/GAMEOVER/gameover_o.png"		// 読み込むテクスチャの名前１
#define GAMEOVER_TEXTURENAME6	"data/TEXTURE/GAMEOVER/gameover_v.png"		// 読み込むテクスチャの名前１
#define GAMEOVER_TEXTURENAME7	"data/TEXTURE/GAMEOVER/gameover_e.png"		// 読み込むテクスチャの名前１
#define GAMEOVER_TEXTURENAME8	"data/TEXTURE/GAMEOVER/gameover_r.png"		// 読み込むテクスチャの名前１
#define GAMEOVER_TEXTURENAME9	"data/TEXTURE/GAMEOVER/PressAnyBotton000.png"		// 読み込むテクスチャの名前２

//G-----------------------------------------------------------------------------------
#define GAMEOVER_G_POS_X	(90)								// ロゴのX座標
#define GAMEOVER_G_POS_Y	(310.0f)							// ロゴのY座標
#define GAMEOVER_G_WIDTH	(160.0f)							// ロゴの幅
#define GAMEOVER_G_HEIGHT (160.0f)								// ロゴの高さ
//A-----------------------------------------------------------------------------------
#define GAMEOVER_A_POS_X	(240)								// ロゴのX座標
#define GAMEOVER_A_POS_Y	(310.0f)							// ロゴのY座標
#define GAMEOVER_A_WIDTH	(160.0f)							// ロゴの幅
#define GAMEOVER_A_HEIGHT (160.0f)								// ロゴの高さ
//M-----------------------------------------------------------------------------------
#define GAMEOVER_M_POS_X	(400)								// ロゴのX座標
#define GAMEOVER_M_POS_Y	(310.0f)							// ロゴのY座標
#define GAMEOVER_M_WIDTH	(160.0f)							// ロゴの幅
#define GAMEOVER_M_HEIGHT (160.0f)								// ロゴの高さ
//E-----------------------------------------------------------------------------------
#define GAMEOVER_E_POS_X	(550)								// ロゴのX座標
#define GAMEOVER_E_POS_Y	(310.0f)							// ロゴのY座標
#define GAMEOVER_E_WIDTH	(160.0f)							// ロゴの幅
#define GAMEOVER_E_HEIGHT (160.0f)								// ロゴの高さ
//O-----------------------------------------------------------------------------------
#define GAMEOVER_O_POS_X	(700)								// ロゴのX座標
#define GAMEOVER_O_POS_Y	(310.0f)							// ロゴのY座標
#define GAMEOVER_O_WIDTH	(160.0f)							// ロゴの幅
#define GAMEOVER_O_HEIGHT (160.0f)								// ロゴの高さ
//V-----------------------------------------------------------------------------------
#define GAMEOVER_V_POS_X	(850)								// ロゴのX座標
#define GAMEOVER_V_POS_Y	(310.0f)							// ロゴのY座標
#define GAMEOVER_V_WIDTH	(160.0f)							// ロゴの幅
#define GAMEOVER_V_HEIGHT (160.0f)								// ロゴの高さ
//E-----------------------------------------------------------------------------------
#define GAMEOVER_E2_POS_X	(1000)								// ロゴのX座標
#define GAMEOVER_E2_POS_Y	(310.0f)							// ロゴのY座標
#define GAMEOVER_E2_WIDTH	(160.0f)							// ロゴの幅
#define GAMEOVER_E2_HEIGHT (160.0f)								// ロゴの高さ
//R-----------------------------------------------------------------------------------
#define GAMEOVER_R_POS_X	(1150)								// ロゴのX座標
#define GAMEOVER_R_POS_Y	(310.0f)							// ロゴのY座標
#define GAMEOVER_R_WIDTH	(160.0f)							// ロゴの幅
#define GAMEOVER_R_HEIGHT (160.0f)								// ロゴの高さ

//動く速さ
#define GAMEOVER_LOGO_MOVE		(2.0f)							// ロゴの移動速度
#define GAMEOVER_LOGO_MOVE_MAX	(250.0f)						// ロゴのY座標の限界

#define GAMEOVER_MOVE_A	(0.8f)							// ロゴのY座標
#define GAMEOVER_MOVE_B	(0.5f)							// ロゴのY座標

#define GAMEOVER_ENTER_POS_X	(SCREEN_WIDTH / 2)				// エンターのX座標
#define GAMEOVER_ENTER_POS_Y	(550.0f)						// エンターのY座標
#define GAMEOVER_ENTER_WIDTH	(400.0f)						// エンターの幅
#define GAMEOVER_ENTER_HEIGHT	(80.0f)							// エンターの高さ
#define GAMEOVER_FLASH_ENTER	(60)							// エンターの点滅間隔

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{// テクスチャの種類
	GAMEOVETEX_LOGO_G = 0,	// G
	GAMEOVETEX_LOGO_A,		// A
	GAMEOVETEX_LOGO_M,		// M
	GAMEOVETEX_LOGO_E,		// E
	GAMEOVETEX_LOGO_O,		// O
	GAMEOVETEX_LOGO_V,		// V
	GAMEOVETEX_LOGO_E2,		// E
	GAMEOVETEX_LOGO_R,		// R
	GAMEOVETEX_ENTER,		// エンター
	GAMEOVETEX_MAX			// テクスチャの総数
}GAMEOVERTEX;

typedef struct
{// タイトルの情報
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 位置
	float fWidth;			// 幅
	float fHeight;			// 高さ
	bool bFlash;			// 点滅するかどうか
	bool bDraw;				// 描画するかどうか
} GameOver;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexGameOver(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureGameOver[GAMEOVETEX_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGameOver = NULL;					// 頂点バッファへのポインタ
GameOver				g_aGameOver[GAMEOVETEX_MAX];				// タイトルの情報
int						g_nCntFpsGameOver;							// 経過時間
int g_nCntFpsOver;

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitGameOver(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 情報の初期化
	for (int nCntTex = 0; nCntTex < GAMEOVETEX_MAX; nCntTex++)
	{
		g_aGameOver[nCntTex].bFlash = false;
		switch (nCntTex)
		{
		case GAMEOVETEX_LOGO_G:
			g_aGameOver[nCntTex].pos = D3DXVECTOR3(GAMEOVER_G_POS_X, GAMEOVER_G_POS_Y, 0.0f);
			g_aGameOver[nCntTex].move = D3DXVECTOR3(0.0f,- 0.3f, 0.0f);
			g_aGameOver[nCntTex].fWidth = GAMEOVER_G_WIDTH;
			g_aGameOver[nCntTex].fHeight = GAMEOVER_G_HEIGHT;
			g_aGameOver[nCntTex].bDraw = true;
			break;
		case GAMEOVETEX_LOGO_A:
			g_aGameOver[nCntTex].pos = D3DXVECTOR3(GAMEOVER_A_POS_X, GAMEOVER_A_POS_Y, 0.0f);
			g_aGameOver[nCntTex].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aGameOver[nCntTex].fWidth = GAMEOVER_A_WIDTH;
			g_aGameOver[nCntTex].fHeight = GAMEOVER_A_HEIGHT;
			g_aGameOver[nCntTex].bDraw = true;
			break;
		case GAMEOVETEX_LOGO_M:
			g_aGameOver[nCntTex].pos = D3DXVECTOR3(GAMEOVER_M_POS_X, GAMEOVER_M_POS_Y, 0.0f);
			g_aGameOver[nCntTex].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aGameOver[nCntTex].fWidth = GAMEOVER_M_WIDTH;
			g_aGameOver[nCntTex].fHeight = GAMEOVER_M_HEIGHT;
			g_aGameOver[nCntTex].bDraw = true;
			break;
		case GAMEOVETEX_LOGO_E:
			g_aGameOver[nCntTex].pos = D3DXVECTOR3(GAMEOVER_E_POS_X, GAMEOVER_E_POS_Y, 0.0f);
			g_aGameOver[nCntTex].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aGameOver[nCntTex].fWidth = GAMEOVER_E_WIDTH;
			g_aGameOver[nCntTex].fHeight = GAMEOVER_E_HEIGHT;
			g_aGameOver[nCntTex].bDraw = true;
			break;
		case GAMEOVETEX_LOGO_O:
			g_aGameOver[nCntTex].pos = D3DXVECTOR3(GAMEOVER_O_POS_X, GAMEOVER_O_POS_Y, 0.0f);
			g_aGameOver[nCntTex].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aGameOver[nCntTex].fWidth = GAMEOVER_O_WIDTH;
			g_aGameOver[nCntTex].fHeight = GAMEOVER_O_HEIGHT;
			g_aGameOver[nCntTex].bDraw = true;
			break;
		case GAMEOVETEX_LOGO_V:
			g_aGameOver[nCntTex].pos = D3DXVECTOR3(GAMEOVER_V_POS_X, GAMEOVER_V_POS_Y, 0.0f);
			g_aGameOver[nCntTex].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aGameOver[nCntTex].fWidth = GAMEOVER_V_WIDTH;
			g_aGameOver[nCntTex].fHeight = GAMEOVER_V_HEIGHT;
			g_aGameOver[nCntTex].bDraw = true;
			break;
		case GAMEOVETEX_LOGO_E2:
			g_aGameOver[nCntTex].pos = D3DXVECTOR3(GAMEOVER_E2_POS_X, GAMEOVER_E2_POS_Y, 0.0f);
			g_aGameOver[nCntTex].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aGameOver[nCntTex].fWidth = GAMEOVER_E2_WIDTH;
			g_aGameOver[nCntTex].fHeight = GAMEOVER_E2_HEIGHT;
			g_aGameOver[nCntTex].bDraw = true;
			break;
		case GAMEOVETEX_LOGO_R:
			g_aGameOver[nCntTex].pos = D3DXVECTOR3(GAMEOVER_R_POS_X, GAMEOVER_R_POS_Y, 0.0f);
			g_aGameOver[nCntTex].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aGameOver[nCntTex].fWidth = GAMEOVER_R_WIDTH;
			g_aGameOver[nCntTex].fHeight = GAMEOVER_R_HEIGHT;
			g_aGameOver[nCntTex].bDraw = true;
			break;
		case GAMEOVETEX_ENTER:
			g_aGameOver[nCntTex].pos = D3DXVECTOR3(GAMEOVER_ENTER_POS_X, GAMEOVER_ENTER_POS_Y, 0.0f);
			g_aGameOver[nCntTex].fWidth = GAMEOVER_ENTER_WIDTH;
			g_aGameOver[nCntTex].fHeight = GAMEOVER_ENTER_HEIGHT;
			g_aGameOver[nCntTex].bDraw = false;
			break;
		}
	}
	g_nCntFpsGameOver = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME1, &g_apTextureGameOver[GAMEOVETEX_LOGO_G]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME2, &g_apTextureGameOver[GAMEOVETEX_LOGO_A]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME3, &g_apTextureGameOver[GAMEOVETEX_LOGO_M]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME4, &g_apTextureGameOver[GAMEOVETEX_LOGO_E]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME5, &g_apTextureGameOver[GAMEOVETEX_LOGO_O]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME6, &g_apTextureGameOver[GAMEOVETEX_LOGO_V]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME7, &g_apTextureGameOver[GAMEOVETEX_LOGO_E2]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME8, &g_apTextureGameOver[GAMEOVETEX_LOGO_R]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME9, &g_apTextureGameOver[GAMEOVETEX_ENTER]);

	// 頂点情報の作成
	MakeVertexGameOver(pDevice);

}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitGameOver(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < GAMEOVETEX_MAX; nCntTex++)
	{
		if (g_apTextureGameOver[nCntTex] != NULL)
		{
			g_apTextureGameOver[nCntTex]->Release();
			g_apTextureGameOver[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffGameOver != NULL)
	{
		g_pVtxBuffGameOver->Release();
		g_pVtxBuffGameOver = NULL;
	}

}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateGameOver(void)
{
	VERTEX_2D *pVtx;
	FADE pFade = *GetFade(); // フェードの取得
	PLAYER *pPlayer = GetPlayer();

	for (int nCntTex = 0; nCntTex < GAMEOVETEX_MAX; nCntTex++)
	{
		g_nCntFpsOver++;

		if ( pPlayer[0].bButton[BUTTON_ENTER] == true)
		{// 決定キー(ENTERキー)が押された場合
			if (pFade == FADE_NONE)
			{// フェードが何もしていない状態の場合
				PlaySound(SOUND_LABEL_ENTER);
				g_aGameOver[GAMEOVETEX_ENTER].bFlash = true;
				SetFade(MODE_RESULT,0,0.0f,0);
			}
		}
			if (g_aGameOver[nCntTex].bFlash == true)
			{
				if (g_nCntFpsOver % 2 == 0)
				{// 2F経過ごとに
					g_aGameOver[nCntTex].bDraw = g_aGameOver[nCntTex].bDraw ? false : true;
				}
			}
			else
			{
				if (nCntTex == GAMEOVETEX_ENTER)
				{// エンターの場合
					if (g_nCntFpsOver % 120 == 0)
					{// 一定時間経過ごとに
						g_aGameOver[nCntTex].bDraw = g_aGameOver[nCntTex].bDraw ? false : true;
					}
				}
			}


		//G-----------------------------------------------------------
		if (g_aGameOver[GAMEOVETEX_LOGO_G].pos.y <= 200)
		{
			g_aGameOver[GAMEOVETEX_LOGO_G].move.y = 0.0f;

			g_aGameOver[GAMEOVETEX_LOGO_A].move.y -= GAMEOVER_MOVE_A;

			if (g_aGameOver[GAMEOVETEX_LOGO_G].move.y == 0.0f)
			{
				g_aGameOver[GAMEOVETEX_LOGO_G].move.y += GAMEOVER_MOVE_B;
			}

		}
		else if (g_aGameOver[GAMEOVETEX_LOGO_G].pos.y >= 320)
		{
			g_aGameOver[GAMEOVETEX_LOGO_G].move.y = 0.0f;
		}

		//A-----------------------------------------------------------
		if (g_aGameOver[GAMEOVETEX_LOGO_A].pos.y <= 200)
		{
			g_aGameOver[GAMEOVETEX_LOGO_A].move.y = 0.0f;

			g_aGameOver[GAMEOVETEX_LOGO_M].move.y -= GAMEOVER_MOVE_A;

			if (g_aGameOver[GAMEOVETEX_LOGO_A].move.y == 0.0f)
			{
				g_aGameOver[GAMEOVETEX_LOGO_A].move.y += GAMEOVER_MOVE_B;
			}
		}
		else if (g_aGameOver[GAMEOVETEX_LOGO_A].pos.y >= 320)
		{
			g_aGameOver[GAMEOVETEX_LOGO_A].move.y = 0.0f;
		}

		//M-----------------------------------------------------------
		if (g_aGameOver[GAMEOVETEX_LOGO_M].pos.y <= 200)
		{
			g_aGameOver[GAMEOVETEX_LOGO_M].move.y = 0.0f;

			g_aGameOver[GAMEOVETEX_LOGO_E].move.y -= GAMEOVER_MOVE_A;

			if (g_aGameOver[GAMEOVETEX_LOGO_M].move.y == 0.0f)
			{
				g_aGameOver[GAMEOVETEX_LOGO_M].move.y += GAMEOVER_MOVE_B;
			}
		}
		else if (g_aGameOver[GAMEOVETEX_LOGO_M].pos.y >= 320)
		{
			g_aGameOver[GAMEOVETEX_LOGO_M].move.y = 0.0f;
		}

		//E-----------------------------------------------------------
		if (g_aGameOver[GAMEOVETEX_LOGO_E].pos.y <= 200)
		{
			g_aGameOver[GAMEOVETEX_LOGO_E].move.y = 0.0f;

			g_aGameOver[GAMEOVETEX_LOGO_O].move.y -= GAMEOVER_MOVE_A;

			if (g_aGameOver[GAMEOVETEX_LOGO_E].move.y == 0.0f)
			{
				g_aGameOver[GAMEOVETEX_LOGO_E].move.y += GAMEOVER_MOVE_B;
			}
		}
		else if (g_aGameOver[GAMEOVETEX_LOGO_E].pos.y >= 320)
		{
			g_aGameOver[GAMEOVETEX_LOGO_E].move.y = 0.0f;
		}

		//O-----------------------------------------------------------
		if (g_aGameOver[GAMEOVETEX_LOGO_O].pos.y <= 200)
		{
			g_aGameOver[GAMEOVETEX_LOGO_O].move.y = 0.0f;

			g_aGameOver[GAMEOVETEX_LOGO_V].move.y -= GAMEOVER_MOVE_A;

			if (g_aGameOver[GAMEOVETEX_LOGO_O].move.y == 0.0f)
			{
				g_aGameOver[GAMEOVETEX_LOGO_O].move.y += GAMEOVER_MOVE_B;
			}
		}
		else if (g_aGameOver[GAMEOVETEX_LOGO_O].pos.y >= 320)
		{
			g_aGameOver[GAMEOVETEX_LOGO_O].move.y = 0.0f;
		}

		//V-----------------------------------------------------------
		if (g_aGameOver[GAMEOVETEX_LOGO_V].pos.y <= 200)
		{
			g_aGameOver[GAMEOVETEX_LOGO_V].move.y = 0.0f;

			g_aGameOver[GAMEOVETEX_LOGO_E2].move.y -= GAMEOVER_MOVE_A;

			if (g_aGameOver[GAMEOVETEX_LOGO_V].move.y == 0.0f)
			{
				g_aGameOver[GAMEOVETEX_LOGO_V].move.y += GAMEOVER_MOVE_B;
			}
		}
		else if (g_aGameOver[GAMEOVETEX_LOGO_V].pos.y >= 320)
		{
			g_aGameOver[GAMEOVETEX_LOGO_V].move.y = 0.0f;
		}

		//E2-----------------------------------------------------------
		if (g_aGameOver[GAMEOVETEX_LOGO_E2].pos.y <= 200)
		{
			g_aGameOver[GAMEOVETEX_LOGO_E2].move.y = 0.0f;

			g_aGameOver[GAMEOVETEX_LOGO_R].move.y -= GAMEOVER_MOVE_A;

			if (g_aGameOver[GAMEOVETEX_LOGO_E2].move.y == 0.0f)
			{
				g_aGameOver[GAMEOVETEX_LOGO_E2].move.y += GAMEOVER_MOVE_B;
			}
		}
		else if (g_aGameOver[GAMEOVETEX_LOGO_E2].pos.y >= 320)
		{
			g_aGameOver[GAMEOVETEX_LOGO_E2].move.y = 0.0f;
		}
		//R-----------------------------------------------------------
		if (g_aGameOver[GAMEOVETEX_LOGO_R].pos.y <= 200)
		{
			g_aGameOver[GAMEOVETEX_LOGO_R].move.y = 0.0f;

			if (g_aGameOver[GAMEOVETEX_LOGO_R].move.y == 0.0f)
			{
				g_aGameOver[GAMEOVETEX_LOGO_R].move.y += GAMEOVER_MOVE_B;
			}
		}
		else if (g_aGameOver[GAMEOVETEX_LOGO_R].pos.y >= 320)
		{
			g_aGameOver[GAMEOVETEX_LOGO_R].move.y = 0.0f;
		}

		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffGameOver->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_G].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_G].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_G].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_G].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_G].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_G].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_G].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_G].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_G].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_G].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_G].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_G].pos;

		pVtx += 4;
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_A].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_A].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_A].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_A].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_A].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_A].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_A].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_A].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_A].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_A].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_A].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_A].pos;

		pVtx += 4;
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_M].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_M].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_M].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_M].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_M].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_M].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_M].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_M].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_M].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_M].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_M].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_M].pos;

		pVtx += 4;
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_E].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_E].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_E].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_E].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_E].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_E].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_E].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_E].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_E].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_E].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_E].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_E].pos;

		pVtx += 4;
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_O].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_O].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_O].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_O].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_O].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_O].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_O].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_O].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_O].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_O].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_O].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_O].pos;

		pVtx += 4;
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_V].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_V].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_V].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_V].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_V].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_V].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_V].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_V].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_V].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_V].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_V].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_V].pos;

		pVtx += 4;
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_E2].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_E2].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_E2].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_E2].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_E2].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_E2].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_E2].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_E2].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_E2].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_E2].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_E2].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_E2].pos;

		pVtx += 4;
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_R].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_R].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_R].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_R].fWidth / 2, -g_aGameOver[GAMEOVETEX_LOGO_R].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_R].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aGameOver[GAMEOVETEX_LOGO_R].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_R].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_R].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aGameOver[GAMEOVETEX_LOGO_R].fWidth / 2, g_aGameOver[GAMEOVETEX_LOGO_R].fHeight / 2, 0.0f) + g_aGameOver[GAMEOVETEX_LOGO_R].pos;


		// 頂点バッファをアンロックする
		g_pVtxBuffGameOver->Unlock();

		g_aGameOver[GAMEOVETEX_LOGO_G].pos.y += g_aGameOver[GAMEOVETEX_LOGO_G].move.y;
		g_aGameOver[GAMEOVETEX_LOGO_A].pos.y += g_aGameOver[GAMEOVETEX_LOGO_A].move.y;
		g_aGameOver[GAMEOVETEX_LOGO_M].pos.y += g_aGameOver[GAMEOVETEX_LOGO_M].move.y;
		g_aGameOver[GAMEOVETEX_LOGO_E].pos.y += g_aGameOver[GAMEOVETEX_LOGO_E].move.y;
		g_aGameOver[GAMEOVETEX_LOGO_O].pos.y += g_aGameOver[GAMEOVETEX_LOGO_O].move.y;
		g_aGameOver[GAMEOVETEX_LOGO_V].pos.y += g_aGameOver[GAMEOVETEX_LOGO_V].move.y;
		g_aGameOver[GAMEOVETEX_LOGO_E2].pos.y += g_aGameOver[GAMEOVETEX_LOGO_E2].move.y;
		g_aGameOver[GAMEOVETEX_LOGO_R].pos.y += g_aGameOver[GAMEOVETEX_LOGO_R].move.y;
	}
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawGameOver(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameOver, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTex = 0; nCntTex < GAMEOVETEX_MAX; nCntTex++)
	{
		if (g_aGameOver[nCntTex].bDraw == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureGameOver[nCntTex]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP	// プリミティブの種類
				, nCntTex * 4		// 開始する頂点のインデックス
				, 2);				// 描画するプリミティブの数
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexGameOver(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * GAMEOVETEX_MAX
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_2D
		, D3DPOOL_MANAGED
		, &g_pVtxBuffGameOver
		, NULL);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGameOver->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTex = 0; nCntTex < GAMEOVETEX_MAX; nCntTex++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aGameOver[nCntTex].fWidth / 2, -g_aGameOver[nCntTex].fHeight / 2, 0.0f) + g_aGameOver[nCntTex].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aGameOver[nCntTex].fWidth / 2, -g_aGameOver[nCntTex].fHeight / 2, 0.0f) + g_aGameOver[nCntTex].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aGameOver[nCntTex].fWidth / 2, g_aGameOver[nCntTex].fHeight / 2, 0.0f) + g_aGameOver[nCntTex].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aGameOver[nCntTex].fWidth / 2, g_aGameOver[nCntTex].fHeight / 2, 0.0f) + g_aGameOver[nCntTex].pos;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffGameOver->Unlock();
}
