//=============================================================================
//
// セレクト画面処理 [Selectmode.cpp]
// Author : ABE YUUTAROU
//
//=============================================================================
#include "Selectmode.h"
#include "main.h"
#include "game.h"
#include "fade.h"
#include "camera.h"
#include "goburin.h"
#include "light.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define BG_TEXTURENAMRE1 "data/TEXTURE/SELECT/GAKUBUTI.png"				//読み込むテクスチャ
#define BG_TEXTURENAMRE2 "data/TEXTURE/SELECT/STORYMODE.png"				//読み込むテクスチャ
#define BG_TEXTURENAMRE3 "data/TEXTURE/SELECT/GAMEMODE.png"				//読み込むテクスチャ
#define BG_TEXTURENAMRE4 "data/TEXTURE/SELECT/SOUNDMODE.png"				 //読み込むテクスチャ
#define BG_TEXTURENAMRE5 "data/TEXTURE/SELECT/GAMEENDMODE.png"				 //読み込むテクスチャ
#define BG_TEXTURENAMRE6 "data/TEXTURE/SELECT/SELECTMODE.png"				//W
#define BG_TEXTURENAMRE7 "data/TEXTURE/SELECT/SELSOUSA.png"				//S
#define BG_TEXTURENAMRE8  "data/TEXTURE/SELECT/Press000.png"				//読み込むテクスチャ
#define BG_TEXTURENAMRE9  "data/TEXTURE/SELECT/KUROMEN.png"				//読み込むテクスチャ
#define BG_TEXTURENAMRE13  "data/TEXTURE/SELECT/ooo.jpg"					//読み込むテクスチャ
#define SEL_WIDTH   (SCREEN_WIDTH)									//背景の幅
#define SEL_HEIGHT   (SCREEN_HEIGHT)								//背景の高さ
#define MAX_SEL (13)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSelectmode[MAX_SEL] = {};				//テクスチャポイント
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectmode = NULL;				//頂点バッファのポイント
D3DXVECTOR2 g_Selectmode[MAX_SEL];
SELECTSTATE g_aSelstate;
int g_aSelectMenu;
int g_PressEnter;
int g_aPress;
int g_nCounterTitle;
int g_SELTITLE;
//=============================================================================
//初期化処理ポリゴン
//=============================================================================
void InitSelectmode(void)
{
	PLAYER *pPlayer = GetPlayer();
	pPlayer[0].state = JAYPAD_STATE_SELECT;
	int *pTime = GetTime();
	pTime[0] = 0;

	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ
	//デバイス取得
	pDevice = GetDevice();
	g_nCounterTitle = 0;
	g_aSelectMenu = 0;
	g_aSelstate = SELECT_NOMAL;
	g_PressEnter = 0;
	g_aPress = 0;

	g_Selectmode[0] = D3DXVECTOR2(290, 70);//メインゲーム
	g_Selectmode[1] = D3DXVECTOR2(290, 190);
	g_Selectmode[2] = D3DXVECTOR2(290, 310);
	g_Selectmode[3] = D3DXVECTOR2(290, 430);//終了
	g_Selectmode[4] = D3DXVECTOR2(840, 125);//SELECTMODE
	g_Selectmode[5] = D3DXVECTOR2(870, 445);//ボタンのやり方
	g_Selectmode[6] = D3DXVECTOR2(300, 630);
	//g_Selectmode[7] = D3DXVECTOR2(0,0);	//プレスエンター
	
	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAMRE9, &g_pTextureSelectmode[0]);		//説明文黒面
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAMRE2, &g_pTextureSelectmode[4]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAMRE3, &g_pTextureSelectmode[5]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAMRE4, &g_pTextureSelectmode[6]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAMRE5, &g_pTextureSelectmode[7]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAMRE6, &g_pTextureSelectmode[8]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAMRE7, &g_pTextureSelectmode[9]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAMRE8, &g_pTextureSelectmode[10]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAMRE1, &g_pTextureSelectmode[11]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAMRE13, &g_pTextureSelectmode[12]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SEL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectmode,
		NULL);

	VERTEX_2D*pVtx;									//頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);
	
		pVtx[0].pos = D3DXVECTOR3(200.0f, 500.0f, 0.0f);		 //右上
		pVtx[1].pos = D3DXVECTOR3(900.0f, 500.0f, 0.0f);		 //右下
		pVtx[2].pos = D3DXVECTOR3(200.0f, 700.0f, 0.0f);		 //左上
		pVtx[3].pos = D3DXVECTOR3(900.0f, 700.0f, 0.0f);		 //左下
		//RHW
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//右上
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				//右下
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				//左上
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				//左下
		
	//頂点バッファアンロックする
	g_pVtxBuffSelectmode->Unlock();

	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < 4; nCount++)
	{
		//頂点設定
		pVtx[16].pos = D3DXVECTOR3(g_Selectmode[nCount].x - 270.0f, g_Selectmode[nCount].y - 20.0f, 0.0f);
		pVtx[17].pos = D3DXVECTOR3(g_Selectmode[nCount].x + 270.0f, g_Selectmode[nCount].y - 20.0f, 0.0f);
		pVtx[18].pos = D3DXVECTOR3(g_Selectmode[nCount].x - 270.0f, g_Selectmode[nCount].y + 20.0f, 0.0f);
		pVtx[19].pos = D3DXVECTOR3(g_Selectmode[nCount].x + 270.0f, g_Selectmode[nCount].y + 20.0f, 0.0f);
		//RHW
		pVtx[16].rhw = 1.0f;
		pVtx[17].rhw = 1.0f;
		pVtx[18].rhw = 1.0f;
		pVtx[19].rhw = 1.0f;
		
		//カラー
		pVtx[16].col = D3DCOLOR_RGBA(90, 90, 90, 255);
		pVtx[17].col = D3DCOLOR_RGBA(90, 90, 90, 255);
		pVtx[18].col = D3DCOLOR_RGBA(90, 90, 90, 255);
		pVtx[19].col = D3DCOLOR_RGBA(90, 90, 90, 255);

		//テクスチャ座標
		pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);				//右上
		pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);				//右下
		pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);				//左上
		pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);				//左下
		pVtx += 4;	//頂点ポインタを4つ進める
	}
	//頂点バッファアンロックする
	g_pVtxBuffSelectmode->Unlock();

	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

		//頂点設定
		pVtx[32].pos = D3DXVECTOR3(g_Selectmode[4].x - 130.0f, g_Selectmode[4].y - 100.0f, 0.0f);
		pVtx[33].pos = D3DXVECTOR3(g_Selectmode[4].x + 400.0f, g_Selectmode[4].y - 100.0f, 0.0f);
		pVtx[34].pos = D3DXVECTOR3(g_Selectmode[4].x - 130.0f, g_Selectmode[4].y + 100.0f, 0.0f);
		pVtx[35].pos = D3DXVECTOR3(g_Selectmode[4].x + 400.0f, g_Selectmode[4].y + 100.0f, 0.0f);
		//RHW
		pVtx[32].rhw = 1.0f;
		pVtx[33].rhw = 1.0f;
		pVtx[34].rhw = 1.0f;
		pVtx[35].rhw = 1.0f;
		//カラー
		pVtx[32].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[33].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[34].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[35].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標
		pVtx[32].tex = D3DXVECTOR2(0.0f, 0.0f);				//右上
		pVtx[33].tex = D3DXVECTOR2(1.0f, 0.0f);				//右下
		pVtx[34].tex = D3DXVECTOR2(0.0f, 1.0f);				//左上
		pVtx[35].tex = D3DXVECTOR2(1.0f, 1.0f);				//左下

		//頂点バッファアンロックする
		g_pVtxBuffSelectmode->Unlock();

		//頂点バッファをロックし、頂点データへのポインタ
		g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4;	//頂点ポインタを4つ進める

		//頂点設定
		pVtx[32].pos = D3DXVECTOR3(g_Selectmode[5].x - 130.0f, g_Selectmode[5].y - 21.0f, 0.0f);
		pVtx[33].pos = D3DXVECTOR3(g_Selectmode[5].x + 400.0f, g_Selectmode[5].y - 21.0f, 0.0f);
		pVtx[34].pos = D3DXVECTOR3(g_Selectmode[5].x - 130.0f, g_Selectmode[5].y + 21.0f, 0.0f);
		pVtx[35].pos = D3DXVECTOR3(g_Selectmode[5].x + 400.0f, g_Selectmode[5].y + 21.0f, 0.0f);
		//RHW
		pVtx[32].rhw = 1.0f;
		pVtx[33].rhw = 1.0f;
		pVtx[34].rhw = 1.0f;
		pVtx[35].rhw = 1.0f;
		//カラー
		pVtx[32].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[33].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[34].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[35].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標
		pVtx[32].tex = D3DXVECTOR2(0.0f, 0.0f);				//右上
		pVtx[33].tex = D3DXVECTOR2(1.0f, 0.0f);				//右下
		pVtx[34].tex = D3DXVECTOR2(0.0f, 1.0f);				//左上
		pVtx[35].tex = D3DXVECTOR2(1.0f, 1.0f);				//左下

	//頂点バッファアンロックする
	g_pVtxBuffSelectmode->Unlock();

	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);
	//頂点設定
	pVtx[40].pos = D3DXVECTOR3(g_Selectmode[6].x - 100.0f, g_Selectmode[6].y - 30.0f, 0.0f);
	pVtx[41].pos = D3DXVECTOR3(g_Selectmode[6].x + 100.0f, g_Selectmode[6].y - 30.0f, 0.0f);
	pVtx[42].pos = D3DXVECTOR3(g_Selectmode[6].x - 100.0f, g_Selectmode[6].y + 30.0f, 0.0f);
	pVtx[43].pos = D3DXVECTOR3(g_Selectmode[6].x + 100.0f, g_Selectmode[6].y + 30.0f, 0.0f);
	//RHW
	pVtx[40].rhw = 1.0f;
	pVtx[41].rhw = 1.0f;
	pVtx[42].rhw = 1.0f;
	pVtx[43].rhw = 1.0f;
	//カラー
	pVtx[40].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[41].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[42].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[43].col = D3DCOLOR_RGBA(255, 0, 0, 255);

	//テクスチャ座標
	pVtx[40].tex = D3DXVECTOR2(0.0f, 0.0f);					//右上
	pVtx[41].tex = D3DXVECTOR2(1.0f, 0.0f);					//右下
	pVtx[42].tex = D3DXVECTOR2(0.0f, 1.0f);					//左上
	pVtx[43].tex = D3DXVECTOR2(1.0f, 1.0f);					//左下
	//頂点バッファアンロックする
	g_pVtxBuffSelectmode->Unlock();

	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[44].pos = D3DXVECTOR3(165.0f, 490.0f, 0.0f);		 //右上
	pVtx[45].pos = D3DXVECTOR3(935.0f, 490.0f, 0.0f);		 //右下
	pVtx[46].pos = D3DXVECTOR3(165.0f, 715.0f, 0.0f);		 //左上
	pVtx[47].pos = D3DXVECTOR3(935.0f, 715.0f, 0.0f);		 //左下
	//RHW
	pVtx[44].rhw = 1.0f;
	pVtx[45].rhw = 1.0f;
	pVtx[46].rhw = 1.0f;
	pVtx[47].rhw = 1.0f;

	//カラー
	pVtx[44].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[45].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[46].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[47].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標
	pVtx[44].tex = D3DXVECTOR2(0.0f, 0.0f);				//右上
	pVtx[45].tex = D3DXVECTOR2(1.0f, 0.0f);				//右下
	pVtx[46].tex = D3DXVECTOR2(0.0f, 1.0f);				//左上
	pVtx[47].tex = D3DXVECTOR2(1.0f, 1.0f);				//左下

	//頂点バッファアンロックする
	g_pVtxBuffSelectmode->Unlock();

	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[48].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						 //右上
	pVtx[49].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);				 //右下
	pVtx[50].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);				 //左上
	pVtx[51].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);		 //左下

	//RHW
	pVtx[48].rhw = 1.0f;
	pVtx[49].rhw = 1.0f;
	pVtx[50].rhw = 1.0f;
	pVtx[51].rhw = 1.0f;

	//カラー
	pVtx[48].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[49].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[50].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[51].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標
	pVtx[48].tex = D3DXVECTOR2(0.0f, 0.0f);				//右上
	pVtx[49].tex = D3DXVECTOR2(1.0f, 0.0f);				//右下
	pVtx[50].tex = D3DXVECTOR2(0.0f, 1.0f);				//左上
	pVtx[51].tex = D3DXVECTOR2(1.0f, 1.0f);				//左下

	//頂点バッファアンロックする
	g_pVtxBuffSelectmode->Unlock();

	//ライトの処理
	InitLight();
	//カメラの初期化
	InitCamera();

}
//=============================================================================
//終了処理更新処理ポリゴン
//=============================================================================
void UninitSelectmode(void)
{
	UninitLight();
	// カメラの初期化処理
	UninitCamera();
	for (int nCount = 0; nCount < MAX_SEL; nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureSelectmode[nCount] != NULL)
		{
			g_pTextureSelectmode[nCount]->Release();
			g_pTextureSelectmode[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSelectmode != NULL)
	{
		g_pVtxBuffSelectmode->Release();
		g_pVtxBuffSelectmode = NULL;
	}

}
//=============================================================================
//更新処理ポリゴン
//=============================================================================
void UpdateSelectmode(void)
{
	VERTEX_2D*pVtx;									//頂点情報のポインタ
	PLAYER *pPlayer = GetPlayer();
	int *pTime = GetTime();

	FADE pFade;
	pFade = *GetFade();

	g_SELTITLE++;
	g_PressEnter++;
	g_nCounterTitle++;
	

	if (g_SELTITLE >= 120)
	{
		g_SELTITLE = 0;
	}
	
	if (g_PressEnter >= 60)
	{//プレスエンター点滅
		g_PressEnter = 0;
	}

	//エンターでセレクト画面へ
	if (pFade == FADE_NONE && pPlayer[0].bButton[BUTTON_ENTER] == true)
	{//エンター押したらセレクトエンドへ
		g_aSelstate = SELECT_END;
		PlaySound(SOUND_LABEL_ENTER);
	}

	if ( pPlayer[0].bButton[BUTTON_S] == true)
	{
		PlaySound(SOUND_LABEL_SELECTMOVE);
		g_aSelectMenu = (g_aSelectMenu + 1) % 4;
		pTime[0] = 0;
	}
	if ( pPlayer[0].bButton[BUTTON_W] == true)
	{
		PlaySound(SOUND_LABEL_SELECTMOVE);
		g_aSelectMenu = (g_aSelectMenu + 3) % 4;
		
		pTime[0] = 0;
	}

	if (pFade == FADE_NONE)
	{
		//キーボード設定
		if ( pPlayer[0].bButton[BUTTON_ENTER] == true)
		{//エンターでセレクト画面へ
			switch (g_aSelectMenu)
			{
			case  0://あらすじ
				SetFade(MODE_STORY, 0, 0.0f, 0);
					g_aSelstate = SELECT_STORY;
				break;
			case 1://ゲームモードへ
				SetFade(MODE_START, 2, 0.0f, 0);
				g_aSelstate = SELECT_GAME;
				break;
			case 2://クレジット
				SetFade(MODE_CREDIT, 0, 0.0f, 0);
				break;
			case 3://終了
				g_aSelstate = SELECT_QUIT;
				break;
			}
		}
	}
	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCount = 0; nCount < 4; nCount++)
		{
			//頂点設定
			pVtx[16].pos = D3DXVECTOR3(g_Selectmode[nCount].x - 270.0f, g_Selectmode[nCount].y - 50.0f, 0.0f);
			pVtx[17].pos = D3DXVECTOR3(g_Selectmode[nCount].x + 270.0f, g_Selectmode[nCount].y - 50.0f, 0.0f);
			pVtx[18].pos = D3DXVECTOR3(g_Selectmode[nCount].x - 270.0f, g_Selectmode[nCount].y + 50.0f, 0.0f);
			pVtx[19].pos = D3DXVECTOR3(g_Selectmode[nCount].x + 270.0f, g_Selectmode[nCount].y + 50.0f, 0.0f);
			
			//カラー
			pVtx[16].col = D3DCOLOR_RGBA(90, 90, 90, 255);
			pVtx[17].col = D3DCOLOR_RGBA(90, 90, 90, 255);
			pVtx[18].col = D3DCOLOR_RGBA(90, 90, 90, 255);
			pVtx[19].col = D3DCOLOR_RGBA(90, 90, 90, 255);

			pVtx += 4;	//頂点ポインタを4つ進める
		}
		//頂点バッファアンロックする
		g_pVtxBuffSelectmode->Unlock();

	switch (g_aSelstate)
	{
	case SELECT_NOMAL://通常
	//頂点バッファをロックし、頂点データへのポインタ
		g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);
		
			pVtx += 4 * (g_aSelectMenu + 1);

			//頂点設定
			pVtx[12].pos = D3DXVECTOR3(g_Selectmode[g_aSelectMenu ].x - 270.0f, g_Selectmode[g_aSelectMenu ].y - 55.0f, 0.0f);
			pVtx[13].pos = D3DXVECTOR3(g_Selectmode[g_aSelectMenu ].x + 420.0f, g_Selectmode[g_aSelectMenu ].y - 55.0f, 0.0f);
			pVtx[14].pos = D3DXVECTOR3(g_Selectmode[g_aSelectMenu ].x - 270.0f, g_Selectmode[g_aSelectMenu ].y + 55.0f, 0.0f);
			pVtx[15].pos = D3DXVECTOR3(g_Selectmode[g_aSelectMenu ].x + 420.0f, g_Selectmode[g_aSelectMenu ].y + 55.0f, 0.0f);
																																						
			//カラー
			pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		
		//頂点バッファアンロックするd
		g_pVtxBuffSelectmode->Unlock();
		break;

	case SELECT_END://選択を点滅
		g_aPress++;
		//頂点バッファをロックし、頂点データへのポインタ
		g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * (g_aSelectMenu + 1);
		pVtx[12].col = D3DCOLOR_RGBA((g_aPress * 250) * 5, (g_aPress * 250) * 5, (g_aPress * 250) * 5, (g_aPress * 250) * 5);
		pVtx[13].col = D3DCOLOR_RGBA((g_aPress * 250) * 5, (g_aPress * 250) * 5, (g_aPress * 250) * 5, (g_aPress * 250) * 5);
		pVtx[14].col = D3DCOLOR_RGBA((g_aPress * 250) * 5, (g_aPress * 250) * 5, (g_aPress * 250) * 5, (g_aPress * 250) * 5);
		pVtx[15].col = D3DCOLOR_RGBA((g_aPress * 250) * 5, (g_aPress * 250) * 5, (g_aPress * 250) * 5, (g_aPress * 250) * 5);

		//頂点バッファアンロックする
		g_pVtxBuffSelectmode->Unlock();
		break;
	}

	UpdateLight();
	UpdateCamera();


	pTime[0]++;

	if (pTime[0] == 900)
	{
		SetFade(MODE_TITLE, 0, 0.0f, 0);
	}

}
//=============================================================================
//描画処理ポリゴン
//=============================================================================
void DrawSelectmode(void)
{
	
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	//デバイス取得
	pDevice = GetDevice();
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelectmode, 0, sizeof(VERTEX_2D));
	//頂点フォーマット設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テェクスチャの設定
	pDevice->SetTexture(0, g_pTextureSelectmode[12]);
	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 12, 2);

	pDevice->SetTexture(0, g_pTextureSelectmode[0]);
	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 * 4, 2);
		
	for (int nCount = 4; nCount < 9; nCount++)
	{
		//テェクスチャの設定
		pDevice->SetTexture(0, g_pTextureSelectmode[nCount]);
		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
	
	//額縁「11」
	pDevice->SetTexture(0, g_pTextureSelectmode[11]);
	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 11, 2);


}
//=============================================================================
//セレクト終了
//=============================================================================
SELECTSTATE GetSelstate(void)
{
	return g_aSelstate;
}

//=============================================================================
//セレクト終了
//=============================================================================
int GetSelMenu(void)
{
	return g_aSelectMenu;
}

