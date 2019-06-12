//=============================================================================
//
// ストーリー処理 [Story.cpp]
// Author : ABE YUUTAROU
//
//=============================================================================
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "story.h"
#include "light.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STORY_TEXTURENAME1	"data/TEXTURE/STORY/box_horror_grbl.png"		// 読み込むテクスチャの名前１
#define STORY_TEXTURENAME2	"data/TEXTURE/STORY/KUROMEN.png"				// 読み込むテクスチャの名前１
//メッセージボックス
#define STORY_BOX_POS_X	(SCREEN_WIDTH/2)								// ロゴのX座標_A
#define STORY_BOX_POS_Y	(SCREEN_HEIGHT/2 +250)							// ロゴのY座標_A
#define STORY_BOX_WIDTH	(900)											// ロゴの幅
#define STORY_BOX_HEIGHT (200)											// ロゴの高さ
//画面開始の黒幕
#define STORY_BLACK_POS_X	(SCREEN_WIDTH/2)							//	黒幕のX座標_A
#define STORY_BLACK_POS_Y	(SCREEN_HEIGHT/2)							//　黒幕のY座標_A
#define STORY_BLACK_WIDTH	(SCREEN_WIDTH)								// 黒幕の幅
#define STORY_BLACK_HEIGHT (SCREEN_HEIGHT)								// 黒幕の高さ

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{// テクスチャの種類
	STORYTEX_BOX = 0,	// ロゴ
	STORYTEX_BLACK,		//　画面開始の黒幕
	STORYTEX_MAX		// テクスチャの総数
} STORYTEX;

typedef struct
{// タイトルの情報
	D3DXVECTOR3 pos;	// 位置
	float fWidth;		// 幅
	float fHeight;		// 高さ
	bool bFlash;		// 点滅するかどうか
	bool bDraw;			// 描画するかどうか
} Story;

typedef enum
{// テクスチャの種類
	STORYTEX_BLACK_NOMAL = 0,	// 通常
	STORYTEX_BLACK_DOWN,
	STORYTEX_BLACK_QUIT,		//　カラーダウン
	STORYTEX_BLACK_MAX				// テクスチャの総数
} BLACKTEX;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexStory(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureStory[STORYTEX_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffStory = NULL;					// 頂点バッファへのポインタ
Story					g_aStory[STORYTEX_MAX];					// タイトルの情報
STORYSTATE				g_aStoryState;							//ストーリー会話状態
BLACKTEX				g_nBlackstate;
int						 g_aStoryMene;;							//ストーリーのメニュー
int						g_nCntFpsStory;							// 経過時間
int						g_nCntLogoStopStory;					// 経過時間
int						g_nBlackColDown;						// 黒幕のカラーダウン
int						g_aStoryCounterTXT;			

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitStory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 情報の初期化
	for (int nCntTex = 0; nCntTex < STORYTEX_MAX; nCntTex++)
	{
		g_aStory[nCntTex].bFlash = false;
		switch (nCntTex)
		{
		case STORYTEX_BOX:			//メッセージボックス
			g_aStory[nCntTex].pos = D3DXVECTOR3(STORY_BOX_POS_X, STORY_BOX_POS_Y, 0.0f);
			g_aStory[nCntTex].fWidth = STORY_BOX_WIDTH;
			g_aStory[nCntTex].fHeight = STORY_BOX_HEIGHT;
			g_aStory[nCntTex].bDraw = true;
			break; 
		case STORYTEX_BLACK:		//画面開始の黒幕
			g_aStory[nCntTex].pos = D3DXVECTOR3(STORY_BLACK_POS_X, STORY_BLACK_POS_Y, 0.0f);
			g_aStory[nCntTex].fWidth = STORY_BLACK_WIDTH;
			g_aStory[nCntTex].fHeight = STORY_BLACK_HEIGHT;
			g_aStory[nCntTex].bDraw = true;
			break; 
		}
	}
	//変数の初期化
	g_nCntFpsStory = 0;
	g_aStoryMene = 0;
	g_nCntLogoStopStory = 0;
	g_nBlackColDown = 0;
	g_nBlackstate = STORYTEX_BLACK_NOMAL;
	g_aStoryCounterTXT = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, STORY_TEXTURENAME1, &g_apTextureStory[STORYTEX_BOX]);
	D3DXCreateTextureFromFile(pDevice, STORY_TEXTURENAME2, &g_apTextureStory[STORYTEX_BLACK]);

	// 頂点情報の作成
	MakeVertexStory(pDevice);

	//ライトの初期化処理
	InitLight();
	//カメラの初期化処理
	InitCamera();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitStory(void)
{
	//ライトの終了処理
	UninitLight();
	// カメラの終了処理
	UninitCamera();

	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < STORYTEX_MAX; nCntTex++)
	{
		if (g_apTextureStory[nCntTex] != NULL)
		{
			g_apTextureStory[nCntTex]->Release();
			g_apTextureStory[nCntTex] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffStory != NULL)
	{
		g_pVtxBuffStory->Release();
		g_pVtxBuffStory = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateStory(void)
{
	VERTEX_2D *pVtx;
	FADE pFade = *GetFade(); // フェードの取得
	PLAYER *pPlayer = GetPlayer();

	g_aStoryCounterTXT = *GetStoryTXT();

	if (pFade == FADE_NONE)
	{
		switch (g_nBlackstate)
		{
		case STORYTEX_BLACK_NOMAL:
			if (pPlayer[0].bButton[BUTTON_ENTER] == true)
			{//エンターでセレクト画面へ
				g_nBlackstate = STORYTEX_BLACK_DOWN;
			}
			break;
		case STORYTEX_BLACK_DOWN:
			g_aStoryMene = 1;
			g_nBlackColDown++;
			if (g_nBlackColDown >= 200 )
			{
				g_aStoryCounterTXT = 200;
				g_nBlackColDown = 255;
				g_nBlackstate = STORYTEX_BLACK_QUIT;

			}
			break;
		case STORYTEX_BLACK_QUIT:
			//キーボード設定
			if ( pPlayer[0].bButton[BUTTON_ENTER] == true && g_aStoryCounterTXT >= 120)
			{//エンターでセレクト画面へ
					g_aStoryMene += 1;
					PlaySound(SOUND_LABEL_STORY_SE);
				
			}
			if (pPlayer[0].bButton[BUTTON_P] == true)
			{
				SetFade(MODE_SELCT, 0, 0.0f, 0);
			}
			
			switch (g_aStoryMene)
			{
			case  0:
				g_aStoryState = STORY_000;
				break;
			case 1:
				g_aStoryState = STORY_001;
				break;
			case 2:
				g_aStoryState = STORY_002;
				
				break;
			case 3:
				g_aStoryState = STORY_003;
				break;
			case 4:
				g_aStoryState = STORY_004;
				break;
			case 5:
				g_aStoryState = STORY_005;
				break;
			case 6:
				g_aStoryState = STORY_006;
				break;
			case 7:
				g_aStoryState = STORY_007;
				break;
			case 8:
				g_aStoryState = STORY_008;
				break;
			case 9:
				g_aStoryState = STORY_009;
				break;
			case 10:
				g_aStoryState = STORY_010;
				break;
			case 11:
				g_aStoryState = STORY_011;
				break;
			case 12:
				g_aStoryState = STORY_012;
				break;
			case 13:
				g_aStoryState = STORY_013;
				break;
			case 14:
				g_aStoryState = STORY_014;
				break;
			case 15:
				g_aStoryState = STORY_015;
				break;
			case 16:
				g_aStoryState = STORY_016;
				SetFade(MODE_SELCT, 0, 0.0f, 0);
				break;
			}
			break;
		}

		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffStory->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255 - g_nBlackColDown);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255 - g_nBlackColDown);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255 - g_nBlackColDown);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255 - g_nBlackColDown);

		// 頂点バッファをアンロックする
		g_pVtxBuffStory->Unlock();
	}

	//ライトの更新処理
	UpdateLight();
	//カメラの更新処理
	UpdateCamera();
}


//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawStory(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// カメラの設定
	SetCamera(0);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStory, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTex = 0; nCntTex < STORYTEX_MAX; nCntTex++)
	{
		if (g_aStory[nCntTex].bDraw == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureStory[nCntTex]);

			// ポリゴンの描画// プリミティブの種類// 開始する頂点のインデックス// 描画するプリミティブの数
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexStory(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * STORYTEX_MAX
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_2D
		, D3DPOOL_MANAGED
		, &g_pVtxBuffStory
		, NULL);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStory->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTex = 0; nCntTex < STORYTEX_MAX; nCntTex++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aStory[nCntTex].fWidth / 2, -g_aStory[nCntTex].fHeight / 2, 0.0f) + g_aStory[nCntTex].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aStory[nCntTex].fWidth / 2, -g_aStory[nCntTex].fHeight / 2, 0.0f) + g_aStory[nCntTex].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aStory[nCntTex].fWidth / 2, g_aStory[nCntTex].fHeight / 2, 0.0f) + g_aStory[nCntTex].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aStory[nCntTex].fWidth / 2, g_aStory[nCntTex].fHeight / 2, 0.0f) + g_aStory[nCntTex].pos;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffStory->Unlock();
}
//=============================================================================
//セレクトの状態情報
//=============================================================================
STORYSTATE GetStstate(void)
{
	return g_aStoryState;
}
//=============================================================================
//セレクトのメニュー情報
//=============================================================================
int GetStoryMenu(void)
{
	return g_aStoryMene;
}