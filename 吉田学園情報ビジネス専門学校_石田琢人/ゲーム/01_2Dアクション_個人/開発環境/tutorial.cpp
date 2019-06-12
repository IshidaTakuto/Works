//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"
#include "block.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TUTORIAL_TEXTURENAME0	"DATA/TEXTURE/tutorial000.png"	// タイトルのテクスチャ名
#define TUTORIAL_TEXTURENAME1	"DATA/TEXTURE/press_a.png"	// タイトルのテクスチャ名
#define TUTORIAL_TEXTURENAME2	"DATA/TEXTURE/press_d.png"	// タイトルのテクスチャ名
#define TUTORIAL_TEXTURENAME3	"DATA/TEXTURE/press_enter.png"	// タイトルのテクスチャ名
#define MAX_TUTORIAL	(4)
#define LOGO_SIZE_X		(150)
#define LOGO_SIZE_Y		(50)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTutorial[MAX_TUTORIAL] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	// 頂点バッファへのポインタ
TUTORIALSTATE g_State;
TUTORIAL g_aTutorial[MAX_TUTORIAL];
float g_fCntMove;

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitTutorial(void)
{
	// 背景初期化処理
	InitBg();

	// ブロック初期化処理
	InitBlock();

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	// 初期化
	g_aTutorial[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aTutorial[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTutorial[0].nType = 0;
	g_aTutorial[0].nTransparent = 0;

	// a
	g_aTutorial[1].pos = D3DXVECTOR3(LOGO_SIZE_X + 20, SCREEN_HEIGHT - (LOGO_SIZE_Y + 20), 0.0f);
	g_aTutorial[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTutorial[1].nType = 1;
	g_aTutorial[1].nTransparent = 0;

	// d
	g_aTutorial[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (LOGO_SIZE_X + 20), SCREEN_HEIGHT - (LOGO_SIZE_Y + 20), 0.0f);
	g_aTutorial[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTutorial[2].nType = 2;
	g_aTutorial[2].nTransparent = 0;

	// enter
	g_aTutorial[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (LOGO_SIZE_X + 20), SCREEN_HEIGHT - (LOGO_SIZE_Y + 20), 0.0f);
	g_aTutorial[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTutorial[3].nType = 3;
	g_aTutorial[3].nTransparent = 0;

	// チュートリアルの状態
	g_State = TUTORIALSTATE_NORMAL;

	g_fCntMove = 0.0f;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TUTORIAL_TEXTURENAME0, &g_pTextureTutorial[0]);
	D3DXCreateTextureFromFile(pDevice, TUTORIAL_TEXTURENAME1, &g_pTextureTutorial[1]);
	D3DXCreateTextureFromFile(pDevice, TUTORIAL_TEXTURENAME2, &g_pTextureTutorial[2]);
	D3DXCreateTextureFromFile(pDevice, TUTORIAL_TEXTURENAME3, &g_pTextureTutorial[3]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TUTORIAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

	for (int nCntTutorial = 1; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		pVtx += 4;	// 頂点データのポインタを4つ進める

		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x - LOGO_SIZE_X, g_aTutorial[nCntTutorial].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x + LOGO_SIZE_X, g_aTutorial[nCntTutorial].pos.y - LOGO_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x - LOGO_SIZE_X, g_aTutorial[nCntTutorial].pos.y + LOGO_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x + LOGO_SIZE_X, g_aTutorial[nCntTutorial].pos.y + LOGO_SIZE_Y, 0.0f);

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

	}

	// 頂点バッファをアンロック
	g_pVtxBuffTutorial->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitTutorial(void)
{
	// 背景終了処理
	UninitBg();

	// ブロック終了処理
	UninitBlock();

	for (int nCntTexture = 0; nCntTexture < MAX_TUTORIAL; nCntTexture++)
	{
		// テクスチャの破棄
		if (g_pTextureTutorial[nCntTexture] != NULL)
		{
			g_pTextureTutorial[nCntTexture]->Release();
			g_pTextureTutorial[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateTutorial(void)
{
	int nCntTutorial;

	// 背景更新処理
	UpdateBg();

	// ブロック更新処理
	UpdateBlock();

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		if (nCntTutorial != 0)
		{
			if (g_aTutorial[nCntTutorial].col.a <= 1.0f && g_aTutorial[nCntTutorial].nTransparent == 0)
			{//	透明度が1.0fの時
				g_aTutorial[nCntTutorial].col.a -= 0.02f;
				if (g_aTutorial[nCntTutorial].col.a <= 0.0f)
				{// 0.0f以下になったとき
					g_aTutorial[nCntTutorial].col.a = 0.0f;
					g_aTutorial[nCntTutorial].nTransparent = 1;
				}
			}
			else if (g_aTutorial[nCntTutorial].col.a >= 0.0f && g_aTutorial[nCntTutorial].nTransparent == 1)
			{//	透明度が0.0fの時
				g_aTutorial[nCntTutorial].col.a += 0.02f;
				if (g_aTutorial[nCntTutorial].col.a >= 1.0f)
				{// 1.0f以上になったとき
					g_aTutorial[nCntTutorial].col.a = 1.0f;
					g_aTutorial[nCntTutorial].nTransparent = 0;
				}
			}
		}
	}

	switch (g_State)
	{
	case TUTORIALSTATE_NORMAL:
		// enterと左を消す
		g_aTutorial[1].col.a = 0.0f;
		g_aTutorial[3].col.a = 0.0f;
		break;

	case TUTORIALSTATE_MOVE_L:
		// テクスチャ座標を変更
		pVtx[0].tex.x = g_fCntMove;
		pVtx[1].tex.x = g_fCntMove + 0.25f;
		pVtx[2].tex.x = g_fCntMove;
		pVtx[3].tex.x = g_fCntMove + 0.25f;

		break;

	case TUTORIALSTATE_MOVE_R:
		// テクスチャ座標を変更
		pVtx[0].tex.x = g_fCntMove;
		pVtx[1].tex.x = g_fCntMove + 0.25f;
		pVtx[2].tex.x = g_fCntMove;
		pVtx[3].tex.x = g_fCntMove + 0.25f;
		break;

	case TUTORIALSTATE_MIDDLE:
		// enterを消す
		g_aTutorial[3].col.a = 0.0f;
		break;

	case TUTORIALSTATE_END:
		// 右を消す
		g_aTutorial[2].col.a = 0.0f;
		break;
	}

		if (pVtx[0].tex.x <= 0.0f)
		{// ステート変更
			g_State = TUTORIALSTATE_NORMAL;
		}
		else if(pVtx[0].tex.x >= 0.75f)
		{// ステート変更
			g_State = TUTORIALSTATE_END;
		}
		else if(pVtx[0].tex.x == 0.25f || pVtx[0].tex.x == 0.5f)
		{// ステート変更
			g_State = TUTORIALSTATE_MIDDLE;
		}

		if (pVtx[0].tex.x >= 0.0f && pVtx[0].tex.x <= 0.75f)
		{
			if (GetKeyboardTrigger(DIK_A) == true)
			{// ステート変更
				if (g_State != TUTORIALSTATE_NORMAL)
				{
					PlaySound(SOUND_LABEL_SE_TURN);
					g_fCntMove -= 0.25f;
					g_State = TUTORIALSTATE_MOVE_L;
				}
			}
			if (GetKeyboardTrigger(DIK_D) == true)
			{// ステート変更
				if (g_State != TUTORIALSTATE_END)
				{
					PlaySound(SOUND_LABEL_SE_TURN);
					g_fCntMove += 0.25f;
					g_State = TUTORIALSTATE_MOVE_R;
				}
			}
		}


	// 頂点バッファをアンロック
	g_pVtxBuffTutorial->Unlock();

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		// 頂点カラー
		pVtx[0].col = g_aTutorial[nCntTutorial].col;
		pVtx[1].col = g_aTutorial[nCntTutorial].col;
		pVtx[2].col = g_aTutorial[nCntTutorial].col;
		pVtx[3].col = g_aTutorial[nCntTutorial].col;

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffTutorial->Unlock();

	FADE pFade;

	// フェードの取得
	pFade = *GetFade();

	if (g_State == TUTORIALSTATE_END)
	{
		if (pFade == FADE_NONE)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);
				// モード選択
				SetFade(MODE_GAME);
			}
		}
	}

	if (rand() % 740 == 0)
	{// ランダムに鳥を出す
		SetBlock(D3DXVECTOR3(-50, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}
	else if (rand() % 640 == 0)
	{// ランダムに鳥を出す
		SetBlock(D3DXVECTOR3(50 + SCREEN_WIDTH, (rand() % 700) * 1.0f, 0.0f), 50.0f, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_TITLE);
	}
}

//=============================================================================
// チュートリアル描画処理
//=============================================================================
void DrawTutorial(void)
{
	// 背景描画処理
	DrawBg();

	// ブロック描画処理
	DrawBlock();

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[g_aTutorial[nCntTutorial].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTutorial, 2);
	}
}
