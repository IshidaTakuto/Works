//=============================================================================
//
// タイトル処理 [ENDSTORY.cpp]
// Author : ABE YUUTAROU
//
//=============================================================================
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "endstory.h"
#include "light.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENDSTORY_TEXTURENAME1	"data/TEXTURE/STORY/box_horror_grbl.png"			// 読み込むテクスチャの名前１
#define ENDSTORY_TEXTURENAME2	"data/TEXTURE/STORY/KUROMEN.png"					// 読み込むテクスチャの名前１
//メッセージボックス
#define ENDSTORY_BOX_POS_X	(SCREEN_WIDTH/2)								// ロゴのX座標_A
#define ENDSTORY_BOX_POS_Y	(SCREEN_HEIGHT/2 +250)							// ロゴのY座標_A
#define ENDSTORY_BOX_WIDTH	(900)											// ロゴの幅
#define ENDSTORY_BOX_HEIGHT (200)											// ロゴの高さ
//画面開始の黒幕
#define ENDSTORY_BLACK_POS_X	(SCREEN_WIDTH/2)							//	黒幕のX座標_A
#define ENDSTORY_BLACK_POS_Y	(SCREEN_HEIGHT/2)							//　黒幕のY座標_A
#define ENDSTORY_BLACK_WIDTH	(SCREEN_WIDTH)								// 黒幕の幅
#define ENDSTORY_BLACK_HEIGHT (SCREEN_HEIGHT)								// 黒幕の高さ

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{// テクスチャの種類
	ENDSTORYTEX_BOX = 0,	// ロゴ
	ENDSTORYTEX_BLACK,		//　画面開始の黒幕
	ENDSTORYTEX_MAX		// テクスチャの総数
} ENDSTORYTEX;

typedef struct
{// タイトルの情報
	D3DXVECTOR3 pos;	// 位置
	float fWidth;		// 幅
	float fHeight;		// 高さ
	bool bFlash;		// 点滅するかどうか
	bool bDraw;			// 描画するかどうか
} ENDSTORY;

typedef enum
{// テクスチャの種類
	ENDSTORYTEX_BLACK_NOMAL = 0,	// 通常
	ENDSTORYTEX_BLACK_DOWN,
	ENDSTORYTEX_BLACK_QUIT,		//　カラーダウン
	ENDSTORYTEX_BLACK_MAX				// テクスチャの総数
} BLACKTEX;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexEndStory(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureENDSTORY[ENDSTORYTEX_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffENDSTORY = NULL;					// 頂点バッファへのポインタ
ENDSTORY				g_aENDSTORY[ENDSTORYTEX_MAX];					// タイトルの情報
ENDSTORYSTATE			g_aENDSTORYState;							//ストーリー会話状態
BLACKTEX				g_eBlackstate;
int						 g_aENDSTORYMene;;							//ストーリーのメニュー
int						g_nCntFpsENDSTORY;							// 経過時間
int						g_nCntLogoStopENDSTORY;					// 経過時間
int						g_eBlackColDown;						// 黒幕のカラーダウン
int						g_aENDStoryCounterTXT;

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitEndStory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 情報の初期化
	for (int nCntTex = 0; nCntTex < ENDSTORYTEX_MAX; nCntTex++)
	{
		g_aENDSTORY[nCntTex].bFlash = false;
		switch (nCntTex)
		{
		case ENDSTORYTEX_BOX:			//メッセージボックス
			g_aENDSTORY[nCntTex].pos = D3DXVECTOR3(ENDSTORY_BOX_POS_X, ENDSTORY_BOX_POS_Y, 0.0f);
			g_aENDSTORY[nCntTex].fWidth = ENDSTORY_BOX_WIDTH;
			g_aENDSTORY[nCntTex].fHeight = ENDSTORY_BOX_HEIGHT;
			g_aENDSTORY[nCntTex].bDraw = true;
			break;
		case ENDSTORYTEX_BLACK:		//画面開始の黒幕
			g_aENDSTORY[nCntTex].pos = D3DXVECTOR3(ENDSTORY_BLACK_POS_X, ENDSTORY_BLACK_POS_Y, 0.0f);
			g_aENDSTORY[nCntTex].fWidth = ENDSTORY_BLACK_WIDTH;
			g_aENDSTORY[nCntTex].fHeight = ENDSTORY_BLACK_HEIGHT;
			g_aENDSTORY[nCntTex].bDraw = true;
			break;
		}
	}
	//変数の初期化
	g_nCntFpsENDSTORY = 0;
	g_aENDSTORYMene = 0;
	g_nCntLogoStopENDSTORY = 0;
	g_eBlackColDown = 0;
	g_eBlackstate = ENDSTORYTEX_BLACK_NOMAL;
	g_aENDStoryCounterTXT = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ENDSTORY_TEXTURENAME1, &g_apTextureENDSTORY[ENDSTORYTEX_BOX]);
	D3DXCreateTextureFromFile(pDevice, ENDSTORY_TEXTURENAME2, &g_apTextureENDSTORY[ENDSTORYTEX_BLACK]);

	// 頂点情報の作成
	MakeVertexEndStory(pDevice);

	//ライトの初期化処理
	InitLight();
	//カメラの初期化処理
	InitCamera();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitEndStory(void)
{
	//ライトの終了処理
	UninitLight();
	// カメラの終了処理
	UninitCamera();

	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < ENDSTORYTEX_MAX; nCntTex++)
	{
		if (g_apTextureENDSTORY[nCntTex] != NULL)
		{
			g_apTextureENDSTORY[nCntTex]->Release();
			g_apTextureENDSTORY[nCntTex] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffENDSTORY != NULL)
	{
		g_pVtxBuffENDSTORY->Release();
		g_pVtxBuffENDSTORY = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateEndStory(void)
{
	VERTEX_2D *pVtx;
	FADE pFade = *GetFade(); // フェードの取得
	PLAYER *pPlayer = GetPlayer();

	g_aENDStoryCounterTXT = *GetENDStoryTXT();

	if (pFade == FADE_NONE)
	{

		switch (g_eBlackstate)
		{
		case ENDSTORYTEX_BLACK_NOMAL:
			if (pPlayer[0].bButton[BUTTON_ENTER] == true)
			{//エンターでセレクト画面へ
				g_eBlackstate = ENDSTORYTEX_BLACK_DOWN;
			}
			break;
		case ENDSTORYTEX_BLACK_DOWN:
			g_aENDSTORYMene = 1;
			g_eBlackColDown++;
			if (g_eBlackColDown >= 220)
			{
				g_aENDStoryCounterTXT = 200;
				g_eBlackColDown = 255;
				g_eBlackstate = ENDSTORYTEX_BLACK_QUIT;

			}
			break;
		case ENDSTORYTEX_BLACK_QUIT:
			//キーボード設定
			if (pPlayer[0].bButton[BUTTON_ENTER] == true && g_aENDStoryCounterTXT >120)
			{//エンターでセレクト画面へ
				g_aENDSTORYMene += 1;
				PlaySound(SOUND_LABEL_STORY_SE);
			}
			switch (g_aENDSTORYMene)
			{
			case  0:
				g_aENDSTORYState = ENDSTORY_000;
				break;
			case 1:
				g_aENDSTORYState = ENDSTORY_001;
				break;
			case 2:
				g_aENDSTORYState = ENDSTORY_002;
				break;
			case 3:
				g_aENDSTORYState = ENDSTORY_003;
				break;
			case 4:
				g_aENDSTORYState = ENDSTORY_004;
				break;
			case 5:
				g_aENDSTORYState = ENDSTORY_005;
				break;
			case 6:
				g_aENDSTORYState = ENDSTORY_006;
				break;
			case 7:
				g_aENDSTORYState = ENDSTORY_007;
				break;
			case 8:
				g_aENDSTORYState = ENDSTORY_008;
				break;
			case 9:
				g_aENDSTORYState = ENDSTORY_009;
				break;
			case 10:
				g_aENDSTORYState = ENDSTORY_010;
				break;
			case 11:
				g_aENDSTORYState = ENDSTORY_011;
				break;
			case 12:
				g_aENDSTORYState = ENDSTORY_012;
				break;
			case 13:
				g_aENDSTORYState = ENDSTORY_013;
				break;
			case 14:
				SetFade(MODE_ENDROLL, 0, 0.0f, 0);
				g_aENDSTORYState = ENDSTORY_014;
				break;
			}


			break;
		}

		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffENDSTORY->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255 - g_eBlackColDown);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255 - g_eBlackColDown);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255 - g_eBlackColDown);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255 - g_eBlackColDown);

		// 頂点バッファをアンロックする
		g_pVtxBuffENDSTORY->Unlock();
	}

	//ライトの更新処理
	UpdateLight();
	//カメラの更新処理
	UpdateCamera();
}


//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawEndStory(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// カメラの設定
	SetCamera(0);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffENDSTORY, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTex = 0; nCntTex < ENDSTORYTEX_MAX; nCntTex++)
	{
		if (g_aENDSTORY[nCntTex].bDraw == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureENDSTORY[nCntTex]);

			// ポリゴンの描画// プリミティブの種類// 開始する頂点のインデックス// 描画するプリミティブの数
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexEndStory(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * ENDSTORYTEX_MAX
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_2D
		, D3DPOOL_MANAGED
		, &g_pVtxBuffENDSTORY
		, NULL);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffENDSTORY->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTex = 0; nCntTex < ENDSTORYTEX_MAX; nCntTex++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aENDSTORY[nCntTex].fWidth / 2, -g_aENDSTORY[nCntTex].fHeight / 2, 0.0f) + g_aENDSTORY[nCntTex].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aENDSTORY[nCntTex].fWidth / 2, -g_aENDSTORY[nCntTex].fHeight / 2, 0.0f) + g_aENDSTORY[nCntTex].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aENDSTORY[nCntTex].fWidth / 2, g_aENDSTORY[nCntTex].fHeight / 2, 0.0f) + g_aENDSTORY[nCntTex].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aENDSTORY[nCntTex].fWidth / 2, g_aENDSTORY[nCntTex].fHeight / 2, 0.0f) + g_aENDSTORY[nCntTex].pos;

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
	g_pVtxBuffENDSTORY->Unlock();
}
//=============================================================================
//セレクトの状態情報
//=============================================================================
ENDSTORYSTATE GetEndStstate(void)
{
	return g_aENDSTORYState;
}
//=============================================================================
//セレクトのメニュー情報
//=============================================================================
int GetEndStoryMenu(void)
{
	return g_aENDSTORYMene;
}