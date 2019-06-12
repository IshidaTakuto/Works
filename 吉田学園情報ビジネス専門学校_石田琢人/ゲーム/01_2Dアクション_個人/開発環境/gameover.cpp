//=============================================================================
//
// ゲームオーバー処理 [gameover.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "gameover.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAMEOVER_TEXTURENAME0	"DATA/TEXTURE/result_bg001.jpg"		// テクスチャ名
#define GAMEOVER_TEXTURENAME1	"DATA/TEXTURE/result_logo001.png"	// テクスチャ名
#define MAX_GAMEOVER	(2)
#define LOGO_SIZE_X	(500.0f)
#define LOGO_SIZE_Y	(100.0f)
#define GAMEOVER_TIME	(600)	// ゲームオーバー画面の表示時間

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGameover[MAX_GAMEOVER] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameover = NULL;		// 頂点バッファへのポインタ
GAMEOVER g_aGameover[MAX_GAMEOVER];
int g_nCntGameover;

//=============================================================================
// 初期化処理
//=============================================================================
void InitGameover(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 初期化
	g_aGameover[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aGameover[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	g_aGameover[0].nType = 0;
	g_aGameover[1].nType = 1;

	g_nCntGameover = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME0, &g_pTextureGameover[0]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME1, &g_pTextureGameover[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAMEOVER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameover,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGameover->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点の座標
	pVtx[4].pos = D3DXVECTOR3(g_aGameover[1].pos.x - LOGO_SIZE_X, g_aGameover[1].pos.y - LOGO_SIZE_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aGameover[1].pos.x + LOGO_SIZE_X, g_aGameover[1].pos.y - LOGO_SIZE_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aGameover[1].pos.x - LOGO_SIZE_X, g_aGameover[1].pos.y + LOGO_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aGameover[1].pos.x + LOGO_SIZE_X, g_aGameover[1].pos.y + LOGO_SIZE_Y, 0.0f);

	//1.0fで固定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラー
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffGameover->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGameover(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_GAMEOVER; nCntTex++)
	{
		if (g_pTextureGameover[nCntTex] != NULL)
		{
			g_pTextureGameover[nCntTex]->Release();
			g_pTextureGameover[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGameover != NULL)
	{
		g_pVtxBuffGameover->Release();
		g_pVtxBuffGameover = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameover(void)
{
	FADE pFade;

	g_nCntGameover++;

	//フェードの取得
	pFade = *GetFade();
	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);

			//モード選択
			SetFade(MODE_RANKING);
		}
		else if (g_nCntGameover >= GAMEOVER_TIME)
		{
			//モード選択
			SetFade(MODE_RANKING);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGameover(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameover, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGameover = 0; nCntGameover < MAX_GAMEOVER; nCntGameover++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGameover[g_aGameover[nCntGameover].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGameover, 2);
	}
}
