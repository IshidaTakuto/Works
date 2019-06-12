//=============================================================================
//
// エンドロール処理 [EndRoll.cpp]
// Author : ABE YUUTAROU
//
//=============================================================================
#include "endroll.h"
#include "input.h"
#include "fade.h"
#include "game.h" 

#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENDROLL_TEXTURENAME_LOGO_1	"data/TEXTURE/ENDROLL/endroll000.png"			// 原案企画
#define ENDROLL_TEXTURENAME_LOGO_2	"data/TEXTURE/ENDROLL/endroll001.png"			// プログラマー
#define ENDROLL_TEXTURENAME_LOGO_3	"data/TEXTURE/ENDROLL/endroll002.png"			// 演出
#define ENDROLL_TEXTURENAME_LOGO_4	"data/TEXTURE/ENDROLL/endroll003.png"			// マップ
#define ENDROLL_TEXTURENAME_LOGO_5	"data/TEXTURE/ENDROLL/endroll004.png"			// モデル
#define ENDROLL_TEXTURENAME_LOGO_6	"data/TEXTURE/ENDROLL/endroll005.png"			// オブジェクト
#define ENDROLL_TEXTURENAME_LOGO_7	"data/TEXTURE/ENDROLL/endroll006.png"			// モーション
#define ENDROLL_TEXTURENAME_LOGO_8	"data/TEXTURE/ENDROLL/endroll007.png"			// サウンド
#define ENDROLL_TEXTURENAME_LOGO_9	"data/TEXTURE/ENDROLL/endroll008.png"			// プロジェクトリーダー
#define ENDROLL_TEXTURENAME_LOGO_10	"data/TEXTURE/ENDROLL/endroll009.png"			// 制作

#define ENDROLL_TEXTURENAME_BG_1	"data/TEXTURE/ENDROLL/endrollbg000.png"			// 背景[1]
#define ENDROLL_TEXTURENAME_BG_2	"data/TEXTURE/ENDROLL/endrollbg001.png"			// 背景[2]
#define ENDROLL_TEXTURENAME_BG_3	"data/TEXTURE/ENDROLL/endrollbg000.png"			// 背景[3]

// 原案企画-------------------------------------------------------------------------------1
#define ENDROLL_LOGO_POS_X_A	((SCREEN_WIDTH/2) + 300)			// ロゴのX座標_A
#define ENDROLL_LOGO_POS_Y_A	(SCREEN_HEIGHT +100)				// ロゴのY座標_A
#define ENDROLL_LOGO_WIDTH_A	(420)								// ロゴの幅
#define ENDROLL_LOGO_HEIGHT_A	(200)								// ロゴの高さ
//プログラマー----------------------------------------------------------------------------2
#define ENDROLL_LOGO_POS_X_B	((SCREEN_WIDTH/2) + 300)			// ロゴのX座標_B
#define ENDROLL_LOGO_POS_Y_B	(SCREEN_HEIGHT + 600 )				// ロゴのY座標_B
#define ENDROLL_LOGO_WIDTH_B	(400/1.4f)							// ロゴの幅
#define ENDROLL_LOGO_HEIGHT_B	(350/1.5f)							// ロゴの高さ
//演出------------------------------------------------------------------------------------3
#define ENDROLL_LOGO_POS_X_C	((SCREEN_WIDTH/2) + 300)			// ロゴのX座標_C
#define ENDROLL_LOGO_POS_Y_C	(SCREEN_HEIGHT + 900 )				// ロゴのY座標_C
#define ENDROLL_LOGO_WIDTH_C	(400/1.4f)							// ロゴの幅
#define ENDROLL_LOGO_HEIGHT_C	(300/1.5f)							// ロゴの高さ
//マップ---------------------------------------------------------------------------------4
#define ENDROLL_LOGO_POS_X_D	((SCREEN_WIDTH/2) + 300)			// ロゴのX座標_D
#define ENDROLL_LOGO_POS_Y_D	(SCREEN_HEIGHT + 1150 )				// ロゴのY座標_D
#define ENDROLL_LOGO_WIDTH_D	(400/1.4f)							// ロゴの幅
#define ENDROLL_LOGO_HEIGHT_D	(200/1.5f)							// ロゴの高さ
//モデル----------------------------------------------------------------------------------5
#define ENDROLL_LOGO_POS_X_E	((SCREEN_WIDTH/2) + 300)			// ロゴのX座標_E
#define ENDROLL_LOGO_POS_Y_E	(SCREEN_HEIGHT + 1400 )				// ロゴのY座標_E
#define ENDROLL_LOGO_WIDTH_E	(400/1.4f)							// ロゴの幅
#define ENDROLL_LOGO_HEIGHT_E	(300/1.5f)							// ロゴの高さ
//オブジェクト----------------------------------------------------------------------------6
#define ENDROLL_LOGO_POS_X_F	((SCREEN_WIDTH/2) + 300)			// ロゴのX座標_F
#define ENDROLL_LOGO_POS_Y_F	(SCREEN_HEIGHT + 1700 )				// ロゴのY座標_F
#define ENDROLL_LOGO_WIDTH_F	(400/1.4f)							// ロゴの幅
#define ENDROLL_LOGO_HEIGHT_F	(300/1.5f)							// ロゴの高さ
//モーション------------------------------------------------------------------------------7
#define ENDROLL_LOGO_POS_X_G	((SCREEN_WIDTH/2) + 300)			// ロゴのX座標_G
#define ENDROLL_LOGO_POS_Y_G	(SCREEN_HEIGHT + 2000 )				// ロゴのY座標_G
#define ENDROLL_LOGO_WIDTH_G	(400/1.4f)							// ロゴの幅
#define ENDROLL_LOGO_HEIGHT_G	(300/1.5f)							// ロゴの高さ
//サウンド--------------------------------------------------------------------------------8
#define ENDROLL_LOGO_POS_X_H	((SCREEN_WIDTH/2) + 300)			// ロゴのX座標_H
#define ENDROLL_LOGO_POS_Y_H	(SCREEN_HEIGHT + 2600 )				// ロゴのY座標_H
#define ENDROLL_LOGO_WIDTH_H	(400/1.4f)							// ロゴの幅
#define ENDROLL_LOGO_HEIGHT_H	(600)								// ロゴの高さ
//プロジェクトリーダー-------------------------------------------------------------------9
#define ENDROLL_LOGO_POS_X_I	((SCREEN_WIDTH/2) + 300)			// ロゴのX座標_I
#define ENDROLL_LOGO_POS_Y_I	(SCREEN_HEIGHT + 3100 )				// ロゴのY座標_I
#define ENDROLL_LOGO_WIDTH_I	(400/1.2f)							// ロゴの幅
#define ENDROLL_LOGO_HEIGHT_I	(200/1.5f)							// ロゴの高さ
//制作-----------------------------------------------------------------------------------10
#define ENDROLL_LOGO_POS_X_J	((SCREEN_WIDTH/2) )					// ロゴのX座標_J
#define ENDROLL_LOGO_POS_Y_J	(SCREEN_HEIGHT + 3300 )				// ロゴのY座標_J
#define ENDROLL_LOGO_WIDTH_J	(500)								// ロゴの幅
#define ENDROLL_LOGO_HEIGHT_J	(200)								// ロゴの高さ
//背景[1]-----------------------------------------------------------------------------------1
#define ENDROLL_BG_POS_X_A	((SCREEN_WIDTH/2/2)+50)					// ロゴのX座標_J
#define ENDROLL_BG_POS_Y_A	((SCREEN_HEIGHT/2/2)+50 )				// ロゴのY座標_J
#define ENDROLL_BG_WIDTH_A	(700)									// ロゴの幅
#define ENDROLL_BG_HEIGHT_A	(400)									// ロゴの高さ
//背景[2]-----------------------------------------------------------------------------------2
#define ENDROLL_BG_POS_X_B	((SCREEN_WIDTH/2/2)+50)					// ロゴのX座標_J
#define ENDROLL_BG_POS_Y_B	((SCREEN_HEIGHT/2/2)+50 )				// ロゴのY座標_J
#define ENDROLL_BG_WIDTH_B	(700)									// ロゴの幅
#define ENDROLL_BG_HEIGHT_B	(400)									// ロゴの高さ
//背景[3]-----------------------------------------------------------------------------------3
#define ENDROLL_BG_POS_X_C	((SCREEN_WIDTH/2/2)+50)					// ロゴのX座標_J
#define ENDROLL_BG_POS_Y_C	((SCREEN_HEIGHT/2/2)+50 )				// ロゴのY座標_J
#define ENDROLL_BG_WIDTH_C	(700)									// ロゴの幅
#define ENDROLL_BG_HEIGHT_C	(400)									// ロゴの高さ

//ロゴ動き
#define ENDROLL_MOVE_NOMAL	(2.0f)									// ロゴの動き
#define ENDROLL_MOVE_ACCELERATA	(5.0f)								// ロゴの加速

//BGの数
#define ENDROLL_BG_MAX (3)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{// テクスチャの種類
	EndRollTEX_LOGO_A = 0,			// ロゴ[原案企画]
	EndRollTEX_LOGO_B,				// ロゴ[プログラマー]
	EndRollTEX_LOGO_C,				// ロゴ[演出]
	EndRollTEX_LOGO_D,				// ロゴ[マップ]
	EndRollTEX_LOGO_E,				// ロゴ[モデル]
	EndRollTEX_LOGO_F,				// ロゴ[オブジェクト]
	EndRollTEX_LOGO_G,				// ロゴ[モーション]
	EndRollTEX_LOGO_H,				// ロゴ[サウンド]
	EndRollTEX_LOGO_I,				// ロゴ[プロジェクトリーダー]
	EndRollTEX_LOGO_J,				// ロゴ[制作]
	EndRollTEX_BG_A,				//背景[1]
	EndRollTEX_BG_B,				//背景[2]
	EndRollTEX_BG_C,				//背景[2]
	EndRollTEX_MAX					// テクスチャの総数
} ENDROLLTEX;

typedef struct
{// タイトルの情報
	D3DXVECTOR3 pos;				// 位置
	float fWidth;					// 幅
	float fHeight;					// 高さ
	bool bDraw;						// 描画するかどうか
} ENDROLL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexEndRoll(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEndRoll[EndRollTEX_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffEndRoll = NULL;					// 頂点バッファへのポインタ
ENDROLL					g_aEndRoll[EndRollTEX_MAX];					// タイトルの情報[ロゴ]
int						g_nCntFpsEndRoll;							// 経過時間
int						g_nLogoStopEndRoll;							// 経過時間

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitEndRoll(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 文字ロゴ情報初期化
	for (int nCntTex = 0; nCntTex < EndRollTEX_MAX; nCntTex++)
	{
		switch (nCntTex)
		{
		case EndRollTEX_LOGO_A://原案企画
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_LOGO_POS_X_A, ENDROLL_LOGO_POS_Y_A, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_LOGO_WIDTH_A;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_LOGO_HEIGHT_A;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_LOGO_B://プログラマー
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_LOGO_POS_X_B, ENDROLL_LOGO_POS_Y_B, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_LOGO_WIDTH_B;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_LOGO_HEIGHT_B;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_LOGO_C://演出
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_LOGO_POS_X_C, ENDROLL_LOGO_POS_Y_C, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_LOGO_WIDTH_C;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_LOGO_HEIGHT_C;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_LOGO_D://マップ
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_LOGO_POS_X_D, ENDROLL_LOGO_POS_Y_D, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_LOGO_WIDTH_D;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_LOGO_HEIGHT_D;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_LOGO_E://モデル
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_LOGO_POS_X_E, ENDROLL_LOGO_POS_Y_E, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_LOGO_WIDTH_E;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_LOGO_HEIGHT_E;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_LOGO_F://オブジェクト
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_LOGO_POS_X_F, ENDROLL_LOGO_POS_Y_F, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_LOGO_WIDTH_F;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_LOGO_HEIGHT_F;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_LOGO_G://モーション
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_LOGO_POS_X_G, ENDROLL_LOGO_POS_Y_G, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_LOGO_WIDTH_G;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_LOGO_HEIGHT_G;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_LOGO_H://サウンド
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_LOGO_POS_X_H, ENDROLL_LOGO_POS_Y_H, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_LOGO_WIDTH_H;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_LOGO_HEIGHT_H;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_LOGO_I://プロジェクトリーダー]
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_LOGO_POS_X_I, ENDROLL_LOGO_POS_Y_I, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_LOGO_WIDTH_I;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_LOGO_HEIGHT_I;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_LOGO_J://制作
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_LOGO_POS_X_J, ENDROLL_LOGO_POS_Y_J, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_LOGO_WIDTH_J;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_LOGO_HEIGHT_J;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_BG_A:	//背景1
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_BG_POS_X_A, ENDROLL_BG_POS_Y_A, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_BG_WIDTH_A;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_BG_HEIGHT_A;
			g_aEndRoll[nCntTex].bDraw = true;
			break;
		case EndRollTEX_BG_B:	//背景2
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_BG_POS_X_B, ENDROLL_BG_POS_Y_B, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_BG_WIDTH_B;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_BG_HEIGHT_B;
			g_aEndRoll[nCntTex].bDraw = false;
			break;
		case EndRollTEX_BG_C:	//背景2
			g_aEndRoll[nCntTex].pos = D3DXVECTOR3(ENDROLL_BG_POS_X_B, ENDROLL_BG_POS_Y_B, 0.0f);
			g_aEndRoll[nCntTex].fWidth = ENDROLL_BG_WIDTH_B;
			g_aEndRoll[nCntTex].fHeight = ENDROLL_BG_HEIGHT_B;
			g_aEndRoll[nCntTex].bDraw = false;
			break;
		}
	}
	//変数の初期化
	g_nCntFpsEndRoll = 0;
	g_nLogoStopEndRoll = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_LOGO_1, &g_apTextureEndRoll[EndRollTEX_LOGO_A]);//原案企画
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_LOGO_2, &g_apTextureEndRoll[EndRollTEX_LOGO_B]);//プログラマー
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_LOGO_3, &g_apTextureEndRoll[EndRollTEX_LOGO_C]);//演出
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_LOGO_4, &g_apTextureEndRoll[EndRollTEX_LOGO_D]);//マップ
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_LOGO_5, &g_apTextureEndRoll[EndRollTEX_LOGO_E]);//モデル
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_LOGO_6, &g_apTextureEndRoll[EndRollTEX_LOGO_F]);//オブジェクト
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_LOGO_7, &g_apTextureEndRoll[EndRollTEX_LOGO_G]);//モーション
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_LOGO_8, &g_apTextureEndRoll[EndRollTEX_LOGO_H]);//サウンド
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_LOGO_9, &g_apTextureEndRoll[EndRollTEX_LOGO_I]);//プロジェクトリーダー]
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_LOGO_10, &g_apTextureEndRoll[EndRollTEX_LOGO_J]);//制作
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_BG_1, &g_apTextureEndRoll[EndRollTEX_BG_A]);		//背景[1]	
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_BG_2, &g_apTextureEndRoll[EndRollTEX_BG_B]);		//背景[2]	
	D3DXCreateTextureFromFile(pDevice, ENDROLL_TEXTURENAME_BG_3, &g_apTextureEndRoll[EndRollTEX_BG_C]);		//背景[3]	

	// 頂点情報の作成
	MakeVertexEndRoll(pDevice);
	// カメラの初期化処理
	InitCamera();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitEndRoll(void)
{
	// 各種オブジェクトの終了処理
	UninitCamera();

	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < EndRollTEX_MAX; nCntTex++)
	{
		if (g_apTextureEndRoll[nCntTex] != NULL)
		{
			g_apTextureEndRoll[nCntTex]->Release();
			g_apTextureEndRoll[nCntTex] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffEndRoll != NULL)
	{
		g_pVtxBuffEndRoll->Release();
		g_pVtxBuffEndRoll = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateEndRoll(void)
{
	FADE pFade = *GetFade(); // フェードの取得
	VERTEX_2D *pVtx;
	PLAYER *pPlayer = GetPlayer();
	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEndRoll->Lock(0, 0, (void**)&pVtx, 0);

	if (pFade == FADE_NONE)
	{
		for (int nCntTex = 0; nCntTex < EndRollTEX_MAX - ENDROLL_BG_MAX; nCntTex++, pVtx += 4)
		{
			if (pPlayer[0].bButton[BUTTON_P] == true)
			{//エンターで加速
				g_aEndRoll[nCntTex].pos.y -= ENDROLL_MOVE_ACCELERATA;
			}
			else
			{
				g_aEndRoll[nCntTex].pos.y -= ENDROLL_MOVE_NOMAL;
			}
			if (g_aEndRoll[nCntTex].pos.y <= -100)
			{	//Y-100で消す
				g_aEndRoll[nCntTex].bDraw = false;
			}
			if (g_aEndRoll[EndRollTEX_LOGO_J].pos.y <= SCREEN_HEIGHT / 2)
			{//最後の制作は真ん中で止める
				g_aEndRoll[EndRollTEX_LOGO_J].pos.y = SCREEN_HEIGHT / 2;
				g_nLogoStopEndRoll++;
				if (g_nLogoStopEndRoll >= 1200 && g_aEndRoll[EndRollTEX_LOGO_J].pos.y == SCREEN_HEIGHT / 2)
				{
					SetFade(MODE_RESULT, 0, 0.0f, 0);
				}
			}

			if (g_aEndRoll[EndRollTEX_LOGO_A].bDraw == false)		//企画がFALSEになったら
			{
				g_aEndRoll[EndRollTEX_BG_A].bDraw = false;			//背景[1]をFALSEにする
				if (g_aEndRoll[EndRollTEX_BG_A].bDraw == false)		//背景[1]をFALSEになったら
				{
					g_aEndRoll[EndRollTEX_BG_B].bDraw = true;		//背景[2]をTRUEにする
				}
			}
			if (g_aEndRoll[EndRollTEX_LOGO_C].bDraw == false)		//企画がFALSEになったら
			{
				g_aEndRoll[EndRollTEX_BG_B].bDraw = false;
				if (g_aEndRoll[EndRollTEX_BG_B].bDraw == false)
				{
					g_aEndRoll[EndRollTEX_BG_C].bDraw = true;
				}
			}
			if (g_aEndRoll[EndRollTEX_LOGO_H].bDraw == false)		//サウンドがFALSEになったら
			{
				g_aEndRoll[EndRollTEX_BG_C].bDraw = false;			//背景[3をFALSE]
			}
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEndRoll[nCntTex].fWidth / 2, -g_aEndRoll[nCntTex].fHeight / 2, 0.0f) + g_aEndRoll[nCntTex].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aEndRoll[nCntTex].fWidth / 2, -g_aEndRoll[nCntTex].fHeight / 2, 0.0f) + g_aEndRoll[nCntTex].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aEndRoll[nCntTex].fWidth / 2, g_aEndRoll[nCntTex].fHeight / 2, 0.0f) + g_aEndRoll[nCntTex].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aEndRoll[nCntTex].fWidth / 2, g_aEndRoll[nCntTex].fHeight / 2, 0.0f) + g_aEndRoll[nCntTex].pos;
		}

		// 頂点バッファをアンロックする
		g_pVtxBuffEndRoll->Unlock();
	}
}


//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawEndRoll(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// カメラの設定
	SetCamera(0);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEndRoll, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTex = 0; nCntTex < EndRollTEX_MAX; nCntTex++)
	{
		if (g_aEndRoll[nCntTex].bDraw == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEndRoll[nCntTex]);

			// ポリゴンの描画// プリミティブの種類// 開始する頂点のインデックス// 描画するプリミティブの数
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexEndRoll(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * EndRollTEX_MAX
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_2D
		, D3DPOOL_MANAGED
		, &g_pVtxBuffEndRoll
		, NULL);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEndRoll->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTex = 0; nCntTex < EndRollTEX_MAX ; nCntTex++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aEndRoll[nCntTex].fWidth / 2, -g_aEndRoll[nCntTex].fHeight / 2, 0.0f) + g_aEndRoll[nCntTex].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aEndRoll[nCntTex].fWidth / 2, -g_aEndRoll[nCntTex].fHeight / 2, 0.0f) + g_aEndRoll[nCntTex].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aEndRoll[nCntTex].fWidth / 2, g_aEndRoll[nCntTex].fHeight / 2, 0.0f) + g_aEndRoll[nCntTex].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aEndRoll[nCntTex].fWidth / 2, g_aEndRoll[nCntTex].fHeight / 2, 0.0f) + g_aEndRoll[nCntTex].pos;

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
	g_pVtxBuffEndRoll->Unlock();
}
