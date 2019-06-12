//=============================================================================
//
// ジェネレータ処理 [generator.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "generator.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GENERATOR_TEXTURENAME	"data/TEXTURE/generator.png"	// 背景のテクスチャ名
#define GENERATOR_SIZE	(50.0f)	// ジェネレータの大きさ
#define MAX_GENERATOR	(7)	// ジェネレータの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGenerator = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGenerator = NULL;	// 頂点バッファへのポインタ
GENERATOR g_aGenerator[MAX_GENERATOR];
int g_nCntGenerator;
int g_nCntpVtx;

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitGenerator(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// ジェネレータの情報を初期化
	for (int nCntGenerator = 0; nCntGenerator < MAX_GENERATOR; nCntGenerator++)
	{
		g_aGenerator[nCntGenerator].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGenerator[nCntGenerator].type = BLOCKTYPE_NORMAL;
		g_aGenerator[nCntGenerator].interval = 0;
		g_aGenerator[nCntGenerator].des = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGenerator[nCntGenerator].fHeight = 0.0f;
		g_aGenerator[nCntGenerator].fWidth = 0.0f;
		g_aGenerator[nCntGenerator].bUse = false;
	}

	g_nCntGenerator = 0;
	g_nCntpVtx = 0;

	// テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, GENERATOR_TEXTURENAME, &g_pTextureGenerator);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GENERATOR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGenerator,
		NULL);

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGenerator->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	for (int nCntGenerator = 0; nCntGenerator < MAX_GENERATOR; nCntGenerator++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x - GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y - GENERATOR_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x + GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y - GENERATOR_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x - GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y + GENERATOR_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x + GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y + GENERATOR_SIZE, 0.0f);

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

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffGenerator->Unlock();
}

//=============================================================================
// ポリゴン終了処理
//=============================================================================
void UninitGenerator(void)
{
	// テクスチャの破棄
	if (g_pTextureGenerator != NULL)
	{
		g_pTextureGenerator->Release();
		g_pTextureGenerator = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffGenerator != NULL)
	{
		g_pVtxBuffGenerator->Release();
		g_pVtxBuffGenerator = NULL;
	}
}

//=============================================================================
// ポリゴン更新処理
//=============================================================================
void UpdateGenerator(void)
{
	int nCntGenerator;
	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	GAMESTATE Gamestate;
	Gamestate = GetGameState();

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGenerator->Lock(0, 0, (void**)&pVtx, 0);

	g_nCntGenerator++;	// ブロックを出す間隔のカウンタ

	for (nCntGenerator = 0; nCntGenerator < MAX_GENERATOR; nCntGenerator++)
	{
		if (Gamestate == GAMESTATE_NORMAL)
		{
			if (g_aGenerator[nCntGenerator].bUse == true)
			{// 使われている
				if (g_nCntGenerator % g_aGenerator[nCntGenerator].interval == 0)
				{// 一個出す
					SetBlock(D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x, g_aGenerator[nCntGenerator].pos.y, 0.0f), g_aGenerator[nCntGenerator].fWidth, g_aGenerator[nCntGenerator].fHeight, g_aGenerator[nCntGenerator].des, g_aGenerator[nCntGenerator].type);
				}
			}
		}

		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x - GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y - GENERATOR_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x + GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y - GENERATOR_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x - GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y + GENERATOR_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x + GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y + GENERATOR_SIZE, 0.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffGenerator->Unlock();
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawGenerator(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGenerator, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGenerator);

	for (int nCntGenerator = 0; nCntGenerator < MAX_GENERATOR; nCntGenerator++)
	{// ポリゴンの描画
		if (g_aGenerator[nCntGenerator].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGenerator, 2);
		}
	}
}

//=============================================================================
// ジェネレータの設定
//=============================================================================
void SetGenerator(D3DXVECTOR3 pos, BLOCKTYPE type, float fWidth, float fHeight, D3DXVECTOR3 des, int interval)
{
	int nCntGenerator;
	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	for (nCntGenerator = 0; nCntGenerator < MAX_GENERATOR; nCntGenerator++)
	{
		if (g_aGenerator[nCntGenerator].bUse == false)
		{
			g_aGenerator[nCntGenerator].pos = pos;
			g_aGenerator[nCntGenerator].type = type;
			g_aGenerator[nCntGenerator].fWidth = fWidth;
			g_aGenerator[nCntGenerator].fHeight = fHeight;
			g_aGenerator[nCntGenerator].des = des;
			g_aGenerator[nCntGenerator].interval = interval;
			g_aGenerator[nCntGenerator].bUse = true;

			// 頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffGenerator->Lock(0, 0, (void**)&pVtx, 0);

			if (g_aGenerator[nCntGenerator].pos.x > SCREEN_WIDTH / 2)
			{// 画面半分より右側
				// テクスチャ座標
				pVtx[0 + g_nCntpVtx].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[1 + g_nCntpVtx].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2 + g_nCntpVtx].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3 + g_nCntpVtx].tex = D3DXVECTOR2(0.0f, 1.0f);
			}
			// 頂点バッファをアンロック
			g_pVtxBuffGenerator->Unlock();

			g_nCntpVtx += 4;
			break;
		}

	}
}